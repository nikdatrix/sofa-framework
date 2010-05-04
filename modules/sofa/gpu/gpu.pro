SOFA_DIR=../../..
TEMPLATE = subdirs
include($${SOFA_DIR}/sofa.cfg)

contains (DEFINES, SOFA_GPU_CUDA) {
	SUBDIRS += cuda
}

contains (DEFINES, SOFA_GPU_OPENCL) {
	SUBDIRS += opencl
}

