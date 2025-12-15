#----------------------------------------------------------------------------------------------------------------------------------
# ObjNavPropPal.tcl
# 		This file implements the TCL code for creating and managing the the properties subtree
#      to manipulate the data belonging to a Object Navigator NTree Node.
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

 #------------------------------------------------------------------------------------------------------------------
 # Properties Palette subtree procedures.
 #
 # 		These procedures define the properties subtree to manipulate the data
 #      belonging to a data structure.
 #      These properties will be managed by the PropertyPalette component. This
 #      component calls the ModifyProperty procedure followed by the name of the
 #      property to be modified and a new value.
 #
 #      Example:
 #         $Parent          ModifyProperty (Name or secondary title) $Value
 #    (Parent's NodeId,                    theta                       35
 #        i.e. Node48)
 #--------------------------------------------------------------------------------------------------------------------

  #  RGBAsPropertiesNTree procedure --
  #
  #		This procedure is used for any Object Navigator NTree like Lighting and
  #		Materials to define the RGB (Red, Green, Blue) intensity.
  #
  # Arguments:
  #
  # Results:
  #
  proc RGBAsPropertiesNTree { Parent Title Name Red Green Blue Alpha } {
    variable Scale01Factor
    set PropertiesNTreeData [ list "$Parent" "$Title" ]
    set TemporaryNTreeA [ TkNTree::Set "$Title" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    set PropertiesNTreeData [ list "$Parent" "$Name\Red" "Scale" [ list $Red "0 1 $Scale01Factor" ] "Red" ]
    set TemporaryNTreeB [ TkNTree::Set "Red = $Red" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "$Name\Green" "Scale" [ list $Green "0 1 $Scale01Factor" ] "Green" ]
    set TemporaryNTreeB [ TkNTree::Set "Green = $Green" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "$Name\Blue" "Scale" [ list $Blue "0 1 $Scale01Factor" ] "Blue" ]
    set TemporaryNTreeB [ TkNTree::Set "Blue = $Blue" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "$Name\Alpha" "Scale" [ list $Alpha "0 1 $Scale01Factor" ] "Alpha" ]
    set TemporaryNTreeB [ TkNTree::Set "Alpha = $Alpha" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }

  # ChangeCamerasProperties procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change the Orientation type of any element in a 3D space (Euler RPY, Euler X, Euler Y, Euler Z, EulerXYZ).
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeCameraValues { NTree RadiusValue LengthType } {
    
    variable ScaleMinDistanceFactor
    variable ScaleCamDistanceMinimum
    
    switch $LengthType {
      0 {
          set ListOfElements [ list $ScaleCamDistanceMinimum 100 $ScaleMinDistanceFactor ]
          MessagesWindow::Puts "Estoy Aqui 1\n" 
          PropertyPalette::ChangePropertyTitleValue $NTree "Radius" "Radius" $RadiusValue "$ListOfElements"
          MessagesWindow::Puts "Estoy Aqui 2\n" 
        }
      1 {
          set ListOfElements [ list $ScaleCamDistanceMinimum 1000 $ScaleMinDistanceFactor ]
          MessagesWindow::Puts "Estoy Aqui 1\n" 
          PropertyPalette::ChangePropertyTitleValue $NTree "Radius" "Radius" $RadiusValue "$ListOfElements"
          MessagesWindow::Puts "Estoy Aqui 2\n"
        }
    }
    #MessagesWindow::Puts "Estoy Aqui\n 3"  
  }

  # CamerasPropertiesNTree procedure --
  #
  #		This procedure is used by the ObjectNavigator's Camera NTree Node to
  #		define the Camera's Properties (X,Y,Z,Radius, Elevation, Azimuth and Twist angles).
  #
  # Arguments:
  #
  # Results:
  #
  proc CamerasPropertiesNTree { Parent CenterX CenterY CenterZ CenterVisible Radius Elevation Azimuth Twist } {
    variable CamerasPropertiesNTreeData
    variable ScaleCamDistanceMinimum
    variable ScaleCamDistanceMaximum
	variable ScaleCamRadiusMaximum
    variable ScaleAngleDegreeMinimum
    variable ScaleAngleDegreeMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleMinAngleFactor
    variable ScaleMaxAngleFactor
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 19 False False Undefined $PropertiesNTreeData ]
    set PropertiesNTreeData [ list "$Parent" "CenterX" "Scale" [ list $CenterX "$ScaleCamDistanceMinimum $ScaleCamDistanceMaximum $ScaleMinDistanceFactor" ] "Center X" ]
    set TemporaryNTreeB [ TkNTree::Set "Center X = $CenterX" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "CenterY" "Scale" [ list $CenterY "$ScaleCamDistanceMinimum $ScaleCamDistanceMaximum $ScaleMinDistanceFactor" ] "Center Y" ]
    set TemporaryNTreeB [ TkNTree::Set "Center Y = $CenterY" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "CenterZ" "Scale" [ list $CenterZ "$ScaleCamDistanceMinimum $ScaleCamDistanceMaximum $ScaleMinDistanceFactor" ] "Center Z" ]
    set TemporaryNTreeB [ TkNTree::Set "Center Z = $CenterZ" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "CenterVisible" "List" [ list $CenterVisible { True False } ] "Center Visible" ]
    set TemporaryNTreeB [ TkNTree::Set "Center Visible = $CenterVisible" StateNone 22 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "Radius" "Scale" [ list $Radius "0 $ScaleCamDistanceMaximum $ScaleMinDistanceFactor" ] ]
    set TemporaryNTreeB [ TkNTree::Set "Radius = $Radius" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "Elevation" "Scale" [ list $Elevation "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeB [ TkNTree::Set "Elevation = $Elevation" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "Azimuth" "Scale" [ list $Azimuth "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeB [ TkNTree::Set "Azimuth = $Azimuth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "Twist" "Scale" [ list $Twist "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeB [ TkNTree::Set "Twist = $Twist" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set CamerasPropertiesNTreeData $TemporaryNTreeA
    return $TemporaryNTreeA
  }

  # LightingsPropertiesNTree procedure --
  #
  #		This procedure is used by the ObjectNavigator's  Lighting NTree Node to
  #		define the Light Source Properties (Ambient, Diffuse, Specular).
  #
  # Arguments:
  #
  # Results:
  #
  proc LightingsPropertiesNTree { Parent LSAmbientRed LSAmbientGreen LSAmbientBlue LSAmbientAlpha LSDiffuseRed LSDiffuseGreen LSDiffuseBlue LSDiffuseAlpha LSSpecularRed LSSpecularGreen LSSpecularBlue LSSpecularAlpha MAmbientRed MAmbientGreen MAmbientBlue MAmbientAlpha } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 17 False False Undefined $PropertiesNTreeData ]
    # Light Source
    set PropertiesNTreeData [ list "$Parent" "Light Source" ]
    set TemporaryNTreeB [ TkNTree::Set "Light Source" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # LSAmbient
    NTree::Add $TemporaryNTreeB [ RGBAsPropertiesNTree "$Parent" "Ambient" "LSAmbient" $LSAmbientRed $LSAmbientGreen $LSAmbientBlue $LSAmbientAlpha ]
    # LSDiffuse
    NTree::Add $TemporaryNTreeB [ RGBAsPropertiesNTree "$Parent" "Diffuse" "LSDiffuse" $LSDiffuseRed $LSDiffuseGreen $LSDiffuseBlue $LSDiffuseAlpha ]
    # LSSpecular
    NTree::Add $TemporaryNTreeB [ RGBAsPropertiesNTree "$Parent" "Specular" "LSSpecular" $LSSpecularRed $LSSpecularGreen $LSSpecularBlue $LSSpecularAlpha ]
    # Model
    set PropertiesNTreeData [ list "$Parent" "Model" ]
    set TemporaryNTreeB [ TkNTree::Set "Model" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # MAmbient
    NTree::Add $TemporaryNTreeB [ RGBAsPropertiesNTree "$Parent" "Ambient" "MAmbient" $MAmbientRed $MAmbientGreen $MAmbientBlue $MAmbientAlpha ]
    return $TemporaryNTreeA
  }

  # GridsPropertiesNTree procedure --
  #
  #		This procedure is used by the ObjectNavigator's  Grid NTree Node to
  #		define the Grid's Properties (Width, Depth, Height).
  #
  # Arguments:
  #
  # Results:
  #
  proc GridsPropertiesNTree { Parent Width Depth Height } {
    variable GridsPropertiesNTreeData
    variable ScaleWidthMinimum
    variable ScaleWidthMaximum
    variable ScaleWidthFactor
    variable ScaleDepthMinimum
    variable ScaleDepthMaximum
    variable ScaleDepthFactor
    variable ScaleHeightMinimum
    variable ScaleHeightMaximum
    variable ScaleHeightFactor

    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 36 False False Undefined $PropertiesNTreeData ]
    set PropertiesNTreeData [ list "$Parent" "Width" "Scale" [ list $Width "$ScaleWidthMinimum $ScaleWidthMaximum $ScaleWidthFactor" ] "Width" ]
    set TemporaryNTreeB [ TkNTree::Set "Width = $Width" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set PropertiesNTreeData [ list "$Parent" "Depth" "Scale" [ list $Depth "$ScaleDepthMinimum $ScaleDepthMaximum $ScaleDepthFactor" ] "Depth" ]
    set TemporaryNTreeB [ TkNTree::Set "Depth = $Depth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set PropertiesNTreeData [ list "$Parent" "Height" "Scale" [ list $Height "$ScaleHeightMinimum $ScaleHeightMaximum $ScaleHeightFactor" ] "Height" ]
    set TemporaryNTreeB [ TkNTree::Set "Height = $Height" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set GridsPropertiesNTreeData $TemporaryNTreeA
    return $TemporaryNTreeA
  }

  # ChangeGridValues procedure --
  #
  #	This procedure is used by any ObjectNavigator's NTree Node to
  #	change the Grid values in a Metric unit transformation.
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeGridValues { NTree WidthValue HeightValue DepthValue LengthType } {
    variable ScaleWidthMinimum
    variable ScaleWidthMaximum
    variable ScaleWidthFactor
    variable ScaleDmWidthMinimum 
    variable ScaleDmWidthMaximum
    variable ScaleDmDepthMinimum
    variable ScaleDmDepthMaximum
    variable ScaleDmHeightMaximum
    
    #MessagesWindow::Puts "NTree = $NTree\n"
    #MessagesWindow::Puts "LengthType = $LengthType\n"
    
    switch $LengthType {
      0 {
          #Width
          set ListOfElements [ list $ScaleWidthMinimum $ScaleWidthMaximum $ScaleWidthFactor ]
          #MessagesWindow::Puts "Estoy Aqui 1\n" 
          PropertyPalette::ChangePropertyTitleValue $NTree "Width" "Width" $WidthValue "$ListOfElements"
          #MessagesWindow::Puts "Estoy Aqui 2\n" 
        }
      1 {
          #Width
          set ListOfElements [ list  $ScaleDmWidthMinimum $ScaleDmWidthMaximum $ScaleWidthFactor ]
          #MessagesWindow::Puts "Estoy Aqui 1\n" 
          PropertyPalette::ChangePropertyTitleValue $NTree "Width" "Width" $WidthValue "$ListOfElements"
          #MessagesWindow::Puts "Estoy Aqui 2\n"
        }
    }
    #MessagesWindow::Puts "Estoy Aqui\n 3"  
  }

  # MetricUnitsPropertiesNTree procedure --
  #
  #		This procedure is used by the ObjectNavigator's MetricUnits NTree Node to
  #		define the MetricUnits's Properties (Length, mass, time, angle, force).
  #
  # Arguments:
  #
  # Results:
  #
  proc MetricUnitsPropertiesNTree { Parent LengthType MassType TimeType AngleType ForceType } {

    variable MetricUnitsPropertiesNTreeData
    variable LstLengthTypes
    variable LstMassTypes
    variable LstTimeTypes
    variable LstAngleTypes
    variable LstForceTypes

    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 22 False False Undefined $PropertiesNTreeData ]

    set PropertiesNTreeData [ list "$Parent" "Length" "List" [ list [ lindex $LstLengthTypes $LengthType ] $LstLengthTypes ] "Length" ]
    set TemporaryNTreeB [ TkNTree::Set "Length = [ lindex $LstLengthTypes $LengthType ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set PropertiesNTreeData [ list "$Parent" "Mass" "List" [ list [ lindex $LstMassTypes $MassType ] $LstMassTypes ] "Mass" ]
    set TemporaryNTreeB [ TkNTree::Set "Mass = [ lindex $LstMassTypes $MassType ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set PropertiesNTreeData [ list "$Parent" "Time" "List" [ list [ lindex $LstTimeTypes $TimeType ] $LstTimeTypes ] "Time" ]
    set TemporaryNTreeB [ TkNTree::Set "Time = [ lindex $LstTimeTypes $TimeType ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set PropertiesNTreeData [ list "$Parent" "Angle" "List" [ list [ lindex $LstAngleTypes $AngleType ] $LstAngleTypes ] "Angle" ]
    set TemporaryNTreeB [ TkNTree::Set "Angle = [ lindex $LstAngleTypes $AngleType ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set PropertiesNTreeData [ list "$Parent" "Force" "List" [ list [ lindex $LstForceTypes $ForceType ] $LstForceTypes ] "Force" ]
    set TemporaryNTreeB [ TkNTree::Set "Force = [ lindex $LstForceTypes $ForceType ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    set MetricUnitsPropertiesNTreeData $TemporaryNTreeA
    return $TemporaryNTreeA
  }

  # MaterialsPropertiesNTree procedure --
  #
  #		This procedure is used by the ObjectNavigator's Materials NTree Node to
  #		define the Materials Properties NTree (Ambient, Diffuse, Specular, Emission, Shininess, Density, DensityType).
  #
  # Arguments:
  #
  # Results:
  #
  proc MaterialsPropertiesNTree { Parent Description AmbientRed AmbientGreen AmbientBlue AmbientAlpha DiffuseRed DiffuseGreen DiffuseBlue DiffuseAlpha SpecularRed SpecularGreen SpecularBlue SpecularAlpha EmissionRed EmissionGreen EmissionBlue EmissionAlpha Shininess Density DensityType } {
    variable Scale01Factor
    variable ScaleDensityMinimum
    variable ScaleDensityMaximum
    variable ScaleDensityFactor
    variable LstDensityTypes

    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 18 False False Undefined $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    	NTree::Add $TemporaryNTreeA [ RGBAsPropertiesNTree "$Parent" "Ambient" "Ambient" $AmbientRed $AmbientGreen $AmbientBlue $AmbientAlpha ]
    	NTree::Add $TemporaryNTreeA [ RGBAsPropertiesNTree "$Parent" "Diffuse" "Diffuse" $DiffuseRed $DiffuseGreen $DiffuseBlue $DiffuseAlpha ]
    	NTree::Add $TemporaryNTreeA [ RGBAsPropertiesNTree "$Parent" "Specular" "Specular" $SpecularRed $SpecularGreen $SpecularBlue $SpecularAlpha ]
    	NTree::Add $TemporaryNTreeA [ RGBAsPropertiesNTree "$Parent" "Emission" "Emission" $EmissionRed $EmissionGreen $EmissionBlue $EmissionAlpha ]
    	set PropertiesNTreeData [ list "$Parent" "Shininess" "Scale" [ list $Shininess "0 128 $Scale01Factor" ] "Shininess" ]
    	set TemporaryNTreeB [ TkNTree::Set "Shininess = $Shininess" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    	set PropertiesNTreeData [ list "$Parent" "Density" "Scale" [ list $Density "$ScaleDensityMinimum $ScaleDensityMaximum $ScaleDensityFactor" ] "Density" ]
    	set TemporaryNTreeB [ TkNTree::Set "Density = $Density" StateExpand 21 False False True $PropertiesNTreeData ]
        set PropertiesNTreeData [ list "$Parent" "DensityType" "List" [ list [ lindex $LstDensityTypes $DensityType ] $LstDensityTypes ] "Measurement Unit" ]
        set TemporaryNTreeC [ TkNTree::Set "Measurement Unit = [ lindex $LstDensityTypes $DensityType ]" StateNone 22 True True True $PropertiesNTreeData ]
        NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    	return $TemporaryNTreeA
  }

  # GeneralsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GeneralsPropertiesNTree { Parent Description } {
    set PropertiesNTreeData [ list "$Parent" "Description" "Text" [ list "$Description" ] "$NTree::Null" ]
    set TemporaryNTreeA [ TkNTree::Set "Description = $Description" StateNone 20 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
  }

  # GeneralsSectionPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GeneralsSectionPropertiesNTree { Parent Name } {
    set PropertiesNTreeData [ list "$Parent" "Name" "Text" [ list "$Name" ] "$NTree::Null" ]
    set TemporaryNTreeA [ TkNTree::Set "Name = $Name" StateNone 20 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
  }

  # ChangePositionsType procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change the Position type of any element in a 3D space (Cartesian, Cylindrical, Spherical).
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangePositionsType { NTree vPO1 vPO2 vPO3 PType } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleAngleDegreeMinimum
    variable ScaleAngleDegreeMaximum
    variable ScaleAngleRadMinimum
    variable ScaleAngleRadMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleMinAngleFactor
    variable ScaleMaxAngleFactor
    variable LstPTypes
    variable AngleType
	variable LengthType

    switch $LengthType {
      0 {
               set ScaleDistanceMinimum -10
               set ScaleDistanceMaximum 10
          }
      1 {
               set ScaleDistanceMinimum -100
               set ScaleDistanceMaximum 100  
          }
	  2 {
              set ScaleDistanceMinimum -1000
              set ScaleDistanceMaximum 1000 
          }
	   3 {
               set ScaleDistanceMinimum -10000
               set ScaleDistanceMaximum 10000
          } 
	  4 {
               set ScaleDistanceMinimum -10
               set ScaleDistanceMaximum 10
          } 
	  5 {
               set ScaleDistanceMinimum -1000
               set ScaleDistanceMaximum 1000
          }  	  
    }
	
    switch $PType {
      0 {
          set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO1" "X" $vPO1 "$ListOfElements"
          set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO2" "Y" $vPO2 "$ListOfElements"
          set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO3" "Z" $vPO3 "$ListOfElements"
        }
      1 {
          set ListOfElements [ list 0 $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO1" "R" $vPO1 "$ListOfElements"

          switch $AngleType {
            0 {
                  set ListOfElements [ list $ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor ]
                }
            1 {
                 set ListOfElements [ list $ScaleAngleRadMinimum $ScaleAngleRadMaximum $ScaleMinAngleFactor ]
                }
          }

          PropertyPalette::ChangePropertyTitleValue $NTree "vPO2" "Theta" $vPO2 "$ListOfElements"
          set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO3" "Z" $vPO3 "$ListOfElements"
        }
      2 {
          set ListOfElements [ list 0 $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO1" "R" $vPO1 "$ListOfElements"

           switch $AngleType {
             0 {
                    set ListOfElements [ list $ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor ]
                 }
             1 {
                    set ListOfElements [ list $ScaleAngleRadMinimum $ScaleAngleRadMaximum $ScaleMinAngleFactor ]
                 }
          }
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO2" "Theta" $vPO2 "$ListOfElements"

          PropertyPalette::ChangePropertyTitleValue $NTree "vPO3" "Phi" $vPO3 "$ListOfElements"
        }
    }
    PropertyPalette::ChangePropertyTitleValue $NTree "PType" "Type" [ lindex $LstPTypes $PType ] $LstPTypes
  }

  # ChangeOrientationsType procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change the Orientation type of any element in a 3D space (Euler RPY, Euler X, Euler Y, Euler Z, EulerXYZ).
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeOrientationsType { NTree vPO4 vPO5 vPO6 OType } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleAngleDegreeMinimum
    variable ScaleAngleDegreeMaximum
    variable ScaleAngleRadMinimum
    variable ScaleAngleRadMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleMinAngleFactor
    variable ScaleMaxAngleFactor
    variable LstOTypes
    variable AngleType

    switch $AngleType {
      0 {
                set ListOfElements [ list $ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor ]
          }
       1 {
                set ListOfElements [ list $ScaleAngleRadMinimum $ScaleAngleRadMaximum $ScaleMinAngleFactor ]
          }
    }
    switch $OType {
      0 {
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO4" "Phi" $vPO4 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO5" "Theta" $vPO5 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO6" "Psi" $vPO6 "$ListOfElements"
        }
      1 {
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO4" "Phi" $vPO4 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO5" "Theta" $vPO5 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO6" "Psi" $vPO6 "$ListOfElements"
        }
      2 {
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO4" "Phi" $vPO4 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO5" "Theta" $vPO5 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO6" "Psi" $vPO6 "$ListOfElements"
        }
      3 {
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO4" "Roll" $vPO4 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO5" "Pitch" $vPO5 "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "vPO6" "Yaw" $vPO6 "$ListOfElements"
        }
    }
    PropertyPalette::ChangePropertyTitleValue $NTree "OType" "Type" [ lindex $LstOTypes $OType ] $LstOTypes
  }

  # ChangeReferencesType procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeReferencesType { NTree RType } {
    variable LstRTypes
    PropertyPalette::ChangePropertyTitleValue $NTree "RType" "Reference Type" [ lindex $LstRTypes $RType ] $LstRTypes
  }

  # VectorsPropertiesNTree procedure --
  #
  #		This procedure is used by any Object Navigator NTree Node that needs
  #       a vector format to show its attributes.
  #
  # Arguments:
  #
  # Results:
  #
  proc VectorsPropertiesNTree { Parent VTitle LstNames LstValues LstTitles } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    set PropertiesNTreeData [ list "$Parent" "$VTitle" ]
    set TemporaryNTreeA [ TkNTree::Set "$VTitle" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    set Length [ llength $LstNames ]
    for { set i 0 } { $i < $Length } { incr i } {
      set Name [ lindex $LstNames $i ]
      set Value [ lindex $LstValues $i ]
      set Title [ lindex $LstTitles $i ]
      set PropertiesNTreeData [ list "$Parent" "$Name" "Scale" [ list $Value "$ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "$Title" ]
      set TemporaryNTreeB [ TkNTree::Set "$Title = $Value" StateNone 21 False False True $PropertiesNTreeData ]
      NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    }
    return $TemporaryNTreeA
  }

  # PositionOrientationsPropertiesNTree procedure --
  #
  #		This procedure shows the Position and Orientation data for any
  #       Object Navigator Node in the Properties Palette window.
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc PositionOrientationsPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {

    #
    #  P = Position       -> vPO1, vPO2, vPO3
    #       0. Cartesian       ( X, Y, Z )
    #       1. Cylindrical     ( R, Theta, Z )
    #       2. Spherical       ( R, Theta, Phi )
    #  O = Orientation    -> vPO4, vPO5, vPO6
    #      0. Euler XYZ       ( Phi, Theta, Psi )
    #      1. Euler X         ( Phi, Theta, Psi )
    #      2. Euler Y         ( Phi, Theta, Psi )
    #      3. Euler RPY       ( Roll, Pitch, Yaw )
    #
    #  RType ( Local Global )
    #

    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleAngleDegreeMinimum
    variable ScaleAngleDegreeMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleMinAngleFactor
    variable ScaleMaxAngleFactor
    variable LstPTypes
    variable LstOTypes
    variable LstRTypes
    switch $PType {
      0 {
          set vPO1Name "X"
          set vPO1ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          set vPO2Name "Y"
          set vPO2ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          set vPO3Name "Z"
          set vPO3ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
        }
      1 {
          set vPO1Name "R"
          set vPO1ListOfElements [ list 0 $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          set vPO2Name "Theta"
          set vPO2ListOfElements [ list $ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor ]
          set vPO3Name "Z"
          set vPO3ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
        }
      2 {
          set vPO1Name "R"
          set vPO1ListOfElements [ list 0 $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
          set vPO2Name "Theta"
          set vPO2ListOfElements [ list $ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor ]
          set vPO3Name "Phi"
          set vPO3ListOfElements [ list $ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor ]
        }
    }
    switch $OType {
      0 {
          set vPO4Name "Phi"
          set vPO5Name "Theta"
          set vPO6Name "Psi"
        }
      1 {
          set vPO4Name "Phi"
          set vPO5Name "Theta"
          set vPO6Name "Psi"
        }
      2 {
          set vPO4Name "Phi"
          set vPO5Name "Theta"
          set vPO6Name "Psi"
        }
      3 {
          set vPO4Name "Roll"
          set vPO5Name "Pitch"
          set vPO6Name "Yaw"
        }
    }
    set PropertiesNTreeData [ list "$Parent" "Transform" ]
    set TemporaryNTreeA [ TkNTree::Set "Transform" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    # Position
    set PropertiesNTreeData [ list "$Parent" "Position" ]
    set TemporaryNTreeB [ TkNTree::Set "Position" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    # vPO1
    set PropertiesNTreeData [ list "$Parent" "vPO1" "Scale" [ list $vPO1 "$vPO1ListOfElements" ] "$vPO1Name" ]
    set TemporaryNTreeC [ TkNTree::Set "$vPO1Name = $vPO1" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    # vPO2
    set PropertiesNTreeData [ list "$Parent" "vPO2" "Scale" [ list $vPO2 "$vPO2ListOfElements" ] "$vPO2Name" ]
    set TemporaryNTreeC [ TkNTree::Set "$vPO2Name = $vPO2" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    # vPO3
    set PropertiesNTreeData [ list "$Parent" "vPO3" "Scale" [ list $vPO3 "$vPO3ListOfElements" ] "$vPO3Name" ]
    set TemporaryNTreeC [ TkNTree::Set "$vPO3Name = $vPO3" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    # PTypes
    set PropertiesNTreeData [ list "$Parent" "PType" "List" [ list [ lindex $LstPTypes $PType ] $LstPTypes ] "Type" ]
    set TemporaryNTreeC [ TkNTree::Set "Type = [ lindex $LstPTypes $PType ]" StateNone 22 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # Orientation
    set PropertiesNTreeData [ list "$Parent" "Orientation" ]
    set TemporaryNTreeB [ TkNTree::Set "Orientation" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    # vPO4
    set PropertiesNTreeData [ list "$Parent" "vPO4" "Scale" [ list $vPO4 "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] "$vPO4Name" ]
    set TemporaryNTreeC [ TkNTree::Set "$vPO4Name = $vPO4" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    # vPO5
    set PropertiesNTreeData [ list "$Parent" "vPO5" "Scale" [ list $vPO5 "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] "$vPO5Name" ]
    set TemporaryNTreeC [ TkNTree::Set "$vPO5Name = $vPO5" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    # vPO6
    set PropertiesNTreeData [ list "$Parent" "vPO6" "Scale" [ list $vPO6 "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] "$vPO6Name" ]
    set TemporaryNTreeC [ TkNTree::Set "$vPO6Name = $vPO6" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    # OTypes
    set PropertiesNTreeData [ list "$Parent" "OType" "List" [ list [ lindex $LstOTypes $OType ] $LstOTypes ] "Type" ]
    set TemporaryNTreeC [ TkNTree::Set "Type = [ lindex $LstOTypes $OType ]" StateNone 22 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    # RTypes
    set PropertiesNTreeData [ list "$Parent" "RType" "List" [ list [ lindex $LstRTypes $Type ] $LstRTypes ] "Reference Type" ]
    set TemporaryNTreeB [ TkNTree::Set "Reference Type = [ lindex $LstRTypes $Type ]" StateNone 22 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }

  # GraphicsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GraphicsPropertiesNTree { Parent { Visible True } } {
    set PropertiesNTreeData [ list "$Parent" "Visible" "List" [ list $Visible { True False} ] ]
    set TemporaryNTreeA [ TkNTree::Set "Visible = $Visible" StateNone 22 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
  }
  
  # GraphicsPOVsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GraphicsPOVsPropertiesNTree { Parent { POVsVisible False } } {
    set PropertiesNTreeData [ list "$Parent" "Position and Orientation Visible" "List" [ list $POVsVisible { False True } ] ]
    set TemporaryNTreeA [ TkNTree::Set "Positon and Orientation Visible = $POVsVisible" StateNone 22 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
  }

  # GraphicsVelocityPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GraphicsVelocityPropertiesNTree { Parent { VelVisible False } } {
    set PropertiesNTreeData [ list "$Parent" "Velocity Visible" "List" [ list $VelVisible { False True } ] ]
    set TemporaryNTreeA [ TkNTree::Set "Velocity Visible = $VelVisible" StateNone 22 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
  }

  # GraphicsAccelPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GraphicsAccelPropertiesNTree { Parent { AccelVisible False } } {
    set PropertiesNTreeData [ list "$Parent" "Acceleration Visible" "List" [ list $AccelVisible { False True } ] ]
    set TemporaryNTreeA [ TkNTree::Set "Acceleration Visible = $AccelVisible" StateNone 22 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
  }
  
  # FollowTargetNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc FollowTargetPropertiesNTree { Parent { Follow False } } {
    set PropertiesNTreeData [ list "$Parent" "Follow Target" "List" [ list $Follow { True False} ] ]
    set TemporaryNTreeA [ TkNTree::Set "Follow Target = $Follow" StateNone 22 False False True $PropertiesNTreeData ]
    return $TemporaryNTreeA
}

  # CSGPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGPropertiesNTree { Parent Description } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 6 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    return $TemporaryNTreeA
  }

  # CSGOperatorUnionPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGOperatorUnionPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    variable LstOperatorTypes

    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 8 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]

    set PropertiesNTreeData [ list "$Parent" "Change Operator Type" "List" [ list [ lindex $LstOperatorTypes 0 ] $LstOperatorTypes ] "Change Operator Type" ]
    set TemporaryNTreeB [ TkNTree::Set "Change Operator Type = [ lindex $LstOperatorTypes 0 ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB

    return $TemporaryNTreeA
  }

  # CSGOperatorIntersectionPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGOperatorIntersectionPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    variable LstOperatorTypes
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 9 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]

    set PropertiesNTreeData [ list "$Parent" "Change Operator Type" "List" [ list [ lindex $LstOperatorTypes 1 ] $LstOperatorTypes ] "Change Operator Type" ]
    set TemporaryNTreeB [ TkNTree::Set "Change Operator Type = [ lindex $LstOperatorTypes 1 ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    
    return $TemporaryNTreeA
  }

  # CSGOperatorDifferencePropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGOperatorDifferencePropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    variable LstOperatorTypes
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 10 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]

    set PropertiesNTreeData [ list "$Parent" "Change Operator Type" "List" [ list [ lindex $LstOperatorTypes 2 ] $LstOperatorTypes ] "Change Operator Type" ]
    set TemporaryNTreeB [ TkNTree::Set "Change Operator Type = [ lindex $LstOperatorTypes 2 ]" StateNone 22 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }

  # CSGSolidSpherePropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGSolidSpherePropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type Radius Slices Stacks Material } {
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScalePolygonMaximum
    variable ScaleSpherePrmMinimum 12
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 11 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    set PropertiesNTreeData [ list "$Parent" "Parameters" ]
    set TemporaryNTreeB [ TkNTree::Set "Parameters" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "SphereRadius" "Scale" [ list $Radius "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Radius" ]
    set TemporaryNTreeC [ TkNTree::Set "Radius = $Radius" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SphereSlices" "Scale" [ list $Slices "$ScaleSpherePrmMinimum $ScalePolygonMaximum 1" ] "Slices" ]
    set TemporaryNTreeC [ TkNTree::Set "Slices = $Slices" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SphereStacks" "Scale" [ list $Stacks "$ScaleSpherePrmMinimum $ScalePolygonMaximum 1" ] "Stacks" ]
    set TemporaryNTreeC [ TkNTree::Set "Stacks = $Stacks" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SolidMaterial" "FileName" [ list "$Material" "Material mtr" ] "Material" ]
    set TemporaryNTreeC [ TkNTree::Set "Material = $Material" StateNone 24 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return $TemporaryNTreeA
  }

  # ChangeSphereValues procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change a sphere geometric values (Radius).
  #
  # Arguments:
  #
  # Results:
  #
  proc  ChangeSphereValues { NTree  Radius } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
     variable ScaleMinDistanceFactor
     variable ScaleMaxDistanceFactor

    set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
    PropertyPalette::ChangePropertyTitleValue $NTree "SphereRadius" "Radius" $Radius "$ListOfElements"
}

  # CSGSolidCylinderPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGSolidCylinderPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type BaseRadius TopRadius Heigth Slices Stacks Material } {
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScalePolygonMaximum
    variable ScaleCylSlicesMinimum 5
    variable ScaleCylStacksMinimum 1
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 13 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    set PropertiesNTreeData [ list "$Parent" "Parameters" ]
    set TemporaryNTreeB [ TkNTree::Set "Parameters" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "CylinderBaseRadius" "Scale" [ list $BaseRadius "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Base Radius" ]
    set TemporaryNTreeC [ TkNTree::Set "Base Radius = $BaseRadius" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CylinderTopRadius" "Scale" [ list $TopRadius "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Top Radius" ]
    set TemporaryNTreeC [ TkNTree::Set "Top Radius = $TopRadius" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CylinderHeigth" "Scale" [ list $Heigth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Heigth" ]
    set TemporaryNTreeC [ TkNTree::Set "Heigth = $Heigth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CylinderSlices" "Scale" [ list $Slices "$ScaleCylSlicesMinimum $ScalePolygonMaximum 1" ] "Slices" ]
    set TemporaryNTreeC [ TkNTree::Set "Slices = $Slices" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CylinderStacks" "Scale" [ list $Stacks "$ScaleCylStacksMinimum $ScalePolygonMaximum 1" ] "Stacks" ]
    set TemporaryNTreeC [ TkNTree::Set "Stacks = $Stacks" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SolidMaterial" "FileName" [ list "$Material" "Material mtr" ] "Material" ]
    set TemporaryNTreeC [ TkNTree::Set "Material = $Material" StateNone 24 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return $TemporaryNTreeA
  }

  # ChangeCylinderValues procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change a cylinder geometric values (Radius).
  #
  # Arguments:
  #
  # Results:
  #
  proc  ChangeCylinderValues { NTree  BaseRadius TopRadius Heigth } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
     variable ScaleMinDistanceFactor
     variable ScaleMaxDistanceFactor

    set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
    PropertyPalette::ChangePropertyTitleValue $NTree "CylinderBaseRadius" "Base Radius" $BaseRadius "$ListOfElements"
    PropertyPalette::ChangePropertyTitleValue $NTree "CylinderTopRadius" "Top Radius" $TopRadius "$ListOfElements"
    PropertyPalette::ChangePropertyTitleValue $NTree "CylinderHeigth" "Heigth" $Heigth "$ListOfElements"
}

  # CSGSolidCubePropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGSolidCubePropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type BaseWidth BaseDepth TopWidth TopDepth Heigth Material } {
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 14 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    set PropertiesNTreeData [ list "$Parent" "Parameters" ]
    set TemporaryNTreeB [ TkNTree::Set "Parameters" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "CubeBaseWidth" "Scale" [ list $BaseWidth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Base Width" ]
    set TemporaryNTreeC [ TkNTree::Set "Base Width = $BaseWidth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CubeBaseDepth" "Scale" [ list $BaseDepth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Base Depth" ]
    set TemporaryNTreeC [ TkNTree::Set "Base Depth = $BaseDepth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CubeTopWidth" "Scale" [ list $TopWidth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Top Width" ]
    set TemporaryNTreeC [ TkNTree::Set "Top Width = $TopWidth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CubeTopDepth" "Scale" [ list $TopDepth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Top Depth" ]
    set TemporaryNTreeC [ TkNTree::Set "Top Depth = $TopDepth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "CubeHeigth" "Scale" [ list $Heigth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Heigth" ]
    set TemporaryNTreeC [ TkNTree::Set "Heigth = $Heigth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SolidMaterial" "FileName" [ list "$Material" "Material mtr" ] "Material" ]
    set TemporaryNTreeC [ TkNTree::Set "Material = $Material" StateNone 24 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return $TemporaryNTreeA
  }

  # ChangeCubeValues procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change a cylinder geometric values (Radius).
  #
  # Arguments:
  #
  # Results:
  #
  proc  ChangeCubeValues { NTree  BaseWidth BaseDepth TopWidth TopDepth Heigth } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
     variable ScaleMinDistanceFactor
     variable ScaleMaxDistanceFactor

    set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
    PropertyPalette::ChangePropertyTitleValue $NTree "CubeBaseWidth" "Base Width" $BaseWidth "$ListOfElements"
    PropertyPalette::ChangePropertyTitleValue $NTree "CubeBaseDepth" "Base Depth" $TopDepth "$ListOfElements"
    PropertyPalette::ChangePropertyTitleValue $NTree "CubeTopWidth" "Top Width" $TopWidth "$ListOfElements"
    PropertyPalette::ChangePropertyTitleValue $NTree "CubeTopDepth" "Top Depth" $TopDepth "$ListOfElements"
    PropertyPalette::ChangePropertyTitleValue $NTree "CubeHeigth" "Heigth" $Heigth "$ListOfElements"
}

  # CSGSolidPyramidPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGSolidPyramidPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type Base Heigth Stacks Material } {
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScalePyrStacksMinimum 1
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 15 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    set PropertiesNTreeData [ list "$Parent" "Parameters" ]
    set TemporaryNTreeB [ TkNTree::Set "Parameters" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "PyramidBase" "Scale" [ list $Base "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Base Radius" ]
    set TemporaryNTreeC [ TkNTree::Set "Base Radius = $Base" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "PyramidHeigth" "Scale" [ list $Heigth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Heigth" ]
    set TemporaryNTreeC [ TkNTree::Set "Heigth = $Heigth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "PyramidStacks" "Scale" [ list $Stacks "$ScalePyrStacksMinimum $ScaleDistanceMaximum 1" ] "Stacks" ]
    set TemporaryNTreeC [ TkNTree::Set "Stacks = $Stacks" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SolidMaterial" "FileName" [ list "$Material" "Material mtr" ] "Material" ]
    set TemporaryNTreeC [ TkNTree::Set "Material = $Material" StateNone 24 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return $TemporaryNTreeA
  }

  # CSGSolidConePropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGSolidConePropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type Base Heigth Slices Stacks Material } {
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScalePolygonMaximum
    variable ScaleConeSlicesMinimum 4
    variable ScaleConeStacksMinimum 1
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 16 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    set PropertiesNTreeData [ list "$Parent" "Parameters" ]
    set TemporaryNTreeB [ TkNTree::Set "Parameters" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "ConeBase" "Scale" [ list $Base "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Base" ]
    set TemporaryNTreeC [ TkNTree::Set "Base = $Base" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "ConeHeigth" "Scale" [ list $Heigth "0 $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] "Heigth" ]
    set TemporaryNTreeC [ TkNTree::Set "Heigth = $Heigth" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "ConeSlices" "Scale" [ list $Slices "$ScaleConeSlicesMinimum $ScalePolygonMaximum 1" ] "Slices" ]
    set TemporaryNTreeC [ TkNTree::Set "Slices = $Slices" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "ConeStacks" "Scale" [ list $Stacks "$ScaleConeStacksMinimum $ScalePolygonMaximum 1" ] "Stacks" ]
    set TemporaryNTreeC [ TkNTree::Set "Stacks = $Stacks" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "SolidMaterial" "FileName" [ list "$Material" "Material mtr" ] "Material" ]
    set TemporaryNTreeC [ TkNTree::Set "Material = $Material" StateNone 24 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    return $TemporaryNTreeA
  }

  # ChangePyrConeValues procedure --
  #
  #		This procedure is used by any ObjectNavigator's NTree Node to
  #		change a pyramid or cone geometric values (Radius).
  #
  # Arguments:
  #
  # Results:
  #
  proc  ChangePyrConeValues { NTree Base Heigth SolidType } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor

    set ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]

    switch $SolidType {
      4 {
          PropertyPalette::ChangePropertyTitleValue $NTree "PyramidBase" "Base" $Base "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "PyramidHeigth" "Heigth" $Heigth "$ListOfElements"
        }
      5 {
          PropertyPalette::ChangePropertyTitleValue $NTree "ConeBase" "Base" $Base "$ListOfElements"
          PropertyPalette::ChangePropertyTitleValue $NTree "ConeHeigth" "Heigth" $Heigth "$ListOfElements"
        }
    }
  }

  # ParametricObjectPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricObjectPropertiesNTree { Parent Description vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 2 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }

  # ParametricRobotPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricRobotPropertiesNTree { Parent Description vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type BaseType } {
    variable LstRobotBaseTypes
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 3 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    # Parametric Robot Base Type
    set PropertiesNTreeData [ list "$Parent" "Base Type" "List" [ list [ lindex $LstRobotBaseTypes $BaseType ] $LstRobotBaseTypes ] "Base Type" ]
	set TemporaryNTreeB [ TkNTree::Set "Base Type = [ lindex $LstRobotBaseTypes $BaseType ]" StateCollapse 23 True True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }
  
#   CreateSerpenoidInputData --
#
#
# Arguments:
#
# Results:
#
  proc CreateSerpenoidInputData { {Type 1}  { Number 2}  { Ondulation 0}  {Periods 0} {Motion 0} {Velocity 0}  {Step 0}  {Time 0} } {
        variable ScalePointsMinimum
        variable ScalePointsMaximum
        variable ScalePointsFactor
        variable ScaleAngleDegreeMinimum
        variable ScaleAngleDegreeMaximum
        variable ScaleMinAngleFactor
        variable ScaleTimeMaximum
        variable ScaleTimeFactor
        
        if { $Type ==1} {
           set PropertyPalette::CurrentNode [ NTree::GetNextBrother [ NTree::GetFirstSon $PropertyPalette::CurrentNode ]  ]
           set Parent [ GetPropertyParent $PropertyPalette::CurrentNode ]          
        }
        if { $Type == 2} {
            set Parent [ GetPropertyParent $PropertyPalette::CurrentNode ]   
        }
        
        #set Parent [ GetPropertyParent $PropertyPalette::CurrentNode ]   
        # Collapse NTree to be changed.
        RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
        # Delete old sons.
        set TemporaryNTree [ NTree::GetFirstSon  $PropertyPalette::CurrentNode ]
        while { ! [ NTree::Empty $TemporaryNTree ] } {
           NTree::DisAssociate $TemporaryNTree
           NTree::Destroy $TemporaryNTree
           set TemporaryNTree [ NTree::GetFirstSon $PropertyPalette::CurrentNode ]
        }
        # Create new NTree.
        #Number of bodies
        set PropertiesNTreeData [ list "$Parent" "Number of bodies" "Scale" [ list $Number "$ScalePointsMinimum $ScalePointsMaximum $ScalePointsFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Number of bodies = $Number" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
        #Ondulation degree of the Curve
        set PropertiesNTreeData [ list "$Parent" "Ondulation degree" "Scale" [ list $Ondulation "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Ondulation degree = $Ondulation" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
        #Periods per lenght unit
        set PropertiesNTreeData [ list "$Parent" "Periods per lenght unit" "Scale" [ list $Periods "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Periods per lenght unit = $Periods" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
        #Serpentine motion bias
        set PropertiesNTreeData [ list "$Parent" "Serpentine motion bias" "Scale" [ list $Motion "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Serpentine motion bias = $Motion" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
       #Velocity Propagation
       set PropertiesNTreeData [ list "$Parent" "Velocity propagation" "Scale" [ list $Velocity "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Velocity propagation = $Velocity" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
        #Step time
        set PropertiesNTreeData [ list "$Parent" "Step time" "Scale" [ list $Step "0 $ScaleTimeMaximum $ScaleTimeFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Step time = $Step" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
        #Total time
        set PropertiesNTreeData [ list "$Parent" "Total time" "Scale" [ list $Time "0 $ScaleTimeMaximum $ScaleTimeFactor" ] ]
        set TemporaryNTreeB [ TkNTree::Set "Total time = $Time" StateNone 21 False False True $PropertiesNTreeData ]
        NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeB
                    
        RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
}

#   CreateSerpenoidInputData --
#
#
# Arguments:
#
# Results:
#
  proc DestroySerpenoidInputData { PrimaryIntType PrimaryMotionType PrimaryCtrlVar PrimaryInput1 PrimaryInput2 } {
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
    
    set Parent [ GetPropertyParent $PropertyPalette::CurrentNode ]
       
       # Collapse NTree to be changed.
       RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode

       # Delete old sons.
       set TemporaryNTree [ NTree::GetFirstSon $PropertyPalette::CurrentNode ]
       while { ! [ NTree::Empty $TemporaryNTree ] } {
            NTree::DisAssociate $TemporaryNTree
            NTree::Destroy $TemporaryNTree
            set TemporaryNTree [ NTree::GetFirstSon $PropertyPalette::CurrentNode ]
         }
         MessagesWindow::Puts "$PropertyPalette::CurrentNode\n"
         # Primary Motion Type
         if { ( $PrimaryIntType==0 ) || ( $PrimaryIntType== 1 ) || ( $PrimaryIntType== 2 ) || ( $PrimaryIntType== 3 ) } {
             set PropertiesNTreeData [ list "$Parent" "PrimaryMotionType" "List" [ list [ lindex $LstTransMotionTypes $PrimaryMotionType ] $LstTransMotionTypes ] "Motion Type" ]
             set TemporaryNTreeA [ TkNTree::Set "Motion Type = [ lindex $LstTransMotionTypes $PrimaryMotionType ]" StateExpand 22 False True True $PropertiesNTreeData ]
         } else {
             set PropertiesNTreeData [ list "$Parent" "PrimaryMotionType" "List" [ list [ lindex $LstSplinesTypes 0 ] $LstSplinesTypes ] "Motion Type" ]
             set TemporaryNTreeA [ TkNTree::Set "Motion Type = [ lindex $LstSplinesTypes 0 ]" StateExpand 22 False True True $PropertiesNTreeData ]
         }
         NTree::Add $PropertyPalette::CurrentNode $TemporaryNTreeA
         
         # Primary Control Variable
         switch $PrimaryMotionType {
            0 {
                    set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstVel $PrimaryCtrlVar ] $LstCtrlVarsConstVel ] "Control Variable" ]
                    set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstVel $PrimaryCtrlVar]" StateExpand 21 False True True $PropertiesNTreeData ]
                     NTree::Add $TemporaryNTreeA $TemporaryNTreeB
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
                         set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstAccel 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                         set tmpPrimaryCtrlVar 3
                     }
                     if { $PrimaryCtrlVar==3 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstAccel 1 ] $LstCtrlVarsConstAccel ] "Control Variable" ]
                        set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstAccel 1]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 4
                     }
                      NTree::Add $TemporaryNTreeA $TemporaryNTreeB
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                }
             2 {
                      if { $PrimaryCtrlVar==4 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 0 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 5
                      }
                      if { $PrimaryCtrlVar==5 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 1 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 6
                      }
                      NTree::Add $TemporaryNTreeA $TemporaryNTreeB
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                   }
                3 {
                      if { $PrimaryCtrlVar==4 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 0 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 0]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 5
                    }
                    if { $PrimaryCtrlVar==5 } {
                        set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 1 ] $LstCtrlVarsTrap ] "Control Variable" ]
                        set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                        set tmpPrimaryCtrlVar 6
                    }
                    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
                    set PrimaryCtrlVar $tmpPrimaryCtrlVar
                 }
             4 {
                      if { $PrimaryCtrlVar==4 } {
                         set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 0 ] $LstCtrlVarsTrap ] "Control Variable" ]
                         set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 0]" StateExpand 21 False True True  $PropertiesNTreeData ]
                         set tmpPrimaryCtrlVar 5
                      }
                      if { $PrimaryCtrlVar==5 } {
                         set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsTrap 1 ] $LstCtrlVarsTrap ] "Control Variable" ]
                         set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsTrap 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                         set tmpPrimaryCtrlVar 6
                      }
                      NTree::Add $TemporaryNTreeA $TemporaryNTreeB
                      set PrimaryCtrlVar $tmpPrimaryCtrlVar
                }
             5 {
                      if { $PrimaryIntType==7} {
                         if { $PrimaryCtrlVar==6 } {
                           set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsBez 0 ] $LstCtrlVarsBez ] "Control Variable" ]
                           set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsBez 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                           set tmpPrimaryCtrlVar 8
                         }
                         if { $PrimaryCtrlVar==7 } {
                           set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsBez 1 ] $LstCtrlVarsBez ] "Control Variable" ]
                           set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsBez 1 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                           set tmpPrimaryCtrlVar 8
                          }
                    } else {
                         set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsVar 0 ] $LstCtrlVarsVar ] "Control Variable" ]
                         set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsVar 0 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                         if { $PrimaryCtrlVar==6 } {
                           set tmpPrimaryCtrlVar 7
                         }
                     }
                     NTree::Add $TemporaryNTreeA $TemporaryNTreeB
                     set PrimaryCtrlVar $tmpPrimaryCtrlVar
                   }
                6 {
                      set PropertiesNTreeData [ list "$Parent" "PrimaryCtrlVar" "List" [ list [ lindex $LstCtrlVarsConstVel 2 ] $LstCtrlVarsConstVel ] "Control Variable" ]
                      set TemporaryNTreeB [ TkNTree::Set "Control Variable = [ lindex  $LstCtrlVarsConstVel 2 ]" StateExpand 21 False True True $PropertiesNTreeData ]
                      NTree::Add $TemporaryNTreeA $TemporaryNTreeB
                      set PrimaryCtrlVar 0
                     }
                }

         # Add number of inputs depending on Control Var
         PrimaryCtrlVarInputsNTree $Parent $TemporaryNTreeB $PrimaryCtrlVar $PrimaryInput1 $PrimaryInput2
         # Expand new NTree.
         RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
                  
}

  # ParametricTrajectoryPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricTrajectoryPropertiesNTree { Parent Description vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 33 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }

 #  ChangeGenModePropertiesNTree procedure.
 #
 # 		This procedure changes an Object Generation Mode Properties Ntree Node
 # 					0. Automatic.
 # 					1. User Defined.
 #
 # Arguments:
 #
 # Results:
 #
 proc ChangeGenModePropertiesNTree { GenMode Mass Volume Density CMx CMy CMz lxx lyy lzz lxy lyz lxz } {
    variable cvsON
    variable frmONTB
    # Take care with IOIC because this choise is based on the current node selecction
    set GenModeNTree $PropertyPalette::CurrentNode
    set Parent [ GetPropertyParent $GenModeNTree ]
	
    # Collapse NTree to be changed.
    RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode

    # Delete old sons.
    set TemporaryNTree [ NTree::GetFirstSon $GenModeNTree ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      NTree::DisAssociate $TemporaryNTree
      NTree::Destroy $TemporaryNTree
      set TemporaryNTree [ NTree::GetFirstSon $GenModeNTree ]
    }

    # Create new Properties NTree.
    GenModePropertiesNTree $Parent $GenModeNTree $GenMode $Mass $Volume $Density $CMx $CMy $CMz $lxx $lyy $lzz $lxy $lyz $lxz 

	# Expand new NTree.
    RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
	 
 }
 
 #  GenModePropertiesNTree procedure --
  #
  # 		This procedure draws the GenMode Properties NTree according to the value
  # 		passed by the NTree ChangeGenModePropertiesNTree procedure.
  #
  # Arguments:
  #
  # Results:
  #
  proc GenModePropertiesNTree { Parent GenModeNTree GenMode MassValue Volume Density CMx CMy CMz lxx lyy lzz lxy lyz lxz } {
    variable ScaleMassPropMinimum
	variable ScaleCenterMassMinimum
    variable ScaleCenterMassMaximum
    variable ScaleMassPropMaximum
	variable ScaleDensityMaximum
    variable ScaleMassPropFactor
	switch $GenMode {
          0 {
					#Automatic
				    # Mass
					set PropertiesNTreeData [ list "$Parent" "Mass" "Scale" [ list $MassValue "$ScaleMassPropMinimum $ScaleMassPropMaximum $ScaleMassPropFactor" ] ]
                    set TemporaryNTreeD [ TkNTree::Set "Mass = $MassValue" StateNone 21 False False False $PropertiesNTreeData ]
                    NTree::Add $GenModeNTree $TemporaryNTreeD
					# Density
					set PropertiesNTreeData [ list "$Parent" "Density" "Number" [ list $Density ] ]
					set TemporaryNTreeD [ TkNTree::Set "Density = $Density" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD 
					# CMxvariable ScaleCenterMassMinimum
					set PropertiesNTreeData [ list "$Parent" "CMx" "Number" [ list $CMx ] ]
					set TemporaryNTreeD [ TkNTree::Set "CMx = $CMx" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# CMy
					set PropertiesNTreeData [ list "$Parent" "CMy" "Number" [ list $CMy ] ]
					set TemporaryNTreeD [ TkNTree::Set "CMy = $CMy" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# CMz
					set PropertiesNTreeData [ list "$Parent" "CMz" "Number" [ list $CMz ] ]
					set TemporaryNTreeD [ TkNTree::Set "CMz = $CMz" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lxx
					set PropertiesNTreeData [ list "$Parent" "lxx" "Number" [ list $lxx ] ]
					set TemporaryNTreeD [ TkNTree::Set "lxx = $lxx" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lyy
					set PropertiesNTreeData [ list "$Parent" "lyy" "Number" [ list $lyy ] ]
					set TemporaryNTreeD [ TkNTree::Set "lyy = $lyy" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lzz
					set PropertiesNTreeData [ list "$Parent" "lzz" "Number" [ list $lzz ] ]
					set TemporaryNTreeD [ TkNTree::Set "lzz = $lzz" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lxy
					set PropertiesNTreeData [ list "$Parent" "lxy" "Number" [ list $lxy ] ]
					set TemporaryNTreeD [ TkNTree::Set "lxy = $lxy" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lyz
					set PropertiesNTreeData [ list "$Parent" "lyz" "Number" [ list $lyz ] ]
					set TemporaryNTreeD [ TkNTree::Set "lyz = $lyz" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lxz
					set PropertiesNTreeData [ list "$Parent" "lxz" "Number" [ list $lxz ] ]
					set TemporaryNTreeD [ TkNTree::Set "lxz = $lxz" StateNone 21 False False False $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
		      }
		 1 {
					 #User Mode
					 # Mass
			        set PropertiesNTreeData [ list "$Parent" "MassMod" "Scale" [ list $MassValue "$ScaleMassPropMinimum $ScaleMassPropMaximum $ScaleMassPropFactor" ] "Mass" ]
                    set TemporaryNTreeD [ TkNTree::Set "Mass = $MassValue" StateNone 21 False False True $PropertiesNTreeData ]
                    NTree::Add $GenModeNTree $TemporaryNTreeD
					# Density
			       set PropertiesNTreeData [ list "$Parent" "DensityMod" "Scale" [ list $Density "$ScaleMassPropMinimum $ScaleDensityMaximum $ScaleMassPropFactor" ] "Density" ]
					set TemporaryNTreeD [ TkNTree::Set "Density = $Density" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD 
					# CMx
			        set PropertiesNTreeData [ list "$Parent" "CMxMod" "Scale" [ list $CMx "$ScaleCenterMassMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "CMx" ]
					set TemporaryNTreeD [ TkNTree::Set "CMx = $CMx" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# CMy
					set PropertiesNTreeData [ list "$Parent" "CMyMod" "Scale" [ list $CMy "$ScaleCenterMassMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "CMy" ]
					set TemporaryNTreeD [ TkNTree::Set "CMy = $CMy" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# CMz
					set PropertiesNTreeData [ list "$Parent" "CMzMod" "Scale" [ list $CMz "$ScaleCenterMassMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "CMz" ]
					set TemporaryNTreeD [ TkNTree::Set "CMz = $CMz" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lxx
					set PropertiesNTreeData [ list "$Parent" "lxxMod" "Scale" [ list $lxx "$ScaleCenterMassMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "lxx" ]
					set TemporaryNTreeD [ TkNTree::Set "lxx = $lxx" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lyy
					set PropertiesNTreeData [ list "$Parent" "lyyMod" "Scale" [ list $lyy "$ScaleCenterMassMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "lyy" ]
					set TemporaryNTreeD [ TkNTree::Set "lyy = $lyy" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lzz
					set PropertiesNTreeData [ list "$Parent" "lzzMod" "Scale" [ list $lzz "$ScaleCenterMassMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "lzz" ]
					set TemporaryNTreeD [ TkNTree::Set "lzz = $lzz" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lxy
					set PropertiesNTreeData [ list "$Parent" "lxyMod" "Scale" [ list $lxy "$ScaleMassPropMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "lxy" ]
					set TemporaryNTreeD [ TkNTree::Set "lxy = $lxy" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lyz
					set PropertiesNTreeData [ list "$Parent" "lyzMod" "Scale" [ list $lyz "$ScaleMassPropMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "lyz" ]
					set TemporaryNTreeD [ TkNTree::Set "lyz = $lyz" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
					# lxz
					set PropertiesNTreeData [ list "$Parent" "lxzMod" "Scale" [ list $lxz "$ScaleMassPropMinimum $ScaleCenterMassMaximum $ScaleMassPropFactor" ] "lxz" ]
					set TemporaryNTreeD [ TkNTree::Set "lxz = $lxz" StateNone 21 False False True $PropertiesNTreeData ]
					NTree::Add $GenModeNTree $TemporaryNTreeD
		      }
		}
  }
  
  
  # ObjectsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ObjectsPropertiesNTree { Parent Description GenMode Mass Volume Density CMx CMy CMz lxx lyy lzz lxy lyz lxz } {
		variable LstMassGenMode
	  
		# Description
		set PropertiesNTreeData [ list "$Parent" "Properties" ]
		set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 2 False False Undefined $PropertiesNTreeData ]
		NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
	 
		# Mass Properties
		set PropertiesNTreeData [ list "$Parent" "Mass Properties" ]
		set TemporaryNTreeB [ TkNTree::Set "Mass Properties" StateExpand 23 False False Undefined $PropertiesNTreeData ]
		NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	
		# Generation Mode
		set PropertiesNTreeData [ list "$Parent" "Generation Mode" "List" [ list [ lindex $LstMassGenMode $GenMode ] $LstMassGenMode ] "Generation Mode" ]
		set TemporaryNTreeC [ TkNTree::Set "Generation Mode = [ lindex $LstMassGenMode $GenMode ]" StateExpand 23 False True True $PropertiesNTreeData ]
		NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	
	    GenModePropertiesNTree $Parent $TemporaryNTreeC $GenMode $Mass $Volume $Density $CMx $CMy $CMz $lxx $lyy $lzz $lxy $lyz $lxz 
		return $TemporaryNTreeA
  }

  # ElementsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ElementsPropertiesNTree { Parent GenMode vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type Mass Volume Density CMx CMy CMz lxx lyy lzz lxy lyz lxz } {
    variable LstMassGenMode
	# Properties
		set PropertiesNTreeData [ list "$Parent" "Properties" ]
		set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 7 False False Undefined $PropertiesNTreeData ]
		NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    # Mass Properties
		set PropertiesNTreeData [ list "$Parent" "Mass Properties" ]
		set TemporaryNTreeB [ TkNTree::Set "Mass Properties" StateExpand 23 False False Undefined $PropertiesNTreeData ]
		NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	# Generation Mode
		set PropertiesNTreeData [ list "$Parent" "Generation Mode" "List" [ list [ lindex $LstMassGenMode $GenMode ] $LstMassGenMode ] "Generation Mode" ]
		set TemporaryNTreeC [ TkNTree::Set "Generation Mode = [ lindex $LstMassGenMode $GenMode ]" StateExpand 23 False True True $PropertiesNTreeData ]
		NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	
	# Mass Properties NTree
		GenModePropertiesNTree $Parent $TemporaryNTreeC $GenMode $Mass $Volume $Density $CMx $CMy $CMz $lxx $lyy $lzz $lxy $lyz $lxz 
	
	NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }

  # RobotsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc RobotsPropertiesNTree { Parent Description vPO1B vPO2B vPO3B vPO4B vPO5B vPO6B PTypeB OTypeB TypeB RobotType } {
    variable LstRobotTypes
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 3 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    # Robot Type
    set PropertiesNTreeData [ list "$Parent" "Robot Type" "List" [ list [ lindex $LstRobotTypes $RobotType ] $LstRobotTypes ] "Robot Type" ]
	set TemporaryNTreeB [ TkNTree::Set "Robot Type = [ lindex $LstRobotTypes $RobotType ]" StateNone 23 False True True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    #Base
    set PropertiesNTreeData [ list "$Parent" "Base" ]
    set TemporaryNTreeB [ TkNTree::Set "Base" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    #Position and Orientation
    NTree::Add $TemporaryNTreeB [ PositionOrientationsPropertiesNTree "$Parent" $vPO1B $vPO2B $vPO3B $vPO4B $vPO5B $vPO6B $PTypeB $OTypeB $TypeB ]
    #Visible
    NTree::Add $TemporaryNTreeB [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }

  # LinksPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc LinksPropertiesNTree { Parent Alpha a Theta d Minimum Maximum Type Origin Nest Jm G B TcP TcN } {
    variable ScaleDistanceMinimum
    variable ScaleDistanceMaximum
    variable ScaleAngleDegreeMinimum
    variable ScaleAngleDegreeMaximum
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleMinAngleFactor
    variable ScaleMaxAngleFactor
	variable ScaleMotorMinimum
	variable ScaleMotorMaximum
	variable ScaleMotorFactor
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 4 False False Undefined $PropertiesNTreeData ]
    set PropertiesNTreeData [ list "$Parent" "Denavit-Hartenberg" ]
	#Denavit-Hartenberg Parameters
    set TemporaryNTreeB [ TkNTree::Set "Denavit-Hartenberg" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    set PropertiesNTreeData [ list "$Parent" "Alpha" "Scale" [ list $Alpha "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Alpha = $Alpha" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "a" "Scale" [ list $a "$ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "a = $a" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    switch $Type {
        "Revolute" {
              set PropertiesNTreeData [ list "$Parent" "Theta" "Scale" [ list $Theta "$Minimum $Maximum $ScaleMinAngleFactor" ] ]
              set TemporaryNTreeC [ TkNTree::Set "Theta = $Theta" StateNone 21 False False True $PropertiesNTreeData ]
              NTree::Add $TemporaryNTreeB $TemporaryNTreeC
              set PropertiesNTreeData [ list "$Parent" "d" "Scale" [ list $d "$ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor" ] ]
              set TemporaryNTreeC [ TkNTree::Set "d = $d" StateNone 21 False False True $PropertiesNTreeData ]
              NTree::Add $TemporaryNTreeB $TemporaryNTreeC
        }
        "Prismatic" {
              set PropertiesNTreeData [ list "$Parent" "Theta" "Scale" [ list $Theta "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
              set TemporaryNTreeC [ TkNTree::Set "Theta = $Theta" StateNone 21 False False True $PropertiesNTreeData ]
              NTree::Add $TemporaryNTreeB $TemporaryNTreeC
              set PropertiesNTreeData [ list "$Parent" "d" "Scale" [ list $d "$Minimum $Maximum $ScaleMinDistanceFactor" ] ]
              set TemporaryNTreeC [ TkNTree::Set "d = $d" StateNone 21 False False True $PropertiesNTreeData ]
              NTree::Add $TemporaryNTreeB $TemporaryNTreeC
         }  
    }     
    
    set PropertiesNTreeData [ list "$Parent" "Minimum" "Scale" [ list $Minimum "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Minimum = $Minimum" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "Maximum" "Scale" [ list $Maximum "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Maximum = $Maximum" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "Type" "List" [ list $Type { Revolute Prismatic } ] ]
    set TemporaryNTreeC [ TkNTree::Set "Type = $Type" StateNone 22 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "Origin" "Scale" [ list $Origin "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Origin = $Origin" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    set PropertiesNTreeData [ list "$Parent" "Nest" "Scale" [ list $Nest "$ScaleAngleDegreeMinimum $ScaleAngleDegreeMaximum $ScaleMinAngleFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Nest = $Nest" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	
	#Motor Dynamic Parameters
    set TemporaryNTreeB [ TkNTree::Set "Motor Dynamic Parameters" StateExpand 23 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	#Jm
	set PropertiesNTreeData [ list "$Parent" "Jm" "Scale" [ list $Jm "$ScaleMotorMinimum $ScaleMotorMaximum $ScaleMotorFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Jm = $Jm" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	#G
	set PropertiesNTreeData [ list "$Parent" "G" "Scale" [ list $G "$ScaleMotorMinimum $ScaleMotorMaximum $ScaleMotorFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "G = $G" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	#B
	set PropertiesNTreeData [ list "$Parent" "B" "Scale" [ list $B "$ScaleMotorMinimum $ScaleMotorMaximum $ScaleMotorFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "B = $B" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	#Tc+
	set PropertiesNTreeData [ list "$Parent" "Tc+" "Scale" [ list $TcP "$ScaleMotorMinimum $ScaleMotorMaximum $ScaleMotorFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Tc+ = $TcP" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
	#Tc-
	set PropertiesNTreeData [ list "$Parent" "Tc-" "Scale" [ list $TcN "$ScaleMotorMinimum $ScaleMotorMaximum $ScaleMotorFactor" ] ]
    set TemporaryNTreeC [ TkNTree::Set "Tc- = $TcN" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeB $TemporaryNTreeC
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }
  
  # ChangeLinkThetaRange procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeLinkThetaRange { Minimum Maximum Range } {
    variable ScaleMinAngleFactor
    set ListOfElements [ list $Minimum $Maximum $ScaleMinAngleFactor ]
    #MessagesWindow::Puts "$ListOfElements Estoy Aqui 2 \n"
    switch $Range {
        "Minimum" {
                set TemporaryNTree [ NTree::GetPreviousBrother $PropertyPalette::CurrentNode ]
                set TemporaryNTree [ NTree::GetPreviousBrother $TemporaryNTree ]
        }
        "Maximum" {
                set TemporaryNTree [ NTree::GetPreviousBrother $PropertyPalette::CurrentNode ]
                set TemporaryNTree [ NTree::GetPreviousBrother $TemporaryNTree ]
                set TemporaryNTree [ NTree::GetPreviousBrother $TemporaryNTree ]
        } 
    }
    SetPropertyListOfElements $TemporaryNTree $ListOfElements
    #MessagesWindow::Puts "$TemporaryNTree Estoy Aqui 3\n"
 }
 
  # ChangeLinkThetaRange procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeLinkDRange { Minimum Maximum Range } {
    variable ScaleMinAngleFactor
    set ListOfElements [ list $Minimum $Maximum $ScaleMinAngleFactor ]
    #MessagesWindow::Puts "$ListOfElements Estoy Aqui 2 \n"
    switch $Range {
        "Minimum" {
                set TemporaryNTree [ NTree::GetPreviousBrother $PropertyPalette::CurrentNode ]
         }
        "Maximum" {
                set TemporaryNTree [ NTree::GetPreviousBrother $PropertyPalette::CurrentNode ]
                set TemporaryNTree [ NTree::GetPreviousBrother $TemporaryNTree ]
         } 
    }
    SetPropertyListOfElements $TemporaryNTree $ListOfElements
    #MessagesWindow::Puts "$TemporaryNTree Estoy Aqui 3\n"
  }

  # TargetsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc TargetsPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 29 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    NTree::Add $TemporaryNTreeA [ GraphicsPropertiesNTree "$Parent" True ]
    return $TemporaryNTreeA
  }
  
# SixDimForcePropertiesNTree procedure --
#
#
#
#
# Arguments:
#
# Results:
#
  proc SixDimForcePropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 } { 
	variable ScaleDistanceMinimum
	variable ScaleDistanceMaximum
    	variable ScaleMinDistanceFactor
      variable ScaleAngleDegreeMinimum
    	variable ScaleAngleDegreeMaximum
      variable ScaleMinAngleFactor
      
      set ScaleAngleDegreeMinimum1 [ expr { $ScaleAngleDegreeMinimum/2 } ]
    	set ScaleAngleDegreeMaximum1 [ expr { $ScaleAngleDegreeMaximum/2 } ]
      
	set vPO1Name "X"
      set vPO1ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
      set vPO2Name "Y"
      set vPO2ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
      set vPO3Name "Z"
      set vPO3ListOfElements [ list $ScaleDistanceMinimum $ScaleDistanceMaximum $ScaleMinDistanceFactor ]
      set vPO4Name "Roll"
      set vPO5Name "Pitch"
      set vPO6Name "Yaw"
          
    	set PropertiesNTreeData [ list "$Parent" "Properties" ]
    	set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 21 False False Undefined $PropertiesNTreeData ]
	# Roll
    	set PropertiesNTreeData [ list "$Parent" "vPO4" "Scale" [ list $vPO4 "$ScaleAngleDegreeMinimum1 $ScaleAngleDegreeMaximum1 $ScaleMinAngleFactor" ] "$vPO4Name" ]
    	set TemporaryNTreeB [ TkNTree::Set "$vPO4Name = $vPO4" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	# Pitch
    	set PropertiesNTreeData [ list "$Parent" "vPO5" "Scale" [ list $vPO5 "$ScaleAngleDegreeMinimum1 $ScaleAngleDegreeMaximum1 $ScaleMinAngleFactor" ] "$vPO5Name" ]
    	set TemporaryNTreeB [ TkNTree::Set "$vPO5Name = $vPO5" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	# Yaw
    	set PropertiesNTreeData [ list "$Parent" "vPO6" "Scale" [ list $vPO6 "$ScaleAngleDegreeMinimum1 $ScaleAngleDegreeMaximum1 $ScaleMinAngleFactor" ] "$vPO6Name" ]
    	set TemporaryNTreeB [ TkNTree::Set "$vPO6Name = $vPO6" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	# X
    	set PropertiesNTreeData [ list "$Parent" "vPO1" "Scale" [ list $vPO1 "$vPO1ListOfElements" ] "$vPO1Name" ]
    	set TemporaryNTreeB [ TkNTree::Set "$vPO1Name = $vPO1" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	# Y
    	set PropertiesNTreeData [ list "$Parent" "vPO2" "Scale" [ list $vPO2 "$vPO2ListOfElements" ] "$vPO2Name" ]
    	set TemporaryNTreeB [ TkNTree::Set "$vPO2Name = $vPO2" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	# Z
    	set PropertiesNTreeData [ list "$Parent" "vPO3" "Scale" [ list $vPO3 "$vPO3ListOfElements" ] "$vPO3Name" ]
    	set TemporaryNTreeB [ TkNTree::Set "$vPO3Name = $vPO3" StateNone 21 False False True $PropertiesNTreeData ]
    	NTree::Add $TemporaryNTreeA $TemporaryNTreeB
	
	return $TemporaryNTreeA
}

  # TeachPendantsLinksPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc TeachPendantsLinksPropertiesNTree { Parent Value Minimum Maximum Type } {
    variable ScaleMinDistanceFactor
    variable ScaleMaxDistanceFactor
    variable ScaleMinAngleFactor
    variable ScaleMaxAngleFactor
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 4 False False Undefined $PropertiesNTreeData ]
    if { $Type == "Revolute" } {
      set Variable "Theta"
      set Factor $ScaleMinAngleFactor
    } else {
      set Variable "d"
      set Factor $ScaleMinDistanceFactor
    }
    set PropertiesNTreeData [ list "$Parent" "$Variable" "Scale" [ list $Value "$Minimum $Maximum $Factor" ] ]
    set TemporaryNTreeB [ TkNTree::Set "$Variable = $Value" StateNone 21 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }

  # ToolTransformsPropertiesNTree procedure --
  #
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ToolTransformsPropertiesNTree { Parent vPO1 vPO2 vPO3 vPO4 vPO5 vPO6 PType OType Type } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 32 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ PositionOrientationsPropertiesNTree "$Parent" $vPO1 $vPO2 $vPO3 $vPO4 $vPO5 $vPO6 $PType $OType $Type ]
    NTree::Add $TemporaryNTreeA [  FollowTargetPropertiesNTree "$Parent" ]
    return $TemporaryNTreeA
  }

 #  GNUPlot PropertyPallete NTree procedure --
 #
 # 		The following procedure is used to create a GNUPlot
 # 		Properties NTree.
 #
 # Arguments:
 #
 # Results:
 #
  proc GNUPlotPropertiesNTree { Parent Title } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 35 False False Undefined $PropertiesNTreeData ]
    # Title
    set PropertiesNTreeData [ list "$Parent" "Title" "Text" [ list "$Title" ] "$NTree::Null" ]
    set TemporaryNTreeB [ TkNTree::Set "Title = $Title" StateNone 20 False False True $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA $TemporaryNTreeB
    return $TemporaryNTreeA
  }

 # World PropertyPallete NTree procedure --
 #
 # 		The following procedure is used to create a Worlds
 # 		Properties NTree Node.
 #
 # Arguments:
 #
 # Results:
 #
  proc WorldsPropertiesNTree { Parent Description } {
    set PropertiesNTreeData [ list "$Parent" "Properties" ]
    set TemporaryNTreeA [ TkNTree::Set "Properties" StateCollapse 1 False False Undefined $PropertiesNTreeData ]
    NTree::Add $TemporaryNTreeA [ GeneralsPropertiesNTree "$Parent" "$Description" ]
    return $TemporaryNTreeA
  }
