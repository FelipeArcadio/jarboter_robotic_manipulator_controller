# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : PropertyPalette.tcl
#  Date         : 2001-11-09
#  Description  : Implementation of property palette manager
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  PropertyPalette.tcl
#		This file implements the property palette manager.
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

  # Images of property palette canvas
  image create photo imgCVSIcon20 -file ./Images/PropertyPalette/Icon20.gif
  image create photo imgCVSIcon21 -file ./Images/PropertyPalette/Icon21.gif
  image create photo imgCVSIcon22 -file ./Images/PropertyPalette/Icon22.gif
  image create photo imgCVSIcon23 -file ./Images/PropertyPalette/Icon23.gif
  image create photo imgCVSIcon24 -file ./Images/PropertyPalette/Icon24.gif

# --------------------------------------------------------------------------------------------------------------------------

namespace eval PropertyPalette {

  # Procedures :
  # proc UpdatePropeties {}
  # proc Bind_entPP_Return {}
  # proc Bind_lstPP_Return {}
  # proc DisplayPropertiesNTree { NTree }
  # proc ScalePPCommand { Value }
  # proc SpinPPCommand { { TextA "" } { Units 0 } { TextB "" } }
  # proc ValidateToolBarButtons {}
  # proc GetNTreeFromPropertyName { NTree PropertyName }
  # proc ChangePropertyTitleValue { NTree PropertyName Title Value ListOfElements }
  # proc ChangePropertyValue { NTree PropertyName Value }
  # proc ChangeNodePropertyValue { NTree Value }
  # proc Initialize {}

  # Initialization of global variables
  variable tplPP .wndPropertyPalette
  variable cvsPP $tplPP.frmPP_A.cvsPropertyPalette
  variable entPP $tplPP.frmPP_A.frmPP_C.entPropertyValue
  variable lstPP $tplPP.frmPP_A.frmPP_C.lstPropertyValue
  variable frmAD $tplPP.frmPP_A.frmPP_D
  variable sclPP $tplPP.frmPP_A.frmPP_D.sclScale
  variable spnPP $tplPP.frmPP_A.frmPP_D.frmPP_E
  variable frmPPTB $tplPP.frmToolBar
  variable CurrentNode $NTree::Null
  variable GeneralFormatReal "%.8f"

  # UpdateProperties procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc UpdatePropeties {} {
    variable entPP
    variable lstPP
    variable frmAD
    variable sclPP
    variable spnPP
    variable CurrentNode
    $entPP delete 0 end
    $lstPP delete 0 end
    if { [ TkNTree::GetUpdateAllowed $CurrentNode ] == $TkNTree::True } {
      set PropertyType [ ObjectNavigator::GetPropertyType $CurrentNode ]
      set PropertyValue [ ObjectNavigator::GetPropertyValue $CurrentNode ]
      if { $PropertyType == "List" } {
        set PropertyListOfElements [ ObjectNavigator::GetPropertyListOfElements $CurrentNode ]
        $entPP configure -takefocus 0 -state disabled
        pack forget $entPP
        pack $lstPP -side top -fill x
        $lstPP configure -takefocus 1
        eval $lstPP insert 0 $PropertyListOfElements
        focus $lstPP
        pack forget $sclPP
        $frmAD configure -height 2
      } else {
        $lstPP configure -takefocus 0
        pack forget $lstPP
        pack $entPP -side top -fill x
        $entPP configure -takefocus 1 -state normal
        focus $entPP
        $entPP insert 0 $PropertyValue
        $entPP selection range 0 end
        if { $PropertyType == "FileName" } {
          bind $entPP <Key> { break }
        } else {
          bind $entPP <Key> { continue }
        }
        if { $PropertyType == "Scale" } {
          set MinMaxResValue [ ObjectNavigator::GetPropertyListOfElements $CurrentNode ]
          $sclPP configure -from [ lindex $MinMaxResValue 0 ]
          $sclPP configure -to [ lindex $MinMaxResValue 1 ]
          $sclPP configure -resolution [ lindex $MinMaxResValue 2 ]
          $sclPP set $PropertyValue
          pack $sclPP -side top -fill x
        } else {
          pack forget $sclPP
          $frmAD configure -height 2
        }
        if { $PropertyType == "Spin" } {
          set HighLowIncrValues [ ObjectNavigator::GetPropertyListOfElements $CurrentNode ]
          $spnPP.entSpinA insert 0 [ lindex $HighLowIncrValues 0 ]
          $spnPP.entSpinB insert 0 [ lindex $HighLowIncrValues 1 ]
          pack $spnPP -side left -fill both -expand true
        } else {
          pack forget $spnPP
          $frmAD configure -height 2
        }
      }
    } else {
      pack forget $entPP
      pack forget $lstPP
      pack forget $sclPP
      $frmAD configure -height 2
    }
  }

  # Bind_entPP_Return procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Bind_entPP_Return {} {
    variable tplPP
    variable cvsPP
    variable entPP
    variable sclPP
    variable CurrentNode
    variable GeneralFormatReal
    set PropertyName [ ObjectNavigator::GetPropertyName $CurrentNode ]
    set PropertyType [ ObjectNavigator::GetPropertyType $CurrentNode ]
    set PropertyValue [ $entPP get ]
    if { [ TkNTree::GetUpdateAllowed $CurrentNode ] == $TkNTree::True } {
      set Ok 1
      $entPP selection range 0 end
      if { $PropertyType == "Number" || $PropertyType == "Scale" } {
        if { [ RobLab::IsReal "$PropertyValue" ] == 0 } {
          set Ok 0
          MessagesWindow::Puts "Error : Can not set value, \"$PropertyValue\" is not a real number.\n\n"
        } elseif { $PropertyType == "Scale" } {
          set MinMaxResValue [ ObjectNavigator::GetPropertyListOfElements $CurrentNode ]
          if { ! ( ( $PropertyValue >= [ lindex $MinMaxResValue 0 ] ) && ( $PropertyValue <= [ lindex $MinMaxResValue 1 ] ) ) } {
            set Ok 0
            MessagesWindow::Puts "Error : Can not set value, $PropertyValue is not a number between [ lindex $MinMaxResValue 0 ] and [ lindex $MinMaxResValue 1 ].\n\n"
          }
        }
        if { $Ok == 1 } {
          set PropertyValue [ format $GeneralFormatReal $PropertyValue ]
        }
      } elseif { $PropertyType == "FileName" } {
        set TypeExtension [ ObjectNavigator::GetPropertyListOfElements $CurrentNode ]
        set FileName [ObjectNavigator::GetOpenFileName [ lindex $TypeExtension 0 ] [ lindex $TypeExtension 1 ] $tplPP ]
        if { $FileName != "" } {
          set PropertyValue $FileName
          $entPP delete 0 end
          $entPP insert 0 $PropertyValue
          focus $entPP
        }
      }
      if { $Ok == 1 } {
        ObjectNavigator::SetPropertyValue $CurrentNode $PropertyValue
        set PropertySecondaryTitle [ ObjectNavigator::GetPropertySecondaryTitle $CurrentNode ]
        if { $PropertySecondaryTitle == "" } {
          TkNTree::SetTitle $CurrentNode "$PropertyName = $PropertyValue"
        } else {
          TkNTree::SetTitle $CurrentNode "$PropertySecondaryTitle = $PropertyValue"
        }
        TkNTree::SetGraphicTitle $cvsPP $CurrentNode [ TkNTree::GetTitle $CurrentNode ]
        if { $PropertyType == "Scale" } {
          $sclPP set $PropertyValue
        }
        if { $PropertyType == "FileName" } {
          if { $FileName != "" } {
            # Begin C Commands !!!
            [ ObjectNavigator::GetPropertyParent $CurrentNode ] ModifyProperty $PropertyName $PropertyValue
            # End C Commands !!!
          }
        } else {
          # Begin C Commands !!!
          [ ObjectNavigator::GetPropertyParent $CurrentNode ] ModifyProperty $PropertyName $PropertyValue
          # End C Commands !!!
        }
        DisplayWindow::RenderWindow
      }
    }
  }

  # Bind_lstPP_Return procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Bind_lstPP_Return {} {
    variable cvsPP
    variable lstPP
    variable CurrentNode
    set PropertyName [ ObjectNavigator::GetPropertyName $CurrentNode ]
    set PropertyType [ ObjectNavigator::GetPropertyType $CurrentNode ]
    if { [ TkNTree::GetUpdateAllowed $CurrentNode ] == $TkNTree::True } {
      set CurrentSelection [ $lstPP curselection ]
      if { $CurrentSelection != "" } {
        set PropertyValue [ $lstPP get $CurrentSelection ]
        ObjectNavigator::SetPropertyValue $CurrentNode $PropertyValue
        set PropertySecondaryTitle [ ObjectNavigator::GetPropertySecondaryTitle $CurrentNode ]
        if { $PropertySecondaryTitle == "" } {
          TkNTree::SetTitle $CurrentNode "$PropertyName = $PropertyValue"
        } else {
          TkNTree::SetTitle $CurrentNode "$PropertySecondaryTitle = $PropertyValue"
        }
        TkNTree::SetGraphicTitle $cvsPP $CurrentNode [ TkNTree::GetTitle $CurrentNode ]
        # Begin C Commands !!!
        [ ObjectNavigator::GetPropertyParent $CurrentNode ] ModifyProperty $PropertyName $PropertyValue
        # End C Commands !!!
        DisplayWindow::RenderWindow
        if { $PropertyType == "Link" } {
          set dNode [ NTree::GetPreviousBrother [ NTree::GetPreviousBrother [ NTree::GetPreviousBrother $CurrentNode ] ] ]
          set ThetaNode [ NTree::GetPreviousBrother $dNode ]
        } else {
          if { $PropertyType == "Dynamic" } {
            ObjectNavigator::ChangeDynamicPropertiesToElementsNTree $PropertyValue
          }
        }
      }
    }
  }

  #  DisplayPropertiesNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DisplayPropertiesNTree { NTree } {
    variable cvsPP
    variable entPP
    variable lstPP
    variable frmAD
    variable sclPP
    variable spnPP
    variable CurrentNode
    $entPP delete 0 end
    $lstPP delete 0 end
    $entPP configure -takefocus 0 -state disabled
    $lstPP configure -takefocus 0
    pack forget $entPP
    pack forget $lstPP
    pack forget $sclPP
    $frmAD configure -height 2
    if { ! [ NTree::Empty $CurrentNode ] } {
      TkNTree::DeleteGraphicNode $cvsPP all
      # This is used to show propertties next time be expanding the tree
      TkNTree::SetState [ NTree::Root $CurrentNode ] StateCollapse
    }
    set CurrentNode $NTree
    if { ! [ NTree::Empty $NTree ] } {
      TkNTree::AddGraphicNode $cvsPP $NTree 0 0
      TkNTree::ExpandNTree $cvsPP $NTree
      TkNTree::InitializeSelection $cvsPP $NTree
    }
    ValidateToolBarButtons
  }

  # ScalePPCommand procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ScalePPCommand { Value } {
    variable cvsPP
    variable entPP
    variable CurrentNode
    set PropertyName [ ObjectNavigator::GetPropertyName $CurrentNode ]
    set PropertyValue [ ObjectNavigator::GetPropertyValue $CurrentNode ]
    if { $PropertyValue != $Value } {
      $entPP delete 0 end
      $entPP insert 0 $Value
      $entPP selection range 0 end
      ObjectNavigator::SetPropertyValue $CurrentNode $Value
      set PropertySecondaryTitle [ ObjectNavigator::GetPropertySecondaryTitle $CurrentNode ]
      if { $PropertySecondaryTitle == "" } {
        TkNTree::SetTitle $CurrentNode "$PropertyName = $Value"
      } else {
        TkNTree::SetTitle $CurrentNode "$PropertySecondaryTitle = $Value"
      }
      TkNTree::SetGraphicTitle $cvsPP $CurrentNode [ TkNTree::GetTitle $CurrentNode ]
      # Begin C Commands !!!
      [ ObjectNavigator::GetPropertyParent $CurrentNode ] ModifyProperty $PropertyName $Value
      # End C Commands !!!
      DisplayWindow::RenderWindow
    }
  }

  # SpinPPCommand procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SpinPPCommand { { TextA "" } { Units 0 } { TextB "" } } {
    MessagesWindow::Puts "-$TextA-, -$Units-, -$TextB- ...\n"
  }

  #  ValidateToolBarButtons procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ValidateToolBarButtons {} {
    variable cvsPP
    variable frmPPTB
    variable CurrentNode
    if { ! [ NTree::Empty $CurrentNode ] } {
      set CurrentNodeState [ TkNTree::GetState $CurrentNode ]
      if { $CurrentNodeState == $TkNTree::StateNone } {
        ToolBars::DisableButton $frmPPTB Expand
        ToolBars::DisableButton $frmPPTB Collapse
        ToolBars::DisableButton $frmPPTB ExpandAll
        ToolBars::DisableButton $frmPPTB CollapseAll
      } else {
        ToolBars::EnableButton $frmPPTB ExpandAll
        ToolBars::EnableButton $frmPPTB CollapseAll
        if { $CurrentNodeState == $TkNTree::StateExpand } {
          ToolBars::DisableButton $frmPPTB Expand
          ToolBars::EnableButton $frmPPTB Collapse
        } else {
          ToolBars::EnableButton $frmPPTB Expand
          ToolBars::DisableButton $frmPPTB Collapse
        }
      }
      TkNTree::SeeCanvasNode $cvsPP $CurrentNode
    } else {
      ToolBars::DisableButton $frmPPTB Expand
      ToolBars::DisableButton $frmPPTB Collapse
      ToolBars::DisableButton $frmPPTB ExpandAll
      ToolBars::DisableButton $frmPPTB CollapseAll
    }
  }

  #  GetNTreeFromPropertyName procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNTreeFromPropertyName { NTree PropertyName } {
    set NewNTree $NTree::Null
    if { ! [ NTree::Empty $NTree ] } {
      if { "$PropertyName" == "[ ObjectNavigator::GetPropertyName $NTree ]" } {
        return $NTree
      }
      set TemporaryNTree [ NTree::GetFirstSon $NTree ]
      while { ( ! [ NTree::Empty $TemporaryNTree ] ) && ( [ NTree::Empty $NewNTree ] ) } {
        set NewNTree [ GetNTreeFromPropertyName $TemporaryNTree $PropertyName ]
        set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
      }
    }
    return $NewNTree
  }

  # ChangePropertyTitleValue procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangePropertyTitleValue { NTree PropertyName Title Value ListOfElements } {
    # NTree is an ObjectNavigator node and first we have to get its properties NTree
    variable cvsPP
    set PropertiesNTree [ ObjectNavigator::GetPropertiesNTree $NTree ]
    set PropertiesNTree [ GetNTreeFromPropertyName $PropertiesNTree $PropertyName ]
    if { ! [ NTree::Empty $PropertiesNTree ] } {
      ObjectNavigator::SetPropertySecondaryTitle $PropertiesNTree $Title
      ObjectNavigator::SetPropertyValue $PropertiesNTree $Value
      ObjectNavigator::SetPropertyListOfElements $PropertiesNTree "$ListOfElements"
      TkNTree::SetTitle $PropertiesNTree "$Title = $Value"
      TkNTree::SetGraphicTitle $cvsPP $PropertiesNTree [ TkNTree::GetTitle $PropertiesNTree ]
    }
  }

  # GetNTreePropertyValue procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNTreePropertyValue { NTree PropertyName } {
    # NTree is an ObjectNavigator node and first we have to get its Properties NTree
    variable cvsPP
    set PropertiesNTree [ ObjectNavigator::GetPropertiesNTree $NTree ]
    set PropertiesNTree [ GetNTreeFromPropertyName $PropertiesNTree $PropertyName ]
    if { ! [ NTree::Empty $PropertiesNTree ] } {
      return [ ObjectNavigator::GetPropertyValue $PropertiesNTree ]
    }
  }

  # AddPropertyTitleValue procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddPropertyTitleValue { NTree PropertyName Title Value ListOfElements } {
    # NTree is an ObjectNavigator node and first we have to get its properties NTree
    variable cvsPP
    set PropertiesNTree [ ObjectNavigator::GetPropertiesNTree $NTree ]
    set PropertiesNTree [ GetNTreeFromPropertyName $PropertiesNTree $PropertyName ]
    if { ! [ NTree::Empty $PropertiesNTree ] } {
      ObjectNavigator::SetPropertySecondaryTitle $PropertiesNTree $Title
      ObjectNavigator::SetPropertyValue $PropertiesNTree $Value
      ObjectNavigator::SetPropertyListOfElements $PropertiesNTree "$ListOfElements"
      TkNTree::SetTitle $PropertiesNTree "$Title = $Value"
      TkNTree::SetGraphicTitle $cvsPP $PropertiesNTree [ TkNTree::GetTitle $PropertiesNTree ]
    }
  }

  # ChangePropertyValue procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangePropertyValue { NTree PropertyName Value } {
    # NTree is a PropertyPalette node that is the properties NTree root
    variable cvsPP
    set NTree [ GetNTreeFromPropertyName $NTree $PropertyName ]
    ChangeNodePropertyValue $NTree $Value
  }

  # ChangeNodePropertyValue procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeNodePropertyValue { NTree Value } {
    # NTree is a PropertyPalette node
    variable cvsPP
    if { ! [ NTree::Empty $NTree ] } {
      set PropertyValue [ ObjectNavigator::GetPropertyValue $NTree ]
      if { $PropertyValue != $Value } {
        ObjectNavigator::SetPropertyValue $NTree $Value
        set PropertyName [ ObjectNavigator::GetPropertyName $NTree ]
        set PropertySecondaryTitle [ ObjectNavigator::GetPropertySecondaryTitle $NTree ]
        if { $PropertySecondaryTitle == "" } {
          TkNTree::SetTitle $NTree "$PropertyName = $Value"
        } else {
          TkNTree::SetTitle $NTree "$PropertySecondaryTitle = $Value"
        }
        TkNTree::SetGraphicTitle $cvsPP $NTree [ TkNTree::GetTitle $NTree ]
      }
    }
  }

  # Initialize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplPP
    variable cvsPP
    variable entPP
    variable lstPP
    variable sclPP
    variable spnPP
    variable frmPPTB
    # Initialization of property-palette window
    RobLab::CreateWindow "" wndPropertyPalette "Properties Palette" "$RobLab::PP_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $PropertyPalette::tplPP } $RobLab::VirtualGridSize
    frame $tplPP.frmPP_A
    frame $tplPP.frmPP_B -borderwidth 1 -width 13 -height 13
    frame $tplPP.frmPP_A.frmPP_C -borderwidth 1 -relief sunken -width 13 -height 17
    frame $tplPP.frmPP_A.frmPP_D -borderwidth 1 -relief sunken -width 13 -height 2

    frame $spnPP -borderwidth 0 -relief flat -width 13 -height 2

    scrollbar $tplPP.frmPP_A.scbHorizontal -borderwidth 1 -orient horiz -command "$cvsPP xview"
    scrollbar $tplPP.scbVertical -borderwidth 1 -orient verti -command "$cvsPP yview"
    frame $tplPP.frmToolBar -width 26
    canvas $cvsPP -borderwidth 1 -relief sunken -width 17 -height 17 -background #fffffffff -xscrollcommand "$tplPP.frmPP_A.scbHorizontal set" -yscrollcommand "$tplPP.scbVertical set" -xscrollincrement 17 -yscrollincrement 17 -confine true -scrollregion { 0 0 0 0 } -highlightthickness 0
    entry $entPP -borderwidth 0 -background #fffffffff -foreground #000000000
    listbox $lstPP -background #fffffffff -foreground #000000000 -borderwidth 0 -selectborderwidth 0 -height 1 -selectforeground #000000000 -selectbackground #fffffffff -highlightcolor #fffffffff -relief flat
    scale $sclPP -borderwidth 0 -resolution 1 -orient horizontal -showvalue 0 -command { PropertyPalette::ScalePPCommand }

    scrollbar $spnPP.scbSpin -borderwidth 1 -orient horiz -command { PropertyPalette::SpinPPCommand }
    $spnPP.scbSpin set 0.5 0.5
    entry $spnPP.entSpinA -borderwidth 1 -background #fffffffff -foreground #000000000 -justify right -width 10
    entry $spnPP.entSpinB -borderwidth 1 -background #fffffffff -foreground #000000000 -justify right -width 10

    pack $tplPP.frmToolBar -side left -fill y
    pack $tplPP.frmPP_A -side left -fill both -expand true
    pack $tplPP.frmPP_A.scbHorizontal -side bottom -fill x
    pack $tplPP.frmPP_A.frmPP_C -side bottom -fill x -pady 1
    pack $tplPP.frmPP_A.frmPP_D -side bottom -fill x -pady 1

    pack $spnPP.entSpinB -side right -fill both -expand true -padx 1 -pady 1
    pack $spnPP.entSpinA -side right -fill both -expand true -padx 1 -pady 1
    pack $spnPP.scbSpin -side right -fill both -expand true -padx 2 -pady 2

    pack $cvsPP -side top -fill both -expand true
    pack $tplPP.frmPP_B -side bottom
    pack $tplPP.scbVertical -side left -fill y
    update
    # Initialization of binds
    bind $tplPP <KeyPress> "TkNTree::MoveCurrentNode %K $cvsPP PropertyPalette::CurrentNode {PropertyPalette::UpdatePropeties; PropertyPalette::ValidateToolBarButtons}"
    $cvsPP bind all <Button-1> "TkNTree::Canvas_Bind_All_Button1 $cvsPP PropertyPalette::CurrentNode {PropertyPalette::UpdatePropeties; PropertyPalette::ValidateToolBarButtons}"
    bind $cvsPP <Configure> "TkNTree::Bind_Canvas_Configure $cvsPP PropertyPalette::CurrentNode"
    bind $entPP <Return> { PropertyPalette::Bind_entPP_Return }
    bind $lstPP <Return> { PropertyPalette::Bind_lstPP_Return }
    # Initialization of property-palette tool bar
    set PPToolBar { {      Expand       "RExpand"      imgONTBExpand disabled {} }
                    {    Collapse     "RCollapse"    imgONTBCollapse disabled {} }
                    {   ExpandAll   "RExpand All"   imgONTBExpandAll disabled {} }
                    { CollapseAll "RCollapse All" imgONTBCollapseAll disabled {} } }
    ToolBars::Create $frmPPTB $PPToolBar Vertical
    ToolBars::SetButtonCommand $frmPPTB      Expand "RobLab::ExpandNTree $cvsPP $frmPPTB PropertyPalette::CurrentNode"
    ToolBars::SetButtonCommand $frmPPTB    Collapse "RobLab::CollapseNTree $cvsPP $frmPPTB PropertyPalette::CurrentNode; focus $cvsPP"
    ToolBars::SetButtonCommand $frmPPTB   ExpandAll "RobLab::ExpandAllNTree $cvsPP $frmPPTB PropertyPalette::CurrentNode"
    ToolBars::SetButtonCommand $frmPPTB CollapseAll "RobLab::CollapseAllNTree $cvsPP $frmPPTB PropertyPalette::CurrentNode"
  }

}
