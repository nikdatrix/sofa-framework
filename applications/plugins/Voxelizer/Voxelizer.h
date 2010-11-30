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
#ifndef SOFA_COMPONENT_LOADER_VOXELIZER_H
#define SOFA_COMPONENT_LOADER_VOXELIZER_H

#include <sofa/core/loader/MeshLoader.h>
//#include <sofa/core/objectmodel/BaseData.h>
#include "initVoxelizer.h"
#include <sofa/gpu/cuda/CudaRasterizer.h>
//#include <tinyxml.h>
#include <sofa/helper/vector.h>
#include <string>
#include <map>
#include <sofa/defaulttype/Vec.h>
#include <sofa/core/objectmodel/BaseNode.h>

#define USE_MAP_FOR_VOLUMES



namespace sofa
{
namespace helper { namespace io { class ImageRAW; }}

namespace component
{

namespace loader
{

using std::string;
using sofa::helper::vector;
using sofa::defaulttype::Vec3d;
using sofa::core::objectmodel::Tag;
using sofa::core::objectmodel::TagSet;
using sofa::core::loader::MeshLoader;
using sofa::core::objectmodel::BaseContext;
using sofa::core::objectmodel::BaseNode;
using sofa::core::objectmodel::BaseObject;

/* class Voxelizer
 * This loader allow to load voxels from the analysis of the LDI structure generated by the rasterizer.
 *
 * Args:
 * vPosition describes the center of the ROIs.
 * vRadius describes the radius of the ROIs. If the radius is null, all the volume defined by the model(s) is filled with voxels.
 * vTriangularModel describes the triangular model which is rasterized to create voxels. If the given model is NULL, voxels are added outside of all the models present in the scene.
 * vVoxelSize describes the size of the generated voxels.
 * vOffsets describes the minimal space between the triangular model(s) and the generated voxels.
 *
*/
template <class DataTypes>
class SOFA_VOXELIZER_API Voxelizer : public MeshLoader
{
public:
    SOFA_CLASS( SOFA_TEMPLATE ( Voxelizer, DataTypes) ,MeshLoader);

    typedef gpu::cuda::CudaRasterizer<DataTypes> Rasterizer;
    typedef typename Rasterizer::CellCountLayer CellCountLayer;
    typedef typename Rasterizer::Cell Cell;
    typedef typename Rasterizer::LDI LDI;

    typedef sofa::core::topology::BaseMeshTopology MTopology;
//        typedef std::pair< Vec3d, Vec3d > BoundingBox;
    typedef typename defaulttype::Vec3f FPoint;
    typedef typename helper::fixed_array<FPoint, 2> BBox;
    typedef typename DataTypes::Real Real;
#ifndef USE_MAP_FOR_VOLUMES
    typedef vector<BoundingBox> RasterizedVol;
#else
    typedef std::multimap< double, std::multimap<double, std::pair< double, double> > > RasterizedVol; // map< x, map< y, pair< zMin, zMax> > >
#endif

    // ROI
    Data<vector<Vec3d> > vPosition;
    Data<vector<double> > vRadius;
    Data<bool> useROI;

    // Global volume
    Data<BBox> bbox;

    // Rasterized Objects
    Data<string> triangularModelPath;
    Data<Vec3d> voxelSize;
    Data<vector<double> > vOffsets; //TODO !! Not yet implemented
    //Data<bool> generateVoxels;

    // Generated files
    Data<bool> generateRAWFiles;
    Data<string> valueFileName;
    Data<string> segmentationFileName;
    Data<string> infoFileName;
    Data<Vec3d> rawOrigin; // MechObj can connect their "position" Data on this one.
    Data<Vec3d> resolution;
    Data<bool> showRasterizedVolumes;

    Voxelizer();
    ~Voxelizer();

    virtual void init();

    virtual void draw();

    virtual bool canLoad();
    virtual bool load();
    //virtual bool createVoxels (RasterizedVol** rasterizedVolume); // rasterizedVolume array indexed following: axis and triangularModel
    virtual bool createImages (RasterizedVol** rasterizedVolume); // rasterizedVolume array indexed following: axis and triangularModel

    template <class T>
    static bool canCreate ( T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg );

protected:
    Rasterizer* rasterizer;
    vector<MTopology*> vTriangularModel;
    vector<bool> hadSelfCollisionTag;

    // Initial rasterizer settings
    TagSet rasterizerTags;
    double rasterizerPixelSize;
    BBox rasterizerBBox;
    RasterizedVol** rasterizedVolumes; // Temporary kept for debugging

    helper::io::ImageRAW* valueImg;
    helper::io::ImageRAW* segmentationImg;

    inline void addVolume( RasterizedVol& rasterizedVolume, double x, double y, double zMin, double zMax, int axis);
    inline void generateFullVolumes( RasterizedVol** rasterizedVolume);
    inline bool isCoordInside( const Vec3d& position, const RasterizedVol& rasterizedVolume, const unsigned int axis); // return true if pos is in the rasterizedVolume.
    inline unsigned int coordIsInside( const Vec3d& position, const RasterizedVol* rasterizedVolume, const unsigned int axis); // return the rasterizeVolume index in which pos is present.
    inline bool isCoordInside( const BBox& bbox, const RasterizedVol& rasterizedVolume, const unsigned int axis); // return true if pos is in the rasterizedVolume.
    inline unsigned int coordIsInside( const BBox& bbox, const RasterizedVol* rasterizedVolume, const unsigned int axis); // return the rasterizeVolume index in which pos is present.
    inline bool isCrossingAABB ( const Vec3d& min1, const Vec3d& max1, const Vec3d& min2, const Vec3d& max2 ) const;
    inline bool isInsideAABB ( const Vec3d& min1, const Vec3d& max1, const Vec3d& point ) const;
    void createVoxel( helper::vector<helper::fixed_array <unsigned int,8> >* hexahedra, const Vec3d& coord);
    int createPoint( const double& x, const double& y, const double& z);
    void temporaryChangeTags();
    void restoreTags();
    template<class T>
    T* findObject( string path, const BaseContext* context);
    bool saveRAW( const std::string filename, const unsigned char* data, const unsigned int size) const;
    void saveInfos();
    void loadInfos();
    inline void changeRasterizerSettings();
    inline void reloadRasterizerSettings();
};

#if defined(WIN32) && !defined(SOFA_BUILD_VOXEL_LOADER_BY_RASTERIZATION)
#pragma warning(disable : 4231)
#ifndef SOFA_FLOAT
extern template class Voxelizer<defaulttype::Vec3dTypes>;
#endif
#ifndef SOFA_DOUBLE
#endif
#endif



} // namespace loader

} // namespace component

} // namespace sofa

#endif
