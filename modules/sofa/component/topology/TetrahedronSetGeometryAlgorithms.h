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
#ifndef SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETGEOMETRYALGORITHMS_H
#define SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETGEOMETRYALGORITHMS_H

#include <sofa/component/topology/TriangleSetGeometryAlgorithms.h>

namespace sofa
{

namespace component
{

namespace topology
{
using core::componentmodel::topology::BaseMeshTopology;
typedef BaseMeshTopology::TetraID TetraID;
typedef BaseMeshTopology::Tetra Tetra;
typedef BaseMeshTopology::SeqTetras SeqTetras;
typedef BaseMeshTopology::VertexTetras VertexTetras;
typedef BaseMeshTopology::EdgeTetras EdgeTetras;
typedef BaseMeshTopology::TriangleTetras TriangleTetras;
typedef BaseMeshTopology::TetraEdges TetraEdges;
typedef BaseMeshTopology::TetraTriangles TetraTriangles;

typedef Tetra Tetrahedron;
typedef TetraEdges TetrahedronEdges;
typedef TetraTriangles TetrahedronTriangles;

/**
* A class that provides geometry information on an TetrahedronSet.
*/
template < class DataTypes >
class TetrahedronSetGeometryAlgorithms : public TriangleSetGeometryAlgorithms<DataTypes>
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::Real Real;
    typedef typename DataTypes::Coord Coord;

    TetrahedronSetGeometryAlgorithms()
        : TriangleSetGeometryAlgorithms<DataTypes>()
    {}

    virtual ~TetrahedronSetGeometryAlgorithms() {}

    void computeTetrahedronAABB(const TetraID i, Coord& minCoord, Coord& maxCoord) const;

    Coord computeTetrahedronCenter(const TetraID i) const;

    Coord computeTetrahedronCircumcenter(const TetraID i) const;

    bool isPointInTetrahedron(const TetraID i, const Vec<3,Real>& p) const;

    void getTetrahedronVertexCoordinates(const TetraID i, Coord[4]) const;

    void getRestTetrahedronVertexCoordinates(const TetraID i, Coord[4]) const;

    /// computes the volume of tetrahedron no i and returns it
    Real computeTetrahedronVolume(const TetraID i) const;

    /// computes the tetrahedron volume of all tetrahedra are store in the array interface
    void computeTetrahedronVolume( BasicArrayInterface<Real> &ai) const;

    /// computes the tetrahedron volume  of tetrahedron no i and returns it
    Real computeRestTetrahedronVolume(const TetraID i) const;

    /// finds the indices of all tetrahedra in the ball of center ind_ta and of radius dist(ind_ta, ind_tb)
    void getTetraInBall(const TetraID ind_ta, const TetraID ind_tb,
            sofa::helper::vector<unsigned int> &indices) const;

    /** \brief Write the current mesh into a msh file
    */
    void writeMSHfile(const char *filename) const;
};

#if defined(WIN32) && !defined(SOFA_COMPONENT_TOPOLOGY_TETRAHEDRONSETGEOMETRYALGORITHMS_CPP)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Vec3dTypes>;
extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Vec2dTypes>;
extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Vec1dTypes>;
//extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Rigid3dTypes>;
//extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Rigid2dTypes>;
#endif

#ifndef SOFA_DOUBLE
extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Vec3fTypes>;
extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Vec2fTypes>;
extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Vec1fTypes>;
//extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Rigid3fTypes>;
//extern template class SOFA_COMPONENT_TOPOLOGY_API TetrahedronSetGeometryAlgorithms<defaulttype::Rigid2fTypes>;
#endif
#endif

} // namespace topology

} // namespace component

} // namespace sofa

#endif
