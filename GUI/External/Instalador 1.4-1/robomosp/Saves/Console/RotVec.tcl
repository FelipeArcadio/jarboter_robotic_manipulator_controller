# Get WorldId
set Index $ObjectNavigator::WorldNodeId
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex  ]
ConsoleWindow::Puts "WorldId = $WorldId.\n"

# First PRobotId
set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
ConsoleWindow::Puts "PRobotId = $PRobotId.\n"


# RotVec
IOIC::PRobot $PRobotId LLRL RotVec 35
IOIC::ForceReDraw
# End
ConsoleWindow::Puts "End !!!\n\n"
