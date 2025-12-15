#----------------------------------------------------------------------------------------------------------------------------------
# RotVecToolBar.tcl
# 		This file implements the TCL code for creating and managing the Arbitrary Axis Rot
#		ToolBar for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-05-25
#
# Requirements:   RobLab.tcl
#			              Menu.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  # Images of Play Back
  image create photo imgPBTBPlay -file ./Images/PlayBackToolBar/play.gif
  image create photo imgPBTBNext -file ./Images/PlayBackToolBar/next.gif
 
# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval RotVecToolBar {

  # Initialization of global variables
  variable tplRotVec .wndRotVecToolBar                  #Main RotVec ToolBar Window.
  variable frmRotVecAll $tplRotVec.frmRotVecAll		 #Main RotVec frame.
  variable frmRotVecTB $tplRotVec.frmToolBar
  variable RotVecToolBarInit 0
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
    variable tplRotVec
    variable RotVecToolBarInit
    variable RotVecToolDebug
    set RotVecToolBarInit 0 
   
    wm withdraw $RotVecToolBar::tplRotVec
  }
  
  #  ReachTarget  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ReachTarget {} {
    #source "./Saves/Console/LLRLSim.tcl"
    # Constants
    set Pi 3.1415926535897932384626433832795
    set Radians [ expr $Pi / 180.0 ]
    set Degrees [ expr 180.0 / $Pi ]
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
      # IKine
      IOIC::PRobot $PRobotId LLRL IKine
      # Get Joint Values
      set mOutJointp [ IOIC::PRobot $PRobotId LLRL mGetOutJointp ]
      # Change Joint Values to Degrees
      set mDOutJointp [ lindex $mOutJointp 0 ]
      set i 0
      foreach { Value } $mDOutJointp {
          set mDOutJointp [ lreplace $mDOutJointp $i $i [ expr $Value * $Degrees ] ]
          incr i
      }
      # Set Links
      eval "IOIC::PRobot $KineCheckList::PRobotId SetLinks $mDOutJointp"
  }  
 }
  
#  RotAngle  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc RotAngle {} {
    variable WorldId
    variable PRobotId
    global Angle
    IOIC::PRobot $PRobotId LLRL RotVec $Angle
    IOIC::ForceReDraw
}
  
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplRotVec
    variable frmRotVecTB
    variable FileName
    variable RotVecToolBarInit
    global Angle
    variable WorldId
    variable PRobotId
    set RotVecToolBarInit 1
    set CellWidth 5
    set Angle 0
    
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
      }  
    }
    
    #Getting PRobot Id
    set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndRotVecToolBar "Arbitrary Axis Rotation" "$RobLab::RotVecTB_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { RotVecToolBar::Delete } $RobLab::VirtualGridSize

    
    #RotVec ToolBar
    frame  $frmRotVecTB -width 26 -height 40 -relief raised -borderwidth 1
    button  $frmRotVecTB.target -text "RReach target" -image imgPBTBPlay -state normal -command {  RotVecToolBar::ReachTarget }
    bind $frmRotVecTB.target <Enter> { ToolBars::Bind_TBButton_Enter %W }
    bind $frmRotVecTB.target <Leave> { ToolBars::Bind_TBButton_Leave %W }
    pack  $frmRotVecTB.target  -side left
    
    frame  $frmRotVecTB.angle -width 10 -height 40 -relief raised -borderwidth 1
    label  $frmRotVecTB.angle.label -text "Angle Value"
    pack  $frmRotVecTB.angle.label  -side left
    entry $frmRotVecTB.angle.entCell -textvariable Angle -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
    pack $frmRotVecTB.angle.entCell -side left
    pack  $frmRotVecTB.angle -side left -fill both -expand true    
    
    button  $frmRotVecTB.rotate -text "RRotate" -image imgPBTBNext -state normal -command {  RotVecToolBar::RotAngle }
    bind $frmRotVecTB.rotate <Enter> { ToolBars::Bind_TBButton_Enter %W }
    bind $frmRotVecTB.rotate <Leave> { ToolBars::Bind_TBButton_Leave %W }
    pack  $frmRotVecTB.rotate  -side right
    pack  $frmRotVecTB -side top -fill both -expand true    
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
