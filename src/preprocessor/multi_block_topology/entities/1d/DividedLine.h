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
//DividedLine.h

#ifndef DIVIDEDLINE_H
#define DIVIDEDLINE_H

#include "Line.h"

namespace XC {

//! @ingroup MultiBlockTopologyEnt
//!
//! @brief Line divided in segments of specified lengths.
class DividedLine: public Line
  {
    std::vector<double> lengths; //!< Length of the line segments.
  protected:
    Pos3dArray get_positions(void) const;
  public:
    DividedLine(Preprocessor *m,const size_t &ndiv= 4);
    DividedLine(const std::string &name= "",Preprocessor *m= nullptr,const size_t &ndiv= 4);
    virtual bool operator==(const DividedLine &) const;
    void setLengths(const Vector &);
    virtual SetEstruct *getCopy(void) const;

    boost::python::dict getPyDict(void) const;
    void setPyDict(const boost::python::dict &);
  };

} //end of XC namespace
#endif
