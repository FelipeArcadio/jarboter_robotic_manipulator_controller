set list {} 
set WorldId [ IOIC::Worlds GetWorldId 1 ]
ConsoleWindow::Puts "WorldId 1 = $WorldId.\n"
#IOIC::World $WorldId Show
set PObjectId [ IOIC::World $WorldId GetPObjectId 1 ]
ConsoleWindow::Puts "PObjectId 1 = $PObjectId.\n"
set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
ConsoleWindow::Puts "PRobotId 1 = $PRobotId.\n"
set DOF [ IOIC::PRobot $PRobotId GetDOF ]
set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId 1 ]
ConsoleWindow::Puts "PTrajectoryId 1 = $PTrajectoryId.\n"

set Count 50

set FPS [ time {
  IOIC::Transform $PObjectId Add P2 10
  IOIC::PRobot $PRobotId AddLinks 5 0 7 0 3 8 6
  IOIC::Transform $PRobotId Add P2 10
  IOIC::Transform $PTrajectoryId Add O2 10
  IOIC::ForceReDraw
} $Count ]
scan $FPS %d FPS
set FPS [ expr 1000000.0 / $FPS ]
ConsoleWindow::Puts "Frames Per Second = $FPS\n"