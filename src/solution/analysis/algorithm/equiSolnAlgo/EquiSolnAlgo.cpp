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
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.cpp,v $
                                                                        
                                                                        
// File: ~/OOP/analysis/algorithm/EquiSolnAlgo.C 
// 
// Written: fmk 
// Created: 11/96 
// Revision: A 
//

// Description: This file contains the class implementation for 
// EquiSolnAlgo. EquiSolnAlgo is an abstract base class, 
// i.e. no objects of it's type can be created.  Its subclasses deifine
// the sequence of operations to be performed in the analysis by static
// equilibrium of a finite element model.  
// 
// What: "@(#)EquiSolnAlgo.C, revA"


#include <solution/analysis/algorithm/equiSolnAlgo/EquiSolnAlgo.h>
#include <solution/analysis/model/AnalysisModel.h>
#include <solution/analysis/integrator/IncrementalIntegrator.h>
#include <solution/system_of_eqn/linearSOE/LinearSOE.h>
#include "solution/SolutionStrategy.h"

//! @brief Constructor.
//! @param owr: SolutionStrategy that owns this solution algorithm.
//! @param classTag: class identifier.
XC::EquiSolnAlgo::EquiSolnAlgo(SolutionStrategy *owr,int classTag)
  :SolutionAlgorithm(owr,classTag) {}

//! @brief Returns a pointer to the convergence test.
const XC::ConvergenceTest *XC::EquiSolnAlgo::getConvergenceTestPtr(void) const
  {
    const SolutionStrategy  *sm= getSolutionStrategy();
    assert(sm);
    return sm->getConvergenceTestPtr();
  }

//! @brief Returns a pointer to the convergence test.
XC::ConvergenceTest *XC::EquiSolnAlgo::getConvergenceTestPtr(void)
  {
    SolutionStrategy  *sm= getSolutionStrategy();
    assert(sm);
    return sm->getConvergenceTestPtr();
  }

//! @brief Returns a pointer to the incremental integrator.
XC::IncrementalIntegrator *XC::EquiSolnAlgo::getIncrementalIntegratorPtr(void)
  { return dynamic_cast<IncrementalIntegrator *>(getIntegratorPtr()); }

//! Send information to the stream based on the integer \p flag.
void XC::EquiSolnAlgo::Print(std::ostream &s, int flag) const
  {
    s << getClassName() << std::endl;
  }
