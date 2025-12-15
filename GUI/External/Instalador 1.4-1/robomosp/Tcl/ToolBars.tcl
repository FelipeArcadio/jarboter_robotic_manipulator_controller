
# --------------------------------------------------------------------------------------------------------------------------
#  ToolBars.tcl
#		This file implements the ROBOMOSP tool bar manager.
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

source ./Tcl/ToolTip.tcl

namespace eval ToolBars {

  # Procedures :
  # proc Create { wdgRoot ToolBar Type }
  # proc Bind_TBButton_Enter { ButtonWidget }
  # proc Bind_TBButton_Leave { ButtonWidget }
  # proc EnableButton { wdgRoot Name }
  # proc DisableButton { wdgRoot Name }
  # proc GetButtonToolTip { wdgRoot Name }
  # proc SetButtonToolTip { wdgRoot Name NewToolTip }
  # proc GetButtonImage { wdgRoot Name }
  # proc SetButtonImage { wdgRoot Name NewImage }
  # proc GetButtonCommand { wdgRoot Name }
  # proc SetButtonCommand { wdgRoot Name NewCommand }
  # proc GetButtonState { wdgRoot Name }
  # proc GetButtonWidget { wdgRoot Name }

  # All images must be 22x22
  # Initialization of global variables
  variable Id 0
  variable Horizontal 0
  variable Vertical 1

  # Create procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Create { wdgRoot ToolBar Type } {
    variable Id
    variable Horizontal
    variable Vertical
    # Button : <0.Name> <1.ToolTip> <2.Image> <3.State> <4.Command>
    foreach Button $ToolBar {
      set Name [ lindex $Button 0 ]
      set ToolTip [ lindex $Button 1 ]
      set Image [ lindex $Button 2 ]
      set State [ lindex $Button 3 ]
      set Command [ lindex $Button 4 ]
      if { $Name != "" } {
        button $wdgRoot.btn$Name -borderwidth 0 -width 19 -height 19 -text $ToolTip -image $Image -state $State -command $Command
        if { [ expr $$Type ] == $Horizontal } {
          pack $wdgRoot.btn$Name -side left -padx 2 -pady 2 -anchor nw -fill none
        } else {
          pack $wdgRoot.btn$Name -side top -padx 2 -pady 2 -anchor nw -fill none
        }
        bind $wdgRoot.btn$Name <Enter> { ToolBars::Bind_TBButton_Enter %W }
        bind $wdgRoot.btn$Name <Leave> { ToolBars::Bind_TBButton_Leave %W }
      } else {
        if { [ expr $$Type ] == $Horizontal } {
          frame $wdgRoot.frmFrame$Id -borderwidth 1 -relief raised -width 2 -height 24
          pack $wdgRoot.frmFrame$Id -side left -padx 2 -pady 1 -anchor nw -fill none
        } else {
          frame $wdgRoot.frmFrame$Id -borderwidth 1 -relief raised -width 24 -height 2
          pack $wdgRoot.frmFrame$Id -side top -padx 1 -pady 2 -anchor nw -fill none
        }
        incr Id
      }
    }
  }

  # Bind_TBButton_Enter procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Bind_TBButton_Enter { ButtonWidget } {
    if { [ $ButtonWidget cget -state ] != "disabled" } {
      pack configure $ButtonWidget -padx 1 -pady 1
      $ButtonWidget configure -borderwidth 1
    }
    if { [ $ButtonWidget cget -text ] != "" } {
      ToolTip::Show $ButtonWidget [ $ButtonWidget cget -text ]
    }
  }

  # Bind_TBButton_Leave procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Bind_TBButton_Leave { ButtonWidget } {
    if { [ $ButtonWidget cget -state ] != "disabled" } {
      $ButtonWidget configure -borderwidth 0
      pack configure $ButtonWidget -padx 2 -pady 2
    }
    if { [ $ButtonWidget cget -text ] != "" } {
      ToolTip::Hide
    }
  }

  # EnableBButton_Leave procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc EnableButton { wdgRoot Name } {
    $wdgRoot.btn$Name configure -state normal
    $wdgRoot.btn$Name configure -borderwidth 0
    pack configure $wdgRoot.btn$Name -padx 2 -pady 2
  }

  # DisableButton procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DisableButton { wdgRoot Name } {
    $wdgRoot.btn$Name configure -state disabled
    $wdgRoot.btn$Name configure -borderwidth 0
    pack configure $wdgRoot.btn$Name -padx 2 -pady 2
  }

  # GetButtonToolTip procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetButtonToolTip { wdgRoot Name } {
    return [ $wdgRoot.btn$Name cget -text ]
  }

  # SetButtonToolTip procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetButtonToolTip { wdgRoot Name NewToolTip } {
    $wdgRoot.btn$Name configure -text $NewToolTip
  }

  # GetButtonImage procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetButtonImage { wdgRoot Name } {
    return [ $wdgRoot.btn$Name cget -image ]
  }

  # SetButtonImage procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetButtonImage { wdgRoot Name NewImage } {
    $wdgRoot.btn$Name configure -image $NewImage
  }

  # GetButtonCommand procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetButtonCommand { wdgRoot Name } {
    return [ $wdgRoot.btn$Name cget -command ]
  }

  # SetButtonCommand procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetButtonCommand { wdgRoot Name NewCommand } {
    $wdgRoot.btn$Name configure -command $NewCommand
  }

  # GetButtonState procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetButtonState { wdgRoot Name } {
    return [ $wdgRoot.btn$Name cget -state ]
  }

  # GetButtonWidget procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetButtonWidget { wdgRoot Name } {
    return $wdgRoot.btn$Name
  }

}
