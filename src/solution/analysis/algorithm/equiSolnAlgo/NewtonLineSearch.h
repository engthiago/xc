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
// $Date: 2005/11/29 22:42:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/NewtonLineSearch.h,v $

// Written: fmk 
// Created: 11/96 
// Modified: Ed "C++" Love 10/00 to perform the line search
//

// Description: This file contains the class definition for 
// NewtonLineSearch. NewtonLineSearch is a class which performs a Newton-Raphson 
// with line search solution algorithm in solving the equations as outline in
// Crissfields book.
// 
// What: "@(#)NewtonLineSearch.h, revA"

#ifndef NewtonLineSearch_h
#define NewtonLineSearch_h

#include <solution/analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.h>

namespace XC {

class LineSearch;

//! @ingroup EQSolAlgo
//
//! @brief performs a Newton-Raphson with line search solution algorithm
//! in solving the equations as outline in Crissfield's book.
//
//! In optimization, the line search strategy is one of two basic iterative
//! approaches to find a local minimum \f$\mathbf{x}^*\f$ of an objective
//! function \f$f:\mathbf {R} ^{n}\to \mathbf {R}\f$. The other approach is
//! trust region.
//!
//! The line search approach first finds a descent direction along which
//! the objective function \f$f\f$ will be reduced and then computes a step
//! size that determines how far \f$\mathbf {x}\f$ should move along that
//! direction. The descent direction can be computed by various methods, such
//! as gradient descent, Newton's method and quasi-Newton method.
class NewtonLineSearch: public EquiSolnAlgo
  {
  private:
    LineSearch *theLineSearch;

    friend class SolutionStrategy;
    friend class FEM_ObjectBroker;
    NewtonLineSearch(SolutionStrategy *);    
    NewtonLineSearch(const NewtonLineSearch &);    
    NewtonLineSearch &operator=(const NewtonLineSearch &);    
    virtual SolutionAlgorithm *getCopy(void) const;
  protected:
    ~NewtonLineSearch(void);
    
    void free_mem(void);
    bool alloc(const std::string &);
    void copy(LineSearch *);
  public:
    int solveCurrentStep(void);

    bool setLineSearchMethod(const std::string &);
    virtual int sendSelf(Communicator &);
    virtual int recvSelf(const Communicator &);

    void Print(std::ostream &s, int flag =0) const;    
  };
} // end of XC namespace

#endif


