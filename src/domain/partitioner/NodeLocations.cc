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
//NodeLocations.cpp

#include "NodeLocations.h"

XC::NodeLocations::NodeLocations(int tag)
:TaggedObject(tag), numPartitions(0)
  {}

void XC::NodeLocations::Print(std::ostream &s, int flag) const
  {
    s << "NodeLocations tag: " << getTag() << " partitions: ";
    for(std::set<int>::const_iterator i= nodePartitions.begin();i!= nodePartitions.end();i++)
      s << " " << *i;
  }

int XC::NodeLocations::addPartition(int partition)
  {
    nodePartitions.insert(partition);
    numPartitions= nodePartitions.size();
    return 0;
  }
