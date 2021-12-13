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
// $Revision: 1.5 $
// $Date: 2004/07/20 22:44:18 $
// $Source: /usr/local/cvs/OpenSees/SRC/nDarray/nDarray.cpp,v $
                                                                        
                                                                        
//############################################################################
//#                                                                          #
//#             /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/~~\              #
//#            |                                          |____|             #
//#            |                                          |                  #
//#            |                                          |                  #
//#            |                 B A S E                  |                  #
//#            |                                          |                  #
//#            |                                          |                  #
//#            |              C L A S S E S               |                  #
//#            |                                          |                  #
//#            |                                          |                  #
//#            |          C + +     S O U R C E           |                  #
//#            |                                          |                  #
//#            |                                          |                  #
//#            |                                          |                  #
//#            |                                          |                  #
//#            |                                          |                  #
//#         /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~/   |                  #
//#        |                                         |    |                  #
//#         \_________________________________________\__/                   #
//#                                                                          #
//#                                                                          #
//############################################################################
//
//   "C makes it easy to shoot yourself in the foot, C++ makes it harder,
//   but when you do, it blows away your whole leg" -- Bjarne Stroustrup
//
//#############################################################################
//                         PAZI 'VAMO:
//
//
//0. make more types for (int), (int, int),
//   (int,int,int) . . .
//
//1. for efficiency code val(int), val(int, int) . . .  up to
//   fourth order inline . . . also operator() . . .
//   Check in Ellis & Stroustrup about inline functions
//
//2. Code symmetric operators ( *, +, -, : . . . ) as friends
//   see Coplien's recomendation . . .
//
//
//
//
////////////////////////////////////////////////////////////////////////////////
///*
//################################################################################
//# COPY-YES  (C):     :-))                                                      #
//# PROJECT:           Object Oriented Finite XC::Element Program                    #
//# PURPOSE:                                                                     #
//# CLASS:             nDarray                                                   #
//#                                                                              #
//# VERSION:                                                                     #
//# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )  #
//# TARGET OS:         DOS || UNIX || . . .                                      #
//# DESIGNER(S):       Boris Jeremic                                             #
//# PROGRAMMER(S):     Boris Jeremic                                             #
//#                                                                              #
//#                                                                              #
//# DATE:              May 28. - July  21  '93                                   #
//# UPDATE HISTORY:    august 05. - august __ '93 base_03 ( BJmatrix, BJvector,      #
//#                                               skyBJmatrix ...) from this one   #
//#                    August 22-29 '94 choped to separate files and worked on   #
//#                                   const and & issues                         #
//#                    August 30-31 '94 added use_def_dim to full the CC         #
//#                                   resolved problem with temoraries for       #
//#                                   operators + and - ( +=, -= )               #
//#                    January 16 '95 fixed the memory leakage introduced        #
//#                                   by previous work on +=, -+. I was          #
//#                                   by mistake decreasing                      #
//#                                   this->pc_nDarray_rep->total_numb--;        #
//#                                   inststead of                               #
//#                                   this->pc_nDarray_rep->n--;                 #
//#                    28June2004     added val4 for efficiency still            #
//#                                   to be worked on                            #
//#                                                                              #
//#                                                                              #
//################################################################################
//*/
// nDarray.cc

#ifndef NDARRAY_CC
#define NDARRAY_CC


#include "nDarray.h"
#include <iostream>

//! @brief Initialize dimensions vector.
void XC::nDarray_rep::init_dim(const size_t &sz, const int &default_dim)
  {
     dim= std::vector<int>(sz);// array for dimensions.
     total_numb= 1;
     for(int i = 0 ; i<nDarray_rank ; i++ )
       {
	 dim[i]= default_dim;
	 total_numb*= default_dim;
       }
  }

//! @brief Initialize dimensions vector.
void XC::nDarray_rep::init_dim(const size_t &sz, const std::vector<int> &pdim)
  {
    dim= std::vector<int>(sz);// array for dimensions.
    total_numb= 1;
    int idim= 0;
    for(int i= 0 ; i<nDarray_rank ; i++ )
      {
	idim= pdim[i];
	dim[i]= idim;
	total_numb*= idim;
      }
  }

//! @brief Constructor.
XC::nDarray::nDarray(int rank_of_nDarray, const double &initval)
  {
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 1
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    const int default_dim  = 1;
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, default_dim);
     

    // allocate memory for the actual XC::nDarray as XC::nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t) pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }

    pc_nDarray_rep->n = 1;  // so far, there's one reference

    for( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
       pc_nDarray_rep->pd_nDdata[i]= initval;

  }

//! @brief Constructor.
XC::nDarray::nDarray(int rank_of_nDarray, const std::vector<int> &pdim, const double *values)
  {
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 2
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);



    // allocate memory for the actual nDarray as nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }

     pc_nDarray_rep->n = 1;  // so far, there's one reference

     for( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
       pc_nDarray_rep->pd_nDdata[i] = values[i];

  }

//! @brief Constructor.
XC::nDarray::nDarray(int rank_of_nDarray, const std::vector<int> &pdim, const std::vector<double> &values)
  {
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 2
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);

    // allocate memory for the actual nDarray as nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }

     pc_nDarray_rep->n = 1;  // so far, there's one reference

      for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
	pc_nDarray_rep->pd_nDdata[i] = values[i];

  }

//! @brief Constructor.
XC::nDarray::nDarray(int rank_of_nDarray, const std::vector<int> &pdim, const boost::python::list &l)
  {
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 3
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);

    // allocate memory for the actual nDarray as nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }

    pc_nDarray_rep->n = 1;  // so far, there's one reference

    for(int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      pc_nDarray_rep->pd_nDdata[i] =boost::python::extract<double>(l[i]);
  }

//##############################################################################
XC::nDarray::nDarray(int rank_of_nDarray, const std::vector<int> &pdim, double initvalue)
  {
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 4
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);

    // allocate memory for the actual XC::nDarray as XC::nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }
    
    pc_nDarray_rep->n = 1;  // so far, there's one reference

    for( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      pc_nDarray_rep->pd_nDdata[i] = initvalue;
  }



//##############################################################################
// special case for XC::BJmatrix and XC::BJvector . . .
XC::nDarray::nDarray(int rank_of_nDarray, int rows, int cols, double *values)
  {
    // create the structure:
    pc_nDarray_rep =  new nDarray_rep; // this 'new' is overloaded 5
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // not needed for BJmatrix or BJvector but who knows #
    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    const std::vector<int> pdim= {rows, cols};
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0,pdim);

    // allocate memory for the actual XC::nDarray as XC::nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }
    pc_nDarray_rep->n = 1;  // so far, there's one reference
    for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      pc_nDarray_rep->pd_nDdata[i] = values[i];
  }

//##############################################################################
// special case for XC::BJmatrix and XC::BJvector . . .
XC::nDarray::nDarray(int rank_of_nDarray, int rows, int cols, double value)
  {
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 6
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // not needed for BJmatrix or BJvector but who knows #
    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    const std::vector<int> pdim= {rows, cols};
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);

    // allocate memory for the actual XC::nDarray as XC::nDarray
    pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!pc_nDarray_rep->pd_nDdata)
      {
        std::cerr << "\a\nInsufficient memory for array\n";
	::exit(1);
      }
    
    pc_nDarray_rep->n = 1;  // so far, there's one reference

    for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      pc_nDarray_rep->pd_nDdata[i] = value;
  }


//---//##############################################################################
//---// special case for XC::BJmatrix and XC::BJvector . . .
//---XC::nDarray::nDarray(int rank_of_nDarray, int rows, int cols, double initvalue)
//---{
//--- // create the structure:
//---  pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded
//---  pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;
//---
//---// not needed for XC::BJmatrix or XC::BJvector but who knows #
//---// in the case of nDarray_rank=0 add one to get right thing from the
//---// operator new
//---  int one_or0 = 0;
//---  if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
//---  std::vector<int> pdim= {rows, cols};
//---  pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);
//---
//---  pc_nDarray_rep->total_numb = 1;
//---
//---  pc_nDarray_rep->dim[0] = rows;
//---  pc_nDarray_rep->dim[1] = cols;
//---  pc_nDarray_rep->total_numb = rows*cols;
//---
//---// allocate memory for the actual XC::nDarray as XC::nDarray
//---  pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
//---    if (!pc_nDarray_rep->pd_nDdata)
//---      {
//---        ::fprintf(stderr,"\a\nInsufficient memory for array\n");
//---        ::exit(1);
//---      }
//---
//---  pc_nDarray_rep->n = 1;  // so far, there's one reference
//---
//---  for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
//---    pc_nDarray_rep->pd_nDdata[i] = initvalue;
//---}
//---





//! @brief create a unit XC::nDarray
XC::nDarray::nDarray(const std::string &flag, int rank_of_nDarray, const std::vector<int> &pdim)
  {
    if( flag[0] != 'I' && flag[0] != 'e' && flag[0] != 'C' )
      {
        std::cerr <<"\n To create a 2nd rank Kronecker delta type: nDarray (\"I\",2,dims);\n"
        //        << " To create a 4th rank unit XC::nDarray type: nDarray (\"I\",4,dims);\n";
	 	  << "To create a 3th rank Levi-Civita XC::BJtensor type: nDarray (\"e\",3,dims);\n"
		  << "To create a 2nd rank Cosserat Kronecker delta type: nDarray (\"C\",3,dims);\n";
        ::exit( 1 );
      }
    // create the structure:
    pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded 7
    pc_nDarray_rep->nDarray_rank = rank_of_nDarray;  //rank_of_nDarray;

    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0,pdim);


  // allocate memory for the actual XC::nDarray as XC::nDarray
     pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
       if (!pc_nDarray_rep->pd_nDdata)
	 {
	   ::fprintf(stderr,"\a\nInsufficient memory for array\n");
	   ::exit(1);
	 }

     pc_nDarray_rep->n = 1;  // so far, there's one reference

      switch(pc_nDarray_rep->nDarray_rank)
	{
	  case 0:
	    {
	      ::printf("\a\n Unit XC::nDarray of rank 0 ???\n");
	      break;
	    }

	  case 1:
	    {
	      ::printf("\a\n Unit XC::nDarray of rank 1 ???\n");
	      break;
	    }

		  case 2:   // Kronecker delta

		  if ( flag[0] == 'I' )
		    {
		      for ( int i2=1 ; i2<=pc_nDarray_rep->dim[0] ; i2++ )
			{
			  for ( int j2=1 ; j2<=pc_nDarray_rep->dim[1] ; j2++ )
			    {
			 val(i2,j2) = (i2 == j2 ? 1  : 0);
		       }
		   }
		 break;

	       }

	  case 3:  // Levi - Civita permutation XC::BJtensor
	     {
	       for ( int i3=1 ; i3<=pc_nDarray_rep->dim[0] ; i3++ )
		 {
		   for ( int j3=1 ; j3<=pc_nDarray_rep->dim[1] ; j3++ )
		     {
		       for ( int k3=1 ; k3<=pc_nDarray_rep->dim[2] ; k3++ )
			 {
			   if ( (i3==1 && j3==2 && k3==3)  ||
				(i3==2 && j3==3 && k3==1)  ||
				(i3==3 && j3==1 && k3==2) )
			     {
			       val(i3,j3,k3) = 1.0;
			     }
			   else if ( (i3==3 && j3==2 && k3==1)  ||
				     (i3==2 && j3==1 && k3==3)  ||
				     (i3==1 && j3==3 && k3==2) )
			     {
			       val(i3,j3,k3) = -1.0;
			     }
			   else val(i3,j3,k3) = 0.0;
			 }
		     }
		 }
	     break;
	     }

  //        case 4:
  //          for ( int i4=1 ; i4<=pc_nDarray_rep->dim[0] ; i4++ )
  //            for ( int j4=1 ; j4<=pc_nDarray_rep->dim[1] ; j4++ )
  //              for ( int k4=1 ; k4<=pc_nDarray_rep->dim[2] ; k4++ )
  //                for ( int l4=1 ; l4<=pc_nDarray_rep->dim[3] ; l4++ )
  //                  val(i4,j4,k4,l4) = (((i4==k4) && (j4==l4)) ? 1 : 0);
  //        break;
	}


  }


//##############################################################################
XC::nDarray::nDarray(const XC::nDarray & x)
 {
  x.pc_nDarray_rep->n++; // we're adding another reference.
  pc_nDarray_rep = x.pc_nDarray_rep;  // point to the new nDarray_rep.
 }



//##############################################################################
XC::nDarray::~nDarray()
  {
   if (--pc_nDarray_rep->n == 0)  // if reference count  goes to 0
      {
    // DEallocate memory of the actual nDarray
    //    delete [pc_nDarray_rep->pc_nDarray_rep->total_numb] pc_nDarray_rep->pd_nDdata;
    //  see ELLIS & STROUSTRUP $18.3
    //  and note on the p.65($5.3.4)
    //  and the page 276 ($12.4)
	delete [] pc_nDarray_rep->pd_nDdata;
	pc_nDarray_rep->clear_dim();
	delete pc_nDarray_rep;
      }
  }


//##############################################################################
// use "from" and initialize already allocated XC::BJtensor from "from" values
void XC::nDarray::Initialize( const XC::nDarray & from )
  {
// copy only data because everything else has already been defined
// WATCH OUT IT HAS TO BE DEFINED BEFORE THIS FUNCTIONS IS CALLED
    for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      this->pc_nDarray_rep->pd_nDdata[i] = from.pc_nDarray_rep->pd_nDdata[i] ;
  }
//##############################################################################
// // reset data to "value"
void XC::nDarray::Reset_to( double value )  // reset data to "value"
  {
// set only data because everything else has already been defined
// WATCH OUT IT HAS TO BE DEFINED BEFORE THIS FUNCTIONS IS CALLED
    for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      this->pc_nDarray_rep->pd_nDdata[i] = value;
  }
//##############################################################################
// use "from" and initialize AND allocate BJtensor from "from" values
// it should work on all the things that are missed by default constructor
//(int rank_of_nDarray, const int *pdim, double *values)
void XC::nDarray::Initialize_all(const nDarray &from)
  {
    // create the structure:
    //   pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded //IN DEFAULT
    this->pc_nDarray_rep->nDarray_rank= from.rank();  //rank_of_nDarray;
    // in the case of nDarray_rank=0 add one to get right thing from the
    // operator new
    int one_or0 = 0;
    if(!this->pc_nDarray_rep->nDarray_rank) one_or0 = 1;
    const std::vector<int> &pdim= from.dim();
    this->pc_nDarray_rep->init_dim(pc_nDarray_rep->nDarray_rank+one_or0, pdim);

    delete [] this->pc_nDarray_rep->pd_nDdata; // get rid of old allocated memory
    // allocate memory for the actual XC::nDarray as XC::nDarray
    this->pc_nDarray_rep->pd_nDdata = new double [(size_t)pc_nDarray_rep->total_numb];
    if(!this->pc_nDarray_rep->pd_nDdata)
       {
         std::cerr << "\a\nInsufficient memory for array in Initialize_all \n";
         exit(1);
       }
     this->pc_nDarray_rep->n = 1;  // so far, there's one reference
     for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
       this->pc_nDarray_rep->pd_nDdata[i] = from.pc_nDarray_rep->pd_nDdata[i];
  }

//##############################################################################
XC::nDarray XC::nDarray::deep_copy()
  {
   // create the structure:
     nDarray temp;
     temp.pc_nDarray_rep->nDarray_rank = this->pc_nDarray_rep->nDarray_rank;

  // in the case of nDarray_rank=0 add one to get right thing from the
  // operator new
     int one_or0 = 0;
     if(!temp.pc_nDarray_rep->nDarray_rank) one_or0 = 1;
     temp.pc_nDarray_rep->init_dim(temp.pc_nDarray_rep->nDarray_rank+one_or0, this->pc_nDarray_rep->dim);

     delete [] temp.pc_nDarray_rep->pd_nDdata;//delete default value
     temp.pc_nDarray_rep->pd_nDdata = new double [temp.pc_nDarray_rep->total_numb];
     if (!temp.pc_nDarray_rep->pd_nDdata)
       {
	 ::fprintf(stderr,"\a\nInsufficient memory for array in deep_copy\n");
	 ::exit(1);
       }

     temp.pc_nDarray_rep->n = 1;  // so far, there's one reference

     for ( int i=0 ; i<temp.pc_nDarray_rep->total_numb ; i++ )
       temp.pc_nDarray_rep->pd_nDdata[i] = this->pc_nDarray_rep->pd_nDdata[i] ;

     return temp;
  }


//##############################################################################
XC::nDarray& XC::nDarray::operator=(const nDarray & rval)
  {
    rval.pc_nDarray_rep->n++;  // tell the rval it has another reference

//   /*  It is important to increment the reference_counter in the new
//       nDarray before decrementing the reference_counter in the
//       old nDarray_rep to ensure proper operation when assigning a
//       nDarray_rep to itself ( after ARKoenig JOOP May/June '90 )  */

 // clean up current value;
    if(--pc_nDarray_rep->n == 0)  // if nobody else is referencing us.
      {
// DEallocate memory of the actual XC::nDarray
//      delete [pc_nDarray_rep->pc_nDarray_rep->total_numb] pc_nDarray_rep->pd_nDdata;
//  see ELLIS & STROUSTRUP $18.3
//  and note on the p.65($5.3.4)
        delete [] pc_nDarray_rep->pd_nDdata;
        pc_nDarray_rep->clear_dim();
        delete pc_nDarray_rep;
      }

 // connect to new value
    pc_nDarray_rep = rval.pc_nDarray_rep;  // point at the rval nDarray_rep
    return *this;
  }


//##############################################################################
const double &XC::nDarray::val(int subscript, ...) const
  {
// if scalar get back
    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
// for all others proceed
    va_list p_arg;
    va_start(p_arg, subscript); // initialize p_arg

    long int first  = 0;
    long int second = 0;

    first = subscript; // first index
    long int where = first - 1;
    for( int Dcount=1 ; Dcount<=pc_nDarray_rep->nDarray_rank-1 ; Dcount++ )
      {    // for all dimensions less then 1 this will be skipped
        second = va_arg(p_arg, int);    // next
        where = where*pc_nDarray_rep->dim[Dcount]+second - 1;
        first = second;
      }
    va_end(p_arg);
//::printf("*w=%2ld ",where);
    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
    return (*p_value);
  }
//##############################################################################
double &XC::nDarray::val(int subscript, ...)
  {
// if scalar get back
    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
// for all others proceed
    va_list p_arg;
    va_start(p_arg, subscript); // initialize p_arg

    long int first  = 0;
    long int second = 0;

    first = subscript; // first index
    long int where = first - 1;
    for( int Dcount=1 ; Dcount<=pc_nDarray_rep->nDarray_rank-1 ; Dcount++ )
      {    // for all dimensions less then 1 this will be skipped
        second = va_arg(p_arg, int);    // next
        where = where*pc_nDarray_rep->dim[Dcount]+second - 1;
        first = second;
      }
    va_end(p_arg);
//::printf("*w=%2ld ",where);
    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
    return (*p_value);
  }
// ..JB..//##############################################################################
// ..JB..// another overloading of operator() . . .  // overloaded for ONE argument
// ..JB..double & XC::nDarray::val(int first)
// ..JB..  {
// ..JB..    long int where = first - 1;
// ..JB..
// ..JB..//::printf(" w=%ld ",where);
// ..JB..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
// ..JB..    return (*p_value);
// ..JB..  }
// ..JB..
// ..JB..//##############################################################################
// ..JB..// another overloading of operator() . . .  // overloaded for TWO arguments
// ..JB..double & XC::nDarray::val(int first, int second)
// ..JB..  {
// ..JB..    long int where = first - 1;
// ..JB..             where = where*pc_nDarray_rep->dim[1]+second - 1;
// ..JB..
// ..JB..//::printf(" w=%ld ",where);
// ..JB..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
// ..JB..    return (*p_value);
// ..JB..  }
// ..JB..
// ..JB..//##############################################################################
// ..JB..// another overloading of operator() . . .  // overloaded for THREE arguments
// ..JB..double & XC::nDarray::val(int first, int second, int third )
// ..JB..  {
// ..JB..    long int where = first - 1;
// ..JB..             where = where*pc_nDarray_rep->dim[1]+second - 1;
// ..JB..             where = where*pc_nDarray_rep->dim[2]+third  - 1;
// ..JB..
// ..JB..//::printf(" w=%ld ",where);
// ..JB..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
// ..JB..    return (*p_value);
// ..JB..  }
// ..JB..
// ..JB..//##############################################################################
// ..JB..// another overloading of operator() . . .  // overloaded for FOUR arguments
// ..JB..double & XC::nDarray::val(int first, int second,
// ..JB..                      int third, int fourth)
// ..JB..  {
// ..JB..    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
// ..JB..
// ..JB..    long int where = first - 1;
// ..JB..
// ..JB..    if(pc_nDarray_rep->nDarray_rank==2)
// ..JB..      {
// ..JB..         where = where*pc_nDarray_rep->dim[1]+second - 1;
// ..JB..      }
// ..JB..
// ..JB..    if(pc_nDarray_rep->nDarray_rank==3)
// ..JB..      {
// ..JB..        where = where*pc_nDarray_rep->dim[2]+third  - 1;
// ..JB..      }
// ..JB..
// ..JB..    if(pc_nDarray_rep->nDarray_rank==4)
// ..JB..      {
// ..JB..        where = where*pc_nDarray_rep->dim[3]+fourth - 1;
// ..JB..      }
// ..JB..
// ..JB..//::printf(" w=%ld ",where);
// ..JB..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
// ..JB..    return (*p_value);
// ..JB..  }
// ..JB..
// ..JB..



//##############################################################################
// another overloading of operator() . . .  // overloaded for FOUR arguments
const double &XC::nDarray::val4(int first, int second, int third, int fourth) const
  {
//JB//JB    long int where = (((first-1)*pc_nDarray_rep->dim[1]+second-1)*pc_nDarray_rep->dim[2]+third-1)*pc_nDarray_rep->dim[3]+fourth-1;
//JB    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//JB// ..JB..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//JB    return (*p_value);
//JB//    return (*pc_nDarray_rep->pd_nDdata + (size_t)(((first-1)*pc_nDarray_rep->dim[1]+second-1)*pc_nDarray_rep->dim[2]+third-1)*pc_nDarray_rep->dim[3]+fourth-1);


    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);

    long int where = first - 1;

    if(pc_nDarray_rep->nDarray_rank==2)
      { where = where*pc_nDarray_rep->dim[1]+second - 1; }

    if(pc_nDarray_rep->nDarray_rank==3)
      { where = where*pc_nDarray_rep->dim[2]+third  - 1; }

    if(pc_nDarray_rep->nDarray_rank==4)
      { where = where*pc_nDarray_rep->dim[3]+fourth - 1; }

//::printf(" w=%ld ",where);
    double *p_value= pc_nDarray_rep->pd_nDdata + (size_t)where;
    return (*p_value);
  }

//##############################################################################
// another overloading of operator() . . .  // overloaded for FOUR arguments
double &XC::nDarray::val4(int first, int second, int third, int fourth)
  {
//JB//JB    long int where = (((first-1)*pc_nDarray_rep->dim[1]+second-1)*pc_nDarray_rep->dim[2]+third-1)*pc_nDarray_rep->dim[3]+fourth-1;
//JB    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//JB// ..JB..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//JB    return (*p_value);
//JB//    return (*pc_nDarray_rep->pd_nDdata + (size_t)(((first-1)*pc_nDarray_rep->dim[1]+second-1)*pc_nDarray_rep->dim[2]+third-1)*pc_nDarray_rep->dim[3]+fourth-1);


    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);

    long int where = first - 1;

    if(pc_nDarray_rep->nDarray_rank==2)
      { where = where*pc_nDarray_rep->dim[1]+second - 1; }

    if(pc_nDarray_rep->nDarray_rank==3)
      { where = where*pc_nDarray_rep->dim[2]+third  - 1; }

    if(pc_nDarray_rep->nDarray_rank==4)
      { where = where*pc_nDarray_rep->dim[3]+fourth - 1; }

//::printf(" w=%ld ",where);
    double *p_value= pc_nDarray_rep->pd_nDdata + (size_t)where;
    return (*p_value);
  }



//..//##############################################################################
//..// another overloading of operator() . . .
//..// overloaded for more than FOUR arguments
//..double & XC::nDarray::val(int first, int second,
//..                      int third, int fourth,
//..                      int subscript, ...)
//..  {
//..// if the scalar is zero order then o'ma back
//..    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
//..// for any higher order go ahead
//..    va_list p_arg;
//..    va_start(p_arg, subscript); // initialize p_arg
//..
//..    int prvi  = subscript;
//..    int drugi = 0;
//..
//..    long int where = first - 1;
//..             where = where*pc_nDarray_rep->dim[1]+second - 1;
//..             where = where*pc_nDarray_rep->dim[2]+third  - 1;
//..             where = where*pc_nDarray_rep->dim[3]+fourth - 1;
//..
//..// dalje iza cetvrtog
//..             where = where*pc_nDarray_rep->dim[4]+prvi - 1;
//..
//..
//..    for ( int Dcount=5 ; Dcount<=pc_nDarray_rep->nDarray_rank-1 ; Dcount++ )
//..      {    // this will actually be skipped for all dimensions less than 2
//..        drugi = va_arg(p_arg, int);    // sledeci
//..        where = where*pc_nDarray_rep->dim[Dcount]+drugi - 1;
//..        prvi = drugi;
//..      }
//..    va_end(p_arg);
//..
//..//::printf(" w=%ld ",where);
//..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//..    return (*p_value);
//..  }



//##############################################################################
double XC::nDarray::cval(int subscript, ...)  const
  {
// if scalar get back
    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
// for all others proceed
    va_list p_arg;
    va_start(p_arg, subscript); // initialize p_arg

    int first  = 0;
    int second = 0;

    first = subscript; // first indeks
    long int where = first - 1;
    for ( int Dcount=1 ; Dcount<=pc_nDarray_rep->nDarray_rank-1 ; Dcount++ )
      {    // for all dimensions less then 2 this will be skipped
        second = va_arg(p_arg, int);    // next
        where = where*pc_nDarray_rep->dim[Dcount]+second - 1;
        first = second;
      }
    va_end(p_arg);

//::printf("*w=%2ld ",where);
    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
    return (*p_value);
  }


//..//##############################################################################
//..// another overloading of operator() . . .
//..double XC::nDarray::operator()(int subscript, ...) const
//..  {
//..// if the scalar is zero order then o'ma back
//..    if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
//..// for any higher order go ahead
//..    va_list p_arg;
//..    va_start(p_arg, subscript); // initialize p_arg
//..
//..    int prvi  = 0;
//..    int drugi = 0;
//..
//..    prvi = subscript; // prvi indeks
//..    long int where = prvi - 1;
//..    for ( int Dcount=1 ; Dcount<=pc_nDarray_rep->nDarray_rank-1 ; Dcount++ )
//..      {    // this will actually be skipped for all dimensions less than 2
//..        drugi = va_arg(p_arg, int);    // sledeci
//..        where = where*pc_nDarray_rep->dim[Dcount]+drugi - 1;
//..        prvi = drugi;
//..      }
//..    va_end(p_arg);
//..
//..//::printf(" w=%ld ",where);
//..    double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//..    return (*p_value);
//..  }
//..
//..
//..

//@@@@@
//@@@@@ //##############################################################################
//@@@@@ // another overloading of operator() . . .  // overloaded for ONE argument
//@@@@@ double XC::nDarray::operator()(int first) const
//@@@@@   {
//@@@@@     long int where = first - 1;
//@@@@@
//@@@@@ //::printf(" w=%ld ",where);
//@@@@@     double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//@@@@@     return (*p_value);
//@@@@@   }
//@@@@@
//@@@@@ //##############################################################################
//@@@@@ // another overloading of operator() . . .  // overloaded for TWO arguments
//@@@@@ double XC::nDarray::operator()(int first, int second) const
//@@@@@   {
//@@@@@     long int where = first - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[1]+second - 1;
//@@@@@
//@@@@@ //::printf(" w=%ld ",where);
//@@@@@     double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//@@@@@     return (*p_value);
//@@@@@   }
//@@@@@
//@@@@@ //##############################################################################
//@@@@@ // another overloading of operator() . . .  // overloaded for THREE arguments
//@@@@@ double XC::nDarray::operator()(int first, int second, int third ) const
//@@@@@   {
//@@@@@     long int where = first - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[1]+second - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[2]+third  - 1;
//@@@@@
//@@@@@ //::printf(" w=%ld ",where);
//@@@@@     double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//@@@@@     return (*p_value);
//@@@@@   }
//@@@@@
//@@@@@ //##############################################################################
//@@@@@ // another overloading of operator() . . .  // overloaded for FOUR arguments
//@@@@@ double XC::nDarray::operator()(int first, int second,
//@@@@@                            int third, int fourth) const
//@@@@@   {
//@@@@@     long int where = first - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[1]+second - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[2]+third  - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[3]+fourth - 1;
//@@@@@
//@@@@@ //::printf(" w=%ld ",where);
//@@@@@     double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//@@@@@     return (*p_value);
//@@@@@   }
//@@@@@
//@@@@@
//@@@@@
//@@@@@ //##############################################################################
//@@@@@ // another overloading of operator() . . .
//@@@@@ // overloaded for more than FOUR arguments
//@@@@@ double XC::nDarray::operator()(int first, int second,
//@@@@@                            int third, int fourth,
//@@@@@                            int subscript, ...) const
//@@@@@   {
//@@@@@ // if the scalar is zero order then o'ma back
//@@@@@     if(pc_nDarray_rep->nDarray_rank==0) return (*pc_nDarray_rep->pd_nDdata);
//@@@@@ // for any higher order go ahead
//@@@@@     va_list p_arg;
//@@@@@     va_start(p_arg, subscript); // initialize p_arg
//@@@@@
//@@@@@     int prvi  = subscript;
//@@@@@     int drugi = 0;
//@@@@@
//@@@@@     long int where = first - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[1]+second - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[2]+third  - 1;
//@@@@@              where = where*pc_nDarray_rep->dim[3]+fourth - 1;
//@@@@@
//@@@@@ // dalje iza cetvrtog
//@@@@@              where = where*pc_nDarray_rep->dim[4]+prvi - 1;
//@@@@@
//@@@@@
//@@@@@     for ( int Dcount=5 ; Dcount<=pc_nDarray_rep->nDarray_rank-1 ; Dcount++ )
//@@@@@       {    // this will actually be skipped for all dimensions less than 2
//@@@@@         drugi = va_arg(p_arg, int);    // sledeci
//@@@@@         where = where*pc_nDarray_rep->dim[Dcount]+drugi - 1;
//@@@@@         prvi = drugi;
//@@@@@       }
//@@@@@     va_end(p_arg);
//@@@@@
//@@@@@ //::printf(" w=%ld ",where);
//@@@@@     double *p_value = pc_nDarray_rep->pd_nDdata + (size_t)where;
//@@@@@     return (*p_value);
//@@@@@   }
//@@@@@
//@@@@@
//@@@@@



//++//##############################################################################
//++// nDarray addition
//++nDarray XC::nDarray::operator+( nDarray & rval)
//++  {
//++    int this_rank_of_nDarray = this->pc_nDarray_rep->nDarray_rank;
//++    int rval_rank_of_nDarray =  rval.pc_nDarray_rep->nDarray_rank;
//++
//++    if(this_rank_of_nDarray != rval_rank_of_nDarray)
//++      {
//++        ::printf("\a\nnDarrays of different ranks: addition not possible\n");
//++        ::exit ( 1 );
//++      }
//++
//++    for ( int i=0 ; i<this_rank_of_nDarray ; i++ )
//++      if (this->pc_nDarray_rep->dim[i] != rval.pc_nDarray_rep->dim[i] )
//++        {
//++          ::fprintf(stderr,"\a\nDimension discrepancy in operator+\n",
//++                           "this->pc_nDarray_rep->dim[%d]=%d\n",
//++                           "arg.pc_nDarray_rep->dim[%d]=%d\n",
//++                           i,this->pc_nDarray_rep->dim[i],
//++                           i,rval.pc_nDarray_rep->dim[i]);
//++          ::exit(1);
//++        }
//++// construct XC::nDarray using the same control numbers as for the
//++// original one .
//++      nDarray add(pc_nDarray_rep->nDarray_rank, pc_nDarray_rep->dim, 0.0);
//++
//++      switch(pc_nDarray_rep->nDarray_rank)
//++        {
//++          case 0:
//++            {
//++              add.val(1) = val(1) + rval.val(1);
//++              break;
//++            }
//++
//++          case 1:
//++            {
//++              for ( int i1=1 ; i1<=this->pc_nDarray_rep->dim[0] ; i1++ )
//++                {
//++                  add.val(i1) = val(i1) + rval.val(i1);
//++                }
//++              break;
//++            }
//++
//++          case 2:
//++            {
//++              for ( int i2=1 ; i2<=this->pc_nDarray_rep->dim[0] ; i2++ )
//++                {
//++                  for ( int j2=1 ; j2<=this->pc_nDarray_rep->dim[1] ; j2++ )
//++                    {
//++                      add.val(i2, j2) = val(i2, j2) + rval.val(i2, j2);
//++                    }
//++                }
//++              break;
//++            }
//++
//++          case 3:
//++            {
//++              for ( int i3=1 ; i3<=this->pc_nDarray_rep->dim[0] ; i3++ )
//++                {
//++                  for ( int j3=1 ; j3<=this->pc_nDarray_rep->dim[1] ; j3++ )
//++                    {
//++                      for ( int k3=1 ; k3<=this->pc_nDarray_rep->dim[2] ; k3++ )
//++                        {
//++                          add.val(i3, j3, k3) = val(i3, j3, k3) + rval.val(i3, j3, k3);
//++                        }
//++                    }
//++                }
//++              break;
//++            }
//++
//++          case 4:
//++            {
//++              for ( int i4=1 ; i4<=this->pc_nDarray_rep->dim[0] ; i4++ )
//++                {
//++                  for ( int j4=1 ; j4<=this->pc_nDarray_rep->dim[1] ; j4++ )
//++                    {
//++                      for ( int k4=1 ; k4<=this->pc_nDarray_rep->dim[2] ; k4++ )
//++                        {
//++                          for ( int l4=1 ; l4<=this->pc_nDarray_rep->dim[3] ; l4++ )
//++                            {
//++                              add.val(i4,j4,k4,l4)=val(i4,j4,k4,l4)+rval.val(i4,j4,k4,l4);
//++                            }
//++                        }
//++                    }
//++                }
//++              break;
//++            }
//++        }
//++
//++    return add;
//++  }

//! @brief nDarray addition
XC::nDarray& XC::nDarray::operator+=(const nDarray & rval)
  {
    int this_rank_of_nDarray = this->pc_nDarray_rep->nDarray_rank;
    int rval_rank_of_nDarray =  rval.pc_nDarray_rep->nDarray_rank;

    if(this_rank_of_nDarray != rval_rank_of_nDarray)
      {
        ::printf("\a\nnDarrays of different ranks: += not possible\n");
        ::exit ( 1 );
      }

    int i = 0;
    for ( i=0 ; i<this_rank_of_nDarray ; i++ )
      if (this->pc_nDarray_rep->dim[i] != rval.pc_nDarray_rep->dim[i] )
        {
	  ::fprintf(stderr,"\a\nDimension discrepancy in operator+=\n\
	  this->pc_nDarray_rep->dim[%d]=%d\n\
	  arg.pc_nDarray_rep->dim[%d]=%d\n",
	  i,this->pc_nDarray_rep->dim[i],
	  i,rval.pc_nDarray_rep->dim[i]);
	  ::exit(1);
        }
// Copy *this if necessary
    if ( this->pc_nDarray_rep->n > 1 )// see ARK in JOOP may/june '90
      {                               // "Letter From a Newcomer"
//..............................................................................
      // create the structure:
        nDarray_rep * New_pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded
        New_pc_nDarray_rep->nDarray_rank = this->pc_nDarray_rep->nDarray_rank;
// in the case of nDarray_rank=0 add one to get right thing from the
// operator new
        int one_or0 = 0;
        if(!New_pc_nDarray_rep->nDarray_rank) one_or0 = 1;
        New_pc_nDarray_rep->init_dim(New_pc_nDarray_rep->nDarray_rank+one_or0, this->pc_nDarray_rep->dim);

        // allocate memory for the actual nDarray as XC::nDarray
        New_pc_nDarray_rep->pd_nDdata = new double [(size_t)New_pc_nDarray_rep->total_numb];
          if (!New_pc_nDarray_rep->pd_nDdata)
            {
              ::fprintf(stderr,"\a\nInsufficient memory for array\n");
              ::exit(1);
            }
         New_pc_nDarray_rep->n = 1;  // so far, there's one reference
         for ( i=0 ; i<New_pc_nDarray_rep->total_numb ; i++ )
           New_pc_nDarray_rep->pd_nDdata[i] = this->pc_nDarray_rep->pd_nDdata[i];
//.........
         this->pc_nDarray_rep->n--;
         this->pc_nDarray_rep = New_pc_nDarray_rep;
//..............................................................................
      }
// it appears that I can add this two nDarrays just as a simple BJvectors:
    for (int j=0 ; j<this->pc_nDarray_rep->total_numb ; j++)
      this->pc_nDarray_rep->pd_nDdata[j] += rval.pc_nDarray_rep->pd_nDdata[j];

    return *this;
  }


//##############################################################################
// nDarray addition
XC::nDarray operator+(const XC::nDarray & lval, const XC::nDarray & rval)
  {
    XC::nDarray result(lval);
    result += rval;
    return result;
  }

//! @brief scalar addition
XC::nDarray XC::nDarray::operator+(const double &rval)
  {
      // construct XC::nDarray using the same control numbers as for the
      // original one.
      nDarray add(pc_nDarray_rep->nDarray_rank, pc_nDarray_rep->dim, 0.0);
      switch(pc_nDarray_rep->nDarray_rank)
        {
          case 0:
            {
              add.val(1) = val(1) + rval;
              break;
            }
          case 1:
            {
              for( int i1=1 ; i1<=this->pc_nDarray_rep->dim[0] ; i1++ )
                { add.val(i1) = val(i1) + rval; }
              break;
            }
          case 2:
            {
              for ( int i2=1 ; i2<=this->pc_nDarray_rep->dim[0] ; i2++ )
                {
                  for ( int j2=1 ; j2<=this->pc_nDarray_rep->dim[1]  ; j2++ )
                     {
                       add.val(i2, j2) = val(i2, j2) + rval;
                     }
                }
              break;
            }
          case 3:
            {
              for ( int i3=1 ; i3<=this->pc_nDarray_rep->dim[0] ; i3++ )
                {
                  for ( int j3=1 ; j3<=this->pc_nDarray_rep->dim[1] ; j3++ )
                    {
                      for ( int k3=1 ; k3<=this->pc_nDarray_rep->dim[2] ; k3++ )
                        {
                          add.val(i3, j3, k3) = val(i3, j3, k3) + rval;
                        }
                     }
                 }
              break;
            }
          case 4:
            {
              for ( int i4=1 ; i4<=this->pc_nDarray_rep->dim[0] ; i4++ )
                {
                  for ( int j4=1 ; j4<=this->pc_nDarray_rep->dim[1] ; j4++ )
                    {
                      for ( int k4=1 ; k4<=this->pc_nDarray_rep->dim[2] ; k4++ )
                        {
                          for ( int l4=1 ; l4<=this->pc_nDarray_rep->dim[3] ; l4++ )
                            {
                              add.val(i4,j4,k4,l4)=val(i4,j4,k4,l4)+rval;
                            }
                        }
                    }
                }
              break;
            }
        }
    return add;
 }


//! @brief scalar multiplication
XC::nDarray &XC::nDarray::operator*=(const double &rval)
   {
      for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
	this->pc_nDarray_rep->pd_nDdata[i]*=rval;
      return *this;
   }

//! @brief scalar multiplication
XC::nDarray  XC::nDarray::operator*(const double &rval) const
   {
      // construct XC::nDarray using the same control numbers as for the
      // original one.
      nDarray mult(pc_nDarray_rep->nDarray_rank, pc_nDarray_rep->dim, 0.0);
      mult*=(rval);
      return mult;
   }

//##############################################################################
// nDarray subtraction
XC::nDarray& XC::nDarray::operator-=(const nDarray & rval)
  {
    int this_rank_of_nDarray = this->pc_nDarray_rep->nDarray_rank;
    int rval_rank_of_nDarray =  rval.pc_nDarray_rep->nDarray_rank;
            if(this_rank_of_nDarray != rval_rank_of_nDarray)
      {
        ::printf("\a\nnDarrays of different ranks: -= not possible\n");
        ::exit ( 1 );
      }

    int i = 0;
    for ( i=0 ; i<this_rank_of_nDarray ; i++ )
      if (this->pc_nDarray_rep->dim[i] != rval.pc_nDarray_rep->dim[i] )
        {
	  ::fprintf(stderr,"\a\nDimension discrepancy in operator+\n\
	  this->pc_nDarray_rep->dim[%d]=%d\n\
	  arg.pc_nDarray_rep->dim[%d]=%d\n",
	  i,this->pc_nDarray_rep->dim[i],
	  i,rval.pc_nDarray_rep->dim[i]);
	  ::exit(1);
        }
// Copy *this if necessary
    if ( this->pc_nDarray_rep->n > 1 )// see ARK in JOOP may/june '90
      {                               // "Letter From a Newcomer"
//..............................................................................
      // create the structure:
        nDarray_rep * New_pc_nDarray_rep = new nDarray_rep; // this 'new' is overloaded
        New_pc_nDarray_rep->nDarray_rank = this->pc_nDarray_rep->nDarray_rank;
// in the case of nDarray_rank=0 add one to get right thing from the
// operator new
        int one_or0 = 0;
        if(!New_pc_nDarray_rep->nDarray_rank) one_or0 = 1;
        New_pc_nDarray_rep->init_dim(New_pc_nDarray_rep->nDarray_rank+one_or0, this->pc_nDarray_rep->dim);

        // allocate memory for the actual XC::nDarray as XC::nDarray
        New_pc_nDarray_rep->pd_nDdata = new double [(size_t)New_pc_nDarray_rep->total_numb];
          if (!New_pc_nDarray_rep->pd_nDdata)
            {
              ::fprintf(stderr,"\a\nInsufficient memory for array\n");
              ::exit(1);
            }
         New_pc_nDarray_rep->n = 1;  // so far, there's one reference
         for ( i=0 ; i<New_pc_nDarray_rep->total_numb ; i++ )
           New_pc_nDarray_rep->pd_nDdata[i] = this->pc_nDarray_rep->pd_nDdata[i];
//.........
         this->pc_nDarray_rep->n--;
         this->pc_nDarray_rep = New_pc_nDarray_rep;
//..............................................................................
      }
// it appears that I can add this two nDarrays just as a simple BJvectors:
    for (int j=0 ; j<this->pc_nDarray_rep->total_numb ; j++)
      this->pc_nDarray_rep->pd_nDdata[j] -= rval.pc_nDarray_rep->pd_nDdata[j];
    return *this;
  }


//##############################################################################
// nDarray subtraction
XC::nDarray operator-(const XC::nDarray &lval, const XC::nDarray &rval)
  {
    XC::nDarray result(lval);
    result -= rval;
    return result;
  }

//++//##############################################################################
//++// nDarray subtraction
//++nDarray XC::nDarray::operator-( nDarray & rval)
//++ {
//++    int this_rank_of_nDarray = this->pc_nDarray_rep->nDarray_rank;
//++    int rval_rank_of_nDarray = rval.pc_nDarray_rep->nDarray_rank;
//++
//++    if(this_rank_of_nDarray != rval_rank_of_nDarray)
//++      {
//++        ::printf("\a\nnDarrays of different ranks:",
//++                 " subtraction not possible\n");
//++        ::exit ( 1 );
//++      }
//++
//++    for ( int i=0 ; i<this_rank_of_nDarray ; i++ )
//++      if (this->pc_nDarray_rep->dim[i] != rval.pc_nDarray_rep->dim[i] )
//++        {
//++          ::fprintf(stderr,"\a\nDimension discrepancy in operator  -  \n",
//++                   "this->pc_nDarray_rep->dim[%d]=%d\n",
//++                   "arg.pc_nDarray_rep->dim[%d]=%d\n",
//++                    i,this->pc_nDarray_rep->dim[i],
//++                    i,rval.pc_nDarray_rep->dim[i]);
//++          ::exit(1);
//++        }
//++// construct XC::nDarray using the same control numbers as for the
//++// original one.
//++      nDarray sub(pc_nDarray_rep->nDarray_rank, pc_nDarray_rep->dim, 0.0);
//++
//++      switch(pc_nDarray_rep->nDarray_rank)
//++        {
//++          case 0:
//++            {
//++              sub.val(1) = val(1) - rval.val(1);
//++              break;
//++            }
//++
//++          case 1:
//++            {
//++              for ( int i1=1 ; i1<=this->pc_nDarray_rep->dim[0] ; i1++ )
//++                {
//++                  sub.val(i1) = val(i1) - rval.val(i1);
//++                }
//++              break;
//++            }
//++
//++          case 2:
//++            {
//++              for ( int i2=1 ; i2<=this->pc_nDarray_rep->dim[0] ; i2++ )
//++                {
//++                  for ( int j2=1 ; j2<=this->pc_nDarray_rep->dim[1] ; j2++ )
//++                    {
//++                      sub.val(i2, j2) = val(i2, j2) - rval.val(i2, j2);
//++                    }
//++                }
//++              break;
//++            }
//++
//++          case 3:
//++            {
//++              for ( int i3=1 ; i3<=this->pc_nDarray_rep->dim[0] ; i3++ )
//++                {
//++                  for ( int j3=1 ; j3<=this->pc_nDarray_rep->dim[1] ; j3++ )
//++                    {
//++                      for ( int k3=1 ; k3<=this->pc_nDarray_rep->dim[2] ; k3++ )
//++                        {
//++                          sub.val(i3, j3, k3) = val(i3, j3, k3) - rval.val(i3, j3, k3);
//++                        }
//++                    }
//++                }
//++              break;
//++            }
//++
//++          case 4:
//++            {
//++              for ( int i4=1 ; i4<=this->pc_nDarray_rep->dim[0] ; i4++ )
//++                {
//++                  for ( int j4=1 ; j4<=this->pc_nDarray_rep->dim[1] ; j4++ )
//++                    {
//++                      for ( int k4=1 ; k4<=this->pc_nDarray_rep->dim[2] ; k4++ )
//++                        {
//++
//++
//++
//++
//++                          for ( int l4=1 ; l4<=this->pc_nDarray_rep->dim[3] ; l4++ )
//++                            {
//++                              sub.val(i4,j4,k4,l4)=val(i4,j4,k4,l4)-rval.val(i4,j4,k4,l4);
//++                            }
//++                        }
//++                    }
//++                }
//++              break;
//++             }
//++        }
//++
//++    return sub;
//++ }
//++

//##############################################################################
// scalar subtraction
XC::nDarray  XC::nDarray::operator-(const double &rval)
  {
    // construct XC::nDarray using the same control numbers as for the
    // original one.
    nDarray sub(pc_nDarray_rep->nDarray_rank, pc_nDarray_rep->dim, 0.0);

    switch(pc_nDarray_rep->nDarray_rank)
      {
        case 0:
          {
            sub.val(1) = val(1) - rval;
            break;
          }

        case 1:
          {
            for ( int i1=1 ; i1<=this->pc_nDarray_rep->dim[0] ; i1++ )
              sub.val(i1) = val(i1) - rval;
            break;
          }

        case 2:
          {
            for ( int i2=1 ; i2<=this->pc_nDarray_rep->dim[0] ; i2++ )
              for ( int j2=1 ; j2<=this->pc_nDarray_rep->dim[1] ; j2++ )
                sub.val(i2, j2) = val(i2, j2) - rval;
            break;
          }

        case 3:
          {
            for ( int i3=1 ; i3<=this->pc_nDarray_rep->dim[0] ; i3++ )
              for ( int j3=1 ; j3<=this->pc_nDarray_rep->dim[1] ; j3++ )
                for ( int k3=1 ; k3<=this->pc_nDarray_rep->dim[2] ; k3++ )
                  sub.val(i3, j3, k3) = val(i3, j3, k3) - rval;
            break;
          }

        case 4:
          {
            for ( int i4=1 ; i4<=this->pc_nDarray_rep->dim[0] ; i4++ )
              for ( int j4=1 ; j4<=this->pc_nDarray_rep->dim[1] ; j4++ )
                for ( int k4=1 ; k4<=this->pc_nDarray_rep->dim[2] ; k4++ )
                  for ( int l4=1 ; l4<=this->pc_nDarray_rep->dim[3] ; l4++ )
                    sub.val(i4,j4,k4,l4)=val(i4,j4,k4,l4)-rval;
            break;
          }
      }

    return sub;
  }
//##############################################################################
//##############################################################################
// unary minus
XC::nDarray XC::nDarray::operator-( void )
  {
    nDarray ret = *this *-1.0;

    return ret;
  }


//#############################################################################
double XC::nDarray::sum() const
  {
    double sum = 0;
    for ( int memb=0 ; memb<pc_nDarray_rep->total_numb ; memb++ )
      sum += pc_nDarray_rep->pd_nDdata[memb];
    return sum;
  }

//##############################################################################
// trace function: trace of a second rank XC::BJtensor
// what about fourth ( 4th ) rank XC::BJtensor trace or any other rank ??
double XC::nDarray::trace() const
  {
    double tr = 0.0;
// ovaj case ne treba vec moze sve do 4-tog reda ( ili kasnije osmog # )
    switch(this->rank())
      {
        case 0:
          {
//            ::printf(" trace=%.4e  ", val(1));
//            ::printf("\n");
            tr = cval(1);
            break;
          }

        case 1:
          {
            if(dim()[0] != 1)
              {
		::printf("\a\nERROR in trace function : not a squared 1-st rank XC::BJtensor\n");
		::exit( 1 );
              }
            tr = cval(1);
            break;
          }

        case 2:
          {
            if(dim()[0] != dim()[1])
              {
		::printf("\a\nERROR in trace function : not a sqared 2nd-rank XC::BJtensor\n");
		::exit( 1 );
              }
            for ( int i2=1 ; i2<=dim()[0] ; i2++ )
              tr += cval(i2, i2);
            break;
          }

        case 3:
          {
            if( dim()[0] != dim()[1] ||
                dim()[1] != dim()[2] ||
                dim()[2] != dim()[0]    )
              {
		::printf("\a\nERROR in trace function : not a sqared 3nd-rank XC::BJtensor\n");
		::exit( 1 );
              }
            for ( int i3=1 ; i3<=dim()[0] ; i3++ )
              tr += cval(i3, i3, i3);
            break;
          }

        case 4:
          {
            if( dim()[0] != dim()[1] ||
                dim()[1] != dim()[2] ||
                dim()[2] != dim()[3] ||
                dim()[3] != dim()[0]    )
              {
		::printf("\a\nERROR in trace function : not a sqared 4nd-rank XC::BJtensor\n");
		::exit( 1 );
              }
            for ( int i4=1 ; i4<=dim()[0] ; i4++ )
              tr += cval(i4, i4, i4, i4);
            break;
          }
      }

//..........    null_indices();

    return tr;
  }



//-----//##############################################################################
//-----// scalar multiplication (nDarray * scalar)
//-----nDarray  XC::nDarray::operator*( double rval)
//----- {
//-----// construct XC::nDarray using the same control numbers as for the
//-----// original one.
//-----    nDarray mult(this->pc_nDarray_rep->nDarray_rank, this->pc_nDarray_rep->dim, 0.0);
//-----
//-----    switch(pc_nDarray_rep->nDarray_rank)
//-----      {
//-----        case 0:
//-----          {
//-----            mult.val(1) = val(1) * rval;
//-----            break;
//-----          }
//-----
//-----        case 1:
//-----          {
//-----            for ( int i1=1 ; i1<=this->pc_nDarray_rep->dim[0] ; i1++ )
//-----              mult.val(i1) = val(i1) * rval;
//-----            break;
//-----          }
//-----
//-----        case 2:
//-----          {
//-----            for ( int i2=1 ; i2<=this->pc_nDarray_rep->dim[0] ; i2++ )
//-----              for ( int j2=1 ; j2<=this->pc_nDarray_rep->dim[1] ; j2++ )
//-----                mult.val(i2, j2) = val(i2, j2) * rval;
//-----            break;
//-----          }
//-----
//-----        case 3:
//-----          {
//-----            for ( int i3=1 ; i3<=this->pc_nDarray_rep->dim[0] ; i3++ )
//-----              for ( int j3=1 ; j3<=this->pc_nDarray_rep->dim[1] ; j3++ )
//-----                for ( int k3=1 ; k3<=this->pc_nDarray_rep->dim[2] ; k3++ )
//-----                  mult.val(i3, j3, k3) = val(i3, j3, k3) * rval;
//-----            break;
//-----          }
//-----
//-----        case 4:
//-----          {
//-----            for ( int i4=1 ; i4<=this->pc_nDarray_rep->dim[0] ; i4++ )
//-----              for ( int j4=1 ; j4<=this->pc_nDarray_rep->dim[1] ; j4++ )
//-----                for ( int k4=1 ; k4<=this->pc_nDarray_rep->dim[2] ; k4++ )
//-----                  for ( int l4=1 ; l4<=this->pc_nDarray_rep->dim[3] ; l4++ )
//-----                    mult.val(i4,j4,k4,l4)=val(i4,j4,k4,l4)*rval;
//-----            break;
//-----          }
//-----      }
//-----
//-----    return mult;
//----- }
//-----

//##############################################################################
// nDarray comparison                    // nDarray comparison
bool XC::nDarray::operator==(const nDarray &rval)  // returns 1 if they are same
  {                                       // returns 0 if they are not
    int true_or_not = 1; // suppose that they are the same

    double sqrt_d_macheps = sqrt(d_macheps());
//    double qub_d_macheps = pow(d_macheps(),(1.0/3.0));

    double tolerance = sqrt_d_macheps;

    int this_rank_of_nDarray = this->pc_nDarray_rep->nDarray_rank;
    int rval_rank_of_nDarray =  rval.pc_nDarray_rep->nDarray_rank;

    if(this_rank_of_nDarray != rval_rank_of_nDarray)
      {
        ::printf("\a\nnDarrays of different ranks: comparison not possible\n");
        ::exit ( 1 );
      }

    for ( int i=0 ; i<this_rank_of_nDarray ; i++ )
      if (this->pc_nDarray_rep->dim[i] != rval.pc_nDarray_rep->dim[i] )
        {
::fprintf(stderr,"\a\nDimension discrepancy in operator==\n\
          this->pc_nDarray_rep->dim[%d]=%d\n\
          arg.pc_nDarray_rep->dim[%d]=%d\n",
          i,this->pc_nDarray_rep->dim[i],
          i,rval.pc_nDarray_rep->dim[i]);
          ::exit(1);
        }
//..// construct XC::nDarray using the same control numbers as for the
//..// original one .
//..      nDarray add(pc_nDarray_rep->nDarray_rank, pc_nDarray_rep->dim, 0.0);

      switch(pc_nDarray_rep->nDarray_rank)
        {
          case 0:
            {
              if ( fabs( val(1)-rval.val(1) ) >= tolerance)
                true_or_not = 0;
              break;
            }

          case 1:
            {
              for ( int i1=1 ; i1<=this->pc_nDarray_rep->dim[0] ; i1++ )
                if ( fabs(  val(i1)-rval.val(i1) ) >= tolerance)
                  true_or_not = 0;
              break;
            }

          case 2:
            {
              for ( int i2=1 ; i2<=this->pc_nDarray_rep->dim[0] ; i2++ )
                for ( int j2=1 ; j2<=this->pc_nDarray_rep->dim[1] ; j2++ )
                  if ( fabs( val(i2,j2)-rval.val(i2,j2) ) >= tolerance)
                    true_or_not = 0;
              break;
            }

          case 3:
            {
              for ( int i3=1 ; i3<=this->pc_nDarray_rep->dim[0] ; i3++ )
                for ( int j3=1 ; j3<=this->pc_nDarray_rep->dim[1] ; j3++ )
                  for ( int k3=1 ; k3<=this->pc_nDarray_rep->dim[2] ; k3++ )
                    if (fabs(val(i3,j3,k3)-rval.val(i3,j3,k3))>=tolerance)
                      true_or_not = 0;
              break;
            }

          case 4:
            {
              for ( int i4=1 ; i4<=this->pc_nDarray_rep->dim[0] ; i4++ )
               for ( int j4=1 ; j4<=this->pc_nDarray_rep->dim[1] ; j4++ )
                for ( int k4=1 ; k4<=this->pc_nDarray_rep->dim[2] ; k4++ )
                 for ( int l4=1 ; l4<=this->pc_nDarray_rep->dim[3] ; l4++ )
                  if(fabs(val(i4,j4,k4,l4)-rval.val(i4,j4,k4,l4))>=tolerance)
                  true_or_not = 0;
              break;
            }
        }

    return true_or_not;
  }



//##############################################################################
// nDarray print function
void XC::nDarray::print(const std::string &name ,const std::string &msg) const
  {
    std::cerr << msg << std::endl;

    switch(pc_nDarray_rep->nDarray_rank)
      {
        case 0:
          {
            ::printf("%s(1)=%+8.4e  ", name.c_str(), cval(1));
            ::printf("\n");
            break;
          }

        case 1:
          {
            for ( int i=1 ; i<=pc_nDarray_rep->dim[0]; i++ )
              {
                ::printf("%s(%2d)=%+8.4e  ", name.c_str(), i, cval(i));
                ::printf("\n");
              }
            break;
          }

        case 2:
          {
            for ( int i2=1 ; i2<=pc_nDarray_rep->dim[0]  ; i2++ )
              {
                for ( int j2=1 ; j2<=pc_nDarray_rep->dim[1] ; j2++ )
                  {
                    ::printf("%s(%2d,%2d)=%+12.8e ", name.c_str(), i2, j2, cval(i2, j2));
                  }
                ::printf("\n");
              }
            break;
          }

        case 3:
          {
            for ( int i3=1 ; i3<=pc_nDarray_rep->dim[0] ; i3++ )
              for ( int j3=1 ; j3<=pc_nDarray_rep->dim[1] ; j3++ )
                {
                  for ( int k3=1 ; k3<=pc_nDarray_rep->dim[2] ; k3++ )
                    {
                      ::printf("%s(%d,%d,%d)=%+8.4e  ", name.c_str(), i3, j3, k3,
                        cval(i3, j3, k3));
                    }
                  ::printf("\n");
                }
            break;
          }

        case 4:
          {
            for ( int i4=1 ; i4<=pc_nDarray_rep->dim[0] ; i4++ )
              for ( int j4=1 ; j4<=pc_nDarray_rep->dim[1] ; j4++ )
                for ( int k4=1 ; k4<=pc_nDarray_rep->dim[2] ; k4++ )
                  {
                    for ( int l4=1 ; l4<=pc_nDarray_rep->dim[3] ; l4++ )
                      {
                        ::printf("%s(%d,%d,%d,%d)=%+8.4e  ",name.c_str(),i4,j4,k4,l4,
                          cval(i4, j4, k4, l4));
                      }
                    ::printf("\n");
                   }
            break;
          }
      }
////*    ::printf("\n");
//
  }

//##############################################################################
// nDarray print function
void XC::nDarray::printshort(const std::string &msg) const
  {
    std::cerr << msg << std::endl;

    switch(pc_nDarray_rep->nDarray_rank)
      {
        case 0:
          {
            ::printf("%+6.2e ",cval(1));
            ::printf("\n");
            break;
          }

        case 1:
          {
            for ( int i=1 ; i<=pc_nDarray_rep->dim[0]; i++ )
              {
                ::printf("%+6.2e ",cval(i));
                ::printf("\n");
              }
            break;
          }

        case 2:
          {
            for ( int i2=1 ; i2<=pc_nDarray_rep->dim[0]  ; i2++ )
              {
                for ( int j2=1 ; j2<=pc_nDarray_rep->dim[1] ; j2++ )
                  {
                    ::printf("%+6.2e ", cval(i2, j2));
                  }
                ::printf("\n");
              }
            break;
          }

        case 3:
          {
            for ( int i3=1 ; i3<=pc_nDarray_rep->dim[0] ; i3++ )
              for ( int j3=1 ; j3<=pc_nDarray_rep->dim[1] ; j3++ )
                {
                  for ( int k3=1 ; k3<=pc_nDarray_rep->dim[2] ; k3++ )
                    {
                      ::printf("%+6.2e  ", cval(i3, j3, k3));
                    }
                  ::printf("\n");
                }
            break;
          }

        case 4:
          {
            for ( int i4=1 ; i4<=pc_nDarray_rep->dim[0] ; i4++ )
              for ( int j4=1 ; j4<=pc_nDarray_rep->dim[1] ; j4++ )
                for ( int k4=1 ; k4<=pc_nDarray_rep->dim[2] ; k4++ )
                  {
                    for ( int l4=1 ; l4<=pc_nDarray_rep->dim[3] ; l4++ )
                      {
                        ::printf("%+6.2e  ", cval(i4, j4, k4, l4));
                      }
                    ::printf("\n");
                   }
            break;
          }
      }
////*    ::printf("\n");
//
  }

//##############################################################################
// nDarray print function for mathematica
void XC::nDarray::mathprint(void) const
  {

    switch(pc_nDarray_rep->nDarray_rank)
      {
        case 0:
          {
            ::printf("{");
            ::printf("%12f, ", cval(1));
            ::printf("},\n");
            break;
          }

        case 1:
          {
            ::printf("{");
            int i = 1;
            for ( i=1 ; i<=pc_nDarray_rep->dim[0]; i++ )
              {
                ::printf("%12f, ", cval(i));
                ::printf("\n");
                if (i<pc_nDarray_rep->dim[0]) ::printf(", ");
                if (i==pc_nDarray_rep->dim[0]) ::printf(" \n");
              }
            if (i<(pc_nDarray_rep->dim[0]+1)) ::printf("},\n");
            if (i==(pc_nDarray_rep->dim[0]+1)) ::printf("}\n");
            break;
          }

        case 2:
          {
            ::printf("{\n");
            for ( int i2=1 ; i2<=pc_nDarray_rep->dim[0]  ; i2++ )
              {
                if (pc_nDarray_rep->dim[1]!=1)
                  {
                    ::printf("{");
                  }
                for ( int j2=1 ; j2<=pc_nDarray_rep->dim[1] ; j2++ )
                  {
                    ::printf("%12f", cval(i2, j2));
                    if (j2<pc_nDarray_rep->dim[1]  )
                      {
                         ::printf(", ");
                      }
//                    if (j2==pc_nDarray_rep->dim[1]) ::printf("");
                  }
//                if (i2<pc_nDarray_rep->dim[1] && pc_nDarray_rep->dim[1]!=1 )
//                  {
//                    ::printf(",\n");
//                  }
                if ( pc_nDarray_rep->dim[1]==1 && i2<pc_nDarray_rep->dim[0] )
                  {
                         ::printf(", ");
                  }
                if (i2<pc_nDarray_rep->dim[1] )
                  {
                    ::printf("},\n");
                  }
                if (i2==pc_nDarray_rep->dim[1] && pc_nDarray_rep->dim[1]!=1 )
                  {
                    ::printf("}\n");
                  }
              }
//            if (i2<(pc_nDarray_rep->dim[0]+1)) ::printf("},\n");
//            if (i2==(pc_nDarray_rep->dim[0]+1))
//              {
                ::printf("}\n");
//              }
            break;
          }

        case 3:
          {
            for ( int i3=1 ; i3<=pc_nDarray_rep->dim[0] ; i3++ )
              for ( int j3=1 ; j3<=pc_nDarray_rep->dim[1] ; j3++ )
                {
                  for ( int k3=1 ; k3<=pc_nDarray_rep->dim[2] ; k3++ )
                    {
                      ::printf("%12f,  ", cval(i3, j3, k3));
                    }
                  ::printf("\n");
                }
            break;
          }

        case 4:
          {
            for ( int i4=1 ; i4<=pc_nDarray_rep->dim[0] ; i4++ )
              for ( int j4=1 ; j4<=pc_nDarray_rep->dim[1] ; j4++ )
                for ( int k4=1 ; k4<=pc_nDarray_rep->dim[2] ; k4++ )
                  {
                    for ( int l4=1 ; l4<=pc_nDarray_rep->dim[3] ; l4++ )
                      {
                        ::printf("%12f,  ", cval(i4, j4, k4, l4));
                      }
                    ::printf("\n");
                   }
            break;
          }
      }
////*    ::printf("\n");
//
  }


//##############################################################################
// nDarray Frobenius_norm function    // return the Frobenius norm of
double XC::nDarray::Frobenius_norm(void)  // BJmatrix, BJtensor, BJvector
  {                                   // see Dennis & Schnabel page 43
    double FN = 0.0;

    switch(pc_nDarray_rep->nDarray_rank)
      {
        case 0:
          {
            FN = (val(1))*(val(1));
            break;
          }

        case 1:
          {
            for ( int i=1 ; i<=pc_nDarray_rep->dim[0]; i++ )
              {
                FN = FN + (val(i))*(val(i));
              }
            break;
          }

        case 2:
          {
            for ( int i2=1 ; i2<=pc_nDarray_rep->dim[0]  ; i2++ )
              {
                for ( int j2=1 ; j2<=pc_nDarray_rep->dim[1] ; j2++ )
                  {
                    FN = FN + (val(i2,j2))*(val(i2,j2));
                  }
              }
            break;
          }

        case 3:
          {
            for ( int i3=1 ; i3<=pc_nDarray_rep->dim[0] ; i3++ )
              for ( int j3=1 ; j3<=pc_nDarray_rep->dim[1] ; j3++ )
                {
                  for ( int k3=1 ; k3<=pc_nDarray_rep->dim[2] ; k3++ )
                    {
                      FN = FN + (val(i3,j3,k3))*(val(i3,j3,k3));
                    }
                }
            break;
          }

        case 4:
          {
          for ( int i4=1 ; i4<=pc_nDarray_rep->dim[0] ; i4++ )
            for ( int j4=1 ; j4<=pc_nDarray_rep->dim[1] ; j4++ )
              for ( int k4=1 ; k4<=pc_nDarray_rep->dim[2] ; k4++ )
                {
                  for ( int l4=1 ; l4<=pc_nDarray_rep->dim[3] ; l4++ )
                    {
                      FN = FN + (val(i4, j4, k4, l4))*(val(i4, j4, k4, l4));
                    }
                 }
          break;
          }
      }

    return sqrt(FN);

  }

//##############################################################################
// nDarray General_norm function        // return the General p-th norm of
double XC::nDarray::General_norm(double p)  // BJmatrix, BJtensor, BJvector
  {                                     // see Dennis & Schnabel page 42
    double N = 0.0;

    switch(pc_nDarray_rep->nDarray_rank)
      {
        case 0:
          {
            N = pow(fabs(val(1)),p);
            break;
          }

        case 1:
          {
            for ( int i=1 ; i<=pc_nDarray_rep->dim[0]; i++ )
              {
                N = N + pow(fabs(val(i)),p);
              }
            break;
          }

        case 2:
          {
            for ( int i2=1 ; i2<=pc_nDarray_rep->dim[0]  ; i2++ )
              {
                for ( int j2=1 ; j2<=pc_nDarray_rep->dim[1] ; j2++ )
                  {
                    N = N + pow(fabs(val(i2,j2)),p);
                  }
              }
            break;
          }

        case 3:
          {
            for ( int i3=1 ; i3<=pc_nDarray_rep->dim[0] ; i3++ )
              for ( int j3=1 ; j3<=pc_nDarray_rep->dim[1] ; j3++ )
                {
                  for ( int k3=1 ; k3<=pc_nDarray_rep->dim[2] ; k3++ )
                    {
                      N = N + pow(fabs(val(i3,j3,k3)),p);
                    }
                }
            break;
          }

        case 4:
          {
          for ( int i4=1 ; i4<=pc_nDarray_rep->dim[0] ; i4++ )
            for ( int j4=1 ; j4<=pc_nDarray_rep->dim[1] ; j4++ )
              for ( int k4=1 ; k4<=pc_nDarray_rep->dim[2] ; k4++ )
                {
                  for ( int l4=1 ; l4<=pc_nDarray_rep->dim[3] ; l4++ )
                    {
                      N = N + pow(fabs(val(i4,j4,k4,l4)),p);
                    }
                 }
          break;
          }
      }

    return pow(N,(1.0/p));

  }



//##############################################################################
int XC::nDarray::number_of_zeros() const  // number of members that are
  {                                   // smaller than sqrt(macheps)
    int n = 0;
    double machepsilon = d_macheps();
    double tolerance   = sqrt(machepsilon);

    for (int j=0 ; j<this->pc_nDarray_rep->total_numb ; j++)
      if ( this->pc_nDarray_rep->pd_nDdata[j] <= tolerance )
        {
          n++;
        }

    return n;
  }


// prebacen u nDarray 14 oktobra 1996
//#############################################################################
XC::nDarray XC::nDarray::eigenvalues(void)
  {
    int rows = this->dim(1);
    int cols = this->dim(2);
    if ( rows != cols && this->rank() != 2 )
      {
        std::cerr << "rows!=cols in eigenvalues and rank != 2 \n";
        ::exit(1);
      }

 //   BJvector EV(rows, 0.0);
    const std::vector<int> pdim({rows});
    nDarray EV(1, pdim, 0.0);

    // most painless to really make a two dimensional string and copy it to 'a'
    // BEWARE they work in NRC as in FORTRAN therefore strings of 1 - n
    double ** a = new double *[rows+1];
    if ( !a ) {::printf("memory exhausted for **a \n"); ::exit(1);}
    for ( int i=0 ; i<(rows+1) ; i++ )
      {
        a[i] = new double[rows+1];
        if ( !a[i] ) {::printf("memory exhausted for *a \n"); ::exit(1);}
      }
    double * d = new double [rows+1];
    double * e = new double [rows+1];

    for ( int j=0 ; j<rows ; j++)
      for ( int k=0 ; k<rows ; k++)
        {
          a[j+1][k+1] = this->cval(j+1,k+1);
        }


// Householder reduction of a real symmetric XC::BJmatrix see NRC page 373
    tred2( a, rows, d, e);
// QL algorithm with implicit shift see NRC page 380
    tqli( d, e, rows, a);
// sort eigenvalues see NRC page 366
    eigsrt( d, a, rows);

    for ( int l=0 ; l<rows ; l++ )
      {
        EV.val(l+1) = d[l+1];
      }

// delete new - ed arrays
//    for ( int i1=0 ; i1<rows ; i1++ )
//      {
//        delete a[i1];
//      }
    for ( int ii=0 ; ii<(rows+1) ; ii++ )
      {
        delete [] a[ii];
      }
    delete [] a;
    delete [] d;
    delete [] e;

    return EV;
  }

//#############################################################################
XC::nDarray XC::nDarray::eigenvectors(void)
  {
    int rows = this->dim(1);
    int cols = this->dim(2);
    if ( rows != cols && this->rank() != 2 )
      {
        ::printf("rows!=cols in eigenBJvectors and rank != 2 \n");
        ::exit(1);
      }

//    BJmatrix EV(rows, rows, 0.0);
    const std::vector<int> pdim({rows, rows});
    nDarray  EV(2, pdim, 0.0);
//    BJmatrix temp( rows, rows, rows, this->data() );

// most painless to really make a two dimensional string and copy it to 'a'
// BEWARE they work in NRC as in FORTRAN therefore strings of 1 - n
    double ** a = new double *[rows+1];
    if ( !a ) {::printf("memory exhausted for **a \n"); ::exit(1);}
    for ( int i=0 ; i<(rows+1) ; i++ )
      {
        a[i] = new double[rows+1];
        if ( !a[i] ) {::printf("memory exhausted for *a \n"); ::exit(1);}
      }
    double * d = new double [rows+1];
    double * e = new double [rows+1];

    for ( int j=0 ; j<rows ; j++)
      for ( int k=0 ; k<rows ; k++)
        {
          a[j+1][k+1] = this->cval(j+1,k+1);
        }
// Householder reduction of a real symmetric XC::BJmatrix see NRC page 373
    tred2( a, rows, d, e);
// QL algorithm with implicit shift see NRC page 380
    tqli( d, e, rows, a);
// sort eigenvalues see NRC page 366
    eigsrt( d, a, rows);

    for ( int l=0 ; l<rows ; l++ )
      for ( int l1=0; l1<rows ; l1++ )
        {
          EV.val(l+1,l1+1) = a[l+1][l1+1];
        }

// delete new - ed arrays
    for ( int i1=0 ; i1<(rows+1) ; i1++ )
      {
        delete [] a[i1];
      }
    delete [] a;
    delete [] d;
    delete [] e;

    return EV;
  }


//##############################################################################
XC::nDarray XC::nDarray::nDsqrt(void)  // returns all elements square root
  {
    nDarray newnD( this->rank(), this->pc_nDarray_rep->dim, this->data());

    for ( int i=0 ; i<pc_nDarray_rep->total_numb ; i++ )
      newnD.pc_nDarray_rep->pd_nDdata[i] = sqrt(this->pc_nDarray_rep->pd_nDdata[i]) ;

    return newnD;

  }




// prebacen u XC::nDarray 14 oktobra 1996
//#############################################################################
#define SIGN(a,b) ((b)<0 ? -fabs(a) : fabs(a))

void XC::nDarray::tqli(double * d, double * e, int n, double ** z)
{
  int m,l,iter,i,k;
  double s,r,p,g,f,dd,c,b;
//      void nrerror();

  for (i=2;i<=n;i++) e[i-1]=e[i];
  e[n]=0.0;
  for (l=1;l<=n;l++) {
    iter=0;
    do {
      for (m=l;m<=n-1;m++) {
        dd=fabs(d[m])+fabs(d[m+1]);
        if (fabs(e[m])+dd == dd) break;
      }
      if (m != l) {
        if (iter++ == 30) { ::printf("Too many iterations in TQLI\n"); ::exit(1); }
        g=(d[l+1]-d[l])/(2.0*e[l]);
        r=sqrt((g*g)+1.0);
        g=d[m]-d[l]+e[l]/(g+SIGN(r,g));
        s=c=1.0;
        p=0.0;
        for (i=m-1;i>=l;i--) {
          f=s*e[i];
          b=c*e[i];
          if (fabs(f) >= fabs(g)) {
            c=g/f;
            r=sqrt((c*c)+1.0);
            e[i+1]=f*r;
            c *= (s=1.0/r);
          } else {
            s=f/g;
            r=sqrt((s*s)+1.0);
            e[i+1]=g*r;
            s *= (c=1.0/r);
          }
          g=d[i+1]-p;
          r=(d[i]-g)*s+2.0*c*b;
          p=s*r;
          d[i+1]=g+p;
          g=c*r-b;
          /* Next loop can be omitted if eigenBJvectors not wanted */
          for (k=1;k<=n;k++) {
            f=z[k][i+1];
            z[k][i+1]=s*z[k][i]+c*f;
            z[k][i]=c*z[k][i]-s*f;
          }
        }
        d[l]=d[l]-p;
        e[l]=g;
        e[m]=0.0;
      }
    } while (m != l);
  }
}


//#############################################################################
void XC::nDarray::tred2(double ** a, int n, double * d, double * e)
{
  int l,k,j,i;
  double scale,hh,h,g,f;

  for (i=n;i>=2;i--) {
    l=i-1;
    h=scale=0.0;
    if (l > 1) {
      for (k=1;k<=l;k++)
        scale += fabs(a[i][k]);
      if (scale == 0.0)
        e[i]=a[i][l];
      else {
        for (k=1;k<=l;k++) {
          a[i][k] /= scale;
          h += a[i][k]*a[i][k];
        }
        f=a[i][l];
        g = f>0 ? -sqrt(h) : sqrt(h);
        e[i]=scale*g;
        h -= f*g;
        a[i][l]=f-g;
        f=0.0;
        for (j=1;j<=l;j++) {
        /* Next statement can be omitted if eigenBJvectors not wanted */
          a[j][i]=a[i][j]/h;
          g=0.0;
          for (k=1;k<=j;k++)
            g += a[j][k]*a[i][k];
          for (k=j+1;k<=l;k++)
            g += a[k][j]*a[i][k];
          e[j]=g/h;
          f += e[j]*a[i][j];
        }
        hh=f/(h+h);
        for (j=1;j<=l;j++) {
          f=a[i][j];
          e[j]=g=e[j]-hh*f;
          for (k=1;k<=j;k++)
            a[j][k] -= (f*e[k]+g*a[i][k]);
        }
      }
    } else
      e[i]=a[i][l];
    d[i]=h;
  }
  /* Next statement can be omitted if eigenBJvectors not wanted */
  d[1]=0.0;
  e[1]=0.0;
  /* Contents of this loop can be omitted if eigenBJvectors not
      wanted except for statement d[i]=a[i][i]; */
  for (i=1;i<=n;i++) {
    l=i-1;
    if (d[i]) {
      for (j=1;j<=l;j++) {
        g=0.0;
        for (k=1;k<=l;k++)
          g += a[i][k]*a[k][j];
        for (k=1;k<=l;k++)
          a[k][j] -= g*a[k][i];
      }
    }
    d[i]=a[i][i];
    a[i][i]=1.0;
    for (j=1;j<=l;j++) a[j][i]=a[i][j]=0.0;
  }
}



//#############################################################################
void XC::nDarray::eigsrt(double * d, double ** v, int n)
{
  int k,j,i;
  double p;

  for (i=1;i<n;i++) {
    p=d[k=i];
    for (j=i+1;j<=n;j++)
      if (d[j] >= p) p=d[k=j];
    if (k != i) {
      d[k]=d[i];
      d[i]=p;
      for (j=1;j<=n;j++) {
        p=v[j][i];
        v[j][i]=v[j][k];
        v[j][k]=p;
      }
    }
  }
}





//##############################################################################
// very private part
//##############################################################################
   double * XC::nDarray::data(void) const
    {
      return this->pc_nDarray_rep->pd_nDdata;
    }

    void XC::nDarray::set_data_pointer(double * data_pointer)
    {
      this->pc_nDarray_rep->pd_nDdata = data_pointer;
    }

    int XC::nDarray::rank(void) const
    {
      return this->pc_nDarray_rep->nDarray_rank;
    }

    void XC::nDarray::rank(int nDarray_rank)
    {
      this->pc_nDarray_rep->nDarray_rank = nDarray_rank;
    }

    long int XC::nDarray::total_number(void) const
      { return this->pc_nDarray_rep->total_numb; }

    void XC::nDarray::total_number(long int number)
      { this->pc_nDarray_rep->total_numb = number; }

    const std::vector<int> &XC::nDarray::dim(void) const
      { return this->pc_nDarray_rep->dim; }

    void XC::nDarray::clear_dim(void)
      { return this->pc_nDarray_rep->clear_dim(); }

    int &XC::nDarray::get_dim_pointer(void) const
      { return this->pc_nDarray_rep->dim[0]; }

    void XC::nDarray::set_dim(const std::vector<int> &pdim)
      { this->pc_nDarray_rep->init_dim(pdim.size(), pdim); }

    int XC::nDarray::dim(int which) const
      { return this->pc_nDarray_rep->dim[which-1]; }

    int XC::nDarray::reference_count(int up_down)
      {
	this->pc_nDarray_rep->n += up_down;
        return(this->pc_nDarray_rep->n);
      }

    void XC::nDarray::set_reference_count(int ref_count)
      { this->pc_nDarray_rep->n=ref_count; }


//tempOUT//##############################################################################
// TENSOR_REP_CC
// #######################
// memory manager part
// overloading operator new in XC::nDarray::nDarray_rep class  ##################
void * XC::nDarray_rep::operator new(size_t s)
  {                                       // see C++ reference manual by
    void *void_pointer;                   // ELLIS and STROUSTRUP page 283.
    void_pointer = ::operator new(s);     // and ECKEL page 529.
//    ::printf("\nnew pointer %p of size %d\n",void_pointer,s);
    if (!void_pointer)
      {
        ::fprintf(stderr,"\a\nInsufficient memory\n");
        ::exit(1);
      }
    return void_pointer;
  }

// overloading operator delete in XC::nDarray::nDarray_rep class  ##################
void XC::nDarray_rep::operator delete(void *p)
  {                                       // see C++ reference manual by
                                          // ELLIS and STROUSTRUP page 283.
                                          // and ECKEL page 529.
//    ::printf("deleted pointer %p\n",p);
    ::operator delete(p);
  }




#endif


