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



// File automatically generated by "generateTypedef"


#ifndef SOFA_TYPEDEF_Mapping_combination_H
#define SOFA_TYPEDEF_Mapping_combination_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>

//Default files needed to create a Mapping
#include <sofa/core/componentmodel/behavior/State.h>
#include <sofa/core/componentmodel/behavior/MappedModel.h>
#include <sofa/core/Mapping.h>


#include <sofa/component/mapping/BarycentricMapping.h>
#include <sofa/component/mapping/BeamLinearMapping.h>
#include <sofa/component/mapping/CenterOfMassMapping.h>
#include <sofa/component/mapping/CenterPointMechanicalMapping.h>
#include <sofa/component/mapping/IdentityMapping.h>
#include <sofa/component/mapping/ImplicitSurfaceMapping.h>
#include <sofa/component/mapping/LineSetSkinningMapping.h>
#include <sofa/component/mapping/Mesh2PointMechanicalMapping.h>
#include <sofa/component/mapping/RigidMapping.h>
#include <sofa/component/mapping/RigidRigidMapping.h>
#include <sofa/component/mapping/SPHFluidSurfaceMapping.h>
#include <sofa/component/mapping/SimpleTesselatedTetraMechanicalMapping.h>
#include <sofa/component/mapping/SkinningMapping.h>
#include <sofa/component/mapping/SubsetMapping.h>
#include <sofa/component/mapping/TubularMapping.h>



//---------------------------------------------------------------------------------------------
//Typedef for BarycentricMapping
typedef sofa::component::mapping::BarycentricMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > BarycentricMapping3f_to_3d;
typedef sofa::component::mapping::BarycentricMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > BarycentricMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for BeamLinearMapping
typedef sofa::component::mapping::BeamLinearMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > BeamLinearMappingRigid3d_to_3f;
typedef sofa::component::mapping::BeamLinearMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > BeamLinearMappingRigid3f_to_3d;



//---------------------------------------------------------------------------------------------
//Typedef for CenterOfMassMapping
typedef sofa::component::mapping::CenterOfMassMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::ExtVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > CenterOfMassMappingRigid3f_to_Ext3d;
typedef sofa::component::mapping::CenterOfMassMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > CenterOfMassMappingRigid3d_to_3f;
typedef sofa::component::mapping::CenterOfMassMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > CenterOfMassMappingRigid3f_to_3d;



//---------------------------------------------------------------------------------------------
//Typedef for CenterPointMechanicalMapping
typedef sofa::component::mapping::CenterPointMechanicalMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > CenterPointMechanicalMapping3f_to_3d;
typedef sofa::component::mapping::CenterPointMechanicalMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > CenterPointMechanicalMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for IdentityMapping
typedef sofa::component::mapping::IdentityMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > IdentityMapping3f_to_3d;
typedef sofa::component::mapping::IdentityMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > IdentityMapping3d_to_3f;
typedef sofa::component::mapping::IdentityMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdRigidTypes<3, double> > > > IdentityMappingRigid3f_to_Rigid3d;
typedef sofa::component::mapping::IdentityMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdRigidTypes<3, float> > > > IdentityMappingRigid3d_to_Rigid3f;
typedef sofa::component::mapping::IdentityMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<2, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdRigidTypes<2, float> > > > IdentityMappingRigid2d_to_Rigid2f;
typedef sofa::component::mapping::IdentityMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<2, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdRigidTypes<2, double> > > > IdentityMappingRigid2f_to_Rigid2d;



//---------------------------------------------------------------------------------------------
//Typedef for ImplicitSurfaceMapping
typedef sofa::component::mapping::ImplicitSurfaceMapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > ImplicitSurfaceMapping3f_to_3d;
typedef sofa::component::mapping::ImplicitSurfaceMapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > ImplicitSurfaceMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for LineSetSkinningMapping
typedef sofa::component::mapping::LineSetSkinningMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > LineSetSkinningMappingRigid3d_to_3f;
typedef sofa::component::mapping::LineSetSkinningMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > LineSetSkinningMappingRigid3f_to_3d;



//---------------------------------------------------------------------------------------------
//Typedef for Mesh2PointMechanicalMapping
typedef sofa::component::mapping::Mesh2PointMechanicalMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > Mesh2PointMechanicalMapping3f_to_3d;
typedef sofa::component::mapping::Mesh2PointMechanicalMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > Mesh2PointMechanicalMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for RigidMapping
typedef sofa::component::mapping::RigidMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > RigidMappingRigid3d_to_3f;
typedef sofa::component::mapping::RigidMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > RigidMappingRigid3f_to_3d;



//---------------------------------------------------------------------------------------------
//Typedef for RigidRigidMapping
typedef sofa::component::mapping::RigidRigidMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdRigidTypes<3, float> > > > RigidRigidMappingRigid3d_to_Rigid3f;
typedef sofa::component::mapping::RigidRigidMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdRigidTypes<3, double> > > > RigidRigidMappingRigid3f_to_Rigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for SPHFluidSurfaceMapping
typedef sofa::component::mapping::SPHFluidSurfaceMapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > SPHFluidSurfaceMapping3f_to_3d;
typedef sofa::component::mapping::SPHFluidSurfaceMapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > SPHFluidSurfaceMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for SimpleTesselatedTetraMechanicalMapping
typedef sofa::component::mapping::SimpleTesselatedTetraMechanicalMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > SimpleTesselatedTetraMechanicalMapping3f_to_3d;
typedef sofa::component::mapping::SimpleTesselatedTetraMechanicalMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > SimpleTesselatedTetraMechanicalMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for SkinningMapping
typedef sofa::component::mapping::SkinningMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > SkinningMappingRigid3d_to_3f;
typedef sofa::component::mapping::SkinningMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > SkinningMappingRigid3f_to_3d;



//---------------------------------------------------------------------------------------------
//Typedef for SubsetMapping
typedef sofa::component::mapping::SubsetMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > SubsetMapping3f_to_3d;
typedef sofa::component::mapping::SubsetMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > SubsetMapping3d_to_3f;



//---------------------------------------------------------------------------------------------
//Typedef for TubularMapping
typedef sofa::component::mapping::TubularMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, double> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > > > TubularMappingRigid3d_to_3f;
typedef sofa::component::mapping::TubularMapping<sofa::core::Mapping<sofa::core::componentmodel::behavior::State<sofa::defaulttype::StdRigidTypes<3, float> >, sofa::core::componentmodel::behavior::MappedModel<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > > > TubularMappingRigid3f_to_3d;





#endif
