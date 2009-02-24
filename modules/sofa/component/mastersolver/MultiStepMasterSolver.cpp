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
#include <sofa/component/mastersolver/MultiStepMasterSolver.h>
#include <sofa/core/ObjectFactory.h>
#include <math.h>
#include <iostream>

using std::cerr;
using std::endl;

namespace sofa
{

namespace component
{

namespace mastersolver
{

int MultiStepMasterSolverClass = core::RegisterObject("The simplest master solver, equivalent to the default behavior when no master solver is used.")
.add< MultiStepMasterSolver >()
;

SOFA_DECL_CLASS(MultiStepMasterSolver);

MultiStepMasterSolver::MultiStepMasterSolver()
: collisionSteps( initData(&collisionSteps,1,"collisionSteps", "number of collision steps between each frame rendering") )
, integrationSteps( initData(&integrationSteps,1,"integrationSteps", "number of integration steps between each collision detection") )
{
}

void MultiStepMasterSolver::step(double dt)
{
    const int ncollis = collisionSteps.getValue();
    const int ninteg = integrationSteps.getValue();
    dt /= (ncollis*ninteg);
    for (int c=0;c<ncollis;++c)
    {
        // First do collision detection and response creation
        if (this->f_printLog.getValue()) std::cout << "collision" << std::endl;
        computeCollision();
        for (int i=0;i<ninteg;++i)
        {
            // Then integrate the time step
            if (this->f_printLog.getValue()) std::cout << "integration" << std::endl;
            integrate(dt);
        }
    }
}

} // namespace mastersolver

} // namespace component

} // namespace sofa

