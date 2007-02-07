#ifndef SOFA_CORE_COMPONENTMODEL_BEHAVIOR_ODESOLVER_H
#define SOFA_CORE_COMPONENTMODEL_BEHAVIOR_ODESOLVER_H

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/defaulttype/SofaBaseMatrix.h>
#include <sofa/helper/io/Encoding.h>

namespace sofa
{

namespace core
{

namespace componentmodel
{

namespace behavior
{

class OdeSolver : public objectmodel::BaseObject
{
public:
    OdeSolver();

    virtual ~OdeSolver();

    virtual void solve (double dt) = 0;

    /// Method called at initialization, during the backwards traversal of the data structure.
    virtual void bwdInit() {}

protected:
    defaulttype::SofaBaseMatrix *mat;

    /// @name Actions and MultiVectors
    /// This provides an abstract view of the mechanical system to animate
    /// @{


protected:

    typedef helper::io::VecId VecId;
    class VectorIndexAlloc
    {
    protected:
        std::set<unsigned int> vused; ///< Currently in-use vectors
        std::set<unsigned int> vfree; ///< Once used vectors
        unsigned int  maxIndex; ///< Max index used
    public:
        VectorIndexAlloc();
        unsigned int alloc();
        bool free(unsigned int v);
    };
    std::map<helper::io::VecType, VectorIndexAlloc > vectors; ///< Current vectors

    double result;
public:
    /// Wait for the completion of previous operations and return the result of the last v_dot call
    virtual double finish();

    virtual VecId v_alloc(helper::io::VecType t);
    virtual void v_free(VecId v);

    virtual void v_clear(VecId v); ///< v=0
    virtual void v_eq(VecId v, VecId a); ///< v=a
    virtual void v_peq(VecId v, VecId a, double f=1.0); ///< v+=f*a
    virtual void v_teq(VecId v, double f); ///< v*=f
    virtual void v_dot(VecId a, VecId b); ///< a dot b ( get result using finish )
    virtual void propagateDx(VecId dx);
    virtual void projectResponse(VecId dx);
    virtual void addMdx(VecId res, VecId dx);
    virtual void integrateVelocity(VecId res, VecId x, VecId v, double dt);
    virtual void accFromF(VecId a, VecId f);
    virtual void propagatePositionAndVelocity(double t, VecId x, VecId v);

    virtual void computeForce(VecId result);
    virtual void computeDf(VecId df);
    virtual void computeAcc(double t, VecId a, VecId x, VecId v);

    virtual void computeMatrix(defaulttype::SofaBaseMatrix *mat=NULL, double mFact=1.0, double bFact=1.0, double kFact=1.0, unsigned int offset=0);
    virtual void getMatrixDimension(unsigned int * const, unsigned int * const);
    virtual void computeOpVector(defaulttype::SofaBaseVector *vect=NULL, unsigned int offset=0);
    virtual void matResUpdatePosition(defaulttype::SofaBaseVector *vect=NULL, unsigned int offset=0);

    virtual void print( VecId v, std::ostream& out );
    /// @}
};

} // namespace behavior

} // namespace componentmodel

} // namespace core

} // namespace sofa

#endif


