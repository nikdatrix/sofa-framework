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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CORE_COMPONENTMODEL_COLLISION_BROADPHASEDETECTION_H
#define SOFA_CORE_COMPONENTMODEL_COLLISION_BROADPHASEDETECTION_H

#include <sofa/core/componentmodel/collision/Detection.h>
#include <vector>
#include <algorithm>

namespace sofa
{

namespace core
{

namespace componentmodel
{

namespace collision
{
/**
 * @brief given a set of root collision models, computes potentially colliding pairs.
 */
class BroadPhaseDetection : virtual public Detection
{
public:
	/// Destructor
	virtual ~BroadPhaseDetection() { }
	
	/// Clear all the potentially colliding pairs detected in the previous simulation step
	virtual void beginBroadPhase()
	{
		cmPairs.clear();
	}

	/// Add a new collision model to the set of root collision models managed by this class
	virtual void addCollisionModel(core::CollisionModel *cm) = 0;
	
	/// Add a list of collision models to the set of root collision models managed by this class
	virtual void addCollisionModels(const sofa::helper::vector<core::CollisionModel *> v)
	{
		for (sofa::helper::vector<core::CollisionModel *>::const_iterator it = v.begin(); it<v.end(); it++)
			addCollisionModel(*it);
	}
	
	/// Actions to accomplish when the broadPhase is finished. By default do nothing.
	virtual void endBroadPhase()
	{
	}
	
	/// Get the potentially colliding pairs detected
	sofa::helper::vector<std::pair<core::CollisionModel*, core::CollisionModel*> >& getCollisionModelPairs() { return cmPairs; }

protected:

	/// Potentially colliding pairs
	sofa::helper::vector< std::pair<core::CollisionModel*, core::CollisionModel*> > cmPairs;
	std::map<Instance,sofa::helper::vector< std::pair<core::CollisionModel*, core::CollisionModel*> > > storedCmPairs;

	virtual void changeInstanceBP(Instance inst)
	{
		storedCmPairs[instance].swap(cmPairs);
		cmPairs.swap(storedCmPairs[inst]);
	}
};

} // namespace collision

} // namespace componentmodel

} // namespace core

} // namespace sofa

#endif
