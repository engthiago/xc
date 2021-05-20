// -*-c++-*-
//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis Claudio Pérez Tato
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
//KDTreeElements.h
#ifndef KDTreeElements_h
#define KDTreeElements_h

#include "utility/geom/pos_vec/KDTreePos.h"
#include "utility/kdtree++/kdtree.hpp"

class Pos3d;

namespace XC {
class Element;

//! @brief Element position for KDTree building.
//! @ingroup FEMisc
class ElemPos: public KDTreePos
  {
  private:
    const Element *elemPtr;
  public:
    ElemPos(const Element &);
    explicit ElemPos(const Pos3d &p);
    inline const Element *getElementPtr(void) const
      { return elemPtr; }

    static inline double tac( ElemPos p, size_t k ) { return p[k]; }
  };

inline bool operator==(const ElemPos &A,const ElemPos &B)
  { return ((A.getElementPtr()== B.getElementPtr()) && (A[0] == B[0]) && (A[1] == B[1]) && (A[2] == B[2])); }


//! @brief k-d tree for searching the nearest element to a given position.
//! @ingroup FEMisc
//!
//! See <a href="https://en.wikipedia.org/wiki/K-d_tree"> k-d tree <\a>
class KDTreeElements: protected kd_tree::KDTree<3, ElemPos, std::pointer_to_binary_function<ElemPos,size_t,double> >
  {
    size_t pend_optimizar;
  public:
    typedef kd_tree::KDTree<3, ElemPos, std::pointer_to_binary_function<ElemPos,size_t,double> > tree_type;
    KDTreeElements(void);

    void insert(const Element &);
    void erase(const Element &);
    void clear(void);

    const Element *getNearest(const Pos3d &pos) const;
    const Element *getNearest(const Pos3d &pos, const double &r) const;
  };

} // end of XC namespace 


#endif
