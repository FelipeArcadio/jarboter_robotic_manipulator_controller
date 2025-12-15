# --------------------------------------------------------------------------------------------------------------------------
#  JacobianDisplay.tcl
#		This file implements the ROBOMOSP jacobian matrix display window.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-05-26
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

namespace eval JacobianMatrixDisplay {

  # Initialization of global variables
  variable tplJC .wndJCMatrixDisplay
  variable frmJCTB $tplJC.frmToolBar
  variable JCTypeLocal Local
  variable JCTypeGlobal Global
  variable JCType $JCTypeGlobal
  variable JacobianMatrixDisplayInit 0
  
  # SetMatrix procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetMatrix { i j Value } {
    variable tplJC
   eval $tplJC.frmRow$i.entCell_$i$j delete 0 end
    if { $Value != "." } {
      $tplJC.frmRow$i.entCell_$i$j insert 0 $Value
    }
}
  
  # Set procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Set { Jacobian } {
    set length [ llength $Jacobian]
    
    set Matrix_m [ expr $length / 6 ] 
    set K 0
     for {set I 0 } { $I < 6 } {incr I} {
          for {set J 0 } { $J < $Matrix_m } {incr J } {
              SetMatrix $I $J [ lindex $Jacobian $K]
              set K [ expr $K+1 ]
          }    
      }       
  }
  
  # CommandSetMDType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetJCType {} {
    variable frmJCTB
    variable JCTypeGlobal
    variable JCTypeLocal
    variable JCType
    set ToolTip [ ToolBars::GetButtonToolTip $frmJCTB JCType ]
    if { $ToolTip == "RLocal" } {
      ToolBars::SetButtonToolTip $frmJCTB JCType "RGlobal"
      ToolBars::SetButtonImage $frmJCTB JCType imgJCTBGlobal
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmJCTB JCType ] "RGlobal"
      set JCType $JCTypeGlobal
      MessagesWindow::Puts "JCType =$JCType\n"
    } else {
      ToolBars::SetButtonToolTip $frmJCTB JCType "RLocal"
      ToolBars::SetButtonImage $frmJCTB JCType imgJCTBLocal
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmJCTB JCType ] "RLocal"
      set JCType $JCTypeLocal
      MessagesWindow::Puts "JCType =$JCType\n"
    }
  }

  # Delete procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
	variable tplJC
	variable JacobianMatrixDisplayInit
	set JacobianMatrixDisplayInit 0

	wm withdraw $tplJC
  }

  # Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize { { Matrix_m 0 } { Matrix_n 0 } } {
    variable tplJC
    variable frmJCTB
    set CellWidth 10
	variable JacobianMatrixDisplayInit
	set JacobianMatrixDisplayInit 1


    # Initialization of matrix display
     RobLab::CreateWindow "" wndJCMatrixDisplay "Jacobian" "$RobLab::JC_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { JacobianMatrixDisplay::Delete } $RobLab::VirtualGridSize
      
    
    frame $frmJCTB -width 26
    pack $frmJCTB -side left -fill y
    
    for {set I 0 } { $I < $Matrix_m} {incr I} {
            # Row i
            frame $tplJC.frmRow$I
        for {set J 0 } { $J < $Matrix_n} {incr J} {
            entry $tplJC.frmRow$I.entCell_$I$J -width $CellWidth -borderwidth 0 -relief sunken -background #fffffffff -font { -size 9 } -justify right
            pack $tplJC.frmRow$I -side top -fill both -expand true
            pack $tplJC.frmRow$I.entCell_$I$J -side left -padx 2 -pady 2 -fill both -expand true
            bind $tplJC.frmRow$I.entCell_$I$J <Key> { break }
            bind $tplJC.frmRow$I.entCell_$I$J <Control-c> { continue }
        }
    }
    
    # Initialization of matrix display tool bar
    set JCToolBar {  {   JCType        "RGlobal"      imgJCTBGlobal normal { JacobianMatrixDisplay::CommandSetJCType } } 
                           }
    ToolBars::Create $frmJCTB $JCToolBar Vertical  
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
