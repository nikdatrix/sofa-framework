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
#ifndef SOFA_SIMULATION_VISITORSCHEDULER_H
#define SOFA_SIMULATION_VISITORSCHEDULER_H

#include <sofa/core/objectmodel/BaseObject.h>
#include "ClassSystem.h"

namespace sofa {

namespace simulation
{


class Visitor;

/// Abstract class defining custom schedule of action execution through the graph.
class VisitorScheduler : public virtual core::objectmodel::BaseObject
{
public:
	virtual ~VisitorScheduler() {}
	
	virtual void executeVisitor(simulation::Node* node, Visitor* act) = 0;
	
	/// Specify whether this scheduler is multi-threaded.
	virtual bool isMultiThreaded() const { return false; }

protected:

	/// Execute the given action recursively
	void doExecuteVisitor(simulation::Node* node, Visitor* act);
};

} // namespace simulation

} // namespace sofa

#endif
