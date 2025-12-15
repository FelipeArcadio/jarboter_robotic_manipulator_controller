#----------------------------------------------------------------------------------------------------------------------------------
# CfgCheckList.tcl
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
namespace eval CfgCheckList {

  # Initialization of global variables
  variable tplCfgCL .wndCfgCLWindow                   #Main Cfg Window.
  variable frmCfgCLAll $tplCfgCL.frmCfgCLAll        #Main Cfg frame.
  variable CfgCLWindowInit 0
  variable WorldId
  variable PRobotId
  variable DOF
  variable NumPTrajectories
  
  
  #  Set  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Set {} {
       #Kinematics variables
        global KinePosOrient
        global KineVel
        global KineAccel
        global Jacobian
       
       #Dynamics variables 
        global Mass
        global Coriolis
        global Centrifugal
	  global Gyroscopic
        global IntegratorType
        global IKineType
        
        #Reset global variables     
        set RobLab::KinePosOrient $KinePosOrient
        set RobLab::KineVel $KineVel
        set RobLab::KineAccel $KineAccel
        set RobLab::Jacobian $Jacobian
        set RobLab::IntegratorType $IntegratorType
        set RobLab::IKineType $IKineType
        
        Delete
  }

  #  Reset  --
  #
  #
  # Arguments:
  #
  # Results:
  #
   proc Reset {} {
      global KinePosOrient
      global KineVel
      global KineAccel
      global Jacobian
      global Mass
      global Coriolis
      global Centrifugal
      global Gyroscopic
      global IntegratorType
      global IKineType
      
      set KinePosOrient 0
      set KineVel            0
      set KineAccel        0
      set Jacobian         0
      set Mass          0
      set Coriolis       0
      set Centrifugal  0 
      set Gyroscopic  0
      set RobLab::IntegratorType Ode45
      set RobLab::IKineType Default
       
      
       
   }
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplCfgCL
    variable CfgCLWindowInit
    set CfgCLWindowInit 0 
    wm withdraw $tplCfgCL
  }
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplCfgCL
    variable CfgCLWindowInit
    variable frmCfgCLAll 
    variable WorldId
    variable PRobotId
    variable DOF
    variable NumPTrajectories
    global KinePosOrient
    global KineVel
    global KineAccel
    global Jacobian
    global Mass
    global Coriolis
    global Centrifugal
    global Gyroscopic
    global IntegratorType
    global IKineType
    
    set CfgCLWindowInit 1
    set CellWidth 5
    set IntegratorType $RobLab::IntegratorType
    set IKineType $RobLab::IKineType
    
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndCfgCLWindow "Preferences" "$RobLab::CfgCLW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { CfgCheckList::Delete } $RobLab::VirtualGridSize
  
  frame $frmCfgCLAll
  
  #Kinematics Frame
  frame $frmCfgCLAll.frmkine -relief groove -borderwidth 2
  	#Parameters to view.
      frame $frmCfgCLAll.frmkine.frmview 
          label $frmCfgCLAll.frmkine.frmview.kine -text "Kinematics:"  -font "-weight bold -size 12" -anchor w
          label $frmCfgCLAll.frmkine.frmview.view -text "Parameters to view:"  -font "-weight bold -size 10" -anchor w      
          checkbutton  $frmCfgCLAll.frmkine.frmview.pos -text {Position and Orientation} -variable KinePosOrient
          checkbutton  $frmCfgCLAll.frmkine.frmview.vel -text Velocity -variable KineVel
          checkbutton  $frmCfgCLAll.frmkine.frmview.accel -text Acceleration -variable KineAccel
          checkbutton  $frmCfgCLAll.frmkine.frmview.jacob -text Jacobian -variable Jacobian
      pack  $frmCfgCLAll.frmkine.frmview $frmCfgCLAll.frmkine.frmview.kine $frmCfgCLAll.frmkine.frmview.view $frmCfgCLAll.frmkine.frmview.pos $frmCfgCLAll.frmkine.frmview.vel $frmCfgCLAll.frmkine.frmview.accel $frmCfgCLAll.frmkine.frmview.jacob -anchor w
	
	#Inverse Kinematics Solver
      frame $frmCfgCLAll.frmkine.frmintgr 
          label $frmCfgCLAll.frmkine.frmintgr.label -text "Inverse kinematics solver:"   -font "-weight bold -size 10"  -anchor w 
          radiobutton  $frmCfgCLAll.frmkine.frmintgr.default -variable IKineType -value Default -text {Default} 
          radiobutton  $frmCfgCLAll.frmkine.frmintgr.user -variable IKineType -value User -text {User defined}      
      pack $frmCfgCLAll.frmkine.frmintgr $frmCfgCLAll.frmkine.frmintgr.label $frmCfgCLAll.frmkine.frmintgr.default $frmCfgCLAll.frmkine.frmintgr.user -anchor w
      
  pack  $frmCfgCLAll.frmkine -expand true -fill both -side left -padx 2 -pady 2
  
  #Dynamics Frame
  frame $frmCfgCLAll.frmdyn -relief groove -borderwidth 2
       #Parameters to view.
       frame $frmCfgCLAll.frmdyn.frmview
          label $frmCfgCLAll.frmdyn.frmview.dyn -text "Dynamics:"  -font "-weight bold -size 12" -justify left
          label $frmCfgCLAll.frmdyn.frmview.view -text "Parameters to view:" -font "-weight bold -size 10"  -anchor w       
          checkbutton $frmCfgCLAll.frmdyn.frmview.mass -text Mass -variable Mass 
          checkbutton  $frmCfgCLAll.frmdyn.frmview.pos -text {Position and Orientation} -variable DynPosOrient
          checkbutton  $frmCfgCLAll.frmdyn.frmview.vel -text Velocity -variable DynVel
          checkbutton  $frmCfgCLAll.frmdyn.frmview.accel -text Acceleration -variable DynAccel
       pack  $frmCfgCLAll.frmdyn.frmview $frmCfgCLAll.frmdyn.frmview.dyn $frmCfgCLAll.frmdyn.frmview.view $frmCfgCLAll.frmdyn.frmview.mass $frmCfgCLAll.frmdyn.frmview.pos $frmCfgCLAll.frmdyn.frmview.vel $frmCfgCLAll.frmdyn.frmview.accel -anchor w
       #Integrator Type
       frame $frmCfgCLAll.frmdyn.frmintgr 
          label $frmCfgCLAll.frmdyn.frmintgr.label -text "Forward dynamics integrator:"   -font "-weight bold -size 10"  -anchor w 
          radiobutton  $frmCfgCLAll.frmdyn.frmintgr.ode45 -variable IntegratorType -value Ode45 -text {Ode 45} 
          radiobutton  $frmCfgCLAll.frmdyn.frmintgr.ver -variable IntegratorType -value Verlet -text {Verlet}      
       pack $frmCfgCLAll.frmdyn.frmintgr $frmCfgCLAll.frmdyn.frmintgr.label $frmCfgCLAll.frmdyn.frmintgr.ode45 $frmCfgCLAll.frmdyn.frmintgr.ver -anchor w
  pack  $frmCfgCLAll.frmdyn -expand true -fill both -side right -padx 2 -pady 2
  
  pack  $frmCfgCLAll -side top -expand true -fill both 
  
    #Exit buttons Frame
     frame $tplCfgCL.frmExitButtons -relief groove -borderwidth 2
        button  $tplCfgCL.frmExitButtons.cancel -text "Cancel" -command { CfgCheckList::Delete}
        pack     $tplCfgCL.frmExitButtons.cancel -side left -fill both -expand true
        button  $tplCfgCL.frmExitButtons.reset -text "Reset" -command { CfgCheckList::Reset}
        pack     $tplCfgCL.frmExitButtons.reset -side left -fill both -expand true
        button  $tplCfgCL.frmExitButtons.ok -text "Ok" -command { CfgCheckList::Set}
        pack  $tplCfgCL.frmExitButtons.ok  -side left -fill both -expand true
      pack  $tplCfgCL.frmExitButtons -side bottom -fill both -expand true -padx 2 -pady 2
         
  }
}  

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
