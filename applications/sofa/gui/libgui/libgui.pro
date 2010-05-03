# Target is a library: sofagui

SOFA_DIR = ../../../..
TEMPLATE = lib
include($${SOFA_DIR}/sofa.cfg)

DEFINES += SOFA_BUILD_SOFAGUI

TARGET = sofagui$$LIBSUFFIX
CONFIG += $$CONFIGLIBRARIES
CONFIG -= staticlib
CONFIG += dll

LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += $$SOFA_MODULES_LIBS
LIBS += $$SOFA_COMPONENTS_LIBS
 


SOURCES = \
	    ../SofaGUI.cpp \
	    ../BatchGUI.cpp \
            ../MouseOperations.cpp \
            ../PickHandler.cpp

HEADERS = \
	    ../SofaGUI.h \
	    ../BatchGUI.h \
            ../OperationFactory.h \
            ../MouseOperations.h \
            ../PickHandler.h

