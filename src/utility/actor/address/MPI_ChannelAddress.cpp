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
// $Date: 2000/09/15 08:23:15 $
// $Source: /usr/local/cvs/OpenSees/SRC/actor/address/MPI_ChannelAddress.cpp,v $
                                                                        
                                                                        
// File: ~/actor/MPI_ChannelAddress.C
//
// Written: fmk 11/95
// Revised:
//
// Purpose: This file contains the class definition for MPI_ChannelAddress.
// It is used to encapsulate the addresses used to send/recv messages
// using Berkeley sockets. MPI_ChannelAddress is needed as a friend by 
// UDP_Socket & TCP_Socket.

#include "utility/actor/address/MPI_ChannelAddress.h"

#include <mpi/mpi.h>

XC::MPI_ChannelAddress::MPI_ChannelAddress(int other)
:ChannelAddress(MPI_TYPE), otherTag(other), otherComm(MPI_COMM_WORLD)
{

}

XC::MPI_ChannelAddress::MPI_ChannelAddress(int other, MPI_Comm otherC)
:ChannelAddress(MPI_TYPE), otherTag(other), otherComm(otherC)
{

}
