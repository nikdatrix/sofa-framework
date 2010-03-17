#ifndef SOFA_GPU_OPENCLMEMORYMANAGER_H
#define SOFA_GPU_OPENCLMEMORYMANAGER_H

#include "myopencl.h"
#include <sofa/helper/MemoryManager.h>
#include <iostream>
#include "stdlib.h"

namespace sofa
{

namespace gpu
{

namespace opencl
{

/**
  * OpenCLMemoryManager
  * OpenCL and multi-GPU version of MemoryManager
  */
template <class T>
class OpenCLMemoryManager: public sofa::helper::MemoryManager<T>
{
public :
    typedef T* host_pointer;
    typedef void* device_pointer; //device_pointer = cl_mem
    typedef GLuint gl_buffer;

    enum { MAX_DEVICES = 8 };
    enum { BSIZE = 32 };
    enum { SUPPORT_GL_BUFFER = 1 };

    static int numDevices()
    {
        return 0;
    }

    static void hostAlloc(void ** hPointer,int n)
    {
        *hPointer = (host_pointer) malloc(n);
    }

    static void memsetHost(host_pointer hPointer, int value,size_t n)
    {
        memset((void*) hPointer, value, n);
    }

    static void hostFree(const host_pointer hSrcPointer)
    {
        free(hSrcPointer);
    }


    static void deviceAlloc(int d,device_pointer* dPointer, int n)
    {
        myopenclCreateBuffer(d,dPointer,n);
    }

    static void deviceFree(int d,const device_pointer dSrcPointer)
    {
        myopenclReleaseBuffer(d,dSrcPointer);
    }

    static void memcpyHostToDevice(int d, device_pointer dDestPointer, const host_pointer hSrcPointer, size_t n)
    {
        myopenclEnqueueWriteBuffer(d,dDestPointer,hSrcPointer,n);
    }

    static void memcpyDeviceToHost(int d, host_pointer hDestPointer, const void * dSrcPointer , size_t n)
    {
        myopenclEnqueueReadBuffer(d,hDestPointer,dSrcPointer,n);
    }

    static void memcpyDeviceToDevice(int d, device_pointer dDestPointer, const device_pointer dSrcPointer , size_t n)
    {
        myopenclEnqueueCopyBuffer(d, dDestPointer, dSrcPointer, n);
    }

    static void memsetDevice(int d, device_pointer dDestPointer, int value, size_t n)
    {
        void* array = (void*) new T[n];
        memset(array, value, n);
        myopenclEnqueueWriteBuffer(d,dDestPointer,array,n);
    }






    static int getBufferDevice()
    {
        return 0;
    }

    static bool bufferAlloc(gl_buffer* /*bId*/, int /*n*/)
    {
        return false;
    }

    static void bufferFree(const gl_buffer /*bId*/)
    {

    }

    static bool bufferRegister(const gl_buffer /*bId*/)
    {
        return false;
    }

    static void bufferUnregister(const gl_buffer /*bId*/)
    {

    }

    static bool bufferMapToDevice(device_pointer* /*dDestPointer*/, const gl_buffer /*bSrcId*/)
    {
        return false;
    }

    static void bufferUnmapToDevice(device_pointer /*dDestPointer*/, const gl_buffer /*bSrcId*/)
    {

    }

};

}

}

}

#endif
