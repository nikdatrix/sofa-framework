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
#ifndef SOFA_COMPONENT_BOXCONSTANTFORCEFIELD_H
#define SOFA_COMPONENT_BOXCONSTANTFORCEFIELD_H


#include <sofa/component/forcefield/ConstantForceField.h>
#include <sofa/defaulttype/Vec.h>

namespace sofa
{

	namespace component
	{

		namespace forcefield
		{
			
			/**
			apply a constant force (_force) to all DOFs contained in the box (_box)
			*/
			template<class DataTypes>
					class BoxConstantForceField : public ConstantForceField<DataTypes>
			{
				public:
					
					typedef typename DataTypes::Real Real;
					typedef typename DataTypes::Deriv Deriv;
					typedef defaulttype::Vec<6,Real> Vec6;
					
					Data<Vec6> _box;
					Data< Deriv > _force;
					
					BoxConstantForceField();
					
					virtual void init();
					
			};
			
			
		}	
	}
}


#endif

