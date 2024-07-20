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
//MapLoadPatterns.h

#ifndef MAPLOADPATTERNS_H
#define MAPLOADPATTERNS_H

#include "preprocessor/prep_handlers/LoadHandlerMember.h"
#include "domain/load/pattern/LoadPattern.h"
#include "domain/load/pattern/TimeSeries.h"
#include <map>

namespace XC {
class TimeSeries;
class LoadHandler;
class Domain;

//! @ingroup LPatterns
//
//! @brief Load pattern container.
class MapLoadPatterns: public LoadHandlerMember
  {
    typedef std::map<std::string,TimeSeries *> map_timeseries;
    map_timeseries tseries; //!< Load/displacement time variation.
    typedef map_timeseries::iterator time_series_iterator;
    typedef map_timeseries::const_iterator time_series_const_iterator;
    
    std::string time_series_name; //!< Time series identifier for new load patterns.

    typedef std::map<std::string,LoadPattern *> map_loadpatterns; //!< Load pattern container type.
    map_loadpatterns loadpatterns; //!< Load pattern container.
    std::string lpcode; //!< Load pattern identifier for new loads.
    int tag_el; //!< Default tag for new elemental load.
    int tag_nl; //!< Default tag for new nodal load.
    int tag_spc; //!< Default tag for new imposed node displacement.

    template <class TS>
    TimeSeries *create_time_series(const std::string &);
    template <class LP>
    LoadPattern *create_load_pattern(const std::string &);
  public:
    typedef map_loadpatterns::iterator iterator;
    typedef map_loadpatterns::const_iterator const_iterator;
  protected:
    friend class LoadHandler;
    void clear_time_series(void);

    DbTagData &getDbTagData(void) const;
    int sendData(Communicator &comm);
    int recvData(const Communicator &comm);
  public:
    MapLoadPatterns(LoadHandler *owr);
    ~MapLoadPatterns(void);

    void clear(void);

    const_iterator begin(void) const
      { return loadpatterns.begin(); }
    const_iterator end(void) const
      { return loadpatterns.end(); }
    iterator begin(void)
      { return loadpatterns.begin(); }
    iterator end(void)
      { return loadpatterns.end(); }
    size_t size(void) const
      { return loadpatterns.size(); }
    bool empty(void) const
      { return loadpatterns.empty(); }

    const std::string getCurrentLoadPatternId(void) const;
    LoadPattern *getCurrentLoadPatternPtr(void);
    const LoadPattern *getCurrentLoadPatternPtr(void) const;
    inline const std::string &getCurrentLoadPattern(void) const
      { return lpcode; }
    inline void setCurrentLoadPattern(const std::string &nmb)
      { lpcode= nmb; }
    LoadPattern *newLoadPattern(const std::string &,const std::string &);
    bool removeLoadPattern(const std::string &);
    void addToDomain(const std::string &);
    void removeFromDomain(const std::string &);
    void removeAllFromDomain(void);

    TimeSeries *findTS(const int &);
    const TimeSeries *findTS(const int &) const;
    TimeSeries *findTS(const std::string &);
    const TimeSeries *findTS(const std::string &) const;
    LoadPattern *findLoadPattern(const std::string &);
    const LoadPattern *findLoadPattern(const std::string &) const;
    LoadPattern *findLoadPattern(const int &);
    const LoadPattern *findLoadPattern(const int &) const;
    const std::string &getLoadPatternName(const LoadPattern *) const;
    TimeSeries *newTimeSeries(const std::string &,const std::string &);
    inline const std::string &getCurrentTimeSeries(void) const
      { return time_series_name; }
    inline void setCurrentTimeSeries(const std::string &nmb)
      { time_series_name= nmb; }
    const std::string &getTimeSeriesName(const TimeSeries *) const;

    std::deque<std::string> getNamesList(void) const;
    boost::python::list getKeys(void) const;

    inline const int &getCurrentElementLoadTag(void) const
      { return tag_el; }
    inline void setCurrentElementLoadTag(const int &n)
      { tag_el= n; }
    inline const int &getCurrentNodeLoadTag(void) const
      { return tag_nl; }
    inline void setCurrentNodeLoadTag(const int &n)
      { tag_nl= n; }
    
    std::list<LoadPattern *> getLoadPatternsActingOn(const Node *);
    boost::python::list getLoadPatternsActingOnPy(const Node *);
    void removeLoadsOn(const Node *);
    void copyLoads(const Node *, const Node *);
    std::list<LoadPattern *> getLoadPatternsActingOn(const Element *);
    boost::python::list getLoadPatternsActingOnPy(const Element *);
    void removeLoadsOn(const Element *);
    void copyLoads(const Element *, const Element *);

    int sendSelf(Communicator &);
    int recvSelf(const Communicator &);
    boost::python::dict getPyDict(void) const;
    void setPyDict(const boost::python::dict &);
  };

//! @brief Create a TimeSeries object.
template <class TS>
TimeSeries *XC::MapLoadPatterns::create_time_series(const std::string &cod_ts)
  {
    TimeSeries *ts= findTS(cod_ts);
    if(!ts) //Doesn't exist.
      {
        TS *nts= new TS();
        assert(nts);
        tseries[cod_ts]= nts;
        ts= nts;
      }
    ts->set_owner(this);
    time_series_name= cod_ts;
    return ts;
  }

//! @brief Creates a new load pattern.
template <class LP>
LoadPattern *XC::MapLoadPatterns::create_load_pattern(const std::string &cod_lp)
  {
    int &tag_lp= this->getTagLP();
    LoadPattern *lp= findLoadPattern(cod_lp);
    if(!lp) //Doesn't exist.
      {
	std::map<std::string,TimeSeries *>::const_iterator its= tseries.find(time_series_name);
        if(its!= tseries.end())
          {
            lp= new LP(tag_lp);
            tag_lp++;
            if(lp)
              {
                lp->setTimeSeries(its->second);
                lp->set_owner(this);
                loadpatterns[cod_lp]= lp;
                //If there is the only we make it the default case.
                if(loadpatterns.empty())
                  lpcode= cod_lp;
              }
          }
        else
	  std::cerr << "MapLoadPatterns; ERROR " 
                    << ", time series: " << time_series_name
		    << " not found." << std::endl;
      }
    return lp;
  }



} // end of XC namespace

#endif
