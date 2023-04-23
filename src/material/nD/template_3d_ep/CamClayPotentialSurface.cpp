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
//=============================================================================
//# COPYRIGHT (C):     :-))                                                   #
//# PROJECT:           Object Oriented Finite Element Program                 #
//# PURPOSE:           Cam Clay potential criterion                           #
//#                      (Ref. Wood p113)                                     #
//# CLASS:             CamClayPotentialSurface                                    #
//#                                                                           #
//# VERSION:                                                                  #
//# LANGUAGE:          C++.ver >= 2.0 ( Borland C++ ver=3.00, SUN C++ ver=2.1)#
//# TARGET OS:         DOS || UNIX || . . .                                   #
//# PROGRAMMER(S):     Boris Jeremic, ZHaohui Yang                            #
//#                                                                           #
//#                                                                           #
//# DATE:              Mar. 28, 2001                                          #
//# UPDATE HISTORY:                                                           #
//#                                                                           #
//#                                                                           #
//#                                                                           #
//#                                                                           #
//#                                                                           #
//=============================================================================

#ifndef CamClayPotentialSurface_CPP
#define CamClayPotentialSurface_CPP

#include "CamClayPotentialSurface.h"
#include <utility/matrix/nDarray/basics.h>
#include <utility/matrix/nDarray/BJtensor.h>
#include "material/nD/template_3d_ep/EPState.h"


//@ brief Constructor.
XC::CamClayPotentialSurface::CamClayPotentialSurface(double Mp )  
  { M = Mp; } 


//================================================================================
//create a clone of itself
//================================================================================

XC::PotentialSurface *XC::CamClayPotentialSurface::getCopy(void) const
  { return new CamClayPotentialSurface(*this); }

//================================================================================
// Copy constrstructor
//================================================================================
//
//CamClayPotentialSurface::CamClayPotentialSurface(CamClayPotentialSurface &CamClayYS ) { }
//

//! @brief tensor dQ/dsigma_ij  the normal to the potential surface
XC::BJtensor XC::CamClayPotentialSurface::dQods(const EPState *EPS) const
  {
    BJtensor dQoverds( def_dim_2, 0.0);
    BJtensor I2("I", def_dim_2);

    double p = EPS->getStress().p_hydrostatic();
    double q = EPS->getStress().q_deviatoric();
    double po = EPS->getScalarVar( 1 );
    BJtensor DpoDs = EPS->getStress().dpoverds();
    BJtensor DqoDs = EPS->getStress().dqoverds();

    double dQoverdp = -1.0*M*M*( po - 2.0*p );
    double dQoverdq = 2.0*q;

    dQoverds = DpoDs  * dQoverdp +
	       DqoDs  * dQoverdq;
    return dQoverds;
  }

//================================================================================
// tensor d2Q/ds2  the normal to the potential surface
//================================================================================

XC::BJtensor XC::CamClayPotentialSurface::d2Qods2(const EPState *EPS) const 
{  
  BJtensor d2Qoverds2(def_dim_4, 0.0);
  return d2Qoverds2;
}


// For Consistent Algorithm, Z Cheng, Jan 2004
XC::BJtensor XC::CamClayPotentialSurface::d2Qodsds1(const EPState *EPS) const 
{  
  BJtensor d2Qoverdsds1(def_dim_2, 0.0);
  BJtensor DpoDs = EPS->getStress().dpoverds();
  double D2QoDpDs1 = -M*M;
  d2Qoverdsds1 = DpoDs * D2QoDpDs1;
  return d2Qoverdsds1;
}
       
// moved to stresstensor Boris Jeremic@ucdavis.edu 21Aug2001
// // I took these functions from mmodel.cpp, programmed by Dr. Jeremic
// //#############################################################################
// BJtensor CamClayPotentialSurface::dpoverds( ) const
//   {
//     BJtensor ret(def_dim_2, 0.0);
//     BJtensor I2("I", def_dim_2);
//     ret = I2*(-1.0/3.0);
//     ret.null_indices();
//     return ret;
//   }
// 
// //#############################################################################
// BJtensor CamClayPotentialSurface::dqoverds(const EPState *EPS) const
//   {
//     
//     stresstensor stress = EPS->getStress();
//     
//     BJtensor ret(def_dim_2, 0.0);
//     double q = stress.q_deviatoric();
//     stresstensor s( 0.0);
// //...    double J2D = stress.Jinvariant2();
// //...    double temp1 = sqrt(J2D);
// //...    double temp2 = sqrt(3.0)/(2.0*temp1);
//     double temp2 = (3.0/2.0)*(1/q);
//     s = stress.deviator();
//     ret = s*temp2;
//     ret.null_indices();
//     return ret;
//   }
// 
// //#############################################################################
// BJtensor CamClayPotentialSurface::dthetaoverds(const EPState *EPS) const
//   {
//     stresstensor stress = EPS->getStress();
// 
//     BJtensor ret(def_dim_2, 0.0);
//     stressBJtensor s( 0.0);
//     stresstensor t( 0.0);
//     BJtensor I2("I", def_dim_2);
// 
// //    double EPS = pow(d_macheps(),(1./3.));
//     double J2D = stress.Jinvariant2();
//     double q     = stress.q_deviatoric();
//     double theta = stress.theta();
// 
// //out    while ( theta >= 2.0*PI )
// //out      theta = theta - 2.0*PI; // if bigger than full cycle
// //out    while ( theta >= 4.0*PI/3.0 )
// //out      theta = theta - 4.0*PI/3.0; // if bigger than four thirds of half cycle
// //out    while ( theta >= 2.0*PI/3.0 )
// //out      theta = theta - 2.0*PI/3.0; // if bigger than two third of half cycle
// //out    while ( theta >= PI/3.0 )
// //out      theta = 2.0*PI/3.0 - theta; // if bigger than one third of half cycle
// //out
// //out    if ( theta < 0.0001 )
// //out      {
// //out        ::printf("theta = %.10e in CamClayPotentialSurface::dthetaoverds(stress)\n",
// //out                           theta);
// //out//>><<>><<>><<        return ret;
// //out      }
// 
//     double c3t = cos(3.0*theta);
//     double s3t = sin(3.0*theta);
// 
//     double tempS = (3.0/2.0)*(c3t/(q*q*s3t));
//     double tempT = (9.0/2.0)*(1.0/(q*q*q*s3t));
// 
//     s = stress.deviator();
//     t = s("qk")*s("kp") - I2*(J2D*(2.0/3.0));
// 
//     s.null_indices();
//     t.null_indices();
// 
//     ret = s*tempS - t*tempT;
//     ret.null_indices();
//     return ret;
//   }
// 
// //#############################################################################
// //!..........................................................................
// //!  BJtensor d2poverds2( def_dim_4, 0.0); //second derivative of p over
// //!                                         // d sigma_pq  d sigma_mn
// //!  d2poverds2 = 0.0; //IDENTICALLY EQUAL TO ZERO
// //!..........................................................................
// //#############################################################################
// BJtensor CamClayPotentialSurface::d2poverds2( void ) const
//   {
//     BJtensor ret(def_dim_4, 0.0);
//     return ret;
//     //!!!!! this one is equivalent to zero at all times so no need to call it !!!
//   }
// 
// 
// //#############################################################################
// BJtensor CamClayPotentialSurface::d2qoverds2(const EPState *EPS) const
//   {
//     stresstensor stress = EPS->getStress();
// 
// 
//     //first the return tensor in order not to fragment the memory
//     BJtensor ret( def_dim_4, 0.0); //  second derivative of q over
//                                     //  d sigma_pq  d sigma_mn
//     //setting up some constants
// 
//     BJtensor I2("I", def_dim_2);      // To check out this three fourth-order
//     BJtensor I_pqmn("I", def_dim_4);  // isotropic tensor please see
//     I_pqmn = I2("pq")*I2("mn");        // W.Michael Lai, David Rubin,
//     I_pqmn.null_indices();
//     BJtensor I_pmqn("I", def_dim_4);  // Erhard Krempl
//     I_pmqn = I_pqmn.transpose0110();   // " Introduction to Continuum Mechanics"
//                                        // QA808.2  ;   ISBN 0-08-022699-X
// 
//     double q_dev   = stress.q_deviatoric();
// 
//     stresstensor s = stress.deviator();
//     s.null_indices();
// 
//     BJtensor iso1( def_dim_4, 0.0); //this will be d_pm*d_nq-d_pq*d_nm*(1/3)
//     iso1 = I_pmqn  - I_pqmn*(1.0/3.0);
// 
//     double tempiso1  = (3.0/2.0)*(1/q_dev);
//     double tempss    = (9.0/4.0)*(1.0/( q_dev * q_dev * q_dev ));
// 
//     ret = iso1*tempiso1 - (s("pq")*s("mn"))*tempss;
//     ret.null_indices();
//     return ret;
//   }
// 
// 
// //#############################################################################
// BJtensor CamClayPotentialSurface::d2thetaoverds2(const EPState *EPS) const
//   {
//     stresstensor stress = EPS->getStress();
// 
//     BJtensor ret( def_dim_4, 0.0);
// 
//     BJtensor I2("I", def_dim_2);
//     BJtensor I_pqmn("I", def_dim_4);  // To check out this three fourth-order
//     I_pqmn = I2("pq")*I2("mn");        // isotropic tensor please see
//     I_pqmn.null_indices();
//     BJtensor I_pmqn("I", def_dim_4);  // W.Michael Lai, David Rubin,
//     I_pmqn = I_pqmn.transpose0110();   // Erhard Krempl
// //no    BJtensor I_pnqm("I", def_dim_4);  // " Introduction to Continuum Mechanics"
// //no    I_pnqm = I_pqmn.transpose0111();   // QA808.2  ;   ISBN 0-08-022699-X
// 
//     double J2D = stress.Jinvariant2();
// 
// //    double EPS = pow(d_macheps(),(1./3.));
// 
//     BJtensor s( def_dim_2, 0.0);
//     BJtensor t( def_dim_2, 0.0);
//     s = stress.deviator();
//     t = s("qk")*s("kp") - I2*(J2D*(2.0/3.0));
// //s.print("s"," \n\n tensor s \n\n");
// //t.print("t"," \n\n tensor t \n\n");
//     s.null_indices();
//     t.null_indices();
// 
//     BJtensor p( def_dim_4, 0.0); //this will be d_mp*d_nq - d_pq*d_nm*(1/3)
//     BJtensor w( def_dim_4, 0.0);
// 
//     double theta = stress.theta();
// //out    while ( theta >= 2.0*PI )
// //out      theta = theta - 2.0*PI; // if bigger than full cycle
// //out    while ( theta >= 4.0*PI/3.0 )
// //out      theta = theta - 4.0*PI/3.0; // if bigger than four thirds of half cycle
// //out    while ( theta >= 2.0*PI/3.0 )
// //out      theta = theta - 2.0*PI/3.0; // if bigger than two third of half cycle
// //out    while ( theta >= PI/3.0 )
// //out      theta = 2.0*PI/3.0 - theta; // if bigger than one third of half cycle
// //out
// //out    if ( theta < 0.0001 )
// //out      {
// //out        ::printf("\n\ntheta = %.10e in CamClayPotentialSurface::d2thetaoverds2(stress)\n",
// //out                           theta);
// //out//>><<>><<>><<        return ret;
// //out      }
// //out
// 
//     double q_dev = stress.q_deviatoric();
// 
//   //setting up some constants
// //......    double sqrt3 = sqrt(3.0);
//     double c3t    = cos(3*theta);
//     double s3t    = sin(3*theta);
//     double s3t3   = s3t*s3t*s3t;
//     double q3     = q_dev * q_dev * q_dev;
//     double q4     = q_dev * q_dev * q_dev * q_dev;
//     double q5     = q_dev * q_dev * q_dev * q_dev * q_dev;
//     double q6     = q_dev * q_dev * q_dev * q_dev * q_dev * q_dev;
// 
// 
//     double tempss = - (9.0/2.0)*(c3t)/(q4*s3t) - (27.0/4.0)*(c3t/(s3t3*q4));
// 
//     double tempst = (81.0/4.0)*(1.0)/(s3t3*q5);
// 
//     double tempts = (81.0/4.0)*(1.0/(s3t*q5)) + (81.0/4.0)*(c3t*c3t)/(s3t3*q5);
// 
//     double temptt = - (243.0/4.0)*(c3t/(s3t3*q6));
// 
//     double tempp  = +(3.0/2.0)*(c3t/(s3t*q_dev*q_dev));
// 
//     double tempw  = -(9.0/2.0)*(1.0/(s3t*q3));
// 
// 
// //))))))))
// //))))))))    double tempss = 0.0;
// //))))))))
// //))))))))    double tempst = 0.0;
// //))))))))
// //))))))))    double tempts = 0.0;
// //))))))))
// //))))))))    double temptt = 0.0;
// //))))))))
// //))))))))    double tempp  = 0.0;
// //))))))))
// //))))))))    double tempw  = 0.0;
// //))))))))
// 
// 
// // fourth order tensors in the final equation for second derivative
// // of theta over ( d \sigma_{pq} d \sigma_{mn} )
// // BE CAREFUL order is   PQ MN
// 
//     BJtensor s_pq_d_mn( def_dim_4, 0.0);
//     BJtensor s_pn_d_mq( def_dim_4, 0.0);
// //...    BJtensor s_pm_d_nq( def_dim_4, 0.0);
// 
//     BJtensor d_pq_s_mn( def_dim_4, 0.0);
//     BJtensor d_pn_s_mq( def_dim_4, 0.0);
// //...    BJtensor d_pm_s_nq( def_dim_4, 0.0);
// 
//     p = I_pmqn  - I_pqmn*(1.0/3.0);
// 
// // privremena stampa
// //..........................................................................
// //.......p.print("p"," tensor p = I_pmqn  - I_pqmn*(1.0/3.0)");
// 
// 
//     s_pq_d_mn = s("pq")*I2("mn");
//     s_pq_d_mn.null_indices();
//     s_pn_d_mq = s_pq_d_mn.transpose0101();
// //...    s_pm_d_nq = s_pn_d_mq.transpose0110();
// 
//     d_pq_s_mn = I2("pq")*s("mn");
//     d_pq_s_mn.null_indices();
//     d_pn_s_mq = d_pq_s_mn.transpose0101();
// //...    d_pm_s_nq = d_pn_s_mq.transpose0110();
// 
// //// privremena stampa
// ////..........................................................................
// //s_pq_d_mn.print("sd"," tensor s_pq_d_mn = s(\"pq\")*I2(\"mn\") ");
// //s_pn_d_mq.print("sd"," tensor s_pn_d_mq = s_pq_d_mn.transpose0101()");
// ////s_pm_d_nq.print("sd"," tensor s_pm_d_nq = s_pn_d_mq.transpose0110()");
// //d_pq_s_mn.print("ds"," tensor d_pq_s_mn = I2(\"pq\")*s(\"mn\") ");
// //d_pn_s_mq.print("ds"," tensor d_pn_s_mq = d_pq_s_mn.transpose0101()");
// ////d_pm_s_nq.print("ds"," tensor d_pm_s_nq = d_pn_s_mq.transpose0110()");
// ////..........................................................................
// 
//     w =  s_pn_d_mq + d_pn_s_mq - s_pq_d_mn*(2.0/3.0) - d_pq_s_mn*(2.0/3.0);
// 
// //.....// symmetric w
// //.....    w = ( s_pn_d_mq + s_pm_d_nq ) * 0.5 +
// //.....        ( d_pn_s_mq + d_pm_s_nq ) * 0.5
// //.....        - s_pq_d_mn*(2.0/3.0) - d_pq_s_mn*(2.0/3.0);
// //.....
// 
// //// privremena stampa
// ////..........................................................................
// //w.print("w","w=s_pn_d_mq+d_pn_s_mq-s_pq_d_mn*(2.0/3.0)-d_pq_s_mn*(2.0/3.0)");
// ////..........................................................................
// 
// ////...// privremena stampa
// ////...//..........................................................................
// //tensor ss = s("pq")*s("mn");
// //tensor st = s("pq")*t("mn");
// //tensor ts = t("pq")*s("mn");
// //tensor tt = t("pq")*t("mn");
// //ss.print("ss","\n tensor ss ");
// //st.print("st","\n tensor st ");
// //ts.print("ts","\n tensor ts ");
// //tt.print("tt","\n tensor tt ");
// ////...
// 
// // finally
//     ret = (   s("pq")*s("mn") * tempss
//             + s("pq")*t("mn") * tempst
//             + t("pq")*s("mn") * tempts
//             + t("pq")*t("mn") * temptt
//             + p               * tempp
//             + w               * tempw );
//           
//     ret.null_indices();
//     return ret;
//   }

//================================================================================
double XC::CamClayPotentialSurface::getM() const
  { return M; }

void XC::CamClayPotentialSurface::setM(const double &d)
  { M= d; }

void XC::CamClayPotentialSurface::print(std::ostream &os) const
  {
    os << "Cam Clay Potential Surface Parameters: " << std::endl;
    os << "M = " << getM() << std::endl;
  }

#endif

