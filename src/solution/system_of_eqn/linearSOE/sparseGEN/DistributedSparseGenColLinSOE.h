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
                                                                        
// $Revision: 1.1 $
// $Date: 2005/12/06 22:20:18 $
// $Source: /usr/local/cvs/OpenSees/SRC/system_of_eqn/linearSOE/sparseGEN/DistributedSparseGenColLinSOE.h,v $
                                                                        
#ifndef DistributedSparseGenColLinSOE_h
#define DistributedSparseGenColLinSOE_h

// Written: fmk 
// Description: This file contains the class definition for DistributedSparseGenColLinSOE
// DistributedSparseGenColLinSOE is a subclass of LinearSOE. It uses the sparse column
// storage scheme. It collects all contributions from the matrices and assembles them onto 
// P0 (not really a distributed SOE .. but until i get a solver that will work with a distributed
// col storage scheme this is what will have to stick with).
//
// What: "@(#) DistributedSparseGenColLinSOE.h, revA"


#include <solution/system_of_eqn/linearSOE/sparseGEN/SparseGenColLinSOE.h>
#include "solution/system_of_eqn/linearSOE/DistributedBandLinSOE.h"

namespace XC {
class DistributedSparseGenColLinSolver;

//! @ingroup LinearSOE
//
//! @brief Sparse nonsymmetric matrix distributed system of equations.
class DistributedSparseGenColLinSOE: public SparseGenColLinSOE, public DistributedBandLinSOE
  {
  protected:
    void calcNonZeros(Graph &);

    friend class SolutionStrategy;
    DistributedSparseGenColLinSOE(SolutionStrategy *);
    SystemOfEqn *getCopy(void) const;
  public:
    // these methods need to be rewritten
    int setSize(Graph &theGraph);
    int addA(const Matrix &, const ID &, double fact = 1.0);
    int addB(const Vector &, const ID &, const double &fact= 1.0);    
    int setB(const Vector &,const double &fact= 1.0);            
    const Vector &getB(void) const;
    int solve(void);


    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
    friend class SuperLU;    
    friend class ThreadedSuperLU;        
    friend class DistributedSuperLU;        

  };
inline SystemOfEqn *DistributedSparseGenColLinSOE::getCopy(void) const
  { return new DistributedSparseGenColLinSOE(*this); }
} // end of XC namespace

#endif

