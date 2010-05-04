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
#ifndef SOFA_COMPONENT_COLLISION_ATTACHBODYPERFORMER_CPP
#define SOFA_COMPONENT_COLLISION_ATTACHBODYPERFORMER_CPP

#include "AttachBodyPerformer.inl"
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/helper/Factory.inl>
#include <sofa/component/forcefield/JointSpringForceField.h>


using namespace sofa::component::forcefield;
using namespace sofa::core::objectmodel;
namespace sofa
{

  namespace component
  {

    namespace collision
    {

#ifndef SOFA_DOUBLE
      template class SOFA_COMPONENT_COLLISION_API  AttachBodyPerformer<defaulttype::Vec3fTypes>;
      template class SOFA_COMPONENT_COLLISION_API  AttachBodyPerformer<defaulttype::Rigid3fTypes>;
#endif
#ifndef SOFA_FLOAT
      template class SOFA_COMPONENT_COLLISION_API  AttachBodyPerformer<defaulttype::Vec3dTypes>;
      template class SOFA_COMPONENT_COLLISION_API  AttachBodyPerformer<defaulttype::Rigid3dTypes>;
#endif


#ifndef SOFA_DOUBLE
      helper::Creator<InteractionPerformer::InteractionPerformerFactory, AttachBodyPerformer<defaulttype::Vec3fTypes> >  AttachBodyPerformerVec3fClass("AttachBody",true);
      helper::Creator<InteractionPerformer::InteractionPerformerFactory, AttachBodyPerformer<defaulttype::Rigid3fTypes> >  AttachBodyPerformerRigid3fClass("AttachBody",true);
      template <>
      void AttachBodyPerformer<defaulttype::Rigid3fTypes>::start_partial(const BodyPicked& picked)
      {
        core::componentmodel::behavior::MechanicalState<Rigid3fTypes>* mstateCollision=NULL;
       
        double restLength = picked.dist;
        mstateCollision = dynamic_cast< core::componentmodel::behavior::MechanicalState<Rigid3fTypes>*  >(picked.mstate);
        std::string name = "contactMouse";
        sofa::component::forcefield::JointSpring<Rigid3fTypes> spring;
           
        forcefield = new JointSpringForceField<Rigid3fTypes>(dynamic_cast<MouseContainer*>(this->interactor->getMouseContainer()), mstateCollision); 
        JointSpringForceField<Rigid3fTypes>* jointspringforcefield = static_cast<JointSpringForceField<Rigid3fTypes>*>(forcefield);
        jointspringforcefield->setName("Spring-Mouse-Contact");
        
        
        spring.setInitLength(this->interactor->getMouseRayModel()->getRay(0).direction()*restLength);
        spring.setSoftStiffnessTranslation(stiffness);
        jointspringforcefield->addSpring(spring);

        const core::objectmodel::TagSet &tags=mstateCollision->getTags();
        for (core::objectmodel::TagSet::const_iterator it=tags.begin();it!=tags.end();++it)
          jointspringforcefield->addTag(*it);        
        mstateCollision->getContext()->addObject(jointspringforcefield);
        
      }

#endif
#ifndef SOFA_FLOAT
      helper::Creator<InteractionPerformer::InteractionPerformerFactory, AttachBodyPerformer<defaulttype::Vec3dTypes> >  AttachBodyPerformerVec3dClass("AttachBody",true);
      helper::Creator<InteractionPerformer::InteractionPerformerFactory, AttachBodyPerformer<defaulttype::Rigid3dTypes> >  AttachBodyPerformerRigid3dClass("AttachBody",true);

      template <>
      void AttachBodyPerformer<defaulttype::Rigid3dTypes>::start_partial(const BodyPicked& picked)
      {
        core::componentmodel::behavior::MechanicalState<Rigid3dTypes>* mstateCollision=NULL;
         
        double restLength = picked.dist;
        mstateCollision = dynamic_cast< core::componentmodel::behavior::MechanicalState<Rigid3dTypes>*  >(picked.mstate);
        std::string name = "contactMouse";
        sofa::component::forcefield::JointSpring<Rigid3dTypes> spring;
           
        forcefield = new JointSpringForceField<Rigid3dTypes>(dynamic_cast<MouseContainer*>(this->interactor->getMouseContainer()), mstateCollision); 
        JointSpringForceField<Rigid3dTypes>* jointspringforcefield = static_cast<JointSpringForceField<Rigid3dTypes>*>(forcefield);
        jointspringforcefield->setName("Spring-Mouse-Contact");
        
        
        spring.setInitLength(this->interactor->getMouseRayModel()->getRay(0).direction()*restLength);
        spring.setSoftStiffnessTranslation(stiffness);
        jointspringforcefield->addSpring(spring);
        const core::objectmodel::TagSet &tags=mstateCollision->getTags();
        for (core::objectmodel::TagSet::const_iterator it=tags.begin();it!=tags.end();++it) 
          jointspringforcefield->addTag(*it);

        mstateCollision->getContext()->addObject(jointspringforcefield);
        
      }
#endif

   
    }
  }
}
#endif
