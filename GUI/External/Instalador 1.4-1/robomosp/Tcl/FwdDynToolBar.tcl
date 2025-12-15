#----------------------------------------------------------------------------------------------------------------------------------
# FwdDynToolBar.tcl
# 		This file implements the TCL code for creating and managing the Forward Dynamics
#		ToolBar for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-05-25
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
namespace eval FwdDynToolBar {

  # Initialization of global variables
  variable tplFDyn .wndFwdDynToolBar            #Main FwdDyn ToolBar Window.
  variable frmFDynAll $tplFDyn.frmFDynAll		#Main FwdDyn frame.
  variable frmFDynTB $tplFDyn.frmToolBar
  variable FwdDynToolBarInit 0
  variable FwdDynToolDebug 0
  variable DOF
  variable WorldId
  variable PRobotId
  variable CurrentOpenDirectory $HOME/Saves/Meschach
  variable TorquesNULL 1
  variable FileName 0
  

  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplFDyn
    variable FwdDynToolBarInit
    variable FwdDynToolDebug
    global FwdDyn
    set FwdDynToolBarInit 0 
    set FwdDynToolDebug 0
    set FwdDyn 0
    
    for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
    }
    
    wm withdraw $FwdDynToolBar::tplFDyn
  }
 
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
		#$tplFDyn.frmTimeS.entCell config -state disabled
    		for {set I 1 } { $I <= $DOF} {incr I} {
          		gets $InFile Line
          		global Vel_$I
          		set Vel_$I   [string range $Line 1 8  ]
          		$tplFDyn.frmRow$I.entCell_$I  config -state disabled
     		}  
     		close $InFile
	}             
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
    if { $TimeDelay=="On"} {
      IOIC::World $WorldId FwdDynWorldSim $PRobotId $Time $VelList TIME_DELAY_ON 1 0 $TorquesNULL $FileName
    } else {
      IOIC::World $WorldId FwdDynWorldSim $PRobotId $Time $VelList TIME_DELAY_OFF 1 0 $TorquesNULL $FileName
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
    variable FwdDynToolBarInit
    variable DOF
    variable WorldId
    variable PRobotId
    global Time
    set Time 0
    global TimeDelay
    set FwdDynToolBarInit 1
    set CellWidth 8
    
    # Error Message if NumWorlds is 0.
    if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
      error "Error:  Not a world has been loaded, please load a World definition file."
    } else { 
      # Get WorldId
      set Index $ObjectNavigator::WorldNodeId
      set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
      set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
      # Error Message if NumPRobots is 0.
      if {  [ llength [ IOIC::World $WorldId LsPRobots ] ] == 0 } {
        error "Error: There are not robots in current world, please load a Robot definition file."
      }  
    }
    
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndFwdDynToolBar "Ode45 Forward Dynamics ToolBar" "$RobLab::FwdDynTB_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { FwdDynToolBar::Delete } $RobLab::VirtualGridSize
   
    # Tool Bars
    frame $frmFDynTB -width 26 -relief raised -borderwidth 1
    pack $frmFDynTB -side top -fill both -expand true
    
    #Time Frame
    frame $tplFDyn.frmTime -width 26 -relief raised -borderwidth 1
    label  $tplFDyn.frmTime.label -text "Total Simulation Time"
    pack  $tplFDyn.frmTime.label  -side left
    entry $tplFDyn.frmTime.entCell -textvariable Time -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
    pack $tplFDyn.frmTime.entCell -side left 
    label  $tplFDyn.frmTime.segs -text "segs"
    pack  $tplFDyn.frmTime.segs  -side left
    pack  $tplFDyn.frmTime -side top -fill both -expand true
    
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

    frame $tplFDyn.frmTimeDelay -width 26 -relief raised -borderwidth 1
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
                                {    Previous   "RPrevious"               imgPBTBPrevious  disabled {  FwdDynToolBar::PreviousPoint } }
                                {    Play         "RPlay"                     imgPBTBPlay         normal   { FwdDynToolBar::Play }    }
                                {    Next         "RNext"                     imgPBTBNext        disabled {  FwdDynToolBar::NextPoint } }
                                {    First         "RFirst"                     imgPBTBFirst         disabled {  FwdDynToolBar::FirstPoint } }
                                { "" }
					  {    Torques   "RLoad Torques"      imgONTBOpen        normal {  FwdDynToolBar::LoadTorques } }
                                {    State         "RPlot State Variables"   imgGPTBButtonq    normal  { FwdDynGNUPlot::SetGraphGNUPlot 1 } }
                            }
    ToolBars::Create $frmFDynTB $FDynToolBar Horizontal 
  }
 
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
