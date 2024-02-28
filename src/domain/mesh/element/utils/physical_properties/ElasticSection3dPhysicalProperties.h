// -*-c++-*-
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
//ElasticSection3dPhysicalProperties.h

#ifndef ElasticSection3dPhysicalProperties_h
#define ElasticSection3dPhysicalProperties_h

#include "ElasticSectionPhysicalProperties.h"
#include "material/section/elastic_section/ElasticSection3d.h"

namespace XC {

//! @ingroup PhysicalProperties
//
//! @brief Three-dimensional elastic section physical properties.
class ElasticSection3dPhysicalProperties: public ElasticSectionPhysicalProperties<ElasticSection3d>
  {
  public:
    inline ElasticSection3dPhysicalProperties(const size_t &nMat= 0)
      : ElasticSectionPhysicalProperties<ElasticSection3d>(nMat) {}
    void set(const size_t &i, const CrossSectionProperties3d &);
    void set(const CrossSectionProperties3d &);

    Vector getGeneralizedStrainAtGaussPointsByName(const std::string &) const;
    Vector getGeneralizedStressAtGaussPointsByName(const std::string &) const;

  }; 

} // end of XC namespace
#endif
