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
//MapSet.cc

#include "MapSet.h"
#include "Set.h"
#include "SetEstruct.h"
#include "preprocessor/multi_block_topology/entities/EntMdlr.h"
#include "preprocessor/Preprocessor.h"


#include "utility/actor/actor/MovableID.h"
#include "domain/domain/Domain.h"
#include "domain/mesh/element/Element.h"
#include "domain/mesh/node/Node.h"

XC::ID XC::MapSet::setsDbTags;

//! @brief Returns a pointer to the set which name is being passed as parameter.
XC::SetBase *XC::MapSet::find_set(const std::string &name)
  {
    XC::SetBase *retval= MapSetBase::find(name);
    if(!retval)
      {
    	map_ent_mdlr::const_iterator j= entities.find(name);
        if(j != entities.end())
          retval= (*j).second;
        else
          retval= nullptr;
      }
    return retval;
  }

//! @brief Returns a pointer to the set which name is being passed as parameter.
const XC::SetBase *XC::MapSet::find_set(const std::string &name) const
  {
    MapSet *this_no_const= const_cast<MapSet *>(this);
    return this_no_const->find_set(name);
  }

//! @brief Return the set which name is being passed as parameter.
XC::SetBase &XC::MapSet::getSet(const std::string &name)
  {
    SetBase *retval= find_set(name);
    if(!retval)
      {
	std::cerr << getClassName() << "::" << __FUNCTION__
	          << "; set named '"
                  << name << "' not found. Total set returned." << std::endl;
        retval= total;
      }
    return *retval;
  }


//! @brief Creates a new set with the name being passed as parameter.
XC::Set *XC::MapSet::create_set(const std::string &name)
  {
    Set *tmp =nullptr;
    if(!exists(name)) //Set is new.
      {
        tmp= new Set(name,getPreprocessor());
	MapSetBase::operator[](name)= tmp;
      }
    else //Set already exists
      tmp= dynamic_cast<Set *>(find_set(name));
    return tmp;
  }

//! @brief Allocates a copy of the set argument.
XC::Set *XC::MapSet::alloc_set(const Set &s)
  {
    const std::string &name= s.getName();
    Set *set_ptr =nullptr;
    if(exists(name))
      removeSet(name);
    set_ptr= new Set(s);
    insert({name, set_ptr});
    return set_ptr;
  }

bool XC::MapSet::is_open(const std::string &name) const
  {
    map_sets::const_iterator i= open_sets.find(name);
    return (i!=open_sets.end());
  }

//! @brief Open the set.
void XC::MapSet::abre_set(const std::string &name)
  {
    SetBase *tmp =nullptr;
    if(!exists(name)) //Set doesn't exists.
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; the set: '"
                << name << "' doesn't exists.\n";
    else //The set exists
      {
        tmp= find_set(name);
        assert(tmp);
        open_sets[name]= tmp;
      }
  }

//! @brief Close set.
void XC::MapSet::cierra_set(const std::string &name)
  {
    if(!exists(name)) //Set doesn't exists.
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; the set: '"
                << name << "' doesn't exists.\n";
    else //The set exists
      {
	iterator i= open_sets.find(name);
        if(i!= open_sets.end())
          open_sets.erase(i);
        else
          if(verbosity>1)
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; the set: '"
                      << name << "' is already closed.\n";
      }
  }

//! @bref Creates a copy of the structured set being passed as parameter.
XC::SetBase *XC::MapSet::broke_set(const std::string &name,const std::string &class_name)
  {
    SetBase *retval= nullptr;
    if(exists(name)) //The set exists
      retval= find_set(name);
    else //the set is new.
      {
        if(class_name == "XC::Set")
          retval= new Set(name,getPreprocessor());
//         else if(class_name == "XC::SetEstruct")
//           retval= new SetEstruct(name,getPreprocessor());
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
	            << "; class name: '"
                    << class_name << "' unknown." << std::endl;
      }
    return retval;
  }

//! @bref Inserts, as a set, a preprocessor entity.
XC::EntMdlr *XC::MapSet::insert_ent_mdlr(EntMdlr *ent_mdlr)
  {
    assert(ent_mdlr);
    const std::string name= ent_mdlr->getName();
    if(exists(name)) //The set exists
      {
	std::cerr << getClassName() << "::" << __FUNCTION__
		  << "the set: " << name
		  << " already exists. New set not inserted." << std::endl;
        return nullptr;
      }
    else //the set is new.
      {
        entities[name]= ent_mdlr;
        return ent_mdlr;
      }
  }

//! @brief Returns a pointer to the set estructurado which name is being passed as parameter.
XC::SetEstruct *XC::MapSet::find_struct_set(const std::string &name)
  {
    SetBase *set= find_set(name);
    SetEstruct *retval= nullptr;
    if(set)
      retval= dynamic_cast<SetEstruct *>(set);
    return retval;
  }

//! @brief Initialize total set.
void XC::MapSet::setup_total(void)
  {
    const std::string str_tot= "total";
    total= create_set(str_tot);
  }

//! @brief Default constructor.
XC::MapSet::MapSet(Preprocessor *prep)
  : PreprocessorContainer(prep), MovableObject(0), total(nullptr)
  { setup_total(); }

//! @brief Copy constructor (DOESN'T COPY SETS).
XC::MapSet::MapSet(const MapSet &other)
  : PreprocessorContainer(other), MovableObject(other),
    MapSetBase(other), total(nullptr)
  { setup_total(); }

//! @brief Assignment operator (DOESN'T COPY SETS).
XC::MapSet &XC::MapSet::operator=(const MapSet &other)
  {
    PreprocessorContainer::operator=(other);
    MovableObject::operator=(other);
    MapSetBase::operator=(other);
    setup_total();
    return *this;
  }

//! @brief Creates a new set with the name which is passed as a parameter.
XC::Set *XC::MapSet::defSet(const std::string &name)
  { return create_set(name); }

//! @brief Deletes the set and removes it from the sets map.
void XC::MapSet::removeSet(const std::string &name)
  {
    if(is_open(name))
      {
	std::clog << getClassName() << "::" << __FUNCTION__
	          << "; warning! removing open set: '"
	          << name << "'\n";
        cierra_set(name);
      }
    MapSetBase::remove(name);
  }

//! @brief Clears all defined sets.
void XC::MapSet::clearSets(void)
  {
    MapSetBase::clearSets();
    total= nullptr;
  }

//! @brief Clears all.
void XC::MapSet::clearAll(void)
  {
    total= nullptr;
    entities.clear();
    clearSets();
  }

//! @brief Clears all.
void XC::MapSet::reset(void)
  {
    clearAll();
    setup_total();
  }

//! @brief Return the sets that contain the pointer to the node whose
//! identifier is being passed as parameter.
std::set<XC::SetBase *> XC::MapSet::get_sets_containing_node(const int &nodeTag)
  {
    std::set<SetBase *> retval;
    const Preprocessor *prep= getPreprocessor();
    if(prep)
      {
        const Domain *dom= prep->getDomain();
	const Node *n= dom->getNode(nodeTag);
	if(n)
	  retval= get_sets(n);
      }
    return retval;
  }

//! @brief Return the a Python list with the sets that contain the pointer
//! to the node whose identifier is being passed as parameter.
boost::python::list XC::MapSet::get_sets_containing_node_py(const int &nodeTag)
  {
    boost::python::list retval;
    const Preprocessor *prep= getPreprocessor();
    if(prep)
      {
        const Domain *dom= prep->getDomain();
	const Node *n= dom->getNode(nodeTag);
	if(n)
	  retval= get_sets_py(n);
      }
    return retval;
  }

//! @brief Return the sets that contain the pointer to the element whose
//! identifier is being passed as parameter.  
std::set<XC::SetBase *> XC::MapSet::get_sets_containing_element(const int &elementTag)
  {
    std::set<SetBase *> retval;
    const Preprocessor *prep= getPreprocessor();
    if(prep)
      {
        const Domain *dom= prep->getDomain();
	const Element *e= dom->getElement(elementTag);
	if(e)
	  retval= get_sets(e);
      }
    return retval;
  }

//! @brief Return the a Python list with the sets that contain the pointer
//! to the element whose identifier is being passed as parameter.
boost::python::list XC::MapSet::get_sets_containing_element_py(const int &elementTag)
  {
    boost::python::list retval;
    const Preprocessor *prep= getPreprocessor();
    if(prep)
      {
        const Domain *dom= prep->getDomain();
	const Element *e= dom->getElement(elementTag);
	if(e)
	  retval= get_sets_py(e);
      }
    return retval;
  }

//! @brief Destructor.
XC::MapSet::~MapSet(void)
  { clearAll(); }

//! @brief Returns a vector to store the dbTags
//! of the class members.
XC::DbTagData &XC::MapSet::getDbTagData(void) const
  {
    static DbTagData retval(6);
    return retval;
  }

//! @brief Send the dbTags of the sets through the communicator argument.
int XC::MapSet::sendSetsDbTags(int posDbTag,Communicator &comm)
  {
    const int size= MapSetBase::size();
    int res= 0;
    if(size>0)
      {
        setsDbTags= getSetsDBTags(comm);
        res+= comm.sendID(setsDbTags,getDbTagData(),CommMetaData(posDbTag));
      }
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; ch failed to send the IDs.\n";
    return res;
  }

//! @brief Send the names of the classes of the sets through
//! the communicator argument.
int XC::MapSet::sendSetsClassNames(int posDbTag,Communicator &comm)
  {
    const int size= MapSetBase::size();
    int res= 0;
    if(size>0)
      {
        setsClassNames= getSetsClassNames();
        res+= comm.sendStrings(setsClassNames,getDbTagData(),CommMetaData(posDbTag));
      }
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; ch failed to send the IDs.\n";
    return res;
  }

//! @brief Receives the dbTags of the sets through the communicator argument.
int XC::MapSet::receiveSetsDbTags(int posDbTag,int size,const Communicator &comm)
  {
    setsDbTags.resize(size);
    int res= 0;
    if(size>0)
      res= comm.receiveID(setsDbTags,getDbTagData(),CommMetaData(posDbTag));
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; ch failed to receive the IDs.\n";
    return res;
  }

//! @brief Receives the class names of the sets through the communicator argument.
int XC::MapSet::receiveSetsClassNames(int posDbTag,int size,const Communicator &comm)
  {
    setsClassNames.resize(size);
    int res= 0;
    if(size>0)
      res= comm.receiveStrings(setsClassNames,getDbTagData(),CommMetaData(posDbTag));
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; ch failed to receive the IDs.\n";
    return res;
  }

//! @brief Send the sets through the communicator argument.
int XC::MapSet::sendSets(int posDbTag1, int posDbTag2, int posDbTag3,Communicator &comm)
  {
    std::deque<std::string> names;
    for(const_iterator i= begin();i!=end();i++)
      names.push_back((*i).first);
    DbTagData &dt= getDbTagData();
    int res= comm.sendStrings(names,dt,CommMetaData(posDbTag1));
    res+= sendSetsDbTags(posDbTag2,comm);
    res+= sendSetsClassNames(posDbTag3,comm);
    int loc= 0;
    DbTagData tags(setsDbTags);
    for(iterator i= begin();i!=end();i++,loc++)
      res+= comm.sendMovable(*(*i).second,tags,CommMetaData(loc));
    return res;
  }

//! @brief Receives the sets through the communicator argument.
int XC::MapSet::receiveSets(int posDbTag1, int posDbTag2, int posDbTag3,const int &sz,const Communicator &comm)
  {
    std::deque<std::string> names;
    int res= comm.receiveStrings(names,getDbTagData(),CommMetaData(posDbTag1));
    res+= receiveSetsDbTags(posDbTag2,sz,comm);
    res+= receiveSetsClassNames(posDbTag3,sz,comm);
    int loc= 0;
    DbTagData tags(setsDbTags);
    for(std::deque<std::string>::iterator i= names.begin();i!=names.end();i++,loc++)
      {
        SetBase *tmp= broke_set(*i,setsClassNames[loc]);
        if(tmp)
          comm.receiveMovable(*tmp,tags,CommMetaData(loc));
        else
	  std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; error receiving set: '" << *i << "'.\n";
      }
    return res;
  }
//! @brief Send members through the communicator argument.
int XC::MapSet::sendOpenSets(int posDbTag1, int posDbTag2,Communicator &comm)
  {
    const size_t sz= open_sets.size();
    int res= comm.sendInt(sz,getDbTagData(),CommMetaData(posDbTag1));
    if(!open_sets.empty())
      {
        std::deque<std::string> open_sets_names;
        for(const_iterator i= open_sets.begin();i!=open_sets.end();i++)
          open_sets_names.push_back((*i).first);
        res+= comm.sendStrings(open_sets_names,getDbTagData(),CommMetaData(posDbTag2));
      }
    return res;
  }
//! @brief Receives members through the communicator argument.
int XC::MapSet::receiveOpenSets(int posDbTag1, int posDbTag2,const Communicator &comm)
  {
     int sz_open_sets= 0;
     int res= comm.receiveInt(sz_open_sets,getDbTagData(),CommMetaData(posDbTag1));
     if(sz_open_sets>0)
       {
         std::deque<std::string> open_sets_names;
         res+= comm.receiveStrings(open_sets_names,getDbTagData(),CommMetaData(posDbTag2));
         for(std::deque<std::string>::const_iterator i= open_sets_names.begin();i!=open_sets_names.end();i++)
           abre_set(*i);
       }
    return res;
  }

//! @brief Send members through the communicator argument.
int XC::MapSet::sendData(Communicator &comm)
  {
    const size_t sz= MapSetBase::size();
    int res= comm.sendInt(sz,getDbTagData(),CommMetaData(0)); //number of sets
    if(sz>0)
      {
        res+= sendSets(1,2,3,comm);
        res+= sendOpenSets(4,5,comm);
      }
    //XX Entities sending (points, lines, surfaces,...) pending.
    return res;
  }

//! @brief Receives members through the communicator argument.
int XC::MapSet::recvData(const Communicator &comm)
  {
    int sz= 0;
    int res= comm.receiveInt(sz,getDbTagData(),CommMetaData(0)); //Number of sets.
    if(sz>0)
      {
        res+= receiveSets(1,2,3,sz,comm);
        res+= receiveOpenSets(4,5,comm);
      }
    //XX Entities receiving (points, lines, surfaces,...) pending.
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::MapSet::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    inicComm(6);
    int res= sendData(comm);

    res+= comm.sendIdData(getDbTagData(),dataTag);
    if(res < 0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; failed to send data\n";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::MapSet::recvSelf(const Communicator &comm)
  {
    inicComm(6);
    const int dataTag= getDbTag();
    int res= comm.receiveIdData(getDbTagData(),dataTag);

    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; failed to receive ids.\n";
    else
      {
        //setTag(getDbTagDataPos(0));
        res+= recvData(comm);
        if(res<0)
          std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; failed to receive data.\n";
      }
    return res;
  }


//! @brief Return a Python dictionary with the object members values.
boost::python::dict XC::MapSet::getPyDict(void) const
  {
    boost::python::dict retval= PreprocessorContainer::getPyDict();
    retval["sets"]= MapSetBase::getPyDict();
    // The entities are populated automatically.
    // boost::python::list entitiesList;
    // for(map_ent_mdlr::const_iterator i= entities.begin(); i!= entities.end(); i++)
    //   {
    // 	const std::string name= (*i).first;
    // 	const EntMdlr *ent_mdlr= (*i).second;
    // 	const std::string className= ent_mdlr->getClassName();
    // 	const int tag= ent_mdlr->getTag();
    // 	boost::python::tuple tagClassNamePair= boost::python::make_tuple(className, tag);
    // 	entitiesList.append(tagClassNamePair);
    //   }
    // retval["entities"]= entitiesList;
    boost::python::list openSetsNames;
    if(!open_sets.empty())
      {
        for(const_iterator i= open_sets.begin();i!=open_sets.end();i++)
          openSetsNames.append((*i).first);
      }
    retval["open_sets"]= openSetsNames; 
    return retval;
  }

//! @brief Set the values of the object members from a Python dictionary.
void XC::MapSet::setPyDict(const boost::python::dict &d)
  {
    PreprocessorContainer::setPyDict(d);
    boost::python::dict setsDict= boost::python::extract<boost::python::dict>(d["sets"]);
    MapSetBase::setPyDict(getPreprocessor(), setsDict);
    boost::python::list openSetsNames= boost::python::extract<boost::python::list>(d["open_sets"]);
    const boost::python::ssize_t sz= len(openSetsNames);
    if(sz>0)
      {
	for(boost::python::ssize_t i=0; i<sz; i++)
	  {
	    const std::string name= boost::python::extract<std::string>(openSetsNames[i]);
	    abre_set(name);
	  }
      }
    // The entities are populated automatically.
    //this->entities.setPyDict(boost::python::extract<boost::python::dict>(d["entities"]));
  }
