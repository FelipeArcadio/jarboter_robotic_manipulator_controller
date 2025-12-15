# Get WorldId
set Index $ObjectNavigator::WorldNodeId
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex  ]

# Get PRobot Dynamic Parameters.
set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
set Dynamicp [ IOIC::PRobot $PRobotId vGetDynamicp ]
ConsoleWindow::Puts "PRobot $PRobotId Dynamic Parameters =\n $Dynamicp.\n\n"
