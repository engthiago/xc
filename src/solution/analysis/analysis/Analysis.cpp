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
/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:16 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/analysis/Analysis.cpp,v $
                                                                        
                                                                        
// File: ~/analysis/analysis/Analysis.cpp
// 
// Written: fmk 
// Created: 11/96
// Revision: A
//
// Description: This file contains the implementation of Analysis.
// Analysis is an abstract base class, i.e. no objects of it's
// type can be created. 
//
// What: "@(#) Analysis.C, revA"

#include <solution/analysis/analysis/Analysis.h>
#include "solution/analysis/ModelWrapper.h"
#include "solution/SolutionStrategy.h"
#include "solution/SolutionProcedure.h"
#include "solution/analysis/model/AnalysisModel.h"



//! @brief Constructor.
//!
//! All analysis are associated with a single solution method, this constructor
//! sets up the link.
//! @param s: solution strategy to which this analysis belongs.
XC::Analysis::Analysis(SolutionStrategy *s)
  :analysis_result(-100), solution_strategy(s) {}

int XC::Analysis::newStepDomain(AnalysisModel *theModel,const double &dT)
  { return theModel->newStepDomain(dT); }

XC::SolutionProcedure *XC::Analysis::getSolutionProcedure(void)
  { return dynamic_cast<SolutionProcedure *>(Owner()); }

const XC::SolutionProcedure *XC::Analysis::getSolutionProcedure(void) const
  { return dynamic_cast<const SolutionProcedure *>(Owner()); }


//! @brief Returns a pointer to the domain.
XC::Domain *XC::Analysis::getDomainPtr(void)
  {
    SolutionProcedure *ps= getSolutionProcedure();
    assert(ps);
    return ps->getDomainPtr();
  }

//! @brief Returns a pointer to the domain.
const XC::Domain *XC::Analysis::getDomainPtr(void) const
  {
    const SolutionProcedure *ps= getSolutionProcedure();
    assert(ps);
    return ps->getDomainPtr();
  }

//! @brief Returns a pointer to the constraint handler.
XC::ConstraintHandler *XC::Analysis::getConstraintHandlerPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getConstraintHandlerPtr();
    else
      return nullptr;
  }

//! @brief Returns a pointer to the DOF numberer.
XC::DOF_Numberer *XC::Analysis::getDOF_NumbererPtr(void) const
  {
    if(solution_strategy)
      return solution_strategy->getDOF_NumbererPtr();
    else
      return nullptr;
  }
//! @brief Returns a pointer to the analysis model.
XC::AnalysisModel *XC::Analysis::getAnalysisModelPtr(void) const
  {
    if(solution_strategy)
      return solution_strategy->getAnalysisModelPtr();
    else
      return nullptr;
  }

//! @brief Returns a pointer to the linear system of equations.
XC::LinearSOE *XC::Analysis::getLinearSOEPtr(void) const
  {
    if(solution_strategy)
      return solution_strategy->getLinearSOEPtr();
    else
      return nullptr;
  }

//! @brief Returns a pointer to the system of equations de eigenvalues.
XC::EigenSOE *XC::Analysis::getEigenSOEPtr(void) const
  {
    if(solution_strategy)
      return solution_strategy->getEigenSOEPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al integrator otherwise it returns nullptr.
XC::Integrator *XC::Analysis::getIntegratorPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getIntegratorPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al integrator otherwise it returns nullptr.
const XC::Integrator *XC::Analysis::getIntegratorPtr(void) const
  {
    if(solution_strategy)
      return solution_strategy->getIntegratorPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al integrator incremental otherwise it returns nullptr.
XC::IncrementalIntegrator *XC::Analysis::getIncrementalIntegratorPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getIncrementalIntegratorPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al EigenIntegrator otherwise it returns nullptr.
XC::EigenIntegrator *XC::Analysis::getEigenIntegratorPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getEigenIntegratorPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al LinearBucklingIntegrator otherwise it returns nullptr.
XC::LinearBucklingIntegrator *XC::Analysis::getLinearBucklingIntegratorPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getLinearBucklingIntegratorPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al EigenIntegrator otherwise it returns nullptr.
XC::TransientIntegrator *XC::Analysis::getTransientIntegratorPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getTransientIntegratorPtr();
    else
      return nullptr;
  }

//! @brief Returns, if possible, a pointer al StaticIntegrator otherwise it returns nullptr.
XC::StaticIntegrator *XC::Analysis::getStaticIntegratorPtr(void)
  {
    StaticIntegrator *retval= nullptr;
    if(solution_strategy)
      retval= solution_strategy->getStaticIntegratorPtr();
    return retval;
  }

//! @brief Return a pointer to the eigenproblem solution
//! algorithm (if it's not defined it returns nullptr).
XC::EigenAlgorithm *XC::Analysis::getEigenSolutionAlgorithmPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getEigenSolutionAlgorithmPtr();
    else
      return nullptr;
  }

//! @brief Return a pointer to the linear SOE solution
//! algorithm (if it's not defined it returns nullptr).
XC::EquiSolnAlgo *XC::Analysis::getEquiSolutionAlgorithmPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getEquiSolutionAlgorithmPtr();
    else
      return nullptr;
  }

//! @brief Return a pointer to the domain decomposition solution
//! algorithm (if it's not defined it returns nullptr).
XC::DomainDecompAlgo *XC::Analysis::getDomainDecompSolutionAlgorithmPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getDomainDecompSolutionAlgorithmPtr();
    else
      return nullptr;
  }

//! @brief Returns a pointer to the convergence test (only for suitable analysis).
XC::ConvergenceTest *XC::Analysis::getConvergenceTestPtr(void)
  {
    if(solution_strategy)
      return solution_strategy->getConvergenceTestPtr();
    else
      return nullptr;
  }

//! @brief Returns a pointer to the convergence test (only for suitable analysis).
const XC::ConvergenceTest *XC::Analysis::getConvergenceTestPtr(void) const
  {
    if(solution_strategy)
      return solution_strategy->getConvergenceTestPtr();
    else
      return nullptr;
  }

//! @brief Returns a pointer to the DomainSolver.
const XC::DomainSolver *XC::Analysis::getDomainSolver(void) const
  {
    std::cerr << "Analysis::getDomainSolver must be redefined in derived classes"
              << std::endl;
    return nullptr;
  }

//! @brief Returns a pointer to the DomainSolver.
XC::DomainSolver *XC::Analysis::getDomainSolver(void)
  {
    std::cerr << "Analysis::getDomainSolver must be redefined in derived classes"
              << std::endl;
    return nullptr;
  }

//! @brief Returns a pointer to the subdomain.
const XC::Subdomain *XC::Analysis::getSubdomain(void) const
  {
    std::cerr << "Analysis::getSubdomain must be redefined in derived classes"
              << std::endl;
    return nullptr;
  }

//! @brief Returns a pointer to the subdomain.
XC::Subdomain *XC::Analysis::getSubdomain(void)
  {
    std::cerr << "Analysis::getSubdomain must be redefined in derived classes"
              << std::endl;
    return nullptr;
  }

//! @brief Deletes all members (Constraint handler, analysis model,...).
void XC::Analysis::clearAll(void)
  {
    std::cerr << "Analysis::clearAll() DEPRECATED." << std::endl;
  }

//! @brief Sets the renumerador to use in the analysis.
int XC::Analysis::setNumberer(DOF_Numberer &theNewNumberer) 
  {
    if(solution_strategy)
      if(solution_strategy->getModelWrapperPtr())
        return solution_strategy->getModelWrapperPtr()->setNumberer(theNewNumberer);
    return 0;
  }

//! @brief Sets the linear system of equations to use in the analysis.
int XC::Analysis::setLinearSOE(LinearSOE &theNewSOE)
  {
    if(solution_strategy)
      return solution_strategy->setLinearSOE(theNewSOE);
    else
      return 0;
  }

//! @brief Sets the system of eigenvalues to use in the analysis.
int XC::Analysis::setEigenSOE(EigenSOE &theSOE)
  {
    if(solution_strategy)
      return solution_strategy->setEigenSOE(theSOE);
    else
      return 0;
  }

//! @brief Sets the integrator to use in the analysis.
int XC::Analysis::setIntegrator(Integrator &theNewIntegrator)
  {
    int retval= 0;
    if(solution_strategy)
      {
	const Integrator *oldIntegratorPtr= this->getIntegratorPtr();
	if(oldIntegratorPtr!=&theNewIntegrator)
            retval= solution_strategy->setIntegrator(theNewIntegrator);
      }
    return retval;
  }

//! @brief Set the solution algorithm to be used in the analysis.
int XC::Analysis::setAlgorithm(SolutionAlgorithm &theNewAlgorithm) 
  {
    if(solution_strategy)
      return solution_strategy->setAlgorithm(theNewAlgorithm);
    else
      return 0;
  }

void XC::Analysis::brokeConstraintHandler(const Communicator &comm,const ID &data)
  { solution_strategy->getModelWrapperPtr()->brokeConstraintHandler(comm,data); }

void XC::Analysis::brokeNumberer(const Communicator &comm,const ID &data)
  { solution_strategy->getModelWrapperPtr()->brokeNumberer(comm,data); }

void XC::Analysis::brokeAnalysisModel(const Communicator &comm,const ID &data)
  { solution_strategy->getModelWrapperPtr()->brokeAnalysisModel(comm,data); }

void XC::Analysis::brokeDDLinearSOE(const Communicator &comm,const ID &data)
  { solution_strategy->brokeDDLinearSOE(comm,data); }

void XC::Analysis::brokeLinearSOE(const Communicator &comm,const ID &data)
  { solution_strategy->brokeLinearSOE(comm,data); }

void XC::Analysis::brokeIncrementalIntegrator(const Communicator &comm,const ID &data)
  { solution_strategy->brokeIncrementalIntegrator(comm,data); }

void XC::Analysis::brokeStaticIntegrator(const Communicator &comm,const ID &data)
  { solution_strategy->brokeStaticIntegrator(comm,data); }

void XC::Analysis::brokeTransientIntegrator(const Communicator &comm,const ID &data)
  { solution_strategy->brokeTransientIntegrator(comm,data); }

void XC::Analysis::brokeDomainDecompAlgo(const Communicator &comm,const ID &data)
  { solution_strategy->brokeDomainDecompAlgo(comm,data); }

void XC::Analysis::brokeEquiSolnAlgo(const Communicator &comm,const ID &data)
  { solution_strategy->brokeEquiSolnAlgo(comm,data); }

