/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
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
#include "mycuda.h"
#include <cuda.h>
#ifdef WIN32
#include <sofa/helper/system/gl.h>
#endif
#include <cuda_gl_interop.h>

#include <sofa/helper/BackTrace.h>


//#define NO_CUDA

cudaDeviceProp mycudaDeviceProp;


#if defined(__cplusplus)
namespace sofa
{
namespace gpu
{
namespace cuda
{
#endif

#ifdef NO_CUDA

bool cudaCheck(cudaError_t, const char*)
{
    return true;
}

bool cudaInitCalled = false;

int mycudaInit(int)
{
    cudaInitCalled = true;
    return 0;
}

void mycudaMalloc(void **devPtr, size_t,int )
{
    *devPtr = NULL;
}

void mycudaMallocPitch(void **devPtr, size_t*, size_t, size_t)
{
    *devPtr = NULL;
}

void mycudaFree(void *, int)
{
}

void mycudaMallocHost(void **hostPtr, size_t size)
{
    *hostPtr = malloc(size);
}

void mycudaFreeHost(void *hostPtr)
{
    free(hostPtr);
}

void mycudaMemcpyHostToDevice(void *, const void *, size_t, int)
{
}

void mycudaMemcpyDeviceToDevice(void *, const void *, size_t,int )
{
}

void mycudaMemcpyDeviceToHost(void *, const void *, size_t,int )
{
}

void mycudaMemcpyHostToDevice2D(void *, size_t, const void *, size_t, size_t, size_t)
{
}

void mycudaMemcpyDeviceToDevice2D(void *, size_t, const void *, size_t, size_t, size_t )
{
}

void mycudaMemcpyDeviceToHost2D(void *, size_t, const void *, size_t, size_t, size_t)
{
}

void mycudaGLRegisterBufferObject(int)
{
}

void mycudaGLUnregisterBufferObject(int)
{
}

void mycudaGLMapBufferObject(void** ptr, int)
{
    *ptr = NULL;
}

void mycudaGLUnmapBufferObject(int)
{
}

int mycudaGetnumDevices()
{
    return 0;
}

int mycudaGetBufferDevice()
{
    return 0;
}

void mycudaMemset(void *devPtr, int val, size_t size,int )
{
}

void cuda_void_kernel()
{
}

#ifdef SOFA_GPU_CUBLAS
cusparseHandle_t getCusparseCtx()
{
    return NULL;
}

cublasHandle_t getCublasCtx()
{
    return NULL;
}

cusparseMatDescr_t getCusparseMatDescr()
{
    return NULL;
}

#endif //SOFA_GPU_CUBLAS

#else

extern "C"
{
    int SOFA_GPU_CUDA_API mycudaGetMultiProcessorCount();
    void cuda_void_kernel();
};

bool cudaCheck(cudaError_t err, const char* src="?")
{
    if (err == cudaSuccess) return true;
    //fprintf(stderr, "CUDA: Error %d returned from %s.\n",(int)err,src);
    mycudaLogError(cudaGetErrorString(err), src);
    return false;
}

bool cudaInitCalled = false;
int deviceCount = 0;

int mycudaInit(int device)
{
    cudaInitCalled = true;
    cudaCheck(cudaGetDeviceCount(&deviceCount),"cudaGetDeviceCount");
    myprintf("CUDA: %d device(s) found.\n", deviceCount);
    for (int i=0; i<deviceCount; i++)
    {
        cudaDeviceProp dev
#ifdef cudaDevicePropDontCare
            = cudaDevicePropDontCare
#endif
                    ;
        //memset(&dev,0,sizeof(dev));
        //dev.name=NULL;
        //dev.bytes=0;
        //dev.major=0;
        //dev.minor=0;
        cudaCheck(cudaGetDeviceProperties(&dev,i),"cudaGetDeviceProperties");
#if CUDA_VERSION >= 2010
        myprintf("CUDA:  %d : \"%s\", %d MB, %d cores at %.3f GHz, revision %d.%d",i,dev.name, dev.totalGlobalMem/(1024*1024), dev.multiProcessorCount*8, dev.clockRate * 1e-6f, dev.major, dev.minor);
        if (dev.kernelExecTimeoutEnabled)
            myprintf(", timeout enabled", dev.kernelExecTimeoutEnabled);
        myprintf("\n");
#elif CUDA_VERSION >= 2000
        myprintf("CUDA:  %d : \"%s\", %d MB, %d cores at %.3f GHz, revision %d.%d\n",i,dev.name, dev.totalGlobalMem/(1024*1024), dev.multiProcessorCount*8, dev.clockRate * 1e-6f, dev.major, dev.minor);
#else //if CUDA_VERSION >= 1000
        myprintf("CUDA:  %d : \"%s\", %d MB, cores at %.3f GHz, revision %d.%d\n",i,dev.name, dev.totalGlobalMem/(1024*1024), dev.clockRate * 1e-6f, dev.major, dev.minor);
//#else
//		myprintf("CUDA:  %d : \"%s\", %d MB, revision %d.%d\n",i,(dev.name==NULL?"":dev.name), dev.bytes/(1024*1024), dev.major, dev.minor);
#endif
    }
    if (device==-1)
    {
        const char* var = mygetenv("CUDA_DEVICE");
        device = (var && *var) ? atoi(var):0;
    }
    if (device >= deviceCount)
    {
        myprintf("CUDA: Device %d not found.\n", device);
        return 0;
    }
    else
    {
        cudaDeviceProp& dev = mycudaDeviceProp;
        cudaCheck(cudaGetDeviceProperties(&dev,device));
        myprintf("CUDA: Using device %d : \"%s\"\n",device,dev.name);
        cudaCheck(cudaSetDevice(device));
        mycudaPrivateInit(device);

    }

#ifdef SOFA_GPU_CUBLAS
    cublasInit();
#endif
    return 1;
}

int mycudaGetMultiProcessorCount()
{
    return mycudaDeviceProp.multiProcessorCount;
}

void mycudaMalloc(void **devPtr, size_t size,int /*d*/)
{
    if (!cudaInitCalled) mycudaInit();
    if (mycudaVerboseLevel>=LOG_INFO) myprintf("CUDA: malloc(%d).\n",size);
    cudaCheck(cudaMalloc(devPtr, size),"cudaMalloc");
    if (mycudaVerboseLevel>=LOG_TRACE) myprintf("CUDA: malloc(%d) -> 0x%x.\n",size, *devPtr);
}

void mycudaMallocPitch(void **devPtr, size_t* pitch, size_t width, size_t height)
{
    if (!cudaInitCalled) mycudaInit();
    if (mycudaVerboseLevel>=LOG_INFO) myprintf("CUDA: mallocPitch(%d,%d).\n",width,height);
    cudaCheck(cudaMallocPitch(devPtr, pitch, width, height),"cudaMalloc2D");
    if (mycudaVerboseLevel>=LOG_TRACE) myprintf("CUDA: mallocPitch(%d,%d) -> 0x%x at pitch %d.\n",width,height, *devPtr, (int)*pitch);
}

void mycudaFree(void *devPtr,int /*d*/)
{
    if (mycudaVerboseLevel>=LOG_TRACE) myprintf("CUDA: free(0x%x).\n",devPtr);
    cudaCheck(cudaFree(devPtr),"cudaFree");
}

void mycudaMallocHost(void **hostPtr, size_t size)
{
    if (!cudaInitCalled) mycudaInit();
    if (mycudaVerboseLevel>=LOG_TRACE) myprintf("CUDA: mallocHost(%d).\n",size);
    cudaCheck(cudaMallocHost(hostPtr, size),"cudaMallocHost");
    if (mycudaVerboseLevel>=LOG_TRACE) myprintf("CUDA: mallocHost(%d) -> 0x%x.\n",size, *hostPtr);
}

void mycudaFreeHost(void *hostPtr)
{
    if (mycudaVerboseLevel>=LOG_TRACE) myprintf("CUDA: freeHost(0x%x).\n",hostPtr);
    cudaCheck(cudaFreeHost(hostPtr),"cudaFreeHost");
}

void mycudaMemcpyHostToDevice(void *dst, const void *src, size_t count,int /*d*/)
{
    //count = (count+3)&(size_t)-4;
    if (!cudaCheck(cudaMemcpy(dst, src, count, cudaMemcpyHostToDevice),"cudaMemcpyHostToDevice"))
        myprintf("in mycudaMemcpyHostToDevice(0x%x, 0x%x, %d)\n",dst,src,count);

    if (mycudaVerboseLevel>=LOG_STACK_TRACE) displayStack("mycudaMemcpyHostToDevice");
}

void mycudaMemcpyDeviceToDevice(void *dst, const void *src, size_t count,int /*d*/		)
{
    cudaCheck(cudaMemcpy(dst, src, count, cudaMemcpyDeviceToDevice),"cudaMemcpyDeviceToDevice");
}

void mycudaMemcpyDeviceToHost(void *dst, const void *src, size_t count,int /*d*/)
{
    //count = (count+3)&(size_t)-4;
    cudaCheck(cudaMemcpy(dst, src, count, cudaMemcpyDeviceToHost),"cudaMemcpyDeviceToHost");

    if (mycudaVerboseLevel>=LOG_STACK_TRACE) displayStack("mycudaMemcpyDeviceToHost");
}

void mycudaMemcpyHostToDevice2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height)
{
    cudaCheck(cudaMemcpy2D(dst, dpitch, src, spitch, width, height, cudaMemcpyHostToDevice),"cudaMemcpyHostToDevice2D");

    if (mycudaVerboseLevel>=LOG_STACK_TRACE) displayStack("mycudaMemcpyHostToDevice2D");
}

void mycudaMemcpyDeviceToDevice2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height)
{
    cudaCheck(cudaMemcpy2D(dst, dpitch, src, spitch, width, height, cudaMemcpyDeviceToDevice),"cudaMemcpyDeviceToDevice2D");
}

void mycudaMemcpyDeviceToHost2D(void *dst, size_t dpitch, const void *src, size_t spitch, size_t width, size_t height)
{
    cudaCheck(cudaMemcpy2D(dst, dpitch, src, spitch, width, height, cudaMemcpyDeviceToHost),"cudaMemcpyDeviceToHost2D");

    if (mycudaVerboseLevel>=LOG_STACK_TRACE) displayStack("mycudaMemcpyDeviceToHost2D");
}

void mycudaMemset(void *devPtr, int val, size_t size,int d)
{
    cudaCheck(cudaMemset(devPtr, val,size),"mycudaMemset");
}


void mycudaThreadSynchronize()
{
    if (!cudaInitCalled) return; // no need to synchronize if no-one used cuda yet

    cudaThreadSynchronize();
}

void mycudaCheckError(const char* src)
{
    if (!cudaInitCalled) return; // no need to check errors if no-one used cuda yet
    cudaThreadSynchronize();
    cudaCheck(cudaGetLastError(),src);
}

void mycudaGLRegisterBufferObject(int id)
{
    if (!cudaInitCalled) mycudaInit();
    myprintf("mycudaGLRegisterBufferObject %d\n",id);
    cudaCheck(cudaGLRegisterBufferObject((GLuint)id),"cudaGLRegisterBufferObject");
}

void mycudaGLUnregisterBufferObject(int id)
{
    cudaCheck(cudaGLUnregisterBufferObject((GLuint)id),"cudaGLUnregisterBufferObject");
}

void mycudaGLMapBufferObject(void** ptr, int id)
{
    cudaCheck(cudaGLMapBufferObject(ptr, (GLuint)id),"cudaGLMapBufferObject");
}

void mycudaGLUnmapBufferObject(int id)
{
    cudaCheck(cudaGLUnmapBufferObject((GLuint)id),"cudaGLUnmapBufferObject");
}

int mycudaGetnumDevices()
{
    if (!cudaInitCalled) mycudaInit();
    return deviceCount;
}

int mycudaGetBufferDevice()
{
    return 0;
}

__global__ void cuda_debug_kernel()
{
}

void cuda_void_kernel()
{
    myprintf("WARNING : cuda_void_kernel should only be used for debug\n");

    dim3 threads(1,1);
    dim3 grid(1,1);
    {cuda_debug_kernel<<< grid, threads >>>(); mycudaDebugError("cuda_debug_kernel");}
}

#ifdef SOFA_GPU_CUBLAS

cublasHandle_t getCublasCtx()
{
    static cublasHandle_t cublashandle = NULL;
    if (cublashandle==NULL)
    {
        cublasStatus_t status = cublasCreate(&cublashandle);
        if (status != CUBLAS_STATUS_SUCCESS)
        {
            myprintf("cublas Handle init failed\n");
        }
    }
    return cublashandle;
}

cusparseHandle_t getCusparseCtx()
{
    static cusparseHandle_t cusparsehandle = NULL;
    if (cusparsehandle==NULL)
    {
        cusparseStatus_t status = cusparseCreate(&cusparsehandle);
        if (status != CUSPARSE_STATUS_SUCCESS)
        {
            myprintf("cusparse Handle init failed\n");
        }
    }
    return cusparsehandle;
}

cusparseMatDescr_t getCusparseMatDescr()
{
    static cusparseMatDescr_t descra=NULL;
    if (descra==NULL)
    {
        static cusparseStatus_t status = cusparseCreateMatDescr(&descra);
        if (status != CUSPARSE_STATUS_SUCCESS)
        {
            myprintf("Matrix descriptor init failed\n");
        }
        else
        {
            cusparseSetMatType(descra, CUSPARSE_MATRIX_TYPE_GENERAL);
            cusparseSetMatIndexBase(descra, CUSPARSE_INDEX_BASE_ZERO);
        }
    }
    return descra;
}

#endif //SOFA_GPU_CUBLAS

#endif

#if defined(__cplusplus)
} // namespace cuda
} // namespace gpu
} // namespace sofa
#endif