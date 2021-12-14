// -*-c++-*-
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
                                                                        
// $Revision: 1.4 $
// $Date: 2005/11/29 22:42:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/BFGS.h,v $
                                                                        
#ifndef BFGS_h
#define BFGS_h

// File: ~/OOP/analysis/algorithm/BFGS.h 
// 
// Written: Ed Love
// Created: 06/01

// Description: This file contains the class definition for
// BFGS.
// 
// What: "@(#)BFGS.h, revA"

#include "solution/analysis/algorithm/equiSolnAlgo/BFBRoydenBase.h"
#include <solution/analysis/integrator/IncrementalIntegrator.h>

namespace XC {

//! @ingroup EQSolAlgo
//
//! @brief Broyden–Fletcher–Goldfarb–Shann solution algorithm.
//!
//! The BFGS method is one of the most effective matrix-update or
//! quasi Newton methods for iteration on a nonlinear system of
//! equations. The method computes new search directions at each
//! iteration step based on the initial jacobian, and subsequent trial
//! solutions. The unlike regular Newton-Raphson does not require the
//! tangent matrix be reformulated and refactored at every iteration, however
//! unlike ModifiedNewton it does not rely on the tangent matrix from a
//! previous iteration. 
class BFGS: public BFBRoydenBase
  {
  private:
    Vector b;  //current right-hand side
    std::vector<double> rdotz;
    std::vector<double> sdotr;

    void BFGSUpdate(IncrementalIntegrator *theIntegrator,
		    LinearSOE *theSOE, Vector &du, Vector &b, 
		    int count);
  protected:
    friend class SolutionStrategy;
    BFGS(SolutionStrategy *,int tangent = CURRENT_TANGENT, int n = 10);    
    BFGS(SolutionStrategy *,ConvergenceTest &theTest, int tangent = CURRENT_TANGENT, int n = 10);
    virtual SolutionAlgorithm *getCopy(void) const;
  public:
    int solveCurrentStep(void);    

    virtual int sendSelf(Communicator &);
    virtual int recvSelf(const Communicator &);

    void Print(std::ostream &s, int flag =0) const;    
  };
} // end of XC namespace

#endif


