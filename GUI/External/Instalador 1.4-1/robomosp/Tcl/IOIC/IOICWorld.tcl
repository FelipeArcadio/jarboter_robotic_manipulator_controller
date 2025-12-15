# --------------------------------------------------------------------------------------------------------------------------
# IOIC.tcl
#                This file implements the World IOIC commands
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-05-19
#
# Requirements:
#
#SCCS: %Z% %M% %I% %E% %U%
# --------------------------------------------------------------------------------------------------------------------------

  # World --
  #
  #                This procedure implements all the IOIC commands that can be applied to all
  #        the elements present in a simulation world.
  #
  #       Arguments                        Result on success
  #
  # Id Close                                             {}
  # Id Save                                              {}
  # Id Show                                             {}
  # Id LsPObjects                                 List of parametric objects in world Id
  # Id LsPRobots                                   List of parametric robots in world Id
  # Id LsPTrajectories                        List of parametric trajectories in world Id
  # Id GetTargetId                               Id
  # Id GetPObjectId Index                Id
  # Id GetPRobotId Index                  Id
  # Id GetPRobotIdDesc Description       Id
  # Id GetPTrajectoryId Index                   Id
  # Id GetRobotIndex Id                               Index
  # Id GetPTrajectoryIndex Id                   Index
  # Id LoadPTrajectory FileName             PTrajectory Id
  # Id WorldSimulate PRobotId PTjcId     Kinematic Simulation
  #
  proc World { Id Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    set IdCmd Node$Id
    if { [ RobLab::IsInteger $Id ] == 0 } {
      error "Invalid Id '$Id': Must be an integer"
    } elseif { [ TkNTree::IsCommand $IdCmd ] == $TkNTree::False } {
      error "Invalid Id '$Id': Given Id doesn't exist"
    } elseif { [ ObjectNavigator::GetObjectType $IdCmd ] != "WorldsTitle" } {
      error "Invalid Id '$Id': Given Id is not a world"
    }
    switch $Option {
      "Close"              {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id Close'"
                             } else {
                               ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON $IdCmd 1
                               ObjectNavigator::DeleteNode no
                             }
                           }
      "Save"               {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id Save'"
                             } else {
                               if { [ ObjectNavigator::GetFileName $IdCmd ] == {} } {
                                 error "Invalid Id '$Id': Given Id has never been saved"
                               } else {
                                 $IdCmd Save [ ObjectNavigator::GetFileName $IdCmd ]
                               }
                             }
                           }
      "Show"               {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id Show'"
                             } else {
                               ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON $IdCmd 1
                             }
                           }
      "LsPObjects"         {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id LsPObjects'"
                             } else {
                               set i 1
                               set TemporaryNTree [ NTree::GetFirstSon [ ObjectNavigator::GetNextNodeType $IdCmd WorldsObjects ] ]
                               while { ! [ NTree::Empty $TemporaryNTree ] } {
                                 lappend Result [ list $i [ string range $TemporaryNTree 4 end ] [ TkNTree::GetTitle $TemporaryNTree ] ]
                                 incr i
                                 set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
                               }
                             }
                           }
      "LsPRobots"          {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id LsPRobots'"
                             } else {
                               set i 1
                               set TemporaryNTree [ NTree::GetFirstSon [ ObjectNavigator::GetNextNodeType $IdCmd WorldsRobots ] ]
                               while { ! [ NTree::Empty $TemporaryNTree ] } {
                                 lappend Result [ list $i [ string range $TemporaryNTree 4 end ] [ TkNTree::GetTitle $TemporaryNTree ] ]
                                 incr i
                                 set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
                               }
                             }
                           }
      "LsPTrajectories"    {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id LsPTrajectories'"
                             } else {
                               set i 1
                               set TemporaryNTree [ NTree::GetFirstSon [ ObjectNavigator::GetNextNodeType $IdCmd WorldsTrajectorys ] ]
                               while { ! [ NTree::Empty $TemporaryNTree ] } {
                                 lappend Result [ list $i [ string range $TemporaryNTree 4 end ] [ TkNTree::GetTitle $TemporaryNTree ] ]
                                 incr i
                                 set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
                               }
                             }
                           }
      "GetTargetId"        {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'World Id GetTargetId'"
                             } else {
                               set Result [ $IdCmd GetTargetId ]
                             }
                           }
      "GetPObjectId"       {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'World Id GetPObjectId Index'"
                             } else {
                               set Index [ lindex $args 0 ]
                               if { [ RobLab::IsInteger $Index ] == 0 } {
                                 error "Invalid index '$Index': Must be an integer"
                               }
                               set Result [ $IdCmd GetPObjectId $Index ]
                             }
                           }
      "GetPRobotId"        {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'World Id GetPRobotId Index'"
                             } else {
                               set Index [ lindex $args 0 ]
                               if { [ RobLab::IsInteger $Index ] == 0 } {
                                 error "Invalid index '$Index': Must be an integer"
                               }
                               set Result [ $IdCmd GetPRobotId $Index ]
                             }
                           }
       "GetPRobotIdDesc"   {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'World Id GetPRobotIdDesc Description'"
                             } else {
                               set Description [ lindex $args 0 ]
                               set Result [ $IdCmd GetPRobotIdDesc $Description ]
                             }
                           }
      "GetPTrajectoryId"   {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'World Id GetPTrajectoryId Index'"
                             } else {
                               set Index [ lindex $args 0 ]
                               if { [ RobLab::IsInteger $Index ] == 0 } {
                                 error "Invalid index '$Index': Must be an integer"
                               }
                               set Result [ $IdCmd GetPTrajectoryId $Index ]
                             }
                           }
       "GetPRobotIndex"    {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'World Id GetRobotIndex Id'"
                             } else {
                               set Id [ lindex $args 0 ]
                               set Id "Node$Id"
                               set i 1
                               set TemporaryNTree  [ NTree::GetLastSon $ObjectNavigator::WorldNodeId ]
                               set TemporaryNTree  [ NTree::GetPreviousBrother $TemporaryNTree ]
                               set TemporaryNTreeA [ NTree::GetFirstSon $TemporaryNTree ]
                               while { ! [ NTree::Empty $TemporaryNTreeA ] } {
                                 if { $Id == $TemporaryNTreeA } {
                                   set Result $i
                                   break
                                 }
                                 incr i
                                 set TemporaryNTreeA [ NTree::GetNextBrother $TemporaryNTreeA ]
                               }
                             }
                           }
   "GetPTrajectoryIndex"   {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'World Id GetPTrajectoryIndex Id'"
                             } else {
                               set Id [ lindex $args 0 ]
                               set Id "Node$Id"
                               set i 1
                               set TemporaryNTree  [ NTree::GetLastSon $ObjectNavigator::WorldNodeId ]
                               set TemporaryNTreeA [ NTree::GetFirstSon $TemporaryNTree ]
                               while { ! [ NTree::Empty $TemporaryNTreeA ] } {
                                 if { $Id == $TemporaryNTreeA } {
                                   set Result $i
                                   break
                                 }
                                 incr i
                                 set TemporaryNTreeA [ NTree::GetNextBrother $TemporaryNTreeA ]
                               }
                             }
                           }
       "LoadPTrajectory"   {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'World Id LoadPTrajectory FileName'"
                             } else {
                               set FileName [ lindex $args 0 ]
                               set FileName "Saves/Trajectories/$FileName"
                               ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetLastSon $ObjectNavigator::CurrentNode ]
                               [ NTree::GetFather $ObjectNavigator::CurrentNode ] AddTrajectory $FileName "" 0 0 0 0 0 0 0 3 0
                               set Result [ string range $ObjectNavigator::CurrentNode 4 end ]
                             }
                           }
        "InvKineWorldSim"    {
                             if { $ArgsLength < 4 } {
                               error "Wrong # args: Should be 'World Id InvKineWorldSim PRobotId PTjcId Time_Delay KineType'"
                             } else {
                               set PRobotId   [ lindex $args 0 ]
                               set PTjcId     [ lindex $args 1 ]
                               set tmpTime_Delay [ lindex $args 2 ]
                               set KineType [ lindex $args 3 ]
			       
                               if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                 error "Invalid Id '$PRobotId': Must be an integer"
                               }
                               if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                 error "Invalid Id '$PTjcId': Must be an integer"
                               }
                               if { $tmpTime_Delay=="TIME_DELAY_ON"} {
                                  set Time_Delay 1
                               } elseif { $tmpTime_Delay=="TIME_DELAY_OFF"} {
                                  set Time_Delay 0
                               } elseif { $tmpTime_Delay==""} {
                                  set Time_Delay 1
                               } else {
                                  error "Invalid Option: Must be TIME_DELAY_ON or TIME_DELAY_OFF"
                               }
                                if { [ RobLab::IsInteger $KineType ] == 0 } {
                                 error "Invalid '$KineType': Must be an integer"
				       }
                               set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                               set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]
                               set Result [ $IdCmd InvKineWorldSim $PRobotIndex $PTjcIndex $Time_Delay $KineType]
                             }
                           }
           "InvKineFirstPoint"    {
                             if { $ArgsLength < 2 } {
                                error "Wrong # args: Should be 'World Id InvKineFirstPoint PRobotId PTjcId'"
                             } else {
                                  set PRobotId   [ lindex $args 0 ]
                                  set PTjcId     [ lindex $args 1 ]
			       
                                  if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                    error "Invalid Id '$PRobotId': Must be an integer"
                                  }
                                  if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                    error "Invalid Id '$PTjcId': Must be an integer"
                                  }
                              
                                  set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                                  set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]
                                  set Result [ $IdCmd InvKineFirstPoint $PRobotIndex $PTjcIndex]
                             }
                           }                      
        "InvKineFinalPoint"    {
                             if { $ArgsLength < 2 } {
                                error "Wrong # args: Should be 'World Id InvKineFinalPoint PRobotId PTjcId'"
                              } else {
                                  set PRobotId   [ lindex $args 0 ]
                                  set PTjcId     [ lindex $args 1 ]
			       
                                  if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                    error "Invalid Id '$PRobotId': Must be an integer"
                                  }
                                  if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                    error "Invalid Id '$PTjcId': Must be an integer"
                                  }
                              
                                  set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                                  set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]
                                  set Result [ $IdCmd InvKineFinalPoint $PRobotIndex $PTjcIndex]
                              }
                          }
            "InvKineNextPoint"    {
                             if { $ArgsLength < 2 } {
                                error "Wrong # args: Should be 'World Id InvKineNextPoint PRobotId PTjcId'"
                              } else {
                                  set PRobotId   [ lindex $args 0 ]
                                  set PTjcId     [ lindex $args 1 ]
			       
                                  if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                    error "Invalid Id '$PRobotId': Must be an integer"
                                  }
                                  if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                    error "Invalid Id '$PTjcId': Must be an integer"
                                  }
                              
                                  set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                                  set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]
                                  set Result [ $IdCmd InvKineNextPoint $PRobotIndex $PTjcIndex]
                            }
                    } 
              "InvKinePrevPoint"    {
                             if { $ArgsLength < 2 } {
                                error "Wrong # args: Should be 'World Id InvKinePrevPoint PRobotId PTjcId'"
                             } else {
                                  set PRobotId   [ lindex $args 0 ]
                                  set PTjcId     [ lindex $args 1 ]
			       
                                  if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                    error "Invalid Id '$PRobotId': Must be an integer"
                                  }
                                  if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                    error "Invalid Id '$PTjcId': Must be an integer"
                                  }
                              
                                  set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                                  set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]
                                  set Result [ $IdCmd InvKinePrevPoint $PRobotIndex $PTjcIndex]
                              }
                          }                                 
        "FwdDynWorldSim"    {
                             if { $ArgsLength < 4 } {
                               error "Wrong # args: Should be 'World Id FwdDynWorldSim PRobotId Time Velocities_list Time_Delay Integration_Type
                                         Time_Step TorquesNULL' "
                             } else {
                                                         
                                     #Gets the PRobot Id command.
                             		 set PRobotId   [ lindex $args 0 ]
                                     #Gets the Time argument.
                                     set Time [ lindex $args 1]
                                     #Gets the VelList argument.
                                     set VelList [lindex $args 2] 
                                     #Gets the Time_Delay Flag.
					       set tmpTime_Delay [ lindex $args 3 ]
                                     #Gets the Integrator Type.
					       set IntgrType [ lindex $args 4 ]
                                     #Gets the Time Step.
					       set TimeStep [ lindex $args 5 ]
                                     #Gets the Torques Flag.
					       set TorquesNULL [ lindex $args 6 ]
                                     #Gets Torques FileName
					        set FileName [ lindex $args 7 ]
                                     
                                     #Transforms it in a TCL List.
                                     set VelList [ split $VelList]
                                     #Gets the VelList Length.
                                     set VelLength [ llength $VelList ]
					                
                                    # Checks that the PRobotId is an integer.
                                    if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                 		error "Invalid Id '$PRobotId': Must be an integer"
                               		}
                                                        
                                    # Checks that Time is a real.
                                    if { [ RobLab::IsReal $Time ] == 0 } {
                                 		error "Invalid Time '$Time': Must be a real number"
                            		}
                                                        
                                    # Checks that all the VelList arguments are real numbers.
                                     for { set i 0 } { $i < $VelLength} { incr i } {
                                              if { [ RobLab::IsReal  [ lindex $VelList $i]  ] == 0 } {
                                              		set [ lindex $VelList $i] 0
									#error "Invalid velocity value ' [ lindex $VelList $i] ': Must be a real number"        
                                              }
                                       }
				       
				                    if { $tmpTime_Delay=="TIME_DELAY_ON"} {
                                  		set Time_Delay 1
				                    } elseif { $tmpTime_Delay=="TIME_DELAY_OFF"} {
                                  		set Time_Delay 0
					                } elseif { $tmpTime_Delay==""} {
                                  		set Time_Delay 1
					                } else {
                                  		error "Invalid Option: Must be TIME_DELAY_ON or TIME_DELAY_OFF"
					                }
                                                        
                                    #Gets the PROBOT DOF
                                    set RobotCmd Node$PRobotId
                            	    set DOF [ $RobotCmd GetDOF ]
                                    if { $DOF == 0 } {
                                          error "Invalid PRobotId '$PRobotId': No link has been added to this parametric robot"
                                    }            
                                    # Checks that VelLength is equal to the PRobot DOF, if not, it adds zero 
                                    # arguments to VelList until it makes them equal.
                                    if {  $VelLength < $DOF } {
                                              set Limit [ expr ( $DOF - $VelLength)  ]   
                                               for { set i 0 } { $i < $Limit} { incr i } {
                                                       lappend VelList 0.0000
                                                }                    
                                       }
                                                        
                                      # Gets the Probot Index from the PRobot List.
                                      set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                                                         
                                      #Calls the FwdDynWorldSim with the right arguments.
                                      set Result [ $IdCmd FwdDynWorldSim $PRobotIndex $Time $VelList $Time_Delay $IntgrType $TimeStep $TorquesNULL $FileName ]           
                               }
                    }
                "InvDynWorldSim"    {
                             if { $ArgsLength < 4 } {
                               error "Wrong # args: Should be 'World Id InvDynWorldSim PRobotId PTjcId Time_Delay TauOption'"
                             } else {
                                                         
                             #Gets the PRobot Id command.
                             set PRobotId   [ lindex $args 0 ]
                             set PTjcId     [ lindex $args 1 ]
                             set tmpTime_Delay [ lindex $args 2 ]
			                 set TauOption [ lindex $args 3 ]
                             
                             # Checks that the PRobotId is an integer.
                             if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                 error "Invalid Id '$PRobotId': Must be an integer"
                             }

                             # Checks that the TjcId is an integer.
                             if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                 error "Invalid Id '$PTjcId': Must be an integer"
                               }

                              #Sets Time Delay to an integer value
			                   if { $tmpTime_Delay=="TIME_DELAY_ON"} {
                                  set Time_Delay 1
			                   } elseif { $tmpTime_Delay=="TIME_DELAY_OFF"} {
                                  set Time_Delay 0
                              } elseif { $tmpTime_Delay==""} {
                                  set Time_Delay 1
                              } else {
                                  error "Invalid Option: Must be TIME_DELAY_ON or TIME_DELAY_OFF"
                              }
                              
                              # Checks that the TjcId is an integer.
                              if { [ RobLab::IsInteger $TauOption ] == 0 } {
                                 error "Invalid Arg '$TauOption': Must be an integer"
                              }
                              
                             # Gets the Probot Index from the PRobot List.
                             set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                             set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]

                             #Calls the InvDynWorldSim with the right arguments.
                             set Result [ $IdCmd InvDynWorldSim $PRobotIndex $PTjcIndex $Time_Delay $TauOption ]
                          }
                       }
    "ComputeBaseTjc"    {
                             if { $ArgsLength < 2 } {
                               error "Wrong # args: Should be 'World Id CreateBaseTjc PRobotId PTjcId'"
                             } else {
                               #Gets the PRobot Id and PTjcId command.
                               set PRobotId   [ lindex $args 0 ]
                               set PTjcId     [ lindex $args 1 ]
                                                       
                               # Checks that the PRobotId is an integer.
                               if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                 error "Invalid Id '$PRobotId': Must be an integer"
                               }

                               # Checks that the TjcId is an integer.
                               if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                 error "Invalid Id '$PTjcId': Must be an integer"
                               }
                              
                               # Gets the Probot Index from the PRobot List and the PTjcIndex from a PTjc List.
                               set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                               set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]

                               #Calls the CreateBaseTjc with the right arguments.
                             set Result [ $IdCmd CreateBaseTjc $PRobotIndex $PTjcIndex ]
                          }
                      }
    default  {
                        error "Bad option '$Option': Must be Close, Save, Show, GetTargetId, GetPRobotId, GetPRobotIdDesc, LsPRobots, GetPTrajectoryId, LsPTrajectories, LoadPTrajectory, WorldSimulate, FwdDynWorldSim, InvDynWorldSim, or ComputeBaseTjc"
                   }
    }
    return $Result
  }
