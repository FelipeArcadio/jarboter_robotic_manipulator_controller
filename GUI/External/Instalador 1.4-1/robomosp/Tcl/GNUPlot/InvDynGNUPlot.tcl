#----------------------------------------------------------------------------------------------------------------------------------
# InvDynGNUPlot.tcl
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
#			      ObjNavMain.tcl
#                 GNUPlot
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

namespace eval InvDynGNUPlot {

  # Procedures :
  # proc Open {}
  # proc Initialize {}
  # proc SetGraphGNUPlot {}
  # proc GraphGNUPlot {Canvas Xwidth Ywidth Index TopFrame Option}

  # Initialization of global variables
  variable tplGP .wndInvDynGNUPlot             #Main GNUPlot Window.
  variable frmGPAll $tplGP.frmGPAll      #Main GNUPlot frame.
  variable CurrentOpenDirectory $HOME/Saves/GNUPlot/InverseDynamics
  variable frmGPTB $tplGP.frmToolBar
  variable FileName
  variable DOF
  variable InvDynGNUPlotInit 0

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
    wm title $tplGP "Inverse Dynamics GNUPlot - $FileName"
  }

  proc Delete {} {
    variable tplGP
    variable InvDynGNUPlotInit
    set InvDynGNUPlotInit 0 
    
     for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
           } 
     }
    
   
   
   #wm withdraw $InvDynGNUPlot::tplGP
  }


  proc ReturnFileName { Option FileType } {
    variable tplGP
    set FileName  [ ObjectNavigator::GetFileName $ObjectNavigator::WorldNodeId ]
    set FileName  [ file tail $FileName ]
    set FileName  [ file rootname $FileName ]
    if { ($Option == 3) } {
         set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryEnergyGNUPlotDatas/$FileName\.plot"
    } else {
         if { ($FileType == 0) } {
            set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryInvDynGNUPlotDatas/$FileName\.plot"
         } elseif { ($FileType == 1) } {
            set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryInvDynGNUPlotDatas/$FileName\-vel.plot"
         } elseif { ($FileType == 2 ) } {
            set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryInvDynGNUPlotDatas/$FileName\-accel.plot"
         }
    }
   MessagesWindow::Puts "Opening .. $FileName\n"
    wm title . "Inverse Dynamics GNUPlot - $FileName" 
    return $FileName
 }

 proc AssignFileName {} {
   variable FileName
   set FileName  [ InvDynGNUPlot::ReturnFileName ]
 }

  proc SetGraphGNUPlot { Option FileType } {
    variable XWidth
    variable YWidth
    variable FileName
    variable InFile
	variable DOF
    set FileName [ InvDynGNUPlot::ReturnFileName $Option $FileType ]
    #These lines of code return the Robots number of degrees of freedom
	set InFile [ open $FileName r ] 
	gets $InFile Line
	set DOF [string index $Line 1] 
	close $InFile 
    
    switch $Option {
     1 {
            set XWidth 610
            set YWidth 400
            
             for { set i 1 } { $i <= 2 } { incr i } {
                set isgraph(ok) [winfo exists .window$i ]
			    if {$isgraph(ok)} {
				   destroy .window$i
                }
             }   
            toplevel .window1

            wm geometry .window1 "$RobLab::InvDynGPw1_Geometry"
            wm title .window1 "Torques"
            frame .window1.frame
            canvas .window1.can -width $XWidth -height $YWidth -bg white

            if {[catch {
                  GraphGNUPlot .window1.can $XWidth $YWidth 1 .window1.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 canvas .window1.can -width $XWidth -height $YWidth -bg white
                 pack .window1.can -in .window1.frame
               } else {
                 canvas .window1.can -width $XWidth -height $YWidth -bg white
                 pack .window1.can -in .window1.frame
               }
               gnuplot .window1.can

               if { $::tcl_platform(platform) == "windows" } {
                 #exec cmd.exe/c del "in.gp1"
                 #exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp1
                 exec rm gp.tk
               }
             }
            pack .window1.can -in .window1 -side top -fill both -expand true
       }
     2 {
			set isgraph(ok) [winfo exists .window1]
				if {$isgraph(ok)} {
					destroy .window1
				}

            toplevel .window1
            wm geometry .window1 "$RobLab::InvKineGPw1_Geometry"
            wm title .window1 "ABI"

            frame .window1.frame
            canvas .window1.can -width $XWidth -height $YWidth -bg white
            if {[catch { 
                  GraphGNUPlot .window1.can $XWidth $YWidth 1 .window1.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 canvas .window1.can -width $XWidth -height $YWidth -bg white
                 pack .window1.can -in .window1.frame
               } else {
                 canvas .window1.can -width $XWidth -height $YWidth -bg white
                 pack .window1.can -in .window1.frame
               }
               gnuplot .window1.can

               if { $::tcl_platform(platform) == "windows" } {
                 #exec cmd.exe/c del "in.gp1"
                 #exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp1
                 exec rm gp.tk
               }
             }
            pack .window1.can -in .window1 -fill both -expand true
       }
      3 {
             set XWidth 320
             set  YWidth 320
             for { set i 1 } { $i <= 3 } { incr i } {
                set isgraph(ok) [winfo exists .window$i ]
			    if {$isgraph(ok)} {
				   destroy .window$i
                }
                toplevel .window$i
            
                if {$i==1} {
						 wm geometry .window$i "$RobLab::InvKineGPw1_Geometry"
						 wm title .window$i "Kinetic Energy Vs. Time"
                }
		        if {$i==2} {
					  wm geometry .window$i "$RobLab::InvKineGPw2_Geometry"
					  wm title .window$i "Potential Energy Vs. Time"
               }
               if {$i==3} {
					  wm geometry .window$i "$RobLab::InvKineGPw3_Geometry"
					  wm title .window$i "Kinetical + Potential Vs. Time"
                }
            
               frame .window$i.frame
               canvas .window$i.can -width $XWidth -height $YWidth -bg white

               if {[catch {
                   GraphGNUPlot .window$i.can $XWidth $YWidth $i .window$i.frame $Option
               }]} {
                  source gp.tk
                  if { $::tcl_platform(platform) == "windows" } {
                  canvas .window$i.can -width $XWidth -height $YWidth -bg white
                  pack .window$i.can -in .window$i.frame
               } else {
                  canvas .window$i.can -width $XWidth -height $YWidth -bg white
                  pack .window$i.can -in .window$i.frame
               }
               gnuplot .window$i.can

               if { $::tcl_platform(platform) == "windows" } {
               #exec cmd.exe/c del "in.gp1"
               #exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp$i
                 exec rm gp.tk
               }
            }
            pack .window$i.can -in .window$i -side top -fill both -expand true
        }
     }
   } 
 }

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

    #Torques
    if {$Option==1} {    
		  puts $Data "set ylabel \"Torques(N.m)\" 0,2"
		  puts -nonewline $Data "plot "
		  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1:[ expr ( $j+1)  ] smooth csplines title 'Joint $j'  ,"
		  }
		  puts -nonewline $Data "\"$FileName\""
		  puts -nonewline $Data " using 1:[ expr ( $DOF+1)  ] smooth csplines title 'Joint $DOF' "
    }

    #Abi
    if {$Option==2} {
		  puts $Data "set ylabel \"Abi(Kg)\" 0,2"
		  puts -nonewline $Data "plot "
		  for { set j 1 } { $j < $DOF } { incr j } {
		 	  puts -nonewline $Data "\"$FileName\""
			  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ]  smooth csplines title 'Joint $j',"
		  }
		  puts -nonewline $Data "\"$FileName\""
		  puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ] smooth csplines title 'Joint $DOF' "
    }
    
    #Energy
    if {$Option==3} {
          #Kinetic
          if {$i==1} {
		      puts $Data "set ylabel \"Joules\" 0,2"
		      puts -nonewline $Data "plot "
		      for { set j 1 } { $j < $DOF } { incr j } {
		 	     puts -nonewline $Data "\"$FileName\""
			     puts -nonewline $Data " using 1: [ expr ( $j+1)  ]  smooth csplines title 'Joint $j',"
              }
		     puts -nonewline $Data "\"$FileName\""
		     puts -nonewline $Data " using 1: [ expr ( $DOF+1)  ] smooth csplines title 'Joint $DOF' "
          } 
          
          #Potential  
           if {$i==2} {
               puts $Data "set ylabel \"Joules\" 0,2"
		       puts -nonewline $Data "plot "
		       for { set j 1 } { $j < $DOF } { incr j } {
		 	      puts -nonewline $Data "\"$FileName\""
			      puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ]  smooth csplines title 'Joint $j',"
               }
		       puts -nonewline $Data "\"$FileName\""
		       puts -nonewline $Data " using 1: [ expr ( $j+$DOF+1)  ] smooth csplines title 'Joint $DOF' "  
           }
           
           #Potential  
           if {$i==3} {
               puts $Data "set ylabel \"Joules\" 0,2"
		       puts -nonewline $Data "plot "
		       for { set j 1 } { $j < $DOF } { incr j } {
		 	      puts -nonewline $Data "\"$FileName\""
			      puts -nonewline $Data " using 1: [ expr ( $j+$DOF+$DOF+1)  ]  smooth csplines title 'Joint $j',"
               }
		       puts -nonewline $Data "\"$FileName\""
		       puts -nonewline $Data " using 1: [ expr ( $j+$DOF+$DOF+1)  ] smooth csplines title 'Joint $DOF' "  
            }
    }
    
    close $Data

    if { $::tcl_platform(platform) == "windows" } {
      exec Tcl/GNUPlot/wgnupl32.exe "in.gp$i"
    } else {
      exec gnuplot "in.gp$i"
    }
    source gp.tk
    canvas $can -width $xw -height $yw -bg white -fill both -expand true
    gnuplot $can
    pack $can -in $top -side top -fill both -expand true

    if { $::tcl_platform(platform) == "windows" } {
      #exec cmd.exe/c del "in.gp1"
      #exec cmd.exe/c del.exe "gp.tk"
    } else {
      exec rm in.gp$i
      exec rm gp.tk
    }
 }

  proc Initialize {} {
    variable tplGP
    variable frmGPTB
    variable FileName
	variable DOF
    variable InvDynGNUPlotInit
    variable CurrentOpenSaveDirectoryInvDynGNUPlotData
    set InvDynGNUPlotInit 1
    
	# Initialization of GNUPlot window
    RobLab::CreateWindow "" wndInvDynGNUPlot "Inverse Dynamics GNUPlot" "$RobLab::InvDynGP_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { InvDynGNUPlot::Delete } $RobLab::VirtualGridSize

    # Initialization of binds

    # Tool Bars
    frame $frmGPTB -width 26
    pack $frmGPTB -side top -fill both -expand true

    # Initialization of tool bar
    set GPToolBar { {    Open     "ROpen"                      imgONTBOpen    normal  { InvDynGNUPlot::Open } }
                    {    Delete   "RDelete"                    imgONTBDelete  normal  { InvDynGNUPlot::Delete } }
                    { "" }
                    {    Button01 "RTorques"          imgGPTBButton01 normal { InvDynGNUPlot::SetGraphGNUPlot 1 } }
                    {    Button02 "RABI"          imgGPTBButton02 normal { InvDynGNUPlot::SetGraphGNUPlot 2 } }
                   
				   }
    ToolBars::Create $frmGPTB $GPToolBar Horizontal
    set FileName  [ InvDynGNUPlot::ReturnFileName ] 1
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
