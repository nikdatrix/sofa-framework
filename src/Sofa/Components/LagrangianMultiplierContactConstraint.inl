#ifndef SOFA_COMPONENTS_LagrangianMultiplierContactConstraint_INL
#define SOFA_COMPONENTS_LagrangianMultiplierContactConstraint_INL

#include "LagrangianMultiplierContactConstraint.h"
#include "Sofa/Core/Constraint.inl"
#include "Sofa/Core/MechanicalObject.inl"
#include "Common/config.h"
#include <assert.h>
#include <GL/gl.h>
#include "GL/template.h"

namespace Sofa
{

namespace Components
{

template<class DataTypes>
void LagrangianMultiplierContactConstraint<DataTypes>::addContact(int m1, int m2, const Deriv& norm, Real dist, Real ks, Real mu_s, Real mu_v)
{
    int i = contacts.size();
    contacts.resize(i+1);
    this->lambda->resize(i+1);
    (*this->lambda->getX())[i] = 0;
    Contact& c = contacts[i];
    c.m1 = m1;
    c.m2 = m2;
    c.norm = norm;
    c.dist = dist;
    c.ks = ks;
    c.mu_s = mu_s;
    c.mu_v = mu_v;
    c.pen = 0;
}

template<class DataTypes>
void LagrangianMultiplierContactConstraint<DataTypes>::addForce()
{
    assert(this->object1);
    assert(this->object2);
    VecDeriv& f1 = *this->object1->getF();
    VecCoord& p1 = *this->object1->getX();
    //VecDeriv& v1 = *this->object1->getV();
    VecDeriv& f2 = *this->object2->getF();
    VecCoord& p2 = *this->object2->getX();
    //VecDeriv& v2 = *this->object2->getV();
    f1.resize(p1.size());
    f2.resize(p2.size());

    LMVecCoord& lambda = *this->lambda->getX();
    LMVecDeriv& flambda = *this->lambda->getF();
    flambda.resize(lambda.size());

    // Create list of active contact
    for (unsigned int i=0; i<contacts.size(); i++)
    {
        Contact& c = contacts[i];
        c.pen = c.pen0 = (p2[c.m2]-p1[c.m1])*c.norm - c.dist;//c.dist - (p1[c.m1]-p2[c.m2])*c.norm;
        lambda[i] = c.pen0;
    }

    // flamdba += C . DOF
    for (unsigned int i=0; i<contacts.size(); i++)
    {
        Contact& c = contacts[i];
        if (lambda[i] > -0.05) continue;
        flambda[i] += p2[c.m2]*c.norm - p1[c.m1]*c.norm - c.dist;
    }

    // f += Ct . lambda
    for (unsigned int i=0; i<contacts.size(); i++)
    {
        Contact& c = contacts[i];
        Real v = lambda[i];
        if (lambda[i] > -0.05) continue;
        f1[c.m1] += c.norm * v;
        f2[c.m2] -= c.norm * v;
    }
}

template<class DataTypes>
void LagrangianMultiplierContactConstraint<DataTypes>::addDForce()
{
    VecDeriv& f1  = *this->object1->getF();
    //VecCoord& p1 = *this->object1->getX();
    VecDeriv& dx1 = *this->object1->getDx();
    VecDeriv& f2  = *this->object2->getF();
    //VecCoord& p2 = *this->object2->getX();
    VecDeriv& dx2 = *this->object2->getDx();
    f1.resize(dx1.size());
    f2.resize(dx2.size());

    LMVecCoord& lambda = *this->lambda->getX();
    LMVecCoord& dlambda = *this->lambda->getDx();
    LMVecDeriv& flambda = *this->lambda->getF();
    flambda.resize(dlambda.size());

    // dflamdba += C . dX
    for (unsigned int i=0; i<contacts.size(); i++)
    {
        Contact& c = contacts[i];
        if (lambda[i] > -0.05) continue;
        flambda[i] += dx2[c.m2]*c.norm - dx1[c.m1]*c.norm;
    }

    // df += Ct . dlambda
    for (unsigned int i=0; i<contacts.size(); i++)
    {
        Contact& c = contacts[i];
        if (lambda[i] > -0.05) continue;
        Real v = dlambda[i];
        f1[c.m1] += c.norm * v;
        f2[c.m2] -= c.norm * v;
    }
}


template<class DataTypes>
void LagrangianMultiplierContactConstraint<DataTypes>::draw()
{
    if (!getContext()->getShowForceFields()) return;
    VecCoord& p1 = *this->object1->getX();
    VecCoord& p2 = *this->object2->getX();
    LMVecCoord& lambda = *this->lambda->getX();
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    for (unsigned int i=0; i<contacts.size(); i++)
    {
        const Contact& c = contacts[i];
        Real d = c.dist - (p2[c.m2]-p1[c.m1])*c.norm;
        if (d > 0)
            glColor4f(1,0,0,1);
        else
            glColor4f(0,1,0,1);
        GL::glVertexT(p1[c.m1]);
        GL::glVertexT(p2[c.m2]);
    }
    glEnd();

    //if (getContext()->getShowNormals())
    {
        glColor4f(1,1,0,1);
        glBegin(GL_LINES);
        for (unsigned int i=0; i<contacts.size(); i++)
        {
            const Contact& c = contacts[i];
            if (c.pen > 0) continue;
            Coord p = p1[c.m1] - c.norm * lambda[i];
            GL::glVertexT(p1[c.m1]);
            GL::glVertexT(p);
            p = p2[c.m2] + c.norm * lambda[i];
            GL::glVertexT(p2[c.m2]);
            GL::glVertexT(p);
        }
        glEnd();
    }
}


} // namespace Components

} // namespace Sofa

#endif
