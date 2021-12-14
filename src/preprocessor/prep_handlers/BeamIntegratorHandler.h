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
//BeamIntegratorHandler.h

#ifndef BEAMINTEGRATORLOADER_H
#define BEAMINTEGRATORLOADER_H

#include "PrepHandler.h"
#include <map>

namespace XC {
class BeamIntegration;

//! @ingroup Ldrs
//! 
//! @brief Beam integrators handler.
class BeamIntegratorHandler: public PrepHandler
  {
  public:
    typedef std::map<std::string,BeamIntegration *> map_beam_integrators;
    typedef map_beam_integrators::const_iterator const_iterator;
    typedef map_beam_integrators::iterator iterator;
  private:
    map_beam_integrators beam_integrators;
    void free_mem(void);
    BeamIntegratorHandler(const BeamIntegratorHandler &);
    BeamIntegratorHandler &operator=(const BeamIntegratorHandler &);
  protected:
    friend class ElementHandler;
  public:
    BeamIntegratorHandler(Preprocessor *owr);
    const map_beam_integrators &Map(void) const;
    const_iterator begin(void) const;
    const_iterator end(void) const;
    iterator begin(void);
    iterator end(void);
    const_iterator find(const std::string &str) const;
    iterator find(const std::string &str);
    BeamIntegration *find_ptr(const std::string &str);
    const BeamIntegration *find_ptr(const std::string &nmb) const;

    inline size_t size(void) const
      { return beam_integrators.size(); }

    bool exists(const std::string &nmb) const;
    BeamIntegration *newBI(const std::string &,const std::string &);
    BeamIntegration &get(const std::string &);

    ~BeamIntegratorHandler(void);
    inline void clearAll(void)
      { free_mem(); }
  };

} // end of XC namespace

#endif
