//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis Claudio Pérez Tato
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
// $Date: 2003/02/25 23:32:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/load/groundMotion/GroundMotionRecord.cpp,v $
                                                                        
                                                                        
// File: ~/earthquake/GroundMotionRecord.C
// 
// Written: fmk 
// Created: 05/98
// Revision: A
//
// Description: This file contains the class definition for 
// GroundMotionRecord. The units are (g).
//
// What: "@(#) GroundMotionRecord.C, revA"

#include "GroundMotionRecord.h"
#include <domain/load/pattern/time_series/PathSeries.h>
#include <domain/load/pattern/time_series/PathTimeSeries.h>
#include <cstdlib>
#include <cmath>
#include <classTags.h>
#include <utility/matrix/Vector.h>
#include <utility/matrix/ID.h>
#include <utility/actor/objectBroker/FEM_ObjectBroker.h>

XC::GroundMotionRecord::GroundMotionRecord(void)
  :GroundMotion(GROUND_MOTION_TAG_GroundMotionRecord), history(0.0) {}

XC::GroundMotionRecord::GroundMotionRecord(const std::string &fileNameAccel, double timeStep, double theFactor, double dT)
  :GroundMotion(GROUND_MOTION_TAG_GroundMotionRecord), history(dT)
  { history.loadAccelFile(fileNameAccel, timeStep, theFactor); }

XC::GroundMotionRecord::GroundMotionRecord(const std::string &fileNameAccel,const std::string &fileNameTime, double theFactor, double dT)
  :GroundMotion(GROUND_MOTION_TAG_GroundMotionRecord), history(dT)
  { history.loadAccelTimeFile(fileNameAccel, fileNameTime, theFactor); }

//! @brief Virtual constructor.
XC::GroundMotion *XC::GroundMotionRecord::getCopy(void) const
  { return new GroundMotionRecord(*this); }

//! @brief Return the duration of the motion history.
double XC::GroundMotionRecord::getDuration(void) const
  { return history.getDuration(); }


double XC::GroundMotionRecord::getPeakAccel(void) const
  { return history.getPeakAccel(); }

double XC::GroundMotionRecord::getPeakVel(void) const
  { return history.getPeakVel(); }

double XC::GroundMotionRecord::getPeakDisp(void) const
  { return history.getPeakDisp(); }

double XC::GroundMotionRecord::getAccel(double time) const
  { return history.getAccel(time); }

double XC::GroundMotionRecord::getVel(double time) const
  { return history.getVel(time); }

double XC::GroundMotionRecord::getDisp(double time) const
  { return history.getDisp(time); }

int XC::GroundMotionRecord::sendSelf(Communicator &comm)
  {
    std::cerr << "XC::GroundMotionRecord::sendSelf not implemented." << std::endl;
//   int dbTag = this->getDbTag();

//   static XC::ID idData(6);
  
//   if (theAccelTimeSeries != 0) {
//     idData(0) = theAccelTimeSeries->getClassTag();
//     int seriesDbTag = theAccelTimeSeries->getDbTag();
//     if (seriesDbTag == 0) {
//       seriesDbTag = theCh.getDbTag();
//       theAccelTimeSeries->setDbTag(seriesDbTag);
//     }
//     idData(1) = seriesDbTag;
//   } else
//     idData(0) = -1;

//   if (theVelTimeSeries != 0) {
//     idData(2) = theVelTimeSeries->getClassTag();
//     int seriesDbTag = theVelTimeSeries->getDbTag();
//     if (seriesDbTag == 0) {
//       seriesDbTag = theCh.getDbTag();
//       theVelTimeSeries->setDbTag(seriesDbTag);
//     }
//     idData(3) = seriesDbTag;
//   } else
//     idData(2) = -1;

//   if (theDispTimeSeries != 0) {
//     idData(4) = theDispTimeSeries->getClassTag();
//     int seriesDbTag = theDispTimeSeries->getDbTag();
//     if (seriesDbTag == 0) {
//       seriesDbTag = theCh.getDbTag();
//       theDispTimeSeries->setDbTag(seriesDbTag);
//     }
//     idData(5) = seriesDbTag;
//   } else
//     idData(4) = -1;

//   int res = comm.sendIdData(getDbTagData(),dbTag);
//   if (res < 0) {
//     std::cerr << "XC::GroundMotionRecord::sendSelf() - ch failed to send data\n";
//     return res;
//   }

//   // now send the series
//   if (theAccelTimeSeries != 0) {
//     res = theAccelTimeSeries->sendSelf(commitTag, theCh);
//     if (res < 0) {
//       std::cerr << "XC::GroundMotionRecord::sendSelf - failed to send accel series\n";
//       return res;
//     }
//   }

//   if (theVelTimeSeries != 0) {
//     res = theVelTimeSeries->sendSelf(commitTag, theCh);
//     if (res < 0) {
//       std::cerr << "XC::GroundMotionRecord::sendSelf - failed to send velocity series\n";
//       return res;
//     }
//   }

//   if (theDispTimeSeries != 0) {
//     res = theDispTimeSeries->sendSelf(commitTag, theCh);
//     if (res < 0) {
//       std::cerr << "XC::GroundMotionRecord::sendSelf - failed to send disp series\n";
//       return res;
//     }
//   }

  return 0;
}


int XC::GroundMotionRecord::recvSelf(const Communicator &comm)
  {
    std::cerr << "XC::GroundMotionRecord::recvSelf not implemented." << std::endl;
//   int dbTag = this->getDbTag();
//   static XC::ID idData(6);
//   int res = comm.receiveIdData(dbTag);
//   if (res < 0) {
//     std::cerr << "XC::UniformExcitation::sendSelf() - ch failed to send data\n";
//     return res;
//   }

//   int seriesClassTag = idData(0);
//   if (seriesClassTag != -1) {
//     int seriesDbTag = idData(1);
//     if (theAccelTimeSeries == 0 || theAccelTimeSeries->getClassTag() != seriesClassTag) {
//       if (theAccelTimeSeries != 0)
//         delete theAccelTimeSeries;
//       theAccelTimeSeries = theBroker.getNewTimeSeries(seriesClassTag);
//       if (theAccelTimeSeries == 0) {
//         std::cerr << "XC::GroundMotionRecord::sendSelf - could not create a XC::TimeSeries object\n";
//         return -2;
//       }
//     }
//     theAccelTimeSeries->setDbTag(seriesDbTag);
//     res = theAccelTimeSeries->recvSelf(commitTag, theCh, theBroker);
//     if (res < 0) {
//       std::cerr << "XC::UniformExcitation::sendSelf() - accel series failed to send data\n";
//       return res;
//     }
//   }

//   seriesClassTag = idData(2);
//   if (seriesClassTag != -1) {
//     int seriesDbTag = idData(3);
//     if (theVelTimeSeries == 0 || theVelTimeSeries->getClassTag() != seriesClassTag) {
//       if (theVelTimeSeries != 0)
//         delete theVelTimeSeries;
//       theVelTimeSeries = theBroker.getNewTimeSeries(seriesClassTag);
//       if (theVelTimeSeries == 0) {
//         std::cerr << "XC::GroundMotionRecord::sendSelf - could not create a XC::TimeSeries object\n";
//         return -2;
//       }
//     }
//     theVelTimeSeries->setDbTag(seriesDbTag);
//     res = theVelTimeSeries->recvSelf(commitTag, theCh, theBroker);
//     if (res < 0) {
//       std::cerr << "XC::UniformExcitation::sendSelf() - accel series failed to send data\n";
//       return res;
//     }
//   }

//   seriesClassTag = idData(4);
//   if (seriesClassTag != -1) {
//     int seriesDbTag = idData(5);
//     if (theDispTimeSeries == 0 || theDispTimeSeries->getClassTag() != seriesClassTag) {
//       if (theDispTimeSeries != 0)
//         delete theDispTimeSeries;
//       theDispTimeSeries = theBroker.getNewTimeSeries(seriesClassTag);
//       if (theDispTimeSeries == 0) {
//         std::cerr << "XC::GroundMotionRecord::sendSelf - could not create a XC::TimeSeries object\n";
//         return -2;
//       }
//     }
//     theDispTimeSeries->setDbTag(seriesDbTag);
//     res = theDispTimeSeries->recvSelf(commitTag, theCh, theBroker);
//     if (res < 0) {
//       std::cerr << "XC::UniformExcitation::sendSelf() - accel series failed to send data\n";
//       return res;
//     }
//   }

  return 0;
}

// AddingSensitivity:BEGIN //////////////////////////////////////////
double XC::GroundMotionRecord::getAccelSensitivity(double time)
  { return history.getAccelSensitivity(time); }

int XC::GroundMotionRecord::setParameter(const std::vector<std::string> &argv, Parameter &param)
  { return history.setParameter(argv, param); }

int XC::GroundMotionRecord::updateParameter(int parameterID, Information &info)
  { return history.updateParameter(parameterID,info); }

int XC::GroundMotionRecord::activateParameter(int pparameterID)
  { return history.activateParameter(pparameterID); }
// AddingSensitivity:END ////////////////////////////////////

