# Get WorldId
set Index $ObjectNavigator::WorldNodeId
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
ConsoleWindow::Puts "WorldId 1 = $WorldId.\n"

# Set Number of Robots
set NumPRobots [ llength [ IOIC::World $WorldId LsPRobots ] ]

# Set Number of Trajectories
set NumPTrajectories [ llength [ IOIC::World $WorldId LsPTrajectories ] ]

# Set Number of Objects
set NumPObjects [ llength [ IOIC::World $WorldId LsPObjects ] ]

# PRobotIds
for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
    set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]
    set PRobotIndex1 [ IOIC::World $WorldId GetPRobotIndex $PRobotId ]
    ConsoleWindow::Puts "PRobotId $PRobotIndex = $PRobotId.\n"
    ConsoleWindow::Puts "PRobotIndex $PRobotId = $PRobotIndex1.\n"
}
# PTrajectoryIds
for { set PTrajectoryIndex 1 } { $PTrajectoryIndex <= $NumPTrajectories } { incr PTrajectoryIndex } {
   set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId $PTrajectoryIndex ]
   set PTrajectoryIndex1 [ IOIC::World $WorldId GetPTrajectoryIndex $PTrajectoryId ]
   ConsoleWindow::Puts "PTrajectoryId $PTrajectoryIndex = $PTrajectoryId.\n"
   ConsoleWindow::Puts "PTrajectoryIndex $PTrajectoryId = $PTrajectoryIndex1.\n"
}
# PObjectIds
for { set PObjectIndex 1 } { $PObjectIndex <= $NumPObjects } { incr PObjectIndex } {
  set PObjectId [ IOIC::World $WorldId GetPObjectId $PObjectIndex ]
  ConsoleWindow::Puts "PObjectId $PObjectIndex = $PObjectId.\n"
}
