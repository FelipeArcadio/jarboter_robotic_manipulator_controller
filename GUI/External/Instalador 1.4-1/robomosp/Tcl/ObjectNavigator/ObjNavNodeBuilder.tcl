#----------------------------------------------------------------------------------------------------------------------------------
# ObjNavNodeBuilder.tcl
# 		This file implements the TCL code for creating the Object Navigator NTree Nodes
#		 for the ROBOMOSP application.
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

 #--------------------------------------------------------------------------------------------------------------
 # Nodes builders procedures --
 #
 # 		These procedures builds de data subtree for all the data types belonging
 # 		to the ROBOMOSP system.
 #--------------------------------------------------------------------------------------------------------------

 # LoadConfigurationsFiles --
 #
 #			This procedure loads the configuration files for any of the elements belonging
 #         to the Configurations NTree Node in the Object Navigator Window.
 #
 # Arguments:
 #
 # Results:
 #
 proc LoadConfigurationsFiles {} {
    variable ApplicationNTree
    variable cvsON
    variable frmONTB
    variable ConfigurationsNode
    variable CfgMaterialsNode
    # Materials
    SwapSelectedObject $cvsON $CfgMaterialsNode 0
    for { set i 0 } { $i < $RobLab::MaterialNumberFiles } { incr i } {
      TclCreateMaterial OpenCfgMaterial $i
    }
    SwapSelectedObject $cvsON $ConfigurationsNode 0
    RobLab::CollapseAllNTree $cvsON $frmONTB ObjectNavigator::CurrentNode
    SwapSelectedObject $cvsON $ApplicationNTree 1
  }

  # CreateConfigurationsNode --
  #
  #			This procedure creates the Configuration NTree Node and calls the creator procedures
  #            for any of the elements that belongs to the Configurations NTree Node in the Object Navigator
  #            Window(Lighting, Camera, Grid, Materials).
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateConfigurationsNode {} {
    variable ConfigurationsNode
    variable CfgMaterialsNode
    set PropertiesNTreeData [ list Configurations $NTree::Null "" ]
    set ConfigurationsNode [ TkNTree::Set "Configurations" StateExpand 30 False False Undefined $PropertiesNTreeData ]
    set PropertiesNTreeData [ list CfgMaterials $NTree::Null "" ]
    # Lighting
    TclCreateLighting Open $RobLab::CfgsFileLighting
    # Camera
    NTree::Add $ConfigurationsNode [ CreateCamerasNode ]
    # Grid
    NTree::Add $ConfigurationsNode [ CreateGridsNode ]
    # Metric Units
    NTree::Add $ConfigurationsNode [ CreateMetricUnitsNode ]
    # Materials
    set CfgMaterialsNode [ TkNTree::Set "Materials" StateNone 18 False False Undefined $PropertiesNTreeData ]
    NTree::Add $ConfigurationsNode $CfgMaterialsNode
    return $ConfigurationsNode
  }

  # CreateLightingsNode --
  #
  #			This procedure creates the Lightings NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateLightingsNode { { LSAmbientRed 0.1 } { LSAmbientGreen 0.1 } { LSAmbientBlue 0.1 } { LSAmbientAlpha 1.0 } { LSDiffuseRed 1.0 } { LSDiffuseGreen 1.0 } { LSDiffuseBlue 1.0 } { LSDiffuseAlpha 1.0 } { LSSpecularRed 0.5 } { LSSpecularGreen 0.5 } { LSSpecularBlue 0.5 } { LSSpecularAlpha 1.0 } { MAmbientRed 0.2 } { MAmbientGreen 0.2 } { MAmbientBlue 0.2 } { MAmbientAlpha 1.0 } } {
    set TemporaryNTreeA [ TkNTree::Set "Lighting" StateNone 17 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list LightingsTitle [ LightingsPropertiesNTree $TemporaryNTreeA $LSAmbientRed $LSAmbientGreen $LSAmbientBlue $LSAmbientAlpha $LSDiffuseRed $LSDiffuseGreen $LSDiffuseBlue $LSDiffuseAlpha $LSSpecularRed $LSSpecularGreen $LSSpecularBlue $LSSpecularAlpha $MAmbientRed $MAmbientGreen $MAmbientBlue $MAmbientAlpha ] $RobLab::CfgsFileLighting ]
    return $TemporaryNTreeA
  }

  # CreateCamerasNode --
  #
  #			This procedure creates the Cameras NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCamerasNode { { CenterX 0 } { CenterY 0 } { CenterZ 0 } { CenterVisible False } { Radius 0 } { Elevation 0 } { Azimuth 0 } { Twist 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "Camera" StateNone 19 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list Camera [ CamerasPropertiesNTree $TemporaryNTreeA $CenterX $CenterY $CenterZ $CenterVisible $Radius $Elevation $Azimuth $Twist ] "" ]
    # Begin C Commands !!!
    TclCreateCamera New $TemporaryNTreeA
    # End C Commands !!!
    set CamerasNTreeData $TemporaryNTreeA
    #MessagesWindow::Puts "CamerasNTree = $TemporaryNTreeA\n"
    #MessagesWindow::Puts "CamerasNTree = $CamerasNTreeData\n"
    return $TemporaryNTreeA
  }

  # CreateGridsNode --
  #
  #			This procedure creates the Grids NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateGridsNode { { Width 12 } { Depth 12 } { Height 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "Grid" StateNone 36 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list Grid [ GridsPropertiesNTree $TemporaryNTreeA $Width $Depth $Height ] "" ]
    # Begin C Commands !!!
    TclCreateGrid New $TemporaryNTreeA
    
    # End C Commands !!!
    set GridsNTreeData $TemporaryNTreeA
    #MessagesWindow::Puts "GridsNTree = $TemporaryNTreeA\n"
    #MessagesWindow::Puts "GridsNTree = $GridsNTreeData\n"
    
    return $TemporaryNTreeA
  }

  # CreateMetricUnitsNode --
  #
  #			This procedure creates the MetricUnits NTree Node in the Object Navigator Window.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateMetricUnitsNode { { LengthType  3 } { MassType  0  }  { TimeType  0  } { AngleType  0  }  { ForceType  0  }} {
    set TemporaryNTreeA [ TkNTree::Set "MetricUnits" StateNone 22 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list MetricUnits [ MetricUnitsPropertiesNTree $TemporaryNTreeA $LengthType $MassType $TimeType $AngleType $ForceType ] "" ]
    # Begin C Commands !!!
      TclCreateMetricUnits New $TemporaryNTreeA
    # End C Commands !!!
    return $TemporaryNTreeA
  }

  # CreateMaterialsNode --
  #
  #			This procedure creates the Materials NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateMaterialsNode { { FileName "" } { Description "" } { AmbientRed 0.2 } { AmbientGreen 0.2 } { AmbientBlue 0.2 } { AmbientAlpha 0.25 } { DiffuseRed 0.8 } { DiffuseGreen 0.8 } { DiffuseBlue 0.8 } { DiffuseAlpha 0.25 } { SpecularRed 0 } { SpecularGreen 0 } { SpecularBlue 0 } { SpecularAlpha 0.25 } { EmissionRed 0 } { EmissionGreen 0 } { EmissionBlue 0 } { EmissionAlpha 0.25 } { Shininess 0 } { Density 1 } { DensityType 0 }} {
    variable Id
    if { $FileName == "" } {
      set TemporaryNTreeA [ TkNTree::Set "Material$Id.mtr" StateNone 5 False True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    }
    TkNTree::SetData $TemporaryNTreeA [ list MaterialsTitle [ MaterialsPropertiesNTree $TemporaryNTreeA $Description $AmbientRed $AmbientGreen $AmbientBlue $AmbientAlpha $DiffuseRed $DiffuseGreen $DiffuseBlue $DiffuseAlpha $SpecularRed $SpecularGreen $SpecularBlue $SpecularAlpha $EmissionRed $EmissionGreen $EmissionBlue $EmissionAlpha $Shininess $Density  $DensityType ] $FileName ]
    return $TemporaryNTreeA
  }

  # ---------------------------------------------------------------------------------------
  # CSG NTree Nodes creators --
  #
  #			The following procedures create all of the CSG NTree Nodes.
  #
  # ---------------------------------------------------------------------------------------

  # CreateCSGNode --
  #
  #			This procedure creates the main CSG NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGNode { { FileName "" } { Description "" } } {
    variable Id
    if { $FileName == "" } {
      set TemporaryNTreeA [ TkNTree::Set "CSG$Id.csg" StateNone 5 False True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    }
    TkNTree::SetData $TemporaryNTreeA [ list CSGsTitle [ CSGPropertiesNTree $TemporaryNTreeA $Description ] $FileName ]
    return $TemporaryNTreeA
  }

  # CreateCSGOperatorUnionNode --
  #
  #			This procedure creates the CSG Union operator NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGOperatorUnionNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Union" StateNone 8 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsOperatorTitle [ CSGOperatorUnionPropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGOperatorIntersectionNode --
  #
  #			This procedure creates the CSG Intersection operator NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGOperatorIntersectionNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Intersection" StateNone 9 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsOperatorTitle [ CSGOperatorIntersectionPropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGOperatorDifferenceNode --
  #
  #			This procedure creates the CSG Difference operator NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGOperatorDifferenceNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Difference" StateNone 10 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsOperatorTitle [ CSGOperatorDifferencePropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGSolidSphereNode --
  #
  #			This procedure creates the CSG Solid Sphere NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGSolidSphereNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } { Radius 1} { Slices 12 } { Stacks 12 } { Material "./CfgsFiles/Default.mtr" } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Sphere" StateNone 11 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsSolidTitle [ CSGSolidSpherePropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $Radius $Slices $Stacks $Material ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGSolidCylinderNode --
  #
  #			This procedure creates the CSG Solid Cylinder NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGSolidCylinderNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } { BaseRadius 1 } { TopRadius 1 } { Heigth 1 } { Slices 12 } { Stacks 3 } { Material "./CfgsFiles/Default.mtr" } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Cylinder" StateNone 13 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsSolidTitle [ CSGSolidCylinderPropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $BaseRadius $TopRadius $Heigth $Slices $Stacks $Material ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGSolidCubeNode --
  #
  #			This procedure creates the CSG Solid Cube NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGSolidCubeNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } { BaseWidth 1 } { BaseDepth 1 } { TopWidth 1 } { TopDepth 1 } { Heigth 1 } { Material "./CfgsFiles/Default.mtr" } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Cube" StateNone 14 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsSolidTitle [ CSGSolidCubePropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $BaseWidth $BaseDepth $TopWidth $TopDepth $Heigth $Material ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGSolidPyramidNode --
  #
  #			This procedure creates the CSG Solid Pyramid NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGSolidPyramidNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } { Base 1 } { Heigth 1 } { Stacks 3 } { Material "./CfgsFiles/Default.mtr" } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Pyramid" StateNone 15 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsSolidTitle [ CSGSolidPyramidPropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $Base $Heigth $Stacks $Material ] "" ]
    return $TemporaryNTreeA
  }

  # CreateCSGSolidConeNode --
  #
  #			This procedure creates the CSG Solid Cone NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateCSGSolidConeNode { { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } { Base 1 } { Heigth 1 } { Slices 12 } { Stacks 3 } { Material "./CfgsFiles/Default.mtr" } } {
    variable CurrentNode
    set TemporaryNTreeA [ TkNTree::Set "Cone" StateNone 16 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list CSGsSolidTitle [ CSGSolidConePropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $Base $Heigth $Slices $Stacks $Material ] "" ]
    return $TemporaryNTreeA
  }

  # ------------------------------------------------------------------------------------
  # Create Objects NTree Nodes procedures --
  #
  #			The following procedures create the Objects  NTree Nodes.
  # ------------------------------------------------------------------------------------

  # CreateObjectsNode --
  #
  #			This procedure creates the main Objects  NTree Node.
  #
  # Arguments:
  #
  # Results:
  #
proc CreateObjectsNode { { FileName "" } { Description "" }  { GenMode 0 } { Mass 0 } { Volume 0 } {Density 0 } { CMx 0 } { CMy 0 } { CMz 0 } { lxx 0 } { lyy 0 } { lzz 0 } { lxy 0 } { lyz 0 } { lxz 0 } { rx 0 } { ry 0 } { rz 0 } { Jm 0 } { G 0 } { B 0 } { TcP 0 } { TcN 0 } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    variable Id
    if { $FileName == "" } {
      set TemporaryNTreeA [ TkNTree::Set "Object$Id.obj" StateExpand 5 False True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateExpand 5 False True True $NTree::Null ]
    }
	 set ObjectsNTreeData $TemporaryNTreeA
    TkNTree::SetData $TemporaryNTreeA [ list ObjectsTitle [ ObjectsPropertiesNTree $TemporaryNTreeA $Description $GenMode $Mass $Volume $Density $CMx $CMy $CMz $lxx $lyy $lzz $lxy $lyz $lxz ] $FileName ]
    set NodeNTreeData [ list ObjectsElementsNTree $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Elements" StateExpand 7 False False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set TemporaryNTreeC [ CreateObjectsElementsSonsNode 0 $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type False ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return [ list $TemporaryNTreeA $TemporaryNTreeC ]
  }

  # CreateObjectsElementsSolidsNode --
  #
  #			This procedure creates the Solid NTree Node attached to the Objects Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateObjectsElementsSolidsNode { FileName } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list ObjectsElementsSolidsTitle $NTree::Null $FileName ]
    return $TemporaryNTreeA
  }

  # CreateObjectsElementsSonsNode --
  #
  #			This procedure creates the Solid Element Sons NTree Node attached to the Objects Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateObjectsElementsSonsNode { { ElemGenMode 0 } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } 
                                       { Mass 0 } { Volume 0 } { Density 0 } { CMx 0 } { CMy 0 } { CMz 0 } { lxx 0 } { lyy 0 } { lzz 0 }
                                       { lxy 0 } { lyz 0 } { lxz 0 } { NodeDeleteAllowed True } } {
    set TemporaryNTreeA [ TkNTree::Set "Element" StateExpand 5 False $NodeDeleteAllowed True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list ObjectsElementsTitle [ ElementsPropertiesNTree $TemporaryNTreeA $ElemGenMode $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $Mass $Volume $Density $CMx $CMy $CMz $lxx $lyy $lzz $lxy $lyz $lxz ] "" ]
    set NodeNTreeData [ list ObjectsElementsSolid $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Solid" StateNone 6 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set NodeNTreeData [ list ObjectsElementsSons $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Sons" StateNone 7 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }

  # ------------------------------------------------------------------------------------
  # Create Robots NTree Nodes procedures --
  #
  #			The following procedures create the Robots  NTree Nodes.
  # ------------------------------------------------------------------------------------

  # CreateRobotsNode --
  #
  #			This procedure creates the main Robots Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateRobotsNode { { FileName "" } { Description "" } { vPO1B 0 } { vPO2B 0 } { vPO3B 0 } { vPO4B 0 } { vPO5B 0 } { vPO6B 0 } { PTypeB 0 } { OTypeB 3 } { TypeB 0 } { Alpha 0 } { a 0 } { Theta 0 } { d 0 } { Minimum -360 } { Maximum 360 } { Type Revolute } { Origin 0 } { Nest 0 } { Jm 0 } { G 0 } { B 0 } { TcP 0 } { TcN 0 }  { RobotType 0} } {
    variable Id
    if { $FileName == "" } {
      set TemporaryNTreeA [ TkNTree::Set "Robot$Id.rbt" StateExpand 5 False True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateExpand 5 False True True $NTree::Null ]
    }
	
    TkNTree::SetData $TemporaryNTreeA [ list RobotsTitle [ RobotsPropertiesNTree $TemporaryNTreeA $Description $vPO1B $vPO2B $vPO3B $vPO4B $vPO5B $vPO6B $PTypeB $OTypeB $TypeB $RobotType ] $FileName ]
    set NodeNTreeData [ list RobotsBasesObjects $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Base Objects" StateNone 2 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set NodeNTreeData [ list RobotsLinksNTree $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Links" StateExpand 4 False False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set TemporaryNTreeC [ CreateRobotsLinksSonsNode $Alpha $a $Theta $d $Minimum $Maximum $Type $Origin $Nest $Jm $G $B $TcP $TcN False]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return [ list $TemporaryNTreeA $TemporaryNTreeC ]
  }

  # CreateRobotsBasesObjectsNode --
  #
  #			This procedure creates a robot's base objects Ntree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateRobotsBasesObjectsNode { FileName { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list RobotsBasesObjectsTitle [ ParametricObjectPropertiesNTree $TemporaryNTreeA $Description $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] $FileName ]
    return $TemporaryNTreeA
  }

  # CreateRobotsLinksObjectsNode --
  #
  #			This procedure creates a robot's link Ntree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateRobotsLinksObjectsNode { FileName { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list RobotsLinksObjectsTitle [ ParametricObjectPropertiesNTree $TemporaryNTreeA $Description $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] $FileName ]
    return $TemporaryNTreeA
  }

  # CreateRobotsLinksSonsNode --
  #
  #			This procedure creates a robot's son link Ntree Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateRobotsLinksSonsNode { { Alpha 0 } { a 0 } { Theta 0 } { d 0 } { Minimum -360 } { Maximum 360 } { Type Revolute } { Origin 0 }  { Nest 0 }  { Jm 0 } { G 0 } { B 0 } { TcP 0 } { TcN 0 } { NodeDeleteAllowed True }  } {
    set TemporaryNTreeA [ TkNTree::Set "Link" StateExpand 5 False $NodeDeleteAllowed True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list RobotsLinksTitle [ LinksPropertiesNTree $TemporaryNTreeA $Alpha $a $Theta $d $Minimum $Maximum $Type $Origin $Nest $Jm $G $B $TcP $TcN ] "" ]
    set NodeNTreeData [ list RobotsLinksObjects $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Object" StateNone 2 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set NodeNTreeData [ list RobotsLinksSons $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Sons" StateNone 4 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }


  # ---------------------------------------------------------------------------------------------------------------
  # World's Node Builders --
  #
  #		The following procedures create a World Node and all its data structures.
  # ----------------------------------------------------------------------------------------------------------------

  # World's Node Builders --
  #
  #		The following procedures creates the main World Node.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateWorldsNode { { FileName "" } { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    variable Id
    if { $FileName == "" } {
      set TemporaryNTreeA [ TkNTree::Set "World$Id.wld" StateExpand 5 False True True $NTree::Null ]
    } else {
      set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateExpand 5 False True True $NTree::Null ]
    }
    TkNTree::SetData $TemporaryNTreeA [ list WorldsTitle [ WorldsPropertiesNTree $TemporaryNTreeA $Description ] $FileName ]
	# Worlds Gravity Force
    set TemporaryNTreeC [ TkNTree::Set "Gravity Force" StateNone 37 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeC [ list WorldsGravity [ SixDimForcePropertiesNTree $TemporaryNTreeC 0 0 9.81 0 0 0 ] "" ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeC 
    # Worlds External Force
    set TemporaryNTreeC [ TkNTree::Set "External Force" StateNone 38 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeC [ list WorldsExtForce [ SixDimForcePropertiesNTree $TemporaryNTreeC 0 0 0 0 0 0 ] "" ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeC 
    # WorldsTarget
    set TemporaryNTreeC [ TkNTree::Set "Target" StateNone 29 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeC [ list WorldsTarget [ TargetsPropertiesNTree $TemporaryNTreeC $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] "" ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeC
    # WorldsObjects
    set NodeNTreeData [ list WorldsObjects $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Objects" StateNone 2 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # WorldsRobots
    set NodeNTreeData [ list WorldsRobots $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Robots" StateNone 3 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # WorldsTrajectorys
    set NodeNTreeData [ list WorldsTrajectorys $NTree::Null "" ]
    set TemporaryNTreeB [ TkNTree::Set "Trajectories XYZ" StateNone 33 True False Undefined $NodeNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
   
    return [ list $TemporaryNTreeA $TemporaryNTreeC ]
  }

  #  CreateWorldsObjectsNode --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateWorldsObjectsNode { FileName { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list WorldsObjectsTitle [ ParametricObjectPropertiesNTree $TemporaryNTreeA $Description $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] $FileName ]
    return $TemporaryNTreeA
  }

  #  AddWorldsRobotsBaseTjcNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddWorldsRobotsBaseTjcNode { WorldsNodeNTree } {
    set NodeNTreeData [ list WorldsRobotsBaseTrajectorys $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Base Trajectory" StateCollapse 33 True False Undefined $NodeNTreeData ]
    NTree::Add $WorldsNodeNTree $TemporaryNTreeA
    return $TemporaryNTreeA
   }
   
  #  OpenWorldsRobotsBaseTjcNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc OpenWorldsRobotsBaseTjcNode { BaseNode } {
        ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON  $ObjectNavigator::CurrentNode 1
        RobLab::ExpandNTree $ObjectNavigator::cvsON $ObjectNavigator::frmONTB ObjectNavigator::CurrentNode
        ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON  $BaseNode 1
        set FileName [ ObjectNavigator::GetOpenFileName Trajectory tjc $ObjectNavigator::tplON ]
        if { $FileName != "" } {
        # Begin C Commands !!!
               $ObjectNavigator::CurrentNode AddTrajectory $FileName "" 0 0 0 0 0 0 0 3 0
        # End C Commands !!!
       }
}

#  DestroyWorldsBaseTjcNode  --
#
#
# Arguments:
#
# Results:
#
  proc DestroyWorldsRobotsBaseTjcNode {  } {
        set BaseNode [ NTree::GetLastSon  $ObjectNavigator::CurrentNode ]
        RobLab::CollapseNTree $ObjectNavigator::cvsON $ObjectNavigator::frmONTB ObjectNavigator::CurrentNode
        NTree::DisAssociate $BaseNode
        NTree::Destroy $BaseNode
        #RobLab::ExpandNTree $ObjectNavigator::cvsON $ObjectNavigator::frmONTB ObjectNavigator::CurrentNode
}


    
  #  AddWorldsRobotsTeachPendantsLinkNode --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddWorldsRobotsTeachPendantsLinkNode {  LinkId State Value Minimum Maximum Type } {
          set TemporaryNTreeA [ TkNTree::Set "$LinkId Link" $State 4 False False True $NTree::Null ]
          TkNTree::SetData $TemporaryNTreeA [ list WorldsRobotsTeachPendantsLink [ TeachPendantsLinksPropertiesNTree $TemporaryNTreeA $Value $Minimum $Maximum $Type ] "" ]
          return $TemporaryNTreeA      
  }

  #  AddWorldsRobotsTeachPendantNode --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddWorldsRobotsTeachPendantNode { WorldsNodeNTree } {
    set NodeNTreeData [ list WorldsRobotsTeachPendant $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Kinematics Configuration" StateCollapse 31 False False Undefined $NodeNTreeData ]
    NTree::Add $WorldsNodeNTree $TemporaryNTreeA
    return $TemporaryNTreeA
  }

  #  AddWorldsRobotsToolTransformNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddWorldsRobotsToolTransformNode { WorldsNodeNTree { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "Tool Transform" StateNone 32 False False Undefined $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list WorldsRobotsToolTransform [ ToolTransformsPropertiesNTree $TemporaryNTreeA $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] "" ]
    NTree::Add $WorldsNodeNTree $TemporaryNTreeA
    return $TemporaryNTreeA
  }

  # CreateWorldsRobotsNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateWorldsRobotsNode { FileName { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 }  { BaseType 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateCollapse 5 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list WorldsRobotsTitle [ ParametricRobotPropertiesNTree $TemporaryNTreeA $Description $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type $BaseType ] $FileName ]
	
	# If the IRLTextEditor Window has been loaded, enable the Simulate button.
	if { $RobLab::IRLWindowInit==1  } {
       ToolBars::EnableButton $IRLWindow::frmIRLTB Simulate
    }
  return $TemporaryNTreeA
  }

  # CreateWorldsTrajectorysNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateWorldsTrajectorysNode { FileName { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 33 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list WorldsTrajectorysTitle [ ParametricTrajectoryPropertiesNTree $TemporaryNTreeA $Description $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] $FileName ]
    set NodeNTreeData [ list WorldsTrajectorysPoints $NTree::Null "" ]
    return $TemporaryNTreeA
  }
  
  # CreateWorldsTrajectorysNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateWorldsRobotTjcTitle { FileName { Description "" } { vPO1 0 } { vPO2 0 } { vPO3 0 } { vPO4 0 } { vPO5 0 } { vPO6 0 } { PType 0 } { OType 3 } { Type 0 } } {
    set TemporaryNTreeA [ TkNTree::Set "[ file tail $FileName ]" StateNone 5 False True True $NTree::Null ]
    TkNTree::SetData $TemporaryNTreeA [ list WorldsRobotBaseTjcTitle [ ParametricTrajectoryPropertiesNTree $TemporaryNTreeA $Description $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ] $FileName ]
    set NodeNTreeData [ list WorldsTrajectorysPoints $NTree::Null "" ]
    return $TemporaryNTreeA
  }

  # CreateGNUPlotNode --
  #
  #			This procedure creates the GNUPlot NTree Node and calls the creator procedures
  #         for any of the elements that belongs to the GNUPlot NTree Node in the Object Navigator
  #         Window(Trajectories, Inverse Kinematic, Forward Dynamic, Inverse Dynamic).
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateGNUPlotNode {} {
    variable GNUPlotNode
#set PropertiesNTreeData [ list GNUPlot $NTree::Null "" ]
#  set GNUPlotNode [ TkNTree::Set "GNUPlot" StateCollapse 35 True False Undefined $PropertiesNTreeData ]

    # Inverse Kinematics
#  NTree::Add $GNUPlotNode [ CreateInvKineNode ]
    # Forward Dynamics
#  NTree::Add $GNUPlotNode [ CreateFwdDynNode ]
    # Inverse Dynamics
#NTree::Add $GNUPlotNode [ CreateInvDynNode ]
  
    return $GNUPlotNode
  }

  # CreateInvKineNode --
  #
  #			This procedure creates the Inverse Kinematics NTree Node in the Object Navigator Window.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateInvKineNode { } {
	
    #Inverse Kinematics
	set NodeNTreeData [ list InverseKinematics $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Inverse Kinematics" StateNone 22 False False True $NodeNTreeData ]
    
    return $TemporaryNTreeA
  }

# CreateFwdDynNode --
#
#			This procedure creates the Forward Dynamics NTree Node in the Object Navigator Window.
#
# Arguments:
#
# Results:
#
  proc CreateFwdDynNode { } {

    #Forward Dynamics
	set NodeNTreeData [ list ForwardDynamics $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Forward Dynamics" StateNone 22 False False True $NodeNTreeData ]
    
    return $TemporaryNTreeA
}

  # CreateInvDynNode --
  #
  #			This procedure creates the Inverse Dynamics NTree Node in the Object Navigator Window.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateInvDynNode { } {

    #Inverse Dynamics
	set NodeNTreeData [ list InverseDynamics $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Inverse Dynamics" StateNone 22 False False True $NodeNTreeData ]
    
    return $TemporaryNTreeA
  }

