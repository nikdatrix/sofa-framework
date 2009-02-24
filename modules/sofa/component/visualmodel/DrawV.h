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
#ifndef SOFA_COMPONENT_VISUALMODEL_DRAWV_H
#define SOFA_COMPONENT_VISUALMODEL_DRAWV_H

#include <string>
#include <sofa/helper/gl/template.h>
#include <sofa/core/VisualModel.h>
#include <sofa/core/componentmodel/behavior/MappedModel.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Vec3Types.h>
#include "OglModel.h" // for ResizeableExtVector

namespace sofa
{

namespace component
{

namespace visualmodel
{

using namespace sofa::defaulttype;

class DrawV : public core::VisualModel, public core::componentmodel::behavior::MappedModel< ExtVectorTypes< Vec<3,GLfloat>, Vec<3,GLfloat> > >
{
public:
    Data<bool> castShadow; ///< True if object cast shadows
    Data<bool> useAlpha; ///< True if velocity displayed using alpha blending
    Data<double> vscale; ///< Scaling of veloity vectors
    
    DrawV();

    void initVisual();
    void updateVisual();

    bool isTransparent();
    float getVScale();
    
    void drawVisual();
    void drawTransparent();
    void drawShadow();

    bool addBBox(double* minBBox, double* maxBBox);
	
    const VecCoord* getX()  const { return &inputX; }
    const VecDeriv* getV()  const { return &inputV; }
    VecCoord* getX()  { return &inputX; }
    VecDeriv* getV()  { return &inputV; }

protected:
    ResizableExtVector<Coord> inputX;
    ResizableExtVector<Deriv> inputV;

    void internalDraw();
};


} // namespace visualmodel

} // namespace component

} // namespace sofa

#endif
