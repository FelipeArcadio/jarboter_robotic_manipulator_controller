# --------------------------------------------------------------------------------------------------------------------------
# IOIC.tcl
#		This file implements the Parametric Robot IOIC commands
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

  # PRobot --
  #
  #		This procedure implements all the IOIC commands that can be applied to a
  #        Parametric Robot.
  #
  #            Arguments                 Result on Success
  # Id AddLinks Values                        {}
  # Id vGetDynamicp               PRobot Dinamic Parameters
  # Id mGetDHp                        DH parameters matrix
  # Id mGetToolHMT              ToolTransform HMT
  # Id GetDOF                            DOF
  # Id GetBaseTjc                       True or False
  # Id Link Index Add Value        {}
  #                            mGetLocal      DH local matrix
  #                            mGetGlobal    DH global matrix
  #                            vGetDHp         DH parameters vector
  #                                                        ( Alpha, a, Theta, d, Minimum, Maximum,
  #                                                        Origin, Nest, Type ( 0 Revolute, 1 Prismatic ) )
  #                           Set Value          {}
  # Id SetLinks Values                  {}
  # Id LLRL mGetDHp                {}
  #                   mGetInJointp          {}
  #                   mGetInHMT           DH parameters matrix ( Alpha, a, Theta,
  #                                                                d ( 0 Revolute, 1 Prismatic ) )
  #                   mGetOutJointp       {}
  #                   mGetOutHMT        DH parameters matrix ( Alpha, a, Theta,
  #                                                             d ( 0 Revolute, 1 Prismatic ) )
  #                   FKine                         {}
  #                   IKine                          {}
  # Id GetToolTransformId         Id
  #
  proc PRobot { Id Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    set IdCmd Node$Id
    if { [ RobLab::IsInteger $Id ] == 0 } {
      error "Invalid Id '$Id': Must be an integer"
    } elseif { [ TkNTree::IsCommand $IdCmd ] == $TkNTree::False } {
      error "Invalid Id '$Id': Given Id doesn't exist"
    } elseif { [ ObjectNavigator::GetObjectType $IdCmd ] != "WorldsRobotsTitle" } {
      error "Invalid Id '$Id': Given Id is not a parametric robot"
    }
    switch $Option {
      "AddLinks"           {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'PRobot Id AddLinks Values'"
                             } else {
                               set DOF [ $IdCmd GetDOF ]
                               if { $DOF == 0 } {
                                 error "Invalid Id '$Id': No link has been added to this parametric robot"
                               }
                               if { $ArgsLength != $DOF } {
                                 error "Invalid # of values '$ArgsLength': Must be $DOF values"
                               }
                               for { set Index 1 } { $Index <= $DOF } { incr Index } {
                                 $IdCmd LinkAdd $Index [ lindex $args [ expr $Index - 1 ] ]
                               }
                             }
                           }
      "vGetDynamicp"       {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PRobot Id vGetDynamicp'"
                             } else {
                               set Result [ $IdCmd GetDynamicp ]
                             }
                           }
      "FollowTarget"       {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PRobot Id FollowTarget'"
                             } else {
                               set DOF [ $IdCmd GetDOF ]
                               if { $DOF == 0 } {
                                 error "Invalid Id '$Id': No link has been added to this parametric robot"
                               }
                               # Set Constants
    						   set Pi 3.1415926535897932384626433832795
    						   set Radians [ expr $Pi / 180.0 ]
    						   set Degrees [ expr 180.0 / $Pi ]
						   # IKine
                               	   $IdCmd LLRLIKine
    						   # Get Joint Values
    						   set mOutJointp [ $IdCmd LLRLGetOutJointp ]
    						   # Change Joint Values to Degrees
    						   set mDOutJointp [ lindex $mOutJointp 0 ]
    						   set i 0
    						   foreach { Value } $mDOutJointp {
      							 set mDOutJointp [ lreplace $mDOutJointp $i $i [ expr $Value * $Degrees ] ]
      							 incr i
    						   }
    						   # Set Links
    						   eval "IOIC::PRobot $Id SetLinks $mDOutJointp"
                             }
                           }
      "mGetDHp"            {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PRobot Id mGetDHp'"
                             } else {
                               set Result [ $IdCmd GetDHp ]
                             }
                           }
      "mGetToolHMT"       {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PRobot Id mGetToolHMT'"
                             } else {
                               set Result [ $IdCmd mGetToolHMT ]
                             }
                           }
      "GetDOF"     {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PRobot Id GetDOF'"
                             } else {
                               set Result [ $IdCmd GetDOF ]
                             }
                           }
      "GetBaseTjc"  {
                               if { $ArgsLength != 0 } {
                                  error "Wrong # args: Should be 'PRobot Id GetBaseTjc'"
                               } else {
                                 set Result [ $IdCmd GetBaseTjc ]
                              }
                           }
    "GetBaseType"  {
                                if { $ArgsLength != 0 } {
                                   error "Wrong # args: Should be 'PRobot Id GetBaseType'"
                                } else {
                                  set Result [ $IdCmd GetBaseType ]
                                }
                             }                 
       "AddBaseTjc"  {
                                 if { $ArgsLength != 0 } {
                                   error "Wrong # args: Should be 'PRobot Id AddBaseTjc'"
                                 } else {
                                   set Result [ $IdCmd AddBaseTjc ]
                                 }
                            }
        "GetBaseTjcId" {
                                       if { $ArgsLength != 0 } {
                                           error "Wrong # args: Should be 'PRobot Id GetBaseTjc'"
                                       } else {
                                           set Result [ $IdCmd GetBaseTjcId ]
                                        }
                                 }
        "GetBaseNumGen" {
                                       if { $ArgsLength != 0 } {
                                           error "Wrong # args: Should be 'PRobot Id GetBaseNumGen'"
                                      } else {
                                           set Result [ $IdCmd  GetBaseNumGen ]
                                      }
                                 }                                                         
      "Link"            {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'PRobot Id Link Index SubOption args'"
                             } else {
                               set Index [ lindex $args 0 ]
                               if { [ RobLab::IsInteger $Index ] == 0 } {
                                 error "Invalid index '$Index': Must be an integer"
                               }
                               set DOF [ $IdCmd GetDOF ]
                               if { $DOF == 0 } {
                                 error "Invalid index '$Index': No link has been added to this parametric robot"
                               }
                               if { ( $Index <= 0 ) || ( $Index > $DOF ) } {
                                 error "Invalid index '$Index': Must be an integer between 1 and $DOF"
                               }
                               set SubOption [ lindex $args 1 ]
                               switch $SubOption {
                                 "Add"         {
                                                 if { $ArgsLength != 3 } {
                                                   error "Wrong # args: Should be 'PRobot Id Link Index Add Value'"
                                                 } else {
                                                   set Value [ lindex $args 2 ]
                                                   if { [ RobLab::IsReal $Value ] == 0 } {
                                                     error "Invalid value '$Value': Must be a real"
                                                   }
                                                   set Result [ $IdCmd LinkAdd $Index $Value ]
                                                 }
                                               }
                                 "mGetLocal"   {
                                                 if { $ArgsLength != 2 } {
                                                   error "Wrong # args: Should be 'PRobot Id Link Index mGetLocal'"
                                                 } else {
                                                   set Result [ $IdCmd LinkGetLocal $Index ]
                                                 }
                                               }
                                 "mGetGlobal"  {
                                                 if { $ArgsLength != 2 } {
                                                   error "Wrong # args: Should be 'PRobot Id Link Index mGetGlobal'"
                                                 } else {
                                                   set Result [ $IdCmd LinkGetGlobal $Index ]
                                                 }
                                               }
                                 "vGetDHp"     {
                                                 if { $ArgsLength != 2 } {
                                                   error "Wrong # args: Should be 'PRobot Id Link Index vGetDHp'"
                                                 } else {
                                                   set Result [ $IdCmd LinkGetDHp $Index ]
                                                 }
                                               }
                                 "Set"         {
                                                 if { $ArgsLength != 3 } {
                                                   error "Wrong # args: Should be 'PRobot Id Link Index Set Value'"
                                                 } else {
                                                   set Value [ lindex $args 2 ]
                                                   if { [ RobLab::IsReal $Value ] == 0 } {
                                                     error "Invalid value '$Value': Must be a real"
                                                   }
                                                   set Result [ $IdCmd LinkSet $Index $Value ]
                                                 }
                                               }
               "GetMinAngle"         {
                                                 	if { $ArgsLength != 2 } {
                                                   		error "Wrong # args: Should be 'PRobot Id Link Index GetMinAngle'"
									} else {
                                                   		set Result [ $IdCmd LinkGetMinAngle $Index ]
									}
								} 
                  "GetMaxAngle"      {
                                                 	if { $ArgsLength != 2 } {
                                                   		error "Wrong # args: Should be 'PRobot Id Link Index GetMaxAngle'"
									} else {
                                                   		set Result [ $IdCmd LinkGetMaxAngle $Index ]
									}
								}
                    "GetAngle"      {
                                                 	if { $ArgsLength != 2 } {
                                                   		error "Wrong # args: Should be 'PRobot Id Link Index GetAngle'"
									} else {
                                                   		set Result [ $IdCmd LinkGetAngle $Index ]
									}
							 }                                         
                                 default       {
                                                 error "Bad Link suboption '$SubOption': Must be Add mGetLocal, mGetGlobal, vGetDHp, Set, GetMinAngle, GetMaxAngle or GetAngle"
                                               }
                               }
                             }
                           }
      "SetLinks"           {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'PRobot Id SetLinks Values'"
                             } else {
                               set DOF [ $IdCmd GetDOF ]
                               if { $DOF == 0 } {
                                 error "Invalid Id '$Id': No link has been added to this parametric robot"
                               }
                               if { $ArgsLength != $DOF } {
                                 error "Invalid # of values '$ArgsLength': Must be $DOF values"
                               }
                               for { set Index 1 } { $Index <= $DOF } { incr Index } {
                                 set Result [ $IdCmd LinkSet $Index [ lindex $args [ expr $Index - 1 ] ] ]
                               }
                             }
                           }
      "LLRL"               {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'PRobot Id LLRL SubOption'"
                             } else {
                               set SubOption [ lindex $args 0 ]
                               switch $SubOption {
                                 "mGetDHp"       {
                                                   if { $ArgsLength != 1 } {
                                                     error "Wrong # args: Should be 'PRobot Id LLRL mGetDHp'"
                                                   } else {
                                                     set Result [ $IdCmd LLRLGetDHp ]
                                                   }
                                                 }
                                 "mGetInJointp"  {
                                                   if { $ArgsLength != 1 } {
                                                     error "Wrong # args: Should be 'PRobot Id LLRL mGetInJointp'"
                                                   } else {
                                                     set Result [ $IdCmd LLRLGetInJointp ]
                                                   }
                                                 }
                                                 
                "mGetOriginAngles"  {
                                                  	if { $ArgsLength != 1 } {
                                                     		error "Wrong # args: Should be 'PRobot Id LLRL mGetOriginAngles'"
								  	} else {
	                                                	set Result [ $IdCmd LLRLGetOriginAngles ]
									}
								 }
            "mGetNestAngles"      {
                                                  	if { $ArgsLength != 1 } {
                                                     		error "Wrong # args: Should be 'PRobot Id LLRL mGetNestAngles'"
									} else {
	                                                	set Result [ $IdCmd LLRLGetNestAngles ]
									}
								  }
                                 "mGetInHMT"     {
                                                    if { $ArgsLength != 1 } {
                                                      error "Wrong # args: Should be 'PRobot Id LLRL mGetInHMT'"
                                                    } else {
                                                      set Result [ $IdCmd LLRLGetInHMT ]
                                                    }
                                                  }
                                 "mSetInHMT"     {
                                                    if { $ArgsLength != 1 } {
                                                      error "Wrong # args: Should be 'PRobot Id LLRL mSetInHMT InHMT'"
                                                    } else {
                                                      set Result [ $IdCmd LLRLGetInHMT InHMT]
                                                    }
                                                  }
                                 "mGetOutJointp"  {
                                                    if { $ArgsLength != 1 } {
                                                      error "Wrong # args: Should be 'PRobot Id LLRL mGetOutJointp'"
                                                    } else {
                                                      set Result [ $IdCmd LLRLGetOutJointp ]
                                                    }
                                                  }
                      "mGetOutHMT"     {
                                                    if { $ArgsLength != 1 } {
                                                      error "Wrong # args: Should be 'PRobot Id LLRL mGetOutHMT'"
                                                    } else {
                                                      set Result [ $IdCmd LLRLGetOutHMT ]
                                                    }
                                                  }
                       "mJacobian"         {
                                                      if { $ArgsLength != 2 } {
                                                          error "Wrong # args: Should be 'PRobot Id LLRL mJacobian Local or Global'"
                                                      } else {
                                                          set JacobType [ lindex $args 1 ]
                                                          set Result [ $IdCmd LLRLmJacobian $JacobType ]
                                                      }
                                                  }                           
                                 "FKine"      {
                                                   if { $ArgsLength != 1 } {
                                                     error "Wrong # args: Should be 'PRobot Id LLRL FKine'"
                                                   } else {
                                                     set Result [ $IdCmd LLRLFKine ]
                                                   }
                                                 }
                                 "IKine"         {
                                                   if { $ArgsLength != 1 } {
                                                     error "Wrong # args: Should be 'PRobot Id LLRL IKine'"
                                                   } else {
                                                     set Result [ $IdCmd LLRLIKine ]
                                                   }
                                                 }
                                 "RotVec"     {
                                                   if { $ArgsLength != 2 } {
                                                     error "Wrong # args: Should be 'PRobot Id LLRL RotVec AngleValue'"
                                                    } else {
                                                      set Angle [ lindex $args 1 ]
                                                        if { [ RobLab::IsInteger $Angle ] == 0 } {
                                                      error "Invalid angle '$Angle': Must be an integer"
                                                     }
                                                     set Result [ $IdCmd LLRLRotVec $Angle ]
                                                    }
                                                }
                                 default     {
                                                   error "Bad Link suboption '$SubOption': Must be mGetDHp, mGetInJointp, mGetInHMT, mGetOutJointp, mGetOutHMT, FKine, IKine or RotVec"
                                                 }
                               }
                             }
                           }
      "GetToolTransformId" {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PRobot Id GetToolTransformId'"
                             } else {
                               set Result [ $IdCmd GetToolTransformId ]
                             }
                           }
      default              {
                             error "Bad option '$Option': Must be AddLinks, mGetDHp, GetDOF, Link, SetLinks, LLRL or GetToolTransformId"
                           }
    }
    return $Result
  }
