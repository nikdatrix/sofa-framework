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
#include <sofa/component/forcefield/RegularGridSpringForceField.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace forcefield
{

SOFA_DECL_CLASS(RegularGridSpringForceField)

using namespace sofa::defaulttype;


// Register in the Factory
int RegularGridSpringForceFieldClass = core::RegisterObject("Spring acting on the edges and faces of a regular grid")
#ifdef SOFA_FLOAT
    .add< RegularGridSpringForceField<Vec3fTypes> >(true) // default template
#else
    .add< RegularGridSpringForceField<Vec3dTypes> >(true) // default template    
#ifndef SOFA_DOUBLE
    .add< RegularGridSpringForceField<Vec3fTypes> >() // default template
#endif
#endif
#ifndef SOFA_FLOAT
.add< RegularGridSpringForceField<Vec2dTypes> >()
.add< RegularGridSpringForceField<Vec1dTypes> >()
.add< RegularGridSpringForceField<Vec6dTypes> >()
#endif
#ifndef SOFA_DOUBLE
.add< RegularGridSpringForceField<Vec2fTypes> >()
.add< RegularGridSpringForceField<Vec1fTypes> >()
.add< RegularGridSpringForceField<Vec6fTypes> >()
#endif
;
#ifndef SOFA_FLOAT
template class RegularGridSpringForceField<Vec3dTypes>;
template class RegularGridSpringForceField<Vec2dTypes>;
template class RegularGridSpringForceField<Vec1dTypes>;
template class RegularGridSpringForceField<Vec6dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class RegularGridSpringForceField<Vec3fTypes>;
template class RegularGridSpringForceField<Vec2fTypes>;
template class RegularGridSpringForceField<Vec1fTypes>;
template class RegularGridSpringForceField<Vec6fTypes>;
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa

