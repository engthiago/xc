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
//BeamColumnWithSectionFDTrf2d.cpp

#include <domain/mesh/element/truss_beam_column/BeamColumnWithSectionFDTrf2d.h>
#include <domain/mesh/element/utils/coordTransformation/CrdTransf2d.h>


//! @brief Asigna la coordinate transformation.
void XC::BeamColumnWithSectionFDTrf2d::set_transf(const CrdTransf *trf)
  {
    if(theCoordTransf)
      {
        delete theCoordTransf;
        theCoordTransf= nullptr;
      }
    if(trf)
      {
        const CrdTransf2d *tmp= dynamic_cast<const CrdTransf2d *>(trf);
        if(tmp)
          theCoordTransf = tmp->getCopy();
        else
          {
            std::cerr << "XC::BeamColumnWithSectionFDTrf2d::set_transf -- failed to get copy of coordinate transformation\n";
            exit(1);
          }
      }
    else
      std::cerr << "BeamColumnWithSectionFDTrf3d::set_transf; pointer to coordinate transformation is null." << std::endl;
  }

XC::BeamColumnWithSectionFDTrf2d::BeamColumnWithSectionFDTrf2d(int tag, int classTag,int numSec)
  :BeamColumnWithSectionFD(tag, classTag,numSec), theCoordTransf(nullptr) {}

XC::BeamColumnWithSectionFDTrf2d::BeamColumnWithSectionFDTrf2d(int tag, int classTag,int numSec,const Material *mat,const CrdTransf *coordTransf)
  :BeamColumnWithSectionFD(tag, classTag,numSec,mat), theCoordTransf(nullptr)
  { set_transf(coordTransf); }

XC::BeamColumnWithSectionFDTrf2d::BeamColumnWithSectionFDTrf2d(int tag, int classTag,int numSec, int nodeI, int nodeJ,CrdTransf2d &coordTransf)
  :BeamColumnWithSectionFD(tag, classTag,numSec,nullptr,nodeI,nodeJ), theCoordTransf(nullptr)
  { set_transf(&coordTransf); }

//! @brief Copy constructor.
XC::BeamColumnWithSectionFDTrf2d::BeamColumnWithSectionFDTrf2d(const BeamColumnWithSectionFDTrf2d &other)
  :BeamColumnWithSectionFD(other), theCoordTransf(nullptr)
  { set_transf(other.theCoordTransf); }

//! @brief Assignment operator.
XC::BeamColumnWithSectionFDTrf2d &XC::BeamColumnWithSectionFDTrf2d::operator=(const BeamColumnWithSectionFDTrf2d &other)
  {
    //BeamColumnWithSectionFD::operator=(other);
    //set_transf(other.theCoordTransf);
    std::cerr << getClassName()
	      << "; assignment operator not implemented." << std::endl;
    return *this;
  }

//! @brief Destructor.
XC::BeamColumnWithSectionFDTrf2d::~BeamColumnWithSectionFDTrf2d(void)
  {
    if(theCoordTransf) delete theCoordTransf;
    theCoordTransf= nullptr;
  }

//! @brief Returns (if possible) a pointer to the coordinate transformation.
XC::CrdTransf *XC::BeamColumnWithSectionFDTrf2d::getCoordTransf(void)
  { return theCoordTransf; }

//! @brief Returns (if possible) a pointer to the coordinate transformation.
const XC::CrdTransf *XC::BeamColumnWithSectionFDTrf2d::getCoordTransf(void) const
  { return theCoordTransf; }

//! @brief Send members through the communicator argument.
int XC::BeamColumnWithSectionFDTrf2d::sendData(Communicator &comm)
  {
    int res= BeamColumnWithSectionFD::sendData(comm);
    res+= sendCoordTransf(10,11,12,comm);
    return res;
  }

//! @brief Receives members through the communicator argument.
int XC::BeamColumnWithSectionFDTrf2d::recvData(const Communicator &comm)
  {
    int res= BeamColumnWithSectionFD::recvData(comm);
    theCoordTransf= recvCoordTransf2d(10,11,12,comm);
    return res;
  }
