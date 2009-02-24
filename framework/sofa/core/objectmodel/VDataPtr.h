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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CORE_OBJECTMODEL_VFIELD_H
#define SOFA_CORE_OBJECTMODEL_VFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/objectmodel/DataPtr.h>

namespace sofa
{

namespace core
{

namespace objectmodel
{

/**
 *  \brief Access to the getV() vector of a MechanicalObject.
 *
 *  @author François Faure
 */
template<class DataTypes>
class VDataPtr : public DataPtr<typename DataTypes::VecDeriv>
{
public:
    typedef typename DataTypes::VecDeriv VecDeriv;
    
    VDataPtr(VecDeriv** ptr, const char* name)
    : DataPtr<VecDeriv>(0,name)
    , m_vecPtr(ptr)
    {
    }

    ~VDataPtr()
    {}

   void init(){this->ptr = *m_vecPtr;}

    VecDeriv* beginEdit()
    {
        this->ptr = *m_vecPtr;
        //std::cerr<<"VDataPtr<DataTypes>::beginEdit()"<<std::endl;
        return this->DataPtr<VecDeriv>::beginEdit();
    }

protected:
    VecDeriv** m_vecPtr; 
};

} // namespace objectmodel

} // namespace core

} // namespace sofa

#endif
