# Get WorldId
set Index $ObjectNavigator::CurrentNode
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
ConsoleWindow::Puts "WorldId = $WorldId.\n"

# Set Number of Robots
set NumPRobots [ llength [ IOIC::World $WorldId LsPRobots ] ]

# WorldDynSimulate
set Time 15
set VelocitiesList  {0 0 0 0} 
# Get PRobotsId
for { set PRobotIndex 1 } { $PRobotIndex <= $NumPRobots } { incr PRobotIndex } {
 set PRobotId [ IOIC::World $WorldId GetPRobotId $PRobotIndex ]

   #Grab dynamic simulation initial time.
   set t1 [ clock seconds ]
   
   #Call to the FwdDynWorldSim IOIC command
   IOIC::World $WorldId FwdDynWorldSim $PRobotId $Time $VelocitiesList  TIME_DELAY_ON

   #Grab dynamic simulation final time.
   set t2 [ clock seconds ]
   
   #Print in the ConsoleWindow the total simulation time
   ConsoleWindow::Puts "Total Time TCL = [ expr ( $t2 - $t1 ) ] seconds\n"
   #ConsoleWindow::Puts "Frames Per Second = [ expr "$NumGen.0" / ( $t2 - $t1 ) ]\n"
 }


