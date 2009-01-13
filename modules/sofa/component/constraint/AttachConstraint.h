/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
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
#ifndef SOFA_COMPONENT_CONSTRAINT_ATTACHCONSTRAINT_H
#define SOFA_COMPONENT_CONSTRAINT_ATTACHCONSTRAINT_H

#include <sofa/core/componentmodel/behavior/PairInteractionConstraint.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/componentmodel/topology/BaseMeshTopology.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/defaulttype/BaseMatrix.h>
#include <sofa/defaulttype/BaseVector.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/helper/vector.h>
#include <sofa/component/topology/PointSubset.h>
#include <set>

namespace sofa
{

namespace component
{

namespace constraint
{

using helper::vector;
using core::objectmodel::Data;
using namespace sofa::core::objectmodel;

/// This class can be overridden if needed for additionnal storage within template specializations.
template <class DataTypes>
class AttachConstraintInternalData
{
};

/** Attach given pair of particles, projecting the positions of the second particles to the first ones.
*/
template <class DataTypes>
class AttachConstraint : public core::componentmodel::behavior::PairInteractionConstraint<DataTypes>, public virtual core::objectmodel::BaseObject
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;
    typedef topology::PointSubset SetIndex;
    typedef helper::vector<unsigned int> SetIndexArray;


protected:
    AttachConstraintInternalData<DataTypes> data;

    sofa::core::componentmodel::topology::BaseMeshTopology* topology;

public:
    Data<SetIndex> f_indices1;
    Data<SetIndex> f_indices2;
    Data<Real> f_radius;
    Data<bool> f_twoWay;
    Data<bool> f_freeRotations;
    Data<bool> f_lastFreeRotation;
    Data<bool> f_restRotations;
    Data<defaulttype::Vector3> f_lastPos;
    Data<defaulttype::Vector3> f_lastDir;
    Data<bool> f_clamp;
    Data<Real> f_minDistance;

    helper::vector<bool> activeFlags;
    helper::vector<bool> constraintReleased;
    helper::vector<Real> lastDist;
    helper::vector<defaulttype::Quat> restRotations;

    AttachConstraint();

    virtual ~AttachConstraint();

    void clearConstraints();
    void addConstraint(unsigned int index1, unsigned int index2);

    // -- Constraint interface
    void init();
    void projectResponse(VecDeriv& dx1, VecDeriv& dx2);
    virtual void projectVelocity(VecDeriv& v1, VecDeriv& v2);
    virtual void projectPosition(VecCoord& x1, VecCoord& x2);

    void applyConstraint(defaulttype::BaseMatrix *mat, unsigned int &offset);
    void applyConstraint(defaulttype::BaseVector *vect, unsigned int &offset);

    // Handle topological changes
    //virtual void handleTopologyChange();

    virtual void draw();


    // this constraint is holonomic
    bool isHolonomic() {return true;}

protected :
    void projectPosition(Coord& x1, Coord& x2, bool /*freeRotations*/, unsigned index)
    {
        // do nothing if distance between x2 & x1 is bigger than f_minDistance
        if (f_minDistance.getValue() != -1 &&
            (x2 - x1).norm() > f_minDistance.getValue())
        {
            constraintReleased[index] = true;
            return;
        }
        constraintReleased[index] = false;

        x2 = x1;
    }
    void projectVelocity(Deriv& x1, Deriv& x2, bool /*freeRotations*/, unsigned index)
    {
        // do nothing if distance between x2 & x1 is bigger than f_minDistance
        if (constraintReleased[index]) return;

        x2 = x1;
    }
    void projectResponse(Deriv& dx1, Deriv& dx2, bool /*freeRotations*/, bool twoway, unsigned index)
    {
        // do nothing if distance between x2 & x1 is bigger than f_minDistance
        if (constraintReleased[index]) return;

        if (!twoway)
        {
            dx2 = Deriv();
        }
        else
        {
            dx1 += dx2;
            dx2 = dx1;
        }
    }
    static unsigned int DerivConstrainedSize(bool /*freeRotations*/) { return Deriv::size(); }

    void calcRestRotations();

    // Define TestNewPointFunction
    //static bool FCTestNewPointFunction(int, void*, const sofa::helper::vector< unsigned int > &, const sofa::helper::vector< double >& );

    // Define RemovalFunction
    //static void FCRemovalFunction ( int , void*);

};


#if defined(WIN32) && !defined(SOFA_COMPONENT_CONSTRAINT_ATTACHCONSTRAINT_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Vec3dTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Vec2dTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Vec1dTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Rigid3dTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Rigid2dTypes>;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Vec3fTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Vec2fTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Vec1fTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Rigid3fTypes>;
extern template class SOFA_COMPONENT_CONSTRAINT_API AttachConstraint<defaulttype::Rigid2fTypes>;
#endif
#endif

} // namespace constraint

} // namespace component

} // namespace sofa


#endif
