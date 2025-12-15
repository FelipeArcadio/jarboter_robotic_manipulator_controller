# Load World
IOIC::Worlds Open "PA10 with Interpolated circle.wld"

# Set WorldId
set Index $ObjectNavigator::CurrentNode
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
ConsoleWindow::Puts "WorldId $WorldIndex  = $WorldId.\n"

# Get PRobotId
set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
ConsoleWindow::Puts "First PRobotId = $PRobotId.\n"

# Get PTrajectoryId
set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId 1 ]
ConsoleWindow::Puts "First PTrajectoryId = $PTrajectoryId.\n"

