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
//Segment.h

#ifndef SEGMENT_H
#define SEGMENT_H

#include "Axis.h"


namespace XC {

//! @ingroup MATSCCEntGeom
//
//! @brief Line segment.
class Segment: public Axis
  {
    Spot *p1; //!< Back end of the segment.
    Spot *p2; //!< Front end of the segment.
  protected:

  public:
    Segment(SectionGeometry *m,const size_t &ndiv= 4);
    Segment(const std::string &name= "",SectionGeometry *m= nullptr,const size_t &ndiv= 4);
    Spot *P1(void);
    const Spot *P1(void) const;
    Spot *P2(void);
    const Spot *P2(void) const;
    void setEndPoints(size_t, size_t);
    int getVtkCellType(void) const;
    //! @brief Return the number of vertices.
    inline virtual size_t getNumberOfVertices(void) const
      { return 2; }
    virtual const Spot *getVertex(const size_t &i) const;
    virtual Spot *getVertex(const size_t &i);
    virtual void SetVertice(const size_t &,Spot *);
    virtual BND2d Bnd(void) const;
    double getLength(void) const;

  };

} //end of XC namespace
#endif
