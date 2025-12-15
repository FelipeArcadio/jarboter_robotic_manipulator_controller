#----------------------------------------------------------------------------------------------------------------------------------
# FwdDynCheckList.tcl
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
namespace eval FwdDynCheckList {

  # Initialization of global variables
  variable tplFwdDynCL .wndFwdDynCLWindow                   #Main FwdDyn Window.
  variable frmFwdDynCLAll $tplFwdDynCL.frmFwdDynCLAll  #Main FwdDyn frame.
  variable FwdDynCLWindowInit 0
  variable WorldId
  variable PRobotId
  variable DOF
  variable NumPTrajectories
  variable Mass1
  variable Coriolis1
  variable Centrifugal1
  variable Gyroscopic1
  
  #  Set  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Set {} {
        global Mass
        global Coriolis
        global Centrifugal
		global Gyroscopic
        global IntegratorType 
        
        variable Mass1
        variable Coriolis1
        variable Centrifugal1
        variable Gyroscopic1
        
        variable WorldId
        variable PRobotId
        variable DOF
        
        Delete
        switch $IntegratorType {
        
               Ode45 {
                      FwdDynToolBar::Initialize
               }
               Verlet {
                      VerletFwdDynToolBar::Initialize
               }
        }             
        
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
      global Mass
      global Coriolis
      global Centrifugal
      global Gyroscopic  
      variable Mass1
      variable Coriolis1
      variable Centrifugal1
      variable Gyroscopic1
      
      set Mass      0
      set Coriolis       0
      set Centrifugal  0 
      set Gyroscopic 0 
      set Mass1 0
      set Coriolis1 0
      set Centrifugalt1 0
      set Gyroscopic1 0
       
   }
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplFwdDynCL
    variable FwdDynCLWindowInit
    set FwdDynCLWindowInit 0 
    wm withdraw $tplFwdDynCL
  }
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplFwdDynCL
    variable FwdDynCLWindowInit
    variable WorldId
    variable PRobotId
    variable DOF
    variable NumPTrajectories
    global Mass
    global Coriolis
    global Centrifugal
    global Gyroscopic
    global IntgratorType  
    variable Mass1
    variable Coriolis1
    variable Centrifugal1
    variable Gyroscopic1
    set FwdDynCLWindowInit 1
    set CellWidth 5
    set Mass1 0
    set Coriolis1 0
    set Centrifugal1 0
    set Gyroscopic1 0
    
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
    RobLab::CreateWindow "" wndFwdDynCLWindow "Forward Dynamics Simulation" "$RobLab::FwdDynCLW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { FwdDynCheckList::Delete } $RobLab::VirtualGridSize
   
      frame $tplFwdDynCL.frmview
        label $tplFwdDynCL.frmview.label -text "Parameters to view:"  -font "-weight bold" -anchor w -wraplength 230 -justify left
        pack  $tplFwdDynCL.frmview.label  -side left 
      pack  $tplFwdDynCL.frmview  -side top -fill both -expand true
      frame $tplFwdDynCL.mass 
        checkbutton $tplFwdDynCL.mass.bt -text Mass -variable Mass 
        pack  $tplFwdDynCL.mass.bt  -side left 
      pack  $tplFwdDynCL.mass  -side top -fill both -expand true
      frame $tplFwdDynCL.pos 
        checkbutton  $tplFwdDynCL.pos.bt -text {Position and Orientation} -variable PosOrient
        pack  $tplFwdDynCL.pos.bt  -side left 
      pack  $tplFwdDynCL.pos  -side top -fill both -expand true
      frame $tplFwdDynCL.vel -width 26 
        checkbutton  $tplFwdDynCL.vel.bt -text Velocity -variable Velocity
        pack  $tplFwdDynCL.vel.bt  -side left 
      pack  $tplFwdDynCL.vel  -side top -fill both -expand true
      frame $tplFwdDynCL.accel -width 26
        checkbutton  $tplFwdDynCL.accel.bt -text Acceleration -variable Acceleration
        pack  $tplFwdDynCL.accel.bt  -side left
      pack  $tplFwdDynCL.accel  -side top -fill both -expand true
      
      frame $tplFwdDynCL.frmcomp 
        label $tplFwdDynCL.frmcomp.label -text "Parameters to be computed:" -font "-weight bold" -anchor w -wraplength 230 -justify left
        pack  $tplFwdDynCL.frmcomp.label  -side left 
      pack  $tplFwdDynCL.frmcomp -side top -fill both -expand true
      frame $tplFwdDynCL.cor -width 26 
        checkbutton  $tplFwdDynCL.cor.bt -text {Coriolisis Acceleration} -variable Coriolis
        pack  $tplFwdDynCL.cor.bt  -side left 
      pack  $tplFwdDynCL.cor  -side top -fill both -expand true
      frame $tplFwdDynCL.cent -width 26
        checkbutton  $tplFwdDynCL.cent.bt -text {Centrifugal Acceleration} -variable Centrifugal
        pack  $tplFwdDynCL.cent.bt  -side left
      pack  $tplFwdDynCL.cent  -side top -fill both -expand true
	  frame $tplFwdDynCL.gyr -width 26
        checkbutton  $tplFwdDynCL.gyr.bt -text {Gyroscopic Force}  -variable Gyroscopic
        pack  $tplFwdDynCL.gyr.bt  -side left
      pack  $tplFwdDynCL.gyr  -side top -fill both -expand true
      
      frame $tplFwdDynCL.frmintgr 
        label $tplFwdDynCL.frmintgr.label -text "Integrator Solver:" -font "-weight bold" -anchor w -wraplength 230 -justify left
        pack  $tplFwdDynCL.frmintgr.label  -side left 
      pack  $tplFwdDynCL.frmintgr -side top -fill both -expand true
      frame $tplFwdDynCL.ode45 -width 26 
        radiobutton  $tplFwdDynCL.ode45.bt -variable IntegratorType -value Ode45 -text {Ode 45} 
        pack  $tplFwdDynCL.ode45.bt  -side left 
      pack  $tplFwdDynCL.ode45  -side top -fill both -expand true
      frame $tplFwdDynCL.ver -width 26
        radiobutton  $tplFwdDynCL.ver.bt -variable IntegratorType -value Verlet -text {Verlet} 
        pack  $tplFwdDynCL.ver.bt  -side left
      pack  $tplFwdDynCL.ver  -side top -fill both -expand true

      frame $tplFwdDynCL.frmExitButtons -relief raised -borderwidth 1
        button  $tplFwdDynCL.frmExitButtons.cancel -text "Cancel" -command { FwdDynCheckList::Delete}
        pack     $tplFwdDynCL.frmExitButtons.cancel -side left -fill both -expand true
        button  $tplFwdDynCL.frmExitButtons.reset -text "Reset" -command { FwdDynCheckList::Reset}
        pack     $tplFwdDynCL.frmExitButtons.reset -side left -fill both -expand true
        button  $tplFwdDynCL.frmExitButtons.ok -text "Ok" -command { FwdDynCheckList::Set}
        pack  $tplFwdDynCL.frmExitButtons.ok  -side left -fill both -expand true
      pack  $tplFwdDynCL.frmExitButtons -side top -fill both -expand true     
  }
}  

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
