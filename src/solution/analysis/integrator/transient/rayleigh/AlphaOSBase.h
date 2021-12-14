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
//AlphaOSBase.h


#ifndef AlphaOSBase_h
#define AlphaOSBase_h

#include "solution/analysis/integrator/transient/rayleigh/HHTBase.h"

namespace XC {
class Vector;

//! @ingroup RayleighIntegrator
//
//! @brief AlphaOSBase is an algorithmic class for performing a transient
//! analysis using the Alpha-Operator-Splitting integration scheme.
//! The parameter alpha corresponds to 1+alpha_{HHT}.
class AlphaOSBase: public HHTBase
  {
  protected:
    int updateCount; //! method should only have one update per step
    ResponseQuantities Upt; //!< predictor quantities at time t
  protected:
    int sendData(Communicator &);
    int recvData(const Communicator &);

    AlphaOSBase(SolutionStrategy *,int classTag);
    AlphaOSBase(SolutionStrategy *,int classTag,double beta, double gamma);
    AlphaOSBase(SolutionStrategy *,int classTag,double beta, double gamma,const RayleighDampingFactors &rF);
  public:
    int newStep(double deltaT);    
    int revertToLastStep(void);
  };
} // end of XC namespace

#endif
