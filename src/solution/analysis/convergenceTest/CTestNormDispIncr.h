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
// $Date: 2005/12/15 00:19:28 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/convergenceTest/CTestNormDispIncr.h,v $

// Written: fmk 
// Date: 09/98
// Modified: 05/05 ahs
//
// Purpose: This file contains the class definition for CTestNormDispIncr.
// A CTestNormDispIncr object tests for convergence using the norm of the 
// solution vector of the LinearSOE object passed in the constructor
// and a tolerance, set in the constructor

#ifndef CTestNormDispIncr_h
#define CTestNormDispIncr_h

#include <solution/analysis/convergenceTest/ConvergenceTestTol.h>

namespace XC {

//! @ingroup CTest
//
//! @brief Specifies a tolerace for the displacement increment norm
//! to be used on each iteration.
//!
//! A CTestNormDispIncr object is an object which can be used in an
//! algorithmic class to test if convergence has been achieved. The
//! CTestNormDispIncr class tests using the norm of the solution Vector of
//! a LinearSOE object and a user specified tolerance value. 
class CTestNormDispIncr: public ConvergenceTestTol
  {
  public:
    // constructors
    CTestNormDispIncr(CommandEntity *owr= nullptr);	    	
    CTestNormDispIncr(CommandEntity *owr,double tol, int maxNumIter, int printFlag, int normType=2);

    ConvergenceTest *getCopy(void) const;
    
    virtual std::string getStatusMsg(const int &flag= 1) const;
    int test(void);
  };
} // end of XC namespace

#endif
