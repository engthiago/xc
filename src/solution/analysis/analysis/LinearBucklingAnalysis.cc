//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  This program derives from OpenSees <http://opensees.berkeley.edu>
//  developed by the  «Pacific earthquake engineering research center».
//
//  Except for the restrictions that may arise from the copyright
//  of the original program (see copyright_opensees.txt)
//  XC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details. 
//
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//LinearBucklingAnalysis.cpp

#include "LinearBucklingAnalysis.h"
#include "solution/analysis/analysis/LinearBucklingEigenAnalysis.h"
#include "solution/analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.h"
#include "solution/analysis/algorithm/eigenAlgo/LinearBucklingAlgo.h"
#include "solution/analysis/model/AnalysisModel.h"
#include "solution/system_of_eqn/linearSOE/LinearSOE.h"
#include "solution/system_of_eqn/eigenSOE/ArpackSOE.h"
#include "solution/system_of_eqn/eigenSOE/BandArpackppSOE.h"
#include "solution/analysis/numberer/DOF_Numberer.h"
#include "solution/analysis/handler/ConstraintHandler.h"
#include "solution/analysis/convergenceTest/ConvergenceTest.h"
#include "solution/analysis/integrator/StaticIntegrator.h"
#include "solution/analysis/integrator/eigen/LinearBucklingIntegrator.h"
#include "domain/domain/Domain.h"
#include "solution/SolutionStrategy.h"
#include "solution/SolutionProcedure.h"


//! @brief Constructor.
//! 
XC::LinearBucklingAnalysis::LinearBucklingAnalysis(SolutionStrategy *static_analysis, SolutionStrategy *buckling_analysis)
  : StaticAnalysis(static_analysis), eigen_solu(buckling_analysis),
    linearBucklingEigenAnalysis(buckling_analysis),
    numModes(0), linear_buckling_analysis_step(0) {}

//! @brief Clears all object members (constraint handler, analysis model,...).
void XC::LinearBucklingAnalysis::clearAll(void)
  {
    // invoke the destructor on all the objects in the aggregation
    StaticAnalysis::clearAll();
    linearBucklingEigenAnalysis.clearAll();
  }

//! @brief Performs the analysis.
//!
//! @param numSteps: number of steps in the analysis
//! (for static analysis, if the loads are constant it's useless to
//! increase the number of steps so \p numSteps= 1)
int XC::LinearBucklingAnalysis::analyze(int numSteps)
  {
    assert(solution_strategy);
    CommandEntity *old= solution_strategy->Owner();
    solution_strategy->set_owner(this);
    assert(eigen_solu);
    CommandEntity *oldE= eigen_solu->Owner();
    eigen_solu->set_owner(this);
    linearBucklingEigenAnalysis.set_owner(getSolutionProcedure());

    int result = 0;

    for(int i=0; i<numSteps; i++)
      {
        if(i == linear_buckling_analysis_step)
          linearBucklingEigenAnalysis.setupPreviousStep(); //Prepara el linear buckling Analysis.

        if(i == (linear_buckling_analysis_step+1))
          linearBucklingEigenAnalysis.analyze(numModes); //Performs el linear buckling Analysis.

        result= run_analysis_step(i,numSteps);

        if(result < 0) //Fallo en run_analysis_step.
          return result;
      }
    solution_strategy->set_owner(old);
    eigen_solu->set_owner(oldE);
    return result;
  }

//! @brief Make the changes that a domain change requires.
int XC::LinearBucklingAnalysis::domainChanged(void)
  {
    int retval= StaticAnalysis::domainChanged();
    retval+= linearBucklingEigenAnalysis.domainChanged();
    return retval;
  }

//! @brief Set the solution algorithm to use for linear buckling analysis.
int XC::LinearBucklingAnalysis::setLinearBucklingAlgorithm(LinearBucklingAlgo &theLinearBucklingAlgorithm)
  { return linearBucklingEigenAnalysis.setAlgorithm(theLinearBucklingAlgorithm); }


//! @briefSets the integrator to use in the analysis de eigenvalues.
int XC::LinearBucklingAnalysis::setLinearBucklingIntegrator(LinearBucklingIntegrator &theLinearBucklingIntegrator)
  { return linearBucklingEigenAnalysis.setIntegrator(theLinearBucklingIntegrator); }

//! @brief Sets the linear system of equations to use in the analysis de eigenvalues.
int XC::LinearBucklingAnalysis::setArpackSOE(ArpackSOE &theEigenSOE)
  { return linearBucklingEigenAnalysis.setEigenSOE(theEigenSOE); }

//! @brief Returns the eigenvector that corresponds to the mode
//! being passed as parameter.
const XC::Vector &XC::LinearBucklingAnalysis::getEigenvector(int mode)
  { return linearBucklingEigenAnalysis.getEigenvector(mode); }

//! @brief Returns the normalized eigenvector that corresponds to the mode
//! being passed as parameter.
XC::Vector XC::LinearBucklingAnalysis::getNormalizedEigenvector(int mode)
  { return linearBucklingEigenAnalysis.getNormalizedEigenvector(mode); }

//! @brief Returns a Python list with the computed eigenvectors as lists.
boost::python::list XC::LinearBucklingAnalysis::getNormalizedEigenvectorsPy(void) const
  { return linearBucklingEigenAnalysis.getNormalizedEigenvectorsPy(); }


//! @brief Return a Python list with the component of the eigenvector
//! corresponding to the given mode.
boost::python::list XC::LinearBucklingAnalysis::getNormalizedEigenvectorPy(int mode) const
  { return linearBucklingEigenAnalysis.getNormalizedEigenvectorPy(mode); }

//! @brief Returns the eigenvalue that corresponds to the mode
//! being passed as parameter.
const double &XC::LinearBucklingAnalysis::getEigenvalue(int mode) const
  { return linearBucklingEigenAnalysis.getEigenvalue(mode); }

//! @brief Returns a Python list with the computed eigenvalues for each mode.
boost::python::list XC::LinearBucklingAnalysis::getEigenvaluesPy(void) const
  { return linearBucklingEigenAnalysis.getEigenvaluesPy(); }











