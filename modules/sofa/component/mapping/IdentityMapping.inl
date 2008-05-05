#ifndef SOFA_COMPONENT_MAPPING_IDENTITYMAPPING_INL
#define SOFA_COMPONENT_MAPPING_IDENTITYMAPPING_INL

#include <sofa/component/mapping/IdentityMapping.h>
#include <sofa/core/componentmodel/behavior/MechanicalMapping.inl>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>


namespace sofa
{

namespace component
{

namespace mapping
{

template<class T1, class T2>
extern void eq(T1& dest, const T2& src)
{
    dest = (T1)(src);
}

template<class T1, class T2>
extern void peq(T1& dest, const T2& src)
{
    dest += (T1)(src);
}

template<int N, typename real1, typename real2>
extern void eq(defaulttype::RigidCoord<N,real1>& dest, const defaulttype::RigidCoord<N,real2>& src)
{
    dest.getCenter() = src.getCenter();
    dest.getOrientation() = src.getOrientation();
}

template<int N, typename real1, typename real2>
extern void peq(defaulttype::RigidCoord<N,real1>& dest, const defaulttype::RigidCoord<N,real2>& src)
{
    dest.getCenter() += src.getCenter();
    dest.getOrientation() += src.getOrientation();
}

template<int N, typename real1, typename real2>
extern void eq(defaulttype::RigidDeriv<N,real1>& dest, const defaulttype::RigidDeriv<N,real2>& src)
{
    dest.getVCenter() = src.getVCenter();
    dest.getVOrientation() = src.getVOrientation();
}

template<int N, typename real1, typename real2>
extern void peq(defaulttype::RigidDeriv<N,real1>& dest, const defaulttype::RigidDeriv<N,real2>& src)
{
    dest.getVCenter() += src.getVCenter();
    dest.getVOrientation() += src.getVOrientation();
}

template<int N, typename real1, typename real2>
extern void eq(defaulttype::Vec<N,real1>& dest, const defaulttype::RigidCoord<N,real2>& src)
{
    dest = src.getCenter();
}

template<int N, typename real1, typename real2>
extern void peq(defaulttype::Vec<N,real1>& dest, const defaulttype::RigidCoord<N,real2>& src)
{
    dest += src.getCenter();
}

template<int N, typename real1, typename real2>
extern void eq(defaulttype::Vec<N,real1>& dest, const defaulttype::RigidDeriv<N,real2>& src)
{
    dest = src.getVCenter();
}

template<int N, typename real1, typename real2>
extern void peq(defaulttype::Vec<N,real1>& dest, const defaulttype::RigidDeriv<N,real2>& src)
{
    dest += src.getVCenter();
}

template<int N, typename real1, typename real2>
extern void eq(defaulttype::RigidCoord<N,real1>& dest, const defaulttype::Vec<N,real2>& src)
{
    dest.getCenter() = src;
}

template<int N, typename real1, typename real2>
extern void peq(defaulttype::RigidCoord<N,real1>& dest, const defaulttype::Vec<N,real2>& src)
{
    dest.getCenter() += src;
}

template<int N, typename real1, typename real2>
extern void eq(defaulttype::RigidDeriv<N,real1>& dest, const defaulttype::Vec<N,real2>& src)
{
    dest.getVCenter() = src;
}

template<int N, typename real1, typename real2>
extern void peq(defaulttype::RigidDeriv<N,real1>& dest, const defaulttype::Vec<N,real2>& src)
{
    dest.getVCenter() += src;
}

template <class BasicMapping>
void IdentityMapping<BasicMapping>::apply( typename Out::VecCoord& out, const typename In::VecCoord& in )
{
    //const unsigned int N = Coord::size() < InCoord::size() ? Coord::size() : InCoord::size();
    out.resize(in.size());
    for(unsigned int i=0; i<out.size(); i++)
    {
        //for (unsigned int j=0;j < N;++j)
        //    out[i][j] = (OutReal)in[i][j];
        //out[i] = in[i];
        eq(out[i], in[i]);
    }
}

template <class BasicMapping>
void IdentityMapping<BasicMapping>::applyJ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
{
    //const unsigned int N = Deriv::size() < InDeriv::size() ? Deriv::size() : InDeriv::size();
    out.resize(in.size());
    for(unsigned int i=0; i<out.size(); i++)
    {
        //for (unsigned int j=0;j < N;++j)
        //    out[i][j] = (OutReal)in[i][j];
        eq(out[i], in[i]);
    }
}

template <class BasicMapping>
void IdentityMapping<BasicMapping>::applyJT( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
{
    //const unsigned int N = Deriv::size() < InDeriv::size() ? Deriv::size() : InDeriv::size();
    for(unsigned int i=0; i<out.size(); i++)
    {
        //for (unsigned int j=0;j < N;++j)
        //    out[i][j] += (Real)in[i][j];
        peq(out[i], in[i]);
    }
}

template <class BaseMapping>
void IdentityMapping<BaseMapping>::applyJT( typename In::VecConst& out, const typename Out::VecConst& in )
{
    out.clear();
    out.resize(in.size());
    //const unsigned int N = Deriv::size() < InDeriv::size() ? Deriv::size() : InDeriv::size();
    for(unsigned int i=0; i<in.size(); i++)
    {
        typename In::SparseVecDeriv& o = out[i];
        o.reserve(in[i].size());
        for(unsigned int j=0; j<in[i].size(); j++)
        {
            const typename Out::SparseDeriv& cIn = in[i][j];
            InDeriv value;
            //for (unsigned int k=0;k<N;++k)
            //    value[k] = (Real) cIn.data[k];
            eq(value, cIn.data);
            o.push_back( typename In::SparseDeriv(cIn.index, value) );
        }
    }
}

} // namespace mapping

} // namespace component

} // namespace sofa

#endif
