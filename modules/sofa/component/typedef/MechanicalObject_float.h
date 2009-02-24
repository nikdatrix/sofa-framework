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
#ifndef SOFA_TYPEDEF_MECHANICAL_FLOAT_H
#define SOFA_TYPEDEF_MECHANICAL_FLOAT_H

#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/LaparoscopicRigidTypes.h>
#include <sofa/component/MechanicalObject.h>

typedef sofa::defaulttype::Vec1fTypes   Particles1f;
typedef Particles1f::VecDeriv           VecDeriv1f;
typedef Particles1f::VecCoord           VecCoord1f;
typedef Particles1f::Deriv              Deriv1f;
typedef Particles1f::Coord              Coord1f;
typedef sofa::defaulttype::Vec2fTypes   Particles2f;
typedef Particles2f::VecDeriv           VecDeriv2f;
typedef Particles2f::VecCoord           VecCoord2f;
typedef Particles2f::Deriv              Deriv2f;
typedef Particles2f::Coord              Coord2f;
typedef sofa::defaulttype::Vec3fTypes   Particles3f;
typedef Particles3f::VecDeriv           VecDeriv3f;
typedef Particles3f::VecCoord           VecCoord3f;
typedef Particles3f::Deriv              Deriv3f;
typedef Particles3f::Coord              Coord3f;
typedef sofa::defaulttype::Vec6fTypes   Particles6f;
typedef Particles6f::VecDeriv           VecDeriv6f;
typedef Particles6f::VecCoord           VecCoord6f;
typedef Particles6f::Deriv              Deriv6f;
typedef Particles6f::Coord              Coord6f;

typedef sofa::defaulttype::Rigid2fTypes Rigid2f;
typedef Rigid2f::VecDeriv               VecDerivRigid2f;
typedef Rigid2f::VecCoord               VecCoordRigid2f;
typedef Rigid2f::Deriv                  DerivRigid2f;
typedef Rigid2f::Coord                  CoordRigid2f;
typedef sofa::defaulttype::Rigid3fTypes Rigid3f;
typedef Rigid3f::VecDeriv               VecDerivRigid3f;
typedef Rigid3f::VecCoord               VecCoordRigid3f;
typedef Rigid3f::Quat                   Quat3f;
typedef Rigid3f::Deriv                  DerivRigid3f;
typedef Rigid3f::Coord                  CoordRigid3f;

//Mechanical Object
//---------------------
//Deformable
typedef sofa::component::MechanicalObject<sofa::defaulttype::Vec1fTypes> MechanicalObject1f;
typedef sofa::component::MechanicalObject<sofa::defaulttype::Vec2fTypes> MechanicalObject2f;
typedef sofa::component::MechanicalObject<sofa::defaulttype::Vec3fTypes> MechanicalObject3f;
typedef sofa::component::MechanicalObject<sofa::defaulttype::Vec6fTypes> MechanicalObject6f;
//---------------------
//Rigid
typedef sofa::component::MechanicalObject<sofa::defaulttype::Rigid2fTypes> MechanicalObjectRigid2f;
typedef sofa::component::MechanicalObject<sofa::defaulttype::Rigid3fTypes> MechanicalObjectRigid3f;
//---------------------
//Laparoscopic
#ifdef SOFA_FLOAT
typedef sofa::component::MechanicalObject<sofa::defaulttype::LaparoscopicRigid3Types> MechanicalObjectLaparoscopicRigid3;
#endif

#ifdef SOFA_FLOAT
typedef MechanicalObject1f MechanicalObject1;
typedef MechanicalObject2f MechanicalObject2;
typedef MechanicalObject3f MechanicalObject3;
typedef MechanicalObject6f MechanicalObject6;

typedef MechanicalObjectRigid2f MechanicalObjectRigid2;
typedef MechanicalObjectRigid3f MechanicalObjectRigid3;


typedef Particles1f          Particles1;    
typedef VecDeriv1f	     VecDeriv1;	
typedef VecCoord1f	     VecCoord1;	
typedef Deriv1f	     	     Deriv1;	
typedef Coord1f	     	     Coord1;	
typedef Particles2f	     Particles2;	
typedef VecDeriv2f	     VecDeriv2;	
typedef VecCoord2f	     VecCoord2;	
typedef Deriv2f	     	     Deriv2;	
typedef Coord2f	     	     Coord2;	
typedef Particles3f	     Particles3;	
typedef VecDeriv3f	     VecDeriv3;	
typedef VecCoord3f	     VecCoord3;	
typedef Deriv3f	     	     Deriv3;	
typedef Coord3f	     	     Coord3;	
typedef Particles6f	     Particles6;	
typedef VecDeriv6f	     VecDeriv6;	
typedef VecCoord6f	     VecCoord6;	
typedef Deriv6f	     	     Deriv6;	
typedef Coord6f	     	     Coord6;	
 
typedef Rigid2f	     	     Rigid2;	
typedef VecDerivRigid2f      VecDerivRigid2;
typedef VecCoordRigid2f      VecCoordRigid2;
typedef DerivRigid2f	     DerivRigid2;	
typedef CoordRigid2f	     CoordRigid2;	
typedef Rigid3f	     	     Rigid3;	
typedef VecDerivRigid3f      VecDerivRigid3;
typedef VecCoordRigid3f      VecCoordRigid3;
typedef Quat3f		     Quat3;		
typedef DerivRigid3f	     DerivRigid3;	
typedef CoordRigid3f	     CoordRigid3;

#endif

#endif
