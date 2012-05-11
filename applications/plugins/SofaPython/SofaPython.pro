load(sofa/pre)
defineAsPlugin(SofaPython)

TARGET = SofaPython

DEFINES += SOFA_BUILD_SOFAPYTHON

unix:macx {
    #QMAKE_LFLAGS_SHLIB *= -Wl,--no-undefined
    #python
    LIBS += -F/opt/local/Library/Frameworks/ -framework Python
    INCLUDEPATH += /opt/local/Library/Frameworks/Python.framework/Headers

    #SofaGUI
    LIBS += -lsofaguimain
}

unix:!macx {
    #python
    INCLUDEPATH *= $$system(python-config --includes | sed -e s/\\ -I/\\ /g -e s/^-I//g)
    LIBS *= $$system(python-config --libs)
}

SOURCES = initSofaPython.cpp \
    ScriptController.cpp \
    PythonScriptController.cpp \
    PythonEnvironment.cpp \
    Binding.cpp \
    Binding_SofaModule.cpp \
    Binding_Base.cpp \
    Binding_BaseData.cpp \
    Binding_Context.cpp \
    Binding_BaseContext.cpp \
    Binding_Node.cpp \
    Binding_GNode.cpp \
    Binding_Vec3.cpp \
    Binding_BaseObjectDescription.cpp

HEADERS = initSofaPython.h \
    ScriptController.h \
    PythonScriptController.h \
    PythonMacros.h \
    PythonEnvironment.h \
    Binding.h \
    Binding_Base.h \
    Binding_SofaModule.h \
    Binding_BaseData.h \
    Binding_Node.h \
    Binding_GNode.h \
    Binding_Context.h \
    Binding_BaseContext.h \
    Binding_Vec3.h \
    Binding_BaseObjectDescription.h

README_FILE = SofaPython.txt

#TODO: add an install target for README files

unix : QMAKE_POST_LINK = cp $$SRC_DIR/$$README_FILE $$LIB_DESTDIR 
win32 : QMAKE_POST_LINK = copy \"$$toWindowsPath($$SRC_DIR/$$README_FILE)\" \"$$LIB_DESTDIR\"

load(sofa/post)
