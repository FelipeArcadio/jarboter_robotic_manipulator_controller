set WorldId [ IOIC::Worlds GetWorldId 1 ]
IOIC::World $WorldId Show
IOIC::World $WorldId LsPTrajectories
set PTrajectoryId [ IOIC::World $WorldId GetPTrajectoryId 1 ]