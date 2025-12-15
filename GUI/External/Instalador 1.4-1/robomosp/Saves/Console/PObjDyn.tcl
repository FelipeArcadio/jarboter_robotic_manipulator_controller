# Get WorldId
set Index $ObjectNavigator::WorldNodeId
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex  ]
ConsoleWindow::Puts "WorldId = $WorldId.\n"

# Get PObject Dynamic Parameters.
set PObjectId [ IOIC::World $WorldId GetPObjectId 1 ]
set Dynamicp [ IOIC::PObject $PObjectId vGetDynamicp ]
ConsoleWindow::Puts "PObject $PObjectId Dynamic Parameters =\n $Dynamicp.\n\n"
