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
#define SOFA_COMPONENT_TOPOLOGY_QUADSETGEOMETRYALGORITHMS_CPP
#include <sofa/component/topology/QuadSetGeometryAlgorithms.h>
#include <sofa/component/topology/QuadSetGeometryAlgorithms.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/core/ObjectFactory.h>
namespace sofa
{

namespace component
{

namespace topology
{
using namespace sofa::defaulttype;
SOFA_DECL_CLASS(QuadSetGeometryAlgorithms)
int QuadSetGeometryAlgorithmsClass = core::RegisterObject("Quad set geometry algorithms")
#ifdef SOFA_FLOAT
        .add< QuadSetGeometryAlgorithms<sofa::defaulttype::Vec3fTypes> >(true) // default template
#else
        .add< QuadSetGeometryAlgorithms<sofa::defaulttype::Vec3dTypes> >(true) // default template
#ifndef SOFA_DOUBLE
        .add< QuadSetGeometryAlgorithms<sofa::defaulttype::Vec3fTypes> >() // default template
#endif
#endif
#ifndef SOFA_FLOAT
        .add< QuadSetGeometryAlgorithms<Vec2dTypes> >()
        .add< QuadSetGeometryAlgorithms<Vec1dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< QuadSetGeometryAlgorithms<Vec2fTypes> >()
        .add< QuadSetGeometryAlgorithms<Vec1fTypes> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class SOFA_COMPONENT_TOPOLOGY_API QuadSetGeometryAlgorithms<sofa::defaulttype::Vec3dTypes>;
template class SOFA_COMPONENT_TOPOLOGY_API QuadSetGeometryAlgorithms<Vec2dTypes>;
template class SOFA_COMPONENT_TOPOLOGY_API QuadSetGeometryAlgorithms<Vec1dTypes>;
#endif

#ifndef SOFA_DOUBLE
template class SOFA_COMPONENT_TOPOLOGY_API QuadSetGeometryAlgorithms<sofa::defaulttype::Vec3fTypes>;
template class SOFA_COMPONENT_TOPOLOGY_API QuadSetGeometryAlgorithms<Vec2fTypes>;
template class SOFA_COMPONENT_TOPOLOGY_API QuadSetGeometryAlgorithms<Vec1fTypes>;
#endif

} // namespace topology

} // namespace component

} // namespace sofa

