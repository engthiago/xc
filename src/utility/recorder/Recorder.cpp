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
                                                                        
// $Revision: 1.2 $
// $Date: 2005/01/10 21:58:47 $
// $Source: /usr/local/cvs/OpenSees/SRC/recorder/Recorder.cpp,v $
                                                                        

// Written: fmk 
// Created: 11/98
// Revision: A
//
// Description: This file contains the class implementation for XC::Recorder.
//
// What: "@(#) Recorder.cpp, revA"

#include "utility/recorder/Recorder.h"
#include "utility/utils/misc_utils/colormod.h"

XC::Recorder::Recorder(int classTag)
  :MovableObject(classTag), CommandEntity() {}

//! @brief //! Invoked by the analyst after the analysis has been performed.
//! What the method does depends on the concrete subtype.
int XC::Recorder::playback(int)
  {
    std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
	      << "; not implemented."
	      << Color::def << std::endl;
    return 0;
  }

//! Invoked by the Domain object when revertToStart() is invoked on
//! the Domain object. What the Recorder does depends on the concrete subtype. 
int XC::Recorder::restart(void)
  { return 0; }

int XC::Recorder::setDomain(Domain &theDomain)
  { return 0; }

int XC::Recorder::sendSelf(Communicator &comm)
  {
    std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
              << "; not yet implemented."
	      << Color::def << std::endl;
    return 0;
  }

int XC::Recorder::recvSelf(const Communicator &comm)
  {
    std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
	      << "; not yet implemented."
	      << Color::def << std::endl;
    return 0;
  }

//! @brief Return a Python dictionary with the object members values.
boost::python::dict XC::Recorder::getPyDict(void) const
  {
    boost::python::dict retval;
    std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
	      << " not implemented yet."
	      << Color::def << std::endl;
    return retval;
  }

//! @brief Set the values of the object members from a Python dictionary.
void XC::Recorder::setPyDict(const boost::python::dict &d)
  {
    std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
	      << " not implemented yet."
	      << Color::def << std::endl;    
  }
