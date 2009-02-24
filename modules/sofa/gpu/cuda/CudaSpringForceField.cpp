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
#include "CudaTypes.h"
#include "CudaSpringForceField.inl"
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace forcefield
{

template class SpringForceField<sofa::gpu::cuda::CudaVec3fTypes>;
template class StiffSpringForceField<sofa::gpu::cuda::CudaVec3fTypes>;
template class MeshSpringForceField<sofa::gpu::cuda::CudaVec3fTypes>;

template class SpringForceField<sofa::gpu::cuda::CudaVec3f1Types>;
template class StiffSpringForceField<sofa::gpu::cuda::CudaVec3f1Types>;
template class MeshSpringForceField<sofa::gpu::cuda::CudaVec3f1Types>;


} // namespace forcefield

} // namespace component

namespace gpu
{

namespace cuda
{

SOFA_DECL_CLASS(CudaSpringForceField)

int SpringForceFieldCudaClass = core::RegisterObject("Supports GPU-side computations using CUDA")
.add< component::forcefield::SpringForceField<CudaVec3fTypes> >()
.add< component::forcefield::SpringForceField<CudaVec3f1Types> >()
;

int StiffSpringForceFieldCudaClass = core::RegisterObject("Supports GPU-side computations using CUDA")
.add< component::forcefield::StiffSpringForceField<CudaVec3fTypes> >()
.add< component::forcefield::StiffSpringForceField<CudaVec3f1Types> >()
;

int MeshSpringForceFieldCudaClass = core::RegisterObject("Supports GPU-side computations using CUDA")
.add< component::forcefield::MeshSpringForceField<CudaVec3fTypes> >()
.add< component::forcefield::MeshSpringForceField<CudaVec3f1Types> >()
;

int TriangleBendingSpringsCudaClass = core::RegisterObject("Supports GPU-side computations using CUDA")
.add< component::forcefield::TriangleBendingSprings<CudaVec3fTypes> >()
.add< component::forcefield::TriangleBendingSprings<CudaVec3f1Types> >()
;

int QuadBendingSpringsCudaClass = core::RegisterObject("Supports GPU-side computations using CUDA")
.add< component::forcefield::QuadBendingSprings<CudaVec3fTypes> >()
.add< component::forcefield::QuadBendingSprings<CudaVec3f1Types> >()
;


} // namespace cuda

} // namespace gpu

} // namespace sofa
