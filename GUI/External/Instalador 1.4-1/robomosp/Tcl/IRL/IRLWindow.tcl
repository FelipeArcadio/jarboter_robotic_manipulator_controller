# --------------------------------------------------------------------------------------------------------------------------
#  IRLWindow.tcl
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
     

# --------------------------------------------------------------------------------------------------------------------------

namespace eval IRLWindow {

  # Other files needed by this namespace
  source ./Tcl/IRL/support.tcl

  # Initialization of global variables
  variable tplIRL .wndIRLWindow
  variable frmMenuBar $tplIRL.frmMenuBar
  variable txtIRL $tplIRL.txtIRL
  variable frmIRLTB $tplIRL.frmToolBar
  variable CurrentOpenDirectory ./Saves/IRL/
  variable CurrentCommand ""
  variable CurrentHistoryEvent 0
  variable PromptA "# "
  variable PromptB "> "
  variable FileName
  variable SourceName
  variable RootName
  variable WorkingDirectory
  variable RootDirectory ../../../

  #  PutsInfo  procedure --
  #
  #
  # Arguments: 
  #
  # Results:
  #
  proc PutsInfo { Message { TagsList TagInfo } } {
    variable txtIRL
    variable CurrentCommand
    $txtIRL insert end "\n$Message\n" $TagsList
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
    variable txtIRL
    $txtIRL insert $Index "$Message" $TagsList
    $txtIRL see $Index
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
    variable txtIRL
    #Puts [ expr $$PromptType ] TagPrompt
    $txtIRL mark set insert "end - 1 char"
    $txtIRL mark set MrkPromptEnd "end - 1 char"
    $txtIRL mark gravity MrkPromptEnd left
  }

 
#  Open  procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc Open {} {
    variable tplIRL
    variable txtIRL
	variable frmIRLTB
    variable CurrentOpenDirectory
    variable FileName
	variable SourceName
	variable WorkingDirectory
	
    set FileTypes { {"Tcl Scripts" ".irl"} {"All Files" "*"} }
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir $CurrentOpenDirectory -parent $tplIRL -title "Open Tcl Script" ]
    if { $FileName != "" } {
	   set SourceName [ file tail $FileName ]
	   set WorkingDirectory [ file dirname $FileName ]
       set $CurrentOpenDirectory [ file dirname $FileName ]
       set fid [open $FileName r]
       while {![eof $fid]} {
           $txtIRL  insert end [read $fid 1000]
       }
       close $fid
      #Enable Compile Button
	  ToolBars::EnableButton $frmIRLTB Compile
    }
	
	 set TitleFileName [ ObjectNavigator::FileRemoveNonPortablePath $FileName ]
	 wm title $tplIRL "IRL Text Editor - $TitleFileName"
	 MessagesWindow::Puts "Opening .. $TitleFileName\n"
  }
  
  #  Save File  procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc SaveFile {} {
       variable FileName
       variable CurrentOpenDirectory
       variable tplIRL
       variable txtIRL
	
	set file [ open $FileName w ]

   # initalize data and save file
	set data [$txtIRL get 1.0 {end - 1 chars}]
    WriteData $FileName $data
	
}
  
  #  Delete  procedure --
  #
  #
  # Arguments: Window
  #
  # Results:
  #
  proc Delete {} {
    variable txtIRL
    $txtIRL delete 1.0 end
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
    variable txtIRL
    $txtIRL delete MrkPromptEnd "end - 1 char"
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
    variable txtIRL
    variable CurrentCommand
    variable CurrentHistoryEvent
    set Command [ $txtIRL get MrkPromptEnd "end - 1 char" ]
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
    variable txtIRL
    if { [ $txtIRL compare insert < MrkPromptEnd ] == 1 } {
      $txtIRL mark set insert "end - 1 char"
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
    variable txtIRL
    if { [ selection own ] == $txtIRL } {
      set Error [ catch {set Selection [ selection get ]} ErrorInfo ]
      if { $Error == 0 } {
        if { [ $txtIRL compare sel.first < MrkPromptEnd ] == 1 } {
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
    variable txtIRL
    variable CurrentHistoryEvent
    switch $CurrentKey {
      Return    {
                  IRLWindow::Evaluate
                  return "break"
                }
      BackSpace {
                  if { [ $txtIRL compare insert <= MrkPromptEnd ] == 1 } {
                    return "break"
                  } else {
                    return "continue"
                  }
                }
      Delete    {
                  set Result [ TestSelection ]
                  if { $Result == "continue" } {
                    if { [ $txtIRL compare insert < MrkPromptEnd ] == 1 } {
                      return "break"
                    } else {
                      RePositionCursor
                      return "continue"
                    }
                  } else {
                    return "break"
                  }
                }
      Up      {
                    return "continue"
                }
      Down  {   
                    return "continue"
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

  # Compile  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Compile {} {
  variable frmIRLTB
  variable WorkingDirectory
  variable RootDirectory
  variable SourceName
  variable RootName
  
  set RootName [ file rootname $SourceName ]
  cd $WorkingDirectory
  
    set Helper [open "| rm -f predefined.irl $RootName.out $RootName.cpp  $RootName.o $RootName" RDWR ]
    while {[gets $Helper Line]!=-1} {
		IRLWindow::Puts "$Line\n"
    }
    close $Helper
   
  set Helper1 [open "| ln -s ../System/predefined.irl predefined.irl" RDWR ]
   while {[gets $Helper1 Line]!=-1} {
 		MessagesWindow::Puts "$Line\n"
   }
   close $Helper1
   
   set Helper2 [ open "| feIRLC $SourceName -S ../System" RDWR ] 
   while {[gets $Helper2 Line]!=-1} {
			MessagesWindow::Puts "$Line\n"
   	}
    catch { close $Helper2 }

    set Helper3 [open "| beIRLC ../Templates/Robomosp.pla $RootName.out $RootName.cpp" RDWR ]
    while {[gets $Helper3 Line]!=-1} {
		MessagesWindow::Puts "$Line\n"
    }
    catch { close $Helper3 }
	
	set Helper3 [open "|  g++ -c $RootName.cpp -ggdb -w -o $RootName.o -I ../Include/Robomosp" RDWR ]
    while {[gets $Helper3 Line]!=-1} {
		MessagesWindow::Puts "$Line\n"
    }
    catch { close $Helper3 }
	
   set Helper4 [open "|  g++ -o $RootName $RootName.o" RDWR ]
   while {[gets $Helper4 Line]!=-1} {
		MessagesWindow::Puts "$Line\n"
   }
   catch { close $Helper4 }
	
    if { [ catch { set Helper5 [open "| ./$RootName" RDWR ] } ] } {
		#Disable Load World button
		ToolBars::DisableButton $frmIRLTB LoadWorld 
	} else {
		#Enable Load World button
	    ToolBars::EnableButton $frmIRLTB LoadWorld 
		while {[gets $Helper5 Line]!=-1} {
			MessagesWindow::Puts "$Line\n"
		}
   		catch { close $Helper5 }
	} 
	 
   catch { set Helper6 [open "| mv $RootName.tjc $RootDirectory/Saves/Trajectories/$RootName.tjc" RDWR ]
 		catch { close $Helper6 }
   }
	 
    #Go back to root directory.
	 cd $RootDirectory
  }
  
  proc LoadWorld {} {
        variable RootName
       # Create World.
  	   IOIC::Worlds Create
	   
	   # Get WorldId.
	   set Index $ObjectNavigator::CurrentNode
       set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
       set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
		
	   # Load generated trajectory.
	   IOIC::World $WorldId LoadPTrajectory "$RootName.tjc"
  }
  
  #  Initialize  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplIRL
    variable txtIRL
    variable frmIRLTB
	variable frmMenuBar
	
    # initialize main variable to one
    set RobLab::IRLWindowInit 1
    
    # Initialization of IRL Text editor window.
    RobLab::CreateWindow "" wndIRLWindow "IRL Text Editor" "$RobLab::IRL_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $IRLWindow::tplIRL } $RobLab::VirtualGridSize
	
    # IRL Tool Bar
    frame $frmIRLTB -width 26	
    # Vertical scrollbar.
    scrollbar $tplIRL.scbVertical -borderwidth 1 -orient verti -command "$txtIRL yview"
    # Horizontal scrollbar.
	scrollbar $tplIRL.scbHorizontal -borderwidth 1 -orient horiz -command "$txtIRL xview"
    # IRL text widget.
    text $txtIRL -background #fffffffff -foreground #333333fff -borderwidth 1 -width 10 -height 20  -xscrollcommand "$tplIRL.scbHorizontal set" -yscrollcommand "$tplIRL.scbVertical set"
    $txtIRL tag configure TagInput -foreground #333333fff
    $txtIRL tag configure TagOutput -foreground #666666666
    $txtIRL tag configure TagError -foreground #fff333333
    $txtIRL tag configure TagPrompt -foreground #333666333
    $txtIRL tag configure TagEmpty -foreground #aaaaaa000
    $txtIRL tag configure TagInfo -foreground #8c8464000
    
	# Packing canvases
	# Packing Menu Buttons.
	#pack [ frame $frmMenuBar -width 5 -relief ridge -borderwidth 2 ] -side top
	#  pack [ menubutton $frmMenuBar.file -text File -menu $frmMenuBar.file.m ] -side left -padx 2 -pady 2 -anchor nw -fill none
    #  pack [ menubutton $frmMenuBar.search -text Search -menu $frmMenuBar.search.m ] -side left -padx 2 -pady 2 -anchor nw -fill none
    pack $frmIRLTB -side top -fill both -expand true
    pack $tplIRL.scbHorizontal -side bottom -fill x
	pack $tplIRL.scbVertical -side right -fill y
	pack $txtIRL -side top -fill both -expand true
	
   #menu $frmMenuBar.file.m
   #menu $frmMenuBar.search.m
	
    # Initialization of binds
    bind $tplIRL <FocusIn> {
      focus $IRLWindow::txtIRL
    }
    bind $txtIRL <KeyPress> {
      set Result [ IRLWindow::ProcessKeyPress %K ]
      eval $Result
    }
    bind $txtIRL <<Copy>> {
    }
    bind $txtIRL <<Cut>> {
      set Result [ IRLWindow::TestSelection ]
      eval $Result
    }
    bind $txtIRL <<Paste>> {
      IRLWindow::RePositionCursor
      catch {%W insert insert [ selection get -selection CLIPBOARD ]}
      break;
    }
    # Initialization of console window tool bar
    set IRLToolBar {   {      Open           "ROpen"          imgONTBOpen    normal  { IRLWindow::Open } }
                      		   {      Save           "RSave"           imgONTBSave    normal  { IRLWindow::SaveFile } }
                      		   {      SaveAs       "RSave As"      imgONTBSaveAs  normal  { ObjectNavigator::SaveFile 1 } }
				      		   {      Delete         "RDelete"          imgMWTBDelete  normal  { IRLWindow::Delete } }
				      		   {      Compile       "RCompile"       imgCVSIcon3    disabled  { IRLWindow::Compile } }
                               {      LoadWorld    "RLoad World"  imgCVSIcon1    disabled  { IRLWindow::LoadWorld } }
				      		   {      Simulate      "RSimulate"      imgCVSIcon19   disabled  { source "./Saves/Console/WorldInvKineSim.tcl" } }
                          }
    ToolBars::Create $frmIRLTB $IRLToolBar Horizontal
    SetPrompt
  }
}
