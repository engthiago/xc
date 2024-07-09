//----------------------------------------------------------------------------
//  xc utils library bilioteca de comandos para el intérprete del lenguaje
//  de entrada de datos.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  XC utils is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details. 
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//Polygon3d.cc

#include "Polygon3d.h"
#include "utility/geom/coo_sys/ref_sys/Ref2d3d.h"
#include "utility/geom/d1/Line2d.h"
#include "utility/geom/d1/Segment2d.h"
#include "utility/geom/d1/Segment3d.h"
#include "utility/geom/d1/Line3d.h"
#include "utility/geom/d3/GeomGroup3d.h"
#include "utility/geom/d3/HalfSpace3d.h"
#include "utility/geom/lists/auxiliary.h"


//! @brief Constructor.
Polygon3d::Polygon3d(const Pos3d &p1,const Pos3d &p2,const Pos3d &p3)
  : D2to3d(p1,p2,p3), plg2d()
  {
    push_back(p1);
    push_back(p2);
    push_back(p3);
  }

//! @brief Constructor.
Polygon3d::Polygon3d(const Ref2d3d &rf,const Polygon2d &p)
  : D2to3d(rf), plg2d(p) {}

//! @brief Constructor.
Polygon3d::Polygon3d(const Ref3d3d &rf,const Polygon2d &p)
  : D2to3d(rf), plg2d(p) {}

//! @brief Constructor.
Polygon3d::Polygon3d(const GeomObj::list_Pos3d &vertices)
  : D2to3d(), plg2d()
  {
    Polygon3d tmp(vertices.begin(), vertices.end());
    (*this)= tmp;
  }

//! @brief Constructor.
Polygon3d::Polygon3d(const boost::python::list &l)
  : D2to3d(), plg2d()
  {
    GeomObj::list_Pos3d vertices;
    const size_t sz= len(l);
    for(size_t i=0; i<sz; i++)
      vertices.push_back(boost::python::extract<Pos3d>(l[i]));

    (*this)= Polygon3d(vertices);
  }

//! @brief Return a Python list containing the positions
//! of the polygon vertices.
GeomObj::list_Pos3d Polygon3d::getVertexList(void) const
  {
    GeomObj::list_Pos3d retval;
    Polygon2d::vertex_iterator i= plg2d.vertices_begin();
    for(;i!=plg2d.vertices_end();i++)
      retval.push_back(to_3d(Pos2d(*i)));
    return retval;
  }

//! @brief Return a Python list containing the positions
//! of the polygon vertices.
boost::python::list Polygon3d::getVertexListPy(void) const
  {
    boost::python::list retval;
    GeomObj::list_Pos3d lst= getVertexList();
    GeomObj::list_Pos3d::const_iterator i= lst.begin();
    for(;i!=lst.end();i++)
      retval.append(*i);
    return retval;
  }

//! @brief Return the i-th edge (i between 1 and num_sides).
Segment3d Polygon3d::Lado(unsigned int i) const
  {
    Segment2d sg2d= plg2d.Lado(i);
    return to_3d(sg2d);
  }

//! @brief Return the i-th side (i between 0 and num_sides-1).
Segment3d Polygon3d::Lado0(unsigned int i) const
  {
    Segment2d sg2d= plg2d.Lado0(i);
    return to_3d(sg2d);
  }

//! @brief Return the index of the distal edge with respect to the point
//! argument.
//! @param p: 2D point.
int Polygon3d::getIndexOfDistalEdge(const Pos3d &p) const
  {
    const unsigned int nl= getNumEdges();
    int retval= 1;
    GEOM_FT d2= Lado0(1).dist2(p);
    GEOM_FT maxDist2= d2;
    for(unsigned int i= 2; i<=nl;i++)
      {
        d2= Lado0(i).dist2(p);
	if(d2>maxDist2)
	  {
	    retval= i;
	    maxDist2= d2;
	  }
      }
    return retval;
  }

//! @brief Return the proximal edge with respect to the point argument.
//! @param p: 2D point.
int Polygon3d::getIndexOfProximalEdge(const Pos3d &p) const
  {
    const unsigned int nl= getNumEdges();
    int retval= 1;
    GEOM_FT d2= Lado0(1).dist2(p);
    GEOM_FT minDist2= d2;
    for(unsigned int i= 2; i<=nl;i++)
      {
        d2= Lado0(i).dist2(p);
	if(d2<minDist2)
	  {
	    retval= i;
	    minDist2= d2;
	  }
      }
    return retval;
  }

//! @brief Return the distal vertex with respect to the point argument.
//! @param p: 2D point.
int Polygon3d::getIndexOfDistalVertex(const Pos3d &p) const
  {
    const unsigned int nl= getNumVertices();
    int retval= 1;
    GEOM_FT d2= Vertice0(1).dist2(p);
    GEOM_FT maxDist2= d2;
    for(unsigned int i= 2; i<=nl;i++)
      {
        d2= Vertice0(i).dist2(p);
	if(d2>maxDist2)
	  {
	    retval= i;
	    maxDist2= d2;
	  }
      }
    return retval;
  }
    
//! @brief Return the proximal vertex with respect to the point argument.
//! @param p: 2D point.
int Polygon3d::getIndexOfProximalVertex(const Pos3d &p) const
  {
    const unsigned int nl= getNumVertices();
    int retval= 1;
    GEOM_FT d2= Vertice0(1).dist2(p);
    GEOM_FT minDist2= d2;
    for(unsigned int i= 2; i<=nl;i++)
      {
        d2= Vertice0(i).dist2(p);
	if(d2<minDist2)
	  {
	    retval= i;
	    minDist2= d2;
	  }
      }
    return retval;
  }

Plane Polygon3d::getPlaneFromSide(unsigned int i) const
  {
    Segment3d lado= Lado(i);
    Vector3d v= -getPlane().Normal(); // interior points -> negative distance.
    return Plane(lado,v);
  }

Plane Polygon3d::getPlaneFromSide0(unsigned int i) const
  { return getPlaneFromSide(i+1); }

//! @brief Return true if the point lies inside the polygon
//! (i. e. the distance from the point to the polygon is less
//! than the tolerance argument).
bool Polygon3d::In(const Pos3d &p,const double &tol) const
  {
    bool retval= false;
    const Plane plane= getPlane();
    if(!plane.In(p,tol))
      { retval= false; }
    else
      {
	const Pos3d prj= plane.Projection(p);
        const Pos2d p2d(to_2d(prj));
        retval= plg2d.In(p2d,tol);
      }
    return retval;
  }

//! @brief Return the center of mass of the polygon.
Pos3d Polygon3d::getCenterOfMass(void) const
  {
    const Pos2d centroid= plg2d.getCenterOfMass();
    return to_3d(centroid);
  }

std::vector<Polygon3d> Polygon3d::getTributaryPolygons(void) const
  {
    const std::vector<Polygon2d> tmp= plg2d.getTributaryPolygons();
    const size_t sz= tmp.size();
    std::vector<Polygon3d> retval(sz);
    for(size_t i= 0;i<sz;i++)
      retval[i]= Polygon3d(getRef(),tmp[i]);
    return retval;
  }

//! @brief Moment of inertia with respect to an axis parallel to
//! the x axis passing through the center of mass.
GEOM_FT Polygon3d::Ix(void) const
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
	      << "; not implemented, 0 is returned." << std::endl;
    return 0;
  }

//! @brief Moment of inertia with respect to an axis parallel to
//! the y axis passing through the center of mass.
GEOM_FT Polygon3d::Iy(void) const
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
	      << "; not implemented, 0 is returned." << std::endl;
    return 0;
  }

//! @brief Moment of inertia with respect to an axis parallel to
//! the z axis passing through the center of mass.
GEOM_FT Polygon3d::Iz(void) const
  {
    std::cerr << getClassName() << "::" << __FUNCTION__
	      << "; not implemented, 0 is returned." << std::endl;
    return 0;
  }

GEOM_FT Polygon3d::GetMax(unsigned short int i) const
  {
    const GeomObj::list_Pos3d lv= getVertexList();
    return get_max_i(i,lv.begin(),lv.end());
  }
GEOM_FT Polygon3d::GetMin(unsigned short int i) const
  {
    const GeomObj::list_Pos3d lv= getVertexList();
    return get_min_i(i,lv.begin(),lv.end());
  }

void Polygon3d::Print(std::ostream &os) const
  {
    unsigned int nv= getNumVertices();
    if(nv<1) return;
    os << Vertice(1);
    for(unsigned int i= 2; i <= nv; i++)
      os << ", " << Vertice(i);
  }

//! @brief Return the squared distance from from point to polygon.
//!
//! The distance is computed as the maximum of:
//!  -The distance from the point to the plane that contains the polygon.
//!  -The signed distances from the point to each of the half spaces
//!  defined by the plane that contains the polygon.
//!
// If the vertices are in counterclockwise order, positive distances
// correspond to point AT THE SAME SIDE OF THE POLYGON with respect
// to the segment, otherwise the sign of the computed distance must
// be changed.
GEOM_FT Polygon3d::distSigno2(const Pos3d &p) const
  {
    const short int signo= (this->plg2d.clockwise() ? 1 : -1);
    const size_t nv= getNumVertices();
    const size_t nl= getNumEdges();
    if(nv==0) return NAN;
    if(nv==1) return p.dist2(Vertice(1));
     
    //Distance to the plane that contains the polygon.
    GEOM_FT d= getPlane().dist2(p);
    // Distance to the half spaces defined by the sides.
    for(unsigned int i=1; i<=nl; i++)
      {
        HalfSpace3d se3d(getPlaneFromSide(i));
        const GEOM_FT d_i= signo*se3d.distSigno2(p);
        d= std::max(d,d_i);
      }
    return d;
  }

GEOM_FT Polygon3d::distSigno(const Pos3d &p) const
  { return sqrt_FT(::Abs(distSigno2(p))); }

//! @brief Return the distance from point to polygon.
//! Distance is computed as the maximum of the distances
//! (signed) from the point to each of the planes
//! that contain a side and are normal to the plane 
//! that contains the polygon.
//! If the point is inside the polygon 0 is returned.
GEOM_FT Polygon3d::dist2(const Pos3d &p) const
  {
    const GEOM_FT retval= distSigno2(p);
    return (retval >= 0 ? retval : 0);
  }


//! @brief Return the distance from point to polygon.
//! This distance is computed as the maximum of the signed distances from the
//! point to each of the planes that contain a side and are perpendicular to the
//! plane that contains the polygon.
//! In the point is inside the polygon 0 is returned.
GEOM_FT Polygon3d::dist(const Pos3d &p) const
  {
    const GEOM_FT retval= distSigno(p);
    return (retval >= 0 ? retval : 0);
  }

//! @brief Return the intersection of the polygon with the line.
Segment3d Polygon3d::Clip(const Line3d &r) const
  {
    Segment3d retval;
    const GEOM_FT angle= getPlane().getAngle(r);
    const GEOM_FT angleTol= 1/180.0*M_PI; // one degree
    if(angle>angleTol)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; line: " << r
	  	  << " is not contained in the polygon plane."
	          << std::endl;
	retval.setExists(false);
      }
    else
      {
	Line2d l2d= to_2d(r);
	Segment2d s2d= plg2d.Clip(l2d);
	retval= to_3d(s2d);
      }	
    return retval;
  }

//! @brief Return the intersection of the polygon and the ray.
Segment3d Polygon3d::Clip(const Ray3d &sr) const
  {
    Segment3d retval;
    const GEOM_FT angle= getPlane().getAngle(sr);
    const GEOM_FT angleTol= 1/180.0*M_PI; // one degree
    if(angle>angleTol)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; ray: " << sr
	  	  << " is not contained in the polygon plane."
	          << std::endl;
	retval.setExists(false);
      }
    else
      {
	Ray2d r2d= to_2d(sr);
	Segment2d s2d= plg2d.Clip(r2d);
	retval= to_3d(s2d);
      }	
    return retval;
  }

//! @brief Return the intersection of the polygon and the segment.
Segment3d Polygon3d::Clip(const Segment3d &sg) const
  {
    Segment3d retval;
    const GEOM_FT angle= getPlane().getAngle(sg);
    const GEOM_FT angleTol= 1/180.0*M_PI; // one degree
    if(angle>angleTol)
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; segment: " << sg
	  	  << " is not contained in the polygon plane."
	          << std::endl;
	retval.setExists(false);
      }
    else
      {
	Segment2d sg2d= to_2d(sg);
        Segment2d s2d= plg2d.Clip(sg2d);
	if(s2d.exists())
	  { retval= to_3d(s2d); }
	else
	  retval.setExists(false);
      }	
    return retval;
  }

//! @brief Return true if the line intersects the polygon.
bool Polygon3d::intersects(const Line3d &r) const
  {
    const Line2d r2d= to_2d(r);
    return plg2d.intersects(r2d);
  }

//! @brief Return the intersection with the line argument.
Segment3d Polygon3d::getIntersection(const Line3d &r) const
  {
    const Line2d r2d= to_2d(r);
    const Segment2d sg2d= plg2d.getIntersection(r2d);
    return to_3d(sg2d);
  }

//! @brief Return true if the plane intersects the polygon.
bool Polygon3d::intersects(const Plane &p) const
  { return p.intersects(*this); }

//! @brief Return the intersection with the plane argument.
Segment3d Polygon3d::getIntersection(const Plane &p) const
  { return p.getIntersection(*this); }

//! @brief Return the polygons that result form cutting the polygon
//! with the argument plane.
std::list<Polygon3d> Polygon3d::Corta(const Plane &pl) const
  {
    std::list<Polygon3d> retval;
    const Plane polygonPlane= getPlane();
    if(polygonPlane==pl) return retval;
    if(parallel(polygonPlane,pl)) return retval;

    GeomGroup3d gint= intersection(polygonPlane,pl);
    GeomObj3d *ptr=(*gint.begin());
    const Line3d r3d= *((Line3d *)ptr);
    const Line2d r2d= to_2d(r3d);

    std::list<Polygon2d> inter= corta(plg2d,r2d);
    for(std::list<Polygon2d>::const_iterator i= inter.begin(); i!=inter.end();i++)
      retval.push_back(Polygon3d(getRef(),*i));

    return retval;
  }

//! @brief Return true if alguno de los vertices toca el cuadrante
//! que se pasa como parámetro.
bool Polygon3d::TocaCuadrante(const int &cuadrante) const
  {
    Polygon2d::vertex_iterator i= plg2d.vertices_begin();
    for(;i!=plg2d.vertices_end();i++)
      if(to_3d(Pos2d(*i)).Cuadrante() == cuadrante)
        return true;
    return false;
  }

//! @brief Return true if the point list is oriented clockwise.
//! @param vPoint: position of the point of view.
bool Polygon3d::clockwise(const Pos3d &vPoint) const
  { return !counterclockwise(vPoint); }

//! @brief Return true if the point list is oriented counterclockwise.
//! @param vPoint: position of the point of view.
bool Polygon3d::counterclockwise(const Pos3d &vPoint) const
  {
    bool retval= plg2d.counterclockwise();
    const Pos3d O= getCenterOfMass();
    const Vector3d K= getKVector();
    const Vector3d vDir= vPoint-O;
    const GEOM_FT dot_product= K.GetDot(vDir);
    if(dot_product<0)
      retval= !retval;
    else
      if(dot_product<1e-6)
	{
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; the point of view: " << vPoint
		    << " is almost contained in the reference plane."
		    << std::endl;
	}
    return retval;
  }

void Polygon3d::swap(void)
  { plg2d.swap(); }

//! @brief Return a polygon parallel to this one at the given distance.
//! The new polygon will be exterior if the distance is positive.
Polygon3d Polygon3d::offset(const GEOM_FT &d) const
  {
    Polygon2d tmp= this->plg2d.offset(d);
    return Polygon3d(this->getRef(), tmp);
  }

//! @brief Returns the buffer (a polygon being the spatial point set
//! collection within a specified maximum distance from this polygon) of this
//! polygon. 
Polygon3d Polygon3d::buffer(const GEOM_FT &buffer_distance) const
  {
    Polygon2d tmp= this->plg2d.buffer(buffer_distance);
    return Polygon3d(this->getRef(), tmp);
  }

//! @brief Return the projection of the given point into the polyline.
//! @param p: point to be projected.
Pos3d Polygon3d::Projection(const Pos3d &p) const
  {
    const Plane plane= getPlane();
    const Pos3d prj= plane.Projection(p);
    const Pos2d p2d(to_2d(prj));
    const Pos2d prj2d= this->plg2d.Projection(p2d);
    const Pos3d retval= to_3d(prj2d);
    return retval;
  }

//! @brief Return the length along the perimeter upto the given point.
GEOM_FT Polygon3d::getLengthUpTo(const Pos3d &p) const
  {
    const Plane plane= getPlane();
    const Pos3d prj= plane.Projection(p);
    const Pos2d p2d(to_2d(prj));
    const GEOM_FT retval= this->plg2d.getLengthUpTo(p2d);
    return retval;
  }

//! @brief Return the orientation of the polygon (clockwise or
//! counterclockwise).
std::string Polygon3d::orientation(const Pos3d &vPoint) const
  {
    std::string retval= "counterclockwise";
    if(clockwise(vPoint))
      { retval= "clockwise"; }
    return retval;
  }
