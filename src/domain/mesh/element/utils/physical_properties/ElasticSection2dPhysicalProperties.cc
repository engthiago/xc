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
//ElasticSection2dPhysicalProperties.cpp

#include "ElasticSection2dPhysicalProperties.h"
#include <domain/domain/Domain.h>
#include "domain/mesh/element/utils/Information.h"
#include "utility/recorder/response/ElementResponse.h"

//! @brief Assigns section properties for integration point i
void XC::ElasticSection2dPhysicalProperties::set(const size_t &i, const CrossSectionProperties2d &sp)
   { theMaterial[i]->setCrossSectionProperties(sp); }

//! @brief Assigns the given section properties to all the integration points.
void XC::ElasticSection2dPhysicalProperties::set(const CrossSectionProperties2d &sp)
   {
     for(material_vector::iterator i= theMaterial.begin();i!= theMaterial.end();i++)
       {
         (*i)->setCrossSectionProperties(sp);
       }     
   }

//! @brief Returns the components of the strain vector which has the code being passed as parameter.
//! @param cod: component code.
XC::Vector XC::ElasticSection2dPhysicalProperties::getGeneralizedStressAtGaussPointsByName(const std::string &cod) const
  {
    const size_t nMat= theMaterial.size();
    Vector retval(nMat);
    for(size_t i= 0; i<nMat; i++)
      {
	const ElasticSection2d *section= dynamic_cast<const ElasticSection2d *>(theMaterial[i]);
	const Vector R= section->getStressResultant();
	if(cod == "N") // Internal axial force.
	  retval[i]= R[0];
	else if(cod == "M" || cod == "Mz") // Bending around the axis 1.
	  retval[i]= R[1];
	else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "stress code: '" << cod << " unknown." << std::endl;
      }
    return retval;
  }

//! @brief Returns the components of the stress vector which has the code being passed as parameter.
//!
//! @param cod component code.
XC::Vector XC::ElasticSection2dPhysicalProperties::getGeneralizedStrainAtGaussPointsByName(const std::string &cod) const
  {
    const size_t nMat= theMaterial.size();
    Vector retval(nMat);
    for(size_t i= 0; i<nMat; i++)
      {
	const ElasticSection2d *section= dynamic_cast<const ElasticSection2d *>(theMaterial[i]);
	const Vector e= section->getSectionDeformation();
	if(cod == "N") // axial strain.
	  retval[i]= e[0];
	else if(cod == "M" || cod == "Mz") // Bending around the axis 1.
	  retval[i]= e[1];
	else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "stress code: '" << cod << " unknown." << std::endl;
      }
    return retval;
  }


