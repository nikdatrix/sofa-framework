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
#include <sofa/helper/system/config.h>
#include <plugins/PhysicsBasedInteractiveModeler/gui/qt/QMouseOperations.h>
#include <sofa/gui/OperationFactory.h>
#include <sofa/gui/qt/SofaMouseManager.h>

#ifndef WIN32
    #define SOFA_EXPORT_DYNAMIC_LIBRARY 
    #define SOFA_IMPORT_DYNAMIC_LIBRARY
    #define SOFA_PIM_API
#else
    #ifdef SOFA_BUILD_PIM
        #define SOFA_PIM_API SOFA_EXPORT_DYNAMIC_LIBRARY
    #else
        #define SOFA_PIM_API SOFA_IMPORT_DYNAMIC_LIBRARY
    #endif
#endif

namespace sofa
{

namespace component
{

using namespace sofa::gui;
using namespace sofa::gui::qt;
using namespace plugins::pim::gui::qt;

	//Here are just several convenient functions to help user to know what contains the plugin

	extern "C" {
                SOFA_PIM_API void initExternalModule();
                SOFA_PIM_API const char* getModuleName();
                SOFA_PIM_API const char* getModuleVersion();
                SOFA_PIM_API const char* getModuleDescription();
                SOFA_PIM_API const char* getModuleComponentList();
	}
	
	void initExternalModule()
	{
		RegisterOperation("Sculpt").add< QSculptOperation >();
                SofaMouseManager::getInstance()->LeftOperationCombo->insertItem(QString(OperationFactory::GetDescription("Sculpt").c_str()));
                SofaMouseManager::getInstance()->MiddleOperationCombo->insertItem(QString(OperationFactory::GetDescription("Sculpt").c_str()));
                SofaMouseManager::getInstance()->RightOperationCombo->insertItem(QString(OperationFactory::GetDescription("Sculpt").c_str()));
                SofaMouseManager::getInstance()->getMapIndexOperation().insert(std::make_pair(SofaMouseManager::getInstance()->getMapIndexOperation().size(), "Sculpt"));
	}

	const char* getModuleName()
	{
		return "Plugin PIM";
	}

	const char* getModuleVersion()
	{
		return "beta 1.0";
	}

	const char* getModuleDescription()
	{
		return "PIM port into SOFA Framework";
	}

	const char* getModuleComponentList()
	{
		return "SculptBodyPerformer, ProgressiveScaling ";
	}


SOFA_LINK_CLASS(ProgressiveScaling)
SOFA_LINK_CLASS(ComputeMeshIntersection)

} 

}
