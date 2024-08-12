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
///////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT (C):     :-))
// PROJECT:           Object Oriented Finite XC::Element Program
// FILE:              BrickSelfWeight
// CLASS:             
// MEMBER FUNCTIONS:
//
// MEMBER VARIABLES
//
// PURPOSE:           Finite XC::Element Class
// RETURN:
// VERSION:
// LANGUAGE:          C++
// TARGET OS:         DOS || UNIX || . . .
// DESIGNER:          Zhaohui Yang
// PROGRAMMER:        Zhaohui Yang 
// DATE:              March 2002
// UPDATE HISTORY:
//
//
//
///////////////////////////////////////////////////////////////////////////////
//

#ifndef BRICKSELFWEIGHT_CPP
#define BRICKSELFWEIGHT_CPP

                                                                        
// Written: ZHYang UC Davis
// Purpose: This file contains the class definition for 8 node brick self weight load.

#include <domain/load/volumetric/BrickSelfWeight.h>
#include <utility/matrix/Vector.h>
#include "utility/matrix/ID.h"

XC::Vector XC::BrickSelfWeight::data(1);

XC::BrickSelfWeight::BrickSelfWeight(int tag, const XC::ID &theElementTags)
  :ThreedimLoad(tag, LOAD_TAG_BrickSelfWeight, theElementTags) {}

XC::BrickSelfWeight::BrickSelfWeight(int tag)
  :ThreedimLoad(tag, LOAD_TAG_BrickSelfWeight) {}

const XC::Vector &XC::BrickSelfWeight::getData(int &type, const double &loadFactor) const
  {
    type= getClassTag();
    //data(0) = P;
    //data(1) = x;
    return data;
  }

//! @brief Send members of the object through the
//! communicator being passed as parameter.
int XC::BrickSelfWeight::sendData(Communicator &comm)
  {
    int res= ThreedimLoad::sendData(comm);
    return res;
  }

//! @brief Receives members of the object through the
//! communicator being passed as parameter.
int XC::BrickSelfWeight::recvData(const Communicator &comm)
  {        
    int res= ThreedimLoad::recvData(comm);
    return res;
  }

int XC::BrickSelfWeight::sendSelf(Communicator &comm)
  {
    static ID data(5);
    int result= sendData(comm);
    
    const int dataTag= getDbTag();
    result+= comm.sendIdData(getDbTagData(),dataTag);
    if(result < 0)
      std::cerr << "BrickSelfWeight::sendSelf - failed to send data\n";
    return result;
  }

int XC::BrickSelfWeight::recvSelf(const Communicator &comm)
  {
    static ID data(5);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << "BrickSelfWeight::recvSelf() - failed to recv data\n";
    else
      res+= recvData(comm);
    return res;
  }

void XC::BrickSelfWeight::Print(std::ostream &s, int flag) const
  {
    s << "BrickSelfWeight...";
    s << "  elements acted on: " << this->getElementTags();
  }

#endif

