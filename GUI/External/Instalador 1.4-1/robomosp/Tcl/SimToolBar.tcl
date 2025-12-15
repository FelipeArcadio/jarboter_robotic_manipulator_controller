#----------------------------------------------------------------------------------------------------------------------------------
# SimToolBart.tcl
# 		This file implements the TCL code for creating and managing the 3D Simulation
#		ToolBar for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-03-09
#
# Requirements:   RobLab.tcl
#			  Menu.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  # Images of object navigator
  image create photo imgCVSIcon35 -file ./Images/ObjectNavigator/Icon35.gif

  # Images of GNUPlot canvas
  image create photo imgGPTBButton01 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton02 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton03 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton04 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton05 -file ./Images/GNUPlotToolBar/Icon35.gif

# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval SimToolBar {

  # Initialization of global variables
  variable tplSM .wndSimToolBar                   #Main 3D Simulation Window.
  variable frmKineSM $tplSM.frmKineSM        #Kinematics Simulation frame.
  variable SimToolBarInit 0

  #  Delete Simulation ToolBar --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplSM
    variable SimToolBarInit
    
    set SimToolBarInit 0 
    wm withdraw $tplSM
  }

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplSM
    variable frmKineSM
    variable FileName
    variable SimToolBarInit  
    
    set SimToolBarInit 1
    
    # Initialization of 3D Simulation Window.
    RobLab::CreateWindow "" wndSimToolBar "Simulation ToolBar" "$RobLab::3DSimTB_Geometry" 25 $RobLab::WndMinSizeY . 0 { SimToolBar::Delete } $RobLab::VirtualGridSize

    # Initialization of binds

    # Tool Bars
    frame $frmKineSM -width 26 -height 55
    label $frmKineSM.label -text "Kinematics"
    button $frmKineSM.knormal_button -text "Normal" -command { source "./Saves/Console/WorldInvKineSim.tcl" }
    button $frmKineSM.kdebug_button -text "Debug" -command { }
    pack  $frmKineSM.label -side top
    pack  $frmKineSM.knormal_button  -side left -fill both -expand true
    pack  $frmKineSM.kdebug_button  -side left -fill both -expand true
    pack  $frmKineSM -side top -fill both -expand true

    
  }
  
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
