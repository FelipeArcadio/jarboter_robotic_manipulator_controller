# --------------------------------------------------------------------------------------------------------------------------

#  File Name    : ConsoleWindow.tcl
#  Date         : 2003-03-09
#  Description  : Implementation of console window manager
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co

# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  ConsoleWindow.tcl
#		This file implements the console window.
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

  # Images of object console window tool bar
      image create photo imgCWTBTrmntCmd -file ./Images/ConsoleWindowToolBar/TrmntCmd.gif
      image create photo imgCWTBHstClear -file ./Images/ConsoleWindowToolBar/HstClear.gif

# --------------------------------------------------------------------------------------------------------------------------

namespace eval ConsoleWindow {

  # Procedures :
  # proc PutsInfo { Message { TagsList TagInfo } }
  # proc Puts { Message { TagsList TagOutput } { Index insert } }
  # proc SetPrompt { PromptType }
  # proc Open {}
  # proc Delete {}
  # proc TerminateCommand {}
  # proc HistoryClear {}
  # proc HistoryPutsEvent { Event }
  # proc Evaluate {}
  # proc RePositionCursor {}
  # proc TestSelection {}
  # proc ProcessKeyPress { CurrentKey }
  # proc Initialize {}

  # Initialization of global variables
  variable tplCW .wndConsoleWindow
  variable txtCW $tplCW.txtConsole
  variable frmCWTB $tplCW.frmToolBar
  variable CurrentOpenDirectory ./Saves/Console
  variable CurrentCommand ""
  variable CurrentHistoryEvent 0
  variable PromptA "# "
  variable PromptB "> "
  variable ConsoleWindowInit 0

  #  PutsInfo  procedure --
  #
  #
  # Arguments: 
  #
  # Results:
  #
  proc PutsInfo { Message { TagsList TagInfo } } {
    variable txtCW
    variable CurrentCommand
    $txtCW insert end "\n$Message\n" $TagsList
    set CurrentCommand ""
    SetPrompt
  }

  #  Puts procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc Puts { Message { TagsList TagOutput } { Index end } } {
    variable txtCW
    $txtCW insert $Index "$Message" $TagsList
    $txtCW see $Index
  }

  #  SetPrompt  procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc SetPrompt { { PromptType "PromptA" } } {
    variable PromptA
    variable PromptB
    variable txtCW
    Puts [ expr $$PromptType ] TagPrompt
    $txtCW mark set insert "end - 1 char"
    $txtCW mark set MrkPromptEnd "end - 1 char"
    $txtCW mark gravity MrkPromptEnd left
  }

  #  Open  procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc Open {} {
    variable tplCW
    variable CurrentOpenDirectory
    set FileTypes { {"Tcl Scripts" ".tcl"} {"All Files" "*"} }
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir $CurrentOpenDirectory -parent $tplCW -title "Open Tcl Script" ]
    if { $FileName != "" } {
      set $CurrentOpenDirectory [ file dirname $FileName ]
      Puts "source \"$FileName\"" TagInput
    }
  }

  #  Delete  procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc Delete {} {
    variable txtCW
    $txtCW delete 1.0 end
    SetPrompt
  }

  #  TerminateCommand  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc TerminateCommand {} {
    variable CurrentCommand
    if { $CurrentCommand != "" } {
      set CurrentCommand ""
      Puts "\n" TagInput
      SetPrompt
    }
  }

  #  HistoryClear  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc HistoryClear {} {
    variable CurrentHistoryEvent
    history clear
    set CurrentHistoryEvent 0
  }

  #  HistoryPutsEvent  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc HistoryPutsEvent { Event } {
    variable txtCW
    $txtCW delete MrkPromptEnd "end - 1 char"
    Puts [ history event $Event ] TagInput
  }

  #  Evaluate  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Evaluate {} {
    variable txtCW
    variable CurrentCommand
    variable CurrentHistoryEvent
    set Command [ $txtCW get MrkPromptEnd "end - 1 char" ]
    if { $CurrentCommand == "" } {
      set CurrentCommand $Command
    } else {
      set CurrentCommand "$CurrentCommand\n$Command"
    }
    Puts "\n" TagInput
    if { $CurrentCommand != "" } {
      if { [ info complete $CurrentCommand ] == 1 } {
        set Error [ catch {set Result [ uplevel #0 $CurrentCommand ]} ErrorInfo ]
        if { $Error != 0 } {
          Puts "$ErrorInfo\n" TagError
        } else {
          if { $Result != "" } {
            Puts "$Result\n" TagOutput
          } else {
            Puts "<Empty Result>\n" TagEmpty
          }
        }
        history add $CurrentCommand
        set CurrentHistoryEvent [ history nextid ]
        set CurrentCommand ""
        SetPrompt
      } else {
        SetPrompt PromptB
      }
    } else {
      SetPrompt
    }
  }

  #  RePositionCursor  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc RePositionCursor {} {
    variable txtCW
    if { [ $txtCW compare insert < MrkPromptEnd ] == 1 } {
      $txtCW mark set insert "end - 1 char"
    }
  }

  #  TestSelection  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc TestSelection {} {
    variable txtCW
    if { [ selection own ] == $txtCW } {
      set Error [ catch {set Selection [ selection get ]} ErrorInfo ]
      if { $Error == 0 } {
        if { [ $txtCW compare sel.first < MrkPromptEnd ] == 1 } {
          return "break"
        }
      } else {
        selection clear
      }
    }
    return "continue"
  }

  #  ProcessKeyPress  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ProcessKeyPress { CurrentKey } {
    variable txtCW
    variable CurrentHistoryEvent
    switch $CurrentKey {
      Return    {
                  ConsoleWindow::Evaluate
                  return "break"
                }
      BackSpace {
                  if { [ $txtCW compare insert <= MrkPromptEnd ] == 1 } {
                    return "break"
                  } else {
                    return "continue"
                  }
                }
      Delete    {
                  set Result [ TestSelection ]
                  if { $Result == "continue" } {
                    if { [ $txtCW compare insert < MrkPromptEnd ] == 1 } {
                      return "break"
                    } else {
                      RePositionCursor
                      return "continue"
                    }
                  } else {
                    return "break"
                  }
                }
      Up        {
                  if { [ $txtCW compare insert < MrkPromptEnd ] == 1 } {
                    return "continue"
                  } else {
                    if { $CurrentHistoryEvent > 1 } {
                      incr CurrentHistoryEvent -1
                      HistoryPutsEvent $CurrentHistoryEvent
                    } else {
                      set CurrentHistoryEvent 0
                      $txtCW delete MrkPromptEnd "end - 1 char"
                    }
                    return "break"
                  }
                }
      Down      {
                  if { [ $txtCW compare insert < MrkPromptEnd ] == 1 } {
                    return "continue"
                  } else {
                    if { $CurrentHistoryEvent < [ expr [ history nextid ] - 1 ] } {
                      incr CurrentHistoryEvent
                      HistoryPutsEvent $CurrentHistoryEvent
                    } else {
                      set CurrentHistoryEvent [ history nextid ]
                      $txtCW delete MrkPromptEnd "end - 1 char"
                    }
                    return "break"
                  }
                }
      Insert    {
                  return "continue"
                }
      Home      {
                  return "continue"
                }
      End       {
                  return "continue"
                }
      Prior     {
                  return "continue"
                }
      Next      {
                  return "continue"
                }
      Right     {
                  return "continue"
                }
      Left      {
                  return "continue"
                }
      Shift_L   {
                  return "continue"
                }
      Control_L {
                  return "continue"
                }
      Alt_L     {
                  return "continue"
                }
      default   {
                  RePositionCursor
                  return "continue"
                }
    }
  }

  #  Initialize  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplCW
    variable txtCW
    variable frmCWTB
    variable ConsoleWindowInit
    set ConsoleWindowInit 1
    # Initialization of console window
    RobLab::CreateWindow "" wndConsoleWindow "Console" "$RobLab::CW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $ConsoleWindow::tplCW } $RobLab::VirtualGridSize
    frame $frmCWTB -width 26
    text $txtCW -background #fffffffff -foreground #333333fff -borderwidth 1 -width 10 -height 20 -yscrollcommand "$tplCW.scbVertical set"
    # Other text widget stuff
    $txtCW tag configure TagInput -foreground #333333fff
    $txtCW tag configure TagOutput -foreground #666666666
    $txtCW tag configure TagError -foreground #fff333333
    $txtCW tag configure TagPrompt -foreground #333666333
    $txtCW tag configure TagEmpty -foreground #aaaaaa000
    $txtCW tag configure TagInfo -foreground #8c8464000
    # ...
    scrollbar $tplCW.scbVertical -borderwidth 1 -orient verti -command "$txtCW yview"
    pack $frmCWTB -side left -fill y
    pack $txtCW -padx 2 -pady 1 -side left -fill both -expand true
    pack $tplCW.scbVertical -side right -fill y
    # Initialization of binds
    bind $tplCW <FocusIn> {
      focus $ConsoleWindow::txtCW
    }
    bind $txtCW <KeyPress> {
      set Result [ ConsoleWindow::ProcessKeyPress %K ]
      eval $Result
    }
    bind $txtCW <<Copy>> {
    }
    bind $txtCW <<Cut>> {
      set Result [ ConsoleWindow::TestSelection ]
      eval $Result
    }
    bind $txtCW <<Paste>> {
      ConsoleWindow::RePositionCursor
      catch {%W insert insert [ selection get -selection CLIPBOARD ]}
      break;
    }
    # Initialization of console window tool bar
    set CWToolBar { {          Open              "ROpen"               imgONTBOpen     normal  { ConsoleWindow::Open } }
                    {        Delete              "RDelete"             imgMWTBDelete   normal  { ConsoleWindow::Delete } }
                    {      TrmntCmd              "RTerminate Command"  imgCWTBTrmntCmd normal  { ConsoleWindow::TerminateCommand } }
                    {      HstClear              "RClear History"      imgCWTBHstClear normal  { ConsoleWindow::HistoryClear } } }
    ToolBars::Create $frmCWTB $CWToolBar Vertical
    SetPrompt
  }

}

