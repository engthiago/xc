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
// $Date: 2005/11/07 23:52:13 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/channel/Channel.cpp,v $
                                                                        
                                                                        
// File: ~/actor/channel/Channel.C
//
// Written: fmk
// Created: 11/96
// Revision: A
//
// Purpose: This file contains the implementation of Channel.
//
// What: "@(#) Channel.C, revA"

#include "utility/actor/channel/Channel.h"
#include "../message/Message.h"
#include "../actor/MovableObject.h"
#include "../objectBroker/FEM_ObjectBroker.h"
#include "boost/lexical_cast.hpp"
#include "utility/matrix/ID.h"
#include "utility/actor/address/ChannelAddress.h"

int XC::Channel::numChannel = 0;

//! @brief Constructor.
XC::Channel::Channel(CommandEntity *owr)
  : CommandEntity(owr)
  {
    numChannel++;
    tag = numChannel;
  }

//! @brief Return true if channel is a data store.
bool XC::Channel::isDatastore(void) const
  { return false; }

//! @brief Return next available database tag.
//!
//! Return the next available database tag, must be an integer value
//! greater than 0, 0 is used by the objects to check if they have yet
//! been assigned a database tag. The method defined for the Channel base
//! class always returns 0, only database channel objects need worry
//! about assigning unique integer values.
int XC::Channel::getDbTag(void) const
  { return 0; }

//! @brief Return the list of dbTags already used.
const XC::ID &XC::Channel::getUsedTags(void) const
  {
    static ID retval;
    const size_t sz= usedDbTags.size();
    retval.resize(sz);
    int cont= 0;
    for(std::set<int>::const_iterator i= usedDbTags.begin();i!=usedDbTags.end();i++,cont++)
      retval[cont]= *i;
    return retval;
  }

//! @brief Check if a \p dbTag is already used.
bool XC::Channel::checkDbTag(const int &dbTag)
  {
    bool retval= true;
    if(usedDbTags.find(dbTag)!=usedDbTags.end())
      {
        std::cerr << "Channel::checkDbTag; dbTag " << dbTag
                  << " is already used." << std::endl;
        retval= false;
      }
    else
      usedDbTags.insert(dbTag);
    return retval;
  }

//! @brief Reset used database tags set.
void XC::Channel::clearDbTags(void)
  { usedDbTags.clear(); }

//! @brief Return the object tag.
int XC::Channel::getTag(void) const
  { return tag; }

//! @brief Send \p theObject.
int XC::Channel::sendMovable(int commitTag, MovableObject &theObject)
  {
    Communicator comm(commitTag,*this);
    return theObject.sendSelf(comm);
  }

//! @brief Receive \p theObject.
int XC::Channel::receiveMovable(int commitTag, MovableObject &theObject, FEM_ObjectBroker &theBroker)
  {
    Communicator comm(commitTag,*this, theBroker);
    return theObject.recvSelf(comm);
  }


