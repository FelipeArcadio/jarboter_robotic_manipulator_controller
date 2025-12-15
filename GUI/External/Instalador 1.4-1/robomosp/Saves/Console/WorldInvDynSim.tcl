# Get WorldId
set Index $ObjectNavigator::CurrentNode
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
ConsoleWindow::Puts "WorldId = $WorldId.\n"

# Set Number of Robots
set NumPRobots [ llength [ IOIC::World $WorldId LsPRobots ] ]

# Set Number of Trajectories
set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]

# InvDynWorldSim

# Get PRobotsId
for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
 set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]

 # Get PTrajectoryIds
 for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
   set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]

   # Get PTrajectory Number of Generated Points
   set NumGen [ IOIC::PTrajectory $PTrajectoryId GetNumGenPoints]

   #Grab simulation initial time.
   set t1 [ clock seconds ]

   IOIC::World $WorldId InvDynWorldSim $PRobotId $PTrajectoryId TIME_DELAY_OFF

   #Grab simulation final time.
   set t2 [ clock seconds ]

   ConsoleWindow::Puts "Total Time TCL = [ expr ( $t2 - $t1 ) ] seconds\n"
 }
}

