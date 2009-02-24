/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
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
#ifndef CUDAMATH_H
#define CUDAMATH_H

#include <cuda_runtime.h>

template<class real>
class CudaVec2;

template<class real>
class CudaVec3;

template<class real>
class CudaVec4;

template<>
class CudaVec2<float> : public float2
{
public:
    typedef float Real;
    static __inline__ __device__ __host__ CudaVec2<float> make(Real x, Real y)
    {
	CudaVec2<float> r; r.x = x; r.y = y; return r;
    }
    static __inline__ __device__ __host__ CudaVec2<float> make(float2 v)
    {
	CudaVec2<float> r; r.x = v.x; r.y = v.y; return r;
    }
    static __inline__ __device__ __host__ CudaVec2<float> make(float3 v)
    {
	CudaVec2<float> r; r.x = v.x; r.y = v.y; return r;
    }
};

template<>
class CudaVec3<float> : public float3
{
public:
    typedef float Real;
    static __inline__ __device__ __host__ CudaVec3<float> make(Real x, Real y, Real z=0)
    {
	CudaVec3<float> r; r.x = x; r.y = y;  r.z = z; return r;
    }
    static __inline__ __device__ __host__ CudaVec3<float> make(float2 v, Real z=0)
    {
	CudaVec3<float> r; r.x = v.x; r.y = v.y;  r.z = z; return r;
    }
    static __inline__ __device__ __host__ CudaVec3<float> make(float3 v)
    {
	CudaVec3<float> r; r.x = v.x; r.y = v.y;  r.z = v.z; return r;
    }
    static __inline__ __device__ __host__ CudaVec3<float> make(float4 v)
    {
	CudaVec3<float> r; r.x = v.x; r.y = v.y;  r.z = v.z; return r;
    }
};

template<>
class CudaVec4<float> : public float4
{
public:
    typedef float Real;
    static __inline__ __device__ __host__ CudaVec4<float> make(Real x, Real y, Real z, Real w=0)
    {
	CudaVec4<float> r; r.x = x; r.y = y;  r.z = z; r.w = w; return r;
    }
    static __inline__ __device__ __host__ CudaVec4<float> make(float3 v, Real w=0)
    {
	CudaVec4<float> r; r.x = v.x; r.y = v.y;  r.z = v.z; r.w = w; return r;
    }
    static __inline__ __device__ __host__ CudaVec4<float> make(float4 v)
    {
	CudaVec4<float> r; r.x = v.x; r.y = v.y;  r.z = v.z; r.w = v.w; return r;
    }
};

typedef CudaVec2<float> CudaVec2f;
typedef CudaVec3<float> CudaVec3f;
typedef CudaVec4<float> CudaVec4f;


template<class real>
__device__ CudaVec3<real> operator+(CudaVec3<real> a, CudaVec3<real> b)
{
    return CudaVec3<real>::make(a.x+b.x, a.y+b.y, a.z+b.z);
}

template<class real>
__device__ void operator+=(CudaVec3<real>& a, CudaVec3<real> b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
}

template<class real>
__device__ void operator+=(CudaVec3<real>& a, real b)
{
	a.x += b;
	a.y += b;
	a.z += b;
}

template<class real>
__device__ CudaVec3<real> operator-(CudaVec3<real> a, CudaVec3<real> b)
{
	return CudaVec3<real>::make(a.x-b.x, a.y-b.y, a.z-b.z);
}

template<class real>
__device__ void operator-=(CudaVec3<real>& a, CudaVec3<real> b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
}

template<class real>
__device__ void operator-=(CudaVec3<real>& a, real b)
{
	a.x -= b;
	a.y -= b;
	a.z -= b;
}

template<class real>
__device__ CudaVec3<real> operator-(CudaVec3<real>& a)
{
	return CudaVec3<real>::make(-a.x, -a.y, -a.z);
}

template<class real>
__device__ CudaVec3<real> operator*(CudaVec3<real> a, real b)
{
	return CudaVec3<real>::make(a.x*b, a.y*b, a.z*b);
}

template<class real>
__device__ CudaVec3<real> operator/(CudaVec3<real> a, real b)
{
	return CudaVec3<real>::make(a.x/b, a.y/b, a.z/b);
}

template<class real>
__device__ void operator*=(CudaVec3<real>& a, real b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
}

template<class real>
__device__ CudaVec3<real> operator*(real a, CudaVec3<real> b)
{
	return CudaVec3<real>::make(a*b.x, a*b.y, a*b.z);
}

template<class real>
__device__ CudaVec3<real> mul(CudaVec3<real> a, CudaVec3<real> b)
{
	return CudaVec3<real>::make(a.x*b.x, a.y*b.y, a.z*b.z);
}

template<class real>
__device__ real dot(CudaVec3<real> a, CudaVec3<real> b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

template<class real>
__device__ CudaVec3<real> cross(CudaVec3<real> a, CudaVec3<real> b)
{
	return CudaVec3<real>::make(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
}

template<class real>
__device__ real norm2(CudaVec3<real> a)
{
    return a.x*a.x+a.y*a.y+a.z*a.z;
}

template<class real>
__device__ real norm(CudaVec3<real> a)
{
    return sqrtf(norm2(a));
}

template<class real>
__device__ real invnorm(CudaVec3<real> a)
{
    return rsqrtf(norm2(a));
}

template<class real>
class /*__align__(4)*/ matrix3
{
public:
    CudaVec3<real> x,y,z;
/*
    CudaVec3<real> getLx() { return x; }
    CudaVec3<real> getLy() { return y; }
    CudaVec3<real> getLz() { return z; }

    CudaVec3<real> getCx() { return CudaVec3<real>::make(x.x,y.x,z.x); }
    CudaVec3<real> getCy() { return CudaVec3<real>::make(x.y,y.y,z.y); }
    CudaVec3<real> getCz() { return CudaVec3<real>::make(x.z,y.z,z.z); }

    void setLx(CudaVec3<real> v) { x = v; }
    void setLy(CudaVec3<real> v) { y = v; }
    void setLz(CudaVec3<real> v) { z = v; }

    void setCx(CudaVec3<real> v) { x.x = v.x; y.x = v.y; z.x = v.z; }
    void setCy(CudaVec3<real> v) { x.y = v.x; y.y = v.y; z.y = v.z; }
    void setCz(CudaVec3<real> v) { x.z = v.x; y.z = v.y; z.z = v.z; }
*/
    __device__ CudaVec3<real> operator*(CudaVec3<real> v)
    {
        return CudaVec3<real>::make(dot(x,v),dot(y,v),dot(z,v));
    }
    __device__ CudaVec3<real> mulT(CudaVec3<real> v)
    {
        return x*v.x+y*v.y+z*v.z;
    }
    __device__ real mulX(CudaVec3<real> v)
    {
        return dot(x,v);
    }
    __device__ real mulY(CudaVec3<real> v)
    {
        return dot(y,v);
    }
    __device__ real mulZ(CudaVec3<real> v)
    {
        return dot(z,v);
    }
    __device__ void readAoS(const real* data)
    {
        x.x=*data; data+=blockDim.x;
        x.y=*data; data+=blockDim.x;
        x.z=*data; data+=blockDim.x;
        y.x=*data; data+=blockDim.x;
        y.y=*data; data+=blockDim.x;
        y.z=*data; data+=blockDim.x;
        z.x=*data; data+=blockDim.x;
        z.y=*data; data+=blockDim.x;
        z.z=*data; data+=blockDim.x;
    }
    __device__ void writeAoS(real* data)
    {
        *data=x.x; data+=blockDim.x;
        *data=x.y; data+=blockDim.x;
        *data=x.z; data+=blockDim.x;
        *data=y.x; data+=blockDim.x;
        *data=y.y; data+=blockDim.x;
        *data=y.z; data+=blockDim.x;
        *data=z.x; data+=blockDim.x;
        *data=z.y; data+=blockDim.x;
        *data=z.z; data+=blockDim.x;
    }
};

#endif
