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

// $Revision: 1.3 $
// $Date: 2005/11/29 22:42:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/Linear.cpp,v $


// File: ~/OOP/analysis/algorithm/Linear.C
//
// Written: fmk
// Created: Sun Sept 15 15:06:47: 1996
// Revision: A
//

// Description: This file contains the class definition for
// Linear. Linear is a class which performs a linear solution algorithm
// to solve the equations. No member functions are declared as virtual as
// it is not expected that this class will be subclassed.
//
// What: "@(#)Linear.C, revA"

#include <solution/analysis/algorithm/equiSolnAlgo/Linear.h>
#include <solution/analysis/model/AnalysisModel.h>
#include <solution/analysis/analysis/StaticAnalysis.h>
#include <solution/analysis/integrator/StaticIntegrator.h>
#include <solution/system_of_eqn/linearSOE/LinearSOE.h>
#include <utility/matrix/Vector.h>
#include <utility/Timer.h>

//! @brief Constructor
XC::Linear::Linear(SolutionStrategy *owr)
  :EquiSolnAlgo(owr,EquiALGORITHM_TAGS_Linear) {}

XC::SolutionAlgorithm *XC::Linear::getCopy(void) const
  { return new Linear(*this); }

//! @brief Performs the linear solution algorithm.
int XC::Linear::resuelve(void)
  {
    AnalysisModel *theAnalysisModel = getAnalysisModelPtr();
    LinearSOE *theSOE = getLinearSOEPtr();
    IncrementalIntegrator *theIncIntegrator= getIncrementalIntegratorPtr();

    if((!theAnalysisModel) || (!theIncIntegrator) || (!theSOE))
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
                  << "; WARNING undefined analysis model,"
                  << " integrator or system of equations.\n";
        return -5;
      }

    if(theIncIntegrator->formTangent()<0) //Builds tangent stiffness matrix.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
                  << "; WARNING the XC::Integrator"
                  << " failed in formTangent().\n";
        return -1;
      }

    if(theIncIntegrator->formUnbalance()<0) //Builds load vector.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
                  << "; WARNING the XC::Integrator"
                  << " failed in formUnbalance().\n";
        return -2;
      }

    if(theSOE->solve() < 0) //launches SOE solution.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
                  << "; WARNING the " << theSOE->getClassName()
                  << " failed in solve()\n";        
        return -3;
      }

    const Vector &deltaU = theSOE->getX(); //Gets the displacement vector.

    if(theIncIntegrator->update(deltaU) < 0) //Updates displacements.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
                  << "the integrator failed in update()\n";        
        return -4;
      }
    return 0;
  }

//! @brief Performs the linear solution algorithm.
//!
//! This method performs the linear solution algorithm:
//! \begin{tabbing}
//! while \= \+ whilewhilewhilewhilewhilewhilewhilewhilewhile \= \kill
//! theIntegrator-\f$>\f$formTangent() \+ // form \f$K_{a}\f$ \-
//! theIntegrator-\f$>\f$formUnbalance() // form \f$R(U_{a})\f$
//! theSOE-\f$>\f$solveX() // solve for \f$\Delta U\f$
//! theIntegrator-\f$>\f$update(theSOE-\f$>\f$getX()) // set \f$U = U_{a} + \Delta U\f$ \-  
//! \end{tabbing}
//! The method returns a 0 if successful, otherwise warning message is
//! printed and a negative number is returned; a \f$-1\f$ if error during {\em
//! formTangent()}, a \f$-2\f$ if error during formUnbalance(), a \f$-3\f$
//! if error during solve(), a \f$-4\f$ if error during {\em
//! update()}. If an error occurs in any of the above operations the
//! method stops at that routine, none of the subsequent operations are
//! invoked. A \f$-5\f$ is returned if any one of the links has not been
//! setup.
int XC::Linear::solveCurrentStep(void)
  {
    // set up some pointers and check they are valid
    // NOTE this could be taken away if we set Ptrs as protected in superclass
    return resuelve();
  }

//! @brief Sets the convergence test to use in the analysis.
int XC::Linear::setConvergenceTest(ConvergenceTest *theNewTest)
  { return 0; }

//! Does  nothing. Returns 0.
int XC::Linear::sendSelf(Communicator &comm)
  { return 0; }

//! Does nothing. Returns 0.
int XC::Linear::recvSelf(const Communicator &comm)
  { return 0; }


//! Sends the class name to the stream.
void XC::Linear::Print(std::ostream &s, int flag) const
  { s << "\t " << getClassName() << " algorithm"; }
