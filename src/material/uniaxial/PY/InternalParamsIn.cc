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
//InternalParamsIn.cc

#include "material/uniaxial/PY/InternalParamsIn.h"

//! @brief Constructor with data
XC::InternalParamsIn::InternalParamsIn(const double &V,const double &r,const double &t,const double &Vin,const double &rin)
  : InternalParamsA(V,r,t), Rin(rin), vin(Vin) {}

void XC::InternalParamsIn::revertToStart(const double &tg)
  {
    vin= 0.0;
    Rin= 0.0;
    InternalParamsA::revertToStart(tg);
  }

//! @brief Send object members through the communicator argument.
int XC::InternalParamsIn::sendData(Communicator &comm)
  {
    int res= InternalParamsA::sendData(comm);
    res+= comm.sendDoubles(Rin,vin,getDbTagData(),CommMetaData(1));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::InternalParamsIn::recvData(const Communicator &comm)
  {
    int res= InternalParamsA::recvData(comm);
    res+= comm.receiveDoubles(Rin,vin,getDbTagData(),CommMetaData(1));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::InternalParamsIn::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(2); 
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << "InternalParamsIn::sendSelf - failed to send data.\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::InternalParamsIn::recvSelf(const Communicator &comm)
  {
    inicComm(2);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << "InternalParamsIn::recvSelf - failed to receive ids.\n";
    else
      {
        res+= recvData(comm);
        if(res<0)
           std::cerr << "InternalParamsIn::recvSelf - failed to receive data.\n";
      }
    return res;
  }
