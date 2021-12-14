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
//Framework2d.cc

#include "Framework2d.h"
#include "PntPtrArray.h"
#include "preprocessor/Preprocessor.h"
#include "domain/mesh/node/Node.h"
#include "domain/mesh/element/Element.h"

#include "preprocessor/multi_block_topology/entities/0d/Pnt.h"
#include "preprocessor/set_mgmt/Set.h"



//! @brief Constructor.
XC::Framework2d::Framework2d(MultiBlockTopology *mbt)
  : ModelComponentContainer<PntPtrArray>(mbt) {}

//! @brief Creates a new point.
XC::PntPtrArray *XC::Framework2d::Crea(void)
  {
    PntPtrArray *retval= new PntPtrArray();
    retval->set_owner(this);
    (*this)[getTag()]= retval;
    //updateSets(retval);
    tag++;
    return retval;
  }

//! @brief Creates a new bi-dimensional framework.
XC::PntPtrArray *XC::Framework2d::makeNew(void)
  {
    PntPtrArray *retval= busca(getTag());
    if(!retval) //The framework is new.
      retval= Crea();
    return retval;
  }
