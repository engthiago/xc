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

// $Revision: 1.1 $
// $Date: 2009/04/17 23:02:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/special/frictionBearing/frictionModel/FrictionModel.h,v $

#ifndef FrictionModel_h
#define FrictionModel_h

// Written: Andreas Schellenberg (andreas.schellenberg@gmx.net)
// Created: 02/06
// Revision: A
//
// Description: This file contains the class definition for FrictionModel.
// FrictionModel is an abstract base class and thus no objects of it's type
// can be instantiated. It has pure virtual functions which must be
// implemented in it's derived classes. 
//
// What: "@(#) FrictionModel.h, revA"

#include "utility/actor/actor/MovableObject.h"
#include "utility/tagged/TaggedObject.h"

namespace XC {
class Response;
  
//! @ingroup FrictionElementGrp
//!
//! @defgroup FrictionModelGrp Friction models.
//
//! @ingroup FrictionModelGrp
//
//! @brief Base class for friction models.
//!
//! A friction model object specifies the behavior of the
//! coefficient of friction in terms of the absolute sliding
//! velocity and the pressure on the contact area
class FrictionModel: public TaggedObject, public MovableObject
  {
  protected:
    double trialN;      // trial normal contact force
    double trialVel;    // trial velocity

    int sendData(Communicator &);
    int recvData(const Communicator &);
  public:
    // constructor
    FrictionModel(int tag, int classTag);
    
    // public methods to set and obtain response
    virtual int setTrial(double normalForce, double velocity = 0.0) = 0;
    virtual double getNormalForce(void);
    virtual double getVelocity(void);
    virtual double getFrictionForce(void) = 0;
    virtual double getFrictionCoeff(void) = 0;
    virtual double getDFFrcDNFrc(void) = 0;
    
    virtual int commitState(void) = 0;
    virtual int revertToLastCommit(void) = 0;
    virtual int revertToStart(void);
    
    virtual FrictionModel *getCopy(void) const=0;
    
    virtual Response *setResponse(const std::vector<std::string> &, Information &);
    virtual int getResponse(int responseID, Information &info);
  };
} // end of XC namespace

#endif
