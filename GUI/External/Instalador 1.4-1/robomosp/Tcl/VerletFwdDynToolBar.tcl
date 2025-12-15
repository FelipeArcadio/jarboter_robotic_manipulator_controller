#----------------------------------------------------------------------------------------------------------------------------------
# VerletFwdDynToolBar.tcl
# 		This file implements the TCL code for creating and managing the Forward Dynamics
#		ToolBar for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2006-02-25
#
# Requirements:   RobLab.tcl
#			              ObjNavMain.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  # Images of Play Back
  image create photo imgPBTBStop -file ./Images/PlayBackToolBar/stop.gif
  image create photo imgPBTBPause -file ./Images/PlayBackToolBar/pause.gif
  image create photo imgPBTBPrevious -file ./Images/PlayBackToolBar/previous.gif
  image create photo imgPBTBPlay -file ./Images/PlayBackToolBar/play.gif
  image create photo imgPBTBNext -file ./Images/PlayBackToolBar/next.gif
  image create photo imgPBTBFirst -file ./Images/PlayBackToolBar/first.gif
  image create photo imgPBTBEnd -file ./Images/PlayBackToolBar/end.gif
  image create photo imgGPTBButtonq -file ./Images/GNUPlotToolBar/q.gif

# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval VerletFwdDynToolBar {

  # Initialization of global variables
  variable tplFDyn .wndVerletFwdDynToolBar            #Main VerletFwdDyn ToolBar Window.
  variable frmFDynAll $tplFDyn.frmFDynAll		            #Main VerletFwdDyn frame.
  variable frmFDynTB $tplFDyn.frmToolBar
  variable CurrentOpenDirectory $HOME/Saves/Meschach
  variable VerletFwdDynToolBarInit 0
  variable VerletFwdDynToolDebug 0
  variable TorquesNULL 1
  variable DOF
  variable WorldId
  variable PRobotId
  variable FileName 0
  
  #  LoadTorques  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc LoadTorques {} {
    variable tplFDyn
    variable frmFDynTB
    variable CurrentOpenDirectory
    variable FileName
	variable InFile
	variable DOF
    variable TorquesNULL
    global Time
    global TimeStep
    set TorquesNULL 0
    set FileTypes { {"Tcl Scripts" ".mech"} {"All Files" "*"} }
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir $CurrentOpenDirectory -parent $tplFDyn -title "Open Tcl Script" ]
    if { $FileName != "" } {
		 set FileName [ ObjectNavigator::FileRemoveNonPortablePath $FileName ]
		 set $CurrentOpenDirectory [ file dirname $FileName ]
    		 wm title $tplFDyn "Torques: [ file tail $FileName ] "
    		 MessagesWindow::Puts "Loading Torques file \"$FileName\" ...\n"
    		  MessagesWindow::Puts "Torques file loaded. \n\n"
    
    		#These lines of code return the Robots number of degrees of freedom
    		set InFile [ open $FileName r ] 
      	gets $InFile Line
      	set Time [string range $Line 1 8  ]
      	set TimeStep  [string range $Line 11 18  ]
    		$tplFDyn.frmTime.entCell config -state disabled
    		$tplFDyn.frmTimeS.entCell config -state disabled
    		for {set I 1 } { $I <= $DOF} {incr I} {
          		gets $InFile Line
          		global Vel_$I
          		set Vel_$I   [string range $Line 1 8  ]
          		$tplFDyn.frmRow$I.entCell_$I  config -state disabled
    		}  
    		close $InFile
	}           
}

  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplFDyn
    variable VerletFwdDynToolBarInit
    variable VerletFwdDynToolDebug
    variable FileName
    variable TorquesNULL
    global FwdDyn
    set VerletFwdDynToolBarInit 0 
    set VerletFwdDynToolDebug 0
    set FwdDyn 0
    set TorquesNULL 1
    set FileName " "
    
    for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
    }
    
    wm withdraw $VerletFwdDynToolBar::tplFDyn
  }
  
  #  PreviousPoint  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc PreviousPoint {} {
  
  }

  #  NextPoint  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc NextPoint {} {
  
  
  }
 
  #  FirstPoint  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc FirstPoint {} {
  
  
  }

  #  FinalPoint  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc FinalPoint {} {
  
  
}

  #  Play  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Play {  } {
    #global variables
    global Time
    global TimeStep
    global TimeDelay
    variable DOF
    variable WorldId
    variable PRobotId
    variable VelList
    variable TorquesNULL
    variable FileName
    
    set VelList {}
    #MessagesWindow::Puts "TimeDelay:$TimeDelay.\n"
    for {set I 1 } { $I <= $DOF} {incr I} {
        global Vel_$I
        set Vel "Vel_$I"
        lappend VelList [ expr  $$Vel ]
    }
    if { $TimeDelay =="On"} {
       IOIC::World $WorldId FwdDynWorldSim $PRobotId $Time $VelList TIME_DELAY_ON 2 $TimeStep $TorquesNULL $FileName
    } else {
      IOIC::World $WorldId FwdDynWorldSim $PRobotId $Time $VelList TIME_DELAY_OFF 2 $TimeStep $TorquesNULL $FileName
    }  
 }

  #  Debug  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Debug {} {
  
  }
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplFDyn
    variable frmFDynTB
    variable FileName
    variable VerletFwdDynToolBarInit
    variable DOF
    variable WorldId
    variable PRobotId
    variable CellWidth
    global Time
    global TimeStep
    global TimeDelay
    set Time 0
    set TimeStep 0
    set VerletFwdDynToolBarInit 1
    set CellWidth 8
    
    # Error Message if NumWorlds is 0.
    if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
      MessagesWindow::Puts "Error:  Not a world has been loaded. Please load a World definition file.\n"
    } else { 
      # Get WorldId
      set Index $ObjectNavigator::WorldNodeId
      set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
      set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
      # Error Message if NumPRobots is 0.
      if {  [ llength [ IOIC::World $WorldId LsPRobots ] ] == 0 } {
        error "Error: There are not robots in current world, please load a Robot definition file."
      } 
      
	# Initialization of Forward Dynamics window
    RobLab::CreateWindow "" wndVerletFwdDynToolBar "Forward Dynamics ToolBar" "$RobLab::VerletFwdDynTB_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { VerletFwdDynToolBar::Delete } $RobLab::VirtualGridSize
   
    # Tool Bars
    frame $frmFDynTB -width 26 -relief groove -borderwidth 2
    pack $frmFDynTB -side top -fill both -expand true
    
    #Time Frame
    frame $tplFDyn.frmTime -width 26 -relief groove -borderwidth 2
    label  $tplFDyn.frmTime.label -text "Total Simulation Time"
    pack  $tplFDyn.frmTime.label  -side left
    entry $tplFDyn.frmTime.entCell -textvariable Time -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left -state normal
    pack $tplFDyn.frmTime.entCell -side left 
    label  $tplFDyn.frmTime.segs -text "segs"
    pack  $tplFDyn.frmTime.segs  -side left
    pack  $tplFDyn.frmTime -side top -fill both -expand true
    
    #Step Time Frame
    frame $tplFDyn.frmTimeS -width 26 -relief groove -borderwidth 2
    label  $tplFDyn.frmTimeS.label -text "Time Step"
    pack  $tplFDyn.frmTimeS.label  -side left
    entry $tplFDyn.frmTimeS.entCell -textvariable TimeStep -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
    pack $tplFDyn.frmTimeS.entCell -side left 
    label  $tplFDyn.frmTimeS.segs -text "segs"
    pack  $tplFDyn.frmTimeS.segs  -side left
    pack  $tplFDyn.frmTimeS -side top -fill both -expand true
    
    #Getting PRobot DOF
    set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
    set DOF [ IOIC::PRobot $PRobotId GetDOF ]
    for {set I 1 } { $I <= $DOF} {incr I} {
       #global variables
        global Vel_$I
        set Vel_$I 0  
        # Row i
        frame $tplFDyn.frmRow$I -width 26 -relief raised -borderwidth 1
          label  $tplFDyn.frmRow$I.label -text "Vel_$I"
          pack  $tplFDyn.frmRow$I.label  -side left -fill both -expand true
          entry $tplFDyn.frmRow$I.entCell_$I -textvariable Vel_$I -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
          pack $tplFDyn.frmRow$I.entCell_$I -side left 
        pack $tplFDyn.frmRow$I -side top -fill both -expand true 
    }

    frame $tplFDyn.frmTimeDelay -width 26 -relief groove -borderwidth 2
      label  $tplFDyn.frmTimeDelay.label -text "Time Delay:"
      pack  $tplFDyn.frmTimeDelay.label  -side left
      radiobutton  $tplFDyn.frmTimeDelay.on -variable TimeDelay -value On -text On 
      pack  $tplFDyn.frmTimeDelay.on  -side left
      radiobutton  $tplFDyn.frmTimeDelay.off -variable TimeDelay -value Off -text Off
      pack  $tplFDyn.frmTimeDelay.off  -side left
    pack  $tplFDyn.frmTimeDelay -side top -fill both -expand true
        
    # Initialization of tool bar
    set FDynToolBar { {    Stop         "RStop"                     imgPBTBStop        disabled  {  } }
                                  {    Pause      "RPause"                   imgPBTBPause     disabled  {  } }
                                  {    Previous   "RPrevious"               imgPBTBPrevious  disabled {  VerletFwdDynToolBar::PreviousPoint } }
                                  {    Play         "RPlay"                     imgPBTBPlay         normal   { VerletFwdDynToolBar::Play }    }
                                  {    Next         "RNext"                     imgPBTBNext        disabled {  VerletFwdDynToolBar::NextPoint } }
                                  {    First         "RFirst"                     imgPBTBFirst         disabled {  VerletFwdDynToolBar::FirstPoint } }
                                  { "" }
                                  {    Torques   "RLoad Torques"      imgONTBOpen        normal {  VerletFwdDynToolBar::LoadTorques } }
                                  {    State         "RPlot State Variables"   imgGPTBButtonq    normal  { FwdDynGNUPlot::SetGraphGNUPlot 1 } }
                            }
    ToolBars::Create $frmFDynTB $FDynToolBar Horizontal 
  }
}  
 
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
