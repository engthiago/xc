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
//LinearBucklingEigenAnalysis.cpp


#include <solution/analysis/analysis/LinearBucklingEigenAnalysis.h>
#include <solution/analysis/algorithm/eigenAlgo/LinearBucklingAlgo.h>
#include <solution/analysis/model/AnalysisModel.h>
#include <solution/system_of_eqn/eigenSOE/ArpackSOE.h>
#include <solution/analysis/numberer/DOF_Numberer.h>
#include <solution/analysis/handler/ConstraintHandler.h>
#include <solution/analysis/integrator/eigen/LinearBucklingIntegrator.h>
#include <domain/domain/Domain.h>


//! @brief Constructor.
XC::LinearBucklingEigenAnalysis::LinearBucklingEigenAnalysis(SolutionStrategy *analysis_aggregation)
  : EigenAnalysis(analysis_aggregation) {}

//! @brief Virtual constructor.
XC::Analysis *XC::LinearBucklingEigenAnalysis::getCopy(void) const
  { return new LinearBucklingEigenAnalysis(*this); }

//! @brief Performs the analysis.
int XC::LinearBucklingEigenAnalysis::setupPreviousStep(void)
  {
    int result = 0;
    Domain *the_Domain = this->getDomainPtr();

    // check for change in XC::Domain since last step. As a change can
    // occur in a commit() in a domaindecomp with load balancing
    // this must now be inside the loop
    int stamp = the_Domain->hasDomainChanged();
    if(stamp != domainStamp)
      {
	domainStamp = stamp;
	result = domainChanged();
	if(result < 0)
          {
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; domainChanged failed\n";
	    return -1;
	  }
      }
    result = getLinearBucklingIntegratorPtr()->formKt(); //Previous step stiffness matrix.
    return result;
  }

//! @brief Sets the algorithm to use in the analysis.
int XC::LinearBucklingEigenAnalysis::setAlgorithm(LinearBucklingAlgo &theAlgo)
  { return EigenAnalysis::setAlgorithm(theAlgo); }

//! @brief Sets the integrator to use in the analysis.
int XC::LinearBucklingEigenAnalysis::setIntegrator(LinearBucklingIntegrator &theIntegrator)
  { return EigenAnalysis::setIntegrator(theIntegrator); }

//! @brief Sets the system of eigenvalues to use in the analysis.
int XC::LinearBucklingEigenAnalysis::setEigenSOE(ArpackSOE &theSOE)
  { return EigenAnalysis::setEigenSOE(theSOE); }

//! @brief Returns the eigenvalue that corresponds to the mode
//! being passed as parameter.
const double &XC::LinearBucklingEigenAnalysis::getEigenvalue(int mode) const
  {
    const double &gamma= EigenAnalysis::getEigenvalue(mode);
    static double retval= 0.0;
    retval= 1.0/(1.0-gamma); //Bathe page 632.
    return retval;
  }

