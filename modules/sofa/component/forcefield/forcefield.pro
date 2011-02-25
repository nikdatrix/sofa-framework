SOFA_DIR = ../../../..
TEMPLATE = lib
TARGET = sofacomponentforcefield

include($${SOFA_DIR}/sofa.cfg)
CONFIG += $$CONFIGLIBRARIES

!contains(CONFIGSTATIC, static) {
	CONFIG -= staticlib
CONFIG += dll
}

DEFINES += SOFA_BUILD_COMPONENT_FORCEFIELD

HEADERS += initForceField.h \
           AspirationForceField.h \
           AspirationForceField.inl \
           BeamFEMForceField.h \
           BeamFEMForceField.inl \
           BuoyantForceField.h \
           BuoyantForceField.inl \
           ConicalForceField.h \
           ConicalForceField.inl \
           ConstantForceField.h \
           ConstantForceField.inl \
           DistanceGridForceField.h \
           DistanceGridForceField.inl \
           EdgePressureForceField.h \
           EdgePressureForceField.inl \
           EllipsoidForceField.h \
           EllipsoidForceField.inl \
           HexahedralFEMForceField.h \
           HexahedralFEMForceField.inl \
           HexahedralFEMForceFieldAndMass.h \
           HexahedralFEMForceFieldAndMass.inl \
           HexahedronFEMForceField.h \
           HexahedronFEMForceField.inl \
           HexahedronFEMForceFieldAndMass.h \
           HexahedronFEMForceFieldAndMass.inl \
           LennardJonesForceField.h \
           LennardJonesForceField.inl \
           LinearForceField.h \
           LinearForceField.inl \
           NonUniformHexahedralFEMForceFieldAndMass.h \
           NonUniformHexahedralFEMForceFieldAndMass.inl \
           NonUniformHexahedronFEMForceFieldAndMass.h \
           NonUniformHexahedronFEMForceFieldAndMass.inl \
           NonUniformHexahedronFEMForceFieldDensity.h \
           NonUniformHexahedronFEMForceFieldDensity.inl \
           OscillatingTorsionPressureForceField.h \
           OscillatingTorsionPressureForceField.inl \
           ParticlesRepulsionForceField.h \
           ParticlesRepulsionForceField.inl \
           PlaneForceField.h \
           PlaneForceField.inl \
           QuadularBendingSprings.h \
           QuadularBendingSprings.inl \
           RestShapeSpringsForceField.h \
           RestShapeSpringsForceField.inl \
           SphereForceField.h \
           SphereForceField.inl \
           SPHFluidForceField.h \
           SPHFluidForceField.inl \
           SurfacePressureForceField.h \
           SurfacePressureForceField.inl \
           TetrahedralCorotationalFEMForceField.h \
           TetrahedralCorotationalFEMForceField.inl \
           TetrahedralTensorMassForceField.h \
           TetrahedralTensorMassForceField.inl \
           TetrahedronFEMForceField.h \
           TetrahedronFEMForceField.inl \
           TrianglePressureForceField.h \
           TrianglePressureForceField.inl \
           TriangularAnisotropicFEMForceField.h \
		   TriangularAnisotropicFEMForceField.inl \
           TriangularBendingSprings.h \
           TriangularBendingSprings.inl \
           TriangularBiquadraticSpringsForceField.h \
           TriangularBiquadraticSpringsForceField.inl \
           TriangleFEMForceField.h \
		   TriangleFEMForceField.inl \
           TriangularFEMForceField.h \
           TriangularFEMForceField.inl \
           TriangularQuadraticSpringsForceField.h \
           TriangularQuadraticSpringsForceField.inl \
           TriangularTensorMassForceField.h \
           TriangularTensorMassForceField.inl \
           VaccumSphereForceField.h \
           VaccumSphereForceField.inl

           
SOURCES += initForceField.cpp \
           AspirationForceField.cpp \
           BeamFEMForceField.cpp \
           BuoyantForceField.cpp \
           ConicalForceField.cpp \
           ConstantForceField.cpp \
           DistanceGridForceField.cpp \
           EdgePressureForceField.cpp \
           EllipsoidForceField.cpp \
           HexahedralFEMForceField.cpp \
           HexahedralFEMForceFieldAndMass.cpp \
           HexahedronFEMForceField.cpp \
           HexahedronFEMForceFieldAndMass.cpp \
           LennardJonesForceField.cpp \
           LinearForceField.cpp \
           NonUniformHexahedralFEMForceFieldAndMass.cpp \
           NonUniformHexahedronFEMForceFieldAndMass.cpp \
           NonUniformHexahedronFEMForceFieldDensity.cpp \
           OscillatingTorsionPressureForceField.cpp \
           ParticlesRepulsionForceField.cpp \
           PlaneForceField.cpp \
           QuadularBendingSprings.cpp \
           RestShapeSpringsForceField.cpp \
           SphereForceField.cpp \
           SPHFluidForceField.cpp \
           SurfacePressureForceField.cpp \
           TetrahedralCorotationalFEMForceField.cpp \
           TetrahedralTensorMassForceField.cpp \
           TetrahedronFEMForceField.cpp \
           TrianglePressureForceField.cpp \
           TriangularAnisotropicFEMForceField.cpp \
           TriangularBendingSprings.cpp \
           TriangularBiquadraticSpringsForceField.cpp \
           TriangleFEMForceField.cpp \
           TriangularFEMForceField.cpp \
           TriangularQuadraticSpringsForceField.cpp \
           TriangularTensorMassForceField.cpp \
           VaccumSphereForceField.cpp

           

contains(DEFINES,SOFA_TEST_FRICTION){

    HEADERS += PenalityContactFrictionForceField.h \
           PenalityContactFrictionForceField.inl

    SOURCES += PenalityContactFrictionForceField.cpp 

}


LIBS += $$SOFA_FRAMEWORK_LIBS
LIBS += -lsofasimulation$$LIBSUFFIX
LIBS += -lsofatree$$LIBSUFFIX
LIBS += -lsofacomponentbase$$LIBSUFFIX
LIBS += $$SOFA_EXT_LIBS 

macx : LIBS += -framework GLUT 

# Make sure there are no cross-dependencies
INCLUDEPATH -= $$SOFA_DIR/applications

exists(forcefield-local.cfg): include(forcefield-local.cfg) 
