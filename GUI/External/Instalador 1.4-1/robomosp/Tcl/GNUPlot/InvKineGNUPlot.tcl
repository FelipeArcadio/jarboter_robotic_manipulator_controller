#----------------------------------------------------------------------------------------------------------------------------------
# InvKineGNUPlot.tcl
# 		This file implements the TCL code for creating and managing the Inverse Kinematics
#		GNUPlot Windows for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-11-10
#
# Requirements:   RobLab.tcl
#			   ObjNavMain.tcl
#                 	   GNUPlot
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  # Images of object navigator
  image create photo imgCVSIcon35 -file ./Images/ObjectNavigator/Icon35.gif

  # Images of GNUPlot canvas
  image create photo imgGPTBButton01 -file ./Images/GNUPlotToolBar/q.gif
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
namespace eval InvKineGNUPlot {

  # Procedures :
  # proc Open {}
  # proc Initialize {}
  # proc SetGraphGNUPlot {}
  # proc GraphGNUPlot {Canvas Xwidth Ywidth Index TopFrame Option}

  # Initialization of global variables
  variable tplGP .wndInvKineGNUPlot             #Main GNUPlot Window.
  variable frmGPAll $tplGP.frmGPAll				#Main GNUPlot frame.
  variable CurrentOpenDirectory $HOME/Saves/GNUPlot/InverseKinematics
  variable frmGPTB $tplGP.frmToolBar
  variable FileName
  variable DOF
  variable InvKineGNUPlotInit 0

  #  Open  --
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
    wm title $tplGP "Inverse Kinematics GNUPlot - $FileName"
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
    variable InvKineGNUPlotInit
    set InvKineGNUPlotInit 0 
    
	for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .windowikine$i]
            if {$isgraph(ok)} {
              destroy .windowikine$i
            }
    }
	
#wm withdraw $InvKineGNUPlot::tplGP
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
    set FileName  [ ObjectNavigator::GetFileName  $ObjectNavigator::WorldNodeId ]
    set FileName  [ file tail $FileName ]
    set FileName  [ file rootname $FileName ]
    set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryInvKineGNUPlotDatas/$FileName\.plot"
    MessagesWindow::Puts "Opening .. $FileName\n"
    wm title . "Inverse Kinematics GNUPlot - $FileName"
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
   set FileName  [ InvKineGNUPlot::ReturnFileName ]
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
    set FileName [ InvKineGNUPlot::ReturnFileName ]
    
    #These lines of code return the Robots number of degrees of freedom
	set InFile [ open $FileName r ] 
	gets $InFile Line
	set DOF [string index $Line 1] 
	close $InFile 

    switch $Option {
       1 {
				 for { set i 1 } { $i <= 3 } { incr i } {
					set isgraph(ok) [winfo exists .windowikine$i]
					 if {$isgraph(ok)} {
						destroy .windowikine$i
				     }
					toplevel .windowikine$i

					if {$i==1} {
						 wm geometry .windowikine$i "$RobLab::InvKineGPw1_Geometry"
						 wm title .windowikine$i "Position Vs. Time"
					}
					if {$i==2} {
					  wm geometry .windowikine$i "$RobLab::InvKineGPw2_Geometry"
					  wm title .windowikine$i "Velocity Vs. Time"
					}
					if {$i==3} {
					  wm geometry .windowikine$i "$RobLab::InvKineGPw3_Geometry"
					   wm title .windowikine$i "Acceleration Vs. Time"
					   focus .windowikine$i
					}
					frame .windowikine$i.frame
					canvas .windowikine$i.can -width $XWidth -height $YWidth -bg white

					if {[catch {
						  GraphGNUPlot .windowikine$i.can $XWidth $YWidth $i .windowikine$i.frame $Option
					}]} {
                
					source gp.tk
					if { $::tcl_platform(platform) == "windowikines" } {
						 pack .windowikine$i.can -in .windowikine$i.frame
					} else {
						canvas .windowikine$i.can -width $XWidth -height $YWidth -bg white
						 pack .windowikine$i.can -in .windowikine$i.frame
					}
					gnuplot .windowikine$i.can

				   if { $::tcl_platform(platform) == "windowikines" } {
						 exec cmd.exe/c del "in.gp$i"
						 exec cmd.exe/c del.exe "gp.tk"
				   } else {
						 exec rm in.gp$i
						 exec rm gp.tk
				   }
				}
				pack .windowikine$i.can -in .windowikine$i
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
			  #MessagesWindow::Puts "[ expr ( $j+1)  ]  \n"
	  }
      puts -nonewline $Data "\"$FileName\""
	  puts -nonewline $Data " using 1:[ expr ( $DOF+1)  ] smooth csplines title 'Joint $DOF' "
	  #MessagesWindow::Puts "[ expr ( $j+1)  ] \n"
    }

    #Velocity
    if {$i==2} {
		  puts $Data "set ylabel \"Velocity\" 0,2"
		  puts -nonewline $Data "plot "
		  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ] smooth csplines title 'Joint $j',"
			  #MessagesWindow::Puts "[ expr ( $j+$DOF+1)  ]  \n"
		  }
		  puts -nonewline $Data "\"$FileName\""
		  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ] smooth csplines title 'Joint $DOF'"
		  #MessagesWindow::Puts "[ expr ( $j+$DOF+1)  ] \n"
    }
	
	 #Acceleration
    if {$i==3} {
		  puts $Data "set ylabel \"Acceleration\" 0,2"
		  puts -nonewline $Data "plot "
		  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+$DOF+1)  ] smooth csplines title 'Joint $j',"
			  #MessagesWindow::Puts "[ expr ( $j+$DOF+$DOF+1)  ]  \n"
		  }
	  
		  puts -nonewline $Data "\"$FileName\""
		  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+$DOF+1)  ] smooth csplines title 'Joint $DOF'"
		  #MessagesWindow::Puts "[ expr ( $j+$DOF+$DOF+1)  ]  \n"
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
    variable InvKineGNUPlotInit
    variable CurrentOpenSaveDirectoryInvKineGNUPlotData
    set InvKineGNUPlotInit 1
    
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndInvKineGNUPlot "Inverse Kinematics GNUPlot" "$RobLab::InvKineGP_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { InvKineGNUPlot::Delete } $RobLab::VirtualGridSize

    # Initialization of binds

    # Tool Bars
    frame $frmGPTB -width 26
    pack $frmGPTB -side top -fill both -expand true

    # Initialization of tool bar
    set GPToolBar { {    Open     "ROpen"                      imgONTBOpen    normal  { InvKineGNUPlot::Open } }
                    {    Delete   "RDelete"                    imgONTBDelete  normal  { InvKineGNUPlot::Delete } }
                    { "" }
                    {    Button01 "RGraphs"          imgGPTBButton01 normal { InvKineGNUPlot::SetGraphGNUPlot 1 } }
                    
				   }
    ToolBars::Create $frmGPTB $GPToolBar Horizontal
    set FileName  [ InvKineGNUPlot::ReturnFileName ]
    
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
