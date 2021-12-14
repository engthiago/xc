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

//================================================================================
// COPYRIGHT (C):     :-))                                                       |
// PROJECT:           Object Oriented Finite Element Program                     |
// PURPOSE:           General platform for elaso-plastic constitutive model      |
//                    implementation                                             |
//                                                                               |
// CLASS:             EvolutionLaw_NL_EijMD (nonlinear tensorial Evolution law)  |
//                                                                               |
//                                                                               |
// VERSION:                                                                      |
// LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1 )   |
// TARGET OS:         DOS || UNIX || . . .                                       |
// DESIGNER(S):       Boris Jeremic, Zhaohui Yang                                |
// PROGRAMMER(S):     Boris Jeremic, Zhaohui Yang                                |
//                                                                               |
//                                                                               |
// DATE:              09-13-2000                                                 |
// UPDATE HISTORY:                                                               |
//                                                                               |
//                                                                               |
//                                                                               |
//                                                                               |
//================================================================================

#ifndef EvolutionLaw_NL_EIJManzariDafalias_H
#define EvolutionLaw_NL_EIJManzariDafalias_H

#include "material/nD/Template3Dep/EvolutionLaw_T.h"
#include "utility/matrix/nDarray/stresst.h"


namespace XC {

//! @ingroup MatNDEL
//!
//! @brief Nonlinear tensorial Evolution law.
class EvolutionLaw_NL_EijMD: public EvolutionLaw_T
  {
  // Private vars to define the MD evolution law
  private:			      
    // the exponential in G = Go (P/P_at)^a and K = Ko (P/P_at)^a
    double eo; //Initial void ratio  
    double a; 		       

    //Critical state parameters
    double Mc;
    double Me;
    double Lambda; // slope of e vs. ln p 
    double ec_ref; // critical void ratio at reference mean effective stress p  
    double p_ref;  // critical void ratio at reference mean effective stress p  

    //surface evolution parameters
    double kc_b;  // b stands for bounding surface
    double kc_d;  // d stands for dilatancy surface
    double ke_b;  // e extension
    double ke_d;  // c compression

    //Hardening parameters
    //double h; // Being calculated using ho and b_ij * n_ij
    double ho;
    double Cm;
    //double e;     // current void ratio ...Moved to EPState Joey 02-12-03

    //Dilatancy parameter
    double D; //also copied to EPS's second scalar var(no direct contribution to hardening)
    double Ao;

    //Get rid of the fabric tensor
    //Fabric parameters
    stresstensor F;   //Fabric tensor which will evolve like alpha_{ij}
    double Fmax;
    double Cf;
    
  public:
    //EvolutionLaw_L_EijMD( );    // default constructor---no parameters
    
    // default constructor
    EvolutionLaw_NL_EijMD( 
                           double eod ,
    			   double ad  ,    
    			   double Mcd ,//1.14, 
    			   double Med ,//1.14, 
    			   double Lambdad ,
    			   double ec_refd , 
    			   double p_refd , 
    			   double kc_bd , 
    			   double kc_dd , 
    			   double ke_bd , 
    			   double ke_dd ,  
    			   double hod ,	 
    			   double Cmd ,
    			   double Aod ,    
    			   double Fmaxd ,   
    			   double Cfd );
    			   //double ed  = 0.85,    
                           
    // Copy constructor
    EvolutionLaw_NL_EijMD(const EvolutionLaw_NL_EijMD &LEL );   
    
    //create a clone of itself
    EvolutionLaw_T *getCopy(void);
    	  
    //double h( EPState *EPS,  double norm_dQods);     // Evaluating hardening function h
    BJtensor h_t( EPState *EPS, PotentialSurface *PS);    // Evaluating hardening function h
    
    //Overwrite  updateEeDm
    //Updating  E, e and D value according to current mean effective stress p and elastic strain
    int updateEeDm(EPState *EPS, double st_vol, double dLamda);
       
    //void UpdateVar( EPState *EPS, double dlamda );  // Evolve corresponding var linearly using de_eq_p
    //Moved to CDriver.cpp

    void print();
    //g++ complaining if don't have this line
    virtual ~EvolutionLaw_NL_EijMD() {}

  private:
    // some accessor functions
		         
    // set D once current stress hits the y.s. or
    // Set D value according to current EPState
    //void setD(EPState *EPS);   

    double geta() const;
    double getMc() const;
    double getMe() const;
    double getLambda() const;
    double getec_ref() const;
    double getp_ref() const; 

    double getkc_b() const;  
    double getkc_d() const;  
    double getke_b() const;  
    double getke_d() const;  
    double getho() const;
    double getCm() const;
    double geteo() const;
    //double gete() const;

    //Dilatancy parameter
    double getAo() const;
    double getD() const;

    double getFmax() const;
    const stresstensor &getF(void) const;
    double getCf() const;

    // Interpolation function  by Agyris
    double g_A(double theta, double e);	 

    // Interpolation function  by Willan-Warkne
    double g_WW(double theta, double e); 
	       
    //================================================================================
    // Overloaded Insertion Operator	  Zhaohui Added Aug. 13, 2000
    // prints Linear EvolutionLaw's contents 
    //================================================================================
    friend std::ostream& operator<<(std::ostream &, const EvolutionLaw_NL_EijMD &);
  };
std::ostream& operator<<(std::ostream &, const EvolutionLaw_NL_EijMD &);
} // end of XC namespace


#endif




