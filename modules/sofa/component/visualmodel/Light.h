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
#ifndef SOFA_COMPONENT_LIGHT
#define SOFA_COMPONENT_LIGHT

#include <sofa/core/VisualModel.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/helper/gl/template.h>
#include <sofa/core/VisualModel.h>
namespace sofa
{

namespace component
{

namespace visualmodel
{

/**
 *  \brief Utility to cast Light into a Opengl scene.
 *
 *  This class must be used in a scene with one LightManager object.
 *  This abstract class defines lights (i.e basically id and color)
 *  The inherited lights are:
 *   - Directional light (direction);
 *   - Positional light (position);
 *   - Spot light (position, direction, cutoff...).
 *
 */

using sofa::defaulttype::Vector3;

class Light : public virtual sofa::core::VisualModel {
protected:
	Data<Vector3> color;
	GLint lightID;

public:

	Light();
	virtual ~Light();

	void setID(const GLint& id);

	virtual void initVisual() ;
	void init();
	virtual void drawLight();
	void draw() { } ;
	virtual void reinit();
	void update() {} ;
};

class DirectionalLight : public Light {
private:
	Data<Vector3> direction;

public:

	DirectionalLight();
	virtual ~DirectionalLight();
	virtual void initVisual() ;
	virtual void drawLight();
	virtual void reinit();


};

class PositionalLight : public Light {
protected:
	Data<Vector3> position;
	Data<float> attenuation;

public:

	PositionalLight();
	virtual ~PositionalLight();
	virtual void initVisual() ;
	virtual void drawLight();
	virtual void reinit();

};

class SpotLight : public PositionalLight {
protected:
	Data<Vector3> direction;
	Data<float> cutoff;
	Data<float> exponent;

public:
	SpotLight();
	virtual ~SpotLight();
	virtual void initVisual() ;
	virtual void drawLight();
	virtual void reinit();


};

} //namespace visualmodel

} //namespace component

} //namespace sofa

#endif //SOFA_COMPONENT_LIGHT
