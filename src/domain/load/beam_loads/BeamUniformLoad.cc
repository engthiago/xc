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
//BeamUniformLoad.cpp

#include "BeamUniformLoad.h"
#include "domain/mesh/element/Element1D.h"
#include "utility/geom/pos_vec/SlidingVectorsSystem3d.h"
#include "utility/geom/pos_vec/SlidingVector3d.h"
#include "domain/mesh/element/utils/coordTransformation/CrdTransf.h"

//! @brief Constructor.
XC::BeamUniformLoad::BeamUniformLoad(int tag,int classTag, double wt, double wa,const XC::ID &theElementTags)
  :BeamMecLoad(tag, classTag,wt,wa,theElementTags) {}

//! @brief Constructor.
XC::BeamUniformLoad::BeamUniformLoad(int tag,int classTag)
  :BeamMecLoad(tag, classTag) {}

//! @brief Constructor.
XC::BeamUniformLoad::BeamUniformLoad(int classTag)
  :BeamMecLoad(0,classTag) {}

std::string XC::BeamUniformLoad::Category(void) const
  { return "uniform"; }

//! brief Returns load resultant (force and moment integration over the elements).
SlidingVectorsSystem3d XC::BeamUniformLoad::getResultant(const Pos3d &centro, bool initialGeometry) const
  {
    SlidingVectorsSystem3d retval(centro);
    Matrix uniformLoads= getDistributedGlobalForces();
    const Domain *ptrDom= getDomain();
    if(ptrDom)
      {
        const size_t sz= uniformLoads.noRows();
        for(size_t i=0; i<sz; i++)
          {
            const size_t elemTag= getElementTags()(i);
            const Element1D *ptrElem= dynamic_cast<const Element1D *>(ptrDom->getElement(elemTag));
            if(ptrElem)
              {
                const CrdTransf *ptrTransf= ptrElem->getCoordTransf();
                if(ptrTransf)
		  {
                    const double l= ptrTransf->getLength(initialGeometry);
                    const Vector3d force(l*uniformLoads(i,0),l*uniformLoads(i,1),l*uniformLoads(i,2));
		    retval+= SlidingVector3d(ptrElem->getCenterOfMassPosition(),force);
		  }
                else
		  std::cerr << getClassName() << "::getResultant; the element: "
                            << elemTag << " no tiene coordinate transformation." << std::endl;

              }
            else
	      std::cerr << getClassName() << "::getResultant; the element: "
                        << elemTag << " is not a 1D element." << std::endl;
          }
      }
    else
      std::cerr << getClassName() << "::getResultant; pointer to domain is NULL." << std::endl;
    return retval;
  }

