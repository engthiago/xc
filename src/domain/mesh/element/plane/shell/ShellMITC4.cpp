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

// $Revision: 1.13 $
// $Date: 2003/02/25 23:33:01 $
// $Source: /usr/local/cvs/OpenSees/SRC/element/plane/shell/ShellMITC4.cpp,v $

// Original implementation: Ed "C++" Love
// Written: Leopoldo Tesser, Diego A. Talledo, Véronique Le Corvec
//
// Bathe MITC 4 four node shell element with membrane and drill
// Ref: Dvorkin,Bathe, A continuum mechanics based four node shell
//      element for general nonlinear analysis,
//      Eng.Comput.,1,77-88,1984

#include "ShellMITC4.h"
#include "utility/matrix/ID.h"
#include "utility/actor/actor/MovableVector.h"
#include "preprocessor/multi_block_topology/matrices/ElemPtrArray3d.h"
#include "preprocessor/multi_block_topology/aux_meshing.h"
#include "domain/mesh/node/Node.h"
#include "material/section/SectionForceDeformation.h"
#include "domain/domain/Domain.h"
#include "domain/mesh/element/utils/coordTransformation/R3vectors.h"
#include "domain/mesh/element/utils/coordTransformation/ShellLinearCrdTransf3d.h"
#include "utility/actor/actor/MovableVector.h"
#include "utility/actor/actor/MovableMatrix.h"
#include "utility/actor/actor/MovableVectors.h"

#include "preprocessor/Preprocessor.h"
#include "preprocessor/prep_handlers/LoadHandler.h"
#include "utility/actor/actor/MatrixCommMetaData.h"
#include "domain/mesh/element/utils/gauss_models/GaussModel.h"

XC::ShellLinearCrdTransf3d XC::ShellMITC4::linear_trf;

//! @brief Default constructor
XC::ShellMITC4::ShellMITC4(void)
  : ShellMITC4Base(ELE_TAG_ShellMITC4,&linear_trf) {}

//! @brief Constructor
//!
//! @param tag: element identifier.
//! @param ptr_mat: pointer to the material object.
XC::ShellMITC4::ShellMITC4(int tag, const SectionForceDeformation *ptr_mat)
  : ShellMITC4Base(tag, ELE_TAG_ShellMITC4, ptr_mat,&linear_trf) {}

//! @brief Constructor
XC::ShellMITC4::ShellMITC4(int tag,int node1,int node2,int node3,int node4,const SectionForceDeformation &theMaterial )
  : ShellMITC4Base(tag, ELE_TAG_ShellMITC4, &theMaterial,&linear_trf) {}

//! @brief Virtual constructor.
XC::Element* XC::ShellMITC4::getCopy(void) const
  { return new ShellMITC4(*this); }


//! @brief Returns a vector to store the dbTags
//! of the class members.
XC::DbTagData &XC::ShellMITC4::getDbTagData(void) const
  {
    static DbTagData retval(18);
    return retval;
  }

//! @brief Sends object through the communicator argument.
int XC::ShellMITC4::sendSelf(Communicator &comm)
  {
    inicComm(getDbTagData().Size());
    int res= sendData(comm);

    const int dataTag= getDbTag();
    res= comm.sendIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; failed to send ID data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::ShellMITC4::recvSelf(const Communicator &comm)
  {
    inicComm(getDbTagData().Size());

    const int dataTag= getDbTag();
    int res = comm.receiveIdData(getDbTagData(),dataTag);
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; failed to receive ID data\n";
    else
      res+= recvData(comm);
    return res;
  }

