# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : MatrixDisplay.tcl
#  Date         : 2002-01-10
#  Description  : Implementation of matrix display
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  MatrixDisplay.tcl
#		This file implements the ROBOMOSP matrix display window.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-05-19
#
# Requirements:
#
#SCCS: %Z% %M% %I% %E% %U%
# --------------------------------------------------------------------------------------------------------------------------


# Initialization of Tk objects

  # Initialization of images

  # Images of matrix display tool bar
      image create photo imgMDTBCopy -file ./Images/MatrixDisplayToolBar/Copy.gif
      image create photo imgMDTBGlobal -file ./Images/MatrixDisplayToolBar/Global.gif
      image create photo imgMDTBLocal -file ./Images/MatrixDisplayToolBar/Local.gif

# --------------------------------------------------------------------------------------------------------------------------

namespace eval MatrixDisplay {

  # Procedures :
  # proc GetMatrix { i j }
  # proc SetMatrix { i j Value }
  # proc CopyMatrixToClipBoard {}
  # proc Initialize {}

  # Initialization of global variables
  variable tplMD .wndMatrixDisplay
  variable frmMDTB $tplMD.frmToolBar
  variable Cell_00 $tplMD.frmRow0.entCell_00
  variable Cell_01 $tplMD.frmRow0.entCell_01
  variable Cell_02 $tplMD.frmRow0.entCell_02
  variable Cell_03 $tplMD.frmRow0.entCell_03
  variable Cell_10 $tplMD.frmRow1.entCell_10
  variable Cell_11 $tplMD.frmRow1.entCell_11
  variable Cell_12 $tplMD.frmRow1.entCell_12
  variable Cell_13 $tplMD.frmRow1.entCell_13
  variable Cell_20 $tplMD.frmRow2.entCell_20
  variable Cell_21 $tplMD.frmRow2.entCell_21
  variable Cell_22 $tplMD.frmRow2.entCell_22
  variable Cell_23 $tplMD.frmRow2.entCell_23
  variable Cell_30 $tplMD.frmRow3.entCell_30
  variable Cell_31 $tplMD.frmRow3.entCell_31
  variable Cell_32 $tplMD.frmRow3.entCell_32
  variable Cell_33 $tplMD.frmRow3.entCell_33
  variable CellWidth 14
  variable MDTypeLocal Local
  variable MDTypeGlobal Global
  variable MDType $MDTypeGlobal

  # GetMatrix procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetMatrix { i j } {
    variable Cell_00
    variable Cell_01
    variable Cell_02
    variable Cell_03
    variable Cell_10
    variable Cell_11
    variable Cell_12
    variable Cell_13
    variable Cell_20
    variable Cell_21
    variable Cell_22
    variable Cell_23
    variable Cell_30
    variable Cell_31
    variable Cell_32
    variable Cell_33
    set Cell "Cell_$i$j"
    return [ [ expr $$Cell ] get ]
  }

  # SetMatrix procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetMatrix { i j Value } {
    variable Cell_00
    variable Cell_01
    variable Cell_02
    variable Cell_03
    variable Cell_10
    variable Cell_11
    variable Cell_12
    variable Cell_13
    variable Cell_20
    variable Cell_21
    variable Cell_22
    variable Cell_23
    variable Cell_30
    variable Cell_31
    variable Cell_32
    variable Cell_33
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
  proc Set { { Cell_00 0 } { Cell_01 0 } { Cell_02 0 } { Cell_03 0 } { Cell_10 0 } { Cell_11 0 } { Cell_12 0 } { Cell_13 0 } { Cell_20 0 } { Cell_21 0 } { Cell_22 0 } { Cell_23 0 } { Cell_30 0 } { Cell_31 0 } { Cell_32 0 } { Cell_33 0 } } {
    SetMatrix 0 0 $Cell_00
    SetMatrix 0 1 $Cell_01
    SetMatrix 0 2 $Cell_02
    SetMatrix 0 3 $Cell_03
    SetMatrix 1 0 $Cell_10
    SetMatrix 1 1 $Cell_11
    SetMatrix 1 2 $Cell_12
    SetMatrix 1 3 $Cell_13
    SetMatrix 2 0 $Cell_20
    SetMatrix 2 1 $Cell_21
    SetMatrix 2 2 $Cell_22
    SetMatrix 2 3 $Cell_23
    SetMatrix 3 0 $Cell_30
    SetMatrix 3 1 $Cell_31
    SetMatrix 3 2 $Cell_32
    SetMatrix 3 3 $Cell_33
  }

  # CopyMatrixToClipBoard  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CopyMatrixToClipBoard {} {
    clipboard clear -displayof .
    clipboard append -displayof . -format STRING -type STRING -- "[ GetMatrix 0 0 ] [ GetMatrix 0 1 ] [ GetMatrix 0 2 ] [ GetMatrix 0 3 ]\n[ GetMatrix 1 0 ] [ GetMatrix 1 1 ] [ GetMatrix 1 2 ] [ GetMatrix 1 3 ]\n[ GetMatrix 2 0 ] [ GetMatrix 2 1 ] [ GetMatrix 2 2 ] [ GetMatrix 2 3 ]\n[ GetMatrix 3 0 ] [ GetMatrix 3 1 ] [ GetMatrix 3 2 ] [ GetMatrix 3 3 ]"
  }

  # CommandSetMDType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetMDType {} {
    variable frmMDTB
    variable MDTypeGlobal
    variable MDTypeLocal
    variable MDType
    set ToolTip [ ToolBars::GetButtonToolTip $frmMDTB MDType ]
    if { $ToolTip == "RLocal" } {
      ToolBars::SetButtonToolTip $frmMDTB MDType "RGlobal"
      ToolBars::SetButtonImage $frmMDTB MDType imgMDTBGlobal
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmMDTB MDType ] "RGlobal"
      set MDType $MDTypeGlobal
    } else {
      ToolBars::SetButtonToolTip $frmMDTB MDType "RLocal"
      ToolBars::SetButtonImage $frmMDTB MDType imgMDTBLocal
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmMDTB MDType ] "RLocal"
      set MDType $MDTypeLocal
    }
    ObjectNavigator::SetNodeMatrix
  }
  
# CommandMatToQuat procedure --
#
#
# Arguments:
#
# Results:
#
  proc CommandMatToQuat {} {
    variable frmMDTB
    variable MDTypeGlobal
    variable MDTypeLocal
    variable MDType
    set ToolTip [ ToolBars::GetButtonToolTip $frmMDTB MDType ]
    if { $ToolTip == "RLocal" } {
      set MDType $MDTypeGlobal
    } else {
      set MDType $MDTypeLocal
    }
    QuatDisplay::CheckInitialize
    ObjectNavigator::SetNodeQuat
}

  # Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplMD
    variable frmMDTB
    variable Cell_00
    variable Cell_01
    variable Cell_02
    variable Cell_03
    variable Cell_10
    variable Cell_11
    variable Cell_12
    variable Cell_13
    variable Cell_20
    variable Cell_21
    variable Cell_22
    variable Cell_23
    variable Cell_30
    variable Cell_31
    variable Cell_32
    variable Cell_33
    variable CellWidth

    # Initialization of matrix display
    RobLab::CreateWindow "" wndMatrixDisplay "Matrix Display" "$RobLab::MD_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $MatrixDisplay::tplMD } $RobLab::VirtualGridSize
    frame $frmMDTB -width 26
    pack $frmMDTB -side left -fill y
    # Row 0
    frame $tplMD.frmRow0
    entry $Cell_00 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_01 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_02 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_03 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    pack $tplMD.frmRow0 -side top -fill x
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
    # Row 1
    frame $tplMD.frmRow1
    entry $Cell_10 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_11 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_12 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_13 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    pack $tplMD.frmRow1 -side top -fill x
    pack $Cell_10 -side left -padx 2 -pady 2
    pack $Cell_11 -side left -padx 2 -pady 2
    pack $Cell_12 -side left -padx 2 -pady 2
    pack $Cell_13 -side left -padx 2 -pady 2
    bind $Cell_10 <Key> { break }
    bind $Cell_11 <Key> { break }
    bind $Cell_12 <Key> { break }
    bind $Cell_13 <Key> { break }
    bind $Cell_10 <Control-c> { continue }
    bind $Cell_11 <Control-c> { continue }
    bind $Cell_12 <Control-c> { continue }
    bind $Cell_13 <Control-c> { continue }
    # Row 2
    frame $tplMD.frmRow2
    entry $Cell_20 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_21 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_22 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_23 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    pack $tplMD.frmRow2 -side top -fill x
    pack $Cell_20 -side left -padx 2 -pady 2
    pack $Cell_21 -side left -padx 2 -pady 2
    pack $Cell_22 -side left -padx 2 -pady 2
    pack $Cell_23 -side left -padx 2 -pady 2
    bind $Cell_20 <Key> { break }
    bind $Cell_21 <Key> { break }
    bind $Cell_22 <Key> { break }
    bind $Cell_23 <Key> { break }
    bind $Cell_20 <Control-c> { continue }
    bind $Cell_21 <Control-c> { continue }
    bind $Cell_22 <Control-c> { continue }
    bind $Cell_23 <Control-c> { continue }
    # Row 3
    frame $tplMD.frmRow3
    entry $Cell_30 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_31 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_32 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    entry $Cell_33 -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify right
    pack $tplMD.frmRow3 -side top -fill x
    pack $Cell_30 -side left -padx 2 -pady 2
    pack $Cell_31 -side left -padx 2 -pady 2
    pack $Cell_32 -side left -padx 2 -pady 2
    pack $Cell_33 -side left -padx 2 -pady 2
    bind $Cell_30 <Key> { break }
    bind $Cell_31 <Key> { break }
    bind $Cell_32 <Key> { break }
    bind $Cell_33 <Key> { break }
    bind $Cell_30 <Control-c> { continue }
    bind $Cell_31 <Control-c> { continue }
    bind $Cell_32 <Control-c> { continue }
    bind $Cell_33 <Control-c> { continue }
    # Initialization of matrix display tool bar
    set MDToolBar { {        Copy                      "RCopy"        imgMDTBCopy   normal { MatrixDisplay::CopyMatrixToClipBoard } }
                             {        MDType                  "RGlobal"      imgMDTBGlobal normal { MatrixDisplay::CommandSetMDType } } 
                             {       MatToQuat              "RQuaternion vector"     imgMDTBGlobal normal { MatrixDisplay::CommandMatToQuat } } 
                           }
    ToolBars::Create $frmMDTB $MDToolBar Vertical
  }

}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
