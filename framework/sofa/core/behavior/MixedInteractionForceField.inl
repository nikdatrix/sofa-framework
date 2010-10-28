/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
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
#ifndef SOFA_CORE_BEHAVIOR_MIXEDINTERACTIONFORCEFIELD_INL
#define SOFA_CORE_BEHAVIOR_MIXEDINTERACTIONFORCEFIELD_INL

#include "MixedInteractionForceField.h"

namespace sofa
{

namespace core
{

namespace behavior
{

template<class DataTypes1, class DataTypes2>
MixedInteractionForceField<DataTypes1, DataTypes2>::MixedInteractionForceField(MechanicalState<DataTypes1> *mm1, MechanicalState<DataTypes2> *mm2)
    : mstate1(mm1), mstate2(mm2), mask1(NULL), mask2(NULL)
{
    if(mm1==0||mm2==0)
        return;
}

template<class DataTypes1, class DataTypes2>
MixedInteractionForceField<DataTypes1, DataTypes2>::~MixedInteractionForceField()
{
    if(mstate1==0||mstate2==0)
        return;
}

template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::init()
{
    BaseInteractionForceField::init();
    this->mask1 = &mstate1->forceMask;
    this->mask2 = &mstate2->forceMask;
}


#ifdef SOFA_SMP
template<class DataTypes1, class DataTypes2>
struct ParallelMixedInteractionForceFieldAddForce
{
    void	operator()(MixedInteractionForceField<DataTypes1, DataTypes2> *ff,
            Shared_rw< objectmodel::Data< typename DataTypes1::VecDeriv> > _f1,Shared_rw< objectmodel::Data< typename DataTypes2::VecDeriv> > _f2,
            Shared_r< objectmodel::Data< typename DataTypes1::VecCoord> > _x1,Shared_r< objectmodel::Data< typename DataTypes2::VecCoord> > _x2,
            Shared_r< objectmodel::Data< typename DataTypes1::VecDeriv> > _v1,Shared_r< objectmodel::Data< typename DataTypes2::VecDeriv> > _v2,
            const MechanicalParams* mparams)
    {
        helper::WriteAccessor< objectmodel::Data<typename DataTypes1::VecDeriv> > f1= _f1.access();
        helper::WriteAccessor< objectmodel::Data<typename DataTypes2::VecDeriv> > f2= _f2.access();
        helper::ReadAccessor< objectmodel::Data<typename DataTypes1::VecCoord> > x1= _x1.read();
        helper::ReadAccessor< objectmodel::Data<typename DataTypes2::VecCoord> > x2= _x2.read();

        if(0&&x1.size()!=f1.size())
        {
            f1.resize(x1.size());
            // f1.zero();
        }
        if(0&&x2.size()!=f2.size())
        {
            f2.resize(x2.size());
            // f2.zero();
        }
        // mparams->setKFactor(1.0);
        ff->addForce(_f1.access(),_f2.access(),_x1.read(),_x2.read(),_v1.read(),_v2.read(),mparams);
    }

};

template<class DataTypes1, class DataTypes2>
struct ParallelMixedInteractionForceFieldAddDForce
{
    void	operator()(MixedInteractionForceField<DataTypes1, DataTypes2> *ff,
            Shared_rw< objectmodel::Data< typename DataTypes1::VecDeriv> > _df1,Shared_rw< objectmodel::Data< typename DataTypes2::VecDeriv > > _df2,
            Shared_r< objectmodel::Data< typename DataTypes1::VecDeriv > > _dx1,Shared_r< objectmodel::Data< typename DataTypes2::VecDeriv > > _dx2,
            const MechanicalParams* mparams)
    {
        helper::WriteAccessor< objectmodel::Data<typename DataTypes1::VecDeriv> > df1 = _df1.access();
        helper::WriteAccessor< objectmodel::Data<typename DataTypes2::VecDeriv> > df2 = _df2.access();
        helper::ReadAccessor< objectmodel::Data<typename DataTypes1::VecDeriv> > dx1 = _dx1.read();
        helper::ReadAccessor< objectmodel::Data<typename DataTypes2::VecDeriv> > dx2 = _dx2.read();
        if(0&&dx1.size()!=df1.size())
        {
            df1.resize(dx1.size());
            // df1.zero();
        }
        if(0&&dx2.size()!=df2.size())
        {
            df2.resize(dx2.size());
            //df2.zero();
        }
        // mparams->setKFactor(1.0);
        ff->addDForce(_df1.access(),_df2.access(),_dx1.read(),_dx2.read(),mparams);
    }

};

#endif /*ifdef SOFA_SMP*/


template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addForce(MultiVecDerivId fId , const MechanicalParams* mparams )
{

    if (mstate1 && mstate2)
    {
        mstate1->forceMask.setInUse(this->useMask());
        mstate2->forceMask.setInUse(this->useMask());
#ifdef SOFA_SMP
        if (mparams->execMode() == ExecParams::EXEC_KAAPI)
            Task<ParallelMixedInteractionForceFieldAddForce< DataTypes1, DataTypes2> >(this,
                    **defaulttype::getShared(*fId[mstate1].write()), **defaulttype::getShared(*fId[mstate2].write()),
                    **defaulttype::getShared(*mparams->readX(mstate1)), **defaulttype::getShared(*mparams->readX(mstate2)),
                    **defaulttype::getShared(*mparams->readV(mstate1)), **defaulttype::getShared(*mparams->readV(mstate2)), mparams);
        else
#endif /*ifdef SOFA_SMP*/
            addForce( *fId[mstate1].write()   , *fId[mstate2].write()   ,
                    *mparams->readX(mstate1), *mparams->readX(mstate2),
                    *mparams->readV(mstate1), *mparams->readV(mstate2), mparams );
    }
}

template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addDForce(MultiVecDerivId dfId , const MechanicalParams* mparams )
{
    if (mstate1 && mstate2)
    {
#ifdef SOFA_SMP
        if (mparams->execMode() == ExecParams::EXEC_KAAPI)
            Task<ParallelMixedInteractionForceFieldAddDForce<DataTypes1, DataTypes2> >(this,
                    **defaulttype::getShared(*dfId[mstate1].write()), **defaulttype::getShared(*dfId[mstate2].write()),
                    **defaulttype::getShared(*mparams->readDx(mstate1)) , **defaulttype::getShared(*mparams->readDx(mstate2)), mparams);
        else
#endif /*ifdef SOFA_SMP*/
            addDForce( *dfId[mstate1].write()    , *dfId[mstate2].write()   ,
                    *mparams->readDx(mstate1) , *mparams->readDx(mstate2), mparams );
    }
}



template<class DataTypes1, class DataTypes2>
double MixedInteractionForceField<DataTypes1, DataTypes2>::getPotentialEnergy(const MechanicalParams* mparams) const
{
    if (mstate1 && mstate2)
        return getPotentialEnergy(*mparams->readX(mstate1),*mparams->readX(mstate2),mparams);
    else return 0;
}

/*
template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addForce(DataVecDeriv1& f1, DataVecDeriv2& f2, const DataVecCoord1& x1, const DataVecCoord2& x2, const DataVecDeriv1& v1, const DataVecDeriv2& v2, const MechanicalParams* )
{
    addForce( *f1.beginEdit() , *f2.beginEdit(),
			  x1.getValue()   , x2.getValue()  ,
			  v1.getValue()   , v2.getValue() );
	f1.endEdit(); f2.endEdit();
}

template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addForce(VecDeriv1& , VecDeriv2& , const VecCoord1& , const VecCoord2& , const VecDeriv1& , const VecDeriv2& )
{
    serr << "ERROR("<<getClassName()<<"): addForce not implemented." << sendl;
}
*/

/*
template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addDForce(DataVecDeriv1& df1, DataVecDeriv2& df2, const DataVecDeriv1& dx1, const DataVecDeriv2& dx2, const MechanicalParams* mparams)
{
    	addDForce(*df1.beginEdit(), *df2.beginEdit(), dx1.getValue(), dx2.getValue(),mparams->kFactor(),mparams->bFactor());
    	df1.endEdit(); df2.endEdit();
}
template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addDForce(VecDeriv1& df1, VecDeriv2& df2, const VecDeriv1& dx1, const VecDeriv2& dx2, double kFactor, double )
{
    if (kFactor == 1.0)
        addDForce(df1, df2, dx1, dx2);
    else if (kFactor != 0.0)
    {
        VecDerivId vtmp1(VecDerivId::V_FIRST_DYNAMIC_INDEX);
        mstate1->vAvail(vtmp1);
        mstate1->vAlloc(vtmp1);
        VecDerivId vdx1(0);
        /// @TODO: Add a better way to get the current VecId of dx
        for (vdx1.index=0;vdx1.index<vtmp1.index;++vdx1.index)
             if (&mstate1->read(ConstVecDerivId(vdx1))->getValue() == &dx1)
		break;
        mstate1->vOp(vtmp1,VecId::null(),vdx1,kFactor);

        VecDerivId vtmp2(VecDerivId::V_FIRST_DYNAMIC_INDEX);
		mstate2->vAvail(vtmp2);
		mstate2->vAlloc(vtmp2);
		VecId vdx2(sofa::core::V_DERIV,0);
		/// @TODO: Add a better way to get the current VecId of dx
		for (vdx2.index=0;vdx2.index<vtmp2.index;++vdx2.index)
				if (&mstate2->read(ConstVecDerivId(vdx2))->getValue() == &dx2)
			break;
		mstate2->vOp(vtmp2,VecId::null(),vdx2,kFactor);

			//addDForce(df1, df2, *mstate1->getVecDeriv(vtmp1.index), *mstate2->getVecDeriv(vtmp2.index));
			addDForce(df1, df2, mstate1->read(ConstVecDerivId(vtmp1))->getValue(), mstate2->read(ConstVecDerivId(vtmp2))->getValue());

		mstate1->vFree(vtmp1);
		mstate2->vFree(vtmp2);
    }
}
template<class DataTypes1, class DataTypes2>
void MixedInteractionForceField<DataTypes1, DataTypes2>::addDForce(VecDeriv1& , VecDeriv2& , const VecDeriv1& , const VecDeriv2& )
{
    serr << "ERROR("<<getClassName()<<"): addDForce not implemented." << sendl;
}
*/

/*
template<class DataTypes1, class DataTypes2>
double MixedInteractionForceField<DataTypes1, DataTypes2>::getPotentialEnergy(const DataVecCoord1& x1, const DataVecCoord2& x2, const MechanicalParams*) const
{
	return getPotentialEnergy( x1.getValue() , x2.getValue() );
}

template<class DataTypes1, class DataTypes2>
double MixedInteractionForceField<DataTypes1, DataTypes2>::getPotentialEnergy(const VecCoord1& , const VecCoord2& ) const
{
    serr << "ERROR("<<getClassName()<<"): getPotentialEnergy(const VecCoord1& , const VecCoord2&) not implemented." << sendl;
    return 0.0;
}
*/


} // namespace behavior

} // namespace core

} // namespace sofa

#endif
