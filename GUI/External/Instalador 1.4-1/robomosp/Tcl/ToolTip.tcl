# --------------------------------------------------------------------------------------------------------------------------
#  ToolTip.tcl
#		This file implements the ROBOMOSP Tooltips manager.
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

namespace eval ToolTip {

  # Procedures :
  # proc Show { Widget Text }
  # proc Hide {}
  # proc SetToolTipText { Widget Text }

  # Initialization of global variables

  # Show procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Show { Widget Text } {
    set Alignment [ string range $Text 0 0 ]
    if { "$Alignment" != "N" } {
      catch { destroy .wndToolTip }
      toplevel .wndToolTip
      set Text [ string range $Text 1 end ]
      if { "$Alignment" == "R" } {
        set Left [ expr [ winfo rootx $Widget ] + [ winfo width $Widget ] + 5 ]
        set Top [ expr [ winfo rooty $Widget ] + ( [ winfo height $Widget ] / 2 ) ]
        wm geometry .wndToolTip "\+$Left\+$Top"
      }
      wm overrideredirect .wndToolTip 1
      label .wndToolTip.lblToolTip -borderwidth 1 -relief raised -background #ffffffe74 -foreground #000000000 -text $Text
      pack .wndToolTip.lblToolTip -ipadx 2
      if { "$Alignment" == "L" } {
        if { [ catch { tkwait visibility .wndToolTip } ] == 0 } {
          set Left [ expr [ winfo rootx $Widget ] - 5 - [ winfo width .wndToolTip ] ]
          set Top [ expr [ winfo rooty $Widget ] + ( [ winfo height $Widget ] / 2 ) ]
          wm geometry .wndToolTip "\+$Left\+$Top"
        }
      }
    }
  }

  # Hide procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Hide {} {
    catch { destroy .wndToolTip }
  }

  # SetToolTipText procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetToolTipText { Widget Text } {
    set NewText [ string range $Text 1 end ]
    .wndToolTip.lblToolTip configure -text $NewText
    Show $Widget $Text
  }

}

