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
//NodeLockers.cc

#include "NodeLockers.h"
#include "domain/domain/Domain.h"


//Constraints.
#include <domain/constraints/SFreedom_Constraint.h>



#include "utility/actor/actor/MovableString.h"
#include "utility/actor/actor/MovableID.h"
#include "utility/matrix/ID.h"

//! @brief Default constructor.
XC::NodeLockers::NodeLockers(Mesh *owr)
  : CommandEntity(owr), MovableObject(0),  tag(0), code("nil") {}

//! @brief Returns a pointer to the blocker whose name is passed as parameter.
XC::NodeLocker *XC::NodeLockers::find_node_locker(const std::string &nmb)
  {
    NodeLocker *retval= nullptr;
    map_node_lockers::iterator i= node_lockers.find(nmb);
    if(i!=node_lockers.end())
      retval= i->second;
    return retval;
  }

//! @brief Returns a pointer to the blocker whose name is passed as parameter.
const XC::NodeLocker *XC::NodeLockers::find_node_locker(const std::string &nmb) const
  {
    const XC::NodeLocker *retval= nullptr;
    map_node_lockers::const_iterator i= node_lockers.find(nmb);
    if(i!=node_lockers.end())
      retval= i->second;
    return retval;
  }

//! @brief Returns a pointer to the blocker whose tag is passed as parameter.
XC::NodeLocker *XC::NodeLockers::buscaNodeLocker(const int &tag)
  {
    NodeLocker *retval= nullptr;
    for(map_node_lockers::iterator i= node_lockers.begin();i!=node_lockers.end();i++)
      if(i->second->getTag()==tag)
        {
          retval= i->second;
          break;
        }
    return retval;
  }

//! @brief Returns a pointer to the blocker whose tag is passed as parameter.
const XC::NodeLocker *XC::NodeLockers::buscaNodeLocker(const int &tag) const
  {
    NodeLocker *retval= nullptr;
    for(map_node_lockers::const_iterator i= node_lockers.begin();i!=node_lockers.end();i++)
      if(i->second->getTag()==tag)
        {
          retval= i->second;
          break;
        }
    return retval;
  }

XC::NodeLocker *XC::NodeLockers::newNodeLocker(const std::string &nmb)
  {
    NodeLocker *retval= nullptr;
    if(!nmb.empty())
      {
        retval= find_node_locker(nmb);
        if(!retval)
          {
            retval= new NodeLocker(tag);
            if(retval)
              {
                node_lockers[nmb]= retval;
                tag++;
              }
            else
              std::cerr << getClassName() << "::" << __FUNCTION__
		        << "; can't create: '"
                        << nmb << "'\n";
          }
        else
	  std::clog << getClassName() << "::" << __FUNCTION__
		    << "; '"
                    << nmb << "' already exists.\n";
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; name is empty." << std::endl;
    return retval;
  }

void XC::NodeLockers::borraNodeLocker(const std::string &nmb)
  {
    map_node_lockers::iterator i= node_lockers.find(nmb);
    if(i!=node_lockers.end())
      {
        delete (*i).second;
        node_lockers.erase(i);
      }
    else
      std::cerr << getClassName() << "::" << __FUNCTION__
	        << "; node locker: '"
                << nmb << "' not found.\n";
  }

void XC::NodeLockers::removeFromDomain(const std::string &cod)
  {
    NodeLocker *p= find_node_locker(cod);
    if(p)
      getDomain()->removeNodeLocker(p);
    else
      {
        std::cerr << getClassName() << "::" << __FUNCTION__
		  << "; load case: '" 
                  << cod << "' not found." << std::endl;
      }
  }

void XC::NodeLockers::removeAllFromDomain(void)
  {
    std::deque<std::string> names;
    for(map_node_lockers::iterator i= node_lockers.begin();i!=node_lockers.end();i++)
      names.push_back((*i).first);
    for(std::deque<std::string>::const_iterator i= names.begin();i!=names.end();i++)
      removeFromDomain(*i);
  }

//! @brief Clears all the objects.
void XC::NodeLockers::clearAll(void)
  {
    removeAllFromDomain();
    for(map_node_lockers::iterator i= node_lockers.begin();i!= node_lockers.end();i++)
      delete (*i).second;
    node_lockers.erase(node_lockers.begin(),node_lockers.end());
  }

//! @brief Destructor.
XC::NodeLockers::~NodeLockers(void)
  { clearAll(); }

const std::string XC::NodeLockers::getCurrentNodeLockerId(void) const
  { return code; }

XC::NodeLocker *XC::NodeLockers::getCurrentNodeLockerPtr(void)
  { return find_node_locker(code); }

const XC::NodeLocker *XC::NodeLockers::getCurrentNodeLockerPtr(void) const
  { return find_node_locker(code); }

//! @brief Returns the domain.
XC::Domain *XC::NodeLockers::getDomain(void)
  {
    Domain *retval= nullptr;
    Mesh *mesh= dynamic_cast<Mesh *>(Owner());
    if(mesh)
      retval= mesh->getDomain();
    else
      std::cerr << "(NodeLockers) mesh needed." << std::endl;
    return retval;
  }

//! @brief Returns the domain.
const XC::Domain *XC::NodeLockers::getDomain(void) const
  {
    const Domain *retval= nullptr;
    const Mesh *mesh= dynamic_cast<const Mesh *>(Owner());
    if(mesh)
      retval= mesh->getDomain();
    else
      std::cerr << "(NodeLockers) mesh needed." << std::endl;
    return retval;
  }

//! @brief Returns a vector to store the dbTags
//! of the class members.
XC::DbTagData &XC::NodeLockers::getDbTagData(void) const
  {
    static DbTagData retval(5);
    return retval;
  }

//! @brief Send members through the communicator argument.
int XC::NodeLockers::sendData(Communicator &comm)
  {
    setDbTagDataPos(0,getTag());
    const size_t sz= node_lockers.size();      
    setDbTagDataPos(1,sz);
    int res= 0;
    if(sz>0)
      {
        DbTagData labelData(sz);
        DbTagData objData(sz);
        int loc= 0;
        for(std::map<std::string,NodeLocker *>::iterator i=node_lockers.begin();i!=node_lockers.end();i++,loc++)
          {
            res+= comm.sendString((*i).first,labelData,CommMetaData(loc));
            res+= comm.sendMovable(*(*i).second,objData,CommMetaData(loc));
          }
        res+= labelData.send(getDbTagData(),comm,CommMetaData(2));
        res+= objData.send(getDbTagData(),comm,CommMetaData(3));
      }
    res+= comm.sendString(code,getDbTagData(),CommMetaData(4));
    return res;
  }

//! @brief Receives members through the communicator argument.
int XC::NodeLockers::recvData(const Communicator &comm)
  {
    tag= getDbTagDataPos(0);
    const size_t sz= getDbTagDataPos(1);
    int res= 0;
    if(sz>0)
      {
        DbTagData labelData(sz);
        res+= labelData.receive(getDbTagData(),comm,CommMetaData(2));
        DbTagData objData(sz);
        res+= objData.receive(getDbTagData(),comm,CommMetaData(3));
        std::string label;
        NodeLocker *tmp= nullptr;
        for(size_t i= 0;i<sz;i++)
          {
            res+= comm.receiveString(label,labelData,CommMetaData(i));
            tmp= newNodeLocker(label);
            if(tmp)
              res+= comm.receiveMovable(*tmp,objData,CommMetaData(i));
            else
	      std::cerr << "Error en NodeLockers::recvData label= " 
                        << label << std::endl;
          }
      }
    res+= comm.receiveString(code,getDbTagData(),CommMetaData(4));
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::NodeLockers::sendSelf(Communicator &comm)
  {
    inicComm(5);
    int result= sendData(comm);

    const int dataTag= getDbTag();
    result+= comm.sendIdData(getDbTagData(),dataTag);
    if(result < 0)
      std::cerr << "NodeLockers::sendSelf() - ch failed to send data\n";
    return result;
  }

//! @brief Receives object through the communicator argument.
int XC::NodeLockers::recvSelf(const Communicator &comm)
  {
    inicComm(5);

    const int dataTag = this->getDbTag();  
    int result = comm.receiveIdData(getDbTagData(),dataTag);
    if(result<0)
      std::cerr << "NodeLockers::sendSelf() - ch failed to receive data\n";
    else
      result+= recvData(comm);
    return result;    
  }
