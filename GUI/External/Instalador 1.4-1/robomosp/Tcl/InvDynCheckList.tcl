#----------------------------------------------------------------------------------------------------------------------------------
# InvDynCheckList.tcl
# 		This file implements the TCL code for creating and managing the Inverse Dynamics
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
namespace eval InvDynCheckList {

  # Initialization of global variables
  variable tplInvDynCL .wndInvDynCLWindow                   #Main InvDyn Window.
  variable frmInvDynCLAll $tplInvDynCL.frmInvDynCLAll  #Main InvDyn frame.
  variable InvDynCLWindowInit 0
  variable WorldId
  variable PRobotId
  variable DOF
  variable NumPTrajectories
  variable Accelerate1
  variable Friction1
  variable Gravitational1
  variable Velocity1 
  
  #  Set  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Set {} {
        global Accelerate
        global Friction
        global Gravitational
        global Velocity
        
        variable Accelerate1
        variable Friction1
        variable Gravitational1
        variable Velocity1
        
        variable WorldId
        variable PRobotId
        variable DOF
        
        Delete
        InvDynToolBar::Initialize
        
        #Initialize Matrix Windows.
  }

  #  Reset  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Reset {} {
      global Accelerate
      global Friction
      global Gravitational
      global Velocity  
      variable Accelerate1
      variable Friction1
      variable Gravitational1
      variable Velocity1
      
      set Accelerate 0
      set Friction 0
      set Gravitational 0
      set Velocity 0
      set Accelerate1 0
      set Friction1 0
      set Gravitational1 0
      set Velocity1 0
   }
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplInvDynCL
    variable InvDynCLWindowInit
    set InvDynCLWindowInit 0 
    wm withdraw $tplInvDynCL
  }
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplInvDynCL
    variable InvDynCLWindowInit
    variable WorldId
    variable PRobotId
    variable DOF
    variable NumPTrajectories
    global Accelerate
    global Friction
    global Gravitational
    global Velocity  
    variable Accelerate1
    variable Friction1
    variable Gravitational1
    variable Velocity1
    set InvDynCLWindowInit 1
    set CellWidth 5
    set Accelerate1 0
    set Friction1 0
    set Gravitational1 0
    set Velocity1 0
    
    # Error Message if NumWorlds is 0.
    if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
        error "Error:  Not a world has been loaded, please load a World definition file."
    } 
 
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndInvDynCLWindow "Inverse Dynamics Simulation" "$RobLab::InvDynCLW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { InvDynCheckList::Delete } $RobLab::VirtualGridSize
   
      frame $tplInvDynCL.frmlabel
        label $tplInvDynCL.frmlabel.label -text "Parameters to be computed:"  -font "-weight bold" -anchor w -wraplength 230 -justify left
        pack  $tplInvDynCL.frmlabel.label  -side left 
      pack  $tplInvDynCL.frmlabel  -side top -fill both -expand true
      frame $tplInvDynCL.acc
        checkbutton $tplInvDynCL.acc.bt -text {Accelerated Force} -variable Accelerate 
        pack  $tplInvDynCL.acc.bt  -side left 
      pack  $tplInvDynCL.acc  -side top -fill both -expand true
      frame $tplInvDynCL.fric 
        checkbutton  $tplInvDynCL.fric.bt -text Friction -variable Friction
        pack  $tplInvDynCL.fric.bt  -side left 
      pack  $tplInvDynCL.fric  -side top -fill both -expand true
      frame $tplInvDynCL.grav
        checkbutton  $tplInvDynCL.grav.bt -text {Gravitational Force} -variable Gravitational
        pack  $tplInvDynCL.grav.bt  -side left
      pack  $tplInvDynCL.grav  -side top -fill both -expand true
	  frame $tplInvDynCL.vel
        checkbutton  $tplInvDynCL.vel.bt -text {Velocity}  -variable Velocity
        pack  $tplInvDynCL.vel.bt  -side left
      pack  $tplInvDynCL.vel  -side top -fill both -expand true

      frame $tplInvDynCL.frmExitButtons -relief raised -borderwidth 1
        button  $tplInvDynCL.frmExitButtons.cancel -text "Cancel" -command { InvDynCheckList::Delete}
        pack     $tplInvDynCL.frmExitButtons.cancel -side left -fill both -expand true
        button  $tplInvDynCL.frmExitButtons.reset -text "Reset" -command { InvDynCheckList::Reset}
        pack     $tplInvDynCL.frmExitButtons.reset -side left -fill both -expand true
        button  $tplInvDynCL.frmExitButtons.ok -text "Ok" -command { InvDynCheckList::Set}
        pack  $tplInvDynCL.frmExitButtons.ok  -side left -fill both -expand true
      pack  $tplInvDynCL.frmExitButtons -side top -fill both -expand true     
  }
}  

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
