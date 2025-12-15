# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------

#  File Name    : GNUPlot.tcl
#  Date         : 2003-11-20
#  Description  : Implementation of the GNUPlot Window.
#  Requirements : 
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co

# --------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  # Images of object navigator
# image create photo imgCVSIcon35 -file ./Images/ObjectNavigator/Icon35.gif

  # Images of GNUPlot canvas
  image create photo imgGPTBButtonX -file ./Images/GNUPlotToolBar/x.gif
  image create photo imgGPTBButtonY -file ./Images/GNUPlotToolBar/y.gif
  image create photo imgGPTBButtonZ -file ./Images/GNUPlotToolBar/z.gif
  image create photo imgGPTBButtonRpy -file ./Images/GNUPlotToolBar/rpy.gif
  image create photo imgGPTBButtonVel -file ./Images/GNUPlotToolBar/vel.gif

# --------------------------------------------------------------------------------------------------------------------------

namespace eval GNUPlot {

  # Procedures :
  # proc Open {}
  # proc Initialize {}
  # proc SetGraphGNUPlot {}
  # proc GraphGNUPlot {Canvas Xwidth Ywidth Index TopFrame Option}

  # Initialization of global variables
  variable tplGP .wndGNUPlot             #Main GNUPlot Window.
  variable frmGPAll $tplGP.frmGPAll      #Main GNUPlot frame.
  variable CurrentOpenDirectory $HOME/Saves/Trajectories/GNUPlotData
  variable frmGPTB $tplGP.frmToolBar
  variable FileName
  variable TjcType 
  variable GNUPlotInit 0
  variable GNUPlotInit2 0

  proc Open {} {
    variable tplGP
    variable CurrentOpenDirectory
    variable FileName
    set FileTypes { {"Tcl Scripts" ".plot"} {"All Files" "*"} }
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir $CurrentOpenDirectory -parent $tplGP -title "Open Tcl Script" ]
    if { $FileName != "" } {
      set FileName [ ObjectNavigator::FileRemoveNonPortablePath $FileName ]
      set $CurrentOpenDirectory [ file dirname $FileName ]
    }
    wm title $tplGP "Trajectories GNUPlot - $FileName"
  }

  proc Delete {} {
    variable tplGP
    for { set i 1 } { $i <= 4 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
    }
   # wm withdraw $GNUPlot::tplGP
  }

  proc ReturnFileName { Option } {
    variable tplGP
    
    if { $Option == 1 } {
        set TitleNode [ ObjectNavigator::GetMainNodeType $ObjectNavigator::CurrentNode TrajectorysTitle ]
        set FileName  [ ObjectNavigator::GetFileName $TitleNode ]
        set FileName  [ file tail $FileName ]
        set FileName  [ file rootname $FileName ]
        set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryTrajectorysGNUPlotDatas/$FileName\.plot"
    } else {
        set FileName  [ ObjectNavigator::GetFileName $ObjectNavigator::WorldNodeId ]
        set FileName  [ file tail $FileName ]
        set FileName  [ file rootname $FileName ]
       set FileName "$ObjectNavigator::CurrentOpenSaveDirectoryInvKineGNUPlotDatas/$FileName\-base.plot"
    }    
    
    wm title . "Cartesian Trajectory GNUPlot - $FileName"
    MessagesWindow::Puts "Opening .. $FileName\n"
    return $FileName
 }

 proc AssignFileName {} {
   variable FileName
   #set FileName  [ GNUPlot::ReturnFileName ]
 }

  proc SetGraphGNUPlot { Option } {
    variable XWidth 300
    variable YWidth 200
    variable TjcType 
    variable FileName
    
    set FileName [ GNUPlot::ReturnFileName $TjcType ]
    
    switch $Option {
     1 {
         set isgraph(ok) [winfo exists .window4]
            if {$isgraph(ok)} {
              destroy .window4
            }
         for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
            toplevel .window$i

            if {$i==1} {
                 wm geometry .window$i "$RobLab::GPw1_Geometry"
            }
            if {$i==2} {
              wm geometry .window$i "$RobLab::GPw2_Geometry"
            }
            if {$i==3} {
              wm geometry .window$i "$RobLab::GPw3_Geometry"
            }
            wm title .window$i "Position Vs. Time"
            frame .window$i.frame
            canvas .window$i.can -width $XWidth -height $YWidth -bg white

            if {[catch {
                  GraphGNUPlot .window$i.can $XWidth $YWidth $i .window$i.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 pack .window$i.can -in .window$i.frame
               } else {
                 canvas .window$i.can -width $XWidth -height $YWidth -bg white
                 pack .window$i.can -in .window$i.frame
               }
               gnuplot .window$i.can

               if { $::tcl_platform(platform) == "windows" } {
                 exec cmd.exe/c del "in.gp$i"
                 exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp$i
                 exec rm gp.tk
               }
             }
            pack .window$i.can -in .window$i
         }
       }
     2 {
         for { set i 1 } { $i <= 4 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
            toplevel .window$i

            if {$i==1} {
              wm geometry .window$i "$RobLab::GPw1_Geometry"
            }
            if {$i==2} {
              wm geometry .window$i "$RobLab::GPw2_Geometry"
            }
            if {$i==3} {
              wm geometry .window$i "$RobLab::GPw3_Geometry"
            }
            if {$i==4} {
              wm geometry .window$i "$RobLab::GPw4_Geometry"
            }
            wm title .window$i "Velocity Vs. Time"
            frame .window$i.frame
            canvas .window$i.can -width $XWidth -height $YWidth -bg white
            if {[catch { 
                  GraphGNUPlot .window$i.can $XWidth $YWidth $i .window$i.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 pack .window$i.can -in .window$i.frame
               } else {
                 canvas .window$i.can -width $XWidth -height $YWidth -bg white
                 pack .window$i.can -in .window$i.frame
               }
               gnuplot .window$i.can

               if { $::tcl_platform(platform) == "windows" } {
                 exec cmd.exe/c del "in.gp$i"
                 exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp$i
                 exec rm gp.tk
               }
             }
            pack .window$i.can -in .window$i
         }
       }
     3 {
         for { set i 1 } { $i <= 4 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
            toplevel .window$i

            if {$i==1} {
              wm geometry .window$i "$RobLab::GPw1_Geometry"
            }
            if {$i==2} {
              wm geometry .window$i "$RobLab::GPw2_Geometry"
            }
            if {$i==3} {
              wm geometry .window$i "$RobLab::GPw3_Geometry"
            }
            if {$i==4} {
              wm geometry .window$i "$RobLab::GPw4_Geometry"
            }
            wm title .window$i "Acceleration Vs. Time"
            frame .window$i.frame
            canvas .window$i.can -width $XWidth -height $YWidth -bg white
            if {[catch { 
                  GraphGNUPlot .window$i.can $XWidth $YWidth $i .window$i.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 pack .window$i.can -in .window$i.frame
               } else {
                 canvas .window$i.can -width $XWidth -height $YWidth -bg white
                 pack .window$i.can -in .window$i.frame
               }
               gnuplot .window$i.can

               if { $::tcl_platform(platform) == "windows" } {
                 exec cmd.exe/c del "in.gp$i"
                 exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp$i
                 exec rm gp.tk
               }
             }
            pack .window$i.can -in .window$i
         }
       }
     4 {
         set isgraph(ok) [winfo exists .window4]
         if {$isgraph(ok)} {
            destroy .window4
         }
         for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
            toplevel .window$i

            if {$i==1} {
                 wm geometry .window$i "$RobLab::GPw1_Geometry"
            }
            if {$i==2} {
              wm geometry .window$i "$RobLab::GPw2_Geometry"
            }
            if {$i==3} {
              wm geometry .window$i "$RobLab::GPw3_Geometry"
            }
            wm title .window$i "Orientation Vs. Time"
            frame .window$i.frame
            canvas .window$i.can -width $XWidth -height $YWidth -bg white

            if {[catch {
                  GraphGNUPlot .window$i.can $XWidth $YWidth $i .window$i.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 pack .window$i.can -in .window$i.frame
               } else {
                 canvas .window$i.can -width $XWidth -height $YWidth -bg white
                 pack .window$i.can -in .window$i.frame
               }
               gnuplot .window$i.can

               if { $::tcl_platform(platform) == "windows" } {
                 exec cmd.exe/c del "in.gp$i"
                 exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp$i
                 exec rm gp.tk
               }
            }
            pack .window$i.can -in .window$i
         }
       }
     5 {
         for { set i 1 } { $i <= 4 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
            toplevel .window$i

            if {$i==1} {
              wm geometry .window$i "$RobLab::GPw1_Geometry"
            }
            if {$i==2} {
              wm geometry .window$i "$RobLab::GPw2_Geometry"
            }
            if {$i==3} {
              wm geometry .window$i "$RobLab::GPw3_Geometry"
            }
            if {$i==4} {
              wm geometry .window$i "$RobLab::GPw4_Geometry"
            }
            wm title .window$i "Angular Velocity Vs. Time"
            frame .window$i.frame
            canvas .window$i.can -width $XWidth -height $YWidth -bg white
            if {[catch { 
                  GraphGNUPlot .window$i.can $XWidth $YWidth $i .window$i.frame $Option
             }]} {
               source gp.tk
               if { $::tcl_platform(platform) == "windows" } {
                 pack .window$i.can -in .window$i.frame
               } else {
                 canvas .window$i.can -width $XWidth -height $YWidth -bg white
                 pack .window$i.can -in .window$i.frame
               }
               gnuplot .window$i.can

               if { $::tcl_platform(platform) == "windows" } {
                 exec cmd.exe/c del "in.gp$i"
                 exec cmd.exe/c del.exe "gp.tk"
               } else {
                 exec rm in.gp$i
                 exec rm gp.tk
               }
             }
            pack .window$i.can -in .window$i
         }
       }
     }
 }

 proc  GraphGNUPlot {can xw yw i top Option} {
    variable FileName
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
    puts $Data "set nokey"
    puts $Data "set grid"
    puts $Data "set autoscale x"
    puts $Data "set autoscale y"
    puts $Data "set tmargin 5"
    puts $Data "set bmargin 5"
    puts $Data "set xlabel \"Time\" 0,-1"
    #Position
    if {$Option==1} {
      if {$i==1} {
        puts $Data "set ylabel \"X\" 0,2"
      }
      if {$i==2} {
        puts $Data "set ylabel \"Y\" 0,2"
      }
      if {$i==3} {
        puts $Data "set ylabel \"Z\" 0,2"
      }
      puts -nonewline $Data "plot "
      puts -nonewline $Data "\"$FileName\""
      puts -nonewline $Data " using 1:$i+1"
      puts $Data " w lines"
    }
    #Velocity
    if {$Option==2} {
      if {$i==1} {
        puts $Data "set ylabel \"Vx\" 0,2"
      }
      if {$i==2} {
        puts $Data "set ylabel \"Vy\" 0,2"
      }
      if {$i==3} {
        puts $Data "set ylabel \"Vz\" 0,2"
      }
      if {$i==4} {
        puts $Data "set ylabel \"V\" 0,2"
      }
      puts -nonewline $Data "plot "
      puts -nonewline $Data "\"$FileName\""
      puts -nonewline $Data " using 1:$i+4"
      puts $Data " w lines"
    }
    #Acceleration
    if {$Option==3} {
      if {$i==1} {
        puts $Data "set ylabel \"Ax\" 0,2"
      }
      if {$i==2} {
        puts $Data "set ylabel \"Ay\" 0,2"
      }
      if {$i==3} {
        puts $Data "set ylabel \"Az\" 0,2"
      }
      if {$i==4} {
        puts $Data "set ylabel \"A\" 0,2"
      }
      puts -nonewline $Data "plot "
      puts -nonewline $Data "\"$FileName\""
      puts -nonewline $Data " using 1:$i+8"
      puts $Data " w lines"
    }
    #Orientation
    if {$Option==4} {
      if {$i==1} {
        puts $Data "set ylabel \"Roll\" 0,2"
      }
      if {$i==2} {
        puts $Data "set ylabel \"Pitch\" 0,2"
      }
      if {$i==3} {
        puts $Data "set ylabel \"Yaw\" 0,2"
      }
      puts -nonewline $Data "plot "
      puts -nonewline $Data "\"$FileName\""
      puts -nonewline $Data " using 1:$i+12"
      puts $Data " w lines"
    }
    #Angular Velocity
    if {$Option==5} {
      if {$i==1} {
         puts $Data "set ylabel \"Vroll\" 0,2"
      }
      if {$i==2} {
         puts $Data "set ylabel \"Vpitch\" 0,2"
      }
      if {$i==3} {
         puts $Data "set ylabel \"Vyaw\" 0,2"
      }
      if {$i==4} {
         puts $Data "set ylabel \"W\" 0,2"
      }
      puts -nonewline $Data "plot "
      puts -nonewline $Data "\"$FileName\""
      puts -nonewline $Data " using 1:$i+15"
      puts $Data " w lines"
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
      #exec cmd.exe/c del "in.gp$i"
      #exec cmd.exe/c del.exe "gp.tk"
    } else {
      exec rm in.gp$i
      exec rm gp.tk
    }
 }
 
 #  Activate  --
 #
 #
 # Arguments:
 #
 # Results:
 #
 proc Activate { Option } {
    variable FileName
    variable CartTjcTB
    variable GNUPlotKine
    variable GNUPlotInit2
    variable TjcType 
    global CartTjc
    
    set TjcType $Option
    
    switch $Option { 
                1 {
                       if { $CartTjc == 1} {
                           if { [NTree::GetNumberSons $ObjectNavigator::TrajectorysNode] == 0 } {
                                set CartTjc 0
                                MessagesWindow::Puts "Error: There are not trajectories loaded in the current session.\nPlease load a Trajectory definition file.\n"
                           } else {
                                ::toolbar::deiconify $CartTjcTB
#set FileName  [ GNUPlot::ReturnFileName 1 ] 
#                              MessagesWindow::Puts "Opening .. $FileName\n"
                           }
                       } else {
                          ::toolbar::HandleCallback $CartTjcTB  -2
                          if { $GNUPlotInit2 == 1 } {
                              set  GNUPlotInit2 0
                          }
                          
                       }
                   }
                2 {
                       ::toolbar::deiconify $CartTjcTB
#set FileName  [ GNUPlot::ReturnFileName 2 ]
                       set  GNUPlotInit2 1 
#                     MessagesWindow::Puts "Opening .. $FileName\n"
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
  proc Initialize {  } {
    
    variable GNUPlotInit
    set GNUPlotInit 1
    variable CurrentOpenSaveDirectoryTrajectorysGNUPlotData
    variable CartTjcTB
    
    
    ## 1) Create a toolbar in the ROBOMOSP main toolbar frame.
    set CartTjcTB [::toolbar::create $RobLab::MainTFrame.tb2 ]
       ## 2) Add some buttons/widgets in this toolbar.
       ::toolbar::addbutton $CartTjcTB -image imgGPTBButtonX -tooltip {Plot Position Vs Time }   -command { GNUPlot::SetGraphGNUPlot 1 } 
       ::toolbar::addbutton $CartTjcTB -image imgGPTBButtonY -tooltip {Plot Velocity Vs Time }   -command { GNUPlot::SetGraphGNUPlot 2 } 
       ::toolbar::addbutton $CartTjcTB -image imgGPTBButtonZ -tooltip {Plot Acceleration Vs Time }   -command { GNUPlot::SetGraphGNUPlot 3} 
       ::toolbar::addbutton $CartTjcTB -image imgGPTBButtonRpy -tooltip {Plot Orientation Vs Time }   -command { GNUPlot::SetGraphGNUPlot 4} 
       ::toolbar::addbutton $CartTjcTB -image imgGPTBButtonVel -tooltip {Plot Angular Velocity Vs Time }   -command { GNUPlot::SetGraphGNUPlot 5} 
       ::toolbar::addbutton $CartTjcTB -image imgONTBDelete -tooltip {Delete Plot Windows}   -command { GNUPlot::Delete }
       ::toolbar::addseparator $CartTjcTB
    ## 3) Pack the toolbar inside the toolbar frame...
    pack $CartTjcTB -side left -fill y
    ## 4) Pack the toolbar frame in our window.
    pack  $RobLab::MainTFrame -side top -fill x
    ::toolbar::HandleCallback $CartTjcTB  -2
  }

}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
