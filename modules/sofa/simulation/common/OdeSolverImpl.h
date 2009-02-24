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
#ifndef SOFA_SIMULATION_ODESOLVERIMPL_H
#define SOFA_SIMULATION_ODESOLVERIMPL_H

#include <sofa/core/componentmodel/behavior/OdeSolver.h>
#include <sofa/core/componentmodel/behavior/LinearSolver.h>

namespace sofa
{

namespace simulation
{


/**
 *  \brief Implementation of LinearSolver/OdeSolver/MasterSolver relying on component::System.
 *
 */
class SolverImpl : public virtual sofa::core::objectmodel::BaseObject
{
public:
    typedef sofa::core::componentmodel::behavior::BaseMechanicalState::VecId VecId;

    SolverImpl();

    virtual ~SolverImpl();

    /// @name Visitors and MultiVectors
    /// These methods provides an abstract view of the mechanical system to animate.
    /// They are implemented by executing Visitors in the subtree of the scene-graph below this solver.
    /// @{

    /// @name Vector operations
    /// Most of these operations can be hidden by using the MultiVector class.
    /// @{

    /// Wait for the completion of previous operations and return the result of the last v_dot call.
    ///
    /// Note that currently all methods are blocking so finish simply return the result of the last v_dot call.
    virtual double finish();

    /// Allocate a temporary vector
    virtual VecId v_alloc(VecId::Type t);
    /// Free a previously allocated temporary vector
    virtual void v_free(VecId v);

    virtual void v_clear(VecId v); ///< v=0
    virtual void v_eq(VecId v, VecId a); ///< v=a
    virtual void v_peq(VecId v, VecId a, double f=1.0); ///< v+=f*a
    virtual void v_teq(VecId v, double f); ///< v*=f
    virtual void v_op(VecId v, VecId a, VecId b, double f=1.0); ///< v=a+b*f
    virtual void v_dot(VecId a, VecId b); ///< a dot b ( get result using finish )
    virtual void v_threshold(VecId a, double threshold); ///< nullify the values below the given threshold
    /// Propagate the given displacement through all mappings
    virtual void propagateDx(VecId dx);
    /// Propagate the given displacement through all mappings and reset the current force delta
    virtual void propagateDxAndResetDf(VecId dx, VecId df);
    /// Apply projective constraints to the given vector
    virtual void projectResponse(VecId dx, double **W=NULL);
    virtual void addMdx(VecId res, VecId dx=VecId(), double factor = 1.0); ///< res += factor M.dx
    virtual void integrateVelocity(VecId res, VecId x, VecId v, double dt); ///< res = x + v.dt
    virtual void accFromF(VecId a, VecId f); ///< a = M^-1 . f

    /// Compute the current force (given the latest propagated position and velocity)
    virtual void computeForce(VecId result, bool clear = true, bool accumulate = true);
    /// Compute the current force delta (given the latest propagated displacement)
    virtual void computeDf(VecId df, bool clear = true, bool accumulate = true);
    /// Compute the current force delta (given the latest propagated velocity)
    virtual void computeDfV(VecId df, bool clear = true, bool accumulate = true);
    /// accumulate $ df += (m M + b B + k K) dx $ (given the latest propagated displacement)
    virtual void addMBKdx(VecId df, double m, double b, double k, bool clear = true, bool accumulate = true);
    /// accumulate $ df += (m M + b B + k K) velocity $
    virtual void addMBKv(VecId df, double m, double b, double k, bool clear = true, bool accumulate = true);
    /// Add dt*Gravity to the velocity
    virtual void addSeparateGravity(double dt);

    virtual void computeContactForce(VecId result);
    virtual void computeContactDf(VecId df);


    /// @}

    /// @name Matrix operations
    /// @{

    // BaseMatrix & BaseVector Computations
	virtual void addMBK_ToMatrix(defaulttype::BaseMatrix *A, double mFact=1.0, double bFact=1.0, double kFact=1.0, unsigned int offset=0);
    //virtual void addMBKdx_ToVector(defaulttype::BaseVector *V, VecId dx, double mFact=1.0, double bFact=1.0, double kFact=1.0, unsigned int offset=0);
    virtual void getMatrixDimension(unsigned int * const, unsigned int * const);
	virtual void multiVector2BaseVector(VecId src, defaulttype::BaseVector *dest=NULL, unsigned int offset=0);
	virtual void multiVectorPeqBaseVector(VecId dest, defaulttype::BaseVector *src=NULL, unsigned int offset=0);

    /// @}

    /// @name Debug operations
    /// @{

    /// Dump the content of the given vector.
    virtual void print( VecId v, std::ostream& out );
    virtual void printWithElapsedTime( VecId v,  unsigned time, std::ostream& out=std::cerr );

    /// @}

    /// @}

protected:

    /// Result of latest v_dot operation
    double result;

};

class OdeSolverImpl : public sofa::core::componentmodel::behavior::OdeSolver, public SolverImpl
{
public:
    typedef sofa::core::componentmodel::behavior::BaseMechanicalState::VecId VecId;
    typedef sofa::core::componentmodel::behavior::MultiVector<OdeSolverImpl> MultiVector;
    typedef sofa::core::componentmodel::behavior::MultiMatrix<OdeSolverImpl> MultiMatrix;
    typedef sofa::core::componentmodel::behavior::MechanicalMatrix MechanicalMatrix;

    /// Propagate the given state (time, position and velocity) through all mappings
    virtual void propagatePositionAndVelocity(double t, VecId x, VecId v);
    /// Compute the acceleration corresponding to the given state (time, position and velocity)
    virtual void computeAcc(double t, VecId a, VecId x, VecId v);
    virtual void computeContactAcc(double t, VecId a, VecId x, VecId v);
    
    /// @name Matrix operations using LinearSolver components
    /// @{
    
    virtual void m_resetSystem();
    virtual void m_setSystemMBKMatrix(double mFact, double bFact, double kFact);
    virtual void m_setSystemRHVector(VecId v);
    virtual void m_setSystemLHVector(VecId v);
    virtual void m_solveSystem();
    virtual void m_print( std::ostream& out );
    
    /// @}
};

} // namespace simulation

} // namespace sofa

#endif
