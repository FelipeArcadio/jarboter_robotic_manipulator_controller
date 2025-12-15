#----------------------------------------------------------------------------------------------------------------------------------
# InvDynToolBar.tcl
# 		This file implements the TCL code for creating and managing the Inverse Dynmatics
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
   image create photo imgGPTBButtonG -file ./Images/GNUPlotToolBar/grav.gif
   image create photo imgGPTBButtonT -file ./Images/GNUPlotToolBar/tau.gif
   image create photo imgGPTBButtonT1 -file ./Images/GNUPlotToolBar/taudq.gif
   image create photo imgGPTBButtonT2 -file ./Images/GNUPlotToolBar/tauqdd.gif
   image create photo imgGPTBButtonJ -file ./Images/GNUPlotToolBar/Energies.gif

# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval InvDynToolBar {

  # Initialization of global variables
  variable tplIDyn .wndInvDynToolBar            #Main InvDyn ToolBar Window.
  variable frmIDynAll $tplIDyn.frmIDynAll		#Main InvDyn frame.
  variable frmIDynTB $tplIDyn.frmToolBar
  variable InvDynToolBarInit 0
  variable InvDynToolDebug 0
  variable SimulationDone 0
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplIDyn
    variable InvDynToolBarInit
    variable InvDynToolDebug
    variable SimulationDone
    set InvDynToolBarInit 0 
    set InvDynToolDebug 0
    set SimulationDone 0
    
    for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
     }

    wm withdraw $InvDynToolBar::tplIDyn
  }
  
  #  Play  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Play {  Option } {
  
      # Get WorldId
      set Index $ObjectNavigator::WorldNodeId
      set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
      set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
      
      # Set Number of Robots
      set NumPRobots [ llength [ IOIC::World $WorldId LsPRobots ] ]
      
      # Set Number of Trajectories
      set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]
      
      # WorldSimulate
      # Get PRobotsId
      for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
        set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]
        set PRobotBaseType [ IOIC::PRobot $PRobotId GetBaseType ]
        
        switch $PRobotBaseType {    
                 Fixed  {                               
                                     InvDynToolBar::FixedBase $WorldId $PRobotId $NumPTrajectories $Option
                            }
                 Flying  { 
                                     InvDynToolBar::FreeFlying $WorldId $PRobotId $NumPTrajectories $Option
                            }
                 Floating  {
                                        InvDynToolBar::FloatingBase $WorldId $PRobotId $NumPTrajectories $Option                        
                                }
                 Serpentine  {
                                        InvDynToolBar::FloatingBase $WorldId $PRobotId $NumPTrajectories $Option                        
                                }
        }
     } 
     InvDynGNUPlot::SetGraphGNUPlot 1  $Option    
  }
  
#  FreeFlying  --
#
#
# Arguments:
#
# Results:
#
  proc FreeFlying { WorldId PRobotId NumPTrajectories Option } {
  
   # Get PTrajectoryIds
   for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
          set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]
          # if end effector trajectory and base trajectory are of the same size
          if { [ IOIC::PTrajectory $PTrajectoryId GetNumGenPoints] ==  [ IOIC::PRobot $PRobotId GetBaseNumGen ] } {
                # Simulate with  InvDynWorldSim
                IOIC::World $WorldId InvDynWorldSim $PRobotId $PTrajectoryId TIME_DELAY_ON $Option                       
          } else {
            # Error message.
            MessagesWindow::Puts " Error: End effector trajectory and Base trajectory must be of the same size.\n"
         }             
   }
  }  
  
  #  FixedBase  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc FixedBase { WorldId PRobotId NumPTrajectories Option } {
  
  # Get PRobotsId
        if { $NumPTrajectories == 0 } {
               # IDyn reach target
               InvKineToolBar::ReachTarget
        } else {
             # Get PTrajectoryIds
              for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
                  set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]
                  # Simulate with  InvDynWorldSim
                  IOIC::World $WorldId InvDynWorldSim $PRobotId $PTrajectoryId TIME_DELAY_ON $Option
              }
         }
   }
   
   

  #  FloatingBase  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc FloatingBase { WorldId PRobotId NumPTrajectories Option } {
  
    
    # Get PTrajectoryIds
              for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
                  set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]
                  #Compute base trajectory
                  #IOIC::World $WorldId ComputeBaseTjc $PRobotId $PTrajectoryId
                  # Simulate with  InvDynWorldSim
                  IOIC::World $WorldId InvDynWorldSim $PRobotId $PTrajectoryId TIME_DELAY_ON $Option
              }
}

  
  #  Activate  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Activate {} {
     variable InvDynTB
     variable WorldId
     variable PRobotId
     variable DOF
     variable NumPTrajectories
     global InvDyn
     
     if { $InvDyn == 1} {
            # Error Message if NumWorlds is 0.
            if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
                 set InvDyn 0
                 MessagesWindow::Puts "Error:  Not a world has been loaded. Please load a World definition file.\n"
            } else { 
                 # Get WorldId
                 set Index $ObjectNavigator::WorldNodeId
                 set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
                 set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
                 # Error Message if NumPRobots is 0.
                 if {  [ llength [ IOIC::World $WorldId LsPRobots ] ] == 0 } {
                     MessagesWindow::Puts "Error:There are not robots in the current world, please load a Robot definition file. \n"             
                 } else {
                     # Get PRobotId
                     set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
                     # Get DOF
		             set DOF [ IOIC::PRobot $PRobotId GetDOF ]
                     # Set Number of Trajectories
                     set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]
                     ::toolbar::deiconify $InvDynTB
                     #set FileName  [ InvDynGNUPlot::ReturnFileName 1]
                  }      
            }
     } else {
       ::toolbar::HandleCallback $InvDynTB  -2 
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
    variable InvDynTB
    variable torque 0
    variable vtorque 1
    variable atorque 2
    
    ## 1) Create the InvDyn toolbar in the ROBOMOSP main toolbar frame.
    set InvDynTB [::toolbar::create $RobLab::MainTFrame.tb3 ]
          ## 2) Add some buttons/widgets in this toolbar.
#::toolbar::addbutton $InvDynTB -image  imgGPTBButtonG -tooltip { Gravity force On/Off } -command {  GravityWindow::Initialize}
          ::toolbar::addbutton $InvDynTB -image  imgGPTBButtonT -tooltip { Compute and Plot Torques } -command {  InvDynToolBar::Play 0 }
          ::toolbar::addbutton $InvDynTB -image  imgGPTBButtonT1 -tooltip { Compute and Plot Velocity Torques } -command { InvDynToolBar::Play 1 }
          ::toolbar::addbutton $InvDynTB -image  imgGPTBButtonT2 -tooltip { Compute and Plot Acceleration Torques } -command { InvDynToolBar::Play 2 }
          ::toolbar::addbutton $InvDynTB -image  imgGPTBButtonJ -tooltip {Plot Energies}   -command { InvDynGNUPlot::SetGraphGNUPlot  3 4}
          ::toolbar::addbutton $InvDynTB -image imgONTBDelete -tooltip {Delete Plot Windows}   -command { InvDynGNUPlot::Delete }
          ::toolbar::addseparator $InvDynTB    
          ## 3) Pack the toolbar inside the toolbar frame.
        pack $InvDynTB -side left -fill y
        ## 4) Pack the toolbar frame in our window.
        pack  $RobLab::MainTFrame -side top -fill x  
        ::toolbar::HandleCallback $InvDynTB  -2    
  }
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
