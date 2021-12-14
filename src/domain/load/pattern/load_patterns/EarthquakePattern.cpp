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
                                                                        
// $Revision: 1.8 $
// $Date: 2005/02/17 01:15:27 $
// $Source: /usr/local/cvs/OpenSees/SRC/domain/load/pattern/EarthquakePattern.cpp,v $
                                                                        
// Written: fmk 11/98
// Revised:
//
// Purpose: This file contains the class definition for XC::EarthquakePattern.
// EarthquakePattern is an abstract class.

#include <domain/load/pattern/load_patterns/EarthquakePattern.h>
#include <domain/load/groundMotion/GroundMotion.h>

#include <domain/domain/Domain.h>
#include <domain/mesh/node/Node.h>
#include <domain/mesh/element/Element.h>
#include "domain/mesh/node/NodeIter.h"
#include "domain/mesh/element/ElementIter.h"
#include <cstdlib>

#include "utility/actor/actor/CommMetaData.h"

void XC::EarthquakePattern::copy(const Vector *a,const Vector *b)
  {
    if(a)
      uDotG= Vector(*a);
    if(b)
      uDotDotG= Vector(*b);
  }

XC::EarthquakePattern::EarthquakePattern(int tag, int _classTag)
  :EQBasePattern(tag, _classTag), uDotG(), uDotDotG(), currentTime(0.0), parameterID(0) {}

//! @brief Applies the earthquake load.
//!
//! Obtains from each GroundMotion, the velocity and acceleration for the
//! time specified. These values are placed in two Vectors of size equal
//! to the number of GroundMotion objects. For each node in the Domain
//! addInertiaLoadToUnbalance() is invoked with the acceleration Vector
//! objects. SIMILAR OPERATION WITH VEL and ACCEL NEES TO BE INVOKED ON
//! ELEMENTS .. NEED TO MODIFY ELEMENT INTERFACE
void XC::EarthquakePattern::applyLoad(double time)
  {
    // see if quick return, i.e. no Ground Motions or domain set
    const size_t nMotions= getNumMotions();
    if(nMotions>0)
      {
        // check if setLoadConstant() has been called
        if(!isConstant)
          currentTime = time;

	Domain *theDomain= getDomain();
	if(theDomain)
	  {
	    // set the vel and accel vector
	    for(size_t i=0;i<nMotions;i++)
	      {
		//    (uDotG)(i) = theMotions[i]->getVel(currentTime);
		(uDotDotG)(i) = theMotions[i]->getAccel(currentTime);
	      }

	    NodeIter &theNodes= theDomain->getNodes();
	    Node *theNode= nullptr;
	    while((theNode = theNodes()) != 0) 
	      theNode->addInertiaLoadToUnbalance(uDotDotG, 1.0);


	    ElementIter &theElements = theDomain->getElements();
	    Element *theElement= nullptr;
	    while ((theElement = theElements()) != 0) 
	      theElement->addInertiaLoadToUnbalance(uDotDotG);
	  }
      }
  }
    
void XC::EarthquakePattern::applyLoadSensitivity(double time)
  {
    // see if quick return, i.e. no Ground Motions or domain set
    const size_t nMotions= getNumMotions();
    if(nMotions == 0)
      return;

    Domain *theDomain= getDomain();
    if(!theDomain)
      return;

    // set the vel and accel vector
    for(size_t i=0; i<nMotions; i++)
      {
        uDotG(i) = theMotions[i]->getVel(time);
        if(parameterID != 0)
          { // Something is random in the motions
            uDotDotG(i) = theMotions[i]->getAccelSensitivity(time);
          }
        else
          { uDotDotG(i) = theMotions[i]->getAccel(time); }
      }
    bool somethingRandomInMotions = false;
    if(parameterID != 0)
      { somethingRandomInMotions = true; }


    NodeIter &theNodes = theDomain->getNodes();
    Node *theNode;
    while((theNode = theNodes()) != 0) 
      theNode->addInertiaLoadSensitivityToUnbalance(uDotDotG, 1.0,  somethingRandomInMotions);

    ElementIter &theElements = theDomain->getElements();
    Element *theElement;
    while((theElement = theElements()) != 0) 
      theElement->addInertiaLoadSensitivityToUnbalance(uDotDotG,  somethingRandomInMotions);
  }
    
//! @brief Adds the GroundMotion, \p theMotion to the list of GroundMotion
//! objects.
int XC::EarthquakePattern::addMotion(GroundMotion &theMotion)
  {
    theMotions.addMotion(theMotion);
    const size_t nMotions= getNumMotions();
    uDotG.resize(nMotions);
    uDotDotG.resize(nMotions);
    return 0;
  }


bool XC::EarthquakePattern::addSFreedom_Constraint(SFreedom_Constraint *)
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; cannot add SFreedom_Constraint to EQ pattern.\n";
    return false;
  }

bool XC::EarthquakePattern::addNodalLoad(NodalLoad *)
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; cannot add NodalLoad to EQ pattern.\n";  
    return false;
  }

bool XC::EarthquakePattern::addElementalLoad(ElementalLoad *)
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; cannot add ElementalLoad to EQ pattern.\n";    
    return false;
  }

//! @brief Send object members through the communicator argument.
int XC::EarthquakePattern::sendData(Communicator &comm)
  {
    int res= EQBasePattern::sendData(comm);
    res+= comm.sendVector(uDotG,getDbTagData(),CommMetaData(17));
    res+= comm.sendVector(uDotDotG,getDbTagData(),CommMetaData(18));
    res+= comm.sendDouble(currentTime,getDbTagData(),CommMetaData(19));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::EarthquakePattern::recvData(const Communicator &comm)
  {
    int res= EQBasePattern::recvData(comm);
    res+= comm.receiveVector(uDotG,getDbTagData(),CommMetaData(17));
    res+= comm.receiveVector(uDotDotG,getDbTagData(),CommMetaData(18));
    res+= comm.receiveDouble(currentTime,getDbTagData(),CommMetaData(19));
    return res;
  }


void XC::EarthquakePattern::Print(std::ostream &s, int flag) const
  { std::cerr << "XC::EarthquakePattern::Print() - not yet implemented\n"; }

// AddingSensitivity:BEGIN ////////////////////////////////////
int XC::EarthquakePattern::setParameter(const std::vector<std::string> &argv, Parameter &param)
  {
    // This is not general, yet
    if(argv[0] == "-randomProcessDiscretizer")
      {
        std::vector<std::string> argv2(argv);
        argv2.erase(argv2.begin(),argv2.begin()+1);
        return theMotions[0]->setParameter(argv2, param);
      }
    else
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
                  << "; unknown parameter in EarthquakePattern." << std::endl;
        return -1;
      }
  }

int XC::EarthquakePattern::updateParameter(int parameterID, Information &info)
  { return theMotions[0]->updateParameter(parameterID,info); }

int XC::EarthquakePattern::activateParameter(int pparameterID)
  {
    parameterID = pparameterID;
    return theMotions[0]->activateParameter(pparameterID);
  }
// AddingSensitivity:END ////////////////////////////////////
