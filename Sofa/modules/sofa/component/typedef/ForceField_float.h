/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 INRIA, USTL, UJF, CNRS, MGH                    *
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


#ifndef SOFA_TYPEDEF_ForceField_float_H
#define SOFA_TYPEDEF_ForceField_float_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>




#include <sofa/component/forcefield/BeamFEMForceField.h>
#include <sofa/component/interactionforcefield/BoxStiffSpringForceField.h>
#include <sofa/component/forcefield/BuoyantForceField.h>
#include <sofa/component/forcefield/ConicalForceField.h>
#include <sofa/component/forcefield/ConstantForceField.h>
#include <sofa/component/forcefield/DistanceGridForceField.h>
#include <sofa/component/forcefield/EllipsoidForceField.h>
#include <sofa/component/forcefield/FastTetrahedralCorotationalForceField.h>
#include <sofa/component/interactionforcefield/FrameSpringForceField.h>
#include <sofa/component/interactionforcefield/GearSpringForceField.h>
#include <sofa/component/forcefield/HexahedralFEMForceField.h>
#include <sofa/component/forcefield/HexahedronFEMForceField.h>
#include <sofa/component/interactionforcefield/InteractionEllipsoidForceField.h>
#include <sofa/component/interactionforcefield/JointSpringForceField.h>
#include <sofa/component/forcefield/LennardJonesForceField.h>
#include <sofa/component/interactionforcefield/LineBendingSprings.h>
#include <sofa/component/forcefield/LinearForceField.h>
#include <sofa/component/interactionforcefield/MeshSpringForceField.h>
#include <sofa/component/forcefield/ParticlesRepulsionForceField.h>
#include <sofa/component/interactionforcefield/PenalityContactForceField.h>
#include <sofa/component/forcefield/PlaneForceField.h>
#include <sofa/component/interactionforcefield/QuadBendingSprings.h>
#include <sofa/component/forcefield/QuadularBendingSprings.h>
#include <sofa/component/interactionforcefield/RegularGridSpringForceField.h>
#include <sofa/component/interactionforcefield/RepulsiveSpringForceField.h>
#include <sofa/component/forcefield/RestShapeSpringsForceField.h>
#include <sofa/component/forcefield/SPHFluidForceField.h>
#include <sofa/component/forcefield/SphereForceField.h>
#include <sofa/component/interactionforcefield/SpringForceField.h>
#include <sofa/component/interactionforcefield/StiffSpringForceField.h>
#include <sofa/component/forcefield/SurfacePressureForceField.h>
#include <sofa/component/forcefield/TetrahedralCorotationalFEMForceField.h>
#include <sofa/component/forcefield/TetrahedralTensorMassForceField.h>
#include <sofa/component/forcefield/TetrahedronFEMForceField.h>
#include <sofa/component/interactionforcefield/TriangleBendingSprings.h>
#include <sofa/component/forcefield/TriangleFEMForceField.h>
#include <sofa/component/forcefield/TriangularAnisotropicFEMForceField.h>
#include <sofa/component/forcefield/TriangularBendingSprings.h>
#include <sofa/component/forcefield/TriangularBiquadraticSpringsForceField.h>
#include <sofa/component/forcefield/TriangularFEMForceField.h>
#include <sofa/component/forcefield/TriangularQuadraticSpringsForceField.h>
#include <sofa/component/forcefield/TriangularTensorMassForceField.h>
#include <sofa/component/forcefield/VaccumSphereForceField.h>
#include <sofa/component/interactionforcefield/VectorSpringForceField.h>
#include <sofa/component/forcefield/WashingMachineForceField.h>




//---------------------------------------------------------------------------------------------
//Typedef for BeamFEMForceField
typedef sofa::component::forcefield::BeamFEMForceField<sofa::defaulttype::StdRigidTypes<3, float> > BeamFEMForceFieldRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for BoxStiffSpringForceField
typedef sofa::component::interactionforcefield::BoxStiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > BoxStiffSpringForceField1f;
typedef sofa::component::interactionforcefield::BoxStiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > BoxStiffSpringForceField2f;
typedef sofa::component::interactionforcefield::BoxStiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > BoxStiffSpringForceField3f;
typedef sofa::component::interactionforcefield::BoxStiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > BoxStiffSpringForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for BuoyantForceField
typedef sofa::component::forcefield::BuoyantForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > BuoyantForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for ConicalForceField
typedef sofa::component::forcefield::ConicalForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > ConicalForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for ConstantForceField
typedef sofa::component::forcefield::ConstantForceField<sofa::defaulttype::StdRigidTypes<2, float> > ConstantForceFieldRigid2f;
typedef sofa::component::forcefield::ConstantForceField<sofa::defaulttype::StdRigidTypes<3, float> > ConstantForceFieldRigid3f;
typedef sofa::component::forcefield::ConstantForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > ConstantForceField1f;
typedef sofa::component::forcefield::ConstantForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > ConstantForceField2f;
typedef sofa::component::forcefield::ConstantForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > ConstantForceField3f;
typedef sofa::component::forcefield::ConstantForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > ConstantForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for DistanceGridForceField
typedef sofa::component::forcefield::DistanceGridForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > DistanceGridForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for EllipsoidForceField
typedef sofa::component::forcefield::EllipsoidForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > EllipsoidForceField1f;
typedef sofa::component::forcefield::EllipsoidForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > EllipsoidForceField2f;
typedef sofa::component::forcefield::EllipsoidForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > EllipsoidForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for FastTetrahedralCorotationalForceField
typedef sofa::component::forcefield::FastTetrahedralCorotationalForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > FastTetrahedralCorotationalForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for FrameSpringForceField
typedef sofa::component::interactionforcefield::FrameSpringForceField<sofa::defaulttype::StdRigidTypes<3, float> > FrameSpringForceFieldRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for GearSpringForceField
typedef sofa::component::interactionforcefield::GearSpringForceField<sofa::defaulttype::StdRigidTypes<3, float> > GearSpringForceFieldRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for HexahedralFEMForceField
typedef sofa::component::forcefield::HexahedralFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > HexahedralFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for HexahedronFEMForceField
typedef sofa::component::forcefield::HexahedronFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > HexahedronFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for InteractionEllipsoidForceField
typedef sofa::component::interactionforcefield::InteractionEllipsoidForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float>, sofa::defaulttype::StdRigidTypes<3, float> > InteractionEllipsoidForceField3f_Rigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for JointSpringForceField
typedef sofa::component::interactionforcefield::JointSpringForceField<sofa::defaulttype::StdRigidTypes<3, float> > JointSpringForceFieldRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for LennardJonesForceField
typedef sofa::component::forcefield::LennardJonesForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > LennardJonesForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for LineBendingSprings
typedef sofa::component::interactionforcefield::LineBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > LineBendingSprings2f;
typedef sofa::component::interactionforcefield::LineBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > LineBendingSprings3f;



//---------------------------------------------------------------------------------------------
//Typedef for LinearForceField
typedef sofa::component::forcefield::LinearForceField<sofa::defaulttype::StdRigidTypes<3, float> > LinearForceFieldRigid3f;
typedef sofa::component::forcefield::LinearForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > LinearForceField1f;
typedef sofa::component::forcefield::LinearForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > LinearForceField2f;
typedef sofa::component::forcefield::LinearForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > LinearForceField3f;
typedef sofa::component::forcefield::LinearForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > LinearForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for MeshSpringForceField
typedef sofa::component::interactionforcefield::MeshSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > MeshSpringForceField1f;
typedef sofa::component::interactionforcefield::MeshSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > MeshSpringForceField2f;
typedef sofa::component::interactionforcefield::MeshSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > MeshSpringForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for ParticlesRepulsionForceField
typedef sofa::component::forcefield::ParticlesRepulsionForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > ParticlesRepulsionForceField2f;
typedef sofa::component::forcefield::ParticlesRepulsionForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > ParticlesRepulsionForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for PenalityContactForceField
typedef sofa::component::interactionforcefield::PenalityContactForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > PenalityContactForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for PlaneForceField
typedef sofa::component::forcefield::PlaneForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > PlaneForceField1f;
typedef sofa::component::forcefield::PlaneForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > PlaneForceField2f;
typedef sofa::component::forcefield::PlaneForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > PlaneForceField3f;
typedef sofa::component::forcefield::PlaneForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > PlaneForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for QuadBendingSprings
typedef sofa::component::interactionforcefield::QuadBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > QuadBendingSprings2f;
typedef sofa::component::interactionforcefield::QuadBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > QuadBendingSprings3f;



//---------------------------------------------------------------------------------------------
//Typedef for QuadularBendingSprings
typedef sofa::component::forcefield::QuadularBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > QuadularBendingSprings3f;



//---------------------------------------------------------------------------------------------
//Typedef for RegularGridSpringForceField
typedef sofa::component::interactionforcefield::RegularGridSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > RegularGridSpringForceField1f;
typedef sofa::component::interactionforcefield::RegularGridSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > RegularGridSpringForceField2f;
typedef sofa::component::interactionforcefield::RegularGridSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > RegularGridSpringForceField3f;
typedef sofa::component::interactionforcefield::RegularGridSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > RegularGridSpringForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for RepulsiveSpringForceField
typedef sofa::component::interactionforcefield::RepulsiveSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > RepulsiveSpringForceField1f;
typedef sofa::component::interactionforcefield::RepulsiveSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > RepulsiveSpringForceField2f;
typedef sofa::component::interactionforcefield::RepulsiveSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > RepulsiveSpringForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for RestShapeSpringsForceField
typedef sofa::component::forcefield::RestShapeSpringsForceField<sofa::defaulttype::StdRigidTypes<3, float> > RestShapeSpringsForceFieldRigid3f;
typedef sofa::component::forcefield::RestShapeSpringsForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > RestShapeSpringsForceField1f;
typedef sofa::component::forcefield::RestShapeSpringsForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > RestShapeSpringsForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for SPHFluidForceField
typedef sofa::component::forcefield::SPHFluidForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > SPHFluidForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for SphereForceField
typedef sofa::component::forcefield::SphereForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > SphereForceField1f;
typedef sofa::component::forcefield::SphereForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > SphereForceField2f;
typedef sofa::component::forcefield::SphereForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > SphereForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for SpringForceField
typedef sofa::component::interactionforcefield::SpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > SpringForceField1f;
typedef sofa::component::interactionforcefield::SpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > SpringForceField2f;
typedef sofa::component::interactionforcefield::SpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > SpringForceField3f;
typedef sofa::component::interactionforcefield::SpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > SpringForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for StiffSpringForceField
typedef sofa::component::interactionforcefield::StiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > StiffSpringForceField1f;
typedef sofa::component::interactionforcefield::StiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > StiffSpringForceField2f;
typedef sofa::component::interactionforcefield::StiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > StiffSpringForceField3f;
typedef sofa::component::interactionforcefield::StiffSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > StiffSpringForceField6f;



//---------------------------------------------------------------------------------------------
//Typedef for SurfacePressureForceField
typedef sofa::component::forcefield::SurfacePressureForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > SurfacePressureForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralCorotationalFEMForceField
typedef sofa::component::forcefield::TetrahedralCorotationalFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TetrahedralCorotationalFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedralTensorMassForceField
typedef sofa::component::forcefield::TetrahedralTensorMassForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TetrahedralTensorMassForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TetrahedronFEMForceField
typedef sofa::component::forcefield::TetrahedronFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TetrahedronFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangleBendingSprings
typedef sofa::component::interactionforcefield::TriangleBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > TriangleBendingSprings2f;
typedef sofa::component::interactionforcefield::TriangleBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangleBendingSprings3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangleFEMForceField
typedef sofa::component::forcefield::TriangleFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangleFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularAnisotropicFEMForceField
typedef sofa::component::forcefield::TriangularAnisotropicFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangularAnisotropicFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularBendingSprings
typedef sofa::component::forcefield::TriangularBendingSprings<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangularBendingSprings3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularBiquadraticSpringsForceField
typedef sofa::component::forcefield::TriangularBiquadraticSpringsForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangularBiquadraticSpringsForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularFEMForceField
typedef sofa::component::forcefield::TriangularFEMForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangularFEMForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularQuadraticSpringsForceField
typedef sofa::component::forcefield::TriangularQuadraticSpringsForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangularQuadraticSpringsForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for TriangularTensorMassForceField
typedef sofa::component::forcefield::TriangularTensorMassForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > TriangularTensorMassForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for VaccumSphereForceField
typedef sofa::component::forcefield::VaccumSphereForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > VaccumSphereForceField1f;
typedef sofa::component::forcefield::VaccumSphereForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > VaccumSphereForceField2f;
typedef sofa::component::forcefield::VaccumSphereForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > VaccumSphereForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for VectorSpringForceField
typedef sofa::component::interactionforcefield::VectorSpringForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > VectorSpringForceField3f;



//---------------------------------------------------------------------------------------------
//Typedef for WashingMachineForceField
typedef sofa::component::forcefield::WashingMachineForceField<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > WashingMachineForceField3f;





#ifdef SOFA_FLOAT
typedef BeamFEMForceFieldRigid3f BeamFEMForceFieldRigid3;
typedef BoxStiffSpringForceField1f BoxStiffSpringForceField1;
typedef BoxStiffSpringForceField2f BoxStiffSpringForceField2;
typedef BoxStiffSpringForceField3f BoxStiffSpringForceField3;
typedef BoxStiffSpringForceField6f BoxStiffSpringForceField6;
typedef BuoyantForceField3f BuoyantForceField3;
typedef ConicalForceField3f ConicalForceField3;
typedef ConstantForceFieldRigid2f ConstantForceFieldRigid2;
typedef ConstantForceFieldRigid3f ConstantForceFieldRigid3;
typedef ConstantForceField1f ConstantForceField1;
typedef ConstantForceField2f ConstantForceField2;
typedef ConstantForceField3f ConstantForceField3;
typedef ConstantForceField6f ConstantForceField6;
typedef DistanceGridForceField3f DistanceGridForceField3;
typedef EllipsoidForceField1f EllipsoidForceField1;
typedef EllipsoidForceField2f EllipsoidForceField2;
typedef EllipsoidForceField3f EllipsoidForceField3;
typedef FastTetrahedralCorotationalForceField3f FastTetrahedralCorotationalForceField3;
typedef FrameSpringForceFieldRigid3f FrameSpringForceFieldRigid3;
typedef GearSpringForceFieldRigid3f GearSpringForceFieldRigid3;
typedef HexahedralFEMForceField3f HexahedralFEMForceField3;
typedef HexahedronFEMForceField3f HexahedronFEMForceField3;
typedef InteractionEllipsoidForceField3f_Rigid3f InteractionEllipsoidForceField3_Rigid3;
typedef JointSpringForceFieldRigid3f JointSpringForceFieldRigid3;
typedef LennardJonesForceField3f LennardJonesForceField3;
typedef LineBendingSprings2f LineBendingSprings2;
typedef LineBendingSprings3f LineBendingSprings3;
typedef LinearForceFieldRigid3f LinearForceFieldRigid3;
typedef LinearForceField1f LinearForceField1;
typedef LinearForceField2f LinearForceField2;
typedef LinearForceField3f LinearForceField3;
typedef LinearForceField6f LinearForceField6;
typedef MeshSpringForceField1f MeshSpringForceField1;
typedef MeshSpringForceField2f MeshSpringForceField2;
typedef MeshSpringForceField3f MeshSpringForceField3;
typedef ParticlesRepulsionForceField2f ParticlesRepulsionForceField2;
typedef ParticlesRepulsionForceField3f ParticlesRepulsionForceField3;
typedef PenalityContactForceField3f PenalityContactForceField3;
typedef PlaneForceField1f PlaneForceField1;
typedef PlaneForceField2f PlaneForceField2;
typedef PlaneForceField3f PlaneForceField3;
typedef PlaneForceField6f PlaneForceField6;
typedef QuadBendingSprings2f QuadBendingSprings2;
typedef QuadBendingSprings3f QuadBendingSprings3;
typedef QuadularBendingSprings3f QuadularBendingSprings3;
typedef RegularGridSpringForceField1f RegularGridSpringForceField1;
typedef RegularGridSpringForceField2f RegularGridSpringForceField2;
typedef RegularGridSpringForceField3f RegularGridSpringForceField3;
typedef RegularGridSpringForceField6f RegularGridSpringForceField6;
typedef RepulsiveSpringForceField1f RepulsiveSpringForceField1;
typedef RepulsiveSpringForceField2f RepulsiveSpringForceField2;
typedef RepulsiveSpringForceField3f RepulsiveSpringForceField3;
typedef RestShapeSpringsForceFieldRigid3f RestShapeSpringsForceFieldRigid3;
typedef RestShapeSpringsForceField1f RestShapeSpringsForceField1;
typedef RestShapeSpringsForceField3f RestShapeSpringsForceField3;
typedef SPHFluidForceField3f SPHFluidForceField3;
typedef SphereForceField1f SphereForceField1;
typedef SphereForceField2f SphereForceField2;
typedef SphereForceField3f SphereForceField3;
typedef SpringForceField1f SpringForceField1;
typedef SpringForceField2f SpringForceField2;
typedef SpringForceField3f SpringForceField3;
typedef SpringForceField6f SpringForceField6;
typedef StiffSpringForceField1f StiffSpringForceField1;
typedef StiffSpringForceField2f StiffSpringForceField2;
typedef StiffSpringForceField3f StiffSpringForceField3;
typedef StiffSpringForceField6f StiffSpringForceField6;
typedef SurfacePressureForceField3f SurfacePressureForceField3;
typedef TetrahedralCorotationalFEMForceField3f TetrahedralCorotationalFEMForceField3;
typedef TetrahedralTensorMassForceField3f TetrahedralTensorMassForceField3;
typedef TetrahedronFEMForceField3f TetrahedronFEMForceField3;
typedef TriangleBendingSprings2f TriangleBendingSprings2;
typedef TriangleBendingSprings3f TriangleBendingSprings3;
typedef TriangleFEMForceField3f TriangleFEMForceField3;
typedef TriangularAnisotropicFEMForceField3f TriangularAnisotropicFEMForceField3;
typedef TriangularBendingSprings3f TriangularBendingSprings3;
typedef TriangularBiquadraticSpringsForceField3f TriangularBiquadraticSpringsForceField3;
typedef TriangularFEMForceField3f TriangularFEMForceField3;
typedef TriangularQuadraticSpringsForceField3f TriangularQuadraticSpringsForceField3;
typedef TriangularTensorMassForceField3f TriangularTensorMassForceField3;
typedef VaccumSphereForceField1f VaccumSphereForceField1;
typedef VaccumSphereForceField2f VaccumSphereForceField2;
typedef VaccumSphereForceField3f VaccumSphereForceField3;
typedef VectorSpringForceField3f VectorSpringForceField3;
typedef WashingMachineForceField3f WashingMachineForceField3;
#endif

#endif