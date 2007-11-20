#ifndef SOFA_COMPONENT_FORCEFIELD_ELLIPSOIDFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_ELLIPSOIDFORCEFIELD_H

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/core/componentmodel/behavior/MechanicalState.h>
#include <sofa/core/VisualModel.h>
#include <sofa/core/objectmodel/Data.h>

namespace sofa
{

namespace component
{

namespace forcefield
{

/// This class can be overridden if needed for additionnal storage within template specializations.
template<class DataTypes>
class EllipsoidForceFieldInternalData
{
public:
};

template<class DataTypes>
class EllipsoidForceField : public core::componentmodel::behavior::ForceField<DataTypes>, public core::VisualModel
{
public:
    typedef core::componentmodel::behavior::ForceField<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;
    enum { N=Coord::static_size };
    typedef defaulttype::Mat<N,N,Real> Mat;

protected:
    class Contact
    {
    public:
        int index;
        Mat m;
        Contact( int index=0, const Mat& m=Mat())
            : index(index), m(m)
        {
        }

        inline friend std::istream& operator >> ( std::istream& in, Contact& c )
        {
            in>>c.index>>c.m;
            return in;
        }

        inline friend std::ostream& operator << ( std::ostream& out, const Contact& c )
        {
            out << c.index << " " << c.m ;
            return out;
        }

    };

    Data<sofa::helper::vector<Contact> > contacts;

    EllipsoidForceFieldInternalData<DataTypes> data;

public:

    Data<Coord> center;
    Data<Coord> vradius;
    Data<Real> stiffness;
    Data<Real> damping;
    Data<defaulttype::Vec3f> color;
    Data<bool> bDraw;

    EllipsoidForceField()
        : contacts(initData(&contacts,"contacts", "Contacts"))
        , center(initData(&center, "center", "ellipsoid center"))
        , vradius(initData(&vradius, "vradius", "ellipsoid radius"))
        , stiffness(initData(&stiffness, (Real)500, "stiffness", "force stiffness (positive to repulse outward, negative inward)"))
        , damping(initData(&damping, (Real)5, "damping", "force damping"))
        , color(initData(&color, defaulttype::Vec3f(0.0f,0.5f,1.0f), "color", "ellipsoid color"))
        , bDraw(initData(&bDraw, true, "draw", "enable/disable drawing of the ellipsoid"))
    {
    }

    void setStiffness(Real stiff)
    {
        stiffness.setValue( stiff );
    }

    void setDamping(Real damp)
    {
        damping.setValue( damp );
    }

    virtual void addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);

    virtual void addDForce (VecDeriv& df, const VecDeriv& dx);

    virtual double getPotentialEnergy(const VecCoord& x);

    // -- VisualModel interface
    void draw();
    void initTextures() { }
    void update() { }
};

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif
