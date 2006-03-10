#ifndef SOFA_CORE_MECHANICALOBJECT_INL
#define SOFA_CORE_MECHANICALOBJECT_INL

#include "MechanicalObject.h"
#include "Sofa/Abstract/Encoding.inl"
#include <assert.h>
#include <iostream>

namespace Sofa
{

namespace Core
{

template <class DataTypes>
MechanicalObject<DataTypes>::MechanicalObject()
    : topology(NULL)
{
    x = new VecCoord;
    v = new VecDeriv;
    f = new VecDeriv;
    dx = new VecDeriv;
    // default size is 1
    resize(1);
    translation[0]=0.0;
    translation[1]=0.0;
    translation[2]=0.0;
}

template <class DataTypes>
MechanicalObject<DataTypes>::~MechanicalObject()
{
}

template <class DataTypes>
void MechanicalObject<DataTypes>::addMapping(Core::BasicMapping *mMap)
{
    assert(mMap);
    mappings.push_back(mMap);
}

template <class DataTypes>
void MechanicalObject<DataTypes>::addForceField(Core::ForceField *mFField)
{
    assert(mFField);
    forcefields.push_back(mFField);
}

template <class DataTypes>
void MechanicalObject<DataTypes>::resize(int vsize)
{
    getX()->resize(vsize);
    getV()->resize(vsize);
    getF()->resize(vsize);
    getDx()->resize(vsize);
}

template <class DataTypes>
void MechanicalObject<DataTypes>::init()
{
    Topology* topo = this->getTopology();
    if (topo!=NULL && topo->hasPos())
    {
        int nbp = topo->getNbPoints();
        std::cout<<"Setting "<<nbp<<" points from topology with translation "<<translation[0]<<" "<<translation[1]<<" "<<translation[2]<<std::endl;
        this->resize(nbp);
        for (int i=0; i<nbp; i++)
        {
            DataTypes::set((*getX())[i], topo->getPX(i)+translation[0], topo->getPY(i)+translation[1], topo->getPZ(i)+translation[2]);
        }
    }

    //this->propagateX();
    //this->propagateV();
    {
        ForceFieldIt it = forcefields.begin();
        ForceFieldIt itEnd = forcefields.end();
        for (; it != itEnd; it++)
            (*it)->init();
    }
    {
        MappingIt it = mappings.begin();
        MappingIt itEnd = mappings.end();
        for (; it != itEnd; it++)
            (*it)->init();
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::beginIteration(double dt)
{
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
    {
        (*it)->beginIteration(dt);
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::endIteration(double dt)
{
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
    {
        (*it)->endIteration(dt);
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::propagateX()
{
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
    {
        (*it)->propagateX();
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::propagateV()
{
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
    {
        (*it)->propagateV();
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::propagateDx()
{
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
    {
        (*it)->propagateDx();
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::resetForce()
{
    f->resize(x->size());
    for (unsigned int i=0; i<f->size(); i++)
        (*f)[i] = typename VecDeriv::value_type();
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
        (*it)->resetForce();
}

template <class DataTypes>
void MechanicalObject<DataTypes>::accumulateForce()
{
    {
        MappingIt it = mappings.begin();
        MappingIt itEnd = mappings.end();
        for (; it != itEnd; it++)
            (*it)->accumulateForce();
    }
    {
        ForceFieldIt it = forcefields.begin();
        ForceFieldIt itEnd = forcefields.end();
        for (; it != itEnd; it++)
            (*it)->addForce();
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::accumulateDf()
{
    {
        MappingIt it = mappings.begin();
        MappingIt itEnd = mappings.end();
        for (; it != itEnd; it++)
            (*it)->accumulateDf();
    }
    {
        ForceFieldIt it = forcefields.begin();
        ForceFieldIt itEnd = forcefields.end();
        for (; it != itEnd; it++)
            (*it)->addDForce();
    }
}

template <class DataTypes>
void MechanicalObject<DataTypes>::applyConstraints()
{
}

template <class DataTypes>
void MechanicalObject<DataTypes>::setObject(Abstract::BehaviorModel* obj)
{
    MappingIt it = mappings.begin();
    MappingIt itEnd = mappings.end();
    for (; it != itEnd; it++)
        (*it)->setObject(obj);
}

template <class DataTypes>
void MechanicalObject<DataTypes>::setTopology(Topology* topo)
{
    this->topology = topo;
}

template <class DataTypes>
Topology* MechanicalObject<DataTypes>::getTopology()
{
    return topology;
}

} // namespace Core

} // namespace Sofa

#endif
