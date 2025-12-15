# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------

#  File Name    : IOIC.tcl
#  Date         : 2003-02-23
#  Description  : Implementation of input output interface commands
#  Requirements : 
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co

# --------------------------------------------------------------------------------------------------------------------------

namespace eval IOIC {

  # Procedures :
  # proc ForceReDraw {}
  # proc Puts { String }
  # proc Transform { Id Option args }
  # proc PTrajectory { Id Option args }
  # proc PObject { Id Option args }
  # proc PRobot { Id Option args }
  # proc World { Id Option args }
  # proc Worlds { Option args }
  
  # Initialization of global variables
  variable Result

  # ---------------------------------------------------------------------
  #
  #                                      Result on success
  #
  # TclForceReDraw                       {}
  #
  # ---------------------------------------------------------------------
  proc ForceReDraw {} {
    variable Result
    set Result {}    
    TclForceReDraw
    return $Result
  }

  # -----------------------------------------------------------
  #
  #                                      Result on success
  #
  # Puts Message                         {}
  #
  # -----------------------------------------------------------
  proc Puts { Message } {
    variable Result
    set Result {}
    MessagesWindow::Puts "$Message\n"
    return $Result
  }

  # --------------------------------------------------------------------
  #
  #                                      Result on success
  #
  # Transform Id Add P1 Value            {}
  #                  P2 Value            {}
  #                  P3 Value            {}
  #                  O1 Value            {}
  #                  O2 Value            {}
  #                  O3 Value            {}
  #              Get P1                  1 Position parameter
  #                  P2                  2 Position parameter
  #                  P3                  3 Position parameter
  #                  PType               Position type ( 0. Cartesian, 1. Cylindrical, 2. Spherical )
  #                  O1                  O1
  #                  O2                  O2
  #                  O3                  O3
  #                  OType               Orientation type ( 0. Euler XYZ, 1. Euler X, 2. Euler Y, 3. Euler RPY )
  #                  RType               Reference type ( 0. Local, 1. Global )
  #                  mLocal              Local matrix
  #                  mGlobal             Global matrix
  #              Set P1 Value            {}
  #                  P2 Value            {}
  #                  P3 Value            {}
  #                  PType Value         {}
  #                  O1 Value            {}
  #                  O2 Value            {}
  #                  O3 Value            {}
  #                  OType Value         {}
  #                  RType Value         {}
  #
  # --------------------------------------------------------------------
  proc Transform { Id Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    set IdCmd Node$Id
    if { [ RobLab::IsInteger $Id ] == 0 } {
      error "Invalid Id '$Id': Must be an integer"
    } elseif { [ TkNTree::IsCommand $IdCmd ] == $TkNTree::False } {
      error "Invalid Id '$Id': Given Id doesn't exist"
    } else {
      set Types [ list WorldsTarget WorldsObjectsTitle WorldsRobotsTitle WorldsRobotsToolTransform WorldsTrajectorysTitle ]
      set Count 0
      foreach Type $Types {
        if { [ ObjectNavigator::GetObjectType $IdCmd ] == $Type } {
          incr Count
        }
      }
      if { $Count == 0 } {
        error "Invalid Id '$Id': Given Id cannot be transformed"
      }
    }
    switch $Option {
      "Add"                {
                             if { $ArgsLength != 2 } {
                               error "Wrong # args: Should be 'Transform Id Add SubOption Value'"
                             } else {
                               set Value [ lindex $args 1 ]
                               if { [ RobLab::IsReal $Value ] == 0 } {
                                 error "Invalid value '$Value': Must be a real"
                               }
                               set SubOption [ lindex $args 0 ]
                               switch $SubOption {
                                 "P1"          {
                                                 set Result [ $IdCmd ModifyProperty vPO1Add $Value ]
                                               }
                                 "P2"          {
                                                 set Result [ $IdCmd ModifyProperty vPO2Add $Value ]
                                               }
                                 "P3"          {
                                                 set Result [ $IdCmd ModifyProperty vPO3Add $Value ]
                                               }
                                 "O1"          {
                                                 set Result [ $IdCmd ModifyProperty vPO4Add $Value ]
                                               }
                                 "O2"          {
                                                 set Result [ $IdCmd ModifyProperty vPO5Add $Value ]
                                               }
                                 "O3"          {
                                                 set Result [ $IdCmd ModifyProperty vPO6Add $Value ]
                                               }
                                 default       {
                                                 error "Bad Add suboption '$SubOption': Must be P1, P2, P3, O1, O2 or O3"
                                               }
                               }
                             }
                           }
      "Get"                {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'Transform Id Get SubOption'"
                             } else {
                               set SubOption [ lindex $args 0 ]
                               switch $SubOption {
                                 "P1"          {
                                                 set Result [ $IdCmd ModifyProperty vPO1Get 0 ]
                                               }
                                 "P2"          {
                                                 set Result [ $IdCmd ModifyProperty vPO2Get 0 ]
                                               }
                                 "P3"          {
                                                 set Result [ $IdCmd ModifyProperty vPO3Get 0 ]
                                               }
                                 "PType"       {
                                                 set Result [ $IdCmd ModifyProperty PTypeGet 0 ]
                                               }
                                 "O1"          {
                                                 set Result [ $IdCmd ModifyProperty vPO4Get 0 ]
                                               }
                                 "O2"          {
                                                 set Result [ $IdCmd ModifyProperty vPO5Get 0 ]
                                               }
                                 "O3"          {
                                                 set Result [ $IdCmd ModifyProperty vPO6Get 0 ]
                                               }
                                 "OType"       {
                                                 set Result [ $IdCmd ModifyProperty OTypeGet 0 ]
                                               }
                                 "RType"       {
                                                 set Result [ $IdCmd ModifyProperty RTypeGet 0 ]
                                               }
                                 "mLocal"      {
                                                 set Result [ $IdCmd ModifyProperty mLocalGet 0 ]
                                               }
                                 "mGlobal"     {
                                                 set Result [ $IdCmd ModifyProperty mGlobalGet 0 ]
                                               }
                                 default       {
                                                 error "Bad Set suboption '$SubOption': Must be P1, P2, P3, PType, O1, O2, O3, OType, RType, mLocal or mGlobal"
                                               }
                               }
                             }
                           }
      "Set"                {
                             if { $ArgsLength != 2 } {
                               error "Wrong # args: Should be 'Transform Id Set SubOption Value'"
                             } else {
                               set Value [ lindex $args 1 ]
                               if { [ RobLab::IsReal $Value ] == 0 } {
                                 error "Invalid value '$Value': Must be a real"
                               }
                               set SubOption [ lindex $args 0 ]
                               switch $SubOption {
                                 "P1"          {
                                                 set Result [ $IdCmd ModifyProperty vPO1 $Value ]
                                               }
                                 "P2"          {
                                                 set Result [ $IdCmd ModifyProperty vPO2 $Value ]
                                               }
                                 "P3"          {
                                                 set Result [ $IdCmd ModifyProperty vPO3 $Value ]
                                               }
                                 "PType"       {
                                                 set Value [ lindex $args 1 ]
                                                 if { [ RobLab::IsInteger $Value ] == 0 } {
                                                   error "Invalid value '$Value': Must be an integer"
                                                 }
                                                 set Types $ObjectNavigator::LstPTypes
                                                 set Length [ expr [ llength $Types ] - 1 ]
                                                 if { ( $Value < 0 ) || ( $Value > $Length ) } {
                                                   error "Invalid value '$Value': Must be an integer between 0 and $Length ( $Types )"
                                                 }
                                                 set Result [ $IdCmd ModifyProperty PType [ lindex $Types $Value ] ]
                                               }
                                 "O1"          {
                                                 set Result [ $IdCmd ModifyProperty vPO4 $Value ]
                                               }
                                 "O2"          {
                                                 set Result [ $IdCmd ModifyProperty vPO5 $Value ]
                                               }
                                 "O3"          {
                                                 set Result [ $IdCmd ModifyProperty vPO6 $Value ]
                                               }
                                 "OType"       {
                                                 set Value [ lindex $args 1 ]
                                                 if { [ RobLab::IsInteger $Value ] == 0 } {
                                                   error "Invalid value '$Value': Must be an integer"
                                                 }
                                                 set Types $ObjectNavigator::LstOTypes
                                                 set Length [ expr [ llength $Types ] - 1 ]
                                                 if { ( $Value < 0 ) || ( $Value > $Length ) } {
                                                   error "Invalid value '$Value': Must be an integer between 0 and $Length ( $Types )"
                                                 }
                                                 set Result [ $IdCmd ModifyProperty OType [ lindex $Types $Value ] ]
                                               }
                                 "RType"       {
                                                 set Value [ lindex $args 1 ]
                                                 if { [ RobLab::IsInteger $Value ] == 0 } {
                                                   error "Invalid value '$Value': Must be an integer"
                                                 }
                                                 set Types $ObjectNavigator::LstRTypes
                                                 set Length [ expr [ llength $Types ] - 1 ]
                                                 if { ( $Value < 0 ) || ( $Value > $Length ) } {
                                                   error "Invalid value '$Value': Must be an integer between 0 and $Length ( $Types )"
                                                 }
                                                 set Result [ $IdCmd ModifyProperty RType [ lindex $Types $Value ] ]
                                               }
                                 default       {
                                                 error "Bad Set suboption '$SubOption': Must be P1, P2, P3, PType, O1, O2, O3, OType or RType"
                                               }
                               }
                             }
                           }
      default              {
                             error "Bad option '$Option': Must be Add, Get or Set"
                           }
    }
    return $Result
  }

  # ------------------------------------------------------------------
  #
  #                                      Result on success
  #
  # PObject Id vGetDynamicp              Dynamic Parameters ( Mass, rx, ry, rz, lxx,
  #                                      lyy, lzz, lxy, lyz, lxz, Jm, G, B, Tc+, Tc- )
  #
  # ------------------------------------------------------------------
  proc PObject { Id Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    set IdCmd Node$Id
    if { [ RobLab::IsInteger $Id ] == 0 } {
      error "Invalid Id '$Id': Must be an integer"
    } elseif { [ TkNTree::IsCommand $IdCmd ] == $TkNTree::False } {
      error "Invalid Id '$Id': Given Id doesn't exist"
    } elseif { [ ObjectNavigator::GetObjectType $IdCmd ] != "WorldsObjectsTitle" } {
      error "Invalid Id '$Id': Given Id is not a parametric object"
    }
    switch $Option {
      "vGetDynamicp"       {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PObject Id vGetDynamicp'"
                             } else {
                               set Result [ $IdCmd GetDynamicp ]
                             }
                           }
      default              {
                             error "Bad option '$Option': Must be vGetDynamicp"
                           }
    }
    return $Result
  }

  # ------------------------------------------------------------------
  #
  #                                      Result on success
  #
  # PTrajectory Id GetParameters         The Trajectory's name and HGLMT structure.
  # PTrajectory Id GetNumGenPoints       The Trajectory's number of generated points.
  # PTrajectory Id GetGenPoint           The vPO vector for a generated point.
  #
  # ------------------------------------------------------------------
  proc PTrajectory { Id Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    set IdCmd Node$Id
    if { [ RobLab::IsInteger $Id ] == 0 } {
      error "Invalid Id '$Id': Must be an integer"
    } elseif { [ TkNTree::IsCommand $IdCmd ] == $TkNTree::False } {
      error "Invalid Id '$Id': Given Id doesn't exist"
    } elseif { [ ObjectNavigator::GetObjectType $IdCmd ] != "WorldsTrajectorysTitle" } {
      error "Invalid Id '$Id': Given Id is not a parametric trajectory"
    }
    switch $Option {
      "GetParameters"     {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PTrajectory Id GetParameters'"
                             } else {
                               set Result [ $IdCmd GetParameters ]
                             }
                           }
      "GetNumGenPoints"    {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'PTrajectory Id GetNumGenPoints'"
                             } else {
                               set Result [ $IdCmd GetNumGenPoints]
                             }
                           }
      "GetGenPoint"       {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'PTrajectory Id GetGenPoint Index'"
                             } else {
                               set Index [ lindex $args 0 ]
                               if { [ RobLab::IsInteger $Index ] == 0 } {
                                 error "Invalid index '$Index': Must be an integer"
                               }
                               set Result [ $IdCmd mGlobalGet $Index]
                             }
                           }
      default              {
                             error "Bad option '$Option': Must be GetParameters, GetNumGenPoints,
                                    GetGenPoint"
                           }
    }
    return $Result
  }

  # -------------------------------------------------------------------------------------------
  #
  #                                      Result on success
  #
  # PRobot Id AddLinks Values            {}
  # PRobot Id vGetDynamicp               PRobot Dinamic Parameters
  # PRobot Id mGetDHp                    DH parameters matrix
  # PRobot Id mGetToolHMT                ToolTransform HMT
  # PRobot Id GetDOF                     DOF
  # PRobot Id Link Index Add Value       {}
  #                      mGetLocal       DH local matrix
  #                      mGetGlobal      DH global matrix
  #                      vGetDHp         DH parameters vector
  #                                     ( Alpha, a, Theta, d, Minimum, Maximum,
  #                                       Origin, Nest, Type ( 0 Revolute, 1 Prismatic ) )
  #                      Set Value       {}
  # PRobot Id SetLinks Values            {}
  # PRobot Id LLRL mGetDHp               {}
  #                mGetInJointp          {}
  #                mGetInHMT             DH parameters matrix ( Alpha, a, Theta,
  #                                                             d ( 0 Revolute, 1 Prismatic ) )
  #                mGetOutJointp         {}
  #                mGetOutHMT            DH parameters matrix ( Alpha, a, Theta,
  #                                                             d ( 0 Revolute, 1 Prismatic ) )
  #                FKine                 {}
  #                IKine                 {}
  # PRobot Id GetToolTransformId         Id
  #
  # -------------------------------------------------------------------------------------------
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
      "GetDOF"             {
                             if { $ArgsLength != 0 } { 
                               error "Wrong # args: Should be 'PRobot Id GetDOF'"
                             } else {
                               set Result [ $IdCmd GetDOF ]
                             }
                           }
      "Link"               {
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
                                 default       {
                                                 error "Bad Link suboption '$SubOption': Must be Add mGetLocal, mGetGlobal, vGetDHp or Set"
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
                             if { $ArgsLength != 1 } {
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
                                 "FKine"         {
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
                                 default         {
                                                   error "Bad Link suboption '$SubOption': Must be mGetDHp, mGetInJointp, mGetInHMT, mGetOutJointp, mGetOutHMT, FKine or IKine"
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

  # -----------------------------------------------------------------
  #
  #                                      Result on success
  #
  # World Id Close                         {}
  # World Id Save                          {}
  # World Id Show                          {}
  # World Id LsPObjects                    List of parametric objects in world Id
  # World Id LsPRobots                     List of parametric robots in world Id
  # World Id LsPTrajectories               List of parametric trajectories in world Id
  # World Id GetTargetId                   Id
  # World Id GetPObjectId Index            Id
  # World Id GetPRobotId Index             Id
  # World Id GetPRobotIdDesc Description   Id
  # World Id GetPTrajectoryId Index        Id
  # World Id GetRobotIndex Id              Index
  # World Id GetPTrajectoryIndex Id        Index
  # World Id LoadPTrajectory FileName      PTrajectory Id
  # World Id WorldSimulate PRobotId PTjcId Kinematic Simulation
  #
  # -----------------------------------------------------------------
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
                               set TemporaryNTree  [ NTree::GetLastSon $ObjectNavigator::CurrentNode ]
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
                               set TemporaryNTree  [ NTree::GetLastSon $ObjectNavigator::CurrentNode ]
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
        "WorldSimulate"    {
                             if { $ArgsLength < 1 } {
                               error "Wrong # args: Should be 'World Id WorldSimulate PRobotId PTjcId'"
                             } else {
                               set PRobotId [ lindex $args 0 ]
                               set PTjcId   [ lindex $args 1 ]
                               if { [ RobLab::IsInteger $PRobotId ] == 0 } {
                                 error "Invalid Id '$PRobotId': Must be an integer"
                               }
                               if { [ RobLab::IsInteger $PTjcId ] == 0 } {
                                 error "Invalid Id '$PTjcId': Must be an integer"
                               }
                               set PRobotIndex [ IOIC::World $Id GetPRobotIndex $PRobotId]
                               set PTjcIndex   [ IOIC::World $Id GetPTrajectoryIndex $PTjcId]
                               set Result [ $IdCmd WorldSimulate $PRobotIndex $PTjcIndex ]
                             }
                           }
      default              {
                             error "Bad option '$Option': Must be Close, Save, Show, GetTargetId, GetPRobotId, GetPRobotIdDesc, LsPRobots, GetPTrajectoryId, LsPTrajectories, LoadPTrajectory, or WorldSimulate"
                           }
    }
    return $Result
  }

  # ---------------------------------------------------------------------------
  #
  #                                      Result on success
  #
  # Worlds Open FileName                 Id
  # Worlds Create                        Id
  # Worlds LsWorlds                      List of worlds in RobLab
  # Worlds GetWorldId                    Returns the Id of the current World Node
  # Worlds GetWorldId Index              Id
  # Worlds GetWorldIndex                 Returns the index of the current World Node
  # Worlds GetActiveWorld                Returns the index of the current active World
  #
  # ---------------------------------------------------------------------------
  proc Worlds { Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    switch $Option {
        "Open"             {
                            if { $ArgsLength != 1 } {
                              error "Wrong # args: Should be 'Worlds Open FileName'"
                            } else {
                              set FileName [ lindex $args 0 ]
                              set FileName "Saves/Worlds/$FileName"
                              ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON $ObjectNavigator::WorldsNode 1
                              ObjectNavigator::OpenFile $FileName
                              set Result [ string range $ObjectNavigator::CurrentNode 4 end ]
                             }
                           }
        "Create"           {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'Worlds Create'"
                             } else {
                               ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON $ObjectNavigator::WorldsNode 1
                               set ObjectNavigator::TclNTreeA [ ObjectNavigator::CreateWorldsNode ]
                               ObjectNavigator::AddGraphicNTree [ lindex $ObjectNavigator::TclNTreeA 0 ]
                               # Begin C Commands !!!
                               TclCreateWorld New [ lindex $ObjectNavigator::TclNTreeA 0 ] [ lindex $ObjectNavigator::TclNTreeA 1 ]
                               # End C Commands !!!
                               ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetLastSon $ObjectNavigator::CurrentNode ] 1
                               set Result [ string range $ObjectNavigator::CurrentNode 4 end ]
                             }
                           }
        "LsWorlds"         {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'Worlds LsWorlds'"
                             } else {
                               set i 1
                               set TemporaryNTree [ NTree::GetFirstSon $ObjectNavigator::WorldsNode ]
                               while { ! [ NTree::Empty $TemporaryNTree ] } {
                                 set ResultLine [ list $i [ string range $TemporaryNTree 4 end ] [ TkNTree::GetTitle $TemporaryNTree ] ]
                                 lappend Result $ResultLine
                                 incr i
                                 set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
                               }
                             }
                           }
        "GetWorldId"       {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'Worlds GetWorldId Index'"
                             } else {
                               set Index [ lindex $args 0 ]
                               if { [ RobLab::IsInteger $Index ] == 0 } {
                                 error "Invalid index '$Index': Must be an integer"
                               }
                               set i 1
                               set TemporaryNTree [ NTree::GetFirstSon $ObjectNavigator::WorldsNode ]
                               if { [ NTree::Empty $TemporaryNTree ] } {
                                 error "Invalid index '$Index': No world has been opened"
                               }
                               while { ! [ NTree::Empty $TemporaryNTree ] } {
                                 if { $i == $Index } {
                                   set Result [ string range $TemporaryNTree 4 end ]
                                   break
                                 }
                                 incr i
                                 set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
                               }
                               if { $Result == {} } {
                                 incr i -1
                                 error "Invalid index '$Index': Must be an integer between 1 and $i"
                               }
                             }
                           }
        "GetWorldIndex"    {
                             if { $ArgsLength != 1 } {
                               error "Wrong # args: Should be 'Worlds GetWorldIndex Node'"
                             } else {
                               set Index [ lindex $args 0 ]
                               set i 1
                               set TemporaryNTree [ NTree::GetFirstSon $ObjectNavigator::WorldsNode ]
                               while { ! [ NTree::Empty $TemporaryNTree ] } {
                                 if { $Index == $TemporaryNTree } {
                                   set Result $i
                                   break
                                 }
                                 incr i
                                 set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
                               }
                             }
                           }
        "GetActiveWorld"   {
                             if { $ArgsLength != 0 } {
                               error "Wrong # args: Should be 'Worlds GetActiveWorld'"
                             } else {
                               set Index $ObjectNavigator::CurrentNode
                               set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
                               set Result [ IOIC::Worlds GetWorldId $WorldIndex ]
                             }
                           }
        default            {
                             error "Bad option '$Option': Must be Open, Create, LsWorlds, GetWorldId,
                                    GetWorldIndex, or GetActiveWorld"
                           }
    }
    return $Result
  }

}

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
