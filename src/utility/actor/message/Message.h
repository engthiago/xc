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
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:16 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/message/Message.h,v $
                                                                        
                                                                        
// File: ~/actor/Message.h
//
// Written: fmk 11/96
// Revised:
//
// Purpose: This file contains the class definition for Message.

#ifndef Message_h
#define Message_h

namespace XC {

//! @ingroup IPComm
//
//! @brief Message between processes.
//!
//! Messages are objects that can be sent between Channels. They
//! are provided to allow data of arbitrary length and type, e.g. structs,
//! to be sent between processes running on similar machine
//! architectures. WARNING Sending Messages between machines with different
//! architectures can result in erroneuos data being received. Each
//! Message object keeps a pointer to the data it represents and an integer
//! outlining the data size. There is no copy of the data kept by the
//! Message.
class Message
  {
  private:
    int length; //!< message length.
    char *data; //!< pointer to data.
  public:
    Message(void);
    Message(double *, int);
    Message(int *, int);
    Message(char *, int);

    virtual int putData(char *theData, int startLoc, int endLoc);    
    virtual const char *getData(void);
    virtual int getSize(void);

    friend class TCP_Socket;
    friend class TCP_SocketNoDelay;
    friend class UDP_Socket;
    friend class MPI_Channel;
  };
} // end of XC namespace

#endif
