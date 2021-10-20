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
//SectionGeometry.cpp

#include <material/section/repres/section_geometry/SectionGeometry.h>

#include <material/section/repres/section_geometry/SectionReferenceFrame.h>
#include <material/section/repres/section_geometry/SectionCartesianReferenceFrame.h>
#include <material/section/repres/section_geometry/Spot.h>
#include <material/section/repres/section_geometry/Segment.h>

#include "preprocessor/prep_handlers/MaterialHandler.h"
#include "material/section/repres/CrossSectionProperties3d.h"
#include "material/section/repres/CrossSectionProperties2d.h"

#include <material/section/repres/section_geometry/region/SectRegion.h>

#include "material/section/repres/section_geometry/reinfLayer/ReinfLayer.h"

#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"
#include "utility/geom/pos_vec/Dir2d.h"
#include "utility/geom/pos_vec/Vector2d.h"
#include "utility/geom/pos_vec/Pos2d.h"
#include "utility/geom/ref_sys/Ref2d2d.h"
#include "utility/geom/d2/2d_polygons/Polygon2d.h"
#include "utility/geom/d2/HalfPlane2d.h"
#include "utility/geom/d1/Segment2d.h"

#include "boost/lexical_cast.hpp"

XC::SectionGeometry::SectionGeometry(MaterialHandler *ml)
  : SectionMassProperties(),material_handler(ml), regions(ml), reinforcement_layers(this,ml), tag_ref_sys(0),tag_spot(0) {}

//! @brief Returns a geometry that contains only the regions
//! defined in this object.
XC::SectionGeometry XC::SectionGeometry::getGMRegions(void) const
  {
    SectionGeometry retval(material_handler);
    retval.regions= regions;
    return retval;
  }

//! @brief Returns a geometry that contains only the regions
//! defined in this object.
XC::SectionGeometry XC::SectionGeometry::getGMReinforcementLayers(void) const
  {
    SectionGeometry retval(material_handler);
    retval.reinforcement_layers= reinforcement_layers;
    return retval;
  }

//! @brief Return a section with only the compressed regions of the section.
XC::SectionGeometry XC::SectionGeometry::getCrackedSection(const HalfPlane2d &sp_compressions) const
  {
    SectionGeometry retval(getGMReinforcementLayers());
    retval.regions= regions.Intersection(sp_compressions);
    return retval;
  }

size_t XC::SectionGeometry::getNumFiberData(void) const
  { return (regions.getNumCells() + reinforcement_layers.getNumReinfBars()); }

//! @brief Returns a pointer to the reference system which identifier is being passed as parameter.
XC::SectionReferenceFrame *XC::SectionGeometry::get_reference_system(const size_t &id)
  {
    SectionReferenceFrame *retval= nullptr;
    if(id>0) //0 is reserved for the universal coordinate system.
      {
        lst_ref_sys::iterator i= reference_systems.find(id);
        if(i!= reference_systems.end()) //Reference system exists.
        retval= (*i).second;
      }
    return retval;
  }

//! @brief Returns a const pointer to the reference system which identifier is being passed as parameter.
const XC::SectionReferenceFrame *XC::SectionGeometry::get_reference_system(const size_t &id) const
  {
    SectionReferenceFrame *retval= nullptr;
    if(id>0) //0 is reserved for the universal coordinate system.
      {
        lst_ref_sys::const_iterator i= reference_systems.find(id);
        if(i!= reference_systems.end()) //Reference system exists.
          retval= (*i).second;
      }
    return retval;
  }

//! @brief Returns a pointer to the point identified by the argument.
XC::Spot *XC::SectionGeometry::find_spot(const size_t &id)
  {
    Spot *retval= nullptr;
    lst_spots::iterator i= spots.find(id);
    if(i!= spots.end()) //Point exists.
      retval= (*i).second;
    return retval;
  }

//! @brief Returns a pointer to the point identified by the argument.
const XC::Spot *XC::SectionGeometry::find_spot(const size_t &id) const
  {
    Spot *retval= nullptr;
    lst_spots::const_iterator i= spots.find(id);
    if(i!= spots.end()) //Point exists.
      retval= (*i).second;
    return retval;
  }

//! @brief Returns a pointer to the axis which name is being passed as parameter.
XC::Axis *XC::SectionGeometry::find_axis(const size_t &id)
  {
    Axis *retval= nullptr;
    axes_container::iterator i= axes.find(id);
    if(i!= axes.end()) //Point exists.
      retval= (*i).second;
    return retval;
  }

//! @brief Returns a pointer to the axis which name is being passed as parameter.
const XC::Axis *XC::SectionGeometry::find_axis(const size_t &id) const
  {
    const Axis *retval= nullptr;
    axes_container::const_iterator i= axes.find(id);
    if(i!= axes.end()) //Point exists..
      retval= (*i).second;
    return retval;
  }

//! @brief Creates a new reference frame of the type being passed as parameter.
XC::SectionReferenceFrame *XC::SectionGeometry::createReferenceFrame(const std::string &type)
  {
    SectionReferenceFrame *retval= get_reference_system(tag_ref_sys);
    if(!retval) //New reference system.
      {
        if((type == "cartesian") || (type == "cartesianas"))
          {
            retval= new SectionCartesianReferenceFrame("r"+boost::lexical_cast<std::string>(tag_ref_sys),this);
            reference_systems[tag_ref_sys]= retval;
            tag_ref_sys++;
          }
        else
	  std::cerr << getClassName() << __FUNCTION__
		    << "; type: '" << type
                    << "' unknown." << std::endl;
      }
    return retval;
  }

//! @brief Creates a new point.
XC::Spot *XC::SectionGeometry::creaSpot(const Pos2d &p)
  {
    Spot *retval= find_spot(tag_spot);
    if(!retval) //New point.
      {
        retval= new Spot("p"+boost::lexical_cast<std::string>(tag_spot),this);
        spots[tag_spot]= retval;
        retval->setPos(p);
        tag_spot++;
      }
    return retval;
  }

//! @brief New spot.
XC::Spot *XC::SectionGeometry::newSpot(const Pos2d &p)
  {
    Pos2d trfP(p);
    if(tag_ref_sys != 0) //Not the global coordinate system.
      {
        SectionReferenceFrame *sr= get_reference_system(tag_ref_sys);
        if(sr)
          trfP= sr->getGlobalPosition(p); //Pass to global coordinates.
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; reference system with identifier: "
		    << tag_ref_sys << " not found.\n";
       }
    Spot *retval= creaSpot(trfP);
    return retval;
  }

//! @brief New segment.
XC::Segment *XC::SectionGeometry::newSegment(size_t p1,size_t p2)
  {
    Axis *e= createAxis<Segment>();
    Segment *s= dynamic_cast<Segment *>(e);
    if(s)
      s->setEndPoints(p1,p2);
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; can't assign endpoints."
                << std::endl;
    return s;
  }


//! @brief Return the distance between the points identified by the
//! arguments.
double XC::SectionGeometry::DistSpots(const size_t &i,const size_t &j) const
  {
    double retval(-1.0);
    const Spot *pA= find_spot(i);
    const Spot *pB= find_spot(j);
    if(!pA)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; point: " << i << " not found. " << std::endl;
    else if(!pB)
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; point: " << j << " not found. " << std::endl;
    else
      retval= pA->getDistanceTo(pB->getPos());
    return retval;
  }

//! @brief Return the contour of the regions.
Polygon2d XC::SectionGeometry::getRegionsContour(void) const
  {
    Polygon2d retval;
    const std::list<Polygon2d> tmp= regions.getContours();
    if(!tmp.empty())
      {
        if(tmp.size()>1)
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; cross section is not a simply connected region."
                    << std::endl;
        retval= *tmp.begin();
      }
    return retval;
  }

//! @brief Return the contour of the compressed part of the regions.
Polygon2d XC::SectionGeometry::getCompressedZoneContour(const HalfPlane2d &sp_compressions) const
  {
    Polygon2d retval;
    Polygon2d tmp= getRegionsContour();
    if(!tmp.empty())
      {
	std::list<Polygon2d> tmpList= tmp.getIntersection(sp_compressions);
        if(tmpList.size()>1)
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; is not a simply connected region."
                    << std::endl;
        retval= *tmpList.begin();
      }
    return retval;
  }

//! @brief Return the working cross-section lever arm from the position
//! of the half-plane being passed as parameter.
//! @param PFtrace: Trace of the bending plane with over plane that
//! contains the cross section.
double XC::SectionGeometry::getLeverArm(const Line2d &PFtrace) const
  {
    Polygon2d contour= getRegionsContour();
    Pos2d C= contour.getCenterOfMass();
    HalfPlane2d sp(PFtrace.Perpendicular(C));
    const size_t num_vertices= contour.getNumVertices();
    double d= 0.0,dpos= 0.0,dneg=0.0;    
    for( size_t i=1;i<=num_vertices;i++)
      {
        d= sp.DistSigno(contour.Vertice(i));
        if(d<dneg) dneg= d;
        if(d>dpos) dpos= d;
      }
    assert(dneg<=0);
    assert(dpos>=0);
    return dpos-dneg;
  }

//! @brief Return the section depth from the border of the half-plane
//! being passed as parameter to the most compressed fiber.
double XC::SectionGeometry::getCompressedZoneDepth(const HalfPlane2d &sp_compressions) const
  {    
    Polygon2d contour= getRegionsContour();
    const size_t num_vertices= contour.getNumVertices();
    double d= 0.0,dneg= 0.0;    
    for( size_t i=1;i<=num_vertices;i++)
      {
        d= sp_compressions.DistSigno(contour.Vertice(i));
        if(d<dneg) dneg= d;
      }
    assert(dneg<=0);
    return dneg;
  }

//! @brief Return the section depth from the border of the half-plane
//! being passed as parameter to the most tensioned fiber.
double XC::SectionGeometry::getTensionedZoneDepth(const HalfPlane2d &sp_compressions) const
  {
    Polygon2d contour= getRegionsContour();
    const size_t num_vertices= contour.getNumVertices();
    double d= 0.0,dpos=0.0;    
    for( size_t i=1;i<=num_vertices;i++)
      {
        d= sp_compressions.DistSigno(contour.Vertice(i));
        if(d>dpos) dpos= d;
      }
    assert(dpos>=0);
    return dpos;
  }

//! @brief Return the lengths of the segments that results of
//! cutting the line being passed as parameter with the section
//! contour.
double XC::SectionGeometry::getCutLength(const Line2d &r) const
  {
    double retval= 0.0;
    Polygon2d contour= append_mid_points(getRegionsContour());
    if(contour.Overlap(r))
      retval= contour.Clip(r).getLength();
    return retval;
  }

//! @brief Return the lengths of the segments that results of
//! cutting the line being passed as parameter with the section
//! contour.
std::vector<double> XC::SectionGeometry::getCutLengths(const std::list<Line2d> &lr) const
  {
    const size_t sz= lr.size();
    std::vector<double> retval;
    if(sz>0)
      {
        Polygon2d contour= append_mid_points(getRegionsContour());
        int conta= 0;
        for(std::list<Line2d>::const_iterator i= lr.begin();i!=lr.end();i++,conta++)
          {
            const Line2d &r= *i;
            if(contour.Overlap(r))
              retval[conta]= contour.Clip(r).getLength();
          }
      }
    return retval;
  }

//! @brief Return the section width for the bending plane intersect
//! being passed as parameter.
double XC::SectionGeometry::getAnchoMecanico(const Line2d &bending_plane_trace) const
  {
    const Polygon2d contour= append_mid_points(getRegionsContour());
    const size_t num_vertices= contour.getNumVertices();
    double d= 0.0,dmax= 0.0;
    Line2d perp;
    Segment2d ancho;
    for( size_t i=1;i<=num_vertices;i++)
      {
        perp= bending_plane_trace.Perpendicular(contour.Vertice(i));
        ancho= contour.Clip(perp);
        d= ancho.getLength();
        if(d>dmax)
          dmax= d;
      }
    assert(dmax>=0);
    return dmax;
  }

//! @brief Return the width «b0» of the compressed strut
//! that corresponds to the arm lever represented by the segment being passed as parameter.
double XC::SectionGeometry::getCompressedStrutWidth(const Segment2d &lever_arm) const
  {
    const Polygon2d contour= append_mid_points(getRegionsContour());
    const size_t num_vertices= contour.getNumVertices();
    Line2d perp= lever_arm.getPerpendicularBisector();
    Segment2d ancho= contour.Clip(perp);
    Pos2d p= intersection_point(ancho,lever_arm);
    assert(p.exists());
    double b2= std::min(dist2(p,ancho.getFromPoint()),dist2(p,ancho.getToPoint()));
    double bmin2= b2;
    bool intersecaBrazo= false;
    for( size_t i=1;i<=num_vertices;i++)
      {
        perp= lever_arm.Perpendicular(contour.Vertice(i));
        intersecaBrazo= lever_arm.intersects(perp);
        if(intersecaBrazo)
          {
            ancho= contour.Clip(perp);
            p= intersection_point(ancho,lever_arm);
            if(p.exists())
              {
                b2= std::min(dist2(p,ancho.getFromPoint()),dist2(p,ancho.getToPoint()));
                if(b2<bmin2)
                  bmin2= b2;
              }
          }
      }
    assert(bmin2>=0);
    return 2*sqrt(bmin2);
  }

//! @brief Return the cover for the position being passed as parameter.
double XC::SectionGeometry::getCover(const Pos2d &p) const
  {
    const double retval= -getRegionsContour().DistSigno(p);
    if(retval<0)
      std::clog << getClassName() << __FUNCTION__
		<< "; warning! position: " << p
                << " is outside the section." << std::endl;
    return retval;
  }

//! @brief Return the homogenized area of the regions.
double XC::SectionGeometry::getAreaHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    if(!regions.empty())
      retval+= regions.getAreaHomogenizedSection(E0);
    if(!reinforcement_layers.empty())
      retval+= reinforcement_layers.getAreaHomogenizedSection(E0);
    return retval;
  }

XC::Vector XC::SectionGeometry::getCenterOfMassHomogenizedSection(const double &E0) const
  {
    Vector retval(2);
    double weight= 0.0;
    double divisor= 0.0;
    weight= regions.getAreaHomogenizedSection(E0);
    retval+= weight*regions.getCenterOfMassHomogenizedSection(E0);
    divisor+= weight;
    weight= reinforcement_layers.getAreaHomogenizedSection(E0);
    retval+= weight*reinforcement_layers.getCenterOfMassHomogenizedSection(E0);
    divisor+= weight;
    retval/= divisor;
    return retval;
  }

//! @brief Returns homogenized moment of inertia of the cross-section with respecto to the axis parallel to y passing through the centroid.
//! @param E0: Reference elastic modulus.
double XC::SectionGeometry::getIyHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    double d= 0.0;
    const double zCenterOfMass= getCenterOfMassHomogenizedSection(E0)[1];

    if(!regions.empty())
      {
        d= regions.getCenterOfMassHomogenizedSection(E0)[1]-zCenterOfMass;
        retval+= regions.getIyHomogenizedSection(E0)+regions.getAreaHomogenizedSection(E0)*sqr(d);
      }
    if(!reinforcement_layers.empty())
      {
        d= reinforcement_layers.getCenterOfMassHomogenizedSection(E0)[1]-zCenterOfMass;
        retval+= reinforcement_layers.getIyHomogenizedSection(E0)+reinforcement_layers.getAreaHomogenizedSection(E0)*sqr(d);
      }
    return retval;
  }

//! @brief Returns homogenized moment of inertia of the cross-section with respecto to the axis parallel to z passing through the centroid.
//! @param E0: Reference elastic modulus.
double XC::SectionGeometry::getIzHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    double d= 0.0;
    const double yCenterOfMass= getCenterOfMassHomogenizedSection(E0)[0];

    if(!regions.empty())
      {
        d= regions.getCenterOfMassHomogenizedSection(E0)[0]-yCenterOfMass;
        retval+= regions.getIzHomogenizedSection(E0)+regions.getAreaHomogenizedSection(E0)*sqr(d);
      }
    if(!reinforcement_layers.empty())
      {
        d= reinforcement_layers.getCenterOfMassHomogenizedSection(E0)[0]-yCenterOfMass;
        retval+= reinforcement_layers.getIzHomogenizedSection(E0)+reinforcement_layers.getAreaHomogenizedSection(E0)*sqr(d);
      }
    return retval;
  }

//! @brief Returns homogenized product of inertia of the cross-section with respecto to the axis parallel to y and z passing through the centroid.
//! @param E0: Reference elastic modulus.
double XC::SectionGeometry::getPyzHomogenizedSection(const double &E0) const
  {
    double retval= 0.0;
    double d2= 0.0;
    const double zCenterOfMass= getCenterOfMassHomogenizedSection(E0)[0];
    const double yCenterOfMass= getCenterOfMassHomogenizedSection(E0)[0];

    if(!regions.empty())
      {
        d2= (regions.getCenterOfMassHomogenizedSection(E0)[0]-yCenterOfMass)*(regions.getCenterOfMassHomogenizedSection(E0)[1]-zCenterOfMass);
        retval+= regions.getPyzHomogenizedSection(E0)+regions.getAreaHomogenizedSection(E0)*d2;
      }
    if(!reinforcement_layers.empty())
      {
        d2= (reinforcement_layers.getCenterOfMassHomogenizedSection(E0)[0]-yCenterOfMass)*(reinforcement_layers.getCenterOfMassHomogenizedSection(E0)[1]-zCenterOfMass);
        retval+= reinforcement_layers.getPyzHomogenizedSection(E0)+reinforcement_layers.getAreaHomogenizedSection(E0)*d2;
      }
    return retval;
  }

//! @brief Returns region's gross section area.
double XC::SectionGeometry::getAreaGrossSection(void) const
  {
    double retval= 0.0;
    if(!regions.empty())
      retval+= regions.getAreaGrossSection();
    // if(!reinforcement_layers.empty())
    //   retval+= reinforcement_layers.getAreaGrossSection();
    return retval;
  }

//! @brief Returns gross section centroid position.
XC::Vector XC::SectionGeometry::getCenterOfMassGrossSection(void) const
  { return regions.getCenterOfMassGrossSection(); }

//! @brief Inertia of the gross section about an axis parallel to y through its centroid.
double XC::SectionGeometry::getIyGrossSection(void) const
  {
    double retval= 0.0;
    double d= 0.0;
    const double zCenterOfMass= getCenterOfMassGrossSection()[1];

    if(!regions.empty())
      {
        d= regions.getCenterOfMassGrossSection()[1]-zCenterOfMass;
        retval+= regions.getIyGrossSection()+regions.getAreaGrossSection()*sqr(d);
      }
    // if(!reinforcement_layers.empty())
    //   {
    //     d= reinforcement_layers.getCenterOfMassGrossSection()[1]-zCenterOfMass;
    //     retval+= reinforcement_layers.getIyGrossSection()+reinforcement_layers.getAreaGrossSection()*sqr(d);
    //   }
    return retval;
  }

//! @brief Inertia of the gross section about an axis parallel to z through its centroid
double XC::SectionGeometry::getIzGrossSection(void) const
  {
    double retval= 0.0;
    double d= 0.0;
    const double yCenterOfMass= getCenterOfMassGrossSection()[0];

    if(!regions.empty())
      {
        d= regions.getCenterOfMassGrossSection()[0]-yCenterOfMass;
        retval+= regions.getIzGrossSection()+regions.getAreaGrossSection()*sqr(d);
      }
    // if(!reinforcement_layers.empty())
    //   {
    //     d= reinforcement_layers.getCenterOfMassGrossSection()[0]-yCenterOfMass;
    //     retval+= reinforcement_layers.getIzGrossSection()+reinforcement_layers.getAreaGrossSection()*sqr(d);
    //   }
    return retval;
  }

//! @brief Product of inertia of the gross section about y and z axis through its centroid.
double XC::SectionGeometry::getPyzGrossSection(void) const
  {
    double retval= 0.0;
    double d2= 0.0;
    const Vector centerOfMassPosition= getCenterOfMassGrossSection();
    const double zCenterOfMass= centerOfMassPosition[1];
    const double yCenterOfMass= centerOfMassPosition[0];

    if(!regions.empty())
      {
        d2= (regions.getCenterOfMassGrossSection()[0]-yCenterOfMass)*(regions.getCenterOfMassGrossSection()[1]-zCenterOfMass);
        retval+= regions.getPyzGrossSection()+regions.getAreaGrossSection()*d2;
      }
    // if(!reinforcement_layers.empty())
    //   {
    //     d2= (reinforcement_layers.getCenterOfMassGrossSection()[0]-yCenterOfMass)*(reinforcement_layers.getCenterOfMassGrossSection()[1]-zCenterOfMass);
    //     retval+= reinforcement_layers.getPyzGrossSection()+reinforcement_layers.getAreaGrossSection()*d2;
    //   }
    return retval;
  }

//! @brief Return a reference to the object name.
const std::string &XC::SectionGeometry::Name(void) const
  {
    static std::string retval= "nil";
    if(material_handler)
      retval= material_handler->getSectionGeometryName(this);
    else
      {
	const NamedEntity *owr= dynamic_cast<const NamedEntity *>(this->Owner());
	if(owr)
	  retval= owr->getName();
	else
	  retval= "nil";
      }
    return retval;
  }

//! @brief Printing.
void XC::SectionGeometry::Print(std::ostream &s, int flag) const
  {
    s << "\nCurrent number of regions: "       << regions.size();
    s << "\nCurrent number of reinforcement layers: " << reinforcement_layers.size();
  }


std::ostream &XC::operator<<(std::ostream &s, XC::SectionGeometry &sectionRepr)
  {
    sectionRepr.Print(s);
    return s;
  }
