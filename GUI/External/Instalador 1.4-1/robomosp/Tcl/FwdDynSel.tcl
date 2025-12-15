#----------------------------------------------------------------------------------------------------------------------------------
# FwdDynSel.tcl
# 		This file implements the TCL code for creating and managing the Forward Dynamics
#		CheckList Window for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-06-09
#
# Requirements:   RobLab.tcl
#			              Menu.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval FwdDynSel {

  
  #  Set  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Set {} {
 
        switch $RobLab::IntegratorType {   
               Ode45 {
                      FwdDynToolBar::Initialize
               }
               Verlet {
                      VerletFwdDynToolBar::Initialize
               }
         }             
   }
  
}  

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
