#include <sofa/component/constraint/LagrangianMultiplierFixedConstraint.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/simulation/tree/xml/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace constraint
{

SOFA_DECL_CLASS(LagrangianMultiplierFixedConstraint)

using namespace sofa::defaulttype;

template class LagrangianMultiplierFixedConstraint<Vec3dTypes>;
template class LagrangianMultiplierFixedConstraint<Vec3fTypes>;

namespace helper   // \todo Why this must be inside helper namespace
{

template<class DataTypes>
void create(LagrangianMultiplierFixedConstraint<DataTypes>*& obj, simulation::tree::xml::ObjectDescription* arg)
{
    XML::createWithParent< LagrangianMultiplierFixedConstraint<DataTypes>, core::componentmodel::behavior::MechanicalState<DataTypes> >(obj, arg);
}

}

Creator<simulation::tree::xml::ObjectFactory, LagrangianMultiplierFixedConstraint<Vec3dTypes> > LagrangianMultiplierFixedConstraintVec3dClass("LagrangianMultiplierFixedConstraint", true);
Creator<simulation::tree::xml::ObjectFactory, LagrangianMultiplierFixedConstraint<Vec3fTypes> > LagrangianMultiplierFixedConstraintVec3fClass("LagrangianMultiplierFixedConstraint", true);

} // namespace constraint

} // namespace component

} // namespace sofa

