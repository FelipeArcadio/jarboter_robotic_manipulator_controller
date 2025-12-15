#----------------------------------------------------------------------------------------------------------------------------------
# ObjNavMain.tcl
# 		This file implements the TCL code for creating and managing the Object Navigator and
#		the Properties Palette windows for the ROBOMOSP application.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
#		       Andrés Jaramillo Botero, ajaramil@puj.edu.co
#		       Wilber Perea Castro, wpcmmx@hotmail.com
#                     Antonio Alejandro Matta Gómez amatta@puj.edu.co
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2004-05-19
#
# Requirements:   ObjNavPropPal.tcl
#                         ObjNavNodeBuilder.tcl
#                         ObjNavTjc.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# -------------------------------------------------------------------------
#  Description of the Object Navigator data structure
# -------------------------------------------------------------------------
    # TkNTree data structure:
    #<0.Title>
    #<1.State>
    #<2.Icon>
    #<3.InsertAllowed>
    #<4.DeleteAllowed>
    #<5.UpdateAllowed>
    #<6.Data>

    # Object Navigator Data:
    #<0.ObjectType>           : kind of object to be managed according to a component
    #                           (described below).
    #<1.PropertiesNTree>      : properties of the elements to be managed by the
    #                           PropertyPalette component (described below).
    #<2.FileName Or SocketId> : node's associated file name or socket id if it is using
    #                           sockets.

    # 1. PropertiesNTree:
    #<0.Parent>         : Command's name to be executed (i.e. Node48).
    #<1.Name>           : Property name to be modified.
    #<2.Type>           : Property's type.
    #                     It can be:
    #                       0. Text
    #                       1. FileName
    #                       2. Number
    #                       3. Scale
    #                       4. List
    #                       5. Spin
    #<3.ValueInfo>      : Information of the stored data. This can be:
    #                       0.Value
    #                       1.ListOfElements
    #<4.SecondaryTitle> : It is used to give a new title to a property when there
    #                     are several properties with the same name in the same set.

  # 0. Object Types for the Object Navigator data:
  # Universe
  # -- Configurations
  #   -- LightingsTitle
  #   -- Camera
  #   -- Materials
  #     -- MaterialsTitle

  # -- Materials
  #   -- MaterialsTitle

  # -- CSGs
  #   -- CSGsTitle
  #     -- CSGsOperatorTitle || CSGsSolidTitle

  # -- Objects
  #   -- ObjectsTitle
  #     -- ObjectsElementsNTree
  #       -- ObjectsElementsTitle
  #         -- ObjectsElementsSolid
  #           -- ObjectsElementsSolidsTitle
  #         -- ObjectsElementsSons

  # -- Robots
  #   -- RobotsTitle
  #     -- RobotsBasesObjects
  #       -- RobotsBasesObjectsTitle
  #     -- RobotsLinksNTree
  #       -- RobotsLinksTitle
  #         -- RobotsLinksObjects
  #           -- RobotsLinksObjectsTitle
  #         -- RobotsLinksSons

  # -- Trajectorys
  #   -- TrajectorysTitle
  #     -- TrajectorysSection
  #       -- TrajectorysControlPoint

  # -- Worlds
  #   -- WorldsTitle
  #     -- WorldsTarget
  #     -- WorldsObjects
  #       -- WorldsObjectsTitle
  #     -- WorldsRobots
  #       -- WorldsRobotsTitle
  #         -- WorldsRobotsBase
  #         -- WorldsRobotsTeachPendant
  #           -- WorldsRobotsTeachPendantsLink
  #         -- WorldsRobotsToolTransform
  #     -- WorldsTrajectorys
  #       -- WorldsTrajectorysTitle
  #         -- WorldsTrajectorysPoints
  # -- GNUPlot
  #  -- InverseDynamics
  #    -- Torques
  #     -- ABI

  # Initialization of Tk objects

  # Initialization of the graphic images (icons) for this window.

  # Images of the Object Navigator canvas
  image create photo  imgCVSIcon0 -file ./Images/ObjectNavigator/Icon00.gif
  image create photo  imgCVSIcon1 -file ./Images/ObjectNavigator/Icon01.gif
  image create photo  imgCVSIcon2 -file ./Images/ObjectNavigator/Icon02.gif
  image create photo  imgCVSIcon3 -file ./Images/ObjectNavigator/Icon03.gif
  image create photo  imgCVSIcon4 -file ./Images/ObjectNavigator/Icon04.gif
  image create photo  imgCVSIcon5 -file ./Images/ObjectNavigator/Icon05.gif
  image create photo  imgCVSIcon6 -file ./Images/ObjectNavigator/Icon06.gif
  image create photo  imgCVSIcon7 -file ./Images/ObjectNavigator/Icon07.gif
  image create photo  imgCVSIcon8 -file ./Images/ObjectNavigator/Icon08.gif
  image create photo  imgCVSIcon9 -file ./Images/ObjectNavigator/Icon09.gif
  image create photo imgCVSIcon10 -file ./Images/ObjectNavigator/Icon10.gif
  image create photo imgCVSIcon11 -file ./Images/ObjectNavigator/Icon11.gif
  image create photo imgCVSIcon12 -file ./Images/ObjectNavigator/Icon12.gif
  image create photo imgCVSIcon13 -file ./Images/ObjectNavigator/Icon13.gif
  image create photo imgCVSIcon14 -file ./Images/ObjectNavigator/Icon14.gif
  image create photo imgCVSIcon15 -file ./Images/ObjectNavigator/Icon15.gif
  image create photo imgCVSIcon16 -file ./Images/ObjectNavigator/Icon16.gif
  image create photo imgCVSIcon17 -file ./Images/ObjectNavigator/Icon17.gif
  image create photo imgCVSIcon18 -file ./Images/ObjectNavigator/Icon18.gif
  image create photo imgCVSIcon19 -file ./Images/ObjectNavigator/Icon19.gif
  image create photo imgCVSIcon29 -file ./Images/ObjectNavigator/Icon29.gif
  image create photo imgCVSIcon30 -file ./Images/ObjectNavigator/Icon30.gif
  image create photo imgCVSIcon31 -file ./Images/ObjectNavigator/Icon31.gif
  image create photo imgCVSIcon32 -file ./Images/ObjectNavigator/Icon32.gif
  image create photo imgCVSIcon33 -file ./Images/ObjectNavigator/Icon33.gif
  image create photo imgCVSIcon34 -file ./Images/ObjectNavigator/Icon34.gif
  image create photo imgCVSIcon36 -file ./Images/ObjectNavigator/Icon36.gif
  image create photo imgCVSIcon37 -file ./Images/ObjectNavigator/Icon37.gif
  image create photo imgCVSIcon38 -file ./Images/ObjectNavigator/Icon38.gif

  #Images of the Object Navigator ToolBar
  image create photo imgONTBOpen -file ./Images/ObjectNavigatorToolBar/Open.gif
  image create photo imgONTBSave -file ./Images/ObjectNavigatorToolBar/Save.gif
  image create photo imgONTBSaveAs -file ./Images/ObjectNavigatorToolBar/SaveAs.gif
  image create photo imgONTBReload -file ./Images/ObjectNavigatorToolBar/Reload.gif
  image create photo imgONTBEdit -file ./Images/ObjectNavigatorToolBar/Edit.gif
  image create photo imgONTBCreate -file ./Images/ObjectNavigatorToolBar/Create.gif
  image create photo imgONTBDelete -file ./Images/ObjectNavigatorToolBar/Delete.gif
  image create photo imgONTBExpand -file ./Images/ObjectNavigatorToolBar/Expand.gif
  image create photo imgONTBCollapse -file ./Images/ObjectNavigatorToolBar/Collapse.gif
  image create photo imgONTBExpandAll -file ./Images/ObjectNavigatorToolBar/ExpandAll.gif
  image create photo imgONTBCollapseAll -file ./Images/ObjectNavigatorToolBar/CollapseAll.gif
  image create photo imgONTBMemory -file ./Images/ObjectNavigatorToolBar/Memory.gif

  # Images of the CSG ToolBar
  image create photo imgCSGTBUnion -file ./Images/CSGToolBar/Union.gif
  image create photo imgCSGTBIntersection -file ./Images/CSGToolBar/Intersection.gif
  image create photo imgCSGTBDifference -file ./Images/CSGToolBar/Difference.gif
  image create photo imgCSGTBSphere -file ./Images/CSGToolBar/Sphere.gif
  image create photo imgCSGTBCylinder -file ./Images/CSGToolBar/Cylinder.gif
  image create photo imgCSGTBCube -file ./Images/CSGToolBar/Cube.gif
  image create photo imgCSGTBPyramid -file ./Images/CSGToolBar/Pyramid.gif
  image create photo imgCSGTBCone -file ./Images/CSGToolBar/Cone.gif

namespace eval ObjectNavigator {

  # Files that are also needed by this namespace
  source ./Tcl/ObjectNavigator/ObjNavPropPal.tcl
  source ./Tcl/ObjectNavigator/ObjNavNodeBuilder.tcl
  source ./Tcl/ObjectNavigator/ObjNavTjc.tcl

  # Initialization of global variables
  variable tplON .wndObjectNavigator                # Top level ObjectNavigator window.
  variable cvsON $tplON.frmON_A.cvsObjectNavigator  # Main ObjectNavigator canvas.
  variable frmONTB $tplON.frmToolBar                # ObjectNavigator ToolBar frame.
  variable frmCSGA $tplON.frmON_A.frmCSG_A          # CSG frame A.
  variable frmCSGTB $frmCSGA.frmCSGToolBar          # CSG ToolBar frame.
  variable CSGToolBar                               # CSG ToolBar.

  # Variables that initializes and stores the Id name for each one of the
  # application's node. All of them are NTrees.
  variable ApplicationNTree $NTree::Null
  variable CurrentNode $NTree::Null
  variable tmpCurrentNode $NTree::Null
  variable ConfigurationsNode $NTree::Null
  variable CfgMaterialsNode $NTree::Null
  variable CSGsNode $NTree::Null
  variable ObjectsNode $NTree::Null
  variable RobotsNode $NTree::Null
  variable TrajectorysNode $NTree::Null
  variable WorldsNode $NTree::Null
  variable GNUPlotNode $NTree::Null

  # Default directories where components filenames are stored.
  variable CurrentOpenSaveDirectoryMaterials $HOME/Saves/Materials
  variable CurrentOpenSaveDirectoryCSGs $HOME/Saves/CSGs
  variable CurrentOpenSaveDirectoryObjects $HOME/Saves/Objects
  variable CurrentOpenSaveDirectoryRobots $HOME/Saves/Robots
  variable CurrentOpenSaveDirectoryTrajectorys $HOME/Saves/Trajectories
  variable CurrentOpenSaveDirectoryTrajectorysSections $HOME/Saves/Trajectories/Sections
  variable CurrentOpenSaveDirectorySections $HOME/Saves/Trajectories/Sections
  variable CurrentOpenSaveDirectoryTrajectorysGNUPlotDatas $HOME/Saves/Trajectories/GNUPlotData
  variable CurrentOpenSaveDirectoryWorlds $HOME/Saves/Worlds
  variable CurrentOpenSaveDirectoryInvKineGNUPlotDatas $HOME/Saves/GNUPlot/InverseKinematics  
  variable CurrentOpenSaveDirectoryFwdDynGNUPlotDatas $HOME/Saves/GNUPlot/ForwardDynamics  
  variable CurrentOpenSaveDirectoryInvDynGNUPlotDatas $HOME/Saves/GNUPlot/InverseDynamics
  variable CurrentOpenSaveDirectoryEnergyGNUPlotDatas $HOME/Saves/GNUPlot/Energy

  # Counter variables
  variable Id 0
  variable CtrlPointId 0
  variable tmpVal 0

  # Result variables
  variable TclNTreeA
  variable TclNTreeB
  variable TclNTreeC
  variable TclNTreeD
  variable TclNTreeE
  variable TclNTreeF
  variable TclNTreeUS

  # Properties NTree data
  variable TclPropertiesNTreeData
  variable CamerasNTreeData
  variable CamerasPropertiesNTreeData
  variable GridsNTreeData
  variable GridsPropertiesNTreeData
  variable MetricUnitsPropertiesNTreeData
  variable UnionTypeNTree
  variable GlobalPrimaryMotionType
  variable GlobalSecondaryMotionType

  # Scale variables
  variable ScaleDistanceMinimum    -10
  variable ScaleDistanceMaximum    10
  variable ScaleAngleDegreeMinimum -360
  variable ScaleAngleDegreeMaximum 360
  variable ScaleAngleRadMinimum    -6.2831931
  variable ScaleAngleRadMaximum    6.2831931
  variable ScaleMinDistanceFactor  0.000001
  variable ScaleMaxDistanceFactor  0.01
  variable ScalePolygonMaximum     100
  variable Scale01Factor           0.000001
  variable ScaleMinAngleFactor     0.000001
  variable ScaleMaxAngleFactor     0.01
  variable ScaleMotorMinimum  -200
  variable ScaleMotorMaximum  200
  variable ScaleMotorFactor 0.00001
  
  # Camera variables
  variable ScaleCamDistanceMinimum -10
  variable ScaleCamDistanceMaximum 10
 
  
  # Grid variables
  variable ScaleWidthMinimum   12
  variable ScaleWidthMaximum   100
  variable ScaleWidthFactor    01
  variable ScaleDepthMinimum   12
  variable ScaleDepthMaximum   100
  variable ScaleDepthFactor  1
  variable ScaleHeightMinimum  0
  variable ScaleHeightMaximum  100
  variable ScaleHeightFactor  1

  # Metric system scales
  # Grid
  variable ScaleDmWidthMinimum   120
  variable ScaleDmWidthMaximum   1000
  variable ScaleDmDepthMinimum   120
  variable ScaleDmDepthMaximum   1000
  variable ScaleDmHeightMaximum  1000

  # Time Scale
  variable ScaleTimeMaximum  100
  variable ScaleTimeFactor   0.00000001

  # Material Density Scale
  variable ScaleDensityMinimum 1
  variable ScaleDensityMaximum 10000
  variable ScaleDensityFactor  1
  
  # Material MassProperties Scale
  variable ScaleMassPropMinimum -100
  variable ScaleMassPropMaximum 100
  variable ScaleCenterMassMinimum -10
  variable ScaleCenterMassMaximum 10
 variable ScaleMassPropFactor 0.0000001
  
 # Material Density Types
  variable LstDensityTypes [ list {Kg/m3} {g/cm3} {lb/ft3} {Kg/dm3}]

  # CSG Operator Types
  variable LstOperatorTypes [ list {Union} {Intersection} {Difference} ]
  
  # CSG Solid Types
  variable LstSolidTypes [ list {Sphere} {Cylinder} {Cube}  {Pyramid}  {Cone} ]

  # Object Mass Properties Generation Types
  variable LstMassGenMode          [ list {Automatic} {User Defined}]
  
  # HGLMT Types (Kinds of HGLMT types to be displayed in the Properties Palette Window).
  variable LstPTypes [ list {Cartesian} {Cylindrical} {Spherical}]
  variable LstOTypes [ list {Euler XYZ} {Euler X} {Euler Y} {Euler RPY} {Quaternion} ]
  variable LstRTypes [ list {Local} {Global} ]

  # Metric System Unit Types (Kinds of  Metric system unit types to be displayed in the Properties Palette Window).
  variable LstLengthTypes [ list {m} {dm} {cm} {mm} {ft} {in} ]
  variable LstMassTypes [ list {kg} {g} {lb} ]
  variable LstTimeTypes [ list {s} ]
  variable LstAngleTypes [ list {deg} {rad} ]
  variable LstForceTypes [ list {N} ]
  variable LstRobotTypes [ list {Serial} {Hyperbranched} ]
  variable LstRobotBaseTypes [ list {Fixed} {Free Flying} {Free Floating}  {Serpentine} ]
  variable AngleType 0
  variable LengthType 0
  
  variable WorldNodeId

  # ChangeApplicationsTitle procedure --
  # 	 This procedure allows, for any Object Navigator's node, to display
  #      on the application's title bar both the path and the component's
  #      file name.
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeApplicationsTitle {} {
    variable CurrentNode
    set FileName [ GetFileName $CurrentNode ]
    if { $FileName == "" } {
      set TitleNode ""
      set ObjectType [ GetObjectType $CurrentNode ]
      switch -glob $ObjectType {


      # This displays the path and a material's file name.

        Materials*   {
                       set TitleNode [ GetMainNodeType $CurrentNode MaterialsTitle ]
                     }
        CSGs*        {
                       set TitleNode [ GetMainNodeType $CurrentNode CSGsTitle ]
                     }
        Objects*     {
                       set TitleNode [ GetMainNodeType $CurrentNode ObjectsTitle ]
                     }
        Robots*      {
                       set TitleNode [ GetMainNodeType $CurrentNode RobotsTitle ]
                     }


       # This displays the path and a trajectory's file name.

        Trajectorys* {
                       set TitleNode [ GetMainNodeType $CurrentNode TrajectorysTitle ]
                     }
        Worlds*      {
                       set TitleNode [ GetMainNodeType $CurrentNode WorldsTitle ]
                     }
      }
      set FileName [ GetFileName $TitleNode ]
    }
    if { $FileName == "" } {
      wm title . "$RobLab::ApplicationsTitle"
    } else {
      wm title . "$RobLab::ApplicationsTitle - $FileName"
    }
  }

 #-------------------------------------------------------------------------
 # OpenFile procedures --
 #       These procedures GetOpenFileName, ParametricOpenFile, OpenFile manage
 #       how a file is loaded into the system for any object that supports
 #       this function.
 #--------------------------------------------------------------------------

  # GetOpenFileName procedure --
  #
  #		This procedure opens the tk "open" dialog box that allows to choose a
  # 	filename according to some items such as initial directory, file extension,
  # 	and the window in which is going to be displayed.
  #
  # Arguments:
  #
  # Results:
  #
  proc GetOpenFileName { Type Extension Parent } {
    variable CurrentNode
    variable CurrentOpenSaveDirectoryMaterials
    variable CurrentOpenSaveDirectoryCSGs
    variable CurrentOpenSaveDirectoryObjects
    variable CurrentOpenSaveDirectoryRobots
    variable CurrentOpenSaveDirectoryWorlds
    variable CurrentOpenSaveDirectoryInvKineGNUPlotDatas 
    variable CurrentOpenSaveDirectoryFwdDynGNUPlotDatas  
    variable CurrentOpenSaveDirectoryInvDynGNUPlotDatas
    variable CurrentOpenSaveDirectoryEnergyGNUPlotDatas

    # This variable contains the path where the trajectories file names
    # are stored.

    variable CurrentOpenSaveDirectoryTrajectorys
    variable CurrentOpenSaveDirectoryTrajectorysGNUPlotDatas
    variable CurrentOpenSaveDirectorySections
    set InitialDir "CurrentOpenSaveDirectory$Type\s"
    set FileTypes [ list [ list "$Type Files" ".$Extension" ] ]
    set FileName [ tk_getOpenFile -filetypes $FileTypes -initialdir [ expr $$InitialDir ] -parent $Parent -title "Open $Type File" ]
    if { $FileName != "" } {
      set FileName [ FileRemoveNonPortablePath $FileName ]
      set $InitialDir [ file dirname $FileName ]
      MessagesWindow::Puts "Loading [ string tolower $Type ] file \"$FileName\" ...\n"
    }
    return $FileName
  }

  # ParametricOpenFile procedure --
  #
  #		This procedure gets the name of the file to be opened, calling the
  #        GetOpenFileName procedure. Then it calls the Open C procedure to process
  #        this file.
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricOpenFile { Type Extension FileName } {
    variable tplON
    if { $FileName == "" } {
      set FileName [ GetOpenFileName $Type $Extension $tplON ]
    }
    if { $FileName != "" } {
      # Begin C Commands !!!
      set CCommand "TclCreate$Type"
      $CCommand Open "$FileName"
      # End C Commands !!!
    }
  }

  # OpenFile procedure --
  #
  #		This procedure calls both the GetOpenFileName and ParametricOpenFile
  #		procedures with the right parameters depending on the selected
  #		Object Navigator node.
  #
  # Arguments:
  #
  # Results:
  #
  proc OpenFile { { FileName "" } } {
    variable frmONTB
    variable CurrentNode
    variable tplON
    set State [ ToolBars::GetButtonState $frmONTB Open ]
    if { $State == "disabled" } {
      return ""
    }
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {
      Materials    {
                     ParametricOpenFile Material mtr $FileName
                   }
      CSGs         {
                     ParametricOpenFile CSG csg $FileName
                   }
      Objects      {
                     ParametricOpenFile Object obj $FileName
                   }
      Robots       {
                     ParametricOpenFile Robot rbt $FileName
                   }
      Trajectorys  {
                     ParametricOpenFile Trajectory tjc $FileName
                   }
      TrajectorysTitle  {
                          set FileName [ GetOpenFileName Section stn $tplON ]
                          $CurrentNode OpenSection $FileName
                        }
      Worlds       {
                     ParametricOpenFile World wld $FileName
                   }
    }
  }

  # FileRemoveNonPortablePath procedure --
  #
  #		This procedure is used to validate that the user is
  #        entering a valid path for all the supporting platforms
  #         whenever a file is being loaded.
  #
  # Arguments:
  #
  # Results:
  #
  proc FileRemoveNonPortablePath { FileName } {
    set FileName [ join $FileName | ]
    set WorkingDirectory [ join [ pwd ] | ]
    set Portable [ scan "$FileName" "$WorkingDirectory/%s" PortablePath ]
    if { $Portable == 1 } {
      return [ split "./$PortablePath" | ]
    } else {
      MessagesWindow::Puts "WARNING : A non portable path has been used.\n"
      return $FileName
    }
  }

 #-------------------------------------------------------------------------
 # SaveFile procedures --
 #       These procedures ParametricSaveFile and SaveFile manage
 #       how a file is saved for any object that supports
 #       this function.
 #--------------------------------------------------------------------------

  # ParametricSaveFile procedure --
  #
  # 		This procedure checks whether the file name to be saved is alreadey saved
  # 		or not. If it has not been saved it opens the save dialog box, so an user
  # 		can enter a file name and the path where the file name is going to be
  # 		saved.
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricSaveFile { Mode NTreeNode Type Extension } {
    variable tplON
    variable cvsON
    variable CurrentOpenSaveDirectoryMaterials
    variable CurrentOpenSaveDirectoryCSGs
    variable CurrentOpenSaveDirectoryObjects
    variable CurrentOpenSaveDirectoryRobots
    variable CurrentOpenSaveDirectoryTrajectorys
    variable CurrentOpenSaveDirectoryTrajectorysSections
    variable CurrentOpenSaveDirectoryWorlds
    set InitialDir "CurrentOpenSaveDirectory$Type\s"
    set FileTypes [ list [ list "$Type Files" ".$Extension" ] ]
    if { $Mode == 0 } {
      set FileName [ GetFileName $NTreeNode ]
    } else {
      set FileName ""
    }
    if { $FileName == "" } {
      set FileName [ tk_getSaveFile -filetypes $FileTypes -defaultextension ".$Extension" -initialdir [ expr $$InitialDir ] -parent $tplON -title "Save $Type File" ]
      if { $FileName != "" } {
        set FileName [ FileRemoveNonPortablePath $FileName ]
        set $InitialDir [ file dirname $FileName ]
        SetFileName $NTreeNode $FileName
        TkNTree::SetTitle $NTreeNode [ file tail $FileName ]
        TkNTree::SetGraphicTitle $cvsON $NTreeNode [ TkNTree::GetTitle $NTreeNode ]
        # Begin C Commands !!!
        $NTreeNode Save $FileName
        # End C Commands !!!
        ChangeApplicationsTitle
      }
    } else {
      # Begin C Commands !!!
      $NTreeNode Save $FileName
      # End C Commands !!!
    }
  }

  # SaveFile procedure --
  #
  # 		This procedure calls the ParametricSaveFile procedure with the right
  # 		parameters depending of the component that is calling it.
  #
  # Arguments:
  #
  # Results:
  #
  proc SaveFile { { Mode 0 } } {
    variable frmONTB
    variable CurrentNode
    if { $Mode == 0 } {
      set State [ ToolBars::GetButtonState $frmONTB Save ]
      if { $State == "disabled" } {
        return ""
      }
    } else {
      set State [ ToolBars::GetButtonState $frmONTB SaveAs ]
      if { $State == "disabled" } {
        return ""
      }
    }
    set ObjectType [ GetObjectType $CurrentNode ]
    switch -glob $ObjectType {
      LightingsTitle {
                       set FileName [ GetFileName $CurrentNode ]
                       # Begin C Commands !!!
                       $CurrentNode Save $FileName
                       # End C Commands !!!
                     }
      Materials*     {
                       set TitleNode [ GetMainNodeType $CurrentNode MaterialsTitle ]
                       ParametricSaveFile $Mode $TitleNode Material mtr
                     }
      CSGs*          {
                       set TitleNode [ GetMainNodeType $CurrentNode CSGsTitle ]
                       ParametricSaveFile $Mode $TitleNode CSG csg
                     }
      Objects*       {
                       set TitleNode [ GetMainNodeType $CurrentNode ObjectsTitle ]
                       ParametricSaveFile $Mode $TitleNode Object obj
                     }
      Robots*        {
                       set TitleNode [ GetMainNodeType $CurrentNode RobotsTitle ]
                       ParametricSaveFile $Mode $TitleNode Robot rbt
                     }
      TrajectorysTitle {
                       set TitleNode [ GetMainNodeType $CurrentNode TrajectorysTitle ]
                       ParametricSaveFile $Mode $TitleNode Trajectory tjc
                       }
      TrajectorysSection {
                           set TitleNode [ GetMainNodeType $CurrentNode TrajectorysSection ]
                           ParametricSaveFile $Mode $TitleNode TrajectorysSection stn
                         }
      Worlds*        {
                       set TitleNode [ GetMainNodeType $CurrentNode WorldsTitle ]
                       ParametricSaveFile $Mode $TitleNode World wld
                     }
    }
  }

  #------------------------------------------------------------------------------------------------
  #                                      ReloadNode procedures
  #
  # 		The following procedures implements the Reload button. It allows
  #         to destroy and reload the same component.
  #------------------------------------------------------------------------------------------------

  # ParametricReloadBaseNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricReloadBaseNode { Type FileName } {
    set Answer [ DeleteNode ]
    if { $Answer != "cancel" } {
      MessagesWindow::Puts "Loading [ string tolower $Type ] file \"$FileName\" ...\n"
      # Begin C Commands !!!
      TclCreate$Type Open "$FileName"
      # End C Commands !!!
    }
  }

  # ParametricReloadNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricReloadNode { Type Command FileName } {
    variable CurrentNode
    set Parameters Null
    if { $Type != "csg" } {
      set Parameters [ $CurrentNode GetParameters ]
    }
    DeleteNode
    MessagesWindow::Puts "Loading $Type file \"$FileName\" ...\n"
    # Begin C Commands !!!
    if { $Type != "csg" } {
       if { $Type == "basetrajectory" } {
          eval $CurrentNode $Command \"$FileName\" $Parameters
       } else {
          eval [ NTree::GetFather $CurrentNode ] $Command \"$FileName\" $Parameters
       }  
    } else {
      [ NTree::GetFather $CurrentNode ] $Command $FileName
    }
    # End C Commands !!!
  }

  # ReloadNode  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ReloadNode {} {
    variable frmONTB
    variable CurrentNode
    set State [ ToolBars::GetButtonState $frmONTB Reload ]
    if { $State == "disabled" } {
      return ""
    }
    set FileName [ GetFileName $CurrentNode ]
    if { $FileName != "" } {
      set ObjectType [ GetObjectType $CurrentNode ]
      switch $ObjectType {
        MaterialsTitle              {
                                      ParametricReloadBaseNode Material $FileName
                                    }
        CSGsTitle                   {
                                      ParametricReloadBaseNode CSG $FileName
                                    }
        ObjectsTitle                {
                                      ParametricReloadBaseNode Object $FileName
                                    }
        ObjectsElementsSolidsTitle  {
                                      ParametricReloadNode csg AddSolid $FileName
                                    }
        RobotsTitle                 {
                                      ParametricReloadBaseNode Robot $FileName
                                    }
        RobotsBasesObjectsTitle     {
                                      ParametricReloadNode object AddBasesObject $FileName
                                    }
        RobotsLinksObjectsTitle     {
                                      ParametricReloadNode object AddObject $FileName
                                    }
        TrajectorysTitle            {
                                      ParametricReloadBaseNode Trajectory $FileName
                                    }
        WorldsTitle                 {
                                      ParametricReloadBaseNode World $FileName
                                    }
        WorldsObjectsTitle          {
                                      ParametricReloadNode object AddObject $FileName
                                    }
        WorldsRobotsTitle           {
                                      ParametricReloadNode robot AddRobot $FileName
                                    }
        WorldsRobotBaseTjcTitle {
                                                      ParametricReloadNode basetrajectory AddTrajectory $FileName
                                                  }
        WorldsTrajectorysTitle      {
                                      ParametricReloadNode trajectory AddTrajectory $FileName
                                    }
      }
    }
  }

  #--------------------------------------------------------------------------------------------------
  #  EditNode procedures
  #
  # 		The following procedures implement the Edit button functionality.
  # 		A button that is used whenever it is required to change a World's
  # 		element property.
  #---------------------------------------------------------------------------------------------------

  # ParametricEditNode procedure --
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ParametricEditNode { Type Node Command FileName } {
    variable cvsON
    SwapSelectedObject $cvsON $Node 0
    MessagesWindow::Puts "Loading $Type file \"$FileName\" ...\n"
    $Command Open "$FileName"
  }

  # EditNode procedure --
  #
  # Arguments:
  #
  # Results:
  #
  proc EditNode {} {
    variable frmONTB
    variable CurrentNode
    variable CSGsNode
    variable ObjectsNode
    variable TrajectorysNode
    variable RobotsNode
    set State [ ToolBars::GetButtonState $frmONTB Edit ]
    if { $State == "disabled" } {
      return ""
    }
    set FileName [ GetFileName $CurrentNode ]
    set ObjectType [ GetObjectType $CurrentNode ]
    switch -glob $ObjectType {
      ObjectsElementsSolidsTitle  {
                                    ParametricEditNode csg $CSGsNode TclCreateCSG $FileName
                                  }
      Robots*ObjectsTitle         {
                                    ParametricEditNode object $ObjectsNode TclCreateObject $FileName
                                  }
      WorldsObjectsTitle          {
                                    ParametricEditNode object $ObjectsNode TclCreateObject $FileName
                                  }
      WorldsRobotsTitle           {
                                    ParametricEditNode robot $RobotsNode TclCreateRobot $FileName
                                  }
      WorldsRobotBaseTjcTitle {
                                                   ParametricEditNode trajectory $TrajectorysNode TclCreateTrajectory $FileName
                                                }                       
      WorldsTrajectorysTitle      {
                                    ParametricEditNode trajectory $TrajectorysNode TclCreateTrajectory $FileName
                                  }
    }
  }

  #--------------------------------------------------------------------------------------------------
  #  CreateNode procedures
  #
  # 		The following procedures implement the Create button functionality.
  # 		A button that is used whenever it is required to create a ROBOMOSP's
  # 		element.
  #---------------------------------------------------------------------------------------------------

  # CreateNode procedure --
  #
  # 		This procedure is called whenever the Create button is pressed. It creates
  # 		a new instance of a component's data type using the procedures described above.
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateNode {} {
    variable tplON
    variable cvsON
    variable frmONTB
    variable CurrentNode
    variable Id
    set State [ ToolBars::GetButtonState $frmONTB Create ]
    if { $State == "disabled" } {
      return ""
    }
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {
      Materials                   {
                                    set TemporaryNTreeA [ CreateMaterialsNode ]
                                    AddGraphicNTree $TemporaryNTreeA
                                    # Begin C Commands !!!
                                    TclCreateMaterial New $TemporaryNTreeA
                                    # End C Commands !!!
                                    SwapSelectedObject $cvsON [ NTree::GetLastSon $CurrentNode ] 1
                                  }
      CSGs                        {
                                    set TemporaryNTreeA [ CreateCSGNode ]
                                    AddGraphicNTree $TemporaryNTreeA
                                    # Begin C Commands !!!
                                    TclCreateCSG New $TemporaryNTreeA
                                    # End C Commands !!!
                                    SwapSelectedObject $cvsON [ NTree::GetLastSon $CurrentNode ] 1
                                  }
      Objects                     {
                                    set TemporaryNTreeAC [ CreateObjectsNode ]
                                    AddGraphicNTree [ lindex $TemporaryNTreeAC 0 ]
                                    # Begin C Commands !!!
                                    TclCreateObject New [ lindex $TemporaryNTreeAC 0 ] [ lindex $TemporaryNTreeAC 1 ]
                                    # End C Commands !!!
                                    SwapSelectedObject $cvsON [ NTree::GetLastSon $CurrentNode ] 1
                                  }
      ObjectsElementsSolid        {
                                    set FileName [ GetOpenFileName CSG csg $tplON ]
                                    if { $FileName != "" } {
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddSolid $FileName
                                      # End C Commands !!!
                                    }
                                  }
      ObjectsElementsSons         {
                                    set TemporaryNTreeA [ CreateObjectsElementsSonsNode ]
                                    AddGraphicNTree $TemporaryNTreeA
                                    # Begin C Commands !!!
                                    [ NTree::GetFather $CurrentNode ] AddElementSon $TemporaryNTreeA
                                    # End C Commands !!!
                                  }
      Robots                      {
                                    set TemporaryNTreeAC [ CreateRobotsNode ]
                                    AddGraphicNTree [ lindex $TemporaryNTreeAC 0 ]
                                    # Begin C Commands !!!
                                    TclCreateRobot New [ lindex $TemporaryNTreeAC 0 ] [ lindex $TemporaryNTreeAC 1 ]
                                    # End C Commands !!!
                                    SwapSelectedObject $cvsON [ NTree::GetLastSon $CurrentNode ] 1
                                  }
      RobotsBasesObjects          {
                                    set FileName [ GetOpenFileName Object obj $tplON ]
                                    if { $FileName != "" } {
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddBasesObject $FileName "" 0 0 0 0 0 0 0 3 0
                                      # End C Commands !!!
                                    }
                                  }
      RobotsLinksObjects          {
                                    set FileName [ GetOpenFileName Object obj $tplON ]
                                    if { $FileName != "" } {
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddObject $FileName "" 0 0 0 0 0 0 0 3 0
                                      # End C Commands !!!
                                    }
                                  }
      RobotsLinksSons {
                                    set NumberSons [ NTree::GetNumberSons $CurrentNode ]
							#if { $NumberSons == 0 } {
                                      set TemporaryNTreeA [ CreateRobotsLinksSonsNode ]
                                      AddGraphicNTree $TemporaryNTreeA
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddLinkSon $TemporaryNTreeA
                                      # End C Commands !!!
#                                  } else {
#                                     MessagesWindow::Puts "At this time only serial manipulators are allowed.\n"
#                                    }
                                  }

      # It creates a new instance of the StrTrajectory data type
      # if the user is over the Trajectories node and a new instance
      # of the CtrlPoint data type if the user is over the CtrlPoint node.

      Trajectorys               {
                                    set TemporaryNTreeA [ CreateTrajectorysNode ]
                                    AddGraphicNTree $TemporaryNTreeA
                                    # Begin C Commands !!!
                                    TclCreateTrajectory New $TemporaryNTreeA
                                    # End C Commands !!!
                                 }
      TrajectorysTitle           {
                                    set TemporaryNTreeA [ CreateTrajectorysSectionNode ]
                                    AddGraphicNTree $TemporaryNTreeA
                                    # Begin C Commands !!!
                                    $CurrentNode AddSection $TemporaryNTreeA
                                    # End C Commands !!!
                                 }

      TrajectorysSection         {
                                    set tmpNTree  [ PropertyPalette::GetNTreePropertyValue $CurrentNode "PrimaryOrderType"]
                                    set TemporaryNTreeA [ CreateTrajectorysCtrlPointNode ]
                                    if { $tmpNTree=="Translation" } {
                                       AddGraphicNTree $TemporaryNTreeA
                                       # Begin C Commands
                                       $CurrentNode AddIndexCtrlPoint $TemporaryNTreeA
                                       # End C Commands
                                    } else {
                                       InsertGraphicNTree $TemporaryNTreeA
                                       # Begin C Commands
                                       $CurrentNode InsertCtrlPoint $TemporaryNTreeA
                                       # End C Commands
                                    }
                                  }
      Worlds                      {
                                    set TemporaryNTreeAC [ CreateWorldsNode ]
                                    AddGraphicNTree [ lindex $TemporaryNTreeAC 0 ]
                                    # Begin C Commands !!!
                                    TclCreateWorld New [ lindex $TemporaryNTreeAC 0 ] [ NTree::GetFirstSon [ lindex $TemporaryNTreeAC 0 ] ]               [ NTree::GetNextBrother [ NTree::GetFirstSon [ lindex $TemporaryNTreeAC 0 ] ] ]  [ lindex $TemporaryNTreeAC 1 ]
                                    # End C Commands !!!
                                    SwapSelectedObject $cvsON [ NTree::GetLastSon $CurrentNode ] 1
                                  }
                                 
      WorldsObjects               {
                                    set FileName [ GetOpenFileName Object obj $tplON ]
                                    if { $FileName != "" } {
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddObject $FileName "" 0 0 0 0 0 0 0 3 0
                                      # End C Commands !!!
                                    }
                                  }
      WorldsRobots                {
                                    set FileName [ GetOpenFileName Robot rbt $tplON ]
                                    if { $FileName != "" } {
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddRobot $FileName "" 0 0 0 0 0 0 0 3 0 0
                                      # End C Commands !!!
                                    }
                                  }
      WorldsRobotsBaseTrajectorys      {
                                      set FileName [ GetOpenFileName Trajectory tjc $tplON ]
                                      if { $FileName != "" } {
                                         # Begin C Commands !!!
                                         $CurrentNode AddTrajectory $FileName "" 0 0 0 0 0 0 0 3 0
                                       # End C Commands !!!
                                      }
                                  }                          
      WorldsTrajectorys           {
                                    set FileName [ GetOpenFileName Trajectory tjc $tplON ]
                                    if { $FileName != "" } {
                                      # Begin C Commands !!!
                                      [ NTree::GetFather $CurrentNode ] AddTrajectory $FileName "" 0 0 0 0 0 0 0 3 0
                                      # End C Commands !!!
                                    }
                                  }
    }
  }

  #------------------------------------------------------------------------
  #                        DeleteNode procedures
  #
  #   The following procedures implement the Delete button functionality.
  #   This button destroys a node and its associated data type.
  #
  #-------------------------------------------------------------------------

  # DeleteCommandsNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteCommandsNTree { NTree } {
    if { ! [ NTree::Empty $NTree ] } {
      set TemporaryNTree [ NTree::GetFirstSon $NTree ]
      while { ! [ NTree::Empty $TemporaryNTree ] } {
        DeleteCommandsNTree $TemporaryNTree
        set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
      }
      catch [ rename $NTree {} ]
    }
  }

  # DeleteNode --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteNode { { Answer "" } } {
    variable tplON
    variable frmONTB
    variable CurrentNode
    variable PrimaryIntType
    variable SecondaryIntType
    global InvKine
    global InvDyn
    global CartTjc
    set State [ ToolBars::GetButtonState $frmONTB Delete ]
    if { $State == "disabled" } {
      return ""
    }
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {
      MaterialsTitle              {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save Material File" -message "Do you want to save the changes made to material?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      DeleteGraphicNTree
                                    }
                                  }
      CSGsTitle                   {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save CSG File" -message "Do you want to save the changes made to CSG?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      DeleteGraphicNTree
                                    }
                                  }
      CSGsOperatorTitle           {
                                    set FatherObjectType [ GetObjectType [ NTree::GetFather $CurrentNode ] ]
                                    if { $FatherObjectType == "CSGsTitle" } {
                                      TkNTree::SetUpdateAllowed [ NTree::GetFather $CurrentNode ] True
                                    }
                                    # Begin C Commands !!!
                                    $CurrentNode DeleteOperatorSolid
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
      CSGsSolidTitle              {
                                    set FatherObjectType [ GetObjectType [ NTree::GetFather $CurrentNode ] ]
                                    if { $FatherObjectType == "CSGsTitle" } {
                                      TkNTree::SetUpdateAllowed [ NTree::GetFather $CurrentNode ] True
                                    }
                                    # Begin C Commands !!!
                                    $CurrentNode DeleteOperatorSolid
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
      ObjectsTitle                {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save Object File" -message "Do you want to save the changes made to object?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      $CurrentNode DeleteObject
                                      DeleteGraphicNTree
                                    }
                                  }
      ObjectsElementsSolidsTitle  {
                                    # Begin C Commands !!!
                                    [ NTree::GetFather [ NTree::GetFather $CurrentNode ] ] DeleteSolid [ NTree::SonIndex $CurrentNode ]
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                    TkNTree::SetInsertAllowed $CurrentNode True
                                    ToolBars::EnableButton $ObjectNavigator::frmONTB Create
                                  }
      ObjectsElementsTitle        {
                                    # Begin C Commands !!!
                                    $CurrentNode DeleteElement
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
      RobotsTitle                 {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save Robot File" -message "Do you want to save the changes made to robot?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      $CurrentNode DeleteRobot
                                      DeleteGraphicNTree
                                    }
                                  }
      RobotsBasesObjectsTitle     {
                                    # Begin C Commands !!!
                                    [ NTree::GetFather [ NTree::GetFather $CurrentNode ] ] DeleteBasesObject [ NTree::SonIndex $CurrentNode ]
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
      RobotsLinksTitle            {
                                    # Begin C Commands !!!
                                    $CurrentNode DeleteLink
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
      RobotsLinksObjectsTitle     {
                                    # Begin C Commands !!!
                                    [ NTree::GetFather [ NTree::GetFather $CurrentNode ] ] DeleteObject [ NTree::SonIndex $CurrentNode ]
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                    TkNTree::SetInsertAllowed $CurrentNode True
                                    ToolBars::EnableButton $ObjectNavigator::frmONTB Create
                                  }
      TrajectorysTitle            {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save Trajectory File" -message "Do you want to save the changes made to trajectory?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      $CurrentNode DeleteTrajectory
                                      DeleteGraphicNTree
                                    }
                                  }
      TrajectorysSection          {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save Section File" -message "Do you want to save the changes made to section?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      # Begin C Commands
                                      [ NTree::GetFather $CurrentNode ] DeleteSection [ NTree::SonIndex $CurrentNode ]
                                      # End C Commands
                                      DeleteGraphicNTree
                                    }
                                  }
      TrajectorysCtrlPoint        {
                                    # Begin C Commands
                                    [ NTree::GetFather $CurrentNode ] DeleteCtrlPoint $CurrentNode
                                    # End C Commands
                                    DeleteGraphicNTree
                                  }
      WorldsTitle                 {
                                    if { $Answer == "" } {
                                      set Answer [ tk_messageBox -title "Save World File" -message "Do you want to save the changes made to world?" -icon question -type yesnocancel -default yes -parent $tplON ]
                                    }
                                    if { $Answer != "cancel" } {
                                      if { $Answer != "no" } {
                                        SaveFile 0
                                      }
                                      $CurrentNode DeleteWorld
                                      DeleteGraphicNTree
                                      if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
                                         set InvKine 0
                                         set InvDyn 0
                                         set CartTjc 0
                                         ::toolbar::HandleCallback $InvKineToolBar::InvKineTB  -2
  						     ::toolbar::HandleCallback $InvDynToolBar::InvDynTB  -2
                                         ::toolbar::HandleCallback $GNUPlot::CartTjcTB -2
                                      }   
                                      
                                    }
                                  }
      WorldsObjectsTitle          {
                                    # Begin C Commands !!!
                                    [ NTree::GetFather [ NTree::GetFather $CurrentNode ] ] DeleteObject [ NTree::SonIndex $CurrentNode ]
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
      WorldsRobotsTitle           {
                                    # Begin C Commands !!!
                                    [ NTree::GetFather [ NTree::GetFather $CurrentNode ] ] DeleteRobot [ NTree::SonIndex $CurrentNode ]
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                  }
        WorldsRobotBaseTjcTitle    {
                                            [ NTree::GetFather $CurrentNode ]  DeleteTrajectory
                                             DeleteGraphicNTree
                                         }                  
      WorldsTrajectorysTitle {
                                    # Begin C Commands !!!
                                    [ NTree::GetFather [ NTree::GetFather $CurrentNode ] ] DeleteTrajectory [ NTree::SonIndex $CurrentNode ]
                                    # End C Commands !!!
                                    DeleteGraphicNTree
                                           }
    }
    return $Answer
  }

  #  GetAllocatedMemory procedure
  #
  # 		This procedure calls the C function that computes the total size in bytes
  # 		for the component's instance allocated memory.
  #
  # Arguments:
  #
  # Results:
  #
  proc GetAllocatedMemory {} {
    variable frmONTB
    variable CurrentNode
    set State [ ToolBars::GetButtonState $frmONTB Memory ]
    if { $State == "disabled" } {
      return ""
    }
    set ObjectType [ GetObjectType $CurrentNode ]
    switch -glob $ObjectType {
      Materials*   {
                     if { $ObjectType != "Materials" } {
                       set TitleNode [ GetMainNodeType $CurrentNode MaterialsTitle ]
                       set AllocatedMemory [ $TitleNode GetAllocatedMemory ]
                     }
                   }
      CSGs*        {
                     if { $ObjectType != "CSGs" } {
                       set TitleNode [ GetMainNodeType $CurrentNode CSGsTitle ]
                       set AllocatedMemory [ $TitleNode GetAllocatedMemory ]
                     }
                   }
      Objects*     {
                     if { $ObjectType != "Objects" } {
                       set TitleNode [ GetMainNodeType $CurrentNode ObjectsTitle ]
                       set AllocatedMemory [ $TitleNode GetAllocatedMemory ]
                     }
                   }
      Robots*      {
                     if { $ObjectType != "Robots" } {
                       set TitleNode [ GetMainNodeType $CurrentNode RobotsTitle ]
                       set AllocatedMemory [ $TitleNode GetAllocatedMemory ]
                     }
                   }
      Trajectorys* {
                     if { $ObjectType != "Trajectorys" } {
                       set TitleNode [ GetMainNodeType $CurrentNode TrajectorysTitle ]
                       set AllocatedMemory [ $TitleNode GetAllocatedMemory ]
                     }
                   }
      Worlds*      {
                     if { $ObjectType != "Worlds" } {
                       set TitleNode [ GetMainNodeType $CurrentNode WorldsTitle ]
                       set AllocatedMemory [ $TitleNode GetAllocatedMemory ]
                     }
                   }
    }
    MessagesWindow::Puts "Total amount of allocated memory by \"[ TkNTree::GetTitle $TitleNode ]\" is $AllocatedMemory Bytes, [ format "%.2f" [ expr $AllocatedMemory / 1024.0 ] ] KB.\n\n"
  }

  # SetDisplayPointer procedure --
  #
  # 		This procedure modifies the DataType and Data variables belonging to the
  # 		ToglDisplays component. They are used in the main graphical procedure loop to
  # 		determine which 3D element is drawn.
  # 		This procedure is called whenever a component's node is selected. This is
  # 		achieved by creating a prefix for any of the component's node.
  #
  # Arguments:
  #
  # Results:
  #
  proc SetDisplayPointer {} {
    variable CurrentNode
    set TitleNode ""
    set ObjectType [ GetObjectType $CurrentNode ]
    switch -glob $ObjectType {
      Configurations {
                       return
                     }
      LightingsTitle {
                       return
                     }
      Camera         {
                       return
                     }
      Grid      {
                       return
                     }
      MetricUnits      {
                                      return
                                   }
      Materials*     {
                       if { $ObjectType != "Materials" } {
                         set TitleNode [ GetMainNodeType $CurrentNode MaterialsTitle ]
                       }
                     }
      CSGs*          {
                       if { $ObjectType != "CSGs" } {
                         set TitleNode [ GetMainNodeType $CurrentNode CSGsTitle ]
                       }
                     }
      Objects*       {
                       if { $ObjectType != "Objects" } {
                         set TitleNode [ GetMainNodeType $CurrentNode ObjectsTitle ]
                       }
                     }
      Robots*        {
                       if { $ObjectType != "Robots" } {
                         set TitleNode [ GetMainNodeType $CurrentNode RobotsTitle ]
                       }
                     }
      Trajectorys*   {
                       if { $ObjectType != "Trajectorys" } {
                         set TitleNode [ GetMainNodeType $CurrentNode TrajectorysTitle ]
                       }
                     }
      Worlds*        {
                       if { $ObjectType != "Worlds" } {
                         set TitleNode [ GetMainNodeType $CurrentNode WorldsTitle ]
                       }
                     }
    }
    if { $TitleNode != "" } {
      $TitleNode SetDisplayPointer
    } else {
      $DisplayWindow::ToglWidget toglSetToglDisplay toglSetDisplayPointerNone
    }
  }

  # SetNodeMatrix procedure --
  #
  # 		This procedure allows to show in the Matrix Display Window the data
  # 		calculated by the HGLMT component.
  #
  # Arguments:
  #
  # Results:
  #
  proc SetNodeMatrix {} {
    variable CurrentNode
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {
      CSGsOperatorTitle              {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      CSGsSolidTitle                 {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      ObjectsElementsTitle           {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      RobotsTitle                    {
                                       # Begin C Commands !!!
                                       $CurrentNode SetBasesMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      RobotsBasesObjectsTitle        {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      RobotsLinksTitle               {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      RobotsLinksObjectsTitle        {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      TrajectorysTitle               {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      TrajectorysCtrlPoint               {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      WorldsTarget                   {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      WorldsObjectsTitle             {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      WorldsRobotsTitle              {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      WorldsRobotsTeachPendantsLink  {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      WorldsRobotsToolTransform      {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      WorldsTrajectorysTitle         {
                                       # Begin C Commands !!!
                                       $CurrentNode SetMatrix $MatrixDisplay::MDType
                                       # End C Commands !!!
                                     }
      default                        {
                                       MatrixDisplay::Set "." "." "." "." "." "." "." "." "." "." "." "." "." "." "." "."
                                     }
    }
  }
  
# SetNodeMatrix procedure --
#
# 		This procedure allows to show in the Matrix Display Window the data
# 		calculated by the HGLMT component.
#
# Arguments:
#
# Results:
#
  proc SetNodeQuat {} {
    variable CurrentNode
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {
        CSGsOperatorTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
        }
        CSGsSolidTitle  {
                        # Begin C Commands !!!
                         $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
        }
        ObjectsElementsTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
        }
        RobotsTitle  {
                        # Begin C Commands !!!
                         $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
        }
        RobotsBasesObjectsTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
        }
      RobotsLinksTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      RobotsLinksObjectsTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      TrajectorysTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      TrajectorysCtrlPoint  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      WorldsTarget  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      WorldsObjectsTitle  {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      WorldsRobotsTitle {
                        # Begin C Commands !!!
                        $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      WorldsRobotsTeachPendantsLink  {
                        # Begin C Commands !!!
                         $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      WorldsRobotsToolTransform {
                        # Begin C Commands !!!
                          $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      WorldsTrajectorysTitle         {
                        # Begin C Commands !!!
                         $CurrentNode MatrixToQuat $MatrixDisplay::MDType
                        # End C Commands !!!
      }
      default  {
              QuatDisplay::Set "." "." "." "." 
      }
   }
}

  # SetNodeCameraFollow procedure --
  #
  # 		This procedure checks if the SetNodeCameraFollow option is selected.
  # 		If it is selected the camera's position is changed following the position
  # 		parameters of the selected element.
  #
  # Arguments:
  #
  # Results:
  #
  proc SetNodeCameraFollow {} {
    variable CurrentNode
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {
      CSGsOperatorTitle              {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      CSGsSolidTitle                 {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      ObjectsElementsTitle           {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      RobotsTitle                    {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      RobotsBasesObjectsTitle        {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      RobotsLinksTitle               {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      RobotsLinksObjectsTitle        {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      TrajectorysTitle               {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      TrajectorysPoint               {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      WorldsTarget                   {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      WorldsObjectsTitle             {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      WorldsRobotsTitle              {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      WorldsRobotsTeachPendantsLink  {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      WorldsRobotsToolTransform      {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
      WorldsTrajectorysTitle         {
                                       # Begin C Commands !!!
                                       $CurrentNode SetCameraFollow
                                       # End C Commands !!!
                                     }
    }
  }

 #----------------------------------------------------------------------------------
 # ValidateToolBarButtons procedures --
 # 		The following procedures are called anytime that the application needs to
 #      	refresh the ToolBar buttons state for any object from the
 #      	Object Navigator Window.
 #----------------------------------------------------------------------------------

  # Validate ToolBarButton  procedure --
  # 	 This procedure allows, for any Object Navigator's node, to display
  #      on the application's title bar both the path and the component's
  #      file name.
  #
  # Arguments:
  #
  # Results:
  #
  proc ValidateToolBarButtons { { SpecialValidations 1 } } {
    variable cvsON
    variable frmONTB
    variable CurrentNode
    variable WorldNodeId
    variable WorldId
    variable PRobotId
    variable tmpNTree
    variable IntType
    global InvKine
    
    if { [ TkNTree::GetInsertAllowed $CurrentNode ] == $TkNTree::True } {
      ToolBars::EnableButton $frmONTB Create
    } else {
      ToolBars::DisableButton $frmONTB Create
    }
    if { [ TkNTree::GetDeleteAllowed $CurrentNode ] == $TkNTree::True } {
      ToolBars::EnableButton $frmONTB Delete
    } else {
      ToolBars::DisableButton $frmONTB Delete
    }
    set CurrentNodeState [ TkNTree::GetState $CurrentNode ]
    if { $CurrentNodeState == $TkNTree::StateNone } {
      ToolBars::DisableButton $frmONTB Expand
      ToolBars::DisableButton $frmONTB Collapse
      ToolBars::DisableButton $frmONTB ExpandAll
      ToolBars::DisableButton $frmONTB CollapseAll
    } else {
      ToolBars::EnableButton $frmONTB ExpandAll
      ToolBars::EnableButton $frmONTB CollapseAll
      if { $CurrentNodeState == $TkNTree::StateExpand } {
        ToolBars::DisableButton $frmONTB Expand
        ToolBars::EnableButton $frmONTB Collapse
      } else {
        ToolBars::EnableButton $frmONTB Expand
        ToolBars::DisableButton $frmONTB Collapse
      }
    }
    ToolBars::DisableButton $frmONTB Open
    ToolBars::EnableButton $frmONTB Save
    ToolBars::EnableButton $frmONTB SaveAs
    ToolBars::EnableButton $frmONTB Memory
    ToolBars::DisableButton $frmONTB Reload
    ToolBars::DisableButton $frmONTB Edit
    HideCSGToolBar
    DisplayWindow::DisableAllToolBarTwoButtons
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $ObjectType {

    # This implements the ToolBar buttons state whenever the
    # Universe's node is chosen.

      Universe         {
                            ToolBars::DisableButton $frmONTB Save
                            ToolBars::DisableButton $frmONTB SaveAs
                            ToolBars::DisableButton $frmONTB Memory
                       }
    # This implements the ToolBar buttons state whenever the
    # Configuration's node is chosen.

      Configurations                {
		                                  ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }

      CfgMaterials                  {
		                                  ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }
      LightingsTitle                {
                                      ToolBars::DisableButton $frmONTB SaveAs
                                    }
      Camera                        {	
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }
      MetricUnits                   {
									  									  
                                    }
      Grid                          {
									  	
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }

      # This implements the ToolBar buttons state whenever the
      # Material's node is chosen.

      Materials                     {
									                    ToolBars::EnableButton $frmONTB Open
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }
      MaterialsTitle                {
                                      set FatherObjectType [ GetObjectType [ NTree::GetFather $CurrentNode ] ]
                                      if { $FatherObjectType == "CfgMaterials" } {
                                        ToolBars::DisableButton $frmONTB SaveAs
                                        ToolBars::DisableButton $frmONTB Delete
                                      } else {
                                        ToolBars::EnableButton $frmONTB Reload
                                      }
                                    }

      # This implements the ToolBar buttons state whenever the
      # CSG's node is chosen.

      CSGs                          {
                                      ToolBars::EnableButton $frmONTB Open
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }
      CSGsTitle                     {
                                      if { [ TkNTree::GetState $CurrentNode ] == $TkNTree::StateNone } {
                                        ShowCSGToolBar
                                      }
                                      ToolBars::EnableButton $frmONTB Reload
                                    }
      CSGsOperatorTitle             {
                                      ShowCSGToolBar
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      CSGsSolidTitle                {
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }

      # This implements the ToolBar buttons state whenever the
      # Object's node is chosen.

      Objects                       {
                                      ToolBars::EnableButton $frmONTB Open
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }
      ObjectsTitle                  {
                                      ToolBars::EnableButton $frmONTB Reload
									   
                                    }
      ObjectsElementsNTree          {
                                    }
      ObjectsElementsTitle          {
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      ObjectsElementsSolid          {
                                    }
      ObjectsElementsSolidsTitle    {
                                      ToolBars::EnableButton $frmONTB Reload
                                      ToolBars::EnableButton $frmONTB Edit
                                    }
      ObjectsElementsSons           {
                                    }

      # This implements the ToolBar buttons state whenever the
      # Robots's node is chosen.

      Robots                        {
                                      ToolBars::EnableButton $frmONTB Open
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                    }
      RobotsTitle                   {
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                      ToolBars::EnableButton $frmONTB Reload
                                    }
      RobotsBasesObjects            {
                                    }
      RobotsBasesObjectsTitle       {
                                      ToolBars::EnableButton $frmONTB Reload
                                      ToolBars::EnableButton $frmONTB Edit
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      RobotsLinksNTree  {
                                          set tmpNTree  [ PropertyPalette::GetNTreePropertyValue  [NTree::GetFather $CurrentNode] "Robot Type"]
                                          if { ( $tmpNTree=="Hyperbranched" ) } {
                                              ToolBars::EnableButton $frmONTB Create
                                          }
                                        
                                    }
      RobotsLinksTitle   {
                                        
                                    }
      RobotsLinksObjects            {
                                    }
      RobotsLinksObjectsTitle       {
                                      ToolBars::EnableButton $frmONTB Reload
                                      ToolBars::EnableButton $frmONTB Edit
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      RobotsLinksSons               {
                                    }

     # This implements the ToolBar buttons state whenever the
     # trajectory's node is chosen.

      Trajectorys                   {
                                      ToolBars::EnableButton $frmONTB Open
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      TrajectorysTitle              {
                                      ToolBars::EnableButton $frmONTB Open
                                      ToolBars::EnableButton $frmONTB Reload
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                      #Set FileName for GNUPlot
                                      set GNUPlot::FileName [ GNUPlot::ReturnFileName 1 ] 
                                     
                                    }
      TrajectorysSection            {
                                      set tmpNTree  [ PropertyPalette::GetNTreePropertyValue $CurrentNode "PrimaryIntType"]
                                      set tmpNTree1 [ PropertyPalette::GetNTreePropertyValue $CurrentNode "SecondaryIntType"]
                                      if { ( $tmpNTree=="Straight Line" ) || ( $tmpNTree=="Arch" ) || ( $tmpNTree=="Circle" ) || ( [ NTree::GetNumberSons $CurrentNode ] < 2 ) ||
                                           ( $tmpNTree=="Bèzier Spline" ) || ( $tmpNTree=="Interpolated" && $tmpNTree1=="None") || ( $tmpNTree=="Interpolated" && $tmpNTree1=="Straight Line") ||
                                           ( $tmpNTree=="Interpolated" && $tmpNTree1=="Arch") || ( $tmpNTree=="Interpolated" && $tmpNTree1=="Circle") ||
                                           ( $tmpNTree=="Interpolated" && $tmpNTree1=="Bèzier Spline") } {
                                        ToolBars::DisableButton $frmONTB Create
                                      } else {
                                        ToolBars::EnableButton $frmONTB Create
                                      }
                                    }
      TrajectorysCtrlPoint          {
                                      if { [ NTree::GetNumberSons [NTree::GetFather $CurrentNode] ] > 3 } {
                                         ToolBars::EnableButton $frmONTB Delete
                                      } else {
                                         ToolBars::DisableButton $frmONTB Delete
                                      }
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }

      # This implements the ToolBar buttons state whenever the
      # World's node is chosen.

      Worlds                   {
                                      ToolBars::EnableButton $frmONTB Open
                                      ToolBars::DisableButton $frmONTB Save
                                      ToolBars::DisableButton $frmONTB SaveAs
                                      ToolBars::DisableButton $frmONTB Memory
                               }
      WorldsTitle              {
                                      ToolBars::EnableButton $frmONTB Reload
                                      set WorldNodeId $CurrentNode
                                      bind $cvsON  <Button-3> "InvKineToolBar::Initialize"
                                      set WorldIndex [ IOIC::Worlds GetWorldIndex $WorldNodeId  ]
                                      set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
                                      if { [ llength [ IOIC::World $WorldId LsPRobots ] ] != 0 } {
                                      	set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
                                      	set PRobotBaseType [ IOIC::PRobot $PRobotId GetBaseType ] 
                                      	if { ( $InvKine == 1) &&  ( $GNUPlot::GNUPlotInit2 == 0 )} {
                                          	GNUPlot::Activate 2
                                      	}
                                       	if { ( $GNUPlot::GNUPlotInit2 == 1 ) && ( $PRobotBaseType == "Fixed")} {
                                           	set GNUPlot::GNUPlotInit2 0
                                          	::toolbar::HandleCallback $GNUPlot::CartTjcTB  -2
                                      	}
						  }    
                               }
      WorldsTarget             {
                                       DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      WorldsObjects                 {
                                    }
      WorldsObjectsTitle            {
                                      ToolBars::EnableButton $frmONTB Reload
                                      ToolBars::EnableButton $frmONTB Edit
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      WorldsRobots                  {
                                    }
      WorldsRobotsTitle             {
                                      ToolBars::EnableButton $frmONTB Reload
                                      ToolBars::EnableButton $frmONTB Edit
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
      WorldsRobotBaseTjcTitle {
                                                  ToolBars::EnableButton $frmONTB Reload
                                                  ToolBars::EnableButton $frmONTB Edit
                                                  DisplayWindow::EnableAllToolBarTwoButtons
                                               } 
      WorldsRobotsTeachPendant      {
                                    }
      WorldsRobotsTeachPendantsLink {
                                    }
      WorldsRobotsToolTransform     {
                                    }
      WorldsTrajectorys             {
                                    }
      WorldsTrajectorysTitle        {
                                      ToolBars::EnableButton $frmONTB Reload
                                      ToolBars::EnableButton $frmONTB Edit
                                      DisplayWindow::EnableAllToolBarTwoButtons
                                    }
                              
    }
    if { $SpecialValidations == 1 } {
      PropertyPalette::DisplayPropertiesNTree [ GetPropertiesNTree $CurrentNode ]
      ChangeApplicationsTitle
      SetDisplayPointer
      SetNodeMatrix
      SetNodeCameraFollow
      TkNTree::SeeCanvasNode $cvsON $CurrentNode
    }
  }

  #  ShowCSGToolBar procedure
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ShowCSGToolBar {} {
    variable frmCSGA
    variable frmCSGTB
    set Length [ llength [ pack slaves $frmCSGA ] ]
    if { $Length == 0 } {
      pack $frmCSGTB -side bottom -fill x
    }
  }

  #  HideCSGToolBar procedure --
  #
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc HideCSGToolBar {} {
    variable frmCSGA
    variable frmCSGTB
    set Length [ llength [ pack slaves $frmCSGA ] ]
    if { $Length != 0 } {
      pack forget $frmCSGTB
      $frmCSGA configure -height 2
    }
  }

  # CSGToolBar procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CSGToolBar { Button } {
    variable CurrentNode
    set ObjectType [ GetObjectType $CurrentNode ]
    switch $Button {
      Union        {
                     set TemporaryNTreeA [ CreateCSGOperatorUnionNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddOperator $TemporaryNTreeA Union
                     # End C Commands !!!
                   }
      Intersection {
                     set TemporaryNTreeA [ CreateCSGOperatorIntersectionNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddOperator $TemporaryNTreeA Intersection
                     # End C Commands !!!
                   }
      Difference   {
                     set TemporaryNTreeA [ CreateCSGOperatorDifferenceNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddOperator $TemporaryNTreeA Difference
                     # End C Commands !!!
                   }
      Sphere       {
                     set TemporaryNTreeA [ CreateCSGSolidSphereNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddSolid $TemporaryNTreeA Sphere
                     # End C Commands !!!
                   }
      Cylinder     {
                     set TemporaryNTreeA [ CreateCSGSolidCylinderNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddSolid $TemporaryNTreeA Cylinder
                     # End C Commands !!!
                   }
      Cube         {
                     set TemporaryNTreeA [ CreateCSGSolidCubeNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddSolid $TemporaryNTreeA Cube
                     # End C Commands !!!
                   }
      Pyramid      {
                     set TemporaryNTreeA [ CreateCSGSolidPyramidNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddSolid $TemporaryNTreeA Pyramid
                     # End C Commands !!!
                   }
      Cone         {
                     set TemporaryNTreeA [ CreateCSGSolidConeNode ]
                     # Begin C Commands !!!
                     $CurrentNode AddSolid $TemporaryNTreeA Cone
                     # End C Commands !!!
                   }
    }
    AddGraphicNTree $TemporaryNTreeA
    ValidateToolBarButtons 0
    return $TemporaryNTreeA
  }

 #-----------------------------------------------------------------------------------------
 # Main manager procedures
 #
 #   The following procedures control and manage the core functionality
 #   for the OpenNavigator manager.
 #-----------------------------------------------------------------------------------------

  # GetObjectType --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetObjectType { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 0 ]
  }

  # GetPropertiesNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertiesNTree { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 1 ]
  }

  # GetPropertyParent --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertyParent { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 0 ]
  }

  # GetPropertyName --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertyName { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 1 ]
  }

  # GetPropertyType --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertyType { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 2 ]
  }

  # SetPropertyName --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetPropertyName { NTree Name } {
    TkNTree::SetData $NTree [ lreplace [ TkNTree::GetData $NTree ] 1 1 $Name ]
  }

  # SetPropertyType --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetPropertyType { NTree Type } {
    TkNTree::SetData $NTree [ lreplace [ TkNTree::GetData $NTree ] 2 2 $Type ]
  }

  # SetPropertyValue --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetPropertyValue { NTree Value } {
    TkNTree::SetData $NTree [ lreplace [ TkNTree::GetData $NTree ] 3 3 [ list "$Value" [ lindex [ lindex [ TkNTree::GetData $NTree ] 3 ] 1 ] ] ]
  }

  # GetPropertyValue --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertyValue { NTree } {
    return [ lindex [ lindex [ TkNTree::GetData $NTree ] 3 ] 0 ]
  }

  #  GetPropertyListOfElements --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertyListOfElements { NTree } {
    return [ lindex [ lindex [ TkNTree::GetData $NTree ] 3 ] 1 ]
  }

  #  SetPropertyListOfElements --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetPropertyListOfElements { NTree ListOfElements } {
    TkNTree::SetData $NTree [ lreplace [ TkNTree::GetData $NTree ] 3 3 [ list [ lindex [ lindex [ TkNTree::GetData $NTree ] 3 ] 0 ] "$ListOfElements" ] ]
  }

  #  SetPropertySecondaryTitle --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetPropertySecondaryTitle { NTree Title } {
    TkNTree::SetData $NTree [ lreplace [ TkNTree::GetData $NTree ] 4 4 $Title ]
  }

  #  GetPropertySecondaryTitle --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetPropertySecondaryTitle { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 4 ]
  }

  #  GetFileName --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetFileName { NTree } {
    return [ lindex [ TkNTree::GetData $NTree ] 2 ]
  }

  #  SetFileName --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetFileName { NTree FileName } {
    TkNTree::SetData $NTree [ lreplace [ TkNTree::GetData $NTree ] 2 2 $FileName ]
  }

  # GetMainNodeType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetMainNodeType { NTree Type } {
    if { [ NTree::Empty $NTree ] } {
      return $NTree::Null
    }
    set ObjectType [ GetObjectType $NTree ]
    if { $ObjectType == $Type } {
      return $NTree
    } else {
      return [ GetMainNodeType [ NTree::GetFather $NTree ] $Type ]
    }
  }

  # GetNextNodeType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNextNodeType { NTree Type } {
    if { [ NTree::Empty $NTree ] } {
      return $NTree::Null
    }
    set TemporaryNTree [ NTree::GetFirstSon $NTree ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      set ObjectType [ GetObjectType $TemporaryNTree ]
      if { $ObjectType == $Type } {
        return $TemporaryNTree
      }
      set SearchResult [ GetNextNodeType $TemporaryNTree $Type ]
      if { ! [ NTree::Empty $SearchResult ] } {
        return $SearchResult
      }
      set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
    }
    return $NTree::Null
  }

  #--------------------------------------------------------------------------------------------------------
  #
  #  Graphical Object Navigator and Properties Palette add and insert ntree
  #  procedures
  #
  #--------------------------------------------------------------------------------------------------------

  # AddGraphicNTree procedure --
  #
  #		This procedure adds a graphic sub-NTree to the ObjectNavigator's main
  # 	TkNTree.
  #
  # Arguments:
  #
  # Results:
  #
  proc AddGraphicNTree { TemporaryNTree } {
    variable cvsON
    variable CurrentNode
    variable Id
    set Id [ expr $Id + 1 ]
    TkNTree::AddGraphicNTree $cvsON $CurrentNode $TemporaryNTree
  }

  # InsertGraphicNTree procedure --
  #
  #		This procedure inserts a graphic sub-NTree to the ObjectNavigator's main
  #		TkNTree.
  #
  # Arguments:
  #
  # Results:
  #
  proc InsertGraphicNTree { TemporaryNTree } {
    variable cvsON
    variable CurrentNode
    variable Id
    set Id [ expr $Id + 1 ]
    TkNTree::InsertGraphicNTree $cvsON $CurrentNode $TemporaryNTree
  }

  # DeleteGraphicNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteGraphicNTree {} {
    variable cvsON
    variable CurrentNode
    #DeleteCommandsNTree $CurrentNode
    TkNTree::DeleteGraphicNTree $cvsON ObjectNavigator::CurrentNode $CurrentNode ObjectNavigator::ValidateToolBarButtons
  }

  # DeleteCtrlGraphicNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteCtrlGraphicNTree { tmpCurrentNode} {
    variable cvsON
    DeleteCommandsNTree $tmpCurrentNode
    TkNTree::DeleteCtrlGraphicNTree $cvsON ObjectNavigator::CurrentNode $tmpCurrentNode ObjectNavigator::ValidateToolBarButtons
  }

  # AddGraphicPropertieNTree procedure --
  #
  #		This procedure adds a graphic sub-NTree to the Properties's main
  #		TkNTree.
  #
  # Arguments:
  #
  # Results:
  #
  proc AddGraphicPropertieNTree { TemporaryNTree } {
    variable cvsPP
    variable CurrentNode
    TkNTree::AddGraphicNTree $PropertyPalette::cvsPP $PropertyPalette::CurrentNode $TemporaryNTree
  }

  # SwapSelectedObject procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SwapSelectedObject { Canvas NewNode { Validate 1 } } {
    variable CurrentNode
    TkNTree::SwapSelectedObject $Canvas $CurrentNode $NewNode
    set CurrentNode $NewNode
    if { $Validate == 1 } {
      ValidateToolBarButtons
    }
  }

 # ExpandPropertyPaletteNTree --
 #
 # 		This procedure expands a graphical representation of a PropertyPalette NTree Node
 #
 # Arguments:
 #
 # Results:
 #
 proc ExpandPropertyPaletteNTree { } {
    # Expand NTree.
    RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
 }

 # CollapsePropertyPaletteNTree.
 #
 # 		This procedure collapses a graphical representation of a PropertyPalette NTree Node
 #
 # Arguments:
 #
 # Results:
 #
 proc CollapsePropertyPaletteNTree { } {
 variable CurrentNode

    #Collapse NTree.
    RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB  PropertyPalette::CurrentNode

 }

 # DeletePropertyPaletteNTree.
 #
 # 		This procedure deletes a graphical representation of a PropertyPalette NTree
 #
 # Arguments:
 #
 # Results:
 #
 proc DeletePropertyPaletteNTree { } {
 variable UnionTypeNTree
    set UnionTypeNTree $UnionTypeNTree
    set Parent [ GetPropertyParent $UnionTypeNTree ]

    #Collapse NTree.
    RobLab::CollapseNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode

    # Delete NTree's sons.
    set TemporaryNTree [ NTree::GetFirstSon $UnionTypeNTree ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      NTree::DisAssociate $TemporaryNTree
      NTree::Destroy $TemporaryNTree
      set TemporaryNTree [ NTree::GetFirstSon $UnionTypeNTree ]
    }
 }

  #  Initialize procedure --
  #
  #   Main initialisation procedure. It fully initializes the object manager of
  #   the Object Navigator Window. It creates the Object Navigator as we see it
  #   when the application starts.
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplON
    variable cvsON
    variable frmONTB
    variable frmCSGTB
    variable CSGToolBar
    variable ApplicationNTree
    variable CurrentNode
    variable CSGsNode
    variable ObjectsNode
    variable RobotsNode
    variable TrajectorysNode
    variable WorldsNode
    variable GNUPlotNode

    # Initialization of object navigator window
    RobLab::CreateWindow "" wndObjectNavigator "Object Navigator" "$RobLab::ON_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $ObjectNavigator::tplON } $RobLab::VirtualGridSize
    frame $tplON.frmON_A -borderwidth 1 -width 13 -height 13
    frame $tplON.frmON_B -borderwidth 1 -width 13 -height 13
    scrollbar $tplON.frmON_A.scbHorizontal -borderwidth 1 -orient horiz -command "$cvsON xview"
    scrollbar $tplON.scbVertical -borderwidth 1 -orient verti -command "$cvsON yview"
    frame $tplON.frmToolBar -width 26
    frame $tplON.frmON_A.frmCSG_A -height 2 -borderwidth 1 -relief sunken
    frame $tplON.frmON_A.frmCSG_A.frmCSGToolBar -height 26 -background #fffffffff
    canvas $cvsON -borderwidth 1 -relief sunken -width 17 -height 17 -background #fffffffff -xscrollcommand "$tplON.frmON_A.scbHorizontal set" -yscrollcommand "$tplON.scbVertical set" -xscrollincrement 17 -yscrollincrement 17 -confine true -scrollregion { 0 0 0 0 } -highlightthickness 0
    pack $tplON.frmToolBar -side left -fill y
    pack $tplON.frmON_A -side left -fill both -expand true
    pack $tplON.frmON_A.scbHorizontal -side bottom -fill x
    pack $tplON.frmON_A.frmCSG_A -side bottom -fill x -padx 0 -pady 0
    pack $cvsON -side top -fill both -expand true
    pack $tplON.frmON_B -side bottom
    pack $tplON.scbVertical -side left -fill y
    update

    # Initialization of ApplicationNTree
    set NodeNTreeData [ list Universe $NTree::Null "" ]
    set ApplicationNTree [ TkNTree::Set "Universe" StateCollapse 0 False False Undefined $NodeNTreeData ]

    # Initialization of the Configurations NTree Node.
    NTree::Add $ApplicationNTree [ CreateConfigurationsNode ]

    # Initialization of the Materials NTree Node.
    set NodeNTreeData [ list Materials $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Materials" StateNone 18 True False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA

    # Initialization of the CSGs NTree Node.
    set NodeNTreeData [ list CSGs $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "CSGs" StateNone 6 True False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set CSGsNode $TemporaryNTreeA

    # Initialization of the Objects NTree Node.
    set NodeNTreeData [ list Objects $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Objects" StateNone 2 True False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set ObjectsNode $TemporaryNTreeA

    # Initialization of the Robots NTree Node.
    set NodeNTreeData [ list Robots $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Robots" StateNone 3 True False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set RobotsNode $TemporaryNTreeA

    # Initialization of the Trajectories NTree Node.
    set NodeNTreeData [ list Trajectorys $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Trajectories" StateNone 33 True False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set TrajectorysNode $TemporaryNTreeA

    # Initialization of the Worlds NTree Node.
    set NodeNTreeData [ list Worlds $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Worlds" StateNone 1 True False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set WorldsNode $TemporaryNTreeA

    # Initialization of the GNUPlot NTree Node.
#NTree::Add $ApplicationNTree [ CreateGNUPlotNode ]
    

    # App
    set CurrentNode $ApplicationNTree
    TkNTree::AddGraphicNode $cvsON $ApplicationNTree 0 0
    TkNTree::ExpandNTree $cvsON $ApplicationNTree
    TkNTree::InitializeSelection $cvsON $ApplicationNTree

    # Initialization of binds
    bind $tplON <KeyPress> "TkNTree::MoveCurrentNode %K $cvsON ObjectNavigator::CurrentNode ObjectNavigator::ValidateToolBarButtons"
    $cvsON bind all <Button-1> "TkNTree::Canvas_Bind_All_Button1 $cvsON ObjectNavigator::CurrentNode ObjectNavigator::ValidateToolBarButtons"
    bind $cvsON <Configure> "TkNTree::Bind_Canvas_Configure $cvsON ObjectNavigator::CurrentNode"

    # Initialization of object navigator tool bar
    set ONToolBar  { {          Open              "ROpen"           imgONTBOpen  disabled  { ObjectNavigator::OpenFile } }
                     {          Save              "RSave"           imgONTBSave  disabled  { ObjectNavigator::SaveFile 0 } }
                     {        SaveAs           "RSave As"         imgONTBSaveAs  disabled  { ObjectNavigator::SaveFile 1 } }
                     {        Reload            "RReload"         imgONTBReload  disabled  { ObjectNavigator::ReloadNode } }
                     {          Edit              "REdit"           imgONTBEdit  disabled  { ObjectNavigator::EditNode } }
                     { "" }
                     {        Create            "RCreate"         imgONTBCreate  disabled  { ObjectNavigator::CreateNode } }
                     {        Delete            "RDelete"         imgONTBDelete  disabled  { ObjectNavigator::DeleteNode } }
                     { "" }
                     {        Expand            "RExpand"         imgONTBExpand  disabled  {} }
                     {      Collapse          "RCollapse"       imgONTBCollapse    normal  {} }
                     {     ExpandAll        "RExpand All"      imgONTBExpandAll    normal  {} }
                     {   CollapseAll      "RCollapse All"    imgONTBCollapseAll    normal  {} }
                     { "" }
                     {        Memory  "RAllocated Memory"         imgONTBMemory  disabled  { ObjectNavigator::GetAllocatedMemory } } }
    ToolBars::Create $frmONTB $ONToolBar Vertical
    ToolBars::SetButtonCommand $frmONTB      Expand "RobLab::ExpandNTree $cvsON $frmONTB ObjectNavigator::CurrentNode"
    ToolBars::SetButtonCommand $frmONTB    Collapse "RobLab::CollapseNTree $cvsON $frmONTB ObjectNavigator::CurrentNode"
    ToolBars::SetButtonCommand $frmONTB   ExpandAll "RobLab::ExpandAllNTree $cvsON $frmONTB ObjectNavigator::CurrentNode"
    ToolBars::SetButtonCommand $frmONTB CollapseAll "RobLab::CollapseAllNTree $cvsON $frmONTB ObjectNavigator::CurrentNode"
    set CSGToolBar { {         Union             "RUnion"         imgCSGTBUnion    normal  { ObjectNavigator::CSGToolBar Union } }
                     {  Intersection      "RIntersection"  imgCSGTBIntersection    normal  { ObjectNavigator::CSGToolBar Intersection } }
                     {    Difference        "RDifference"    imgCSGTBDifference    normal  { ObjectNavigator::CSGToolBar Difference } }
                     { "" }
                     {        Sphere            "RSphere"        imgCSGTBSphere    normal  { ObjectNavigator::CSGToolBar Sphere } }
                     {      Cylinder          "RCylinder"      imgCSGTBCylinder    normal  { ObjectNavigator::CSGToolBar Cylinder } }
                     {          Cube              "RCube"          imgCSGTBCube    normal  { ObjectNavigator::CSGToolBar Cube } }
                     {       Pyramid           "RPyramid"       imgCSGTBPyramid    normal  { ObjectNavigator::CSGToolBar Pyramid } }
                     {          Cone              "RCone"          imgCSGTBCone    normal  { ObjectNavigator::CSGToolBar Cone } } }
    ToolBars::Create $frmCSGTB $CSGToolBar Horizontal
  }
}
