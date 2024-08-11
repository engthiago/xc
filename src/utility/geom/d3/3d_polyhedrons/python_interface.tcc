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
//python_interface.cxx

class_<PolyhedronBase, bases<GeomObj3d>, boost::noncopyable >("PolyhedronBase", no_init)
  ;

class_<Polyhedron3d, bases<PolyhedronBase> >("Polyhedron3d")
  ;

class_<Tetrahedron3d, bases<PolyhedronBase> >("Polyhedron3d")
  ;

class_<Hexahedron3d, bases<Polyhedron3d> >("Hexahedron3d")
  .def(init<>())
  .def(init<Pos3d, Pos3d, Pos3d, Pos3d, Pos3d, Pos3d, Pos3d, Pos3d>())  
  .add_property("signedVolume", &Hexahedron3d::getVolumeWithSign, "returns object volume with sign.")
  .add_property("volume", &Hexahedron3d::getVolume, "returns object volume.")
  ;

