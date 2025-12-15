#----------------------------------------------------------------------------------------------------------------------------------
# FwdDynGNUPlot.tcl
# 		This file implements the TCL code for creating and managing the Forward Dynamics
#		GNUPlot Windows for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-07-30
#
# Requirements:   RobLab.tcl
#			                 ObjNavMain.tcl
#                 	         GNUPlot
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  # Images of object navigator
  image create photo imgCVSIcon35 -file ./Images/ObjectNavigator/Icon35.gif

  # Images of GNUPlot canvas
  image create photo imgGPTBButton01 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton02 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton03 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton04 -file ./Images/GNUPlotToolBar/Icon35.gif
  image create photo imgGPTBButton05 -file ./Images/GNUPlotToolBar/Icon35.gif

# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval FwdDynGNUPlot {

  # Procedures :
  # proc Open {}
  # proc Initialize {}
  # proc SetGraphGNUPlot {}
  # proc GraphGNUPlot {Canvas Xwidth Ywidth Index TopFrame Option}

  # Initialization of global variables
  variable tplGP .wndFwdDynGNUPlot             #Main GNUPlot Window.
  variable frmGPAll $tplGP.frmGPAll				#Main GNUPlot frame.
  variable CurrentOpenDirectory $HOME/Saves/GNUPlot/ForwardDynamics
  variable frmGPTB $tplGP.frmToolBar
  variable FileName
  variable DOF
  variable FwdDynGNUPlotInit 0

   #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Open {} {
    variable tplGP
    variable CurrentOpenDirectory
    variable FileName
	variable InFile
	variable DOF
    set FileTypes { {"Tcl Scripts" ".plot"} {"All Files" "*"} }
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir $CurrentOpenDirectory -parent $tplGP -title "Open Tcl Script" ]
    if { $FileName != "" } {
		  set FileName [ ObjectNavigator::FileRemoveNonPortablePath $FileName ]
	  
		  #These lines of code return the Robots number of degrees of freedom
		  set InFile [ open $FileName r ] 
		  gets $InFile Line
		  set DOF [string index $Line 1] 
		  close $InFile
	  
		  set $CurrentOpenDirectory [ file dirname $FileName ]
    }
    wm title $tplGP "Forward Dynamics GNUPlot - $FileName"
  }

   #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplGP
    variable FwdDynGNUPlotInit
    set FwdDynGNUPlotInit 0 
    
	for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .windowfdyn$i]
            if {$isgraph(ok)} {
              destroy .windowfdyn$i
            }
    }
	
    wm withdraw $FwdDynGNUPlot::tplGP
  }

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ReturnFileName {} {
    variable tplGP
    set FileName  [ ObjectNavigator::GetFileName $ObjectNavigator::WorldNodeId ]
    set FileName  [ file tail $FileName ]
    set FileName  [ file rootname $FileName ]
    set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryFwdDynGNUPlotDatas/$FileName\.plot"
    #wm title $tplGP "ForwardDynamicsGNUPlot - $FileName"
    wm title . "Forward Dynamics GNUPlot - $FileName"
    MessagesWindow::Puts "Opening .. $FileName\n"
    return $FileName
    
 }

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
 proc AssignFileName {} {
   variable FileName
   set FileName  [ FwdDynGNUPlot::ReturnFileName ]
 }

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetGraphGNUPlot { Option } {
    variable XWidth 320
    variable YWidth 320
    variable FileName
    variable InFile
	variable DOF
    set FileName [ FwdDynGNUPlot::ReturnFileName ]
    #These lines of code return the Robots number of degrees of freedom
	set InFile [ open $FileName r ] 
	gets $InFile Line
	set DOF [string index $Line 1] 
	close $InFile 

    switch $Option {
       1 {
				 for { set i 1 } { $i <= 3 } { incr i } {
					set isgraph(ok) [winfo exists .windowfdyn$i]
					 if {$isgraph(ok)} {
						  destroy .windowfdyn$i
				     }
					toplevel .windowfdyn$i

					if {$i==1} {
						 wm geometry .windowfdyn$i "$RobLab::FwdDynGPw1_Geometry"
						 wm title .windowfdyn$i "Position Vs. Time"
					}
					if {$i==2} {
					  wm geometry .windowfdyn$i "$RobLab::FwdDynGPw2_Geometry"
					  wm title .windowfdyn$i "Velocity Vs. Time"
					}
                    if {$i==3} {
					  wm geometry .windowfdyn$i "$RobLab::FwdDynGPw3_Geometry"
					   wm title .windowfdyn$i "Acceleration Vs. Time"
					   focus .windowfdyn$i
					}
					frame .windowfdyn$i.frame
					canvas .windowfdyn$i.can -width $XWidth -height $YWidth -bg white

					if {[catch {
						  GraphGNUPlot .windowfdyn$i.can $XWidth $YWidth $i .windowfdyn$i.frame $Option
					}]} {
                
					source gp.tk
					if { $::tcl_platform(platform) == "windows" } {
						 pack .window$i.can -in .windowfdyn$i.frame
					} else {
						canvas .windowfdyn$i.can -width $XWidth -height $YWidth -bg white
						 pack .windowfdyn$i.can -in .windowfdyn$i.frame
					}
					gnuplot .windowfdyn$i.can

				   if { $::tcl_platform(platform) == "windows" } {
						 exec cmd.exe/c del "in.gp$i"
						 exec cmd.exe/c del.exe "gp.tk"
				   } else {
						 exec rm in.gp$i
						 exec rm gp.tk
				   }
				}
				pack .windowfdyn$i.can -in .windowfdyn$i
			}
        }
    
    }
 }
 
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
 proc GraphGNUPlot {can xw yw i top Option} {
    variable FileName
	variable DOF
    
	set isgraph(ok) [winfo exists $can]
    if {$isgraph(ok)} {
    destroy $can
    }
    
	set isbitmap(ok) [winfo exists .top.bitmap]
    if {$isbitmap(ok)} {
    destroy .top.bitmap
    }

    set Data [open "in.gp$i" w]
    puts $Data "set term tkcanvas"
    puts $Data "set out \"gp.tk\""
    puts $Data "set key"
    puts $Data "set grid"
    puts $Data "set autoscale x"
    puts $Data "set autoscale y"
    puts $Data "set tmargin 5"
    puts $Data "set bmargin 5"
    puts $Data "set xlabel \"Time(s)\" 0,-1"

    #Position
    if {$i==1} {
      
      puts $Data "set ylabel \"Position\" 0,2"
      puts -nonewline $Data "plot "
	  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1:[ expr ( $j+1)  ] smooth csplines title 'Joint $j',"
			  
	  }
      puts -nonewline $Data "\"$FileName\""
	  puts -nonewline $Data " using 1:[ expr ( $DOF+1)  ] smooth csplines title 'Joint $DOF' "
	  
    }

    #Velocity
    if {$i==2} {
		  puts $Data "set ylabel \"Velocity\" 0,2"
		  puts -nonewline $Data "plot "
		  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ] smooth csplines title 'Joint $j',"
			  
		  }
		  puts -nonewline $Data "\"$FileName\""
		  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ] smooth csplines title 'Joint $DOF'"
		  
    }
	
	 #Acceleration
    if {$i==3} {
		  puts $Data "set ylabel \"Acceleration\" 0,2"
		  puts -nonewline $Data "plot "
		  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+$DOF+1)  ] smooth csplines title 'Joint $j',"
		  }
	  
		  puts -nonewline $Data "\"$FileName\""
		  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+$DOF+1)  ] smooth csplines title 'Joint $DOF'"
   }
    
    close $Data

    if { $::tcl_platform(platform) == "windows" } {
      exec Tcl/GNUPlot/wgnupl32.exe "in.gp$i"
    } else {
      exec gnuplot "in.gp$i"
    }
    source gp.tk
    canvas $can -width $xw -height $yw -bg white
    pack $can -in $top
    gnuplot $can

    if { $::tcl_platform(platform) == "windows" } {
      #exec cmd.exe/c del "in.gp$"
      #exec cmd.exe/c del.exe "gp.tk"
    } else {
      exec rm in.gp$i
      exec rm gp.tk
    }
 }

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplGP
    variable frmGPTB
    variable FileName
    variable FwdDynGNUPlotInit
    variable CurrentOpenSaveDirectoryFwdDynGNUPlotData
    set FwdDynGNUPlotInit 1
    
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndFwdDynGNUPlot "Forward Dynamics GNUPlot" "$RobLab::FwdDynGP_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { FwdDynGNUPlot::Delete } $RobLab::VirtualGridSize

    # Initialization of binds

    # Tool Bars
    frame $frmGPTB -width 26
    pack $frmGPTB -side top -fill both -expand true

    # Initialization of tool bar
    set GPToolBar { {    Open     "ROpen"                      imgONTBOpen    normal  { FwdDynGNUPlot::Open } }
                    {    Delete   "RDelete"                    imgONTBDelete  normal  { FwdDynGNUPlot::Delete } }
                    { "" }
                    {    Button01 "RGraphs"          imgGPTBButton01 normal { FwdDynGNUPlot::SetGraphGNUPlot 1 } }
                    
				   }
    ToolBars::Create $frmGPTB $GPToolBar Horizontal
    set FileName  [ FwdDynGNUPlot::ReturnFileName ]
    
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
