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
//ImposedMotionBase.cc

#include <domain/constraints/single_freedom_constraints/ImposedMotionBase.h>
#include <utility/matrix/ID.h>
#include <domain/domain/Domain.h>
#include <domain/load/pattern/LoadPattern.h>
#include "domain/mesh/node/Node.h"

//! @brief Constructor.
//! @param classTag: tag of the object class.
XC::ImposedMotionBase::ImposedMotionBase(int classTag)
:SFreedom_Constraint(classTag), groundMotionTag(0), patternTag(0),
 theGroundMotion(nullptr), theNode(nullptr), theGroundMotionResponse(3) {}

//! @brief Constructor.
//! @param classTag: tag of the object class.
//! @param tag: tag for the multi-freedom constraint.
XC::ImposedMotionBase::ImposedMotionBase(int classTag,int tag, int node, int ndof, int pattern, int motion)
:SFreedom_Constraint(tag, node, ndof, classTag),
 groundMotionTag(motion), patternTag(pattern), theGroundMotion(nullptr), theNode(nullptr), theGroundMotionResponse(3) {}

//! @brief Destructor.
XC::ImposedMotionBase::~ImposedMotionBase(void)
  {
    if(theNode)
      theNode->disconnect(this);
  }

//! @brief Sets the domain of the constraint.
void XC::ImposedMotionBase::setDomain(Domain *theDomain)
  {
    if(theDomain == nullptr)
      {
        std::cerr << "WARNING ImposedMotionBase::SetDomain: Specified domain does not exist";
        std::cerr << "Domain = 0\n";
        return;
      }
    SFreedom_Constraint::setDomain(theDomain);

    if(theDomain)
      {
        // get a pointer to the retained and constrained nodes - make sure they exist
        theNode = theDomain->getNode(getNodeTag());
        if(!theNode)
          {
            std::cerr << "ImposedMotionBase::setup - retained node " <<  getNodeTag() <<  " not in domain\n";
          }
        else
          theNode->connect(this);
      }
  }

//! @brief Returns true if the prescribed value is zero.
bool XC::ImposedMotionBase::isHomogeneous(void) const
  { return true; }

//! @brief Returns the prescribed value.
double XC::ImposedMotionBase::getValue(void) const
  {
    // always return 0.0 - applyConstraint() sets the values at XC::Node 
    return theGroundMotionResponse(0);
  }

//! @brief Gets the constraint motion.
int XC::ImposedMotionBase::getMotion(void)
  {
    if(!theNode)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; null pointer to node: "
                  <<  getNodeTag() <<  "\n";
      }
    if(!theGroundMotion)
      {

        LoadPattern *theLoadPattern = getDomain()->getConstraints().getLoadPattern(patternTag);
        if(theLoadPattern == 0)
          return -3;

        theGroundMotion = theLoadPattern->getMotion(groundMotionTag);
        if (theGroundMotion == 0)
          return -4;
      }
    return 0;
  }

//! @brief Sends object members through the communicator argument.
int XC::ImposedMotionBase::sendData(Communicator &comm)
  {
    int result= SFreedom_Constraint::sendData(comm);
    result+= comm.sendInts(groundMotionTag,patternTag,getDbTagData(),CommMetaData(7));
    return result;
  }

//! @brief Receives object through the communicator argument.
int XC::ImposedMotionBase::recvData(const Communicator &comm)
  {
    int res= SFreedom_Constraint::recvData(comm);
    res+= comm.receiveInts(groundMotionTag,patternTag,getDbTagData(),CommMetaData(7));
    return res;
  }

//! @brief Return a Python dictionary with the object members values.
boost::python::dict XC::ImposedMotionBase::getPyDict(void) const
  {
    boost::python::dict retval= SFreedom_Constraint::getPyDict();
    retval["groundMotionTag"]= this->groundMotionTag;
    retval["patternTag"]= this->patternTag;
    return retval;
  }
//! @brief Set the values of the object members from a Python dictionary.
void XC::ImposedMotionBase::setPyDict(const boost::python::dict &d)
  {
    SFreedom_Constraint::setPyDict(d);
    this->groundMotionTag= boost::python::extract<int>(d["groundMotionTag"]);
    this->patternTag= boost::python::extract<int>(d["patternTag"]);
  }


//! @brief Sends object through the communicator argument.
int XC::ImposedMotionBase::sendSelf(Communicator &comm)
  {
    static ID data(8);
    int result= sendData(comm);
    const int dbTag= getDbTag();
    result+= comm.sendIdData(getDbTagData(),dbTag);
    if(result < 0)
      std::cerr << "ImposedMotionBase::sendSelf() - failed to send extra data\n";
    return result;
  }

//! @brief Receives object through the communicator argument.
int XC::ImposedMotionBase::recvSelf(const Communicator &comm)
  {
    static ID data(8);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << "ImposedMotionBase::recvSelf() - data could not be received\n" ;
    else
      res+= recvData(comm);
    return res;
  }
