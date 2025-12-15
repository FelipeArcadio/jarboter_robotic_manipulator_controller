set WorldId [ IOIC::Worlds GetWorldId 1 ]
ConsoleWindow::Puts "WorldId 1 = $WorldId.\n"
#IOIC::World $WorldId Show
set PObjectId [ IOIC::World $WorldId GetPObjectId 1 ]
ConsoleWindow::Puts "PObjectId 1 = $PObjectId.\n"

set Count 50

set FPS [ time {
  IOIC::Transform $PObjectId Add P2 10
  IOIC::ForceReDraw
} $Count ]
scan $FPS %d FPS
set FPS [ expr 1000000.0 / $FPS ]
ConsoleWindow::Puts "Frames Per Second = $FPS\n"