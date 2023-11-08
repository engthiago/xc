//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
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
//python_interface.tcc

const XC::Vector &(XC::DeformationPlane::*getStrains)(void) const= &XC::DeformationPlane::getDeformation;
class_<XC::DeformationPlane, bases<Plane> >("DeformationPlane")
  .def(init<const double&>())
  .def(init<Plane>())
  .def(init<Pos2d,double,Pos2d,double,Pos2d,double>())
  .def(init<Pos3d,Pos3d,Pos3d>())
  .def(init<const XC::Vector &>())
  .def("constantStrain",&XC::DeformationPlane::ConstantStrain)
  .def("getDeformation",&XC::DeformationPlane::Strain,"DEPRECATED; returns strain at position.")
  .def("getStrains", make_function(getStrains,return_value_policy<copy_const_reference>()), "Returns the generalized strains vector (epsilon, curvZ, curvY)")
  .def("setStrains",&XC::DeformationPlane::setDeformation,"Sets the generalized strains vector (epsilon, curvZ, curvY)")
  .add_property("strains", make_function(getStrains,return_value_policy<copy_const_reference>()), &XC::DeformationPlane::setDeformation, "Gets/sets the generalized strains vector (epsilon, curvZ, curvY)")
  .def("getStrain",&XC::DeformationPlane::Strain,"Returns strain at the given position.")
  .def("getGeneralizedStrains",&XC::DeformationPlane::getGeneralizedStrainsPy, "return the generalized strains in the order specified by the given list of strings")
  ;

class_<XC::ComputePivots, bases<Ref3d3d>, boost::noncopyable >("ComputePivots", no_init)
  ;

class_<XC::PivotsUltimateStrains, bases<CommandEntity>, boost::noncopyable >("PivotsUltimateStrains", no_init)
  .add_property("ultimateStrainAPivot",make_function(&XC::PivotsUltimateStrains::getUltimateStrainAPivot,return_value_policy<copy_const_reference>()),&XC::PivotsUltimateStrains::setUltimateStrainAPivot)
  .add_property("ultimateStrainBPivot",make_function(&XC::PivotsUltimateStrains::getUltimateStrainBPivot,return_value_policy<copy_const_reference>()),&XC::PivotsUltimateStrains::setUltimateStrainBPivot)
  .add_property("ultimateStrainCPivot",make_function(&XC::PivotsUltimateStrains::getUltimateStrainCPivot,return_value_policy<copy_const_reference>()),&XC::PivotsUltimateStrains::setUltimateStrainCPivot)
  ;

class_<XC::InteractionDiagramData, bases<CommandEntity>>("InteractionDiagramParameters")
  .add_property("threshold",make_function(&XC::InteractionDiagramData::getThreshold,return_value_policy<copy_const_reference>()),&XC::InteractionDiagramData::setThreshold)
  .add_property("incEps",make_function(&XC::InteractionDiagramData::getIncEps,return_value_policy<copy_const_reference>()),&XC::InteractionDiagramData::setIncEps)
  .add_property("incTheta",make_function(&XC::InteractionDiagramData::getIncTheta,return_value_policy<copy_const_reference>()),&XC::InteractionDiagramData::setIncTheta)
  .add_property("pivotsUltimateStrains",make_function(&XC::InteractionDiagramData::getPivotsUltimateStrains,return_internal_reference<>()),&XC::InteractionDiagramData::setPivotsUltimateStrains)
  .add_property("concreteSetName",make_function(&XC::InteractionDiagramData::getConcreteSetName,return_internal_reference<>()),&XC::InteractionDiagramData::setConcreteSetName)
  .add_property("concreteTag",make_function(&XC::InteractionDiagramData::getConcreteTag,return_value_policy<copy_const_reference>()),&XC::InteractionDiagramData::setConcreteTag)
  .add_property("rebarSetName",make_function(&XC::InteractionDiagramData::getRebarSetName,return_internal_reference<>()),&XC::InteractionDiagramData::setRebarSetName)
  .add_property("reinforcementTag",make_function(&XC::InteractionDiagramData::getReinforcementTag,return_value_policy<copy_const_reference>()),&XC::InteractionDiagramData::setReinforcementTag)
  ;

class_<XC::ClosedTriangleMesh, bases<GeomObj3d>, boost::noncopyable >("ClosedTriangleMesh", no_init)
  .def("getMax",&XC::ClosedTriangleMesh::GetMax)
  .def("getMin",&XC::ClosedTriangleMesh::GetMin)
  .def("getArea",&XC::ClosedTriangleMesh::getArea)
  .def("getVolume",&XC::ClosedTriangleMesh::getVolume)
  .def("getIx",&XC::ClosedTriangleMesh::Ix)
  .def("getIy",&XC::ClosedTriangleMesh::Iy)
  .def("getIz",&XC::ClosedTriangleMesh::Iz)
  .def("getDimension",&XC::ClosedTriangleMesh::Dimension)
  .def("getNumFacetas",&XC::ClosedTriangleMesh::GetNumFacetas)
  .def("getTriangleMap",&XC::ClosedTriangleMesh::getTriangleMap)
  ;

double (XC::InteractionDiagram::*getCF)(const Pos3d &esf_d) const= &XC::InteractionDiagram::getCapacityFactor;
class_<XC::InteractionDiagram, bases<XC::ClosedTriangleMesh>, boost::noncopyable >("InteractionDiagram", no_init)
  .def("centroid",&XC::InteractionDiagram::getCenterOfMass)
  .def("getLength",&XC::InteractionDiagram::getLength)
  .def("getIntersection",&XC::InteractionDiagram::getIntersection,"Returns the intersection of the ray O->point(N,My,Mz) with the interaction diagram.")
  .def("getCapacityFactor",getCF)
  .def("writeTo",&XC::InteractionDiagram::writeTo)
  .def("readFrom",&XC::InteractionDiagram::readFrom)
  ;

double (XC::InteractionDiagram2d::*getCF2d)(const Pos2d &esf_d) const= &XC::InteractionDiagram2d::getCapacityFactor;
class_<XC::InteractionDiagram2d, bases<Polygon2d>, boost::noncopyable >("InteractionDiagram2d", no_init)
  .def("getIntersection",&XC::InteractionDiagram2d::getIntersection,"Returns the intersection of the ray O->point(N,My,Mz) with the interaction diagram.")
  .def("getCapacityFactor",getCF2d)
  .def("simplify",&XC::InteractionDiagram2d::Simplify)
  ;
