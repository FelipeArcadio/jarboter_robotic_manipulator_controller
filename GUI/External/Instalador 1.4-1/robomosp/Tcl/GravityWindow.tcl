#----------------------------------------------------------------------------------------------------------------------------------
# VerletFwdDynToolBar.tcl
# 		This file implements the TCL code for creating and managing the Forward Dynamics
#		ToolBar for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2006-02-25
#
# Requirements:   RobLab.tcl
#			              ObjNavMain.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval GravityWindow {

  # Initialization of global variables
  variable tplGrav .wndGravityWindow            #Main Gravity Window.
  variable frmGravAll $tplGrav.frmGravAll		#Main VerletFwdDyn frame.
  variable frmGravTB $tplGrav.frmToolBar
  variable DOF
  variable WorldId
  variable PRobotId
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplGrav
    wm withdraw $GravityWindow::tplGrav
  }
  
  #  Reset  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Reset {} {
      global Roll
      global Pitch
      global Yaw
      global X
      global Y
      global Z
      
      set Roll 	0
      set Pitch	0
      set Yaw	0
      set X         	0
      set Y        	0
      set Z      	-9.81
}

  #  Ok  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Ok {} {
      global Roll
      global Pitch
      global Yaw
      global X
      global Y
      global Z    
  }
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplGrav
    variable frmGravTB
    variable CellWidth
    global Roll
    global Pitch
    global Yaw
    global X
    global Y
    global Z
    set Roll 0
    set Pitch 0
    set Yaw 0
    set X 0
    set Y 0
    set Z -9.81
    set CellWidth 5
      
	# Initialization of Gravity window
    RobLab::CreateWindow "" wndGravityWindow "Gravity Window" "$RobLab::GravityWindow_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { GravityWindow::Delete } $RobLab::VirtualGridSize
   
    # Tool Bars
    frame $frmGravTB -width 26 -relief groove -borderwidth 2
    pack $frmGravTB -side top -fill both -expand true
      
	# GRAV Roll
	frame $tplGrav.frmRowRoll -width 26 -relief raised -borderwidth 1
		label  $tplGrav.frmRowRoll.label -text "Roll"
		pack  $tplGrav.frmRowRoll.label  -side left -fill both -expand true
		entry $tplGrav.frmRowRoll.entCellR -textvariable Roll -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
		pack $tplGrav.frmRowRoll.entCellR -side left 
	pack $tplGrav.frmRowRoll -side top -fill both -expand true
      
	# GRAV Pitch
	frame $tplGrav.frmRowPitch -width 26 -relief raised -borderwidth 1
		label  $tplGrav.frmRowPitch.label -text "Pitch"
		pack  $tplGrav.frmRowPitch.label  -side left -fill both -expand true
		entry $tplGrav.frmRowPitch.entCellP -textvariable Pitch -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
		pack $tplGrav.frmRowPitch.entCellP -side left 
	pack $tplGrav.frmRowPitch -side top -fill both -expand true
      
	# GRAV Yaw
	frame $tplGrav.frmRowYaw -width 26 -relief raised -borderwidth 1
		label  $tplGrav.frmRowYaw.label -text "Yaw"
		pack  $tplGrav.frmRowYaw.label  -side left -fill both -expand true
		entry $tplGrav.frmRowYaw.entCellYaw -textvariable Yaw -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
		pack $tplGrav.frmRowYaw.entCellYaw -side left 
	pack $tplGrav.frmRowYaw -side top -fill both -expand true
      
	# GRAV X
	frame $tplGrav.frmRowX -width 26 -relief raised -borderwidth 1
		label  $tplGrav.frmRowX.label -text "X"
		pack  $tplGrav.frmRowX.label  -side left -fill both -expand true
		entry $tplGrav.frmRowX.entCellX -textvariable X -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
		pack $tplGrav.frmRowX.entCellX -side left 
	pack $tplGrav.frmRowX -side top -fill both -expand true
      
	# GRAV Y
	frame $tplGrav.frmRowY -width 26 -relief raised -borderwidth 1
		label  $tplGrav.frmRowY.label -text "Y"
		pack  $tplGrav.frmRowY.label  -side left -fill both -expand true
		entry $tplGrav.frmRowY.entCellY -textvariable Y -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
		pack $tplGrav.frmRowY.entCellY -side left 
	pack $tplGrav.frmRowY -side top -fill both -expand true
      
	# GRAV Z
	frame $tplGrav.frmRowZ -width 26 -relief raised -borderwidth 1
		label  $tplGrav.frmRowZ.label -text "Z"
		pack  $tplGrav.frmRowZ.label  -side left -fill both -expand true
		entry $tplGrav.frmRowZ.entCellZ -textvariable Z -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
		pack $tplGrav.frmRowZ.entCellZ -side left 
	pack $tplGrav.frmRowZ -side top -fill both -expand true
      
	#Exit buttons Frame
     	frame $tplGrav.frmExitButtons -relief raised -borderwidth 2
        	button  $tplGrav.frmExitButtons.reset -text "Reset" -command {  GravityWindow::Reset }
        	pack     $tplGrav.frmExitButtons.reset -side left -fill both -expand true
        	button  $tplGrav.frmExitButtons.ok -text "Ok" -command {  GravityWindow::Ok }
        	pack  $tplGrav.frmExitButtons.ok  -side left -fill both -expand true
      pack  $tplGrav.frmExitButtons -side bottom -fill both -expand true -padx 2 -pady 2
  }  
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
