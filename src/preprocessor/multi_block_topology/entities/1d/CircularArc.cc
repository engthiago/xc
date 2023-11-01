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
//CircularArc.cc

#include "preprocessor/Preprocessor.h"
#include "CircularArc.h"
#include "preprocessor/multi_block_topology/entities/0d/Pnt.h"
#include "utility/geom/d3/BND3d.h"
#include "utility/geom/pos_vec/Pos3dArray.h"
#include "utility/geom/pos_vec/Pos3dArray3d.h"
#include "utility/geom/d2/CircularSector3d.h"

#include "domain/mesh/node/Node.h"
#include "domain/mesh/element/Element.h"
#include "vtkCellType.h"

//! @brief Constructor.
XC::CircularArc::CircularArc(Preprocessor *m)
  : LineBase(m), p3(nullptr) {}

//! @brief Constructor.
XC::CircularArc::CircularArc(const std::string &name,Preprocessor *m)
  : LineBase(name,m), p3(nullptr) {}

//! @brief Comparison operator.
bool XC::CircularArc::operator==(const CircularArc &other) const
  {
    bool retval= false;
    if(this==&other)
      retval= true;
    else
      {
        retval= LineBase::operator==(other);
        if(retval)
          retval= (p3==other.p3);
       }
    return retval;
  }

//! @brief Virtual constructor.
XC::SetEstruct *XC::CircularArc::getCopy(void) const
  { return new CircularArc(*this); }

//! @brief Return a constant pointer to the midpoint of the arc.
const XC::Pnt *XC::CircularArc::P3(void) const
  { return p3; }

//! @brief Return the i-th vertex.
const XC::Pnt *XC::CircularArc::getVertex(const size_t &i) const
  {
    if(i<3)
      return LineBase::getVertex(i);
    else
      return p3;
  }

//! @brief Set the i-th vertex.
void XC::CircularArc::SetVertice(const size_t &i,Pnt *p)
  {
    if(i<3)
      LineBase::SetVertice(i,p);
    else
      {
        if(p3) p3->erase_line(this);
        p3= p;
        if(p3)
          {
            p3->setGenMesh(false); //Intermediate point of the line.
            p3->insert_line(this);
          }
      }
  }

//! @brief Check that the points are defined.
bool XC::CircularArc::check_points(void) const
  {
    bool retval= false;
    if(p1 && p2 && p3)
      retval= true;
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; arc: '" << getName()
                << " is not defined." << std::endl;
    return retval;
  }
  
//! @brief Return the circle sector correlated with the arc.
const CircularSector3d XC::CircularArc::get_sector_circular3d(void) const
  {
    CircularSector3d retval;
    if(check_points())
      retval= CircularSector3d(p1->getPos(),p3->getPos(),p2->getPos());
    return retval;
  }

//! @brief Return the arc length.
double XC::CircularArc::getLength(void) const
  {
    double retval= 0;
    if(check_points())
      retval= get_sector_circular3d().getArcLength();
    return retval;
  }

//! @brief Return the centroid of the arc.
Pos3d XC::CircularArc::getCentroid(void) const
  {
    Pos3d retval(0.0,0.0,0.0);
    if(check_points())
      retval= get_sector_circular3d().getCenterOfMass();
    return retval;
  }

//! @brief Return the angle subtended by the arc.
double XC::CircularArc::getIncludedAngle(void) const
  {
    double retval= 0;
    if(check_points())
      retval= get_sector_circular3d().getIncludedAngle();
    return retval;
  }

//! @brief Return the start angle.
double XC::CircularArc::getTheta1(void) const
  {
    double retval= 0;
    if(check_points())
      retval= get_sector_circular3d().Theta1();
    return retval;
  }

//! @brief Return the end angle.
double XC::CircularArc::getTheta2(void) const
  {
    double retval= 0;
    if(check_points())
      retval= get_sector_circular3d().Theta2();
    return retval;
  }

//! @brief Return the parameter of the point on the arc (distance to the
//! arc's first point measured over the arc)
double XC::CircularArc::getLambda(const Pos3d &p) const
  {
    double retval= 0;
    if(check_points())
      retval= get_sector_circular3d().getLambda(p);
    return retval;
  }

//! @brief Return the center of the circumference.
Pos3d XC::CircularArc::getCenter(void) const
  {
    Pos3d retval;
    if(check_points())
      retval= get_sector_circular3d().Centro();
    return retval;
  }

//! @brief Return the start point.
Pos3d XC::CircularArc::getPInic(void) const
  {
    Pos3d retval;
    if(check_points())
      retval= get_sector_circular3d().PInic();
    return retval;
  }

//! @brief Return the end point.
Pos3d XC::CircularArc::getPFin(void) const
  {
    Pos3d retval;
    if(check_points())
      retval= get_sector_circular3d().PFin();
    return retval;
  }

//! @brief Return the midpoint.
Pos3d XC::CircularArc::getPMed(void) const
  {
    Pos3d retval;
    if(check_points())
      retval= get_sector_circular3d().PMed();
    return retval;
  }

//! @brief Return the radius.
double XC::CircularArc::getRadius(void) const
  {
    double retval= 0;
    if(check_points())
      retval= get_sector_circular3d().getRadius();
    return retval;
  }

//! @brief Updates topology
void XC::CircularArc::update_topology(void)
  {
    LineBase::update_topology();
    if(P3()) P3()->insert_line(this);
  }

//! @brief Returns object boundary.
BND3d XC::CircularArc::Bnd(void) const
  { return get_sector_circular3d().Bnd(); }

//! @brief Return ndiv+1 equally-sapaced positions along the arc.
Pos3dArray XC::CircularArc::get_positions(void) const
{ return get_sector_circular3d().getArcPoints(NDiv()+1); }

//! @brief Interface with VTK.
int XC::CircularArc::getVtkCellType(void) const
  { return VTK_QUADRATIC_EDGE; }

//! @brief Return k-points.
XC::ID XC::CircularArc::getKPoints(void) const
  {
    ID retval(3);
    retval[0]= P1()->getTag();
    retval[1]= P2()->getTag();
    retval[2]= P2()->getTag();
    return retval;
  }

//! @brief Return the segments that compose the edge.
std::deque<Segment3d> XC::CircularArc::getSegments(void) const
  {
    std::deque<Segment3d> retval;
    std::cerr << getClassName() << "::" << __FUNCTION__
              << "; not implemented." << std::endl;
    return retval;
  }

//! @brief Return a Python dictionary with the object members values.
boost::python::dict XC::CircularArc::getPyDict(void) const
  {
    boost::python::dict retval= LineBase::getPyDict();
    int tmp= -1;
    if(p3)
      tmp= p3->getTag();
    retval["p3"]= tmp;
    return retval;
  }

//! @brief Set the values of the object members from a Python dictionary.
void XC::CircularArc::setPyDict(const boost::python::dict &d)
  {
    LineBase::setPyDict(d);
    const int tagP3= boost::python::extract<int>(d["p3"]);
    Preprocessor *preprocessor= getPreprocessor();
    if(preprocessor)
      {
	MultiBlockTopology &mbt= preprocessor->getMultiBlockTopology();
	PntMap &points= mbt.getPoints();
	if(tagP3>=0)
	  p3= points.busca(tagP3);
	this->update_topology();
      }
    else
      std::cerr << getClassName() << __FUNCTION__
	        << "; preprocessor needed." << std::endl;
  }

