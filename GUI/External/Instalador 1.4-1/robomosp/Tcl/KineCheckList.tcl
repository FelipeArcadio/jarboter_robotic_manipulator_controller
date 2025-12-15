#----------------------------------------------------------------------------------------------------------------------------------
# KineCheckList.tcl
# 		This file implements the TCL code for creating and managing the Kinematics
#		CheckList Window for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-06-06
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
namespace eval KineCheckList {

  # Initialization of global variables
  variable tplKineCL .wndKineCLWindow               #Main FwdDyn ToolBar Window.
  variable frmKineCLAll $tplKineCL.frmKineCLAll		#Main FwdDyn frame.
  variable KineCLWindowInit 0
  variable WorldId
  variable PRobotId
  variable DOF
  variable NumPTrajectories
  variable PosOrient1
  variable Velocity1
  variable Acceleration1
  variable Jacobian1
  
  #  Set  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Set {} {
        global PosOrient
        global Velocity
        global Acceleration
		global Jacobian
        
        variable PosOrient1
        variable Velocity1
        variable Acceleration1
		variable Jacobian1
        
        variable WorldId
        variable PRobotId
        variable DOF
        
        Delete
        InvKineToolBar::Initialize
        
        #Initialize Matrix Windows.
        if { $PosOrient == 1} {
            FKineMatrixDisplay::Initialize
			IOIC::PRobot $PRobotId LLRL FKine
			IOIC::PRobot $PRobotId LLRL mGetOutHMT
            set PosOrient1 1
        }  
        if { $Velocity == 1} {
            VelocityMatrixDisplay::Initialize
            set Velocity1 1
        }
        if { $Acceleration == 1} {
            AccelMatrixDisplay::Initialize
            set Acceleration1 1
        }
		if { $Jacobian == 1} {
            JacobianMatrixDisplay::Initialize 6 $DOF
			if { $JacobianMatrixDisplay::JCType  == "Global" } {
                set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Local ]
            } else {
                set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Global ]
            }
            JacobianMatrixDisplay::Set [ join $mJacobian ]
            set Jacobian1 1
        }
  }

  #  Set  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Reset {} {
        global PosOrient
        global Velocity
        global Acceleration
		global Jacobian
        variable PosOrient1
        variable Velocity1
        variable Acceleration1
		variable Jacobian1
        
        set PosOrient 0
        set Velocity 0
        set Acceleration 0
		set Jacobian 0
        set PosOrient1 0
        set Velocity1 0
        set Acceleration1 0
		set Jacobian1 0
    }
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplKineCL
    variable KineCLWindowInit
    set KineCLWindowInit 0 
    wm withdraw $tplKineCL
  }
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplKineCL
    variable KineCLWindowInit
    variable WorldId
    variable PRobotId
    variable DOF
    variable NumPTrajectories
    global PosOrient
    global Velocity
    global Acceleration
    global Jacobian
    variable PosOrient1
    variable Velocity1
    variable Acceleration1
	variable Jacobian1
    set KineCLWindowInit 1
    set CellWidth 5
    set PosOrient1 0
    set Velocity1 0
    set Acceleration1 0
	set Jacobian1 0
    
    # Error Message if NumWorlds is 0.
    if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
        error "Error:  Not a world has been loaded."
    } else { 
      # Get WorldId
      set Index $ObjectNavigator::WorldNodeId
      set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
      set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
      # Error Message if NumPRobots is 0.
      if {  [ llength [ IOIC::World $WorldId LsPRobots ] ] == 0 } {
        error "Error: There are not robots in current world."
      } else {
        # Get PRobotId
        set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
        # Get DOF
		set DOF [ IOIC::PRobot $PRobotId GetDOF ]
        # Set Number of Trajectories
        set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]
      } 
   }
    
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndKineCLWindow "Kinematics Simulation" "$RobLab::KineCLW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { KineCheckList::Delete } $RobLab::VirtualGridSize
   
      frame $tplKineCL.frmlabel 
        label $tplKineCL.frmlabel.label -text "Parameters to view:"  -font "-weight bold" -anchor w -wraplength 230 -justify left
        pack  $tplKineCL.frmlabel.label  -side left 
      pack  $tplKineCL.frmlabel  -side top -fill both -expand true
      frame $tplKineCL.pos -width 26 
        checkbutton $tplKineCL.pos.bt -text {Position and Orientation} -variable PosOrient 
        pack  $tplKineCL.pos.bt  -side left 
      pack  $tplKineCL.pos  -side top -fill both -expand true
      frame $tplKineCL.vel -width 26 
        checkbutton  $tplKineCL.vel.bt -text Velocity -variable Velocity -state disabled
        pack  $tplKineCL.vel.bt  -side left 
      pack  $tplKineCL.vel  -side top -fill both -expand true
      frame $tplKineCL.accel -width 26
        checkbutton  $tplKineCL.accel.bt -text Acceleration -variable Acceleration -state disabled
        pack  $tplKineCL.accel.bt  -side left
      pack  $tplKineCL.accel  -side top -fill both -expand true
	  frame $tplKineCL.jacob -width 26
        checkbutton  $tplKineCL.jacob.bt -text Jacobian -variable Jacobian
        pack  $tplKineCL.jacob.bt  -side left
      pack  $tplKineCL.jacob  -side top -fill both -expand true

      frame $tplKineCL.frmExitButtons -relief raised -borderwidth 1
        button  $tplKineCL.frmExitButtons.cancel -text "Cancel" -command { KineCheckList::Delete}
        pack     $tplKineCL.frmExitButtons.cancel -side left -fill both -expand true
        button  $tplKineCL.frmExitButtons.reset -text "Reset" -command { KineCheckList::Reset}
        pack     $tplKineCL.frmExitButtons.reset -side left -fill both -expand true
        button  $tplKineCL.frmExitButtons.ok -text "Ok" -command { KineCheckList::Set}
        pack  $tplKineCL.frmExitButtons.ok  -side left -fill both -expand true
      pack  $tplKineCL.frmExitButtons -side top -fill both -expand true     
  }
}  

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
