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

#include "UniaxialStateVars.h"

XC::UniaxialStateVars::UniaxialStateVars(void)
  :MovableObject(0), strain(0.0), stress(0.0), tangent(0.0) {}


int XC::UniaxialStateVars::revertToStart(const double &E)
  {
    strain= 0.0;
    stress= 0.0;
    tangent= E;
    return 0;
  }

//! @brief Send object members through the communicator argument.
int XC::UniaxialStateVars::sendData(Communicator &comm)
  {
    int res= comm.sendDoubles(strain,stress,tangent,getDbTagData(),CommMetaData(0));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::UniaxialStateVars::recvData(const Communicator &comm)
  {
    int res= comm.receiveDoubles(strain,stress,tangent,getDbTagData(),CommMetaData(0));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::UniaxialStateVars::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(10); 
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << "UniaxialStateVars::sendSelf - failed to send data.\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::UniaxialStateVars::recvSelf(const Communicator &comm)
  {
    inicComm(10);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << "UniaxialStateVars::recvSelf - failed to receive ids.\n";
    else
      {
        res+= recvData(comm);
        if(res<0)
           std::cerr << "UniaxialStateVars::recvSelf - failed to receive data.\n";
      }
    return res;
  }

void XC::UniaxialStateVars::Print(std::ostream &s, int flag) const
  {
    s << "UniaxialStateVars, strain: " << strain << std::endl;
    s << "  stress: " << stress << std::endl;
    s << "  tangent: " << tangent << std::endl;
  }


