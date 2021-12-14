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
                                                                        
// $Revision: 1.3 $
// $Date: 2003/02/14 23:01:37 $
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/repres/section_geometry/reinfLayer/StraightReinfLayer.cpp,v $
                                                                        
                                                                        
// File: StraightReinfLayer.C 
// Written by Remo M. de Souza 
// December 1998

#include <cmath>
#include <utility/matrix/Matrix.h>

#include <material/section/repres/section_geometry/reinfLayer/StraightReinfLayer.h>
#include <material/section/repres/section_geometry/reinfBar/ReinfBar.h>

#include "utility/utils/text/text_string.h"
#include "utility/geom/d1/Segment2d.h"


XC::StraightReinfLayer::StraightReinfLayer(ListReinfLayer *owr,Material *mat)
  : ReinfLayer(owr,mat,0,0.0,0.0),
    initPosit(2), finalPosit(2) {}

XC::StraightReinfLayer::StraightReinfLayer(ListReinfLayer *owr,Material *mat, int numReinfBars, 
                                       double reinfBarArea,
                                       const XC::Vector &InitialPosition,
                                       const XC::Vector &FinalPosition)
  : ReinfLayer(owr,mat,numReinfBars,0.0,reinfBarArea),
    initPosit(InitialPosition), finalPosit(FinalPosition) {}


void XC::StraightReinfLayer::setInitialPosition (const XC::Vector &initialPosition)
  { initPosit = initialPosition; }

void XC::StraightReinfLayer::setFinalPosition (const XC::Vector &finalPosition)
  { finalPosit = finalPosition; }

void XC::StraightReinfLayer::setInitialPos(const Pos2d &p)
  {
    initPosit[0]= p.x();
    initPosit[1]= p.y();
  }

Pos2d XC::StraightReinfLayer::getInitialPos(void) const
  { return Pos2d(initPosit(0),initPosit(1)); }

void XC::StraightReinfLayer::setFinalPos(const Pos2d &p)
  {
    finalPosit[0]= p.x();
    finalPosit[1]= p.y();
  }

Pos2d XC::StraightReinfLayer::getFinalPos(void) const
  { return Pos2d(finalPosit(0),finalPosit(1)); }


double XC::StraightReinfLayer::getMaxY(void) const
  {
    double retval= initPosit(0);
    retval= std::max(retval,finalPosit(0));
    return retval;
  }
double XC::StraightReinfLayer::getMaxZ(void) const
  {
    double retval= initPosit(1);
    retval= std::max(retval,finalPosit(1));
    return retval;
  }
double XC::StraightReinfLayer::getMinY(void) const
  {
    double retval= initPosit(0);
    retval= std::min(retval,finalPosit(0));
    return retval;
  }
double XC::StraightReinfLayer::getMinZ(void) const
  {
    double retval= initPosit(1);
    retval= std::min(retval,finalPosit(1));
    return retval;
  }

//! @brief Returns an array of reinforcement bars.
const XC::VectorReinfBar &XC::StraightReinfLayer::getReinfBars(void) const
  {
    Vector barPosit(2);
    const int num_barras= getNumReinfBars();
    const double area_barra= getReinfBarArea();

    if(num_barras == 1)
      {
        barPosit(0) = (initPosit(0) + finalPosit(0)) / 2;
        barPosit(1) = (initPosit(1) + finalPosit(1)) / 2;
    
        reinfBars.resize(1);
        reinfBars.put(0,ReinfBar(area_barra,getMaterialPtr(),barPosit));
      }
    else if(num_barras > 1)
      {
        const double dy = (finalPosit(0) - initPosit(0))/(num_barras - 1);
        const double dz = (finalPosit(1) - initPosit(1))/(num_barras - 1);

        reinfBars.resize(num_barras);

        for(int i= 0;i<num_barras;i++)
          {
            barPosit(0)= initPosit(0) + dy * i;
            barPosit(1)= initPosit(1) + dz * i;
            reinfBars.put(i,ReinfBar(area_barra,getMaterialPtr(),barPosit));
          }
      }
    return reinfBars;
  }

const XC::Vector &XC::StraightReinfLayer::getInitialPosition(void) const
  { return initPosit; }

const XC::Vector &XC::StraightReinfLayer::getFinalPosition(void) const
  { return finalPosit; }

//! @brief Returns the segment that lies both ends.
Segment2d XC::StraightReinfLayer::getLineSegment(void) const
  { return Segment2d(Pos2d(initPosit(0),initPosit(1)),Pos2d(finalPosit(0),finalPosit(1))); }

double XC::StraightReinfLayer::getLength(void) const
  { return getLineSegment().getLength(); }

//! @brier Returns spacement between rebars.
double XC::StraightReinfLayer::getSpacement(void) const
  { return getLength()/getNumReinfBars(); }

XC::ReinfLayer *XC::StraightReinfLayer::getCopy (void) const
  { return new StraightReinfLayer(*this); }

void XC::StraightReinfLayer::Print(std::ostream &s, int flag) const
  {
     s << "\nReinforcing Layer type:  Straight";
     ReinfLayer::Print(s,flag);
     s << "\nInitial Position: " << initPosit;
     s << "\nFinal Position: " << finalPosit;
  }

