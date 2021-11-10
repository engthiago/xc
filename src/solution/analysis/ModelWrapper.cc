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
//ModelWrapper.cc

#include "ModelWrapper.h"
#include "solution/SolutionStrategy.h"
#include <solution/analysis/model/AnalysisModel.h>

//Constraints handler.
#include <solution/analysis/handler/ConstraintHandler.h>
#include <solution/analysis/handler/LagrangeConstraintHandler.h>
#include <solution/analysis/handler/PenaltyConstraintHandler.h>
#include <solution/analysis/handler/PlainHandler.h>
#include <solution/analysis/handler/TransformationConstraintHandler.h>

//Numberer.
#include "solution/analysis/numberer/DOF_Numberer.h"
#include "solution/analysis/numberer/ParallelNumberer.h"
#include "solution/analysis/numberer/PlainNumberer.h"

#include <utility/actor/objectBroker/FEM_ObjectBroker.h>
#include "utility/matrix/ID.h"

void XC::ModelWrapper::free_analysis_model(void)
  {
    if(theModel)
      {
        delete theModel;
        theModel= nullptr;
      }
  }

void XC::ModelWrapper::alloc_analysis_model(void)
  {
    free_analysis_model();
    theModel= new AnalysisModel(this);
  }

void XC::ModelWrapper::copy_analysis_model(AnalysisModel *ptr)
  {
    if(ptr)
      {
        free_analysis_model();
        theModel= ptr->getCopy();
        theModel->set_owner(this);
      }
    else
      std::cerr << "ModelWrapper::copy_analysis_model; null pointer to analysis model." << std::endl;
  }

void XC::ModelWrapper::free_constraint_handler(void)
  {
    if(theHandler)
      {
        delete theHandler;
        theHandler= nullptr;
      }
  }

bool XC::ModelWrapper::alloc_constraint_handler(const std::string &nmb)
  {
    free_constraint_handler();

    if(nmb=="lagrange_constraint_handler")
      theHandler=new LagrangeConstraintHandler(this);
    else if(nmb=="penalty_constraint_handler")
      theHandler=new PenaltyConstraintHandler(this);
    else if(nmb=="plain_handler")
      theHandler=new PlainHandler(this);
    else if(nmb=="transformation_constraint_handler")
      theHandler=new TransformationConstraintHandler(this);
    else
      std::cerr << "Constraint handler: '"
                << nmb << " unknown." << std::endl;

    if(theHandler)
      theHandler->set_owner(this);

    return (theHandler!=nullptr);
  }

void XC::ModelWrapper::copy_constraint_handler(const ConstraintHandler *ptr)
  {
    if(ptr)
      {
        free_constraint_handler();
        theHandler= ptr->getCopy();
        theHandler->set_owner(this);
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; null pointer to constraint handler."
		<< std::endl;
  }

//! @brief Create a constraints handler of the type passed as parameter.
XC::ConstraintHandler &XC::ModelWrapper::newConstraintHandler(const std::string &nmb)
  {
    alloc_constraint_handler(nmb);
    assert(theHandler);
    return *theHandler;
  }

void XC::ModelWrapper::free_numerador(void)
  {
    if(theDOFNumberer)
      {
        delete theDOFNumberer;
        theDOFNumberer= nullptr;
      }
  }

bool XC::ModelWrapper::setup_numerador(void)
  {
    bool retval= false;
    if(theDOFNumberer)
      {
        assert(theModel);
        retval= true;
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; numberer is not defined."
                << std::endl;
    return retval;
  }

bool XC::ModelWrapper::alloc_numerador(const std::string &nmb)
  {
    free_numerador();

    if(nmb=="default_numberer")
      theDOFNumberer= new DOF_Numberer(this);
    else if(nmb=="plain_numberer")
      theDOFNumberer= new PlainNumberer(this);
    else if(nmb=="parallel_numberer")
      theDOFNumberer= new ParallelNumberer(this);
    else
      std::cerr << "ModelWrapper::alloc_numerador. ERROR; can't interpret"
                << " string: '" << nmb << "'\n";
    return setup_numerador();
  }

void XC::ModelWrapper::copy_numerador(const DOF_Numberer *dn)
  {
    if(dn)
      {
        free_numerador();
        theDOFNumberer= dn->getCopy();
        theDOFNumberer->set_owner(this);
        setup_numerador();
      }
    else
     std::cerr << "ModelWrapper::copy_numerador; null pointer to DOF freedom." << std::endl;
  }

//! @brief Creates a numberer of the type passed as parameter.
XC::DOF_Numberer &XC::ModelWrapper::newNumberer(const std::string &nmb)
  {
    alloc_numerador(nmb);
    assert(theDOFNumberer);
    return *theDOFNumberer;
  }

void XC::ModelWrapper::free_mem(void)
  {
    free_analysis_model();
    free_constraint_handler();
    free_numerador();
  }

void XC::ModelWrapper::copy(const ModelWrapper &other)
  {
    if(other.theModel) copy_analysis_model(other.theModel);
    if(other.theHandler) copy_constraint_handler(other.theHandler);
    if(other.theDOFNumberer) copy_numerador(other.theDOFNumberer);
  }

//! @brief Return a pointer to the object owner.
XC::SolutionStrategy *XC::ModelWrapper::getSolutionStrategy(void)
  { return dynamic_cast<SolutionStrategy *>(Owner()); }

//! @brief Return a pointer to the object owner.
const XC::SolutionStrategy *XC::ModelWrapper::getSolutionStrategy(void) const
  { return dynamic_cast<const SolutionStrategy *>(Owner()); }


//! @brief Default constructor.
XC::ModelWrapper::ModelWrapper(SolutionStrategy *owr)
  : CommandEntity(owr), theModel(nullptr), theHandler(nullptr), theDOFNumberer(nullptr)
  { alloc_analysis_model(); }

//! @brief Copy constructor.
XC::ModelWrapper::ModelWrapper(const ModelWrapper &other)
  : CommandEntity(other), theModel(nullptr), theHandler(nullptr),theDOFNumberer(nullptr)
  { copy(other); }

//! @brief Assignment operator.
XC::ModelWrapper &XC::ModelWrapper::operator=(const ModelWrapper &other)
  {
    CommandEntity::operator=(other);
    copy(other);
    return *this;
  }

//! @brief Destructor.
XC::ModelWrapper::~ModelWrapper(void)
  { free_mem(); }

void XC::ModelWrapper::clearAll(void)
  { free_mem(); }

//! @brief Returns the name of the material.
std::string XC::ModelWrapper::getName(void) const
  {
    const SolutionStrategy *se= getSolutionStrategy();
    return se->getModelWrapperName(this);
  }


//! @brief Return a pointer to the domain.
XC::Domain *XC::ModelWrapper::getDomainPtr(void)
  {
    SolutionStrategy *sm= getSolutionStrategy();
    assert(sm);
    return sm->getDomainPtr();
  }

//! @brief Return a pointer to the domain.
const XC::Domain *XC::ModelWrapper::getDomainPtr(void) const
  {
    const SolutionStrategy *sm= getSolutionStrategy();
    assert(sm);
    return sm->getDomainPtr();
  }

//! @brief Return a pointer to the integrator.
XC::Integrator *XC::ModelWrapper::getIntegratorPtr(void)
  {
    SolutionStrategy *sm= getSolutionStrategy();
    assert(sm);
    return sm->getIntegratorPtr();
  }

//! @brief Return a pointer to the integrator.
const XC::Integrator *XC::ModelWrapper::getIntegratorPtr(void) const
  {
    const SolutionStrategy *sm= getSolutionStrategy();
    assert(sm);
    return sm->getIntegratorPtr();
  }

void XC::ModelWrapper::brokeConstraintHandler(const Communicator &comm,const ID &data)
  {
    theHandler= comm.brokeConstraintHandler(data(0));
    theHandler->set_owner(this);
  }

void XC::ModelWrapper::brokeNumberer(const Communicator &comm,const ID &data)
  { 
    theDOFNumberer= comm.brokeNumberer(data(1));
    theDOFNumberer->set_owner(this);
  }

void XC::ModelWrapper::brokeAnalysisModel(const Communicator &comm,const ID &data)
  {
    theModel= comm.brokeAnalysisModel(data(2));
    theModel->set_owner(this);
  }

//! @brief Set the numberer to be used in the analysis.
int XC::ModelWrapper::setNumberer(DOF_Numberer &theNewNumberer) 
  {
    copy_numerador(&theNewNumberer);
    return 0;
  }

//! @brief Verify that the point are not null.
bool XC::ModelWrapper::CheckPointers(void)
  {
    if(!theModel)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; error, you haven't set the analysis model."
		  << std::endl;
        return false;
      }
    if(!theHandler)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; error, you haven't set the constraints handler."
		  << std::endl;
        return false;
      }
    if(!theDOFNumberer)
      {
        std::cerr << "ModelWrapper::check_pointers; error, you haven't set the numberer." << std::endl;
        return false;
      }
    return true;
  }
