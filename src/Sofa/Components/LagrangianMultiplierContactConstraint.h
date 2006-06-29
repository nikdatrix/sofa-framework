#ifndef SOFA_COMPONENTS_LagrangianMultiplierContactConstraint_H
#define SOFA_COMPONENTS_LagrangianMultiplierContactConstraint_H

#include "Sofa/Core/InteractionForceField.h"
#include "LagrangianMultiplierConstraint.h"
#include "Sofa/Abstract/VisualModel.h"

#include <vector>

namespace Sofa
{

namespace Components
{

template<class DataTypes>
class LagrangianMultiplierContactConstraint : public LagrangianMultiplierConstraint<DataTypes>, public Core::InteractionForceField, public Abstract::VisualModel
{
public:
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;
    typedef Common::StdVectorTypes<Real, Real, Real> LMTypes;
    typedef typename LMTypes::VecCoord LMVecCoord;
    typedef typename LMTypes::VecDeriv LMVecDeriv;

protected:
    Core::MechanicalModel<DataTypes>* object1;
    Core::MechanicalModel<DataTypes>* object2;

    struct Contact
    {
        int m1, m2;   ///< the two extremities of the spring: masses m1 and m2
        Deriv norm;   ///< contact normal, from m1 to m2
        Real dist;    ///< minimum distance between the points
        Real ks;      ///< spring stiffness
        Real mu_s;    ///< coulomb friction coefficient (currently unused)
        Real mu_v;    ///< viscous friction coefficient
        Real pen0;     ///< penetration at start of timestep
        Real pen;     ///< current penetration
    };

    std::vector<Contact> contacts;

public:

    LagrangianMultiplierContactConstraint(Core::MechanicalModel<DataTypes>* object1, Core::MechanicalModel<DataTypes>* object2)
        : object1(object1), object2(object2)
    {
    }

    LagrangianMultiplierContactConstraint(Core::MechanicalModel<DataTypes>* object)
        : object1(object), object2(object)
    {
    }

    Core::MechanicalModel<DataTypes>* getObject1() { return object1; }
    Core::MechanicalModel<DataTypes>* getObject2() { return object2; }
    Core::BasicMechanicalModel* getMechModel1() { return object1; }
    Core::BasicMechanicalModel* getMechModel2() { return object2; }

    void clear(int reserve = 0)
    {
        contacts.clear();
        if (reserve)
            contacts.reserve(reserve);
        this->lambda->resize(0);
    }

    void addContact(int m1, int m2, const Deriv& norm, Real dist, Real ks, Real mu_s = 0.0f, Real mu_v = 0.0f);

    virtual void addForce();

    virtual void addDForce();

    // -- VisualModel interface
    void draw();
    void initTextures() { }
    void update() { }
};

} // namespace Components

} // namespace Sofa

#endif
