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
//SparseGenSOEBase.cpp

#include <solution/system_of_eqn/linearSOE/sparseGEN/SparseGenSOEBase.h>

//! @brief Constructor.
//!
//! @param owr: analysis aggregation that owns this object.
//! @param classTag: class identifier.
//! @param N: system size.
//! @param NNZ: number of non-zeros.
XC::SparseGenSOEBase::SparseGenSOEBase(SolutionStrategy *owr,int classTag,int N, int NNZ)
  :SparseSOEBase(owr,classTag,N,NNZ) {}

//! @brief Zeros the entries in the 1d array for \f$A\f$ and marks the system
//! as not having been factored.
void XC::SparseGenSOEBase::zeroA(void)
  {
    A.Zero();
    factored = false;
  }

