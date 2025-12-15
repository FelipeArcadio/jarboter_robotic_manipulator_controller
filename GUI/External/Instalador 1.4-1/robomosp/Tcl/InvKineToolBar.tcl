#----------------------------------------------------------------------------------------------------------------------------------
# InvKineToolBar.tcl
# 		This file implements the TCL code for creating and managing the Inverse Kinematics
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
  image create photo imgPBTBDebug -file ./Images/PlayBackToolBar/debug.gif
   image create photo imgGPTBButtonq -file ./Images/GNUPlotToolBar/q.gif
# --------------------------------------------------------------------------------------------------------------------------

 #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval InvKineToolBar {

# Initialization of namespace variables
  variable InvKineTB                                        #Main InvKine ToolBar Widget.
  variable tplIKine .wndInvKineToolBar            #Main InvKine ToolBar Window.
  variable frmIKineAll $tplIKine.frmIKineAll		 #Main InvKine frame.
  variable frmIKineTB $tplIKine.frmToolBar
  variable InvKineToolBarInit 0
  variable InvKineToolDebug 0
  variable DOF
  variable WorldId
  variable PRobotId
  variable PTrajectoryId
  variable NumPTrajectories
  variable CurrentOpenDirectory $HOME/Saves/Scripts
  variable Pi 3.1415926535897932384626433832795
  variable Radians [ expr $Pi / 180.0 ]
  variable Degrees [ expr 180.0 / $Pi ]
  
   #  Open  procedure --
   #
   #
   # Arguments: Window
   #
   # Results:
   #
   proc Open {} {
    variable InvKineTB
    variable CurrentOpenDirectory
    global IKineType
    set RobLab::IKineType User
    set FileTypes { {"Tcl Scripts" ".tcl"} {"All Files" "*"} }
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir $CurrentOpenDirectory -parent $InvKineTB -title "Open Tcl Script" ]
    if { $FileName != "" } {
      	set $CurrentOpenDirectory [ file dirname $FileName ]
		#Puts "source \"$FileName\"" TagInput
      	source $FileName    
      	MessagesWindow::Puts "Loading Script file \"$FileName\" ...\n"
    		MessagesWindow::Puts "Script  file loaded. \n\n"
    }
}
  
  #  ReInitActiveWindows  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ReInitActiveWindows {} {
  variable PRobotId
  variable DOF
     #Position and Orientation
     if { ( $RobLab::KinePosOrient == 1) && ( $FKineMatrixDisplay::FKineMatrixDisplayInit == 0 ) } {
         FKineMatrixDisplay::Initialize
	     IOIC::PRobot $PRobotId LLRL FKine
		 IOIC::PRobot $PRobotId LLRL mGetOutHMT
     }
     #Velocity
      if { ( $RobLab::KineVel == 1 ) && ( $RobLab::VelMatrixDisplayInit == 0 ) } {
        VelocityMatrixDisplay::Initialize
     }
     #Acceleration
     if { ( $RobLab::KineAccel == 1 ) && ( $AccelMatrixDisplay::AccelMatrixDisplayInit == 0 ) } {
        AccelMatrixDisplay::Initialize
     }
     #Jacobian
     if { ( $RobLab::Jacobian == 1 ) && ( $JacobianMatrixDisplay::JacobianMatrixDisplayInit == 0 )} {
         JacobianMatrixDisplay::Initialize 6 $DOF
		 if { $JacobianMatrixDisplay::JCType  == "Global" } {
                set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Local ]
         } else {
                set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Global ]
         }
         JacobianMatrixDisplay::Set [ join $mJacobian ]
     }
  
  
  }
  #  RefreshActiveWindows  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc RefreshActiveWindows {} {
     variable PRobotId
	# Constants
	set Pi 3.1415926535897932384626433832795
	set Degrees [ expr 180.0 / $Pi ]
     IOIC::ForceReDraw
    # Position and Orientation Window.
    if { $RobLab::KinePosOrient == 1 } {
            IOIC::PRobot $PRobotId LLRL FKine
            IOIC::PRobot $PRobotId LLRL mGetOutHMT
    }
    # Jacobian Window.
   if { $RobLab::Jacobian == 1 } {  
   if { $JacobianMatrixDisplay::JCType  == "Global" } {
             set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Local ]
       } else {
            set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Global ]
       }
       JacobianMatrixDisplay::Set [ join $mJacobian ]
    }
    
	# Teach Pendant.
       if { $TeachPendantWindow::TeachWindowInit == 1 } {  
            set mOutJointp [ IOIC::PRobot $PRobotId LLRL mGetInJointp ]
		#mDOutAngles
		set mDOutAngles [ lindex $mOutJointp 0 ]
		set i 0
		foreach { Value } $mDOutAngles {
			set mDOutAngles [ lreplace $mDOutAngles $i $i [ expr $Value * $Degrees ] ]
			incr i
		}
       	TeachPendantWindow::UpdateJointsValues $mDOutAngles
	  }
  }
  
  #  GetWorldsIds  --
  #  It Gets needed Ids from current world
  #
  # Arguments:
  #
  # Results:
  #
  proc GetWorldsIds {} {
     variable WorldId
     variable PRobotId
     variable NumPTrajectories
     
     # Get WorldId
     set Index $ObjectNavigator::WorldNodeId
     set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
     set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
     # Get PRobotId
     set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
     # Set Number of Trajectories
     set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]      
  

  }
  
  #  UserDefinedReachTarget  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc UserDefinedReachTarget {} {
  	variable PRobotId
  	variable Pi
      variable Radians 
      variable Degrees 
      
	# mDHp
	set mDHp [ IOIC::PRobot $PRobotId LLRL mGetDHp ]	
	
      # mInHMT
	set mInHMT [ IOIC::PRobot $PRobotId LLRL mGetInHMT ]
      
	# IKine
      set mOutJointp [ IKineUserDefined $mDHp $mInHMT ]
      MessagesWindow::Puts "mOutJointp = $mOutJointp\n"
	
	# Change Joint Values to Degrees
    	set i 0
    	foreach { Value } $mOutJointp {
      	set mOutJointp [ lreplace $mOutJointp $i $i [ expr $Value * $Degrees ] ]
      	incr i
	}
      MessagesWindow::Puts "mOutJointp = $mOutJointp\n"
      
	# Set Links
	eval "IOIC::PRobot $PRobotId SetLinks $mOutJointp"
}

#  UserDefinedInvKine  --
#
#
# Arguments:
#
# Results:
#	
proc UserDefinedInvKine {} {
	variable WorldId
  	variable PRobotId
      variable PTrajectoryId
      variable Radians 
  	variable Degrees 
      
	# mDHp
	set mDHp [ IOIC::PRobot $PRobotId LLRL mGetDHp ]	
      
	# Get PTrajectory Number of Generated Points
      set NumGen [ IOIC::PTrajectory $PTrajectoryId GetNumGenPoints]     
      MessagesWindow::Puts "NumGen = $NumGen\n"
      
      for { set PointsIndex 1 } { $PointsIndex <= $NumGen } { incr PointsIndex } {
		# mInHMT
		set mInHMT [ IOIC::PTrajectory $PTrajectoryId GetGenPoint $PointsIndex ]
		#MessagesWindow::Puts "mInHMT $PointsIndex = \n [ join $mInHMT \n ] \n\n"
		# IKine
		lappend mOutJointp [ IKineUserDefined $mDHp $mInHMT ]
		#MessagesWindow::Puts "mOutJointp = $mOutJointp\n"
	}
      
	# Get Number of Joints Points List 
      set JointNum [ llength $mOutJointp ]     
      MessagesWindow::Puts "JointNum = $JointNum\n"
      
      for { set PointsIndex 1 } { $PointsIndex < $NumGen } { incr PointsIndex } {
      	set JointPoint [ lindex $mOutJointp $PointsIndex ]
		MessagesWindow::Puts "JointPoint $PointsIndex : = $JointPoint\n"
            
		# Change Joint Values to Degrees
    		set i 0
    		foreach { Value } $JointPoint {
      		set JointPoint [ lreplace $JointPoint $i $i [ expr $Value * $Degrees ] ]
      		incr i
		}
            MessagesWindow::Puts "DJointPoint $PointsIndex : = $JointPoint\n"
      
		# Set Links
		eval "IOIC::PRobot $PRobotId SetLinks $JointPoint"
		IOIC::ForceReDraw   
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
    variable tplIKine
    variable InvKineToolBarInit
    variable InvKineToolDebug
    set InvKineToolBarInit 0 
    set InvKineToolDebug 0

	if { $FKineMatrixDisplay::FKineMatrixDisplayInit == 1  } {
   		FKineMatrixDisplay::Delete
	}
    
    if { $RobLab::VelMatrixDisplayInit == 1  } {
   		VelocityMatrixDisplay::Delete
    }
    
    if { $AccelMatrixDisplay::AccelMatrixDisplayInit == 1  } {
   		AccelMatrixDisplay::Delete
    }

	if { $JacobianMatrixDisplay::JacobianMatrixDisplayInit == 1  } {
   		JacobianMatrixDisplay::Delete
	}
    
    for { set i 1 } { $i <= 3 } { incr i } {
            set isgraph(ok) [winfo exists .window$i]
            if {$isgraph(ok)} {
              destroy .window$i
            }
     }


    wm withdraw $InvKineToolBar::tplIKine
  }
  
  #  Play  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Play { KineType } {
    
    variable WorldId
    variable NumPTrajectories
    variable PTrajectoryId
    
    ReInitActiveWindows
   #It Gets needed Ids from current world
    GetWorldsIds 
      # Set Number of Robots
      set NumPRobots [ llength [ IOIC::World $WorldId LsPRobots ] ]

      # WorldSimulate
      # Get PRobotsId
      for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
        set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]
	  set PRobotBaseType [ IOIC::PRobot $PRobotId GetBaseType ]
	  #MessagesWindow::Puts "BaseType: $PRobotBaseType \n"
        if { ( $NumPTrajectories == 0 && $PRobotBaseType == "Fixed" ) } {
            # IKine
            if { $RobLab::IKineType == "Default" } {
            	IOIC::PRobot $PRobotId FollowTarget
		} else {
            	UserDefinedReachTarget   
		}
        } else {
              # Get PTrajectoryIds
              for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
                	set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]

                	# Get PTrajectory Number of Generated Points
                	set NumGen [ IOIC::PTrajectory $PTrajectoryId GetNumGenPoints]

                	#Grab simulation initial time.
                	#set t1 [ clock seconds ]
			#IOIC::World $WorldId InvKineWorldSim $PRobotId $PTrajectoryId TIME_DELAY_ON $KineType
			if { $RobLab::IKineType == "Default" } {
             		IOIC::World $WorldId InvKineWorldSim $PRobotId $PTrajectoryId TIME_DELAY_ON $KineType
		    	} else {
              		UserDefinedInvKine   
		    	}        

                	#Grab simulation final time.
                	#set t2 [ clock seconds ]
                	#MessagesWindow::Puts "Total Time = [ expr ( $t2 - $t1 ) ] seconds\n"
                	#MessagesWindow::Puts "Frames Per Second = [ expr "$NumGen.0" / ( $t2 - $t1 ) ]\n"
              }
        }
      }   
      RefreshActiveWindows 
  }

  #  Debug  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Debug {} {
    variable InvKineToolDebug
    variable frmIKineTB
    
    if { $InvKineToolDebug== 0 } {
        ToolBars::SetButtonToolTip $frmIKineTB  Debug "RDebug On"
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmIKineTB Debug ] "RDebug On"
        set InvKineToolDebug 1
        
        if { $KineCheckList::PosOrient1 == 1  } {
            FKineMatrixDisplay::Initialize
            IOIC::PRobot $KineCheckList::PRobotId LLRL FKine
            IOIC::PRobot $KineCheckList::PRobotId LLRL mGetOutHMT
        }
        if { $KineCheckList::Velocity1 == 1  } {
   			VelocityMatrixDisplay::Initialize
        }
        if { $KineCheckList::Acceleration1 == 1  } {
   			AccelMatrixDisplay::Initialize
       	}
		if { $KineCheckList::Jacobian1 == 1  } {
   			JacobianMatrixDisplay::Initialize 6 $KineCheckList::DOF
            if { $JacobianMatrixDisplay::JCType  == "Global" } {
               set mJacobian   [ IOIC::PRobot $KineCheckList::PRobotId LLRL mJacobian Local ]
            } else {
               set mJacobian   [ IOIC::PRobot $KineCheckList::PRobotId LLRL mJacobian Global ]
            }
            JacobianMatrixDisplay::Set [ join $mJacobian ]
       	}
    
    } else {
		if { $FKineMatrixDisplay::FKineMatrixDisplayInit == 1  } {
   			FKineMatrixDisplay::Delete
		}
        if { $RobLab::VelMatrixDisplayInit == 1  } {
   		   VelocityMatrixDisplay::Delete
        }  
        if { $AccelMatrixDisplay::AccelMatrixDisplayInit == 1  } {
   		   AccelMatrixDisplay::Delete
        }
		if { $JacobianMatrixDisplay::JacobianMatrixDisplayInit == 1  } {
   			JacobianMatrixDisplay::Delete
		}
        ToolBars::SetButtonToolTip $frmIKineTB  Debug "RDebug Off"
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmIKineTB Debug ] "RDebug Off"
        set InvKineToolDebug 0
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
      variable InvKineTB
      variable FileName
      variable InvKineToolBarInit
      variable InvKineToolDebug
      variable WorldId
      variable PRobotId
      variable DOF
      variable NumPTrajectories
      variable PRobotBaseType
      global InvKine
      global CartTjc
      
      if { $InvKine == 1} {
            # Error Message if NumWorlds is 0.
            if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
                 set InvKine 0
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
                   set InvKineToolBarInit 1
                   set InvKineToolDebug 1
                   # Get PRobotId
                   set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
                   # Get DOF
		           set DOF [ IOIC::PRobot $PRobotId GetDOF ]
                   # Set Number of Trajectories
                   set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]
                   #::toolbar::activate $InvKineTB
                   ::toolbar::deiconify $InvKineTB
                   #set FileName  [ InvKineGNUPlot::ReturnFileName ]
                     #Create base GNUPlot Toolbar if it is a free flying or free floating robot.
                     set PRobotBaseType [ IOIC::PRobot $PRobotId GetBaseType ] 
                     if { ($PRobotBaseType =="Flying") ||  ($PRobotBaseType == "Floating") || ($PRobotBaseType == "Serpentine")} {
                        GNUPlot::Activate 2
                     }   
                } 
           }     
      } else {
           #::toolbar::deactivate $InvKineTB
           ::toolbar::HandleCallback $InvKineTB  -2
           if { ($PRobotBaseType =="Flying") ||  ($PRobotBaseType == "Floating")  ||  ($PRobotBaseType == "Serpentine") || ( $GNUPlot::GNUPlotInit2 == 1 ) } {
                  set GNUPlot::GNUPlotInit2 0
                  if { $CartTjc != 1} {
                      ::toolbar::HandleCallback $GNUPlot::CartTjcTB  -2
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
  proc Initialize {} {
     variable InvKineTB
   
        ## 1) Create the InvKine toolbar in the ROBOMOSP main toolbar frame.
        set InvKineTB [::toolbar::create $RobLab::MainTFrame.tb1 ]
           ## 2) Add some buttons/widgets in this toolbar.
           ::toolbar::addbutton $InvKineTB -image imgPBTBStop -tooltip {Stop}   -command { }
           ::toolbar::addbutton $InvKineTB -image imgPBTBPause -tooltip {Pause}   -command { }
           ::toolbar::addbutton $InvKineTB -image imgPBTBPrevious -tooltip {Previous}   -command { InvKineToolBar::Play 2 }
           ::toolbar::addbutton $InvKineTB -image imgPBTBPlay -tooltip {Play}   -command { InvKineToolBar::Play 4 } 
           ::toolbar::addbutton $InvKineTB -image imgPBTBNext -tooltip {Next}   -command { InvKineToolBar::Play 3 }
           ::toolbar::addbutton $InvKineTB -image imgPBTBFirst -tooltip {First}   -command { InvKineToolBar::Play  0 }
           ::toolbar::addbutton $InvKineTB -image imgPBTBEnd -tooltip {Last}   -command { InvKineToolBar::Play 1 }
  		::toolbar::addbutton $InvKineTB -image imgONTBOpen -tooltip { Load Method }   -command { InvKineToolBar::Open }
           ::toolbar::addbutton $InvKineTB -image  imgGPTBButtonq -tooltip { Plot State Variables}   -command {  InvKineGNUPlot::SetGraphGNUPlot  1}
           ::toolbar::addbutton $InvKineTB -image imgONTBDelete -tooltip {Delete Plot Windows}   -command { InvKineGNUPlot::Delete }
           ::toolbar::addseparator $InvKineTB
        ## 3) Pack the toolbar inside the toolbar frame.
        pack $InvKineTB -side left -fill y
     ## 4) Pack the toolbar frame in our window.
     pack  $RobLab::MainTFrame -side top -fill x  
     #::toolbar::deactivate $InvKineTB
     ::toolbar::HandleCallback $InvKineTB  -2    
  } 
}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
