# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------

#  File Name    : DisplayWindow.tcl
#  Date         : 2001-11-09
#  Description  : Implementation of display window
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co

# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  DisplayWindow.tcl
#		This file implements the ROBOMOSP display window.
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

# Initialization of Tk objects

  # Images of display window canvas
  image create photo imgDPTBTop -file ./Images/DisplayWindowToolBar/Top.gif
  image create photo imgDPTBLeft -file ./Images/DisplayWindowToolBar/Left.gif
  image create photo imgDPTBRight -file ./Images/DisplayWindowToolBar/Right.gif
  image create photo imgDPTBUser -file ./Images/DisplayWindowToolBar/User.gif
  image create photo imgDPTBAllViews -file ./Images/DisplayWindowToolBar/AllViews.gif
  image create photo imgDPTBOrthographic -file ./Images/DisplayWindowToolBar/Orthographic.gif
  image create photo imgDPTBPerspective -file ./Images/DisplayWindowToolBar/Perspective.gif
  image create photo imgDPTBSMFlat -file ./Images/DisplayWindowToolBar/SMFlat.gif
  image create photo imgDPTBSMSmooth -file ./Images/DisplayWindowToolBar/SMSmooth.gif
  image create photo imgDPTBWireframe -file ./Images/DisplayWindowToolBar/PMWireframe.gif
  image create photo imgDPTBHiddenLines -file ./Images/DisplayWindowToolBar/PMHiddenLines.gif
  image create photo imgDPTBPreview -file ./Images/DisplayWindowToolBar/PMPreview.gif
  image create photo imgDPTBLightingOff -file ./Images/DisplayWindowToolBar/LightingOff.gif
  image create photo imgDPTBLightingOn -file ./Images/DisplayWindowToolBar/LightingOn.gif
  image create photo imgDPTBGridOff -file ./Images/DisplayWindowToolBar/GridOff.gif
  image create photo imgDPTBGridOn -file ./Images/DisplayWindowToolBar/GridOn.gif
  image create photo imgDPTBAxesOff -file ./Images/DisplayWindowToolBar/AxesOff.gif
  image create photo imgDPTBAxesOn -file ./Images/DisplayWindowToolBar/AxesOn.gif
  image create photo imgDPTBSolidsOff -file ./Images/DisplayWindowToolBar/SolidsOff.gif
  image create photo imgDPTBSolidsOn -file ./Images/DisplayWindowToolBar/SolidsOn.gif
  image create photo imgDPTBCSGOff -file ./Images/DisplayWindowToolBar/CSGOff.gif
  image create photo imgDPTBCSGOn -file ./Images/DisplayWindowToolBar/CSGOn.gif
  image create photo imgDPTBSelectionOn -file ./Images/DisplayWindowToolBar/SelectionOn.gif
  image create photo imgDPTBSelectionOff -file ./Images/DisplayWindowToolBar/SelectionOff.gif
  image create photo imgDPTBCMOn -file ./Images/DisplayWindowToolBar/CMOn.gif
  image create photo imgDPTBCMOff -file ./Images/DisplayWindowToolBar/CMOff.gif
  image create photo imgDPTBFollowOn -file ./Images/DisplayWindowToolBar/CmrFollowOn.gif
  image create photo imgDPTBFollowOff -file ./Images/DisplayWindowToolBar/CmrFollowOff.gif
  image create photo imgDPTBTBoth -file ./Images/DisplayWindowToolBar/TBoth.gif
  image create photo imgDPTBTFirst -file ./Images/DisplayWindowToolBar/TFirst.gif
  image create photo imgDPTBTSecond -file ./Images/DisplayWindowToolBar/TSecond.gif
  image create photo imgDPTBP1P2 -file ./Images/DisplayWindowToolBar/P1P2.gif
  image create photo imgDPTBP1P3 -file ./Images/DisplayWindowToolBar/P1P3.gif
  image create photo imgDPTBP2P3 -file ./Images/DisplayWindowToolBar/P2P3.gif
  image create photo imgDPTBO1O2 -file ./Images/DisplayWindowToolBar/O1O2.gif
  image create photo imgDPTBO1O3 -file ./Images/DisplayWindowToolBar/O1O3.gif
  image create photo imgDPTBO2O3 -file ./Images/DisplayWindowToolBar/O2O3.gif
  image create photo imgDPTBPan -file ./Images/DisplayWindowToolBar/CmrPan.gif
  image create photo imgDPTBTrackBall -file ./Images/DisplayWindowToolBar/CmrTrackBall.gif
  image create photo imgDPTBTwist -file ./Images/DisplayWindowToolBar/CmrTwist.gif
  image create photo imgDPTBZoom -file ./Images/DisplayWindowToolBar/CmrZoom.gif

namespace eval DisplayWindow {

  # Procedures :
  # proc BindtglWindowDoubleButton1 {}
  # proc BindtglWindowButton1 { CurrentMouseX CurrentMouseY }
  # proc BindtglWindowB1Motion { CurrentMouseX CurrentMouseY }
  # proc BindtglWindowButton3 { CurrentMouseX CurrentMouseY }
  # proc BindtglWindowB3Motion { CurrentMouseX CurrentMouseY }
  # proc BindtglWindowDoubleButton3 { Geometry }
  # proc BindtplDPKeyPress { Keysym }
  # proc CommandSetProjectionType {}
  # proc CommandSetShadeModel {}
  # proc CommandSetPolygonMode {}
  # proc CommandSetLighting {}
  # proc CommandSetGrid {}
  # proc CommandSetAxes {}
  # proc CommandSetSolids {}
  # proc CommandSetCSGMode {}
  # proc CommandSetCMMode {}
  # proc CommandSetSelectionMode {}
  # proc CommandSetTransformation { Text }
  # proc CommandSetAxesToTransform {}
  # proc CommandSetCamera { Text }
  # proc RenderWindow {}
  # proc EnableAllToolBarTwoButtons {}
  # proc DisableAllToolBarTwoButtons {}
  # proc Initialize {}

  # Initialization of global variables
  variable tplDP .wndDisplayWindow
  variable frmDPTB1 $tplDP.frmToolBarOne
  variable frmDPTB2 $tplDP.frmToolBarTwo
  variable frmDPTB3 $tplDP.frmToolBarThree
  variable ToglWidget $tplDP.tglWidget
  variable MouseX
  variable MouseY
  variable Transformation ""
  variable OldTransformation ""
  variable Camera "Track Ball"

  #  BindtglWindowDoubleButtion1  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc BindtglWindowDoubleButton1 {} {
    variable tplDP
    set Flag 0
    set ObjectType [ ObjectNavigator::GetObjectType $ObjectNavigator::CurrentNode ]
    if { $ObjectType == "CSGsSolidTitle" } {
      set Flag 1
    }
    if { $Flag == 1 } {
      set FileName [ ObjectNavigator::GetOpenFileName Material mtr $tplDP ]
      if { $FileName != "" } {
        # Begin C Commands !!!
        $ObjectNavigator::CurrentNode ModifyProperty SolidMaterial $FileName
        # End C Commands !!!
      }
    }
  }

  #  BindtglWindowButtion1  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc BindtglWindowButton1 { CurrentMouseX CurrentMouseY } {
    variable ToglWidget
    variable MouseX
    variable MouseY
    set MouseX $CurrentMouseX
    set MouseY $CurrentMouseY
    $ToglWidget toglSetToglDisplay toglSetRenderMode Select $CurrentMouseX $CurrentMouseY
    $ToglWidget render
  }

  #  BindtglWindowB1Motion  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  
proc BindtglWindowB1Motion { CurrentMouseX CurrentMouseY } {
    variable frmDPTB1
    variable MouseX
    variable MouseY
    variable Transformation
    
    #Length Metrics Transformation
    switch $ObjectNavigator::LengthType {
         0 {
              set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0*0.1]
              set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0*0.1]
            }   
         1 {
              set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0 ]
              set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0 ]
            }
         2 {
              set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0*10]
              set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0*10]
            }
         3 {
              set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0*100]
              set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0*100]
            }
         4 {
              set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0*0.328084]
              set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0*0.328084]
            }
         0 {
              set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0*3.9370079]
              set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0*3.9370079]
            }    
    }
    
    #Angle Metrics Transformation
	if { $ObjectNavigator::AngleType == 1 } {
			set DeltaRX [ expr ( ( $CurrentMouseX - $MouseX ) / 2.0*0.0174533 ) ]
			set DeltaRY [ expr ( ( $CurrentMouseY - $MouseY ) / 2.0*0.0174533 ) ]
	} else {
			set DeltaRX [ expr ( $CurrentMouseX - $MouseX ) / 2.0 ]
			set DeltaRY [ expr ( $CurrentMouseY - $MouseY ) / 2.0 ]
    }
	
    set ToolTipATT [ ToolBars::GetButtonToolTip $frmDPTB1 AxesToTransform ]
    switch $Transformation {
      "P1, P2"       {
                       # Begin C Commands !!!
                       if { $ToolTipATT == "LBoth Axes" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO2Add $DeltaTX
                         $ObjectNavigator::CurrentNode ModifyProperty vPO1Add $DeltaTY
                       } elseif { $ToolTipATT == "LFirst Axis" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO2Add $DeltaTX
                       } else {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO1Add $DeltaTY
                       }
                       # End C Commands !!!
                     }
      "P1, P3"       {
                       # Begin C Commands !!!
                       if { $ToolTipATT == "LBoth Axes" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO1Add [ expr -$DeltaTX ]
                         $ObjectNavigator::CurrentNode ModifyProperty vPO3Add [ expr -$DeltaTY ]
                       } elseif { $ToolTipATT == "LFirst Axis" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO1Add [ expr -$DeltaTX ]
                       } else {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO3Add [ expr -$DeltaTY ]
                       }
                       # End C Commands !!!
                     }
      "P2, P3"       {
                       # Begin C Commands !!!
                       if { $ToolTipATT == "LBoth Axes" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO2Add $DeltaTX
                         $ObjectNavigator::CurrentNode ModifyProperty vPO3Add [ expr -$DeltaTY ]
                       } elseif { $ToolTipATT == "LFirst Axis" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO2Add $DeltaTX
                       } else {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO3Add [ expr -$DeltaTY ]
                       }
                       # End C Commands !!!
                     }
      "O1, O2"       {
                       # Begin C Commands !!!
                       if { $ToolTipATT == "LBoth Axes" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO4Add [ expr -$DeltaRX ]
                         $ObjectNavigator::CurrentNode ModifyProperty vPO5Add $DeltaRY
                       } elseif { $ToolTipATT == "LFirst Axis" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO4Add [ expr -$DeltaRX ]
                       } else {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO5Add $DeltaRY
                       }
                       # End C Commands !!!
                     }
      "O1, O3"       {
                       # Begin C Commands !!!
                       if { $ToolTipATT == "LBoth Axes" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO6Add $DeltaRX
                         $ObjectNavigator::CurrentNode ModifyProperty vPO4Add [ expr -$DeltaRY ]
                       } elseif { $ToolTipATT == "LFirst Axis" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO6Add $DeltaRX
                       } else {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO4Add [ expr -$DeltaRY ]
                       }
                       # End C Commands !!!
                     }
      "O2, O3"       {
                       # Begin C Commands !!!
                       if { $ToolTipATT == "LBoth Axes" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO6Add $DeltaRX
                         $ObjectNavigator::CurrentNode ModifyProperty vPO5Add $DeltaRY
                       } elseif { $ToolTipATT == "LFirst Axis" } {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO6Add $DeltaRX
                       } else {
                         $ObjectNavigator::CurrentNode ModifyProperty vPO5Add $DeltaRY
                       }
                       # End C Commands !!!
                     }
    }
    set MouseX $CurrentMouseX
    set MouseY $CurrentMouseY
  }

  #  BindtglWindowButtion3  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc BindtglWindowButton3 { CurrentMouseX CurrentMouseY } {
    variable MouseX
    variable MouseY
    set MouseX $CurrentMouseX
    set MouseY $CurrentMouseY
  }

  #  BindtglWindowB3Motion  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc BindtglWindowB3Motion { CurrentMouseX CurrentMouseY } {
    variable ToglWidget
    variable frmDPTB1
    variable MouseX
    variable MouseY
    variable Camera
    set DeltaTX [ expr ( $CurrentMouseX - $MouseX ) / 25.0 ]
    set DeltaTY [ expr ( $CurrentMouseY - $MouseY ) / 25.0 ]
	set DeltaRX [ expr ( $CurrentMouseX - $MouseX ) / 2.0 ]
	set DeltaRY [ expr ( $CurrentMouseY - $MouseY ) / 2.0 ]
    set ToolTipATT [ ToolBars::GetButtonToolTip $frmDPTB1 AxesToTransform ]
    switch $Camera {
      "Translate XY"     {
                           # Begin C Commands !!!
                           if { $ToolTipATT == "LBoth Axes" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterY [ expr -$DeltaTX ]
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterX [ expr -$DeltaTY ]
                           } elseif { $ToolTipATT == "LFirst Axis" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterY [ expr -$DeltaTX ]
                           } else {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterX [ expr -$DeltaTY ]
                           }
                           # End C Commands !!!
                         }
      "Translate XZ"         {
                           # Begin C Commands !!!
                           if { $ToolTipATT == "LBoth Axes" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterX $DeltaTX
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterZ $DeltaTY
                           } elseif { $ToolTipATT == "LFirst Axis" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterX $DeltaTX
                           } else {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterZ $DeltaTY
                           }
                           # End C Commands !!!
                         }
      "Translate YZ"     {
                           # Begin C Commands !!!
                           if { $ToolTipATT == "LBoth Axes" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterY [ expr -$DeltaTX ]
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterZ $DeltaTY
                           } elseif { $ToolTipATT == "LFirst Axis" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterY [ expr -$DeltaTX  ]
                           } else {
                             $ToglWidget toglSetToglDisplay toglSetCamera AddCenterZ $DeltaTY
                           }
                           # End C Commands !!!
                         }
      "Pan"              {
                           # Begin C Commands !!!
                           if { $ToolTipATT == "LBoth Axes" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera TranslateCenter [ expr -$DeltaTX ] $DeltaTY
                           } elseif { $ToolTipATT == "LFirst Axis" } {
                             $ToglWidget toglSetToglDisplay toglSetCamera TranslateCenter [ expr -$DeltaTX ] 0
                           } else {
                             $ToglWidget toglSetToglDisplay toglSetCamera TranslateCenter 0 $DeltaTY
                           }
                           # End C Commands !!!
                         }
      "Track Ball"       {
                           # Begin C Commands !!!
                           $ToglWidget toglSetToglDisplay toglSetCamera AddAzimuth [ expr -$DeltaRX ]
                           $ToglWidget toglSetToglDisplay toglSetCamera AddElevation [ expr -$DeltaRY ]
                           # End C Commands !!!
                         }
      "Twist"            {
                           # Begin C Commands !!!
                           $ToglWidget toglSetToglDisplay toglSetCamera AddTwist $DeltaRX
                           # End C Commands !!!
                         }
      "Zoom"             {
                           # Begin C Commands !!!
                           $ToglWidget toglSetToglDisplay toglSetCamera AddZoom $DeltaTX
                           # End C Commands !!!
                         }
    }
    set MouseX $CurrentMouseX
    set MouseY $CurrentMouseY
  }

  #  BindtglWindowDoubleButtion3  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc BindtglWindowDoubleButton3 { Geometry } {
    variable tplDP
    variable ToglWidget
    wm withdraw $tplDP
    if { $Geometry == "FullScreen" } {
      bind $ToglWidget <Double-Button-3> "DisplayWindow::BindtglWindowDoubleButton3 [ winfo geometry $tplDP ]"
      wm geometry $tplDP "[ winfo screenwidth . ]x[ winfo screenheight . ]+0+0"
      wm overrideredirect $tplDP 1
    } else {
      bind $ToglWidget <Double-Button-3> "DisplayWindow::BindtglWindowDoubleButton3 FullScreen"
      wm geometry $tplDP $Geometry
      wm overrideredirect $tplDP 0
    }
    wm deiconify $tplDP
    focus $tplDP
  }

  #  CommandSetProjectionType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetProjectionType {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 ProjectionType ]
    if { $ToolTip == "LOrthographic" } {
      ToolBars::SetButtonToolTip $frmDPTB1 ProjectionType "LPerspective"
      ToolBars::SetButtonImage $frmDPTB1 ProjectionType imgDPTBPerspective
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 ProjectionType ] "LPerspective"
      $ToglWidget toglSetToglDisplay toglSetProjectionType Perspective
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 ProjectionType "LOrthographic"
      ToolBars::SetButtonImage $frmDPTB1 ProjectionType imgDPTBOrthographic
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 ProjectionType ] "LOrthographic"
      $ToglWidget toglSetToglDisplay toglSetProjectionType Orthographic
    }
  }

  #  CommandSetShadeModel procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetShadeModel {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 ShadeModel ]
    if { $ToolTip == "LFlat" } {
      ToolBars::SetButtonToolTip $frmDPTB1 ShadeModel "LSmooth"
      ToolBars::SetButtonImage $frmDPTB1 ShadeModel imgDPTBSMSmooth
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 ShadeModel ] "LSmooth"
      $ToglWidget toglSetToglDisplay toglSetShadeModel Smooth
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 ShadeModel "LFlat"
      ToolBars::SetButtonImage $frmDPTB1 ShadeModel imgDPTBSMFlat
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 ShadeModel ] "LFlat"
      $ToglWidget toglSetToglDisplay toglSetShadeModel Flat
    }
  }

  #  CommandSetPolygonMode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetPolygonMode {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 PolygonMode ]
    if { $ToolTip == "LWireframe" } {
      ToolBars::SetButtonToolTip $frmDPTB1 PolygonMode "LHidden Lines"
      ToolBars::SetButtonImage $frmDPTB1 PolygonMode imgDPTBHiddenLines
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 PolygonMode ] "LHidden Lines"
      $ToglWidget toglSetToglDisplay toglSetPolygonMode HiddenLines
    } else {
      if { $ToolTip == "LHidden Lines" } {
        ToolBars::SetButtonToolTip $frmDPTB1 PolygonMode "LPreview"
        ToolBars::SetButtonImage $frmDPTB1 PolygonMode imgDPTBPreview
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 PolygonMode ] "LPreview"
        $ToglWidget toglSetToglDisplay toglSetPolygonMode Preview
        # Set Lighting On
        set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 Lighting ]
        if { $ToolTip == "LLighting Off" } {
          ToolBars::SetButtonToolTip $frmDPTB1 Lighting "LLighting On"
          ToolBars::SetButtonImage $frmDPTB1 Lighting imgDPTBLightingOn
          ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Lighting ] "LLighting On"
          $ToglWidget toglSetToglDisplay toglSetLightingMode On
        }
      } else {
        ToolBars::SetButtonToolTip $frmDPTB1 PolygonMode "LWireframe"
        ToolBars::SetButtonImage $frmDPTB1 PolygonMode imgDPTBWireframe
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 PolygonMode ] "LWireframe"
        $ToglWidget toglSetToglDisplay toglSetPolygonMode Wireframe
        # Set CSGMode off
        set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 CSGMode ]
        if { $ToolTip == "LCSG On" } {
          ToolBars::SetButtonToolTip $frmDPTB1 CSGMode "LCSG Off"
          ToolBars::SetButtonImage $frmDPTB1 CSGMode imgDPTBCSGOff
          ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 CSGMode ] "LCSG Off"
          $ToglWidget toglSetToglDisplay toglSetCSGMode Off
        }
      }
    }
  }

  #  CommandSetLighting procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetLighting {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 Lighting ]
    if { $ToolTip == "LLighting Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 Lighting "LLighting On"
      ToolBars::SetButtonImage $frmDPTB1 Lighting imgDPTBLightingOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Lighting ] "LLighting On"
      $ToglWidget toglSetToglDisplay toglSetLightingMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 Lighting "LLighting Off"
      ToolBars::SetButtonImage $frmDPTB1 Lighting imgDPTBLightingOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Lighting ] "LLighting Off"
      $ToglWidget toglSetToglDisplay toglSetLightingMode Off
      # Set PolygonMode Wireframe
      set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 PolygonMode ]
      if { $ToolTip == "LPreview" } {
        ToolBars::SetButtonToolTip $frmDPTB1 PolygonMode "LWireframe"
        ToolBars::SetButtonImage $frmDPTB1 PolygonMode imgDPTBWireframe
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 PolygonMode ] "LWireframe"
        $ToglWidget toglSetToglDisplay toglSetPolygonMode Wireframe
      }
      # Set CSGMode off
      set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 CSGMode ]
      if { $ToolTip == "LCSG On" } {
        ToolBars::SetButtonToolTip $frmDPTB1 CSGMode "LCSG Off"
        ToolBars::SetButtonImage $frmDPTB1 CSGMode imgDPTBCSGOff
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 CSGMode ] "LCSG Off"
        $ToglWidget toglSetToglDisplay toglSetCSGMode Off
      }
    }
  }

  #  CommandSetGrid procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetGrid {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 Grid ]
    if { $ToolTip == "LGrid Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 Grid "LGrid On"
      ToolBars::SetButtonImage $frmDPTB1 Grid imgDPTBGridOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Grid ] "LGrid On"
      $ToglWidget toglSetToglDisplay toglSetGridMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 Grid "LGrid Off"
      ToolBars::SetButtonImage $frmDPTB1 Grid imgDPTBGridOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Grid ] "LGrid Off"
      $ToglWidget toglSetToglDisplay toglSetGridMode Off
    }
  }

  #  CommandSetAxes procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetAxes {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 Axes ]
    if { $ToolTip == "LAxes Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 Axes "LAxes On"
      ToolBars::SetButtonImage $frmDPTB1 Axes imgDPTBAxesOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Axes ] "LAxes On"
      $ToglWidget toglSetToglDisplay toglSetAxesMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 Axes "LAxes Off"
      ToolBars::SetButtonImage $frmDPTB1 Axes imgDPTBAxesOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Axes ] "LAxes Off"
      $ToglWidget toglSetToglDisplay toglSetAxesMode Off
    }
  }

  #  CommandSetSolids procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetSolids {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 Solids ]
    if { $ToolTip == "LSolids Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 Solids "LSolids On"
      ToolBars::SetButtonImage $frmDPTB1 Solids imgDPTBSolidsOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Solids ] "LSolids On"
      $ToglWidget toglSetToglDisplay toglSetSolidsMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 Solids "LSolids Off"
      ToolBars::SetButtonImage $frmDPTB1 Solids imgDPTBSolidsOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Solids ] "LSolids Off"
      $ToglWidget toglSetToglDisplay toglSetSolidsMode Off
    }
  }

  #  CommandSetCSGMode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetCSGMode {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 CSGMode ]
    if { $ToolTip == "LCSG Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 CSGMode "LCSG On"
      ToolBars::SetButtonImage $frmDPTB1 CSGMode imgDPTBCSGOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 CSGMode ] "LCSG On"
      $ToglWidget toglSetToglDisplay toglSetCSGMode On
      # Set PolygonMode Preview
      set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 PolygonMode ]
      if { $ToolTip != "LPreview" } {
        ToolBars::SetButtonToolTip $frmDPTB1 PolygonMode "LPreview"
        ToolBars::SetButtonImage $frmDPTB1 PolygonMode imgDPTBPreview
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 PolygonMode ] "LPreview"
        $ToglWidget toglSetToglDisplay toglSetPolygonMode Preview
      }
      # Set Lighting On
      set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 Lighting ]
      if { $ToolTip == "LLighting Off" } {
        ToolBars::SetButtonToolTip $frmDPTB1 Lighting "LLighting On"
        ToolBars::SetButtonImage $frmDPTB1 Lighting imgDPTBLightingOn
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 Lighting ] "LLighting On"
        $ToglWidget toglSetToglDisplay toglSetLightingMode On
      }
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 CSGMode "LCSG Off"
      ToolBars::SetButtonImage $frmDPTB1 CSGMode imgDPTBCSGOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 CSGMode ] "LCSG Off"
      $ToglWidget toglSetToglDisplay toglSetCSGMode Off
    }
  }
  
   #  CommandSetCMMode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
 proc CommandSetCMMode {} {
    variable ToglWidget
    variable frmDPTB1
	
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 CMMode ]
	
    if { $ToolTip == "LCenter of Mass Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 CMMode "LCenter of Mass On"
      ToolBars::SetButtonImage $frmDPTB1 CMMode imgDPTBCMOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 CMMode ] "LCenter of Mass On"
      $ToglWidget toglSetToglDisplay toglSetCMMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 CMMode "LCenter of Mass Off"
      ToolBars::SetButtonImage $frmDPTB1 CMMode imgDPTBCMOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 CMMode ] "LCenter of Mass Off"
      $ToglWidget toglSetToglDisplay toglSetCMMode Off
    }
  }

  #  CommandSetSelectionMode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetSelectionMode {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 SelectionMode ]
    if { $ToolTip == "LSelection Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 SelectionMode "LSelection On"
      ToolBars::SetButtonImage $frmDPTB1 SelectionMode imgDPTBSelectionOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 SelectionMode ] "LSelection On"
      $ToglWidget toglSetToglDisplay toglSetSelectionMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 SelectionMode "LSelection Off"
      ToolBars::SetButtonImage $frmDPTB1 SelectionMode imgDPTBSelectionOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 SelectionMode ] "LSelection Off"
      $ToglWidget toglSetToglDisplay toglSetSelectionMode Off
    }
  }

  #  CommandSetFollowMode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  
proc CommandSetFollowMode {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 FollowMode ]
    if { $ToolTip == "LFollow Off" } {
      ToolBars::SetButtonToolTip $frmDPTB1 FollowMode "LFollow On"
      ToolBars::SetButtonImage $frmDPTB1 FollowMode imgDPTBFollowOn
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 FollowMode ] "LFollow On"
      $ToglWidget toglSetToglDisplay toglSetFollowMode On
    } else {
      ToolBars::SetButtonToolTip $frmDPTB1 FollowMode "LFollow Off"
      ToolBars::SetButtonImage $frmDPTB1 FollowMode imgDPTBFollowOff
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 FollowMode ] "LFollow Off"
      $ToglWidget toglSetToglDisplay toglSetFollowMode Off
    }
  }

  #  CommandSetTransformation procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetTransformation { Text } {
    variable frmDPTB2
    variable Transformation
    variable OldTransformation
    set Transformation $Text
    set OldTransformation $Transformation
    $frmDPTB2.lblTransformation configure -text $Text
  }

  #  CommandSetAxesToTransform procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetAxesToTransform {} {
    variable ToglWidget
    variable frmDPTB1
    set ToolTip [ ToolBars::GetButtonToolTip $frmDPTB1 AxesToTransform ]
    if { $ToolTip == "LBoth Axes" } {
      ToolBars::SetButtonToolTip $frmDPTB1 AxesToTransform "LFirst Axis"
      ToolBars::SetButtonImage $frmDPTB1 AxesToTransform imgDPTBTFirst
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 AxesToTransform ] "LFirst Axis"
    } else {
      if { $ToolTip == "LFirst Axis" } {
        ToolBars::SetButtonToolTip $frmDPTB1 AxesToTransform "LSecond Axis"
        ToolBars::SetButtonImage $frmDPTB1 AxesToTransform imgDPTBTSecond
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 AxesToTransform ] "LSecond Axis"
      } else {
        ToolBars::SetButtonToolTip $frmDPTB1 AxesToTransform "LBoth Axes"
        ToolBars::SetButtonImage $frmDPTB1 AxesToTransform imgDPTBTBoth
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmDPTB1 AxesToTransform ] "LBoth Axes"
      }
    }
  }

  #  CommandSetCamera procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetCamera { Text } {
    variable frmDPTB3
    variable Camera
    set Camera $Text
    $frmDPTB3.lblCamera configure -text $Text
  }

  #  RenderWindow procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc RenderWindow {} {
    variable ToglWidget
    $ToglWidget render
  }

  #  EnableAllToolBarTwoButtons procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc EnableAllToolBarTwoButtons {} {
    variable frmDPTB2
    variable Transformation
    variable OldTransformation
    ToolBars::EnableButton $frmDPTB2 TranslateXY
    ToolBars::EnableButton $frmDPTB2 TranslateXZ
    ToolBars::EnableButton $frmDPTB2 TranslateYZ
    ToolBars::EnableButton $frmDPTB2 RotateXY
    ToolBars::EnableButton $frmDPTB2 RotateZX
    ToolBars::EnableButton $frmDPTB2 RotateZY
    set Transformation $OldTransformation
    $frmDPTB2.lblTransformation configure -text "$OldTransformation"
  }

  # DisableAllToolBarTwoButtons procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DisableAllToolBarTwoButtons {} {
    variable frmDPTB2
    variable Transformation
    variable OldTransformation
    ToolBars::DisableButton $frmDPTB2 TranslateXY
    ToolBars::DisableButton $frmDPTB2 TranslateXZ
    ToolBars::DisableButton $frmDPTB2 TranslateYZ
    ToolBars::DisableButton $frmDPTB2 RotateXY
    ToolBars::DisableButton $frmDPTB2 RotateZX
    ToolBars::DisableButton $frmDPTB2 RotateZY
    if { $Transformation != "" } {
      set OldTransformation $Transformation
    }
    $frmDPTB2.lblTransformation configure -text ""
    set Transformation ""
  }

  # DisableAllToolBarTwoButtons procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplDP
    variable frmDPTB1
    variable frmDPTB2
    variable frmDPTB3
    variable ToglWidget
    # Initialization of display window
    RobLab::CreateWindow "" wndDisplayWindow "Display" "$RobLab::DP_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $DisplayWindow::tplDP } $RobLab::VirtualGridSize
    # Tool Bars
    frame $frmDPTB1 -width 26
    togl $ToglWidget -width 2 -height 1 -rgba true -double true -depth true -accum true -stencil true
    frame $frmDPTB3 -height 26
    label $frmDPTB3.lblCamera -text "Track Ball" -width 10
    frame $frmDPTB2 -height 26
    label $frmDPTB2.lblTransformation -text "" -width 10
    pack $frmDPTB1 -side left -fill y
    pack $ToglWidget -fill both -expand true -padx 2 -pady 2
    pack $frmDPTB3 -side bottom -fill x
    pack $frmDPTB3.lblCamera -side left -padx 6
    pack $frmDPTB2 -side bottom -fill x
    pack $frmDPTB2.lblTransformation -side left -padx 6
    # Initialization of tool bar one (Vertical ToolBar)
    set DPToolBar      { {             Top               "LTop"              imgDPTBTop     normal {} }
                         {            Left              "LLeft"             imgDPTBLeft     normal {} }
                         {           Right             "LRight"            imgDPTBRight     normal {} }
                         {            User              "LUser"             imgDPTBUser     normal {} }
                         {        AllViews         "LAll Views"         imgDPTBAllViews     normal {} }
                         { "" }
                         {  ProjectionType      "LOrthographic"     imgDPTBOrthographic     normal {} }
                         {      ShadeModel            "LSmooth"         imgDPTBSMSmooth     normal {} }
                         {     PolygonMode           "LPreview"          imgDPTBPreview     normal {} }
                         {        Lighting       "LLighting On"       imgDPTBLightingOn     normal {} }
						 { "" }
						 {          Solids         "LSolids On"         imgDPTBSolidsOn     normal {} }
						 {         CSGMode           "LCSG Off"           imgDPTBCSGOff  normal {} }
						 {            Axes           "LAxes On"           imgDPTBAxesOn     normal {} }
                         {            Grid           "LGrid On"           imgDPTBGridOn     normal {} }
						 {   CMMode      "LCenter of Mass Off"    imgDPTBCMOff     normal {} }
                         {   SelectionMode      "LSelection Off"    imgDPTBSelectionOff     normal {} }
						 { "" }
                         {      FollowMode        "LFollow Off"        imgDPTBFollowOff     normal {} }
                         { AxesToTransform         "LBoth Axes"            imgDPTBTBoth     normal {} } }
    ToolBars::Create $frmDPTB1 $DPToolBar Vertical
    ToolBars::SetButtonCommand $frmDPTB1             Top "$ToglWidget toglSetToglDisplay toglSetView Top"
    ToolBars::SetButtonCommand $frmDPTB1            Left "$ToglWidget toglSetToglDisplay toglSetView Left"
    ToolBars::SetButtonCommand $frmDPTB1           Right "$ToglWidget toglSetToglDisplay toglSetView Right"
    ToolBars::SetButtonCommand $frmDPTB1            User "$ToglWidget toglSetToglDisplay toglSetView User"
    ToolBars::SetButtonCommand $frmDPTB1        AllViews "$ToglWidget toglSetToglDisplay toglSetView All"
    ToolBars::SetButtonCommand $frmDPTB1  ProjectionType "DisplayWindow::CommandSetProjectionType"
    ToolBars::SetButtonCommand $frmDPTB1      ShadeModel "DisplayWindow::CommandSetShadeModel"
    ToolBars::SetButtonCommand $frmDPTB1     PolygonMode "DisplayWindow::CommandSetPolygonMode"
    ToolBars::SetButtonCommand $frmDPTB1        Lighting "DisplayWindow::CommandSetLighting"
    ToolBars::SetButtonCommand $frmDPTB1            Grid "DisplayWindow::CommandSetGrid"
    ToolBars::SetButtonCommand $frmDPTB1            Axes "DisplayWindow::CommandSetAxes"
    ToolBars::SetButtonCommand $frmDPTB1          Solids "DisplayWindow::CommandSetSolids"
    ToolBars::SetButtonCommand $frmDPTB1         CSGMode "DisplayWindow::CommandSetCSGMode"
	ToolBars::SetButtonCommand $frmDPTB1   CMMode "DisplayWindow::CommandSetCMMode"
    ToolBars::SetButtonCommand $frmDPTB1   SelectionMode "DisplayWindow::CommandSetSelectionMode"
    ToolBars::SetButtonCommand $frmDPTB1      FollowMode "DisplayWindow::CommandSetFollowMode"
    ToolBars::SetButtonCommand $frmDPTB1 AxesToTransform "DisplayWindow::CommandSetAxesToTransform"
    # Initialization of tool bar two
    set DPToolBarTwo   { {     TranslateXY            "RP1, P2"             imgDPTBP1P2   disabled {} }
                         {     TranslateXZ            "RP1, P3"             imgDPTBP1P3   disabled {} }
                         {     TranslateYZ            "RP2, P3"             imgDPTBP2P3   disabled {} }
                         { "" }
                         {        RotateXY            "RO1, O2"             imgDPTBO1O2   disabled {} }
                         {        RotateZX            "RO1, O3"             imgDPTBO1O3   disabled {} }
                         {        RotateZY            "RO2, O3"             imgDPTBO2O3   disabled {} } }
    ToolBars::Create $frmDPTB2 $DPToolBarTwo Horizontal
    ToolBars::SetButtonCommand $frmDPTB2 TranslateXY "DisplayWindow::CommandSetTransformation {P1, P2}"
    ToolBars::SetButtonCommand $frmDPTB2 TranslateXZ "DisplayWindow::CommandSetTransformation {P1, P3}"
    ToolBars::SetButtonCommand $frmDPTB2 TranslateYZ "DisplayWindow::CommandSetTransformation {P2, P3}"
    ToolBars::SetButtonCommand $frmDPTB2    RotateXY "DisplayWindow::CommandSetTransformation {O1, O2}"
    ToolBars::SetButtonCommand $frmDPTB2    RotateZX "DisplayWindow::CommandSetTransformation {O1, O3}"
    ToolBars::SetButtonCommand $frmDPTB2    RotateZY "DisplayWindow::CommandSetTransformation {O2, O3}"
    # Initialization of tool bar three
    set DPToolBarThree { {     TranslateXY      "RTranslate XY"             imgDPTBP1P2     normal {} }
                         {     TranslateXZ      "RTranslate XZ"             imgDPTBP1P3     normal {} }
                         {     TranslateYZ      "RTranslate YZ"             imgDPTBP2P3     normal {} }
                         { "" }
                         {             Pan               "RPan"              imgDPTBPan     normal {} }
                         {       TrackBall        "RTrack Ball"        imgDPTBTrackBall     normal {} }
                         {           Twist             "RTwist"            imgDPTBTwist     normal {} }
                         {            Zoom              "RZoom"             imgDPTBZoom     normal {} } }
    ToolBars::Create $frmDPTB3 $DPToolBarThree Horizontal
    ToolBars::SetButtonCommand $frmDPTB3 TranslateXY "$ToglWidget toglSetToglDisplay toglSetView Top; DisplayWindow::CommandSetCamera {Translate XY}"
    ToolBars::SetButtonCommand $frmDPTB3 TranslateXZ "$ToglWidget toglSetToglDisplay toglSetView Right; DisplayWindow::CommandSetCamera {Translate XZ}"
    ToolBars::SetButtonCommand $frmDPTB3 TranslateYZ "$ToglWidget toglSetToglDisplay toglSetView Left; DisplayWindow::CommandSetCamera {Translate YZ}"
    ToolBars::SetButtonCommand $frmDPTB3        Pan "DisplayWindow::CommandSetCamera {Pan}"
    ToolBars::SetButtonCommand $frmDPTB3  TrackBall "DisplayWindow::CommandSetCamera {Track Ball}"
    ToolBars::SetButtonCommand $frmDPTB3      Twist "DisplayWindow::CommandSetCamera {Twist}"
    ToolBars::SetButtonCommand $frmDPTB3       Zoom "DisplayWindow::CommandSetCamera {Zoom}"
    # Initialization & Bindings
    $ToglWidget toglSetToglDisplay toglSetView User
    bind $ToglWidget  <Double-Button-1> "DisplayWindow::BindtglWindowDoubleButton1"
    bind $ToglWidget         <Button-1> "DisplayWindow::BindtglWindowButton1       %x %y"
    bind $ToglWidget        <B1-Motion> "DisplayWindow::BindtglWindowB1Motion      %x %y"
    bind $ToglWidget         <Button-3> "DisplayWindow::BindtglWindowButton3       %x %y"
    bind $ToglWidget        <B3-Motion> "DisplayWindow::BindtglWindowB3Motion      %x %y"
    bind $ToglWidget  <Double-Button-3> "DisplayWindow::BindtglWindowDoubleButton3 FullScreen"
  }

}

