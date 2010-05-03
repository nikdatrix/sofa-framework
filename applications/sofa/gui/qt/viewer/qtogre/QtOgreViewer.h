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
#ifndef __sofaOgreWidget_H__
#define __sofaOgreWidget_H__

#include <stdlib.h>

#include <sofa/gui/qt/viewer/qtogre/DotSceneLoader.h>
#include <sofa/gui/qt/viewer/SofaViewer.h>
#include <sofa/gui/qt/viewer/qtogre/QOgreLightWidget.h>
#include "../../WFloatLineEdit.h"
#include <sofa/helper/gl/Capture.h>
#include <sofa/helper/system/FileRepository.h>
#include <sofa/helper/system/SetDirectory.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/simulation/common/Simulation.h>


#ifdef SOFA_QT4
#include <QPaintEvent>
#include <QWidget>
#include <Q3GroupBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGLWidget>
#else
#include <qlayout.h>
#include <qgroupbox.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qgl.h> 
typedef QGroupBox Q3GroupBox;
#endif

#include <Ogre.h>



#if defined(SOFA_GPU_CUDA)
#include <sofa/gpu/cuda/mycuda.h>
using namespace sofa::gpu::cuda;
#endif


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
std::string macBundlePath();
#endif


//*********************************************************************************//
// Widget with Ogre embedded
//*********************************************************************************//

namespace sofa 
{

  namespace gui
  {  

    namespace qt
    {

      namespace viewer
      {

	namespace qtogre
	{


using namespace sofa::helper::system::thread;
using namespace sofa::simulation;

	  class QtOgreViewer : public QGLWidget, public sofa::gui::qt::viewer::SofaViewer
	    {
	      Q_OBJECT
		public:

	      std::string sceneName;


	      /// Activate this class of viewer.
	      /// This method is called before the viewer is actually created
	      /// and can be used to register classes associated with in the the ObjectFactory.
	      static int EnableViewer();

	      /// Disable this class of viewer.
	      /// This method is called after the viewer is destroyed
	      /// and can be used to unregister classes associated with in the the ObjectFactory.
	      static int DisableViewer();

	      QtOgreViewer( QWidget *parent=0, const char *name=0 );
	      ~QtOgreViewer();

	      QWidget* getQWidget() { return (QWidget*)this; }

	      bool ready(){return _waitForRender;};
	      void showEntireScene(void);


	      void setScene(sofa::simulation::Node* scene, const char* filename, bool keepParams=false);

	      void setup(void);
	      void setupView(void);          //Creation of the first window of visualization
	      QString helpString();

	      void removeViewerTab(QTabWidget *t);
	      void configureViewerTab(QTabWidget *t);

	      void moveRayPickInteractor(int eventX, int eventY);

	      void setBackgroundColour(float r, float g, float b);
	    private:
				
	      Ogre::String mResourcePath;
	      //------------------------------------------
	      //Setup
	      Ogre::RenderWindow* mRenderWindow;
	      Ogre::SceneManager* mSceneMgr;
	      //  SofaListener* mFrameListener;
	      Ogre::Camera* mCamera;
	      Ogre::Viewport* mVp;
	      Ogre::Root* mRoot;
	      Ogre::ShadowTechnique shadow;
	      ///////
	      bool _mouseInteractorMoving;
	      int _mouseInteractorSavedPosX;
	      int _mouseInteractorSavedPosY;
	      unsigned int number_visualModels;
	      //sofa::helper::gl::Capture capture;

	      void setupResources(void);     //Ogre Initialization 

	      void setupConfiguration(void); //Graphic configuration


	      void loadResources(void)       //Launch the Resource Manager
	      {
		// Initialise, parse scripts etc
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	      }

	      void createScene(void);        

	      //******************************Configuration Panel for Ogre***********************************
	      virtual bool configure(void)
	      {
		// Show the configuration dialog and initialise the system
		// You can skip this and use root.restoreConfig() to load configuration
		// settings if you were sure there are valid ones saved in ogre.cfg
		std::cerr<<"Show Dialog " << mRoot<<"\n";
		if(mRoot->showConfigDialog())
		  {
		    // Custom option - to use PlaneOptimalShadowCameraSetup we must have
		    // double-precision. Thus, set the D3D floating point mode if present, 
		    // no matter what was chosen
		    Ogre::ConfigOptionMap& optMap = mRoot->getRenderSystem()->getConfigOptions();
		    Ogre::ConfigOptionMap::iterator i = optMap.find("Floating-point mode");
		    if (i != optMap.end())
		      {
			if (i->second.currentValue != "Consistent")
			  {
			    i->second.currentValue = "Consistent";
			    Ogre::LogManager::getSingleton().logMessage("ExampleApplication: overriding "
									"D3D floating point mode to 'Consistent' to ensure precision "
									"for numerical computations");
			  }
		      }
		    // If returned true, user clicked OK so initialise

		    mRoot->initialise(false, "SOFA - OGRE");
		    return true;
		  }
		else
		  {
		    return false;
		  }
	      }


	      void addDirLight(std::string lightName=std::string());
	      void addPointLight(std::string lightName=std::string());
	      void addSpotLight(std::string lightName=std::string());
	    protected:
	      Ogre::ManualObject* drawUtility;
	      Ogre::MaterialPtr   drawMaterial;
	      ctime_t _beginTime;

	      bool m_mouseLeftPressed;
	      bool m_mouseRightPressed;
	      bool m_mouseMiddlePressed;
	      QPoint m_mousePressPos;
	      QPoint m_mousePos;
	      bool pickDone;

	      std::string sceneFile;
	      //Tab in the GUI containing the interface to configure the lights
	      QWidget  *tabLights;

	      //Viewer Tab Widget
	      QPushButton *saveLightsButton;
	      WFloatLineEdit *ambient[3];
	      //Lights	     
	      QWidget  *dirLight; 
	      QSpinBox *numDirLight;
	      std::vector< QOgreDirectionalLightWidget *> dirLightOgreWidget;
	      QWidget  *pointLight;
	      QSpinBox *numPointLight;
	      std::vector< QOgrePointLightWidget       *> pointLightOgreWidget;
	      QWidget  *spotLight;
	      QSpinBox *numSpotLight;
	      std::vector< QOgreSpotLightWidget        *> spotLightOgreWidget;
	      bool needUpdateParameters;
	      


	      Ogre::Vector3 m_mTranslateVector;
	      Ogre::Radian m_mRotX, m_mRotY;
	      Ogre::Real m_mMoveSpeed;
	      Ogre::Degree m_mRotateSpeed;
	      Ogre::SceneNode* zeroNode;
	      bool _waitForRender;
	      float _factorWheel;

	      void resize();	      	      

	      bool updateInteractor( QMouseEvent * e );	
				
	      void updateIntern();
 	      virtual void paintEvent(QPaintEvent*);				 
 	      virtual void resizeEvent(QResizeEvent*);			 
	      virtual void timerEvent(QTimerEvent * event){Q_UNUSED(event);updateIntern();}

	      virtual void keyPressEvent ( QKeyEvent * e );
	      virtual void mousePressEvent(QMouseEvent* evt);
	      virtual void mouseReleaseEvent(QMouseEvent* evt);
	      virtual void mouseMoveEvent(QMouseEvent* evt);
	      virtual void wheelEvent(QWheelEvent* evt); 
			
	      void moveCamera(void)
	      {

		mCamera->yaw(m_mRotX);
		mCamera->pitch(m_mRotY);

		//Reset to zero
		m_mRotX = m_mRotY = Ogre::Degree(0);
		m_mTranslateVector = Ogre::Vector3::ZERO;
	      }


	      public slots:	      
	      virtual void update(){
		updateIntern();
	      };
	      
	      void updateViewerParameters();

	      void resizeDirLight(int v);
	      void resizePointLight(int v);
	      void resizeSpotLight(int v);

	      void saveLights();

	      virtual void resetView();
	      virtual void saveView();  
	      virtual void setSizeW(int); 
	      virtual void setSizeH(int);

	    signals:
	      void redrawn(); 
	      void resizeW( int ); 
	      void resizeH( int ); 
	      void quit();

	    };
	} //qtogre
      } //viewer
    } //qt
  } //gui
} //sofa


#endif

