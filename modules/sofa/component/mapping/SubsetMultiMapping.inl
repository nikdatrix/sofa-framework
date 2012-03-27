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
#ifndef SOFA_COMPONENT_MAPPING_SUBSETMULTIMAPPING_INL
#define SOFA_COMPONENT_MAPPING_SUBSETMULTIMAPPING_INL

#include <sofa/core/visual/VisualParams.h>
#include <sofa/component/mapping/SubsetMultiMapping.h>
#include <sofa/component/linearsolver/EigenSparseRectangularMatrix.h>
#include <sofa/core/MultiMapping.inl>
#include <iostream>
using std::cerr;
using std::endl;

namespace sofa
{

namespace component
{

namespace mapping
{

using namespace sofa::core;


template <class TIn, class TOut>
void SubsetMultiMapping<TIn, TOut>::init()
{
//    Inherit::init();
//    unsigned int total = computeTotalInputPoints();
//    this->toModels[0]->resize(total);

//    // create the jacobians
//    jacobians.resize( this->getFrom().size() );
//    baseMatrices.resize( this->getFrom().size() );
//    for(unsigned i=0; i<jacobians.size(); i++ ){
//        baseMatrices[i] = jacobians[i] = new SparseMatrixEigen;
//    }

//    // fill the jacobians
//    unsigned Nin = TIn::deriv_total_size, Nout = Nin, blockRow=0; // TIn = TOut necessarily for subset mappings
//    for(typename std::map<const core::State<In>*, IndexArray>::iterator i=m_indices.begin(), iend=m_indices.end(); i!=iend; i++ )
//    {
//        jacobians[i].resize(total,(*i).first->getSize() * Nin );
//        const IndexArray& indices = (*i).second;
//        for(unsigned j=0; j<indices.size(); j++){
//            for(unsigned k=0; k<Nin; k++ ){
//                jacobians[i].addTriplet(Nout*blockRow+k, Nin*indices[j]+k, (SReal)1.0);
//            }
//            blockRow++;
//        }
//    }

    Inherit::init();
    this->toModels[0]->resize(indexPairs.size());
    unsigned Nin = TIn::deriv_total_size, Nout = Nin;
//    typedef linearsolver::EigenSparseRectangularMatrix<TIn,TOut>    SparseMatrixEigen;

    // create the jacobians
//    jacobians.resize( this->getFrom().size() );
//    cerr<<"SubsetMultiMapping<TIn, TOut>::init(), getFrom().size() = "<< this->getFrom().size() <<", fromModels.size() = "<<this->fromModels.size()<<endl;
    baseMatrices.resize( this->getFrom().size() );
    for(unsigned i=0; i<baseMatrices.size(); i++ )
    {
        baseMatrices[i] = new linearsolver::EigenSparseRectangularMatrix<TIn,TOut>;
        baseMatrices[i]->resize(Nout*indexPairs.size(),Nin*this->fromModels[i]->readPositions().size() );
    }

    // fill the jacobians
    for(unsigned i=0; i<indexPairs.size(); i++)
    {
        for(unsigned k=0; k<Nin; k++ )
            baseMatrices[ indexPairs[i].first ]->set( Nout*i+k, Nin*indexPairs[i].second, (SReal)1. );
    }

    // finalize the jacobians
    for(unsigned i=0; i<baseMatrices.size(); i++ )
    {
        baseMatrices[i]->endEdit();
    }


}

template <class TIn, class TOut>
const helper::vector<sofa::defaulttype::BaseMatrix*>* SubsetMultiMapping<TIn, TOut>::getJs()
{
    return &baseMatrices;
}


template <class TIn, class TOut>
void SubsetMultiMapping<TIn, TOut>::addPoint( const core::BaseState* from, int index)
{
//    const core::State<In>* fromModel = dynamic_cast<const core::State<In>*>(from);
//    if( !fromModel ) serr<<"SubsetMultiMapping<TIn, TOut>::addPoint, fromModel=0 ! " << sendl;
//    typename std::map<const core::State<In>*, IndexArray>::iterator cur  = m_indices.find(fromModel);

//    if ( cur != m_indices.end() )
//    {
//        (*cur).second.push_back(index);
//    }
//    else
//    {
//        IndexArray ptSubset;
//        ptSubset.push_back(index);
//        m_indices[fromModel] = ptSubset;
//    }

    // find the index of the parent state
    unsigned i;
    for(i=0; i<this->fromModels.size(); i++)
        if(this->fromModels.get(i)==from )
            break;
    if(i==this->fromModels.size())
    {
        serr<<"SubsetMultiMapping<TIn, TOut>::addPoint, parent "<<from->getName()<<" not found !"<< sendl;
        assert(0);
    }
    indexPairs.push_back(IndexPair(i,index));
//    jacobianSize[i]++;

}


template <class TIn, class TOut>
void SubsetMultiMapping<TIn, TOut>::apply(const vecOutVecCoord& outPos, const vecConstInVecCoord& inPos)
{
    OutVecCoord& out = *outPos[0];
    out.resize(indexPairs.size());
    for(unsigned i=0; i<indexPairs.size(); i++)
    {
//        cerr<<"SubsetMultiMapping<TIn, TOut>::apply, i = "<< i <<", indexPair = " << indexPairs[i].first << ", " << indexPairs[i].second <<", inPos size = "<< inPos.size() <<", inPos[i] = " << (*inPos[indexPairs[i].first]) << endl;
//        cerr<<"SubsetMultiMapping<TIn, TOut>::apply, out = "<< out << endl;
        out[i] = (*inPos[indexPairs[i].first])[indexPairs[i].second];
    }

//    typename std::map<const core::State<In>* , IndexArray >::iterator iterMap;
//    core::State<Out>* output = this->toModels[0];
//    unsigned int total = computeTotalInputPoints();
//    output->resize(total);

//    OutVecCoord* outVecCoord = outPos[0];
//    unsigned int size = 0;
//    for( unsigned int i = 0; i < inPos.size() ; i++)
//    {
//        core::State<In>* current = this->fromModels[i];
//        const InVecCoord* currentVecCoord = inPos[i];
//        iterMap = m_indices.find( current );
//        if ( iterMap != m_indices.end() )
//        {
//            IndexArray indices = (*iterMap).second;
//            for( unsigned int j = 0; j < indices.size() ; j++)
//            {
//                (*outVecCoord)[size+j] = (*currentVecCoord)[ indices[j] ];
//            }
//            size += indices.size();

//        }
//    }
}


template <class TIn, class TOut>
void SubsetMultiMapping<TIn, TOut>::applyJ(const helper::vector< OutVecDeriv*>& outDeriv, const helper::vector<const InVecDeriv*>& inDeriv)
{
    OutVecDeriv& out = *outDeriv[0];
    out.resize(indexPairs.size());
    for(unsigned i=0; i<indexPairs.size(); i++)
    {
        out[i] = (*inDeriv[indexPairs[i].first])[indexPairs[i].second];
    }
//    core::State<Out>* output = this->toModels[0];
//    unsigned int total = computeTotalInputPoints();
//    output->resize(total);

//    typename std::map<const core::State<In>* , IndexArray >::iterator iterMap;
//    OutVecDeriv* outVecDeriv = outDeriv[0];
//    unsigned int size = 0;

//    for( unsigned int i = 0; i < inDeriv.size() ; i++)
//    {
//        core::State<In>* current = this->fromModels[i];
//        const InVecDeriv* currentVecDeriv = inDeriv[i];
//        iterMap = m_indices.find( current );
//        if ( iterMap != m_indices.end() )
//        {
//            IndexArray indices = (*iterMap).second;
//            for( unsigned int j = 0; j < indices.size() ; j++)
//            {
//                (*outVecDeriv)[size+j] = (*currentVecDeriv)[ indices[j] ];
//            }
//            size += indices.size();

//        }
//    }
}


template <class TIn, class TOut>
void SubsetMultiMapping<TIn, TOut>::applyJT(const helper::vector<InVecDeriv*>& parentDeriv , const helper::vector<const OutVecDeriv*>& childDeriv )
{
    const InVecDeriv& cder = *childDeriv[0];
    for(unsigned i=0; i<indexPairs.size(); i++)
    {
        (*parentDeriv[indexPairs[i].first])[indexPairs[i].second] += cder[i];
    }
//    typename std::map<const core::State<In>* , IndexArray >::iterator iterMap;
//    const OutVecDeriv* mappedVecDeriv = inDeriv[0];

//    unsigned int size = 0;
//    for( unsigned int i = 0; i < outDeriv.size() ; i++)
//    {
//        core::State<In>* current = this->fromModels[i];
//        InVecDeriv* currentVecDeriv = outDeriv[i];
//        iterMap = m_indices.find( current );
//        if ( iterMap != m_indices.end() )
//        {
//            IndexArray indices = (*iterMap).second;
//            for( unsigned int j = 0; j < indices.size() ; j++)
//            {
//                (*currentVecDeriv)[ indices[j] ] += (*mappedVecDeriv)[size + j] ;
//            }
//            size += indices.size();
//        }
//    }
}

} // namespace mapping

} // namespace component

} // namespace sofa

#endif //SOFA_COMPONENT_MAPPING_SUBSETMULTIMAPPING_INL
