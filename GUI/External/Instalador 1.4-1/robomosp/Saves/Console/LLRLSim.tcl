# Get WorldId
set Index $ObjectNavigator::WorldNodeId
set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
set WorldId [ IOIC::Worlds GetWorldId $WorldIndex  ]
ConsoleWindow::Puts "First WorldId = $WorldId.\n"

# First PRobotId
set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
ConsoleWindow::Puts "First PRobotId = $PRobotId.\n"
# DOF
set DOF [ IOIC::PRobot $PRobotId GetDOF ]
ConsoleWindow::Puts "DOF = $DOF.\n\n"
# mDHp
set mDHp [ IOIC::PRobot $PRobotId LLRL mGetDHp ]
ConsoleWindow::Puts "mDHp =\n[ join $mDHp \n ].\n\n"
# mInJointp
set mInJointp [ IOIC::PRobot $PRobotId LLRL mGetInJointp ]
ConsoleWindow::Puts "mInJointp =\n[ join $mInJointp \n ].\n\n"
# mInHMT
set mInHMT [ IOIC::PRobot $PRobotId LLRL mGetInHMT ]
ConsoleWindow::Puts "mInHMT =\n[ join $mInHMT \n ].\n\n"
# mOutJointp
set mOutJointp [ IOIC::PRobot $PRobotId LLRL mGetOutJointp ]
ConsoleWindow::Puts "mOutJointp =\n[ join $mOutJointp \n ].\n\n"
# mOutHMT
set mOutHMT [ IOIC::PRobot $PRobotId LLRL mGetOutHMT ]
ConsoleWindow::Puts "mOutHMT =\n[ join $mOutHMT \n ].\n\n"
# FKine
IOIC::PRobot $PRobotId LLRL FKine
ConsoleWindow::Puts "FKine( mDHp,  mInJointp ) -> mOutHMT.\n"
# mOutHMT
set mOutHMT [ IOIC::PRobot $PRobotId LLRL mGetOutHMT ]
ConsoleWindow::Puts "mOutHMT =\n[ join $mOutHMT \n ].\n\n"
# IKine
IOIC::PRobot $PRobotId LLRL IKine
ConsoleWindow::Puts "IKine( mDHp,  mInHMT ) -> mOutJointp.\n"
# mOutJointp
set mOutJointp [ IOIC::PRobot $PRobotId LLRL mGetOutJointp ]
ConsoleWindow::Puts "mOutJointp =\n[ join $mOutJointp \n ].\n\n"
#If debug enabled.
if { $InvKineToolBar::InvKineToolDebug== 1 } {
  IOIC::PRobot $PRobotId LLRL FKine
  set mOutHMT [ IOIC::PRobot $PRobotId LLRL mGetOutHMT ]
  if { $JacobianMatrixDisplay::JCType  == "Global" } {
      set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Local ]
   } else {
      set mJacobian   [ IOIC::PRobot $PRobotId LLRL mJacobian Global ]
   }
   JacobianMatrixDisplay::Set [ join $mJacobian ]
}   

# Constants
set Pi 3.1415926535897932384626433832795
set Radians [ expr $Pi / 180.0 ]
set Degrees [ expr 180.0 / $Pi ]
# mDOutJointp
ConsoleWindow::Puts "mOutJointp To Degrees.\n"
set mDOutJointp [ lindex $mOutJointp 0 ]
set i 0
foreach { Value } $mDOutJointp {
  set mDOutJointp [ lreplace $mDOutJointp $i $i [ expr $Value * $Degrees ] ]
  incr i
}
ConsoleWindow::Puts "mDOutJointp =\n$mDOutJointp.\n\n"
# Set Links
eval "IOIC::PRobot $PRobotId SetLinks $mDOutJointp"
# End
ConsoleWindow::Puts "End !!!\n\n"
