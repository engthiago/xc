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
                                                                        
// $Revision: 1.1.1.1 $
// $Date: 2000/09/15 08:23:22 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/repres/section/SectionRepres.cpp,v $
                                                                        
                                                                        
// File: SectionRepres.C
//
// Written by Remo M. de Souza
// November 1998

#include <material/section/repres/section/SectionRepres.h>
#include <material/section/repres/section_geometry/SectionGeometry.h>
#include "preprocessor/prep_handlers/MaterialHandler.h"
#include "utility/utils/misc_utils/colormod.h"


XC::SectionRepres::SectionRepres(int tag,MaterialHandler *ml)
  : TaggedObject(tag), material_handler(ml), gmSecc(nullptr) {}

//! @brief Copy constructor.
XC::SectionRepres::SectionRepres(const SectionRepres &other)
  : TaggedObject(other), material_handler(other.material_handler), gmSecc(other.gmSecc) {}

//! @brief Assignment operator.
XC::SectionRepres &XC::SectionRepres::operator=(const SectionRepres &other)
  {
    SectionRepres::operator=(other);
    material_handler= other.material_handler;
    gmSecc= other.gmSecc;
    return *this;
  }

//! @brief Returns the total number of cells.
int XC::SectionRepres::getNumCells(void) const
  {
    int ncells= 0;
    if(gmSecc)
      ncells= gmSecc->getRegions().getNumCells();
    return ncells;
  }

//! @brief Return a pointer to the material handler.
XC::MaterialHandler *XC::SectionRepres::getMaterialHandler(void)
  { return material_handler; }

//! @brief Return a pointer to the material handler.
const XC::MaterialHandler *XC::SectionRepres::getMaterialHandler(void) const
  { return material_handler; }

//! @brief Sets section geometry from its name.
void XC::SectionRepres::setGeomNamed(const std::string &nmbGeom)
  {
    const SectionGeometry *tmp= material_handler->find_ptr_section_geometry(nmbGeom);
    if(tmp)
      setGeom(tmp);
    else
     std::cerr << Color::red << getClassName() << "::" << __FUNCTION__
	       << " section geometry: '"
               << nmbGeom << "' not found."
               << Color::def << std::endl;
  }

//! @brief Sets section geometry.
void XC::SectionRepres::setGeom(const SectionGeometry *g)
  { gmSecc= g; }

//! @brief Returns a pointer to section geometry.
const XC::SectionGeometry *XC::SectionRepres::getGeom(void) const
  { return gmSecc; }

//! @brief Printing...
void XC::SectionRepres::Print(std::ostream &s, int flag) const
  {
    s << "\nSection representation type: " << getType() << std::endl;
  }


std::ostream &XC::operator<<(std::ostream &s, XC::SectionRepres &sectionRepr)
  {
    sectionRepr.Print(s);
    return s;
  }
