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
#define SOFA_COMPONENT_CONSTRAINTSET_BILATERALINTERACTIONCONSTRAINT_CPP

#include <sofa/component/constraintset/BilateralInteractionConstraint.inl>

#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/component/container/MechanicalObject.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace constraintset
{

using namespace sofa::defaulttype;
using namespace sofa::helper;

#ifndef SOFA_FLOAT

#ifdef SOFA_DEV
template<>
void BilateralInteractionConstraint<Rigid3dTypes>::getConstraintResolution(std::vector<core::behavior::ConstraintResolution*>& resTab, unsigned int& offset)
{
    unsigned minp=min(m1.getValue().size(),m2.getValue().size());
    for (unsigned pid=0; pid<minp; pid++)
    {
        // 	for(int i=0; i<6; i++)
        // 	resTab[offset++] = new BilateralConstraintResolution();

        resTab[offset] = new BilateralConstraintResolution3Dof();
        offset += 3;
        BilateralConstraintResolution3Dof* temp = new BilateralConstraintResolution3Dof();
        temp->tolerance = 0.0001;	// specific (smaller) tolerance for the rotation
        resTab[offset] = temp;
        offset += 3;
    }
}
#endif

template <>
void BilateralInteractionConstraint<Rigid3dTypes>::buildConstraintMatrix(const core::ConstraintParams* /*cParams*/ /* PARAMS FIRST */, DataMatrixDeriv &c1_d, DataMatrixDeriv &c2_d, unsigned int &constraintId
        , const DataVecCoord &/*x1*/, const DataVecCoord &/*x2*/)
{
    unsigned minp = min(m1.getValue().size(),m2.getValue().size());
    cid.resize(minp);
    for (unsigned pid=0; pid<minp; pid++)
    {
        int tm1 = m1.getValue()[pid];
        int tm2 = m2.getValue()[pid];

        MatrixDeriv &c1 = *c1_d.beginEdit();
        MatrixDeriv &c2 = *c2_d.beginEdit();

        const Vec<3, Real> cx(1,0,0), cy(0,1,0), cz(0,0,1);
        //	const Vec<3, Real> qId = q.toEulerVector();
        const Vec<3, Real> vZero(0,0,0);

        cid[pid] = constraintId;
        constraintId += 6;

        //Apply constraint for position
        MatrixDerivRowIterator c1_it = c1.writeLine(cid[pid]);
        c1_it.addCol(tm1, Deriv(-cx, vZero));

        MatrixDerivRowIterator c2_it = c2.writeLine(cid[pid]);
        c2_it.addCol(tm2, Deriv(cx, vZero));

        c1_it = c1.writeLine(cid[pid] + 1);
        c1_it.setCol(tm1, Deriv(-cy, vZero));

        c2_it = c2.writeLine(cid[pid] + 1);
        c2_it.setCol(tm2, Deriv(cy, vZero));

        c1_it = c1.writeLine(cid[pid] + 2);
        c1_it.setCol(tm1, Deriv(-cz, vZero));

        c2_it = c2.writeLine(cid[pid] + 2);
        c2_it.setCol(tm2, Deriv(cz, vZero));

        //Apply constraint for orientation
        c1_it = c1.writeLine(cid[pid] + 3);
        c1_it.setCol(tm1, Deriv(vZero, -cx));

        c2_it = c2.writeLine(cid[pid] + 3);
        c2_it.setCol(tm2, Deriv(vZero, cx));

        c1_it = c1.writeLine(cid[pid] + 4);
        c1_it.setCol(tm1, Deriv(vZero, -cy));

        c2_it = c2.writeLine(cid[pid] + 4);
        c2_it.setCol(tm2, Deriv(vZero, cy));

        c1_it = c1.writeLine(cid[pid] + 5);
        c1_it.setCol(tm1, Deriv(vZero, -cz));

        c2_it = c2.writeLine(cid[pid] + 5);
        c2_it.setCol(tm2, Deriv(vZero, cz));

        c1_d.endEdit();
        c2_d.endEdit();
    }
}


template <>
void BilateralInteractionConstraint<Rigid3dTypes>::getConstraintViolation(const core::ConstraintParams* /*cParams*/ /* PARAMS FIRST */, defaulttype::BaseVector *v, const DataVecCoord &x1, const DataVecCoord &x2
        , const DataVecDeriv &/*v1*/, const DataVecDeriv &/*v2*/)
{
    unsigned minp = min(m1.getValue().size(),m2.getValue().size());
    dfree.resize(minp);
    for (unsigned pid=0; pid<minp; pid++)
    {
        const Coord dof1 = x1.getValue()[m1.getValue()[pid]];
        const Coord dof2 = x2.getValue()[m2.getValue()[pid]];

        getVCenter(dfree[pid]) = dof2.getCenter() - dof1.getCenter();
        getVOrientation(dfree[pid]) =  dof1.rotate(q.angularDisplacement(dof2.getOrientation() , dof1.getOrientation())) ;

        for (unsigned int i=0 ; i<dfree[pid].size() ; i++)
            v->set(cid[pid]+i, dfree[pid][i]);
    }
}
#endif

#ifndef SOFA_DOUBLE
template <>
void BilateralInteractionConstraint<Rigid3fTypes>::buildConstraintMatrix(const core::ConstraintParams* /*cParams*/ /* PARAMS FIRST */, DataMatrixDeriv &c1_d, DataMatrixDeriv &c2_d, unsigned int &constraintId
        , const DataVecCoord &/*x1*/, const DataVecCoord &/*x2*/)
{
    unsigned minp = min(m1.getValue().size(),m2.getValue().size());
    cid.resize(minp);
    for (unsigned pid=0; pid<minp; pid++)
    {
        int tm1 = m1.getValue()[pid];
        int tm2 = m2.getValue()[pid];

        MatrixDeriv &c1 = *c1_d.beginEdit();
        MatrixDeriv &c2 = *c2_d.beginEdit();

        const Vec<3, Real> cx(1,0,0), cy(0,1,0), cz(0,0,1);
        //	const Vec<3, Real> qId = q.toEulerVector();
        const Vec<3, Real> vZero(0,0,0);

        cid[pid] = constraintId;
        constraintId += 6;

        //Apply constraint for position
        MatrixDerivRowIterator c1_it = c1.writeLine(cid[pid]);
        c1_it.addCol(tm1, Deriv(-cx, vZero));

        MatrixDerivRowIterator c2_it = c2.writeLine(cid[pid]);
        c2_it.addCol(tm2, Deriv(cx, vZero));

        c1_it = c1.writeLine(cid[pid] + 1);
        c1_it.setCol(tm1, Deriv(-cy, vZero));

        c2_it = c2.writeLine(cid[pid] + 1);
        c2_it.setCol(tm2, Deriv(cy, vZero));

        c1_it = c1.writeLine(cid[pid] + 2);
        c1_it.setCol(tm1, Deriv(-cz, vZero));

        c2_it = c2.writeLine(cid[pid] + 2);
        c2_it.setCol(tm2, Deriv(cz, vZero));

        //Apply constraint for orientation
        c1_it = c1.writeLine(cid[pid] + 3);
        c1_it.setCol(tm1, Deriv(vZero, -cx));

        c2_it = c2.writeLine(cid[pid] + 3);
        c2_it.setCol(tm2, Deriv(vZero, cx));

        c1_it = c1.writeLine(cid[pid] + 4);
        c1_it.setCol(tm1, Deriv(vZero, -cy));

        c2_it = c2.writeLine(cid[pid] + 4);
        c2_it.setCol(tm2, Deriv(vZero, cy));

        c1_it = c1.writeLine(cid[pid] + 5);
        c1_it.setCol(tm1, Deriv(vZero, -cz));

        c2_it = c2.writeLine(cid[pid] + 5);
        c2_it.setCol(tm2, Deriv(vZero, cz));

        c1_d.endEdit();
        c2_d.endEdit();
    }
}


template <>
void BilateralInteractionConstraint<Rigid3fTypes>::getConstraintViolation(const core::ConstraintParams* /*cParams*/ /* PARAMS FIRST */, defaulttype::BaseVector *v, const DataVecCoord &x1, const DataVecCoord &x2
        , const DataVecDeriv &/*v1*/, const DataVecDeriv &/*v2*/)
{
    unsigned min = min(m1.getValue().size(),m2.getValue().size());
    dfree.resize(min);
    for (unsigned pid=0; pid<min; pid++)
    {
        Coord dof1 = x1.getValue()[m1.getValue()[pid]];
        Coord dof2 = x2.getValue()[m1.getValue()[pid]];

        getVCenter(dfree[pid]) = dof2.getCenter() - dof1.getCenter();
        getVOrientation(dfree[pid]) =  dof1.rotate(q.angularDisplacement(dof2.getOrientation() , dof1.getOrientation()));

        for (unsigned int i=0 ; i<dfree[pid].size() ; i++)
            v->set(cid[pid]+i, dfree[pid][i]);
    }
}

#ifdef SOFA_DEV
template<>
void BilateralInteractionConstraint<Rigid3fTypes>::getConstraintResolution(std::vector<core::behavior::ConstraintResolution*>& resTab, unsigned int& offset)
{
    unsigned minp=min(m1.getValue().size(),m2.getValue().size());
    for (unsigned pid=0; pid<minp; pid++)
    {
        // 	for(int i=0; i<6; i++)
        // 	resTab[offset++] = new BilateralConstraintResolution();

        resTab[offset] = new BilateralConstraintResolution3Dof();
        offset += 3;
        BilateralConstraintResolution3Dof* temp = new BilateralConstraintResolution3Dof();
        temp->tolerance = 0.0001;	// specific (smaller) tolerance for the rotation
        resTab[offset] = temp;
        offset += 3;
    }
}
#endif

#endif


SOFA_DECL_CLASS(BilateralInteractionConstraint)

int BilateralInteractionConstraintClass = core::RegisterObject("TODO-BilateralInteractionConstraint")
#ifndef SOFA_FLOAT
        .add< BilateralInteractionConstraint<Vec3dTypes> >()
        .add< BilateralInteractionConstraint<Rigid3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< BilateralInteractionConstraint<Vec3fTypes> >()
        .add< BilateralInteractionConstraint<Rigid3fTypes> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class SOFA_COMPONENT_CONSTRAINTSET_API BilateralInteractionConstraint<Vec3dTypes>;
template class SOFA_COMPONENT_CONSTRAINTSET_API BilateralInteractionConstraint<Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_COMPONENT_CONSTRAINTSET_API BilateralInteractionConstraint<Vec3fTypes>;
template class SOFA_COMPONENT_CONSTRAINTSET_API BilateralInteractionConstraint<Rigid3fTypes>;
#endif

} // namespace constraintset

} // namespace component

} // namespace sofa

