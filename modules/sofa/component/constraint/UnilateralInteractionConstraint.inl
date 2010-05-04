/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_CONSTRAINT_UNILATERALINTERACTIONCONSTRAINT_INL
#define SOFA_COMPONENT_CONSTRAINT_UNILATERALINTERACTIONCONSTRAINT_INL

#include <sofa/component/constraint/UnilateralInteractionConstraint.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/gl/template.h>
namespace sofa
{

namespace component
{

namespace constraint
{
	
template<class DataTypes>
void UnilateralInteractionConstraint<DataTypes>::addContact(double mu, Deriv norm, Coord P, Coord Q, Real contactDistance, int m1, int m2, Coord Pfree, Coord Qfree, long id, PersistentID localid)
{
    // compute dt and delta
    Real delta = dot(P-Q, norm) - contactDistance; 
    Real deltaFree = dot(Pfree-Qfree, norm) - contactDistance;
    Real dt;
    int i = contacts.size();
    contacts.resize(i+1);
    Contact& c = contacts[i];

	//sout<<"delta : "<<delta<<" - deltaFree : "<<deltaFree <<sendl;
	//sout<<"P : "<<P<<" - PFree : "<<Pfree <<sendl;
	//sout<<"Q : "<<Q<<" - QFree : "<<Qfree <<sendl;


// for visu
    c.P = P;
    c.Q = Q;
    c.Pfree = Pfree;
    c.Qfree = Qfree;
//
    c.m1 = m1;
    c.m2 = m2;
    c.norm = norm;
    c.delta = delta;
    c.t = Deriv(norm.z(), norm.x(), norm.y());
    c.s = cross(norm,c.t);
    c.s = c.s / c.s.norm();
    c.t = cross((-norm), c.s);
    c.mu = mu;
	c.contactId = id;
    c.localId = localid;
	

    if (rabs(delta - deltaFree) > 0.001 * delta)
    {
        dt = delta / (delta - deltaFree);
        if (dt > 0.0 && dt < 1.0  ){
            sofa::defaulttype::Vector3 Qt, Pt;
            Qt = Q*(1-dt) + Qfree*dt;
            Pt = P*(1-dt) + Pfree*dt;
            c.dfree = deltaFree;// dot(Pfree-Pt, c.norm) - dot(Qfree-Qt, c.norm);
            c.dfree_t = dot(Pfree-Pt, c.t) - dot(Qfree-Qt, c.t);
            c.dfree_s = dot(Pfree-Pt, c.s) - dot(Qfree-Qt, c.s);
            //printf("\n ! dt = %f, c.dfree = %f, deltaFree=%f, delta = %f", dt, c.dfree, deltaFree, delta);
        }
        else
        {
            if (deltaFree < 0.0)
            {
                dt=0.0;
                c.dfree = deltaFree; // dot(Pfree-P, c.norm) - dot(Qfree-Q, c.norm);
                //printf("\n dt = %f, c.dfree = %f, deltaFree=%f, delta = %f", dt, c.dfree, deltaFree, delta);
                c.dfree_t = dot(Pfree-P, c.t) - dot(Qfree-Q, c.t);
                c.dfree_s = dot(Pfree-P, c.s) - dot(Qfree-Q, c.s);
            }
            else
            {
                dt=1.0;
                c.dfree = deltaFree;
                c.dfree_t = 0;
                c.dfree_s = 0;
            }
        }
    }
    else
    {
        dt = 0;
        c.dfree = deltaFree;
        c.dfree_t = 0;
        c.dfree_s = 0;
        //printf("\n dt = %f, c.dfree = %f, deltaFree=%f, delta = %f", dt, c.dfree, deltaFree, delta);
    }


	//sout<<"R_nts = ["<<c.norm<<" ; "<<c.t<<" ; "<<c.s<<" ];"<<sendl;
}


template<class DataTypes>
void UnilateralInteractionConstraint<DataTypes>::applyConstraint(unsigned int &contactId)
{
	assert(this->object1);
	assert(this->object2);
	
	if (this->object1 == this->object2)
	{

	VecConst& c1 = *this->object1->getC();

	for (unsigned int i=0; i<contacts.size(); i++)
	{
		Contact& c = contacts[i];

		//mu = c.mu;
		//c.mu = mu;
		c.id = contactId++;

		SparseVecDeriv svd1;

		this->object1->setConstraintId(c.id);
		svd1.add(c.m1, -c.norm);
		svd1.add(c.m2, c.norm);
		c1.push_back(svd1);
		
		if (c.mu > 0.0)
		{
			contactId += 2;
			this->object1->setConstraintId(c.id+1);
			svd1.set(c.m1, -c.t);
			svd1.set(c.m2, c.t);
			c1.push_back(svd1);

			this->object1->setConstraintId(c.id+2);
			svd1.set(c.m1, -c.s);
			svd1.set(c.m2, c.s);
			c1.push_back(svd1);
		}
	}

	}
	else
	{

	VecConst& c1 = *this->object1->getC();
	VecConst& c2 = *this->object2->getC();

	for (unsigned int i=0; i<contacts.size(); i++)
	{
		Contact& c = contacts[i];

		//mu = c.mu;
		//c.mu = mu;
		c.id = contactId++;

		SparseVecDeriv svd1;
		SparseVecDeriv svd2;

		this->object1->setConstraintId(c.id);
		svd1.add(c.m1, -c.norm);
		c1.push_back(svd1);

		this->object2->setConstraintId(c.id);
		svd2.add(c.m2, c.norm);
		c2.push_back(svd2);
		
		if (c.mu > 0.0)
		{
			contactId += 2;
			this->object1->setConstraintId(c.id+1);
			svd1.set(c.m1, -c.t);
			c1.push_back(svd1);

			this->object1->setConstraintId(c.id+2);
			svd1.set(c.m1, -c.s);
			c1.push_back(svd1);

			this->object2->setConstraintId(c.id+1);
			svd2.set(c.m2, c.t);
			c2.push_back(svd2);

			this->object2->setConstraintId(c.id+2);
			svd2.set(c.m2, c.s);
			c2.push_back(svd2);
		}
	}

	}
}

template<class DataTypes>
void UnilateralInteractionConstraint<DataTypes>::getConstraintValue(defaulttype::BaseVector * v, bool freeMotion)
{
	if (!freeMotion)
		sout<<"WARNING Not Implemented for resolution non based on freeMotion"<<sendl;
		
	for (unsigned int i=0; i<contacts.size(); i++)
	{
		Contact& c = contacts[i]; // get each contact detected
		v->set(c.id,c.dfree); 
		if (c.mu > 0.0)
		{
			v->set(c.id+1,c.dfree_t); // dfree_t & dfree_s are added to v to compute the friction 
			v->set(c.id+2,c.dfree_s);
		}
	}
}

template<class DataTypes>
void UnilateralInteractionConstraint<DataTypes>::getConstraintId(long* id, unsigned int &offset)
{
	if (!yetIntegrated)
	{
		for (unsigned int i=0; i<contacts.size(); i++)
		{
			Contact& c = contacts[i];
			id[offset++] = -c.contactId;
		}

		yetIntegrated = true;
	}
	else
	{
		for (unsigned int i=0; i<contacts.size(); i++)
		{
			Contact& c = contacts[i];
			id[offset++] = c.contactId; 
		}
	}
}

template<class DataTypes>
void UnilateralInteractionConstraint<DataTypes>::getConstraintInfo(VecConstraintBlockInfo& blocks, VecPersistentID& ids, VecConstCoord& /*positions*/, VecConstDeriv& directions, VecConstArea& /*areas*/)
{
    if (contacts.empty()) return;
    const bool friction = (contacts[0].mu > 0.0); /// @TODO: can there be both friction-less and friction contacts in the same UnilateralInteractionConstraint ???
    ConstraintBlockInfo info;
    info.parent = this;
    info.const0 = contacts[0].id;
    info.nbLines = friction ? 3 : 1;
    info.hasId = true;
    info.offsetId = ids.size();
    info.hasDirection = true;
    info.offsetDirection = directions.size();
    info.nbGroups = contacts.size();

    for (unsigned int i=0; i<contacts.size(); i++)
    {
        Contact& c = contacts[i];
        ids.push_back( yetIntegrated ? c.contactId : -c.contactId);
        directions.push_back( c.norm );
        if (friction)
        {
            directions.push_back( c.t );
            directions.push_back( c.s );
        }
    }

    yetIntegrated = true;

    blocks.push_back(info);
}


template<class DataTypes>
void UnilateralInteractionConstraint<DataTypes>::draw()
{
	if (!getContext()->getShowInteractionForceFields()) return;

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor4f(1,0,0,1);
	for (unsigned int i=0; i<contacts.size(); i++)
	{
		glLineWidth(1);
		glColor4f(1,0,0,1);
		const Contact& c = contacts[i];
		helper::gl::glVertexT(c.P);
		helper::gl::glVertexT(c.Q);
		glColor4f(1,1,0,1);
		helper::gl::glVertexT(c.P);
		helper::gl::glVertexT(c.P+c.norm*(c.dfree));
		helper::gl::glVertexT(c.Q);
		helper::gl::glVertexT(c.Q-c.norm*(c.dfree));

		/*
		if (c.dfree < 0)
		{
			glLineWidth(5);
			glColor4f(0,1,0,1);
			helper::gl::glVertexT(c.Pfree);
			helper::gl::glVertexT(c.Qfree);
		}
		*/
	}
	glEnd();
}

} // namespace constraint

} // namespace component

} // namespace sofa

#endif
