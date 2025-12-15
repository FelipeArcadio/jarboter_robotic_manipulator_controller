# --------------------------------------------------------------------------------------------------------------------------
#  Menu.tcl
#		This file implement the menu manager window.
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

namespace eval Menu {

  # Procedures :
  # proc ShowWindow { Window }
  # proc Initialize {}

  # Initialization of global variables
  variable MainMenu

  #  ShowWindow procedure --
  #
  #		This procedure activates a window from the menu
  #
  # Arguments: Window
  #
  # Results:
  #
  proc ShowWindow { Window } {
    #wm deiconify $Window
    focus $Window
  }

  #   Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable MainMenu
    global InvKine
    global InvDyn
    global  FwdDyn
    global  CartTjc
    if { $::tcl_platform(platform) == "macintosh" } {
      set Modifier "Command"
    } else {
      set Modifier "Control"
    }
    
    set MainMenu .mnuRobLab
    menu $MainMenu -tearoff 0

    #
    # Object Navigator menu
    #
    set CurrentMenu $MainMenu.mnuObjectNavigator
    menu $CurrentMenu -tearoff 0

    # Open
    set Command { ObjectNavigator::OpenFile }
    $CurrentMenu add command -label "Open" -underline 0 -accelerator "$Modifier+O" -command $Command

    # Close
    set Command {}
    $CurrentMenu add command -label "Close" -underline 0 -accelerator "$Modifier+W" -command $Command

   # Save
    set Command { ObjectNavigator::SaveFile 0 }
    $CurrentMenu add command -label "Save" -underline 0 -accelerator "$Modifier+S" -command $Command

    # Save As
    set Command { ObjectNavigator::SaveFile 1 }
    $CurrentMenu add command -label "Save As" -underline 5 -accelerator "$Modifier+A" -command $Command

    # Reload
    set Command { ObjectNavigator::ReloadNode }
    $CurrentMenu add command -label "Reload" -underline 0 -accelerator "$Modifier+R" -command $Command

    # Edit
    set Command { ObjectNavigator::EditNode }
    $CurrentMenu add command -label "Edit" -underline 2 -accelerator "$Modifier+I" -command $Command

    # Separator
    $CurrentMenu add separator

    # Create
    set Command { ObjectNavigator::CreateNode }
    $CurrentMenu add command -label "Create" -underline 4 -accelerator "$Modifier+T" -command $Command

    # Delete
    set Command { ObjectNavigator::DeleteNode }
    $CurrentMenu add command -label "Delete" -underline 0 -accelerator "$Modifier+D" -command $Command

    # Separator
    $CurrentMenu add separator

    # Allocated Memory
    set Command { ObjectNavigator::GetAllocatedMemory }
    $CurrentMenu add command -label "Allocated Memory" -underline 10 -accelerator "$Modifier+M" -command $Command

    # Separator
    $CurrentMenu add separator

    # Exit
    set Command { RobLab::OnApplicationsExit }
    $CurrentMenu add command -label "Exit" -underline 0 -accelerator "$Modifier+E" -command $Command

     # Add object navigator menu
    $MainMenu add cascade -label "Object Navigator" -menu $CurrentMenu -underline 0

    #
    # Window menu
    #
    set CurrentMenu $MainMenu.mnuWindow
    menu $CurrentMenu -tearoff 0

   # RobLab
    set Command { Menu::ShowWindow . }
    $CurrentMenu add command -label "Robomosp" -underline 0 -accelerator "$Modifier+1" -command $Command
    bind all <$Modifier-KeyPress-1> $Command

   # Object Navigator
    set Command { Menu::ShowWindow $ObjectNavigator::tplON }
    $CurrentMenu add command -label "Object Navigator" -underline 0 -accelerator "$Modifier+2" -command $Command
    bind all <$Modifier-KeyPress-2> $Command

    # Properties Palette
    set Command { Menu::ShowWindow $PropertyPalette::tplPP }
    $CurrentMenu add command -label "Properties Palette" -underline 0 -accelerator "$Modifier+3" -command $Command
    bind all <$Modifier-KeyPress-3> $Command

    # Display
    set Command { Menu::ShowWindow $DisplayWindow::tplDP }
    $CurrentMenu add command -label "Display" -underline 0 -accelerator "$Modifier+4" -command $Command
    bind all <$Modifier-KeyPress-4> $Command

    # Matrix Display
    set Command { Menu::ShowWindow $MatrixDisplay::tplMD }
    $CurrentMenu add command -label "Matrix Display" -underline 2 -accelerator "$Modifier+5" -command $Command
    bind all <$Modifier-KeyPress-5> $Command

    # Messages
    set Command { Menu::ShowWindow $MessagesWindow::tplMW }
    $CurrentMenu add command -label "Messages" -underline 0 -accelerator "$Modifier+6" -command $Command
    bind all <$Modifier-KeyPress-6> $Command

    # Console
     if { $ConsoleWindow::ConsoleWindowInit==0  } {
       set Command { ConsoleWindow::Initialize}
    } else {
       set Command { Menu::ShowWindow $ConsoleWindow::tplCW }
    }
    $CurrentMenu add command -label "Console" -underline 0 -accelerator "$Modifier+7" -command $Command
    bind all <$Modifier-KeyPress-7> $Command

    # Sockets
     if { $SocketsWindow::SKWindowInit==0  } {
        if { $ConsoleWindow::ConsoleWindowInit==0  } {
             ConsoleWindow::Initialize
        }
       set Command { SocketsWindow::Initialize}
    } else {
       set Command { Menu::ShowWindow $SocketsWindow::tplSK }
    }
    $CurrentMenu add command -label "Sockets" -underline 0 -accelerator "$Modifier+8" -command $Command
    bind all <$Modifier-KeyPress-8> $Command

    # IRL Text Editor
    if { $RobLab::IRLWindowInit==0  } {
       set Command { IRLWindow::Initialize}
    } else {
      set Command { Menu::ShowWindow $IRLWindow::tplIRL }
    }
    $CurrentMenu add command -label "IRL Text Editor" -underline 0 -accelerator "$Modifier+I" -command $Command
    bind all <$Modifier-KeyPress-I> $Command
    
    #Teach Pendant Window
    set Command { TeachPendantWindow::Initialize}
    $CurrentMenu add command -label "Teach Pendant" -underline 0 -accelerator "$Modifier+T" -command $Command

    #USB Controler
    set Command { exec PMDControler/PMDControler.exe &}
    $CurrentMenu add command -label "USB Controler" -underline 0 -accelerator "$Modifier+U" -command $Command

    # Add window menu
    $MainMenu add cascade -label "Window" -menu $CurrentMenu -underline 0
    
  
    #
    # Simulation menu
    #
    # Inverse Kinematics Toolbar.
    set CurrentMenu $MainMenu.mnuView
    menu $CurrentMenu -tearoff 0
    $CurrentMenu add command -label "Preferences.." -underline 0 -command { CfgCheckList::Initialize }
    $CurrentMenu add checkbutton -label "Inverse Kinematics ToolBar" -variable InvKine -command { InvKineToolBar::Activate }
    # Inverse Dynamics Toolbar.
    $CurrentMenu add checkbutton -label "Inverse Dynamics ToolBar" -variable InvDyn -command { InvDynToolBar::Activate }
    # Forward Dynamics Toolbar.
    $CurrentMenu add checkbutton -label "Forward Dynamics ToolBar" -variable FwdDyn -command { FwdDynSel::Set }
    # Trajectories GNUPlot Toolbar.
    $CurrentMenu add checkbutton -label "Trajectories GNUPlot ToolBar" -variable CartTjc -command { GNUPlot::Activate 1}
    
    # Add Simulation menu
    $MainMenu add cascade -label "Simulation" -menu $CurrentMenu -underline 0
    
    
    #
    # Help menu
    #
    set CurrentMenu $MainMenu.mnuHelp
    menu $CurrentMenu -tearoff 0
    set Command {  help::init ./Tcl/Help/helpfiles/contents.help}
    $CurrentMenu add command -label "About $RobLab::ApplicationsTitle ..." -command $Command

   # Add help menu
    $MainMenu add cascade -label "Help" -menu $CurrentMenu -underline 0

    # Bind menu to root window
    . configure -menu $MainMenu
  }

}

