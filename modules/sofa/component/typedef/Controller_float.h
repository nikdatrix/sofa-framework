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


#ifndef SOFA_TYPEDEF_Controller_float_H
#define SOFA_TYPEDEF_Controller_float_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>



#include <sofa/component/controller/EdgeSetController.h>
#include <sofa/component/controller/MechanicalStateController.h>



//---------------------------------------------------------------------------------------------
//Typedef for EdgeSetController
typedef sofa::component::controller::EdgeSetController<sofa::defaulttype::StdRigidTypes<3, float> > EdgeSetControllerRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for MechanicalStateController
typedef sofa::component::controller::MechanicalStateController<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > MechanicalStateController2f;
typedef sofa::component::controller::MechanicalStateController<sofa::defaulttype::StdRigidTypes<3, float> > MechanicalStateControllerRigid3f;





#ifdef SOFA_FLOAT
typedef EdgeSetControllerRigid3f EdgeSetControllerRigid3;
typedef MechanicalStateController2f MechanicalStateController2;
typedef MechanicalStateControllerRigid3f MechanicalStateControllerRigid3;
#endif

#endif
