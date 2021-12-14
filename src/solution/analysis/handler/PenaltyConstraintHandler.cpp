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
//  MERCANTIL o de APTITUD PARA UN PROPÓSITO DETERMINADO.
//  GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// junto a este programa.
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

// $Revision: 1.6 $
// $Date: 2005/11/29 22:04:40 $
// $Source: /usr/local/cvs/OpenSees/SRC/analysis/handler/PenaltyConstraintHandler.cpp,v $


// Written: fmk
// Created: May 1998
// Revision: A
//
// What: "@(#) PenaltyConstraintHandler.C, revA"

#include <solution/analysis/handler/PenaltyConstraintHandler.h>
#include <cstdlib>
#include <solution/analysis/model/AnalysisModel.h>
#include <domain/domain/Domain.h>
#include <solution/analysis/model/fe_ele/FE_Element.h>
#include <solution/analysis/model/dof_grp/DOF_Group.h>
#include <domain/mesh/node/Node.h>
#include <domain/mesh/element/Element.h>
#include "domain/mesh/node/NodeIter.h"
#include "domain/mesh/element/ElementIter.h"
#include <domain/constraints/SFreedom_ConstraintIter.h>
#include <domain/constraints/SFreedom_Constraint.h>
#include <domain/constraints/MFreedom_ConstraintIter.h>
#include <domain/constraints/MFreedom_Constraint.h>
#include <domain/constraints/MRMFreedom_ConstraintIter.h>
#include <domain/constraints/MRMFreedom_Constraint.h>
#include <solution/analysis/integrator/Integrator.h>
#include <utility/matrix/ID.h>
#include "domain/domain/subdomain/Subdomain.h"
#include <solution/analysis/model/fe_ele/penalty/PenaltySFreedom_FE.h>
#include <solution/analysis/model/fe_ele/penalty/PenaltyMFreedom_FE.h>
#include <solution/analysis/model/fe_ele/penalty/PenaltyMRMFreedom_FE.h>


//! @brief Constructor.
//! @param owr: pointer to the model wrapper that owns the handler.
//! @param sp: factor to be used with the single freedom constraints.
//! @param mp: factor to be used with the multi-freedom constraints.
XC::PenaltyConstraintHandler::PenaltyConstraintHandler(ModelWrapper *owr,const double &sp, const double &mp)
  :FactorsConstraintHandler(owr,HANDLER_TAG_PenaltyConstraintHandler,sp,mp) {}

//! @brief Virtual constructor.
XC::ConstraintHandler *XC::PenaltyConstraintHandler::getCopy(void) const
  { return new PenaltyConstraintHandler(*this); }

//! @brief Handle the constraints.
//!
//! Determines the number of FE\_Elements and DOF\_Groups needed from the
//! Domain (a one to one mapping between Elements and FE\_Elements,
//! SFreedom\_Constraints and PenaltySFreedom\_FEs, MFreedom\_Constraints and
//! PenaltyMFreedom\_FEs and Nodes and DOF\_Groups). Creates two arrays of
//! pointers to store the FE\_Elements and DOF\_Groups, returning a warning
//! message and a \f$-2\f$ or \f$-3\f$ if not enough memory is available for
//! these arrays. Then the object will iterate through the Nodes of the Domain,
//! creating a DOF\_Group for each node and setting the initial id for each dof
//! to \f$-2\f$ or \f$-3\f$ if the node identifier is in {\em
//! nodesToBeNumberedLast}. The object then iterates through the Elements
//! of the Domain creating a FE\_Element for each Element, if the Element
//! is a Subdomain setFE\_ElementPtr() is invoked on the Subdomain
//! with the new FE\_Element as the argument. If not enough memory is
//! available for any DOF\_Group or FE\_element a warning message is
//! printed and a \f$-4\f$ or \f$-5\f$ is returned. 
//! The object then iterates through the SFreedom\_Constraints
//! of the Domain creating a PenaltySFreedom\_FE for each constraint, using the
//! Domain, the constraint and \p alphaSP as the arguments in the
//! constructor.
//! The object then iterates through the MP\_Constraints
//! of the Domain creating a PenaltyMP\_FE for each constraint, using the
//! Domain, the constraint and \p alphaMP as the arguments in the constructor.
//! Finally the method returns the
//! number of degrees-of-freedom associated with the DOF\_Groups in {\em
//! nodesToBeNumberedLast}.
int XC::PenaltyConstraintHandler::handle(const ID *nodesLast)
  {
    // first check links exist to a XC::Domain and an AnalysisModel object
    Domain *theDomain = this->getDomainPtr();
    AnalysisModel *theModel = this->getAnalysisModelPtr();
    Integrator *theIntegrator = this->getIntegratorPtr();

    if((!theDomain) || (!theModel) || (!theIntegrator))
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; domain, model or integrator was not set.\n";
        return -1;
      }

    // get number ofelements and nodes in the domain
    // and init the theFEs and theDOFs arrays
    int numSPs = 0;
    SFreedom_ConstraintIter &theSPs = theDomain->getConstraints().getDomainAndLoadPatternSPs();
    SFreedom_Constraint *spPtr= nullptr;
    while((spPtr = theSPs()) != 0)
      numSPs++;

    // initialize the DOF_Groups and add them to the XC::AnalysisModel.
    //    : must of course set the initial IDs
    NodeIter &theNod= theDomain->getNodes();
    Node *nodPtr= nullptr;
    DOF_Group *dofPtr= nullptr;

    int numDofGrp = 0;
    int count3 = 0;
    int countDOF =0;
    while((nodPtr = theNod()) != 0)
      {
        dofPtr= theModel->createDOF_Group(numDofGrp++, nodPtr);
        // initially set all the ID value to -2
        countDOF+= dofPtr->inicID(-2);
      }

    theModel->setNumEqn(countDOF);

    // set the number of eqn in the model
    // now see if we have to set any of the dof's to -3
    //    int numLast = 0;
    if(nodesLast != nullptr)
      for(int i=0; i<nodesLast->Size(); i++)
        {
          int nodeID = (*nodesLast)(i);
          Node *nodPtr = theDomain->getNode(nodeID);
          if(nodPtr != 0)
            {
              dofPtr = nodPtr->getDOF_GroupPtr();

              const ID &id = dofPtr->getID();
              // set all the dof values to -3
              for(int j=0; j < id.Size(); j++)
                if(id(j) == -2)
                  {
                    dofPtr->setID(j,-3);
                    count3++;
                  }
                else
                  {
                    std::cerr << getClassName() << "::" << __FUNCTION__
		              << "; boundary sp constraint in subdomain"
                              << " this should not be - results suspect \n";
                  }
            }
        }

    // create the FE_Elements for the Elements and add to the XC::AnalysisModel
    ElementIter &theEle = theDomain->getElements();
    Element *elePtr= nullptr;

    int numFeEle = 0;
    FE_Element *fePtr= nullptr;
    while((elePtr = theEle()) != 0)
      { fePtr= theModel->createFE_Element(numFeEle++, elePtr); }

    // create the PenaltySFreedom_FE for the SFreedom_Constraints and
    // add to the AnalysisModel
    SFreedom_ConstraintIter &theSPss = theDomain->getConstraints().getDomainAndLoadPatternSPs();
    while((spPtr = theSPss()) != 0)
      {
        fePtr= theModel->createPenaltySFreedom_FE(numFeEle, *spPtr, alphaSP);
        numFeEle++;
      }

    // create the PenaltyMFreedom_FE for the MFreedom_Constraints and
    // add to the AnalysisModel
    MFreedom_ConstraintIter &theMPs = theDomain->getConstraints().getMPs();
    MFreedom_Constraint *mpPtr= nullptr;
    while((mpPtr = theMPs()) != 0)
      {
	fePtr= theModel->createPenaltyMFreedom_FE(numFeEle, *mpPtr, alphaMP);
        numFeEle++;
      }

    // create the PenaltyMRMFreedom_FE for the MRMFreedom_Constraints and
    // add to the AnalysisModel
    MRMFreedom_ConstraintIter &theMRMPs = theDomain->getConstraints().getMRMPs();
    MRMFreedom_Constraint *mrmpPtr= nullptr;
    while((mrmpPtr = theMRMPs()) != 0)
      {
	fePtr= theModel->createPenaltyMRMFreedom_FE(numFeEle, *mrmpPtr, alphaMP);
        numFeEle++;
      }
    return count3;
  }


