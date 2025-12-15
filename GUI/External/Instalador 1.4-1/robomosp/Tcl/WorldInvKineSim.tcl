# Get Number of Worlds
set NumWorlds [ llength [ IOIC::Worlds LsWorlds ] ]

# Error Message if NumWorlds is 0.
if { $NumWorlds == 0 } {
  MessagesWindow::Puts "Error:  Not a world has been loaded.\n"
} else {
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

         if { $NumPTrajectories== 0 } {
              # IKine
              source "./Saves/Console/LLRLSim.tcl"
         } else {
            # Get PTrajectoryIds
            for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
                set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]

                # Get PTrajectory Number of Generated Points
                set NumGen [ IOIC::PTrajectory $PTrajectoryId GetNumGenPoints]

                #Grab simulation initial time.
                set t1 [ clock seconds ]
                IOIC::World $WorldId InvKineWorldSim $PRobotId $PTrajectoryId TIME_DELAY_ON

                #Grab simulation final time.
                set t2 [ clock seconds ]
                #MessagesWindow::Puts "Total Time = [ expr ( $t2 - $t1 ) ] seconds\n"
                #MessagesWindow::Puts "Frames Per Second = [ expr "$NumGen.0" / ( $t2 - $t1 ) ]\n"
            }
        }    
    }
    #If debug enabled.
    if { $InvKineToolBar::InvKineToolDebug== 1 } {
        IOIC::PRobot $PRobotId LLRL FKine
        set mOutHMT [ IOIC::PRobot $PRobotId LLRL mGetOutHMT ]
        ConsoleWindow::Puts "mOutHMT =\n[ join $mOutHMT \n ].\n\n"
        if { $JacobianMatrixDisplay::JCType  == "Global" } {
            set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Local ]
        } else {
            set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Global ]
        }
        ConsoleWindow::Puts "mJacobian =\n[ join $mJacobian \n ].\n\n"  
        JacobianMatrixDisplay::Set [ join $mJacobian ]
    } 
}

