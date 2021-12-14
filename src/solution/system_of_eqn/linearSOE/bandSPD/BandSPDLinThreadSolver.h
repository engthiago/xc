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
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:29 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/bandSPD/BandSPDLinThreadSolver.h,v $
                                                                        
                                                                        
// File: ~/system_of_eqn/linearSOE/bandSPD/BandSPDLinThreadSolver.h
//
// Written: fmk 
// Created: Mar 1998
// Revision: A
//
// Description: This file contains the class definition for 
// BandSPDLinThreadSolver. It solves the BandSPDLinSOE in parallel
// using solaris threads.
//
// What: "@(#) BandSPDLinThreadSolver.h, revA"

#ifndef BandSPDLinThreadSolver_h
#define BandSPDLinThreadSolver_h

#include <solution/system_of_eqn/linearSOE/bandSPD/BandSPDLinSolver.h>

namespace XC {
//! @ingroup LinearSolver
//
//! @brief Solves the BandSPDLinSOE in parallel
//! using solaris threads.
class BandSPDLinThreadSolver: public BandSPDLinSolver
  {
  private:
    int NP;
    int running;
    int blockSize;

    BandSPDLinThreadSolver(void);    
    BandSPDLinThreadSolver(int numProcessors, int blockSize);        
    virtual LinearSOESolver *getCopy(void) const;
  public:

    int solve(void);
    int setSize(void);
    
    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);  
    
  };

inline LinearSOESolver *BandSPDLinThreadSolver::getCopy(void) const
   { return new BandSPDLinThreadSolver(*this); }
} // end of XC namespace

#endif


