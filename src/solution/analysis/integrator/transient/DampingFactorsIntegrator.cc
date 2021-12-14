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
//DampingFactors.cpp

#include "DampingFactorsIntegrator.h"


//! @brief Constructor.
//!
//! @param owr: analysis aggregation that will own this object.
//! @param classTag: class identifier.
XC::DampingFactorsIntegrator::DampingFactorsIntegrator(SolutionStrategy *owr,int classTag)
  : TransientIntegrator(owr,classTag), rayFactors() {}

//! @brief Constructor.
//!
//! @param owr: analysis aggregation that will own this object.
//! @param classTag: class identifier.
//! @param fF: Rayleigh damping factors.
XC::DampingFactorsIntegrator::DampingFactorsIntegrator(SolutionStrategy *owr,int classTag,const RayleighDampingFactors &rF)
  : TransientIntegrator(owr,classTag), rayFactors(rF) {}

void XC::DampingFactorsIntegrator::setRayleighDampingFactors(void)
  {
    // if damping factors exist set them in the ele & node of the domain
    if(!rayFactors.nullValues())
      Integrator::setRayleighDampingFactors(rayFactors);
  }

void XC::DampingFactorsIntegrator::Print(std::ostream &s, int flag) const
  {
    TransientIntegrator::Print(s,flag);
    s << "  Rayleigh Damping: " << rayFactors << std::endl;
  }

//! @brief Send object members through the communicator argument.
int XC::DampingFactorsIntegrator::sendData(Communicator &comm)
  {
    int res= TransientIntegrator::sendData(comm);
    res+= comm.sendMovable(rayFactors,getDbTagData(),CommMetaData(2));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::DampingFactorsIntegrator::recvData(const Communicator &comm)
  {
    int res= TransientIntegrator::recvData(comm);
    res+= comm.receiveMovable(rayFactors,getDbTagData(),CommMetaData(2));
    return res;
  }
