#ifndef SOFA_GUI_QT_SOFAGUIQT_H
#define SOFA_GUI_QT_SOFAGUIQT_H

#include <sofa/helper/system/config.h>

#ifdef SOFA_BUILD_SOFAGUIQT
#	define SOFA_SOFAGUIQT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_SOFAGUIQT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#endif 
