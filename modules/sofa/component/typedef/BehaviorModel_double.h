/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2010 MGH, INRIA, USTL, UJF, CNRS                    *
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


#ifndef SOFA_TYPEDEF_BehaviorModel_double_H
#define SOFA_TYPEDEF_BehaviorModel_double_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>


#ifdef SOFA_GPU_CUDA
#include <sofa/gpu/cuda/CudaTypesBase.h>
#include <sofa/gpu/cuda/CudaTypes.h>
#endif
#ifdef SOFA_GPU_OPENCL
#include <sofa/gpu/opencl/OpenCLTypes.h>
#endif


#include <sofa/component/collision/MouseInteractor.h>
#include <sofa/component/forcefield/VectorField.h>



//---------------------------------------------------------------------------------------------
//Typedef for MouseInteractor
typedef  sofa::component::collision::MouseInteractor< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > MouseInteractor3d;
typedef  sofa::component::collision::MouseInteractor< sofa::defaulttype::StdRigidTypes<3,double> > MouseInteractorRigid3d;



//---------------------------------------------------------------------------------------------
//Typedef for VectorField
typedef  sofa::component::forcefield::VectorField< sofa::defaulttype::StdVectorTypes< sofa::defaulttype::Vec<3,double>, sofa::defaulttype::Vec<3,double>,double> > VectorField3d;





#ifndef SOFA_FLOAT
typedef MouseInteractor3d MouseInteractor3;
typedef MouseInteractorRigid3d MouseInteractorRigid3;
typedef VectorField3d VectorField3;
#endif

#endif
