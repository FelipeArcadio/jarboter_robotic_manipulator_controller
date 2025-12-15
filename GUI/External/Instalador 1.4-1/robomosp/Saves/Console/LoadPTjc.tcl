# Get Number of Worlds
set NumWorlds [ llength [ IOIC::Worlds LsWorlds ] ]

# Create a world if NumWorlds is 0.
if { $NumWorlds == 0 } {
  IOIC::Worlds Create
}
# Set WorldId
set Index $ObjectNavigator::CurrentNode
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
ConsoleWindow::Puts "WorldId $WorldIndex  = $WorldId.\n"

IOIC::World $WorldId LoadPTrajectory "Arch - Meters.tjc"
