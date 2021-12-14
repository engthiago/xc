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
//ResponseQuantities.cpp

#include "ResponseQuantities.h"
#include <utility/actor/objectBroker/FEM_ObjectBroker.h>
#include "utility/actor/actor/CommMetaData.h"

XC::ResponseQuantities::ResponseQuantities(const int &size)
  :MovableObject(0), R(size), Rdot(size), Rdotdot(size) {}


void XC::ResponseQuantities::resize(const int &size)
  {  
    R.resize(size);
    Rdot.resize(size);
    Rdotdot.resize(size);
  }

void XC::ResponseQuantities::setDisp(const ID &id,const Vector &disp)
  {
    const int idSize = id.Size();
    for(int i=0;i<idSize; i++)
      {
        const int loc = id(i);
        if(loc >= 0)
          { R(loc) = disp(i); }
      }
  }

void XC::ResponseQuantities::setVel(const ID &id,const Vector &vel)
  {
    const int idSize = id.Size();
    for(int i=0;i<idSize;i++)
      {
        const int loc = id(i);
        if(loc >= 0)
         { Rdot(loc) = vel(i); }
      }
  }

void XC::ResponseQuantities::setAccel(const ID &id,const Vector &accel)
  {
    const int idSize = id.Size();
    for(int i=0;i<idSize;i++)
      {
        const int loc = id(i);
        if(loc >= 0)
          { Rdotdot(loc) = accel(i); }
      }
  }

void XC::ResponseQuantities::Zero(void)
  {  
    R.Zero();
    Rdot.Zero();
    Rdotdot.Zero();
  }

//! @brief Send object members through the communicator argument.
int XC::ResponseQuantities::sendData(Communicator &comm)
  {
    int res= 0;//MovableObject::sendData(comm);
    res+= comm.sendVector(R,getDbTagData(),CommMetaData(1));
    res+= comm.sendVector(Rdot,getDbTagData(),CommMetaData(2));
    res+= comm.sendVector(Rdotdot,getDbTagData(),CommMetaData(3));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::ResponseQuantities::recvData(const Communicator &comm)
  {
    int res= 0;//MovableObject::receiveData(comm);
    res+= comm.receiveVector(R,getDbTagData(),CommMetaData(1));
    res+= comm.receiveVector(Rdot,getDbTagData(),CommMetaData(2));
    res+= comm.receiveVector(Rdotdot,getDbTagData(),CommMetaData(3));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::ResponseQuantities::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(27);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << "ResponseQuantities::sendSelf() - failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::ResponseQuantities::recvSelf(const Communicator &comm)
  {
    inicComm(27);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << "ResponseQuantities::recvSelf - failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << "ResponseQuantities::recvSelf - failed to receive data.\n";
      }
    return res;
  }

void XC::ResponseQuantities::Print(std::ostream &s) const
  {
    s << " R: " << R << " Rdot: " << Rdot << std::endl;
    s << " Rdotdot: " << Rdotdot;	    
  }

