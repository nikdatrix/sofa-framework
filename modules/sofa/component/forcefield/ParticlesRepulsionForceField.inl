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
#ifndef SOFA_COMPONENT_FORCEFIELD_PARTICLESREPULSIONFORCEFIELD_INL
#define SOFA_COMPONENT_FORCEFIELD_PARTICLESREPULSIONFORCEFIELD_INL

#include <sofa/core/componentmodel/behavior/ForceField.inl>
#include <sofa/component/forcefield/ParticlesRepulsionForceField.h>
#include <sofa/component/container/SpatialGridContainer.inl>
#include <sofa/helper/system/config.h>
#include <sofa/helper/gl/template.h>
#include <math.h>
#include <iostream>




namespace sofa
{

namespace component
{

namespace forcefield
{

template<class DataTypes>
ParticlesRepulsionForceField<DataTypes>::ParticlesRepulsionForceField()
: distance (initData(&distance, Real(1)   , "distance", "Distance to maintain between particles"))
, stiffness(initData(&stiffness, Real(100), "stiffness", "Stiffness"))
, damping  (initData(&damping  , Real(0.1), "damping", "Damping"))
, grid(NULL)
{
}

template<class DataTypes>
void ParticlesRepulsionForceField<DataTypes>::init()
{
	this->Inherit::init();
	this->getContext()->get(grid); //new Grid(distance.getValue());
	if (grid==NULL)
	    serr<<"SpatialGridContainer not found by ParticlesRepulsionForceField, slow O(n2) method will be used !!!" << sendl;
	//int n = (*this->mstate->getX()).size();
	//particles.resize(n);
	//for (int i=0;i<n;i++)
	//{
	//	particles[i].neighbors.clear();
	//}
}

template<class DataTypes>
void ParticlesRepulsionForceField<DataTypes>::addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v)
{
	const Real h = distance.getValue();
	const Real h2 = h*h;
	const Real ks = stiffness.getValue();
	const Real kd = damping.getValue();
	const int n = x.size();

	// Initialization
	f.resize(n);
	particles.resize(n);
	for (int i=0;i<n;i++)
	{
		particles[i].neighbors.clear();
	}

	// First compute the neighbors
	// This is an O(n2) step, except if a hash-grid is used to optimize it
	if (grid == NULL)
	{
		for (int i=0;i<n;i++)
		{
			const Coord& ri = x[i];
			for (int j=i+1;j<n;j++)
			{
				const Coord& rj = x[j];
				Real r2 = (rj-ri).norm2();
				if (r2 < h2)
					particles[i].neighbors.push_back(j);
			}
		}
	}
	else
	{
		grid->updateGrid(x);
		grid->findNeighbors(this, h);
	}

	// Compute the forces
	for (int i=0;i<n;i++)
	{
		Particle& Pi = particles[i];

		for (typename std::vector< int >::const_iterator it = Pi.neighbors.begin(); it != Pi.neighbors.end(); ++it)
		{
			const int j = *it;

			Deriv force;

            Deriv n = (x[j]-x[i]);
            Real d2 = n.norm2();

            if (d2 < h2)
            {
                Deriv vi = v[j]-v[i];
                Real d = sqrt(d2);
                n /= d;
                d -= h;
                Real forceIntensity = ks*d;
                Real dampingIntensity = kd*d;
                force = n*forceIntensity - vi*dampingIntensity;
            }

			f[i] += force;
			f[j] -= force;
		}
	}
}

template<class DataTypes>
void ParticlesRepulsionForceField<DataTypes>::addDForce(VecDeriv& df, const VecDeriv& dx, double kFactor, double /*bFactor*/)
{
    const VecCoord& x = *this->mstate->getX();
	const Real h = distance.getValue();
	const Real h2 = h*h;
	const Real ks = stiffness.getValue()*kFactor;
	//const Real kd = damping.getValue()*bFactor;
	const int n = x.size();
    df.resize(dx.size());

	// Compute the forces
	for (int i=0;i<n;i++)
	{
		Particle& Pi = particles[i];

		for (typename std::vector< int >::const_iterator it = Pi.neighbors.begin(); it != Pi.neighbors.end(); ++it)
		{
			const int j = *it;

			Deriv dforce;

            Deriv n = (x[j]-x[i]);
            Real d2 = n.norm2();

            if (d2 < h2)
            {
                Deriv dxi = dx[j]-dx[i];
                dforce = n*((ks * dot(dxi,n))/d2);
            }

			df[i] += dforce;
			df[j] -= dforce;
		}
	}
}

template <class DataTypes>
double ParticlesRepulsionForceField<DataTypes>::getPotentialEnergy(const VecCoord&)
{
    serr<<"ParticlesRepulsionForceField::getPotentialEnergy-not-implemented !!!"<<sendl;
    return 0;
}


template<class DataTypes>
void ParticlesRepulsionForceField<DataTypes>::draw()
{
	if (!getContext()->getShowForceFields() && !getContext()->getShowInteractionForceFields()) return;
	//if (grid != NULL)
	//	grid->draw();
	const VecCoord& x = *this->mstate->getX();
    const Real h = distance.getValue();
	glDisable(GL_LIGHTING);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthMask(0);
	glColor3f(0,1,1);
	glLineWidth(1);
	glBegin(GL_LINES);
	for (unsigned int i=0;i<particles.size();i++)
	{
		Particle& Pi = particles[i];
		for (typename std::vector< int >::const_iterator it = Pi.neighbors.begin(); it != Pi.neighbors.end(); ++it)
		{
			const int j = *it;
			const float r_h = (float)((x[j]-x[i]).norm() / h);
			float f = r_h*2;
			if (f < 1)
			{
				glColor4f(0,1-f,f,1-r_h);
			}
			else
			{
				glColor4f(f-1,0,2-f,1-r_h);
			}
			helper::gl::glVertexT(x[i]);
			helper::gl::glVertexT(x[j]);
		}
	}
	glEnd();
	glDisable(GL_BLEND);
	glDepthMask(1);
}

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif
