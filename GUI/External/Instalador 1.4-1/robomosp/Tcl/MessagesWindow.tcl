# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : MessagesWindow.tcl
#  Date         : 2001-11-09
#  Description  : Implementation of messages window manager
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  MessagesWindow.tcl
#		This file implements the ROBOMOSP messages manager window .
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

  # Images of messages window tool bar
  image create photo imgMWTBDelete -file ./Images/MessagesWindowToolBar/Delete.gif

namespace eval MessagesWindow {

  # Procedures :
  # proc Puts { Message }
  # proc Delete {}
  # proc Initialize {}

  # Initialization of global variables
  variable tplMW .wndMessagesWindow
  variable txtMW $tplMW.txtMessages
  variable frmMWTB $tplMW.frmToolBar

  #  Puts procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Puts { Message } {
    variable txtMW
    $txtMW insert end "$Message"
    $txtMW see end
  }

  #  Delete procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable txtMW
    $txtMW delete 1.0 end
  }

  #  Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplMW
    variable txtMW
    variable frmMWTB
    # Initialization of messages window
    RobLab::CreateWindow "" wndMessagesWindow "Messages" "$RobLab::MW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $MessagesWindow::tplMW } $RobLab::VirtualGridSize
    frame $frmMWTB -width 26
    text $txtMW -background #fffffffff -foreground #000000000 -borderwidth 1 -width 10 -height 20 -yscrollcommand "$tplMW.scbVertical set"
    scrollbar $tplMW.scbVertical -borderwidth 1 -orient verti -command "$txtMW yview"
    pack $frmMWTB -side left -fill y
    pack $txtMW -padx 2 -pady 1 -side left -fill both -expand true
    pack $tplMW.scbVertical -side right -fill y
    # Initialization of binds
    bind $txtMW <Key> { break }
    bind $txtMW <Control-c> { continue }
    bind $txtMW <Up> { continue }
    bind $txtMW <Down> { continue }
    bind $txtMW <Left> { continue }
    bind $txtMW <Right> { continue }
    bind $txtMW <Home> { continue }
    bind $txtMW <End> { continue }
    bind $txtMW <Prior> { continue }
    bind $txtMW <Next> { continue }
    # Initialization of messages window tool bar
    set MWToolBar { {      Delete                    "RDelete"      imgMWTBDelete   normal { MessagesWindow::Delete } } }
    ToolBars::Create $frmMWTB $MWToolBar Vertical
  }

}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
