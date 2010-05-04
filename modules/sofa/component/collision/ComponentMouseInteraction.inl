/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_COLLISION_COMPONENTMOUSEINTERACTION_INL
#define SOFA_COMPONENT_COLLISION_COMPONENTMOUSEINTERACTION_INL

#include <sofa/component/collision/ComponentMouseInteraction.h>

#include <sofa/component/container/MechanicalObject.inl>
#include <sofa/component/collision/RayModel.h>
#include <sofa/component/collision/MouseInteractor.inl>
#include <sofa/component/mapping/IdentityMapping.inl>

#include <sofa/simulation/common/InitVisitor.h>

#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/simulation/common/Node.h>

namespace sofa
{

  namespace component
  {

    namespace collision
    {

      using sofa::simulation::Node;
      using sofa::component::collision::BodyPicked;





		template <class DataTypes>
		void TComponentMouseInteraction<DataTypes>::createRayPickObjects(Node* node)
		{
      ComponentMouseInteraction::createRayPickNode(node);
      nodeRayPick->setName(nodeRayPick->getName() + "_" + DataTypes::Name());
			parentNode->addChild(nodeRayPick);

			mouseInSofa = new MouseContainer; mouseInSofa->resize(1);
			mouseInSofa->setName("MousePosition");
			nodeRayPick->addObject(mouseInSofa);

			mouseInteractor = new Interactor;
			mouseInteractor->setName("MouseInteractor");
			nodeRayPick->addObject(mouseInteractor);

			MousePosition *mecha = dynamic_cast<MousePosition*>(node->getMechanicalState());
			mouseMapping = new IdentityMechanicalMapping(mecha, static_cast<MouseContainer*>(mouseInSofa));
			nodeRayPick->addObject(mouseMapping);

			mouseMapping->setMechanical(false);

                        mouseInSofa->init();
                        mouseInteractor->init();                        
                        mouseMapping->init();

			parentNode->removeChild(nodeRayPick);
		}

        template <class DataTypes>
		bool TComponentMouseInteraction<DataTypes>::isCompatible( core::objectmodel::BaseContext *context) const
        {
          return (dynamic_cast<MouseContainer*>(context->getMechanicalState()) != NULL);
        }
    }
  }
}

#endif
