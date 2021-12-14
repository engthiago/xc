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
// $Date: 2003/02/14 23:00:41 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/algorithm/eigenAlgo/FrequencyAlgo.cpp,v $
                                                                        
                                                                        
// File: ~/analysis/algorithm/eigenAlgo/FrequencyAlgo.C
//
// Written: Jun Peng
// Created: Mon Feb. 8, 1999
// Revision: A
//
// Description: This file contains the class definition of XC::FrequencyAlgo.
// FrequencyAlgo is a class which performs a eigen solution algorithm
// to solve the Generalized eigen equations. It is not expected that 
// this class will have subclasses.
//
// This class is inheritanted from the base class of XC::SolutionAlgorithm
// which was created by fmk (Frank).


#include <solution/analysis/algorithm/eigenAlgo/FrequencyAlgo.h>
#include <solution/analysis/analysis/EigenAnalysis.h>
#include <solution/analysis/integrator/EigenIntegrator.h>
#include <solution/system_of_eqn/eigenSOE/EigenSOE.h>

//! @brief Constructor.
XC::FrequencyAlgo::FrequencyAlgo(SolutionStrategy *owr)
  :EigenAlgorithm(owr,EigenALGORITHM_TAGS_Frequency) {}

//! @brief Calculate the eigenvalues for the current step.
int XC::FrequencyAlgo::solveCurrentStep(int nModes)
  {
    AnalysisModel *theModel= getAnalysisModelPtr();
    EigenIntegrator *theIntegrator= getEigenIntegratorPtr();
    EigenSOE *theSOE = getEigenSOEPtr();

    if((theModel == 0) || (theIntegrator == 0) || (theSOE == 0))
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "WARNING; domain, model or integrator not assigned.\n";
        return -1;
      }
    if(theIntegrator->formK()<0) //Builds tangent stiffness matrix.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "WARNING ;the Integrator failed in formK().\n";
        return -2;
      }
    if(theIntegrator->formM()<0) //Form the mass matrix.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "WARNING; the Integrator failed in formM().\n";
        return -3;
      }
    if(theSOE->solve(nModes) < 0) //Computes eigenmodes.
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "WARNING; the XC::EigenSOE failed in solve().\n";
        return -4;
      }
    eigen_to_model(nModes); //Sends eigenvectors and eigenvalues to the model.
    return 0;
  }

//! @brief Send.
int XC::FrequencyAlgo::sendSelf(Communicator &comm)
  { return 0; }

//! @brief Receive.
int XC::FrequencyAlgo::recvSelf(const Communicator &comm)
  { return 0; }

//! @brief Print the object.
void XC::FrequencyAlgo::Print(std::ostream &s, int flag) const
  { s << "\t Eigen Algorithm \n"; }


