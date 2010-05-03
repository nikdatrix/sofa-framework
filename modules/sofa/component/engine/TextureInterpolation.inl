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
#ifndef SOFA_COMPONENT_ENGINE_TEXTUREINTERPOLATION_INL
#define SOFA_COMPONENT_ENGINE_TEXTUREINTERPOLATION_INL

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/component/engine/TextureInterpolation.h>
#include <sofa/helper/system/glut.h>
#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/common/Simulation.h>

namespace sofa
{

namespace component
{

namespace engine
{

  using namespace sofa::helper;
  using namespace sofa::defaulttype;
  using namespace core::objectmodel;

  template <class DataTypes>
  TextureInterpolation<DataTypes>::TextureInterpolation()
    : _inputField (initData (&_inputField, "input_states", "input array of state values."))
    , _inputCoords (initData (&_inputCoords, "input_coordinates", "input array of coordinates values."))
    , _outputCoord (initData (&_outputCoord, "output_coordinates", "output array of texture coordinates."))
    , _minVal (initData (&_minVal, (Real)0, "min_value", "minimum value of state value for interpolation."))
    , _maxVal (initData (&_maxVal, (Real)0, "max_value", "maximum value of state value for interpolation."))
    , _changeScale (initData (&_changeScale, false, "manual_scale", "compute texture interpolation on manually scale defined above."))
    ,_viewPotentiels (initData (&_viewPotentiels, (bool) false, "debugViewStates", "Debug: view state values."))
    ,_ViewIndicesScale (initData(&_ViewIndicesScale, (float) 0.0001, "debugView_indiceScale", "Debug : scale of state values displayed."))
  {
  }

  
  template <class DataTypes>
  void TextureInterpolation<DataTypes>::init()
  {
      addInput(&_inputField);
      addOutput(&_outputCoord);
      setDirtyValue();
  }

  
  template <class DataTypes>
  void TextureInterpolation<DataTypes>::reinit()
  {
    update();
  }


  template <class DataTypes>
  void TextureInterpolation<DataTypes>::update()
  {
    cleanDirty();

    const sofa::helper::vector <Coord>& inputs = _inputField.getValue();
    ResizableExtVector2D& outputs = *(_outputCoord.beginEdit());
    outputs.resize (inputs.size());

    Real minVal = 0;
    Real maxVal = 0;
        
    // Check min and max values:
    if(_changeScale.getValue())
    {
      if( _minVal.getValue() < _maxVal.getValue() )
      {
	minVal = _minVal.getValue();
	maxVal = _maxVal.getValue();
      }
      else
	std::cerr << "Error: in scale for TextureInterpolation, min_value is not smaller than max_value." << std::endl;
    } else {
      Coord min_tmp = *(std::min_element (inputs.begin(), inputs.end())); 
      Coord max_tmp = *(std::max_element (inputs.begin(), inputs.end()));
      
      minVal = min_tmp[0];
      maxVal = max_tmp[0];
    }
    
    Real scale = 1/(maxVal - minVal);
    
    for (unsigned int i=0; i<inputs.size(); ++i)
    {
      outputs[i][0] = (inputs[i][0] - minVal)*scale;
      outputs[i][1] = 0.0;
    }

    _outputCoord.endEdit();
  }
  
  
  template <class DataTypes>
  void TextureInterpolation<DataTypes>::draw()
  {
    // to force update. getX() must have call to endEdit() 
    _outputCoord.getValue();

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
     
    if (_viewPotentiels.getValue())
    {
      Mat<4,4, GLfloat> modelviewM;
      Vec<3, SReal> sceneMinBBox, sceneMaxBBox;

      const VecCoord& potentiels = _inputField.getValue();
      const VecCoord3D& coords = _inputCoords.getValue();

      if(potentiels.size() != coords.size())
      {
	std::cout << "Error: vector sizes differ" << std::endl;
	return;
      }
      unsigned int nbr = potentiels.size();
      
            
      sofa::simulation::Node* context = dynamic_cast<sofa::simulation::Node*>(this->getContext());
      glColor3f(1.0,1.0,1.0);
      glDisable(GL_LIGHTING);
      sofa::simulation::getSimulation()->computeBBox((sofa::simulation::Node*)context, sceneMinBBox.ptr(), sceneMaxBBox.ptr());
      
       // Recompute, in case Box has moved.
      float scale = (sceneMaxBBox - sceneMinBBox).norm() * _ViewIndicesScale.getValue();
      
      for (unsigned int i = 0; i<nbr; i++)
      {
	std::ostringstream oss;
	oss << (float)potentiels[i][0];
	std::string tmp = oss.str();
	const char* s = tmp.c_str();
	glPushMatrix();

	glTranslatef(coords[i][0], coords[i][1], coords[i][2]);
	glScalef(scale,scale,scale);

	// Makes text always face the viewer by removing the scene rotation
	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelviewM.ptr() );
	modelviewM.transpose();

	Vec3d temp(coords[i][0], coords[i][1], coords[i][2]);
	temp = modelviewM.transform(temp);
	
	//glLoadMatrixf(modelview);
	glLoadIdentity();
	
	glTranslatef(temp[0], temp[1], temp[2]);
	glScalef(scale,scale,scale);
	
	while(*s){
	  glutStrokeCharacter(GLUT_STROKE_ROMAN, *s);
	  s++;
	}
	
	glPopMatrix();
	
      }
    }
    
      
      
  }


} // namespace engine

} // namespace component

} // namespace sofa

#endif
