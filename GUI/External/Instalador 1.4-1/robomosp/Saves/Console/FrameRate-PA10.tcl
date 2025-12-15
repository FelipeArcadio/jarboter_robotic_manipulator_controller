# Set WorldId
set Index $ObjectNavigator::CurrentNode
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]

# Set Number of Robots
set NumPRobots [ llength [ IOIC::World $WorldId LsPRobots ] ]

# Frame Rate procedure
set Count 50
# Form A
set FPS [ time {
  for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
    set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]
    IOIC::PRobot $PRobotId AddLinks 5 0 7 0 3 8 6
  }
  IOIC::ForceReDraw
} $Count ]
scan $FPS %d FPS
set FPS [ expr 1000000.0 / $FPS ]
ConsoleWindow::Puts "Form A:\n"
ConsoleWindow::Puts "Frames Per Second = $FPS\n"

# Form B
set t1 [ clock seconds ]
for { set Index 1 } { $Index <= $Count } { incr Index } {
  for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
    set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]
    IOIC::PRobot $PRobotId AddLinks 5 0 7 0 3 8 6
  }
  IOIC::ForceReDraw
}
set t2 [ clock seconds ]
ConsoleWindow::Puts "Form B:\n"
ConsoleWindow::Puts "Frames Per Second = [ expr "$Count.0" / ( $t2 - $t1 ) ]\n"
