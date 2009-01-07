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
#ifndef SOFA_COMPONENT_TOPOLOGY_DYNAMICSPARSEGRIDGEOMETRYALGORITHMS_H
#define SOFA_COMPONENT_TOPOLOGY_DYNAMICSPARSEGRIDGEOMETRYALGORITHMS_H

#include <sofa/component/topology/HexahedronSetGeometryAlgorithms.h>

namespace sofa
{
namespace component
{
namespace topology
{
/**
* A class that provides geometry information on an HexahedronSet.
*/
template < class DataTypes >
class DynamicSparseGridGeometryAlgorithms : public HexahedronSetGeometryAlgorithms<DataTypes>
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Coord Coord;

    DynamicSparseGridGeometryAlgorithms()
        : HexahedronSetGeometryAlgorithms<DataTypes>()
    {}

    virtual ~DynamicSparseGridGeometryAlgorithms() {}

    /// finds a hexahedron which is nearest to a given point. Computes barycentric coordinates and a distance measure.
    virtual int findNearestElement ( const Coord& pos, defaulttype::Vector3& baryC, Real& distance ) const;

    /// given a vector of points, find the nearest hexa for each point. Computes barycentric coordinates and a distance measure.
    virtual void findNearestElements ( const VecCoord& pos, helper::vector<int>& elem,
            helper::vector<defaulttype::Vector3>& baryC, helper::vector<Real>& dist ) const;
};

} // namespace topology

} // namespace component

} // namespace sofa

#endif
