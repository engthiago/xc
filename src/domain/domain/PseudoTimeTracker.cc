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
//PseudoTimeTracker.cc

#include "PseudoTimeTracker.h"
#include <cassert>
#include "utility/actor/actor/MovableVector.h"
#include "utility/matrix/ID.h"

XC::PseudoTimeTracker::PseudoTimeTracker(void)
  : MovableObject(0), currentTime(0.0), committedTime(0.0), dT(0.0), eigenvalueTimeSet(0.0) {}

const double &XC::PseudoTimeTracker::getCurrentTime(void) const
  { return currentTime; }

const double &XC::PseudoTimeTracker::getCommittedTime(void) const
  { return committedTime; }

const double &XC::PseudoTimeTracker::getDt(void) const
  { return dT; }

const double &XC::PseudoTimeTracker::getEigenValueTimeSet(void) const
  { return eigenvalueTimeSet; }

void XC::PseudoTimeTracker::Inic(const double &newTime)
  {
    currentTime= newTime;
    committedTime= newTime;
    dT= 0.0;
  }

void XC::PseudoTimeTracker::setCurrentTime(const double &newTime)
  {
    currentTime= newTime;
    dT= currentTime - committedTime;
  }

void XC::PseudoTimeTracker::setCommittedTime(const double &newTime)
  {
    committedTime= newTime;
    dT= currentTime - committedTime;
    assert(dT>=0.0);
  }

void XC::PseudoTimeTracker::setEigenvalueTime(void)
  { eigenvalueTimeSet= currentTime; }

void XC::PseudoTimeTracker::Zero(void)
  { Inic(0.0); }

//! @brief Returns a vector to store the dbTags
//! of the class members.
XC::DbTagData &XC::PseudoTimeTracker::getDbTagData(void) const
  {
    static DbTagData retval(2);
    return retval;
  }

//! @brief Send data through the communicator argument.
int XC::PseudoTimeTracker::sendData(Communicator &comm)
  {
    int res= comm.sendDoubles(currentTime,committedTime,dT,eigenvalueTimeSet,getDbTagData(),CommMetaData(1));
    return res;
  }

//! @brief Receive data through the communicator argument.
int XC::PseudoTimeTracker::recvData(const Communicator &comm)
  {
    int res= comm.receiveDoubles(currentTime,committedTime,dT,eigenvalueTimeSet,getDbTagData(),CommMetaData(1));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::PseudoTimeTracker::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(2);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << "PseudoTimeTracker::sendSelf() - failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::PseudoTimeTracker::recvSelf(const Communicator &comm)
  {
    inicComm(2);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << "PseudoTimeTracker::recvSelf - failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << "PseudoTimeTracker::recvSelf - failed to receive data.\n";
      }
    return res;
  }

//! @brief Return a Python dictionary with the object members values.
boost::python::dict XC::PseudoTimeTracker::getPyDict(void) const
  {
    boost::python::dict retval;
    retval["currentTime"]= currentTime; // current pseudo time
    retval["committedTime"]= committedTime; // the committed pseudo time
    retval["dT"]= dT; // difference between committed and current time
    retval["eigenvalueTimeSet"]= eigenvalueTimeSet;
    return retval;
  }

//! @brief Set the values of the object members from a Python dictionary.
void XC::PseudoTimeTracker::setPyDict(const boost::python::dict &d)
  {
    currentTime= boost::python::extract<double>(d["currentTime"]);
    committedTime= boost::python::extract<double>(d["committedTime"]);
    dT= boost::python::extract<double>(d["dT"]);
    eigenvalueTimeSet= boost::python::extract<double>(d["eigenvalueTimeSet"]);
  }

//! @brief Print stuff.
void XC::PseudoTimeTracker::Print(std::ostream &s, int flag) const
  {
    s << "Pseudo time Information\n";
    s << "\tCurrent Time: " << currentTime;
    s << "\ntCommitted Time: " << committedTime;
  }

std::ostream &XC::operator<<(std::ostream &s,const PseudoTimeTracker &tt)
  {
    tt.Print(s);
    return s;
  }
