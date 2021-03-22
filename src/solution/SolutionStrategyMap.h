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
//SolutionStrategyMap.h

#ifndef MAPSOLUMETHOD_H
#define MAPSOLUMETHOD_H

#include "xc_utils/src/kernel/CommandEntity.h"
#include "utility/handler/DataOutputHandler.h"
#include "SolutionStrategy.h"

namespace XC {

class SolutionProcedureControl;
class ModelWrapper;

//!  @ingroup Solu
//! 
//! @brief Solution strategies container.
class SolutionStrategyMap: public CommandEntity
  {
  public:
    typedef std::map<std::string,SolutionStrategy> map_solu_method;
    typedef map_solu_method::iterator iterator;
    typedef map_solu_method::const_iterator const_iterator;

  private:
    map_solu_method solu_methods; //!< Solution strategies container.
  public:
    SolutionStrategyMap(SolutionProcedureControl *owr);

    inline iterator begin()
      { return solu_methods.begin(); }
    inline iterator end()
      { return solu_methods.end(); }
    inline const_iterator begin() const
      { return solu_methods.begin(); }
    inline const_iterator end() const
      { return solu_methods.end(); }

    inline bool empty(void) const
      { return solu_methods.empty(); }
    inline size_t size(void) const
      { return solu_methods.size(); }

    bool SolutionStrategyExists(const std::string &) const;
    SolutionStrategy &createSolutionStrategy(const std::string &,ModelWrapper *);
    const SolutionStrategy *getSolutionStrategy(const std::string &) const;
    SolutionStrategy *getSolutionStrategy(const std::string &);

    SolutionStrategy &newSolutionStrategy(const std::string &,const std::string &);

    void revertToStart(void);
    void clearAll(void);
  };

} // end of XC namespace

#endif
