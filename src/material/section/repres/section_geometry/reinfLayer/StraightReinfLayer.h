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
// $Source: /usr/local/cvs/OpenSees/SRC/material/section/repres/section_geometry/reinfLayer/StraightReinfLayer.h,v $
                                                                        
                                                                        
// File: StraightReinfLayer.h
// Written by Remo M. de Souza
// December 1998


#ifndef StraightReinfLayer_h 
#define StraightReinfLayer_h 

#include <material/section/repres/section_geometry/reinfLayer/ReinfLayer.h>
#include <utility/matrix/Vector.h>

class Segment2d;

namespace XC {
class ReinfBar;

//! @ingroup MATSCCReinforcement
//
//! @brief Set of rebars distributed along a segment.
class StraightReinfLayer: public ReinfLayer
  {
  private:
    Vector initPosit; //!< Segment back end.
    Vector finalPosit; //!< Segment front end.
  protected:


    friend class ListReinfLayer;
    StraightReinfLayer(ListReinfLayer *,Material *mat);
    StraightReinfLayer(ListReinfLayer *,Material *mat, int numReinfBars, double  reinfBarArea,
                       const Vector &initialPosition, 
                       const Vector &finalPosition);
    ReinfLayer *getCopy(void) const;

  public:
    // edition functions
    void setInitialPosition(const Vector &);
    const Vector &getInitialPosition(void) const;
    void setFinalPosition(const Vector &);
    const Vector &getFinalPosition(void) const;
    void setInitialPos(const Pos2d &);
    Pos2d getInitialPos(void) const;
    void setFinalPos(const Pos2d &);
    Pos2d getFinalPos(void) const;

    // inquiring functions
    const VectorReinfBar &getReinfBars(void) const;
  

    double getMaxY(void) const;
    double getMaxZ(void) const;
    double getMinY(void) const;
    double getMinZ(void) const;
    Segment2d getLineSegment(void) const;
    double getLength(void) const;
    double getSpacement(void) const;


    void Print(std::ostream &s, int flag =0) const;   
  };
} // end of XC namespace


#endif

