# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : IOIC.tcl
#  Date         : 2003-02-23
#  Description  : Implementation of input output interface commands
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
# IOIC.tcl
#		This file implements the TCL code for the Input Output Interface Commands (IOIC)
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-05-19
#
# Requirements:  IOICPRobot.tcl
#                               IOICWorld.tcl
#
#SCCS: %Z% %M% %I% %E% %U%
# --------------------------------------------------------------------------------------------------------------------------

namespace eval IOIC {

  # Files that are also needed by this namespace
     # (IOIC commands for a parametric robot)
  	 source ./Tcl/IOIC/IOICPRobot.tcl

    # (IOIC commands for a simulation world)
    source ./Tcl/IOIC/IOICWorld.tcl

   #Initialization of global variables
   variable Result

  # TclForceReDraw procedure --
  #
  #		This procedure is used when the whole display scene is needed to
  #		be redrawn from a console command.
  #
  # Arguments:
  #
  # Results on success: {}
  #
  proc ForceReDraw {} {
    variable Result
    set Result {}
    TclForceReDraw
    return $Result
  }
  
  # FKIne procedure --
  #
  #
  # Arguments:
  #
  # Results on success: {}
  #
  proc FKine  { mDHp mJointp } {
    variable Result
    MessagesWindow::Puts "mDHp = $mDHp\n"
    MessagesWindow::Puts "mJointp = $mJointp\n"
    
    set NumJoints [ llength $mJointp ]
    MessagesWindow::Puts "NumJoints = $NumJoints\n"
    
    set PRobotIndex [ IOIC::World $InvKineToolBar::WorldId GetPRobotIndex $InvKineToolBar::PRobotId]
    MessagesWindow::Puts "ProbotId = $InvKineToolBar::PRobotId \n"
    MessagesWindow::Puts "ProbotIndex = $PRobotIndex \n"
    
    set Result [ $ObjectNavigator::WorldNodeId FKine $mJointp $NumJoints $PRobotIndex ]
    return $Result
  }

  # Puts Message --
  #
  #		This procedure prints a message in the Messages window.
  #
  # Arguments:
  #
  # Results on success: {}
  #
  proc Puts { Message } {
    variable Result
    set Result {}
    MessagesWindow::Puts "$Message\n"
    return $Result
  }

  # Transform --
  #
  #		This procedure applies an homogeneus transformation to any ROBOMOSP element that has a
  #        HGLMT structure.
  #
  #     Arguments        Result on success
  #
  # Id Add P1 Value            {}
  #               P2 Value            {}
  #               P3 Value            {}
  #               O1 Value            {}
  #               O2 Value            {}
  #               O3 Value            {}
  #               Get P1                 1 Position parameter
  #                       P2                 2 Position parameter
  #                       P3                 3 Position parameter
  #               PType                 Position type ( 0. Cartesian, 1. Cylindrical, 2. Spherical )
  #                  O1                     O1
  #                  O2                     O2
  #                  O3                     O3
  #               OType                Orientation type ( 0. Euler XYZ, 1. Euler X, 2. Euler Y, 3. Euler RPY )
  #               RType                Reference type ( 0. Local, 1. Global )
  #               mLocal               Local matrix
  #               mGlobal             Global matrix
  #              Set P1 Value           {}
  #                     P2 Value           {}
  #                     P3 Value           {}
  #                     PType Value   {}
  #                     O1 Value          {}
  #                     O2 Value          {}
  #                     O3 Value          {}
  #                     OType Value  {}
  #                     RType Value   {}
  #
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

  # PObject --
  #
  #		This procedure implements all the IOIC commands that can be applied to a
  #        Parametric Object.
  #
  #       Arguments                                  Result on success
  # Id vGetDynamicp       Dynamic Parameters ( Mass, rx, ry, rz, lxx,
  #                                                                                       lyy, lzz, lxy, lyz, lxz, Jm, G, B, Tc+, Tc- )
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

  # PTrajectory --
  #
  #		This procedure implements all the IOIC commands that can be applied to a
  #        Parametric Trajectory.
  #
  #            Arguments                             Result on success
  # Id GetParameters           The Trajectory's name and its HGLMT structure.
  # Id GetNumGenPoints    The Trajectory's number of generated points.
  # Id GetGenPoint                The vPO vector for a generated point.
  #
  proc PTrajectory { Id Option args } {
    variable Result
    set Result {}
    set ArgsLength [ llength $args ]
    set IdCmd Node$Id
    if { [ RobLab::IsInteger $Id ] == 0 } {
      error "Invalid Id '$Id': Must be an integer"
    } elseif { [ TkNTree::IsCommand $IdCmd ] == $TkNTree::False } {
      error "Invalid Id '$Id': Given Id doesn't exist" 
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

  # Worlds --
  #
  #		This command is used when many worlds are active.
  #
  #     Arguments                            Result on success
  #
  # Open FileName                                    Id
  # Create                                                     Id
  # LsWorlds                             List of worlds in RobLab
  # GetWorldId                         Returns the Id of the current World Node
  # GetWorldId Index                                Id
  # GetWorldIndex                  Returns the index of the current World Node
  # GetActiveWorld                Returns the index of the current active World
  #
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
