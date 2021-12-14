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
// $Date: 2005/11/29 22:42:42 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/equiSolnAlgo/PeriodicNewton.cpp,v $

// Written: MHS
// Created: Oct 2002
//
// Description: This file contains the class definition for
// PeriodicNewton. PeriodicNewton is a class which uses the
// Newton-Raphson solution algorithm
// to solve the equations. No member functions are declared as virtual as
// it is not expected that this class will be subclassed.

#include <solution/analysis/algorithm/equiSolnAlgo/PeriodicNewton.h>
#include <solution/analysis/model/AnalysisModel.h>
#include <solution/analysis/analysis/StaticAnalysis.h>
#include <solution/analysis/integrator/IncrementalIntegrator.h>
#include <solution/system_of_eqn/linearSOE/LinearSOE.h>
#include <utility/matrix/ID.h>
#include <solution/analysis/convergenceTest/ConvergenceTest.h>
#include "solution/SolutionStrategy.h"

//! @brief Constructor
XC::PeriodicNewton::PeriodicNewton(SolutionStrategy *owr,int theTangentToUse, int mc)
  :NewtonBased(owr,EquiALGORITHM_TAGS_PeriodicNewton,theTangentToUse), maxCount(mc) {}

//! @brief Virtual constructor.
XC::SolutionAlgorithm *XC::PeriodicNewton::getCopy(void) const
  { return new PeriodicNewton(*this); }

//! @brief resuelve el paso actual.
int XC::PeriodicNewton::solveCurrentStep(void)
  {
    // set up some pointers and check they are valid
    // NOTE this could be taken away if we set Ptrs as protecetd in superclass
    AnalysisModel *theAnalysisModel= getAnalysisModelPtr();
    IncrementalIntegrator *theIncIntegratorr= getIncrementalIntegratorPtr();
    LinearSOE *theSOE = getLinearSOEPtr();
    ConvergenceTest *theTest= getConvergenceTestPtr();

    if(!theAnalysisModel || !theIncIntegratorr || !theSOE || !theTest)
      {
        std::cerr << "WARNING PeriodicNewton::solveCurrentStep() - "
                  << "no se ha asignado modelo, integrator o system of equations.\n";
        return -5;
      }

    // we form the tangent

    if(theIncIntegratorr->formUnbalance() < 0)
      {
        std::cerr << "WARNING XC::PeriodicNewton::solveCurrentStep() -"
                  << "the XC::Integrator failed in formUnbalance()\n";
        return -2;
      }

    if(theIncIntegratorr->formTangent(tangent) < 0)
      {
        std::cerr << "WARNING XC::PeriodicNewton::solveCurrentStep() -"
                  << "the XC::Integrator failed in formTangent()\n";
        return -1;
      }

    // set itself as the XC::ConvergenceTest objects XC::EquiSolnAlgo
    theTest->set_owner(getSolutionStrategy());
    if(theTest->start() < 0)
      {
        std::cerr << "XC::PeriodicNewton::solveCurrentStep() -"
                  << "the XC::ConvergenceTest object failed in start()\n";
        return -3;
      }

    // repeat until convergence is obtained or reach max num iterations
    int result = -1;
    int count = 0;
    int iter = 0;
    do
      {
        if(theSOE->solve() < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the LinearSysOfEqn failed in solve()\n";
            return -3;
          }

        if(theIncIntegratorr->update(theSOE->getX()) < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the Integrator failed in update()\n";
            return -4;
          }

        if(theIncIntegratorr->formUnbalance() < 0)
          {
            std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; WARNING the Integrator failed in formUnbalance()\n";
            return -2;
          }

        this->record(count++); //Call the record(...) method of all the recorders.
        result = theTest->test();

        iter++;
        if(iter > maxCount)
          {
            if(theIncIntegratorr->formTangent(tangent) < 0)
              {
                std::cerr << getClassName() << "::" << __FUNCTION__
			  << "; WARNING the Integrator failed in formTangent()\n";
                return -1;
              }
            iter = 0;
          }
      }
    while (result == -1);

    if(result == -2)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; the ConvergenceTest object failed in test() at iter: "
	          << iter << std::endl
                  << "convergence test message: "
		  << theTest->getStatusMsg(1) << std::endl;
        return -3;
      }
    return result;
  }

//! @brief Send object members through the communicator argument.
int XC::PeriodicNewton::sendData(Communicator &comm)
  {
    int res= NewtonBased::sendData(comm);
    res+= comm.sendDouble(maxCount,getDbTagData(),CommMetaData(3));
    return res;
  }

//! @brief Receives object members through the communicator argument.
int XC::PeriodicNewton::recvData(const Communicator &comm)
  {
    int res= NewtonBased::recvData(comm);
    double tmp= maxCount;
    res+= comm.receiveDouble(tmp,getDbTagData(),CommMetaData(3));
    maxCount= tmp;
    return res;
  }

int XC::PeriodicNewton::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(4);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "sendSelf() - failed to send data\n";
    return res;
  }

int XC::PeriodicNewton::recvSelf(const Communicator &comm)
  {
    inicComm(4);
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

void XC::PeriodicNewton::Print(std::ostream &s, int flag) const
  {
    if(flag == 0)
      {
        s << "PeriodicNewton" << std::endl;
        s << "Max count: " << maxCount << std::endl;
      }
  }
