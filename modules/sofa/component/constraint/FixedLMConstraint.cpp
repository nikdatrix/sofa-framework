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
#include <sofa/component/constraint/FixedLMConstraint.inl>
#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/RigidTypes.h>

namespace sofa
{

namespace component
{

namespace constraint
{

SOFA_DECL_CLASS(FixedLMConstraint)

int FixedLMConstraintClass = core::RegisterObject("Maintain a set of particle to a fixed position using LMConstraint")
#ifndef SOFA_FLOAT
  .add< FixedLMConstraint<defaulttype::Vec3dTypes> >()
  .add< FixedLMConstraint<defaulttype::Rigid3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
  .add< FixedLMConstraint<defaulttype::Vec3fTypes> >()
  .add< FixedLMConstraint<defaulttype::Rigid3fTypes> >()
#endif
;

#ifndef SOFA_FLOAT
template class FixedLMConstraint<Vec3dTypes>;
template class FixedLMConstraint<Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class FixedLMConstraint<Vec3fTypes>;
template class FixedLMConstraint<Rigid3fTypes>;
#endif


#ifndef SOFA_FLOAT
template <>
void FixedLMConstraint<Rigid3dTypes>::draw()
{
	const SetIndexArray & indices = f_indices.getValue().getArray();
	if (!getContext()->getShowBehaviorModels()) return;
	VecCoord& x = *constrainedObject1->getX();

        std::vector< Vector3 > points;
        for (SetIndexArray::const_iterator it = indices.begin();
             it != indices.end();
             ++it)
          {
            points.push_back(x[*it].getCenter());
          }

        if( _drawSize.getValue() == 0) // old classical drawing by points
          {
            simulation::getSimulation()->DrawUtility.drawPoints(points, 10, Vec<4,float>(1,0.5,0.5,1));
          }
        else
          {
            simulation::getSimulation()->DrawUtility.drawSpheres(points, (float)_drawSize.getValue(), Vec<4,float>(1.0f,0.35f,0.35f,1.0f));
          }
}

#endif

#ifndef SOFA_DOUBLE
template <>
void FixedLMConstraint<Rigid3fTypes>::draw()
{
	const SetIndexArray & indices = f_indices.getValue().getArray();
	if (!getContext()->getShowBehaviorModels()) return;
	VecCoord& x = *constrainedObject1->getX();

        std::vector< Vector3 > points;
        for (SetIndexArray::const_iterator it = indices.begin();
             it != indices.end();
             ++it)
          {
            points.push_back(x[*it].getCenter());
          }

        if( _drawSize.getValue() == 0) // old classical drawing by points
          {
            simulation::getSimulation()->DrawUtility.drawPoints(points, 10, Vec<4,float>(1,0.5,0.5,1));
          }
        else
          {
            simulation::getSimulation()->DrawUtility.drawSpheres(points, (float)_drawSize.getValue(), Vec<4,float>(1.0f,0.35f,0.35f,1.0f));
          }
}

#endif



} // namespace constraint

} // namespace component

} // namespace sofa

