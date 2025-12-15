#----------------------------------------------------------------------------------------------------------------------------------
# ObjNavTjc.tcl
# 		This file implements the TCL code for creating and managing the Trajectories Object
#      Navigator and  Properties Palette procedures.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-05-19
#
# Requirements:
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

  # Initialization of global variables needed by the trayectory procedures
  #Tp Scales
  variable ScaleTpMinimum   0.00000001
  variable ScaleTpMaximum   0.90000000
  variable ScaleTpFactor    0.00000001

  variable ScaleOpenTpMinimum   0.00100000
  variable ScaleOpenTpMaximum   0.99999999
  variable ScaleOpenTpFactor    0.00000001

  variable ScaleClosedTpMinimum   0.00000001
  variable ScaleClosedTpMaximum   0.49999999
  variable ScaleClosedTpFactor    0.00000001

  # Control Points Scale
  variable ScalePointsMinimum 2
  variable ScalePointsMaximum 5000
  variable ScalePointsFactor  1

  # List of Trajectory's attributes to be displayed in the Properties Palette Window.
  variable LstSpaceTypes         [ list {Cartesian} {Joint}]
  variable LstOrderTypes         [ list {Translation} {Rotation} {None}]
  variable LstTjcTransTypes      [ list {None} {Straight Line} {Arch} {Circle} {Natural Cubic Spline} {Cubic Spline} {Fourth Degree Spline(Each point velocity control)} {Bezier Spline} {Serpenoid} ]
  variable LstTjcRotTypes        [ list {None} {Interpolated} {Arbitrary Axes}]
  variable LstTjcJointTypes      [ list {None} {Sincronous PTP} {Each Axes PTP}]
  variable LstTransMotionTypes   [ list {Constant Velocity} {Constant Acceleration} {Trapezoidal Velocity with Acceleration} {Trapezoidal Velocity with Deceleration} {Trapezoidal Velocity with Acceleration and Deceleration} {None} {None} ]
  variable LstRotMotionTypes     [ list {Constant Velocity} {Constant Acceleration} {Trapezoidal Velocity with Acceleration} {Trapezoidal Velocity with Deceleration} {Trapezoidal Velocity with Acceleration and Deceleration} {None} {None} ]
  variable LstSplinesTypes       [ list {Variable} {None}]
  variable LstCtrlVarsConstVel   [ list {Time} {Velocity} {None}]
  variable LstCtrlVarsConstAccel [ list {Time and Vo} {Vo and Vf} {None}]
  variable LstCtrlVarsTrap       [ list {Time} {Velocity} {None}]
  variable LstCtrlVarsVar        [ list {Control Points} ]
  variable LstCtrlVarsBez        [ list {Control Points} {Vo and Vf} {None}]
  variable LstUnionTypes         [ list {False} {True}]
  variable LstUnionMode          [ list {Automatic} {User Defined}]

  variable LstSecondCtrlVarsConstVel   [ list {Time} ]
  variable LstSecondCtrlVarsConstAccel [ list {Time and Vo} ]
  variable LstSecondCtrlVarsTrap       [ list {Time} ]
  variable LstSecondCtrlVarsBez        [ list {Vo and Vf} ]


  # ---------------------------------------------------------------------------------------------------------
  #
  #  The following procedures builds the data subtree for the StrTrajectory
  #   data type
  # ---------------------------------------------------------------------------------------------------------

  # CreateTrajectorysCtrlPointNode --
  #
  #			This procedure creates a trajectory's control point Ntree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateTrajectorysCtrlPointNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } { VlcX 0 } { VlcY 0 } { VlcZ 0 } { VelR 0 } { VelP 0 } { VelY 0 } { PointTime 0 }} {
    set TemporaryNTreeA [ TkNTree::Set "Control Point" StateNone 34 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list TrajectorysCtrlPoint [ CtrlPointsPropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $VlcX $VlcY $VlcZ $VelR $VelP $VelY $PointTime] "" ]
    return $TemporaryNTreeA
  }

  # CreateTrajectorysSectionNode --
  #
  #			This procedure creates a trajectory's section Ntree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateTrajectorysSectionNode { { Name "" } {SpaceType 0} { PrimaryOrderType 2 } { SecondaryOrderType 2 } {PrimaryIntType 0} { PrimaryMotionType 6 } {PrimaryCtrlVar 0} {SecondaryIntType 0} { SecondaryMotionType 6 } {SecondaryCtrlVar 0} {SectionGenPoints 3} {SectionCtrlPoints 0} {PrimaryInput1 0} {PrimaryInput2 0} {SecondaryInput1 0} {SecondaryInput2 0} } {
    variable Id
    variable GlobalPrimaryMotionType
    variable GlobalSecondaryMotionType
    set GlobalPrimaryMotionType $PrimaryMotionType
    set GlobalSecondaryMotionType $SecondaryMotionType
    if { $Name == "" } {
      set TemporaryNTreeA [ TkNTree::Set "Section$Id" StateNone 5 True True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "$Name" StateNone 5 True True True $NTree::Null ]
    }
    TkNTree::SetData $TemporaryNTreeA [ list TrajectorysSection [ SectionPropertiesNTree $TemporaryNTreeA $Name $SpaceType $PrimaryOrderType $SecondaryOrderType $PrimaryIntType $PrimaryMotionType $PrimaryCtrlVar $SecondaryIntType $SecondaryMotionType $SecondaryCtrlVar $SectionGenPoints $SectionCtrlPoints $PrimaryInput1 $PrimaryInput2 $SecondaryInput1 $SecondaryInput2 ] "" ]
    return $TemporaryNTreeA
  }

  # ChangeUnionParameterTime procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
 proc ChangeUnionParameterTime { NTree UnionTime } {
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    set ListOfElements [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]
    PropertyPalette::ChangeNodePropertyValue $NTree $UnionTime
 }

 #  ChangeUnionTypePropertiesNTree procedure.
 #
 # 		This procedure changes the Union Type properties Ntree
 # 			0. Automatic.
 # 			1. User Defined.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeUnionTypePropertiesNTree { UnionType UnionGenMode UnionTime UnionPercentage UnionPoints } {
    variable cvsON
    variable frmONTB
    # Take care with IOIC because this choise is based on the current node selecction
    set UnionTypeNTreeA $PropertyPalette::CurrentNode
    set Parent [ GetPropertyParent $UnionTypeNTreeA ]

    # Collapse NTree to be changed.
    RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
    # Delete old sons.
    set TemporaryNTree [ NTree::GetFirstSon $UnionTypeNTreeA ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      NTree::DisAssociate $TemporaryNTree
      NTree::Destroy $TemporaryNTree
      set TemporaryNTree [ NTree::GetFirstSon $UnionTypeNTreeA ]
    }

    # Create new Properties NTree.
    UnionTypePropertiesNTree $Parent $UnionTypeNTreeA $UnionType $UnionGenMode $UnionTime $UnionPercentage $UnionPoints
    # Expand new NTree.
    RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
 }

  #  UnionTypePropertiesNTree procedure --
  #
  # 		This procedure draws the UnionType Properties NTree according to the value
  # 		passed by the NTree ChangeUnionTypePropertiesNTree procedure.
  #
  # Arguments:
  #
  # Results:
  #
  proc UnionTypePropertiesNTree { Parent UnionTypeNTreeA UnionType UnionGenMode UnionTime UnionPercentage UnionPoints } {
   variable ScaleTimeMaximum
   variable ScaleTimeFactor
   variable ScalePointsMaximum
   variable ScalePointsFactor
   variable ScaleTimeMaximum
   variable ScaleTimeFactor
   variable ScaleTpMinimum
   variable ScaleTpMaximum
   variable ScaleTpFactor
   variable LstUnionMode
   variable CurrentNode
   switch $UnionGenMode {
          0 {
              #Automatic
               switch $UnionType {
                       0 {
                           # None
                           # Union Time
                           set PropertiesNTreeData [ list "$Parent" "UnionTime" "Scale" [ list $UnionTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Union Time" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time = $UnionTime" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Number of Generated Points
                           set PropertiesNTreeData [ list "$Parent" "UnionPoints" "Scale" [ list $UnionPoints "3 $ScalePointsMaximum $ScalePointsFactor" ] "Union Number of Generated Points" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Number of Generated Points = $UnionPoints" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Time Percentage
                           set PropertiesNTreeData [ list "$Parent" "UnionPercentage" "Scale" [ list $UnionPercentage "$ScaleTpMinimum $ScaleTpMaximum $ScaleTpFactor" ] "Union Time Percentage" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time Percentage = $UnionPercentage" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                         }
                       1 {
                           #SUSG
                           # Union Time
                           set PropertiesNTreeData [ list "$Parent" "UnionTime" "Scale" [ list $UnionTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Union Time" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time = $UnionTime" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Number of Generated Points
                           set PropertiesNTreeData [ list "$Parent" "UnionPoints" "Scale" [ list $UnionPoints "3 $ScalePointsMaximum $ScalePointsFactor" ] "Union Number of Generated Points" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Number of Generated Points = $UnionPoints" StateNone 21 False True True $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Time Percentage
                           set PropertiesNTreeData [ list "$Parent" "UnionPercentage" "Scale" [ list $UnionPercentage "$ScaleTpMinimum $ScaleTpMaximum $ScaleTpFactor" ] "Union Time Percentage" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time Percentage = $UnionPercentage" StateNone 21 False True True $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                         }
                       2 {
                          #NUSG
                          # Union Time
                          set PropertiesNTreeData [ list "$Parent" "UnionTime" "Scale" [ list $UnionTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Union Time" ]
                          set TemporaryNTreeC [ TkNTree::Set "Union Time = $UnionTime" StateNone 21 False False False $PropertiesNTreeData ]
                          NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                          # Union Number of Generated Points
                          set PropertiesNTreeData [ list "$Parent" "UnionPoints" "Scale" [ list $UnionPoints "3 $ScalePointsMaximum $ScalePointsFactor" ] "Union Number of Generated Points" ]
                          set TemporaryNTreeC [ TkNTree::Set "Union Number of Generated Points = $UnionPoints" StateNone 21 False True True $PropertiesNTreeData ]
                          NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                          # Union Time Percentage
                          set PropertiesNTreeData [ list "$Parent" "UnionPercentage" "Scale" [ list $UnionPercentage "$ScaleTpMinimum $ScaleTpMaximum $ScaleTpFactor" ] "Union Time Percentage" ]
                          set TemporaryNTreeC [ TkNTree::Set "Union Time Percentage = $UnionPercentage" StateNone 21 False False False $PropertiesNTreeData ]
                          NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                         }
               }
            }
          1 {  #User Defined
               switch $UnionType {
                       0 {
                           # None
                           # Union Time
                           set PropertiesNTreeData [ list "$Parent" "UnionTime" "Scale" [ list $UnionTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Union Time" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time = $UnionTime" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Number of Generated Points
                           set PropertiesNTreeData [ list "$Parent" "UnionPoints" "Scale" [ list $UnionPoints "3 $ScalePointsMaximum $ScalePointsFactor" ] "Union Number of Generated Points" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Number of Generated Points = $UnionPoints" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Time Percentage
                           set PropertiesNTreeData [ list "$Parent" "UnionPercentage" "Scale" [ list $UnionPercentage "$ScaleTpMinimum $ScaleTpMaximum $ScaleTpFactor" ] "Union Time Percentage" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time Percentage = $UnionPercentage" StateNone 21 False False False $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                         }
                       1 {
                           #SUSG
                           # Union Time
                           set PropertiesNTreeData [ list "$Parent" "UnionTime" "Scale" [ list $UnionTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Union Time" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time = $UnionTime" StateNone 21 False True True $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Number of Generated Points
                           set PropertiesNTreeData [ list "$Parent" "UnionPoints" "Scale" [ list $UnionPoints "3 $ScalePointsMaximum $ScalePointsFactor" ] "Union Number of Generated Points" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Number of Generated Points = $UnionPoints" StateNone 21 False True True $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                           # Union Time Percentage
                           set PropertiesNTreeData [ list "$Parent" "UnionPercentage" "Scale" [ list $UnionPercentage "$ScaleTpMinimum $ScaleTpMaximum $ScaleTpFactor" ] "Union Time Percentage" ]
                           set TemporaryNTreeC [ TkNTree::Set "Union Time Percentage = $UnionPercentage" StateNone 21 False True True $PropertiesNTreeData ]
                           NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                         }
                       2 {
                          #NUSG
                          # Union Time
                          set PropertiesNTreeData [ list "$Parent" "UnionTime" "Scale" [ list $UnionTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Union Time" ]
                          set TemporaryNTreeC [ TkNTree::Set "Union Time = $UnionTime" StateNone 21 False True True $PropertiesNTreeData ]
                          NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                          # Union Number of Generated Points
                          set PropertiesNTreeData [ list "$Parent" "UnionPoints" "Scale" [ list $UnionPoints "3 $ScalePointsMaximum $ScalePointsFactor" ] "Union Number of Generated Points" ]
                          set TemporaryNTreeC [ TkNTree::Set "Union Number of Generated Points = $UnionPoints" StateNone 21 False True True $PropertiesNTreeData ]
                          NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                          # Union Time Percentage
                          set PropertiesNTreeData [ list "$Parent" "UnionPercentage" "Scale" [ list $UnionPercentage "$ScaleTpMinimum $ScaleTpMaximum $ScaleTpFactor" ] "Union Time Percentage" ]
                          set TemporaryNTreeC [ TkNTree::Set "Union Time Percentage = $UnionPercentage" StateNone 21 False False False $PropertiesNTreeData ]
                          NTree::Add $UnionTypeNTreeA $TemporaryNTreeC
                         }
              }
          }
    }

  }

  #  CreateUnionSectionNode procedure.
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateUnionSectionNode { { UnionType 0 } { UnionGenMode 0 } { UnionTime 0 } { UnionPercentage 0.25 } { UnionPoints 3 }  { Id 0 }  { Option 0 } } {
  variable ScaleTimeMaximum
  variable ScaleTimeFactor
  variable ScaleDistanceMaximum
  variable ScaleTimeMaximum
  variable ScaleTimeFactor
  variable LstUnionMode
  variable CurrentNode
  variable UnionTypeNTree
  if { $Option==0} {
     set UnionTypeNTree $PropertyPalette::CurrentNode
     set Parent [ GetPropertyParent $UnionTypeNTree ]
  }
  if { $Option==1} {
     set UnionTypeNTree $UnionTypeNTree
     set Parent [ GetPropertyParent $UnionTypeNTree ]
  }
  if { $Option==2} {
     set Parent [ GetPropertyParent $UnionTypeNTree ]
     set PropertyPalette::CurrentNode [ NTree::GetLastSon $PropertyPalette::CurrentNode]
  }

  #Section
  set TemporaryNTreeA [ TkNTree::Set "Union Section No $Id" StateExpand 5 False False False $NTree::Null ]
  NTree::Add $UnionTypeNTree $TemporaryNTreeA

  # Union Generated Mode
  set PropertiesNTreeData [ list "$Parent" "UnionGenMode" "List" [ list [ lindex $LstUnionMode $UnionGenMode ] $LstUnionMode ] "Union Generation Mode" ]
  set TemporaryNTreeB [ TkNTree::Set "Union Generation Mode = [ lindex $LstUnionMode $UnionGenMode ]" StateExpand 22 False True True $PropertiesNTreeData ]
  NTree::Add $TemporaryNTreeA $TemporaryNTreeB

  UnionTypePropertiesNTree $Parent $TemporaryNTreeB $UnionType $UnionGenMode $UnionTime $UnionPercentage $UnionPoints

  return $TemporaryNTreeA
 }

  #  CreateTrajectorysNode procedure.
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateTrajectorysNode { { FileName "" } { Description "" } { UnionType 0 } { TjcNumPoints 2 } { TjcTotalTime 0 } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    variable Id
    if { $FileName == "" } {
      set TemporaryNTreeA [ TkNTree::Set "Trajectory$Id.tjc" StateNone 5 True True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 True True True $NTree::Null ]
    }
    TkNTree::SetData $TemporaryNTreeA [ list TrajectorysTitle [ TrajectorysPropertiesNTree $TemporaryNTreeA $Description $UnionType $TjcNumPoints $TjcTotalTime $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] $FileName ]
    return $TemporaryNTreeA
  }


 #----------------------------------------------------------------------------
 # Trajectory's PropertyPallete procedures --
 #
 #		The following procedures belong to the Node properties subtree procedures
 #      and they are used to create and modify the Trajectory`s Properties NTree.
 # ----------------------------------------------------------------------------

  # CtrlPointsPropertiesNTree procedure --
  #
  #		The following procedure is used to create a ontrolPoint
  # 	Properties NTree.
  #
  # Arguments:
  #
  # Results:
  #
  proc CtrlPointsPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type VlcX VlcY VlcZ VelR VelP VelY PointTime} {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 33 False False Undefined $PropertiesNTreeData ]
    # PositionOrientation
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    # Velocity
    NTree::Add $TemporaryNTreeA [ VectorsPropertiesNTree "$Parent" "Velocity" { "VlcX" "VlcY" "VlcZ" } [ list $VlcX $VlcY $VlcZ ] { "X" "Y" "Z" } ]
    # Angular Velocity
    NTree::Add $TemporaryNTreeA [ VectorsPropertiesNTree "$Parent" "Angular Velocity" { "VelR" "VelP" "VelY" } [ list $VelR $VelP $VelY ] { "Roll" "Pitch" "Yaw" } ]
    # Point Time
    set PropertiesNTreeData [ list "$Parent" "PointTime" "Scale" [ list $PointTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
    set TemporaryNTreeB [ TkNTree::Set "Time = $PointTime" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # Visibility
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }

 # ChangeTjcNumberPoints procedure --
 #
 #		This procedure changes the number of Total points belonging to a trajectory.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeTjcNumberPoints { NTree NumberPoints } {

    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor
    set TjcNTree [ NTree::GetFather $NTree ]
    set ListOfElements [ list $ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor ]

    PropertyPalette::ChangePropertyTitleValue $TjcNTree "TjcNumPoints" "Number of Points" $NumberPoints "$ListOfElements"
 }

 # ChangeTjcTimePoints procedure --
 #
 #		This procedure changes a trajectory`s total time and
 #      total number of points.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeTjcTotalTimePoints { Id TotalPoints TotalTime} {

    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor
    set TjcNTree Node$Id
    set ListOfTimeElements [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]
    set ListOfPointsElements [ list $ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor ]

    PropertyPalette::ChangePropertyTitleValue $TjcNTree "TjcNumPoints" "Number of Points" $TotalPoints "$ListOfPointsElements"
    PropertyPalette::ChangePropertyTitleValue $TjcNTree "TjcTotalTime" "Total Time" $TotalTime "$ListOfTimeElements"

 }

 # ChangeSectionNumberCtrlPoints procedure --
 #
 #		This procedure changes the number of Ctrl points in the Section's Property
 #		Palette Window.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSectionNumberCtrlPoints { NTree NumberCtrlPoints MainConstruction IntType} {

    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor

    set ListOfElements [ list $ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor ]

    PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" $NumberCtrlPoints "$ListOfElements"

    if { ($IntType==4) || ($IntType==5) || ($IntType==6) || ($IntType==7) } {

       switch $MainConstruction {
         0 {
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "Control Points" $NumberCtrlPoints "$ListOfElements"
           }
         1 {
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "Control Points" $NumberCtrlPoints "$ListOfElements"
           }
       }
    }
 }

 # ChangeSecondaryInput1 procedure --
 #
 #		This procedure changes the Secondary Input1 value.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSecondaryInput1 { NTree SecondaryInput1 } {

    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    set ListOfElements [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]

    PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "Time" $SecondaryInput1 "$ListOfElements"
 }

 # AddSectionNumberCtrlPoints procedure --
 #
 #		This procedure adds a Ctrl point in the Section's Object Navigator and
 #		Property Palette Window at the end of the CtrlPointsList.
 #
 # Arguments:
 #
 # Results:
 #
 proc AddSectionNumberCtrlPoints { } {
   variable CurrentNode
   set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
   AddGraphicNTree $TemporaryNTreeA
   # Begin C Commands
   $CurrentNode AddCtrlPoint $TemporaryNTreeA
   # End C Commands
 }

 # AddIndexSectionCtrlPoints procedure --
 #
 #		This procedure adds a Ctrl Point in the Section's Object Navigator and
 #		Property Palette Window at the end of the current Section.
 #
 # Arguments:
 #
 # Results:
 #
 proc AddIndexSectionCtrlPoints { } {
   variable CurrentNode
   set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
   AddGraphicNTree $TemporaryNTreeA
   # Begin C Commands
   $CurrentNode AddIndexCtrlPoint $TemporaryNTreeA
   # End C Commands
 }

 # InsertSectionNumberCtrlPoints procedure --
 #
 #		This procedure inserts a control point in the Section's Object Navigator and
 #		Property Palette Window.
 #
 # Arguments:
 #
 # Results:
 #
 proc InsertSectionNumberCtrlPoints { } {
   variable CurrentNode
   set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
   InsertGraphicNTree $TemporaryNTreeA
   # Begin C Commands
   $CurrentNode InsertCtrlPoint $TemporaryNTreeA
   # End C Commands
 }

# DeleteSectionNumberCtrlPoints procedure.
 #
 #		This procedure deletes a Ctrl point in the Section's Object Navigator and
 #		Property Palette Window.
 #
 # Arguments:
 #
 # Results:
 #
 proc DeleteSectionNumberCtrlPoints { Index MainConstruction } {
   variable CurrentNode
   variable tmpCurrentNode

   switch $MainConstruction {
      0 {
          set tmpCurrentNode [ NTree::GetLastSon $CurrentNode ]
          # Begin C Commands
          $CurrentNode DeleteIndexCtrlPoint $Index
          # End C Commands
          DeleteCtrlGraphicNTree $tmpCurrentNode
        }
      1 {
          set tmpCurrentNode [ NTree::GetLastSon $CurrentNode ]
          set tmpCurrentNode [ NTree::GetPreviousBrother $tmpCurrentNode ]
          # Begin C Commands
          $CurrentNode DeleteIndexCtrlPoint $Index
          # End C Commands
          DeleteCtrlGraphicNTree $tmpCurrentNode
        }
     }
  }

 # ChangePrimaryMotionType procedure.
 #
 #		This procedure changes the Motion Type value for a
 #		Primary Transformation.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangePrimaryMotionType { NTree Option} {

   variable LstTransMotionTypes
   variable LstSplinesTypes
   variable LstCtrlVarsConstVel
   variable ScaleTimeMaximum
   variable ScaleTimeFactor
   variable LstCtrlVarsVar
   variable LstCtrlVarsBez

   set ListOfElements [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]

   switch $Option {
      0 {
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsConstVel 2 ] $LstCtrlVarsConstVel
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
        }
      1 {
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
        }
      2 {
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsBez 0 ] $LstCtrlVarsBez
        }
   }
 }

 # ChangePrimaryMotionCtrlVar procedure --
 #
 #		This procedure changes the control variable input value for a
 #		Primary Transformation.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangePrimaryMotionCtrlVar { NTree PrimaryCtrlVar } {

    variable LstCtrlVarsConstVel
    variable LstCtrlVarsConstAccel
    variable LstCtrlVarsTrap
    variable LstCtrlVarsVar
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor
    variable ScaleOpenTpMinimum
    variable ScaleOpenTpMaximum
    variable ScaleOpenTpFactor
    variable ScaleClosedTpMinimum
    variable ScaleClosedTpMaximum
    variable ScaleClosedTpFactor

    set ListOfElements    [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]
    set ListOfElementsOTp [ list $ScaleOpenTpMinimum $ScaleOpenTpMaximum $ScaleOpenTpFactor ]
    set ListOfElementsCTp [ list $ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor ]

    switch $PrimaryCtrlVar {
      0 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsConstVel 2 ] $LstCtrlVarsConstVel
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput2" "None" 0 "$ListOfElements"
        }
      1 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsConstAccel 2 ] $LstCtrlVarsConstAccel
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput2" "None" 0 "$ListOfElements"
        }
      2 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsTrap 2 ] $LstCtrlVarsTrap
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput2" "None" 0 "$ListOfElementsOTp"
        }
      3 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsTrap 2 ] $LstCtrlVarsTrap
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput2" "None" 0 "$ListOfElementsOTp"
        }
      4 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsTrap 2 ] $LstCtrlVarsTrap
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput2" "None" 0 "$ListOfElementsCTp"
        }
      5 {
         set ListOfElements [ list $ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor ]
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 1 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput2" "None" 0 "$ListOfElements"
        }
     }
  }

 # ChangeSecondaryMotionType procedure --
 #
 #		This procedure changes the Motion Type value for a
 #		Secondary Transformation.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSecondaryMotionType { NTree Option} {

   variable LstTransMotionTypes
   variable LstSplinesTypes
   variable LstCtrlVarsConstVel
   variable ScaleTimeMaximum
   variable ScaleTimeFactor
   variable LstCtrlVarsVar
   variable LstSecondCtrlVarsBez

   set ListOfElements [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]

   switch $Option {
      0 {
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsConstVel 2 ] $LstCtrlVarsConstVel
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
        }
      1 {
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
        }
      2 {
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsBez 0 ] $LstSecondCtrlVarsBez
        }
   }
 }

 # ChangeSecondaryMotionCtrlVar procedure --
 #
 #		This procedure changes the control variable input value for a Secondary
 #		transformation.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSecondaryMotionCtrlVar { NTree SecondaryCtrlVar } {

    variable LstSecondCtrlVarsConstVel
    variable LstSecondCtrlVarsConstAccel
    variable LstSecondCtrlVarsTrap
    variable LstSecondCtrlVarsVar
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor
    variable ScaleOpenTpMinimum
    variable ScaleOpenTpMaximum
    variable ScaleOpenTpFactor
    variable ScaleClosedTpMinimum
    variable ScaleClosedTpMaximum
    variable ScaleClosedTpFactor

    set ListOfElements [ list 0 $ScaleTimeMaximum $ScaleTimeFactor ]
    set ListOfElementsOTp [ list $ScaleOpenTpMinimum $ScaleOpenTpMaximum $ScaleOpenTpFactor ]
    set ListOfElementsCTp [ list $ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor ]

    switch $SecondaryCtrlVar {
      0 {
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsConstVel 0 ] $LstSecondCtrlVarsConstVel
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput2" "None" 0 "$ListOfElements"
        }
      1 {
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsConstAccel 0 ] $LstSecondCtrlVarsConstAccel
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput2" "None" 0 "$ListOfElements"
        }
      2 {
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsTrap 0 ] $LstSecondCtrlVarsTrap
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput2" "None" 0 "$ListOfElementsOTp"
        }
      3 {
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsTrap 0 ] $LstSecondCtrlVarsTrap
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput2" "None" 0 "$ListOfElementsOTp"
        }
      4 {
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsTrap 0 ] $LstSecondCtrlVarsTrap
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput2" "None" 0 "$ListOfElementsCTp"
        }
      5 {
         set ListOfElements [ list $ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor ]
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "None" 0 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput2" "None" 0 "$ListOfElements"
       }
    }
  }

  # ChangeInterpolationType procedure.
 #
 #		This procedure sets the Interpolation Type for a primary or secondary
 #		transformation.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeInterpolationType { NTree InterpolationType SectionCtrlPoints} {

    variable CurrentNode
	variable tplON
    variable cvsON
    variable frmONTB
    variable LstSplinesTypes
    variable LstTransMotionTypes
    variable LstRotMotionTypes
    variable LstCtrlVarsVar
    variable LstCtrlVarsBez
    variable LstSecondCtrlVarsBez
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor

    set ListOfElements [ list $ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor ]

    switch $InterpolationType {
      1 {
         #Primary Straight Line.
          PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 2 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes

          for { set i 0 } { $i < 2 } { incr i } {
              set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
              AddGraphicNTree $TemporaryNTreeA
              # Begin C Commands
              $CurrentNode AddCtrlPoint $TemporaryNTreeA
              # End C Commands
          }
        }
      2 {
         #Primary Arch.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes

         for { set i 0 } { $i < 3 } { incr i } {
             set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
             AddGraphicNTree $TemporaryNTreeA
             # Begin C Commands
             $CurrentNode AddCtrlPoint $TemporaryNTreeA
             # End C Commands
         }
        }
      3 {
         #Primary Circle.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes

         for { set i 0 } { $i < 3 } { incr i } {
             set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
             AddGraphicNTree $TemporaryNTreeA
             # Begin C Commands
             $CurrentNode AddCtrlPoint $TemporaryNTreeA
             # End C Commands
         }
        }
      4 {
         #Primary Natural Cubic Spline
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "Control Points" 3 "$ListOfElements"
		 ToolBars::EnableButton $ObjectNavigator::frmONTB Create

         for { set i 0 } { $i < 3 } { incr i } {
             set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
             AddGraphicNTree $TemporaryNTreeA
             # Begin C Commands
             $CurrentNode AddCtrlPoint $TemporaryNTreeA
             # End C Commands
         }
        }
      5 {
         #Primary Cubic Spline
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "Control Points" 3 "$ListOfElements"

         for { set i 0 } { $i < 3 } { incr i } {
             set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
             AddGraphicNTree $TemporaryNTreeA
             # Begin C Commands
             $CurrentNode AddCtrlPoint $TemporaryNTreeA
             # End C Commands
         }
        }
      6 {
         #Primary Fourth Degree Spline.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryInput1" "Control Points" 3 "$ListOfElements"

        for { set i 0 } { $i < 3 } { incr i } {
              set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
              AddGraphicNTree $TemporaryNTreeA
              # Begin C Commands
              $CurrentNode AddCtrlPoint $TemporaryNTreeA
              # End C Commands
         }
        }
      7 {
         #Primary Bèzier Spline
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 4 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsBez 0 ] $LstCtrlVarsBez

         for { set i 0 } { $i < 4 } { incr i } {
             set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
             AddGraphicNTree $TemporaryNTreeA
             # Begin C Commands
             $CurrentNode AddCtrlPoint $TemporaryNTreeA
             # End C Commands
         }
        }
      8 {
         #Primary Interpolated
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 2 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstRotMotionTypes 5 ] $LstRotMotionTypes

         for { set i 0 } { $i < 2 } { incr i } {
            set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
            AddGraphicNTree $TemporaryNTreeA
            # Begin C Commands
            $CurrentNode AddCtrlPoint $TemporaryNTreeA
            # End C Commands
         }
        }
      9 {
         #Primary Arbitrary Axes
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 2 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryMotionType" "Motion Type" [ lindex $LstRotMotionTypes 5 ] $LstRotMotionTypes

         for { set i 0 } { $i < 2 } { incr i } {
           set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
           AddGraphicNTree $TemporaryNTreeA
           # Begin C Commands
           $CurrentNode AddCtrlPoint $TemporaryNTreeA
           # End C Commands
         }
        }
     10 {
         #Secondary Straight Line.
          PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 2 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes
        }
     11 {
         #Secondary Arch.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes

         set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
         InsertGraphicNTree $TemporaryNTreeA
         # Begin C Commands
         $CurrentNode InsertCtrlPoint $TemporaryNTreeA
         # End C Commands
        }
     12 {
         #Secondary Circle.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 3 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstTransMotionTypes 5 ] $LstTransMotionTypes

            set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
            InsertGraphicNTree $TemporaryNTreeA
            # Begin C Commands
            $CurrentNode InsertCtrlPoint $TemporaryNTreeA
            # End C Commands
        }
     13 {
         #Secondary Natural Cubic Spline
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" $SectionCtrlPoints "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "Control Points" $SectionCtrlPoints "$ListOfElements"

            set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
            InsertGraphicNTree $TemporaryNTreeA
            # Begin C Commands
            $CurrentNode InsertCtrlPoint $TemporaryNTreeA
            # End C Commands
        }
     14 {
         #Secondary Cubic Spline.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" $SectionCtrlPoints "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "Control Points" $SectionCtrlPoints "$ListOfElements"

            set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
            InsertGraphicNTree $TemporaryNTreeA
            # Begin C Commands
            $CurrentNode InsertCtrlPoint $TemporaryNTreeA
            # End C Commands
        }
     15 {
         #Secondary Fourth Degree Spline.
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" $SectionCtrlPoints "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryInput1" "Control Points" $SectionCtrlPoints "$ListOfElements"

            set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
            InsertGraphicNTree $TemporaryNTreeA
            # Begin C Commands
            $CurrentNode InsertCtrlPoint $TemporaryNTreeA
            # End C Commands
        }
     16 {
         #Secondary Bèzier Spline
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" $SectionCtrlPoints "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryCtrlVar" "Control Variable" [ lindex $LstSecondCtrlVarsBez 0 ] $LstSecondCtrlVarsBez
        }
     17 {
         #Secondary Interpolated
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 2 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstRotMotionTypes 5 ] $LstRotMotionTypes
        }
     18 {
         #Secondary Arbitrary Axes
         PropertyPalette::ChangePropertyTitleValue $NTree "SectionCtrlPoints" "Number of Control Points" 2 "$ListOfElements"
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryMotionType" "Motion Type" [ lindex $LstRotMotionTypes 5 ] $LstRotMotionTypes
        }
   19 {
            #Serpenoid
            CreateSerpenoidInputData 2 2 0 0 0 0 0 0
            MessagesWindow::Puts "[ NTree::GetNumberSons $ObjectNavigator::CurrentNode ]\n"
            if { [ NTree::GetNumberSons $ObjectNavigator::CurrentNode ] == 0} {
               for { set i 0 } { $i < 2 } { incr i } {
                  set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
                  AddGraphicNTree $TemporaryNTreeA
                  # Begin C Commands
                  $CurrentNode AddCtrlPoint $TemporaryNTreeA
                  # End C Commands            
               }
            }      
         
         }
     }
  }

 # EnableCreateButton procedure --
 #
 #		This procedure Enables/Disables the Create Button of the Object Navigator ToolBar
 #		for a primary or secondary transformation.
 #
 # Arguments:
 #
 # Results:
 #
 proc EnableCreateButton { InterpolationType } {

    variable frmONTB

    switch $InterpolationType {
      1 {
         #Primary Straight Line.
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
      2 {
         #Primary Arch.
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
      3 {
         #Primary Circle.
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
      4 {
         #Primary Natural Cubic Spline
         ToolBars::EnableButton $ObjectNavigator::frmONTB Create
        }
      5 {
         #Primary Cubic Spline
         ToolBars::EnableButton $ObjectNavigator::frmONTB Create
        }
      6 {
         #Primary Fourth Degree Spline.
         ToolBars::EnableButton $ObjectNavigator::frmONTB Create
        }
      7 {
         #Primary Bèzier Spline
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
      8 {
         #Primary Interpolated
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
      9 {
         #Primary Arbitrary Axes
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     10 {
         #Secondary Straight Line.
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     11 {
         #Secondary Arch.
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     12 {
         #Secondary Circle.
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     13 {
         #Secondary Natural Cubic Spline
         ToolBars::EnableButton $ObjectNavigator::frmONTB Create
        }
     14 {
         #Secondary Cubic Spline.
         ToolBars::EnableButton $ObjectNavigator::frmONTB Create
        }
     15 {
         #Secondary Fourth Degree Spline.
         ToolBars::EnableButton $ObjectNavigator::frmONTB Create
        }
     16 {
         #Secondary Bèzier Spline
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     17 {
         #Secondary Interpolated
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     18 {
         #Secondary Arbitrary Axes
         ToolBars::DisableButton $ObjectNavigator::frmONTB Create
        }
     }
  }
# ChangeSpaceOrderType procedure --
 #
 #		This procedure sets the order of transformations for a cartesian section.
 #		 0. Translation/Rotation.
 #		 1. Rotation/Translation.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSpaceOrderType { NTree OrderType } {
    variable LstOrderTypes
    variable LstTjcRotTypes
    variable LstTjcTransTypes
    variable TransInput
    variable RotInput
    switch $OrderType {
      0 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryOrderType"   "Primary Transformation" [ lindex $LstOrderTypes $OrderType ] $LstOrderTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryOrderType" "Secondary Transformation" [ lindex $LstOrderTypes 1 ] $LstOrderTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryIntType" "Interpolation Type" [ lindex $LstTjcTransTypes 0] $LstTjcTransTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryIntType" "Interpolation Type" [ lindex $LstTjcRotTypes 0] $LstTjcRotTypes
        }
      1 {
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryOrderType"   "Primary Transformation" [ lindex $LstOrderTypes $OrderType ] $LstOrderTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryOrderType" "Secondary Transformation" [ lindex $LstOrderTypes 0 ] $LstOrderTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "PrimaryIntType" "Interpolation Type" [ lindex $LstTjcRotTypes 0] $LstTjcRotTypes
         PropertyPalette::ChangePropertyTitleValue $NTree "SecondaryIntType" "Interpolation Type" [ lindex $LstTjcTransTypes 0] $LstTjcTransTypes
        }
    }
  }

  # ChangePrimaryCtrlVarInputsNTree procedure --
 #
 #		This procedure changes the Primary Ctrl Var Inputs
 # 			0. None.
 # 			1. Constant Velocity (Time).
 # 			2. Constant Velocity (Velocity).
 # 			3. Constant Acceleration (Time and Vo).
 # 			4. Constant Acceleration (Vo and Vf).
 # 			5. Trapezoidal (Time and Tp).
 # 			6. Trapezoidal (Velocity and Tp).
 # 			7. Variable (Points).
 # 			8. Variable (Points) Disabled.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangePrimaryCtrlVarInputsNTree { InputChooser PrimaryCtrlVar PrimaryInput1 PrimaryInput2 } {
    variable cvsON
    variable frmONTB
    # Take care with IOIC because this choise is based on the current node selecction
    if { $InputChooser==1 } {
       set InputTypeNTree $PropertyPalette::CurrentNode
       set InputTypeNTree [ NTree::GetFirstSon $InputTypeNTree ]
       set InputTypeNTree [ NTree::GetFirstSon $InputTypeNTree ]
    } else {
       set InputTypeNTree $PropertyPalette::CurrentNode
    }
    set Parent [ GetPropertyParent $InputTypeNTree ]

    # Collapse NTree to be changed.
    if { $InputChooser==1 } {
       RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
    } else {
       RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
    }
    # Delete old sons.
    set TemporaryNTree [ NTree::GetFirstSon $InputTypeNTree ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      NTree::DisAssociate $TemporaryNTree
      NTree::Destroy $TemporaryNTree
      set TemporaryNTree [ NTree::GetFirstSon $InputTypeNTree ]
    }
    # Create new NTree.
    PrimaryCtrlVarInputsNTree $Parent $InputTypeNTree $PrimaryCtrlVar $PrimaryInput1 $PrimaryInput2

    # Expand new NTree.
    if { $InputChooser==1 } {
       RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
    } else {
       RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
    }
  }

 # PrimaryCtrlVarInputsNTree procedure --
 #
 #		This procedure draws the Primary Control Var Inputs NTree according to the value
 #		passed by the ChangePrimaryCtrlVarInputsNTree procedure.
 #
 # Arguments:
 #
 # Results:
 #
 proc PrimaryCtrlVarInputsNTree { Parent InputTypeNTree PrimaryCtrlVar PrimaryInput1 PrimaryInput2 } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor
    variable ScaleOpenTpMinimum
    variable ScaleOpenTpMaximum
    variable ScaleOpenTpFactor
    variable ScaleClosedTpMinimum
    variable ScaleClosedTpMaximum
    variable ScaleClosedTpFactor
    variable CurrentNode
    variable GlobalPrimaryMotionType

    switch $PrimaryCtrlVar {
    0 {
       #None
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleDistanceMaximum 1" ] "None" ]
       set TemporaryNTreeG [ TkNTree::Set "None = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    1 {
       #Constant Velocity (Time)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
       set TemporaryNTreeG [ TkNTree::Set "Time = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    2 {
       #Constant Velocity (Velocity)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Velocity" ]
       set TemporaryNTreeG [ TkNTree::Set "Velocity = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    3 {
       #Constant Acceleration (Time and Vo)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
       set TemporaryNTreeG [ TkNTree::Set "Time = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
       # Primary Input 2
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput2" "Scale" [ list $PrimaryInput2 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Vo" ]
       set TemporaryNTreeG [ TkNTree::Set "Vo = $PrimaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    4 {
       #Constant Acceleration (Vo and Vf)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Vo" ]
       set TemporaryNTreeG [ TkNTree::Set "Vo = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
       # Primary Input 2
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput2" "Scale" [ list $PrimaryInput2 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Vf" ]
       set TemporaryNTreeG [ TkNTree::Set "Vf = $PrimaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    5 {
       #Trapezoidal (Time and Tp)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
       set TemporaryNTreeG [ TkNTree::Set "Time = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG

       # Primary Input 2
       if { $GlobalPrimaryMotionType == 4 } {
          set PropertiesNTreeData [ list "$Parent" "PrimaryInput2" "Scale" [ list $PrimaryInput2 "$ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor" ] "Tp" ]
          set TemporaryNTreeG [ TkNTree::Set "Tp = $PrimaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       } else {
          set PropertiesNTreeData [ list "$Parent" "PrimaryInput2" "Scale" [ list $PrimaryInput2 "$ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor" ] "Tp" ]
          set TemporaryNTreeG [ TkNTree::Set "Tp = $PrimaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       }
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    6 {
       #Trapezoidal (Velocity and Tp)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Velocity" ]
       set TemporaryNTreeG [ TkNTree::Set "Velocity = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
       # Primary Input 2
       if { $GlobalPrimaryMotionType == 4 } {
          set PropertiesNTreeData [ list "$Parent" "PrimaryInput2" "Scale" [ list $PrimaryInput2 "$ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor" ] "Tp" ]
          set TemporaryNTreeG [ TkNTree::Set "Tp = $PrimaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       } else {
          set PropertiesNTreeData [ list "$Parent" "PrimaryInput2" "Scale" [ list $PrimaryInput2 "$ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor" ] "Tp" ]
          set TemporaryNTreeG [ TkNTree::Set "Tp = $PrimaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       }
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    7 {
       #Variable (Points)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "$ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor" ] "Control Points" ]
       set TemporaryNTreeG [ TkNTree::Set "Control Points = $PrimaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    8 {
       #Variable (Points) (Disabled)
       # Primary Input 1
       set PropertiesNTreeData [ list "$Parent" "PrimaryInput1" "Scale" [ list $PrimaryInput1 "$ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor" ] "Control Points" ]
       set TemporaryNTreeG [ TkNTree::Set "Control Points = $PrimaryInput1" StateNone 21 False False False $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
   }
 }

 # ChangeSecondaryCtrlVarInputsNTree procedure --
 #
 # 		This procedure changes the Secondary Ctrl Var Inputs NTree
 # 			0. None.
 # 			1. Constant Velocity (Time).
 # 			2. Constant Acceleration (Time and Vo).
 # 			3. Trapezoidal (Time and Tp).
 # 			4. Variable (Points)
 # 			5. Variable (Points) Disabled
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSecondaryCtrlVarInputsNTree { InputChooser SecondaryCtrlVar SecondaryInput1 SecondaryInput2 } {
    variable cvsON
    variable frmONTB

    # Take care with IOIC because this choise is based on the current node selecction

    set InputTypeNTree $PropertyPalette::CurrentNode

    if { $InputChooser==1 } {
       set InputTypeNTree [ NTree::GetFirstSon $InputTypeNTree ]
    }
    if { $InputChooser==2 } {
       set InputTypeNTree [ NTree::GetFirstSon $InputTypeNTree ]
       set InputTypeNTree [ NTree::GetFirstSon $InputTypeNTree ]
    }

    set Parent [ GetPropertyParent $InputTypeNTree ]

    # Collapse NTree to be changed.
    RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode

    # Delete old sons
    set TemporaryNTree [ NTree::GetFirstSon $InputTypeNTree ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      NTree::DisAssociate $TemporaryNTree
      NTree::Destroy $TemporaryNTree
      set TemporaryNTree [ NTree::GetFirstSon $InputTypeNTree ]
    }
    # New Properties
    SecondaryCtrlVarInputsNTree $Parent $InputTypeNTree $SecondaryCtrlVar $SecondaryInput1 $SecondaryInput2

    # Expand
    RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
  }

 # SecondaryCtrlVarInputsNTree procedure --
 #
 #		This procedure draws the Secondary Ctrl Var Inputs NTree according to the value
 # 		passed by the ChangeSecondaryCtrlVarInputsNTree procedure.
 #
 # Arguments:
 #
 # Results:
 #
 proc SecondaryCtrlVarInputsNTree { Parent InputTypeNTree SecondaryCtrlVar SecondaryInput1 SecondaryInput2 } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor
    variable ScaleOpenTpMinimum
    variable ScaleOpenTpMaximum
    variable ScaleOpenTpFactor
    variable ScaleClosedTpMinimum
    variable ScaleClosedTpMaximum
    variable ScaleClosedTpFactor
    variable GlobalSecondaryMotionType

    switch $SecondaryCtrlVar {
    0 {
       # None
       # Secondary Input 1
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput1" "Scale" [ list $SecondaryInput1 "0 $ScaleDistanceMaximum 1" ] "None" ]
       set TemporaryNTreeG [ TkNTree::Set "None = $SecondaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    1 {
       # Constant Velocity (Time)
       # Secondary Input 1
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput1" "Scale" [ list $SecondaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
       set TemporaryNTreeG [ TkNTree::Set "Time = $SecondaryInput1" StateNone 21 False False False $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    2 {
       # Constant Acceleration (Time and Vo)
       # Secondary Input 1
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput1" "Scale" [ list $SecondaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
       set TemporaryNTreeG [ TkNTree::Set "Time = $SecondaryInput1" StateNone 21 False False False $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG

       # Secondary Input 2
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput2" "Scale" [ list $SecondaryInput2 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Vo" ]
       set TemporaryNTreeG [ TkNTree::Set "Vo = $SecondaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    3 {
       # Trapezoidal (Time and Tp)
       # Secondary Input 1
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput1" "Scale" [ list $SecondaryInput1 "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "Time" ]
       set TemporaryNTreeG [ TkNTree::Set "Time = $SecondaryInput1" StateNone 21 False False False $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG

       # Secondary Input 2
       if { $GlobalSecondaryMotionType==4 } {
          set PropertiesNTreeData [ list "$Parent" "SecondaryInput2" "Scale" [ list $SecondaryInput2 "$ScaleClosedTpMinimum $ScaleClosedTpMaximum $ScaleClosedTpFactor" ] "Tp" ]
          set TemporaryNTreeG [ TkNTree::Set "Tp = $SecondaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       } else {
          set PropertiesNTreeData [ list "$Parent" "SecondaryInput2" "Scale" [ list $SecondaryInput2 "$ScaleOpenTpMinimum $ScaleOpenTpMaximum $ScaleOpenTpFactor" ] "Tp" ]
          set TemporaryNTreeG [ TkNTree::Set "Tp = $SecondaryInput2" StateNone 21 False False True $PropertiesNTreeData ]
       }
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    4 {
       # Variable (Points)
       # Secondary Input 1
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput1" "Scale" [ list $SecondaryInput1 "$ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor" ] "Control Points" ]
       set TemporaryNTreeG [ TkNTree::Set "Control Points = $SecondaryInput1" StateNone 21 False False True $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
    5 {
       # Variable (Points) Disabled.
       # Secondary Input 1
       set PropertiesNTreeData [ list "$Parent" "SecondaryInput1" "Scale" [ list $SecondaryInput1 "$ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor" ] "Control Points" ]
       set TemporaryNTreeG [ TkNTree::Set "Control Points = $SecondaryInput1" StateNone 21 False False False $PropertiesNTreeData ]
       NTree::Add $InputTypeNTree $TemporaryNTreeG
      }
   }
 }

  # ChangeSpaceTypePropertiesNTree procedure --
 #
 # 		This procedure changes the Space Type properties Ntree
 # 			0. Cartesian space type.
 # 			1. Joint space type.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeSpaceTypePropertiesNTree { SpaceType PrimaryOrderType SecondaryOrderType PrimaryIntType SecondaryIntType PrimaryMotionType SecondaryMotionType PrimaryCtrlVar SecondaryCtrlVar PrimaryInput1 PrimaryInput2 SecondaryInput1 SecondaryInput2 } {
    variable cvsON
    variable frmONTB
    # Take care with IOIC because this choise is based on the current node selecction
    set SpaceTypeNTree $PropertyPalette::CurrentNode
    set Parent [ GetPropertyParent $SpaceTypeNTree ]

    # Collapse NTree to be changed.
    RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode

    # Delete old sons.
    set TemporaryNTree [ NTree::GetFirstSon $SpaceTypeNTree ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      NTree::DisAssociate $TemporaryNTree
      NTree::Destroy $TemporaryNTree
      set TemporaryNTree [ NTree::GetFirstSon $SpaceTypeNTree ]
    }

    # Create new Properties NTree.
    SpaceTypePropertiesNTree $Parent $SpaceTypeNTree $SpaceType $PrimaryOrderType $SecondaryOrderType $PrimaryIntType $SecondaryIntType $PrimaryMotionType $SecondaryMotionType $PrimaryCtrlVar $SecondaryCtrlVar $PrimaryInput1 $PrimaryInput2 $SecondaryInput1 $SecondaryInput2

    # Expand new NTree.
    RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
 }

 # SpaceTypePropertiesNTree procedure --
 #
 # 		This procedure draws the SpaceType Properties NTree according to the value
 # 		passed by the NTree ChangeSpaceTypePropertiesNTree procedure.
 #
 # Arguments:
 #
 # Results:
 #
 proc SpaceTypePropertiesNTree { Parent SpaceTypeNTree SpaceType PrimaryOrderType SecondaryOrderType PrimaryIntType SecondaryIntType PrimaryMotionType SecondaryMotionType PrimaryCtrlVar SecondaryCtrlVar PrimaryInput1 PrimaryInput2 SecondaryInput1 SecondaryInput2 } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable LstOrderTypes
    variable LstTjcTransTypes
    variable LstTjcRotTypes
    variable LstTransMotionTypes
    variable LstSplinesTypes
    variable LstRotMotionTypes
    variable LstCtrlVarsConstVel
    variable LstCtrlVarsConstAccel
    variable LstCtrlVarsTrap
    variable LstCtrlVarsVar
    variable LstCtrlVarsBez
    variable LstSecondCtrlVarsConstVel
    variable LstSecondCtrlVarsConstAccel
    variable LstSecondCtrlVarsTrap
    variable LstSecondCtrlVarsBez
    switch $SpaceType {
      0 {
          # Cartesian.
          # Primary Transformation (Translation / Rotation)
          set PropertiesNTreeData [ list "$Parent" "PrimaryOrderType" "List" [ list [ lindex $LstOrderTypes $PrimaryOrderType ] $LstOrderTypes ] "Primary Transformation" ]
          set TemporaryNTreeC [ TkNTree::Set "Primary Transformation = [ lindex $LstOrderTypes $PrimaryOrderType ]" StateCollapse 22 False True True $PropertiesNTreeData ]
          NTree::Add $SpaceTypeNTree  $TemporaryNTreeC
          # Primary Interpolation Type
           switch $PrimaryOrderType {
              0 {
                  set PropertiesNTreeData [ list "$Parent" "PrimaryIntType" "List" [ list [ lindex $LstTjcTransTypes $PrimaryIntType ] $LstTjcTransTypes ] "Interpolation Type" ]
			set TemporaryNTreeD [ TkNTree::Set "Interpolation Type = [ lindex $LstTjcTransTypes $PrimaryIntType ]" StateExpand 21 False False True $PropertiesNTreeData ]
                }
              1 {
                  set PropertiesNTreeData [ list "$Parent" "PrimaryIntType" "List" [ list [ lindex $LstTjcRotTypes $PrimaryIntType ] $LstTjcRotTypes ] "Interpolation Type" ]
                  set TemporaryNTreeD [ TkNTree::Set "Interpolation Type = [ lindex $LstTjcRotTypes $PrimaryIntType ]" StateExpand 21 False False True $PropertiesNTreeData ]
                }
              2 {
                  set PropertiesNTreeData [ list "$Parent" "PrimaryIntType" "List" [ list [ lindex $LstTjcTransTypes $PrimaryIntType ] $LstTjcTransTypes ] "Interpolation Type" ]
                  set TemporaryNTreeD [ TkNTree::Set "Interpolation Type = [ lindex $LstTjcTransTypes $PrimaryIntType ]" StateCollapse 21 False False True $PropertiesNTreeData ]
                }
            }
            NTree::Add $TemporaryNTreeC $TemporaryNTreeD
            # Primary Motion Type
            if { ( $PrimaryIntType==0 ) || ( $PrimaryIntType== 1 ) || ( $PrimaryIntType== 2 ) || ( $PrimaryIntType== 3 ) } {
                set PropertiesNTreeData [ list "$Parent" "PrimaryMotionType" "List" [ list [ lindex $LstTransMotionTypes $PrimaryMotionType ] $LstTransMotionTypes ] "Motion Type" ]
                set TemporaryNTreeE [ TkNTree::Set "Motion Type = [ lindex $LstTransMotionTypes $PrimaryMotionType ]" StateExpand 22 False True True $PropertiesNTreeData ]
            } else {
                set PropertiesNTreeData [ list "$Parent" "PrimaryMotionType" "List" [ list [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes ] "Motion Type" ]
                set TemporaryNTreeE [ TkNTree::Set "Motion Type = [ lindex $LstSplinesTypes 0 ]" StateExpand 22 False True True $PropertiesNTreeData ]
            }
            NTree::Add $TemporaryNTreeD $TemporaryNTreeE
               # Primary Control Variable
               switch $PrimaryMotionType {
                  0 {
                      set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstVel $PrimaryCtrlVar ] $LstCtrlVarsConstVel ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstVel $PrimaryCtrlVar]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      if { $PrimaryCtrlVar==0 } {
                        set tmpPrimaryCtrlVar 1
                      }
                      if { $PrimaryCtrlVar==1 } {
                        set tmpPrimaryCtrlVar 2
                      }
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                    }
                  1 {
                      if { $PrimaryCtrlVar==2 } {
                         set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstAccel 0 ] $LstCtrlVarsConstAccel ] "Control Variable" ]
                         set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstAccel 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                         set tmpPrimaryCtrlVar 3
                      }
                      if { $PrimaryCtrlVar==3 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstAccel 1 ] $LstCtrlVarsConstAccel ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstAccel 1]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 4
                      }
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                    }
                  2 {
                      if { $PrimaryCtrlVar==4 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 0 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 5
                      }
                      if { $PrimaryCtrlVar==5 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 1 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 6
                      }
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                    }
                  3 {
                      if { $PrimaryCtrlVar==4 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 0 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 5
                      }
                      if { $PrimaryCtrlVar==5 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 1 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 6
                      }
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                    }
                  4 {
                      if { $PrimaryCtrlVar==4 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 0 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 5
                      }
                      if { $PrimaryCtrlVar==5 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 1 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 6
                      }
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                    }
                  5 {
                      if { $PrimaryIntType==7} {
                         if { $PrimaryCtrlVar==6 } {
                           set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsBez 0 ] $LstCtrlVarsBez ] "Control Variable" ]
                           set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsBez 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                           set tmpPrimaryCtrlVar 8
                         }
                         if { $PrimaryCtrlVar==7 } {
                           set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsBez 1 ] $LstCtrlVarsBez ] "Control Variable" ]
                           set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsBez 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                           set tmpPrimaryCtrlVar 8
                         }
                      } else {
                         set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar ] "Control Variable" ]
                         set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsVar 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                         if { $PrimaryCtrlVar==6 } {
                           set tmpPrimaryCtrlVar 7
                         }
                      }
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                    }
                  6 {
                      set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstVel 2 ] $LstCtrlVarsConstVel ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstVel 2 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set PrimaryCtrlVar 0
                    }
               }

              # Add number of inputs depending on Control Var
              PrimaryCtrlVarInputsNTree $Parent $TemporaryNTreeF $PrimaryCtrlVar $PrimaryInput1 $PrimaryInput2

          # Secondary Transformation (Translation / Rotation)
          set PropertiesNTreeData [ list "$Parent" "SecondaryOrderType" "List" [ list [ lindex $LstOrderTypes $SecondaryOrderType ] $LstOrderTypes ] "Secondary Transformation" ]
          set TemporaryNTreeC [ TkNTree::Set "Secondary Transformation = [ lindex $LstOrderTypes $SecondaryOrderType ]" StateCollapse 22 False False True $PropertiesNTreeData ]
          NTree::Add $SpaceTypeNTree $TemporaryNTreeC
            # Secondary Interpolation Type
            switch $PrimaryOrderType {
              0 {
                  set PropertiesNTreeData [ list "$Parent" "SecondaryIntType" "List" [ list [ lindex $LstTjcRotTypes $SecondaryIntType ] $LstTjcRotTypes ] "Interpolation Type" ]
                  set TemporaryNTreeD [ TkNTree::Set "Interpolation Type = [ lindex $LstTjcRotTypes $SecondaryIntType ]" StateExpand 21 False False True $PropertiesNTreeData ]
                }
              1 {
                  set PropertiesNTreeData [ list "$Parent" "SecondaryIntType" "List" [ list [ lindex $LstTjcTransTypes $SecondaryIntType ] $LstTjcTransTypes ] "Interpolation Type" ]
                  set TemporaryNTreeD [ TkNTree::Set "Interpolation Type = [ lindex $LstTjcTransTypes $SecondaryIntType ]" StateExpand 21 False False True $PropertiesNTreeData ]
                }
              2 {
                   set PropertiesNTreeData [ list "$Parent" "SecondaryIntType" "List" [ list [ lindex $LstTjcRotTypes $SecondaryIntType ] $LstTjcRotTypes ] "Interpolation Type" ]
                   set TemporaryNTreeD [ TkNTree::Set "Interpolation Type = [ lindex $LstTjcRotTypes $SecondaryIntType ]" StateExpand 21 False True True $PropertiesNTreeData ]
                }
            }
            NTree::Add $TemporaryNTreeC $TemporaryNTreeD
            # Secondary Motion Type
            if { ( $SecondaryIntType==0 ) || ( $SecondaryIntType== 1 ) || ( $SecondaryIntType== 2 ) } {
                set PropertiesNTreeData [ list "$Parent" "SecondaryMotionType" "List" [ list [ lindex $LstRotMotionTypes $SecondaryMotionType ] $LstRotMotionTypes ] "Motion Type" ]
                set TemporaryNTreeE [ TkNTree::Set "Motion Type = [ lindex $LstRotMotionTypes $SecondaryMotionType ]" StateExpand 22 False True True $PropertiesNTreeData ]
            } else {
                set PropertiesNTreeData [ list "$Parent" "SecondaryMotionType" "List" [ list [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes ] "Motion Type" ]
                set TemporaryNTreeE [ TkNTree::Set "Motion Type = [ lindex $LstSplinesTypes 0 ]" StateExpand 22 False True True $PropertiesNTreeData ]
            }
            NTree::Add $TemporaryNTreeD $TemporaryNTreeE
              # Secondary Control Variable
              switch $SecondaryMotionType {
                 0 {
                      set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstSecondCtrlVarsConstVel 0 ] $LstSecondCtrlVarsConstVel ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex $LstSecondCtrlVarsConstVel 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      if { $SecondaryCtrlVar==0 } {
                          set SecondaryCtrlVar 1
                      }
                   }
                 1 {
                      set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstSecondCtrlVarsConstAccel 0 ] $LstSecondCtrlVarsConstAccel ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex $LstSecondCtrlVarsConstAccel 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      if { $SecondaryCtrlVar==2 } {
                          set SecondaryCtrlVar 2
                      }
                   }
                 2 {
                      set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstSecondCtrlVarsTrap 0 ] $LstSecondCtrlVarsTrap ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex $LstSecondCtrlVarsTrap 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      if { $SecondaryCtrlVar==4 } {
                          set SecondaryCtrlVar 3
                      }
                   }
                 3 {
                      set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstSecondCtrlVarsTrap 0 ] $LstSecondCtrlVarsTrap ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex $LstSecondCtrlVarsTrap 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      if { $SecondaryCtrlVar==4 } {
                          set SecondaryCtrlVar 3
                      }
                   }
                 4 {
                      set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstSecondCtrlVarsTrap 0 ] $LstSecondCtrlVarsTrap ] "Control Variable" ]
                      set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex $LstSecondCtrlVarsTrap 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      if { $SecondaryCtrlVar==4 } {
                          set SecondaryCtrlVar 3
                      }
                   }
                 5 {
                      if { $SecondaryIntType==7} {
                         set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstSecondCtrlVarsBez 0 ] $LstSecondCtrlVarsBez ] "Control Variable" ]
                         set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstSecondCtrlVarsBez 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                         set tmpSecondaryCtrlVar 5
                      } else {
                         set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar ] "Control Variable" ]
                         set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsVar 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                         set tmpSecondaryCtrlVar 4
                      }
                      NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                      set SecondaryCtrlVar $tmpSecondaryCtrlVar
                    }

                 6 {
                    set PropertiesNTreeData [ list "$Parent" "SecondaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstVel 2 ] $LstCtrlVarsConstVel ] "Control Variable" ]
                    set TemporaryNTreeF [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstVel 2 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                    NTree::Add $TemporaryNTreeE $TemporaryNTreeF
                    set SecondaryCtrlVar 0
                   }

              }
              # Add number of inputs depending on Control Var
              SecondaryCtrlVarInputsNTree $Parent $TemporaryNTreeF $SecondaryCtrlVar $SecondaryInput1 $SecondaryInput2


        }
      1 {
          # Joint
          # Primary Transformation (Translation / Rotation)
          set PropertiesNTreeData [ list "$Parent" "PrimaryOrderType" "List" [ list [ lindex $LstOrderTypes $PrimaryOrderType ] $LstOrderTypes ] "Primary Transformation" ]
          set TemporaryNTreeC [ TkNTree::Set "Primary Transformation = [ lindex $LstOrderTypes $PrimaryOrderType ]" StateCollapse 22 False True True $PropertiesNTreeData ]
          NTree::Add $SpaceTypeNTree $TemporaryNTreeC
        }
    }
  }

  #	Section PropertyPallete NTree procedure --
 #
 # 		The following procedure is used to create a Trajectory's Section
 # 		Properties NTree.
 #
 # Arguments:
 #
 # Results:
 #
 proc SectionPropertiesNTree { Parent Name SpaceType PrimaryOrderType SecondaryOrderType PrimaryIntType PrimaryMotionType PrimaryCtrlVar SecondaryIntType SecondaryMotionType SecondaryCtrlVar SectionGenPoints SectionCtrlPoints PrimaryInput1 PrimaryInput2 SecondaryInput1 SecondaryInput2 } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
	variable ScalePointsMaximum 
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable LstSpaceTypes

    # Section Description
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 33 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsSectionPropertiesNTree "$Parent" "$Name" ]

    # SpaceType
    set PropertiesNTreeData [ list "$Parent" "SpaceType" "List" [ list [ lindex $LstSpaceTypes $SpaceType ] $LstSpaceTypes ] "Space Type" ]
    set TemporaryNTreeB [ TkNTree::Set "Space Type = [ lindex $LstSpaceTypes $SpaceType ]" StateExpand 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    SpaceTypePropertiesNTree $Parent $TemporaryNTreeB $SpaceType $PrimaryOrderType $SecondaryOrderType $PrimaryIntType $SecondaryIntType $PrimaryMotionType $SecondaryMotionType $PrimaryCtrlVar $SecondaryCtrlVar $PrimaryInput1 $PrimaryInput2 $SecondaryInput1 $SecondaryInput2

    # Section Number of Control Points
    set PropertiesNTreeData [ list "$Parent" "SectionCtrlPoints" "Scale" [ list $SectionCtrlPoints "0 $ScalePointsMaximum  1" ] "Number of Control Points" ]
    set TemporaryNTreeB [ TkNTree::Set "Number of Control Points = $SectionCtrlPoints" StateNone 21 False False False $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    # Section Number of Generated Points
    set PropertiesNTreeData [ list "$Parent" "SectionGenPoints" "Scale" [ list $SectionGenPoints "3 $ScalePointsMaximum  1" ] "Number of Generated Points" ]
    set TemporaryNTreeB [ TkNTree::Set "Number of Generated Points = $SectionGenPoints" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    return $TemporaryNTreeA
 }

 # TrajectorysPropertiesNTree procedure.
 #
 # 		The following procedures are used to create a Trajectory`s
 # 		Properties NTree.
 #
 # Arguments:
 #
 # Results:
 #
 proc TrajectorysPropertiesNTree { Parent Description UnionType TjcNumPoints TjcTotalTime vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleTimeMaximum
    variable ScaleTimeFactor
    variable ScalePointsMinimum
    variable ScalePointsMaximum
    variable ScalePointsFactor

    variable LstUnionTypes
    variable UnionTypeNTree

    #Description
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 33 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]

    # Number of Points
    set PropertiesNTreeData [ list "$Parent" "TjcNumPoints" "Scale" [ list $TjcNumPoints "$ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor" ] "TjcNumPoints" ]
    set TemporaryNTreeB [ TkNTree::Set "Number of Points = $TjcNumPoints" StateNone 21 False False False $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    # Total Time
    set PropertiesNTreeData [ list "$Parent" "TjcTotalTime" "Scale" [ list $TjcTotalTime "0 $ScaleTimeMaximum $ScaleTimeFactor" ] "TjcTotalTime" ]
    set TemporaryNTreeB [ TkNTree::Set "Total Time = $TjcTotalTime" StateNone 21 False False False $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    # PositionOrientation
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]

    # Visibility
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]

    # Position and Orientation vectors Visibility
    NTree::Add $TemporaryNTreeA [ GraphicsPOVsPropertiesNTree "$Parent" ]

    # Velocity Visibility
    NTree::Add $TemporaryNTreeA [ GraphicsVelocityPropertiesNTree "$Parent" ]

    # Accel Visibility
    NTree::Add $TemporaryNTreeA [ GraphicsAccelPropertiesNTree "$Parent" ]

    # Section Smoothing
    switch $UnionType {
        0 { set PropertiesNTreeData [ list "$Parent" "SectionSmoothing" "List" [ list [ lindex $LstUnionTypes $UnionType ] $LstUnionTypes ] "Section Smoothing Visible" ]
            set TemporaryNTreeB [ TkNTree::Set "Section Smoothing Visible = [ lindex $LstUnionTypes $UnionType ]" StateCollapse 22 True True True $PropertiesNTreeData ]
            NTree::Add $TemporaryNTreeA $TemporaryNTreeB
          }
        1 { set PropertiesNTreeData [ list "$Parent" "SectionSmoothing" "List" [ list [ lindex $LstUnionTypes $UnionType ] $LstUnionTypes ] "Section Smoothing Visible" ]
            set TemporaryNTreeB [ TkNTree::Set "Section Smoothing Visible = [ lindex $LstUnionTypes $UnionType ]" StateExpand 22 True True True $PropertiesNTreeData ]
            NTree::Add $TemporaryNTreeA $TemporaryNTreeB
          }
    }
    set UnionTypeNTree $TemporaryNTreeB
    return $TemporaryNTreeA
  }

