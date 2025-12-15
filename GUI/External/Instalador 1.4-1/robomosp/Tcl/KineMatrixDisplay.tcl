# --------------------------------------------------------------------------------------------------------------------------
#  KineMatrixDisplay.tcl
#		This file implements the ROBOMOSP kine matrix display window.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-06-06
#
# Requirements:
#
#SCCS: %Z% %M% %I% %E% %U%
# --------------------------------------------------------------------------------------------------------------------------


# Initialization of Tk objects
# Initialization of images
image create photo imgJCTBGlobal -file ./Images/MatrixDisplayToolBar/Global.gif
image create photo imgJCTBLocal -file ./Images/MatrixDisplayToolBar/Local.gif

# --------------------------------------------------------------------------------------------------------------------------

namespace eval KineMatrixDisplay {

  # Initialization of global variables
  variable tplKineM .wndKineMatrixDisplay
  variable KineMTypeLocal Local
  variable KineMTypeGlobal Global
  variable KineMType $KineMTypeGlobal
  variable KineMatrixDisplayInit 0
  
  
  # Delete procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
	variable tplKineM
	variable KineMatrixDisplayInit
	set KineMatrixDisplayInit 0

	wm withdraw $tplKineM
  }

  # Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize { { PosOrient 0 } { Vel 0 }  { Accel 0 }} {
    variable tplKineM
    variable frmKineMTB
    set CellWidth 5
	variable KineMatrixDisplayInit
	set KineMatrixDisplayInit 1      
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
