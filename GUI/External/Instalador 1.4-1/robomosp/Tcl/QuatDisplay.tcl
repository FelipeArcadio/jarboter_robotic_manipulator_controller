# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : QuatDisplay.tcl
#  Date         : 2005-05-17
#  Description  : Implementation of quaternion display
#  Requirements :
#  Copyright    : Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  MatrixDisplay.tcl
#		This file implements the ROBOMOSP quaternion display window.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-05-17
#
# Requirements:
#
#SCCS: %Z% %M% %I% %E% %U%
# --------------------------------------------------------------------------------------------------------------------------


# Initialization of Tk objects

  # Initialization of images

  # Images of matrix display tool bar
      image create photo imgQuatTBDeg -file ./Images/QuatDisplayToolBar/Global.gif
      image create photo imgQuatTBRad -file ./Images/QuatDisplayToolBar/Local.gif

# --------------------------------------------------------------------------------------------------------------------------

namespace eval QuatDisplay {

  # Procedures :
  # proc GetMatrix { i j }
  # proc SetMatrix { i j Value }
  # proc CopyMatrixToClipBoard {}
  # proc Initialize {}

  # Initialization of global variables
  variable tplQuat .wndQuatDisplay
  variable frmQuatTB $tplQuat.frmToolBar
  variable Cell_00 $tplQuat.frmRow0.entCell_00
  variable Cell_01 $tplQuat.frmRow0.entCell_01
  variable Cell_02 $tplQuat.frmRow0.entCell_02
  variable Cell_03 $tplQuat.frmRow0.entCell_03
  variable CellWidth 14
  variable QuatAngleTypeDeg  Deg
  variable QuatAngleTypeRad  Rad
  variable QuatType $QuatAngleTypeDeg
  variable QuatDisplayInit 0
  
  # GetQuat procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetQuat { i j } {
    variable Cell_00
    variable Cell_01
    variable Cell_02
    variable Cell_03
    set Cell "Cell_$i$j"
    return [ [ expr $$Cell ] get ]
  }

  # SetQuat procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetQuat { i j Value } {
    variable Cell_00
    variable Cell_01
    variable Cell_02
    variable Cell_03
    set Cell "Cell_$i$j"
    eval [ expr $$Cell ] delete 0 end
    if { $Value != "." } {
      eval [ expr $$Cell ] insert 0 $Value
    }
  }

  # Set procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Set { { Cell_00 0 } { Cell_01 0 } { Cell_02 0 } { Cell_03 0 } } {
    SetQuat 0 0 $Cell_00
    SetQuat 0 1 $Cell_01
    SetQuat 0 2 $Cell_02
    SetQuat 0 3 $Cell_03
  }
  
  # Set procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplQuat
    variable QuatDisplayInit 
    set QuatDisplayInit 0 
    
     wm withdraw $tplQuat
}
  
  # CommandSetAngleType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetAngleType {} {
    variable frmQuatTB
    variable QuatAngleTypeDeg
    variable QuatAngleTypeRad
    variable QuatType
    set ToolTip [ ToolBars::GetButtonToolTip $frmQuatTB QuatType ]
    if { $ToolTip == "RRad" } {
      set QuatType $QuatAngleTypeDeg
    } else {
      set QuatType $QuatAngleTypeRad
    }
  #ObjectNavigator::SetNodeQuat
  }

  # CheckInitialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CheckInitialize {} {
    variable tplQuat
    variable QuatDisplayInit
    if { $QuatDisplayInit==0  } {
       QuatDisplay::Initialize
    } else {
      Menu::ShowWindow $tplQuat
    }
  }
  
  # Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplQuat
    variable frmQuatTB
    variable QuatDisplayInit
    variable Cell_00
    variable Cell_01
    variable Cell_02
    variable Cell_03
    variable CellWidth
    set QuatDisplayInit 1

    # Initialization of matrix display
    RobLab::CreateWindow "" wndQuatDisplay "Quaternion" "$RobLab::Quat_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { QuatDisplay::Delete } $RobLab::VirtualGridSize
    frame $frmQuatTB -width 26
    pack $frmQuatTB -side left -fill y
    # Row 0
    frame $tplQuat.frmRow0
    entry $Cell_00 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_01 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_02 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_03 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    pack $tplQuat.frmRow0 -side top -fill x
    pack $Cell_00 -side left -padx 2 -pady 2
    pack $Cell_01 -side left -padx 2 -pady 2
    pack $Cell_02 -side left -padx 2 -pady 2
    pack $Cell_03 -side left -padx 2 -pady 2
    bind $Cell_00 <Key> { break }
    bind $Cell_01 <Key> { break }
    bind $Cell_02 <Key> { break }
    bind $Cell_03 <Key> { break }
    bind $Cell_00 <Control-c> { continue }
    bind $Cell_01 <Control-c> { continue }
    bind $Cell_02 <Control-c> { continue }
    bind $Cell_03 <Control-c> { continue }
    
    # Initialization of matrix display tool bar
    set QuatToolBar {  {       QuatAngleType      "RDeg"   imgQuatTBDeg   disabled    { QuatDisplay::CommandSetAngleType }  } 
                             }
    ToolBars::Create $frmQuatTB $QuatToolBar Vertical
  }

}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
