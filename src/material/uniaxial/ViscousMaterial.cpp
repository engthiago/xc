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
                                                                        
// $Revision: 1.5 $
// $Date: 2003/04/02 22:02:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/uniaxial/ViscousMaterial.cpp,v $
                                                                        
// Written: Mehrdad Sasani 
// Created: June 2000
// Revision: A
//
// Description: This file contains the class implementation for 
// ViscousMaterial. 

#include <cmath>

#include "ViscousMaterial.h"
#include "utility/matrix/Vector.h"

XC::ViscousMaterial::ViscousMaterial(int tag, double c, double a)
:UniaxialMaterial(tag,MAT_TAG_Viscous), trialRate(0.0), C(c), Alpha(a)
  {
    if (Alpha < 0.0)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; Alpha < 0.0, setting to 1.0\n";
        Alpha = 1.0;
      }
  }

//! @brief Set the value of the damping coefficient.
void XC::ViscousMaterial::setDampingCoeff(const double &d)
  { C= d; }

//! @brief Return the value of the damping coefficient.
double XC::ViscousMaterial::getDampingCoeff(void) const
  { return C; }
//! @brief Set the value of the power factor (alpha=1.0 means linear
//! damping).
void XC::ViscousMaterial::setAlpha(const double &d)
  {  Alpha= d; }
//! @brief Return the value of the power factor (alpha=1.0 means linear
//! damping).
double XC::ViscousMaterial::getAlpha(void) const
  { return Alpha; }

int XC::ViscousMaterial::setTrialStrain(double strain, double strainRate)
  {
    trialRate = strainRate;
    return 0;
  }

double XC::ViscousMaterial::getStress(void) const
  {
    double stress = C*pow(fabs(trialRate),Alpha);
    if(trialRate < 0.0)
      return -stress;
    else
      return  stress;
  }

//! @brief Return the material tangent stiffness.
double XC::ViscousMaterial::getTangent(void) const
  { return 0.0; }

//! @brief Return the material initial stiffness.
double XC::ViscousMaterial::getInitialTangent(void) const
  { return 0.0; }

double XC::ViscousMaterial::getDampTangent(void) const
  {
    static const double minvel = 1.e-11;
    double absRate = fabs(trialRate);
    if(absRate < minvel)
      return Alpha*C*pow(minvel,Alpha-1.0);
    else
      return Alpha*C*pow(absRate,Alpha-1.0);        
  }


double XC::ViscousMaterial::getStrain(void) const
  { return 0.0; }

double XC::ViscousMaterial::getStrainRate(void) const
  { return trialRate; }

//! @brief Commit the state of the material.
int XC::ViscousMaterial::commitState(void)
  { return 0; }

int XC::ViscousMaterial::revertToLastCommit(void)
  { return 0; }

int XC::ViscousMaterial::revertToStart(void)
  {
    int retval= UniaxialMaterial::revertToStart();
    trialRate = 0.0;
    return retval;
  }

XC::UniaxialMaterial *XC::ViscousMaterial::getCopy(void) const
  { return new ViscousMaterial(*this); }


//! @brief Send object members through the communicator argument.
int XC::ViscousMaterial::sendData(Communicator &comm)
  {
    int res= UniaxialMaterial::sendData(comm);
    res+= comm.sendDoubles(trialRate,C,Alpha,getDbTagData(),CommMetaData(2));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::ViscousMaterial::recvData(const Communicator &comm)
  {
    int res= UniaxialMaterial::recvData(comm);
    res+= comm.receiveDoubles(trialRate,C,Alpha,getDbTagData(),CommMetaData(2));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::ViscousMaterial::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(3);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "sendSelf() - failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::ViscousMaterial::recvSelf(const Communicator &comm)
  {
    inicComm(3);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << getClassName() << "::recvSelf - failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << getClassName() << "::recvSelf - failed to receive data.\n";
      }
    return res;
  }

void XC::ViscousMaterial::Print(std::ostream &s, int flag) const
  {
    s << "Viscous tag: " << this->getTag() << std::endl;
    s << "  C: " << C << std::endl;
    s << "  Alpha: " << Alpha << std::endl;
  }


