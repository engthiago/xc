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
//Spot.h

#ifndef SPOT_H
#define SPOT_H

#include "EntGeomSection.h"
#include "utility/geom/pos_vec/Pos2d.h"

class BND2d;

namespace XC {
class Axis;

//! @ingroup MATSCCEntGeom
//
//! @brief Point object for section geometry definition.
class Spot: public EntGeomSection
  {
  private:
    Pos2d p; //!< Position of the point.
    std::set<const Axis *> pt_axes; //!< Edges that begin or end in this point (topology).
    friend class Axis;
    void insert_linea(Axis *l);
    void borra_linea(Axis *l);
  protected:

    virtual void update_topology(void);

    friend class GeomSection;
    //! @brief Constructor.
    Spot(const Pos2d &pto= Pos2d());
    //! @brief Constructor.
    Spot(GeomSection *m)
      : EntGeomSection("",m) {}
    //! @brief Constructor.
    Spot(const std::string &name= "",GeomSection *m= nullptr)
      : EntGeomSection(name,m) {}

  public:
    //! @brief Return the object dimension (0, 1, 2 or 3).
    inline virtual unsigned short int GetDimension(void) const
      { return 0; }
    BND2d Bnd(void) const;

    //! @brief Return the object position.
    const Pos2d &GetPos(void) const
      { return p; }
    //! @brief Return the object position.
    Pos2d &GetPos(void)
      { return p; }
    inline void setPos(const Pos2d &pos)
      { p= pos; }
    Vector2d VectorPos(void) const;

    //! @brief Return the list of lines that begin or end in this point.
    const std::set<const Axis *> &getConnectedAxes(void) const
      { return pt_axes; }
    bool isConnectedTo(const Axis &l) const;
    inline size_t nLines(void) const
      { return pt_axes.size(); }

    double getDistanceTo(const Pos2d &pt) const;


  };

} //end of XC namespace

#endif
