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
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_FORCEFIELD_TRIANGULARBIQUADRATICSPRINGSFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_TRIANGULARBIQUADRATICSPRINGSFORCEFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/core/componentmodel/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/component/topology/TriangleData.h>
#include <sofa/component/topology/EdgeData.h>


namespace sofa
{

namespace component
{


namespace forcefield
{
using namespace sofa::helper;
using namespace sofa::defaulttype;
using namespace sofa::component::topology;


template<class DataTypes>
class TriangularBiquadraticSpringsForceField : public core::componentmodel::behavior::ForceField<DataTypes>
{
public:
  SOFA_CLASS(SOFA_TEMPLATE(TriangularBiquadraticSpringsForceField, DataTypes), SOFA_TEMPLATE(core::componentmodel::behavior::ForceField, DataTypes));

	typedef core::componentmodel::behavior::ForceField<DataTypes> Inherited;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord    Coord   ;
    typedef typename DataTypes::Deriv    Deriv   ;
    typedef typename Coord::value_type   Real    ;


	class Mat3 : public fixed_array<Deriv,3>
	{
	public:
		Deriv operator*(const Deriv& v)
		{
			return Deriv((*this)[0]*v,(*this)[1]*v,(*this)[2]*v);
		}
		Deriv transposeMultiply(const Deriv& v) 
		{
			return Deriv(v[0]*((*this)[0])[0]+v[1]*((*this)[1])[0]+v[2]*((*this)[2][0]),
				v[0]*((*this)[0][1])+v[1]*((*this)[1][1])+v[2]*((*this)[2][1]),
				v[0]*((*this)[0][2])+v[1]*((*this)[1][2])+v[2]*((*this)[2][2]));
		}
	};

protected: 


  class EdgeRestInformation
	{
	public:
		Real  restSquareLength;	// the rest length  
		Real  currentSquareLength; 	// the current edge length 
		Real  deltaL2;  // the current unit direction
		Real stiffness;
		
		EdgeRestInformation() {
		}

		/// Output stream
		inline friend std::ostream& operator<< ( std::ostream& os, const EdgeRestInformation& /*eri*/ )
		{
		return os;
		}
		
		/// Input stream
		inline friend std::istream& operator>> ( std::istream& in, EdgeRestInformation& /*eri*/ )
		{
		return in;
		}
	};

   class TriangleRestInformation
	{
	public:
		Real  gamma[3];	// the angular stiffness 
		Real stiffness[3]; // the elongation stiffness
		Mat3 DfDx[3]; /// the edge stiffness matrix

		Coord currentNormal;
		Coord lastValidNormal;
		Real area;
		Real restArea;
		Coord areaVector[3];
		Deriv dp[3];
		Real J;

		TriangleRestInformation() {
		}
		/// Output stream
		inline friend std::ostream& operator<< ( std::ostream& os, const TriangleRestInformation& /*tri*/ )
		{
		return os;
		}
		
		/// Input stream
		inline friend std::istream& operator>> ( std::istream& in, TriangleRestInformation& /*vec*/ )
		{
		return in;
		}
	};
	
	TriangleData<TriangleRestInformation> triangleInfo;
	EdgeData<EdgeRestInformation> edgeInfo;
		
	sofa::core::componentmodel::topology::BaseMeshTopology* _topology;
	Data < VecCoord >  _initialPoints;										///< the intial positions of the points

	bool updateMatrix;

	Data<Real> f_poissonRatio;
	Data<Real> f_youngModulus;
	Data<Real> f_dampingRatio;
	Data<bool> f_useAngularSprings; // whether angular springs should be included

	Data<bool> f_compressible; // whether the material is compressible or not
	/**** coefficient that controls how the material can cope with very compressible cases
	must be between 0 and 1 : if 0 then the deformation may diverge for large compression
	if 1 then the material can undergo large compression even inverse elements ***/
	Data<Real> f_stiffnessMatrixRegularizationWeight; 

	Real lambda;  /// first Lam� coefficient
	Real mu;    /// second Lam� coefficient
public:

	TriangularBiquadraticSpringsForceField();

	virtual ~TriangularBiquadraticSpringsForceField();

        virtual double getPotentialEnergy(const VecCoord& x) const;

	virtual void init();
	virtual void addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v);
	virtual void addDForce(VecDeriv& df, const VecDeriv& dx);

	virtual Real getLambda() const { return lambda;}
	virtual Real getMu() const { return mu;}

	// handle topological changes
	virtual void handleTopologyChange();

	void setYoungModulus(const double modulus) {
		f_youngModulus.setValue((Real)modulus);
	}
	void setPoissonRatio(const double ratio) {
		f_poissonRatio.setValue((Real)ratio);
	}

	void draw();
	/// compute lambda and mu based on the Young modulus and Poisson ratio
	void updateLameCoefficients();



protected :

	EdgeData<EdgeRestInformation> &getEdgeInfo() {return edgeInfo;}

    static void TRBSEdgeCreationFunction(int edgeIndex, void* param, EdgeRestInformation &ei,
                                         const Edge& ,  const sofa::helper::vector< unsigned int > &,
                                         const sofa::helper::vector< double >&);
	static void TRBSTriangleCreationFunction (int , void* , 
							TriangleRestInformation &,
							const Triangle& , const sofa::helper::vector< unsigned int > &, const sofa::helper::vector< double >&);
									

	static void TRBSTriangleDestroyFunction ( int , void* , TriangleRestInformation &);

};
} //namespace forcefield

} // namespace Components


} // namespace Sofa



#endif /* _TriangularBiquadraticSpringsForceField_H_ */
