/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#include <sofa/component/mapping/IdentityMapping.inl>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/componentmodel/behavior/MappedModel.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/componentmodel/behavior/MechanicalMapping.h>

namespace sofa
{

namespace component
{

namespace mapping
{

using namespace sofa::defaulttype;
using namespace core;
using namespace core::componentmodel::behavior;

SOFA_DECL_CLASS(IdentityMapping)

// Register in the Factory
int IdentityMappingClass = core::RegisterObject("Special case of mapping where the child points are the same as the parent points")
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3fTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<Vec3dTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<Vec3dTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<Vec3fTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<Vec3fTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<ExtVec3dTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<ExtVec3dTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<ExtVec3fTypes> > > >()
        .add< IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<ExtVec3fTypes> > > >()

        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec2dTypes>, MechanicalState<Vec2dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec2fTypes>, MechanicalState<Vec2dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec2dTypes>, MechanicalState<Vec2fTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec2fTypes>, MechanicalState<Vec2fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<Vec2dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<Vec2dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<Vec2fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<Vec2fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<ExtVec2dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<ExtVec2dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<ExtVec2fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<ExtVec2fTypes> > > >()

        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Vec1dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Vec1dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Vec1fTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Vec1fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<Vec1dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<Vec1dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<Vec1fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<Vec1fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<ExtVec1dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<ExtVec1dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<ExtVec1fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<ExtVec1fTypes> > > >()

        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec6dTypes>, MechanicalState<Vec6dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec6fTypes>, MechanicalState<Vec6dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec6dTypes>, MechanicalState<Vec6fTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Vec6fTypes>, MechanicalState<Vec6fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<Vec6dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<Vec6dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<Vec6fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<Vec6fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<ExtVec6dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<ExtVec6dTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<ExtVec6fTypes> > > >()
// .add< IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<ExtVec6fTypes> > > >()

        .add< IdentityMapping< MechanicalMapping< MechanicalState<Rigid3dTypes>, MechanicalState<Rigid3dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Rigid3fTypes>, MechanicalState<Rigid3fTypes> > > >()

        .add< IdentityMapping< MechanicalMapping< MechanicalState<Rigid2dTypes>, MechanicalState<Rigid2dTypes> > > >()
        .add< IdentityMapping< MechanicalMapping< MechanicalState<Rigid2fTypes>, MechanicalState<Rigid2fTypes> > > >()
        ;


// Mech -> Mech
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3dTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec3dTypes>, MechanicalState<Vec3fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec3fTypes>, MechanicalState<Vec3dTypes> > >;

// Mech -> Mapped
template class IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<Vec3dTypes> > >;
template class IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<Vec3fTypes> > >;
template class IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<Vec3fTypes> > >;
template class IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<Vec3dTypes> > >;

// Mech -> ExtMapped
template class IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<ExtVec3dTypes> > >;
template class IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<ExtVec3fTypes> > >;
template class IdentityMapping< Mapping< MechanicalState<Vec3dTypes>, MappedModel<ExtVec3fTypes> > >;
template class IdentityMapping< Mapping< MechanicalState<Vec3fTypes>, MappedModel<ExtVec3dTypes> > >;


// Mech -> Mech
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec2dTypes>, MechanicalState<Vec2dTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec2fTypes>, MechanicalState<Vec2fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec2dTypes>, MechanicalState<Vec2fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec2fTypes>, MechanicalState<Vec2dTypes> > >;

// // Mech -> Mapped
// template class IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<Vec2dTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<Vec2fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<Vec2fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<Vec2dTypes> > >;
//
// // Mech -> ExtMapped
// template class IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<ExtVec2dTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<ExtVec2fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec2dTypes>, MappedModel<ExtVec2fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec2fTypes>, MappedModel<ExtVec2dTypes> > >;


// Mech -> Mech
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Vec1dTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Vec1fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec1dTypes>, MechanicalState<Vec1fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec1fTypes>, MechanicalState<Vec1dTypes> > >;

// // Mech -> Mapped
// template class IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<Vec1dTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<Vec1fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<Vec1fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<Vec1dTypes> > >;
//
// // Mech -> ExtMapped
// template class IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<ExtVec1dTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<ExtVec1fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec1dTypes>, MappedModel<ExtVec1fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec1fTypes>, MappedModel<ExtVec1dTypes> > >;


// Mech -> Mech
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec6dTypes>, MechanicalState<Vec6dTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec6fTypes>, MechanicalState<Vec6fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec6dTypes>, MechanicalState<Vec6fTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Vec6fTypes>, MechanicalState<Vec6dTypes> > >;

// // Mech -> Mapped
// template class IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<Vec6dTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<Vec6fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<Vec6fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<Vec6dTypes> > >;
//
// // Mech -> ExtMapped
// template class IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<ExtVec6dTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<ExtVec6fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec6dTypes>, MappedModel<ExtVec6fTypes> > >;
// template class IdentityMapping< Mapping< MechanicalState<Vec6fTypes>, MappedModel<ExtVec6dTypes> > >;


// Mech -> Mech
template class IdentityMapping< MechanicalMapping< MechanicalState<Rigid3dTypes>, MechanicalState<Rigid3dTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Rigid3fTypes>, MechanicalState<Rigid3fTypes> > >;


// Mech -> Mech
template class IdentityMapping< MechanicalMapping< MechanicalState<Rigid2dTypes>, MechanicalState<Rigid2dTypes> > >;
template class IdentityMapping< MechanicalMapping< MechanicalState<Rigid2fTypes>, MechanicalState<Rigid2fTypes> > >;

} // namespace mapping

} // namespace component

} // namespace sofa

