# Get WorldId
set Index $ObjectNavigator::WorldNodeId
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex  ]
ConsoleWindow::Puts "WorldId = $WorldId.\n"

# PTrajectoryId
set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId 1 ]
ConsoleWindow::Puts "PTrajectoryId 1 = $PTrajectoryId.\n"
# PTrajectory Number of Generated Points
set NumGen [ IOIC::PTrajectory $PTrajectoryId GetNumGenPoints]
ConsoleWindow::Puts "Nùmero de puntos generados = $NumGen.\n"
# PTrajectory GetGenPoint
for { set i 1 } { $i <= $NumGen } { incr i } {
set GenPoints [ IOIC::PTrajectory $PTrajectoryId GetGenPoint $i ]
ConsoleWindow::Puts "Generated Point No $i Pos and Rpy Parameters =\n $GenPoints.\n\n"
}
