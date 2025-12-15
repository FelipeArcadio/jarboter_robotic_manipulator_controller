# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : TkNTree.tcl
#  Date         : 2001-11-09
#  Description  : Implementation of Tk n-ary trees (Graphic NTree)
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  TkNTree.tcl
#		This file implements the Tk n-ary trees (Graphic NTree).
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

source ./Tcl/NTree.tcl

# --------------------------------------------------------------------------------------------------------------------------

namespace eval TkNTree {

  # Procedures :
  # proc SetTitle { NTree Title }
  # proc GetTitle { NTree }
  # proc SetState { NTree State }
  # proc GetState { NTree }
  # proc SetIcon { NTree Icon }
  # proc GetIcon { NTree }
  # proc SetInsertAllowed { NTree InsertAllowed }
  # proc GetInsertAllowed { NTree }
  # proc SetDeleteAllowed { NTree DeleteAllowed }
  # proc GetDeleteAllowed { NTree }
  # proc SetUpdateAllowed { NTree UpdateAllowed }
  # proc GetUpdateAllowed { NTree }
  # proc SetData { NTree Data }
  # proc GetData { NTree }
  # proc Set { Title State Icon InsertAllowed DeleteAllowed UpdateAllowed { Data {} } }
  # proc AddGraphicNTreeLines { Canvas InitialNode NTree { Level 2 } }
  # proc GetTagNode { Canvas Id }
  # proc GetTagObjectType { Canvas Id }
  # proc GetTagObjectTypeNode { Canvas Id }
  # proc SetGraphicTitle { Canvas Node Title }
  # proc TurnGraphicNodeAllowed { Canvas CurrentNode Node }
  # proc SeeCanvasNode { Canvas Node }
  # proc SetCanvasScrollRegion { Canvas }
  # proc AddGraphicNode { Canvas Node X Y }
  # proc DeleteGraphicNode { Canvas Node }
  # proc MoveGraphicNode { Canvas Node Items }
  # proc MoveGraphicNTree { Canvas NTree Items }
  # proc MoveAllGraphicNTree { Canvas NTree Items }
  # proc Expand { Canvas NTree { Level 0 } { X 0 } }
  # proc Collapse { Canvas NTree { Level 0 } }
  # proc ExpandNTree { Canvas NTree }
  # proc CollapseNTree { Canvas NTree }
  # proc ChangeNTreeState { NTree NewState }
  # proc ChangeGraphicNodeState { Canvas NTree NewState }
  # proc ExpandAllNTree { Canvas NTree }
  # proc CollapseAllNTree { Canvas NTree }
  # proc MakeVisible { Canvas NTree }
  # proc SwapSelectedObject { Canvas OldNode NewNode }
  # proc LeftClickOnStateObject { Canvas OldNode NewNode }
  # proc LeftClickOnIconObject { Canvas OldNode NewNode }
  # proc LeftClickOnTitleObject { Canvas OldNode NewNode }
  # proc InitializeSelection { Canvas NTree }
  # proc GetNumberDisplayedElements { Canvas }
  # proc GetNextGraphicNodeUpRecursive { Node }
  # proc GetNextGraphicNodeUp { CurrentNode }
  # proc GetNextGraphicNodeDownRecursive { Node }
  # proc GetNextGraphicNodeDown { CurrentNode }
  # proc MoveCurrentNode { Keysym cvsCanvas CurrentNode Command }
  # proc Canvas_Bind_All_Button1 { cvsCanvas CurrentNode Command }
  # proc Bind_Canvas_Configure { cvsCanvas CurrentNode }
  # proc AddGraphicNTree { cvsCanvas NTreeFather NTreeSon }
  # proc DeleteGraphicNTree { cvsCanvas CurrentNode NTree }

  # Initialization of global variables
  variable StateCollapse 0
  variable StateExpand 1
  variable StateNone 2
  variable False 0
  variable True 1
  variable Undefined 2
  variable TemporaryX 0
  variable TemporaryY 0
  variable NodeTextColorGreen #000950000
  variable NodeTextColorBlack #000000000
  variable NodeTextColorGray #950950950

  # SetTitle procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetTitle { NTree Title } {
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 0 0 $Title ]
  }

  # GetTitle procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetTitle { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 0 ]
  }

  # SetState procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetState { NTree State } {
    variable StateCollapse
    variable StateExpand
    variable StateNone
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 1 1 [ expr $$State ] ]
  }

  # GetState procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetState { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 1 ]
  }

  # SetIcon procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetIcon { NTree Icon } {
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 2 2 $Icon ]
  }

  # GetIcon procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetIcon { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 2 ]
  }

  # SetInsertAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetInsertAllowed { NTree InsertAllowed } {
    variable False
    variable True
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 3 3 [ expr $$InsertAllowed ] ]
  }

 # GetInsertAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetInsertAllowed { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 3 ]
  }

  # SetDeleteAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetDeleteAllowed { NTree DeleteAllowed } {
    variable False
    variable True
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 4 4 [ expr $$DeleteAllowed ] ]
  }

  # GetDeleteAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetDeleteAllowed { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 4 ]
  }

  # SetUpdateAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetUpdateAllowed { NTree UpdateAllowed } {
    variable False
    variable True
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 5 5 [ expr $$UpdateAllowed ] ]
  }

  # GetUpdateAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetUpdateAllowed { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 5 ]
  }

  # SetData procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetData { NTree Data } {
    NTree::SetData $NTree [ lreplace [ NTree::GetData $NTree ] 6 6 $Data ]
  }

  # GetData procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetData { NTree } {
    return [ lindex [ NTree::GetData $NTree ] 6 ]
  }

  # Set procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Set { Title State Icon InsertAllowed DeleteAllowed UpdateAllowed { Data {} } } {
    variable StateCollapse
    variable StateExpand
    variable StateNone
    variable False
    variable True
    variable Undefined
    # <0.Title> <1.State> <2.Icon> <3.InsertAllowed> <4.DeleteAllowed> <5.UpdateAllowed> <6.Data>
    # 0.Title: Node's name.
    # 1.State: Node's visualization state.
    #		Possible values:
    #			0 StateCollapse: its sons are not displayed.
    #			1 StateExpand:   its sons are displayed.
    #     		2 StateNone:     node without sons.
    # 2.Icon:  Node's graphic image.
    # 3.InsertAllowed: It indicates whether the insertion of data is allowed or not (0 1 2).
    # 4.DeleteAllowed: It indicates if data can be deleted or not (0 1 2).
    # 5.UpdateAllowed: It indicates if data can be updated or not (0 1 2).
    # 			0 False
    # 			1 True
    # 			2 Undefined
    # 6.Data: It stores the node's graphic data. For this application the ObjectNavigator
    # 	      node data is stored.
    return [ NTree::Set [ list $Title [ expr $$State ] $Icon [ expr $$InsertAllowed ] [ expr $$DeleteAllowed ] [ expr $$UpdateAllowed ] $Data ] ]
  }

  # AddGraphicNTreeLines procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddGraphicNTreeLines { Canvas InitialNode NTree { Level 2 } } {
    if { ! [ NTree::Empty [ NTree::GetFather $NTree ] ] } {
      if { ! [ NTree::Empty [ NTree::GetNextBrother [ NTree::GetFather $NTree ] ] ] } {
        set X [ lindex [ $Canvas coords State$InitialNode ] 0 ]
        set Y [ lindex [ $Canvas coords State$InitialNode ] 1 ]
        $Canvas create image [ expr $X - ( 17 * $Level ) ] $Y -anchor nw -image imgLine0 -tags "$InitialNode Line Line$InitialNode"
      }
      AddGraphicNTreeLines $Canvas $InitialNode [ NTree::GetFather $NTree ] [ expr $Level + 1 ]
    }
  }

  # GetTagNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetTagNode { Canvas Id } {
    return [ lindex [ $Canvas gettags $Id ] 0 ]
  }

  # GetTagObjectType procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetTagObjectType { Canvas Id } {
    return [ lindex [ $Canvas gettags $Id ] 1 ]
  }

  # GetTagObjectTypeNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetTagObjectTypeNode { Canvas Id } {
    return [ lindex [ $Canvas gettags $Id ] 2 ]
  }

  # SetGraphicTitle procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetGraphicTitle { Canvas Node Title } { 
    $Canvas itemconfigure Title$Node -text $Title
  }

  # TurnGraphicNodeAllowed procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc TurnGraphicNodeAllowed { Canvas CurrentNode Node } { 
    variable True
    variable False
    variable NodeTextColorBlack
    variable NodeTextColorGray
    set NodeTextColor $NodeTextColorBlack
    if { [ GetUpdateAllowed $Node ] == $True } {
      set FontName TkNTree_fntHelveticaNormal
    } elseif { [ GetUpdateAllowed $Node ] == $False } {
      set NodeTextColor $NodeTextColorGray
      set FontName TkNTree_fntHelveticaItalic
    } else {
      set FontName TkNTree_fntHelveticaBold
    }    
    $Canvas itemconfigure Title$Node -font $FontName
    if { $CurrentNode != $Node } {
      $Canvas itemconfigure Title$Node -fill $NodeTextColor
    }
  }

  # SeeCanvasNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SeeCanvasNode { Canvas Node } {
    set x1 0
    set y1 0
    set x2 0
    set y2 0
    scan [ $Canvas bbox Title$Node ] "%i %i %i %i" x1 y1 x2 y2
    scan [ $Canvas xview ] "%f %f" Left Right
    scan [ $Canvas yview ] "%f %f" Top Bottom
    scan [ $Canvas cget -scrollregion ] "%i %i %i %i" xMin yMin xMax yMax
    set xPos [ expr { ( ( ( $x2 + $x1 ) / 2.0 ) / $xMax ) - ( ( $Right - $Left ) / 2.0 ) } ]
    set yPos [ expr { ( ( ( $y2 + $y1 ) / 2.0 ) / $yMax ) - ( ( $Bottom - $Top ) / 2.0 ) } ]
    $Canvas xview moveto $xPos
    $Canvas yview moveto $yPos
  }

  # SetCanvasScrollRegion procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SetCanvasScrollRegion { Canvas } {
    set Width [ winfo width $Canvas ]
    set Height [ winfo height $Canvas ]
    set x1 0
    set y1 0
    set x2 0
    set y2 0
    scan [ $Canvas bbox all ] "%i %i %i %i" x1 y1 x2 y2
    if { $x2 < $Width } {
      set x2 $Width
    }
    if { $y2 < $Height } {
      set y2 $Height
    }
    $Canvas configure -scrollregion [ list $x1 $y1 [ expr $x2 + 51 ] $y2 ]
  }

  # AddGraphicNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddGraphicNode { Canvas Node X Y } { 
    variable True
    variable False
    variable NodeTextColorBlack
    variable NodeTextColorGray
    # Tags <0.Node> <1.Object Type> <2.Object Type + NTree Node>
    $Canvas create image $X $Y -anchor nw -image imgState[ GetState $Node ] -tags "$Node State State$Node"
    $Canvas create image [ expr $X + 17 ] $Y -anchor nw -image imgCVSIcon[ GetIcon $Node ] -tags "$Node Icon Icon$Node"
    if { $::tcl_platform(platform) == "macintosh" } {
      set DeltaY 0
    } elseif { $::tcl_platform(platform) == "windows" } {
      set DeltaY 0
    } else {
      set DeltaY 2
    }
    set NodeTextColor $NodeTextColorBlack
    if { [ GetUpdateAllowed $Node ] == $True } {
      set FontName TkNTree_fntHelveticaNormal
    } elseif { [ GetUpdateAllowed $Node ] == $False } {
      set NodeTextColor $NodeTextColorGray
      set FontName TkNTree_fntHelveticaItalic
    } else {
      set FontName TkNTree_fntHelveticaBold
    }
    $Canvas create text [ expr $X + 40 ] [ expr $Y + $DeltaY ] -anchor nw -text [ GetTitle $Node ] -fill $NodeTextColor -font $FontName -tags "$Node Title Title$Node"
    if { ! [ NTree::Empty [ NTree::GetFather $Node ] ] } {
      if { ! [ NTree::Empty [ NTree::GetNextBrother $Node ] ] } {
        $Canvas create image [ expr $X - 17 ] $Y -anchor nw -image imgLine1 -tags "$Node Line Line$Node"
      } else {
        $Canvas create image [ expr $X - 17 ] $Y -anchor nw -image imgLine2 -tags "$Node Line Line$Node"
      }
    }
    AddGraphicNTreeLines $Canvas $Node $Node
    SetCanvasScrollRegion $Canvas
  }

  # DeleteGraphicNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteGraphicNode { Canvas Node } {
    $Canvas delete $Node
    SetCanvasScrollRegion $Canvas
  }

  # MoveGraphicNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc MoveGraphicNode { Canvas Node Items } {
    $Canvas move $Node 0 [ expr $Items * 17 ]
  }

  # MoveGraphicNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc MoveGraphicNTree { Canvas NTree Items } {
    variable StateExpand
    if { ! [ NTree::Empty $NTree ] } {
      MoveGraphicNode $Canvas $NTree $Items
      if { [ GetState $NTree ] == $StateExpand } {
        set TemporaryNTree [ NTree::GetFirstSon $NTree ]
        while { ! [ NTree::Empty $TemporaryNTree ] } {
          MoveGraphicNTree $Canvas $TemporaryNTree $Items
          set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
        }
      }
    } 
  }

  # MoveAllGraphicNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc MoveAllGraphicNTree { Canvas NTree Items } {
    if { ! [ NTree::Empty [ NTree::GetFather $NTree ] ] } {
      set TemporaryNTree [ NTree::GetNextBrother $NTree ]
      while { ! [ NTree::Empty $TemporaryNTree ] } {
        MoveGraphicNTree $Canvas $TemporaryNTree $Items
        set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
      }
      MoveAllGraphicNTree $Canvas [ NTree::GetFather $NTree ] $Items
    }
  }

  # Expand procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Expand { Canvas NTree { Level 0 } { X 0 } } {
    variable StateExpand
    variable TemporaryX
    variable TemporaryY
    if { ! [ NTree::Empty $NTree ] } {
      if { $Level == 0 } {
        set TemporaryX [ lindex [ $Canvas coords State$NTree ] 0 ]
        set TemporaryY [ lindex [ $Canvas coords State$NTree ] 1 ]
        set vWeight -1
      } else {
        set vWeight 0
        AddGraphicNode $Canvas $NTree $X $TemporaryY
      }
      if { [ GetState $NTree ] == $StateExpand } {
        set TemporaryNTree [ NTree::GetFirstSon $NTree ]
        set X [ expr $TemporaryX + ( ( $Level + 1 ) * 17 ) ]
        while { ! [ NTree::Empty $TemporaryNTree ] } {
          set TemporaryY [ expr $TemporaryY + 17 ]
          set vWeight [ expr $vWeight + [ Expand $Canvas $TemporaryNTree [ expr $Level + 1 ] $X ] ]
          set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
        }
      }
      return [ expr $vWeight + 1 ]
    } else {
      return 0
    }
  }

  # Collapse procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Collapse { Canvas NTree { Level 0 } } {
    variable StateExpand
    if { ! [ NTree::Empty $NTree ] } {
      if { $Level == 0 } {
        set vWeight -1
      } else {
        set vWeight 0
        DeleteGraphicNode $Canvas $NTree
      }
      if { [ GetState $NTree ] == $StateExpand } {
        set TemporaryNTree [ NTree::GetFirstSon $NTree ]
        while { ! [ NTree::Empty $TemporaryNTree ] } {
          set vWeight [ expr $vWeight + [ Collapse $Canvas $TemporaryNTree [ expr $Level + 1 ] ] ]
          set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
        }
      }
      return [ expr $vWeight + 1 ]
    } else {
      return 0
    }
  }

  # ExpandNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ExpandNTree { Canvas NTree } {
    variable StateCollapse
    variable StateExpand
    if { [ GetState $NTree ] == $StateCollapse } {
      SetState $NTree StateExpand
      $Canvas itemconfigure State$NTree -image imgState[ expr $StateExpand ]
      set vWeight [ Expand $Canvas $NTree ]
      MoveAllGraphicNTree $Canvas $NTree $vWeight
    }
  }

  # CollapseNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CollapseNTree { Canvas NTree } {
    variable StateCollapse
    variable StateExpand
    if { [ GetState $NTree ] == $StateExpand } {
      set vWeight [ Collapse $Canvas $NTree ]
      SetState $NTree StateCollapse
      $Canvas itemconfigure State$NTree -image imgState[ expr $StateCollapse ]
      MoveAllGraphicNTree $Canvas $NTree -$vWeight
    }
  }

  # ChangeNTreeState procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeNTreeState { NTree NewState } {
    variable StateNone
    if { ! [ NTree::Empty $NTree ] } {
      if { [ GetState $NTree ] != $StateNone } {
        SetState $NTree $NewState
      }
      set TemporaryNTree [ NTree::GetFirstSon $NTree ]
      while { ! [ NTree::Empty $TemporaryNTree ] } {
        ChangeNTreeState $TemporaryNTree $NewState
        set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
      }
    }
  }

  # ChangeGraphicNodeState procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeGraphicNodeState { Canvas NTree NewState } {
    variable StateCollapse
    variable StateExpand
    variable StateNone
    $Canvas itemconfigure State$NTree -image imgState[ expr $$NewState ]
  }

  # ChangeGraphicIcon procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeGraphicIcon { Canvas NTree NewIcon } {
    #$Canvas itemconfigure Title$Node -text $Title
    $Canvas itemconfigure Icon$NTree -image imgCVSIcon[ expr $NewIcon ]
    #$Canvas SetIcon $NTree $NewIcon
  }

  # ExpandAllNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ExpandAllNTree { Canvas NTree } {
    CollapseNTree $Canvas $NTree
    ChangeNTreeState $NTree StateExpand
    SetState $NTree StateCollapse
    ExpandNTree $Canvas $NTree
  }

  # CollapseAllNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CollapseAllNTree { Canvas NTree } {
    CollapseNTree $Canvas $NTree
    ChangeNTreeState $NTree StateCollapse
  }

  # MakeVisible procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc MakeVisible { Canvas NTree } {
    variable StateCollapse
    if { [ NTree::Empty $NTree ] } {
      return $NTree::Null
    }
    MakeVisible $Canvas [ NTree::GetFather $NTree ]
    set State [ GetState $NTree ]
    if { $State == $StateCollapse } {
      ExpandNTree $Canvas $NTree
    }
  }

  # SwapSelectedObject procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SwapSelectedObject { Canvas OldNode NewNode } {
    variable False
    variable NodeTextColorGreen
    variable NodeTextColorBlack
    variable NodeTextColorGray
    MakeVisible $Canvas [ NTree::GetFather $NewNode ]
    set OldX [ lindex [ $Canvas coords Title$OldNode ] 0 ]
    set OldY [ lindex [ $Canvas coords Title$OldNode ] 1 ]
    set NewX [ lindex [ $Canvas coords Title$NewNode ] 0 ]
    set NewY [ lindex [ $Canvas coords Title$NewNode ] 1 ]
    if { $OldNode != $NewNode } {
      set NodeTextColor $NodeTextColorBlack
      if { [ GetUpdateAllowed $OldNode ] == $False } {
        set NodeTextColor $NodeTextColorGray
      }
      $Canvas itemconfigure Title$OldNode -fill $NodeTextColor
      $Canvas itemconfigure Title$NewNode -fill $NodeTextColorGreen
      $Canvas move Selection [ expr $NewX - $OldX ] [ expr $NewY - $OldY ]
      SetCanvasScrollRegion $Canvas
      # Commands to execute each time a node is exchanged for any reason as a click, a key press or a swap
      # This must be deleted when ObjectNavigator namespace is not present
      # Next is used for selection purposes
      if { $Canvas == $ObjectNavigator::cvsON } {
        $DisplayWindow::ToglWidget toglSetToglDisplay toglSetCurrentONNode $NewNode
      }
      # Other stuff
      #MessagesWindow::Puts "NewNode = $NewNode\n"
    }
  }

  # LeftClickOnStateObject procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc LeftClickOnStateObject { Canvas OldNode NewNode } {
    variable StateCollapse
    variable StateExpand
    SwapSelectedObject $Canvas $OldNode $NewNode
    set State [ GetState $NewNode ]
    if { $State == $StateCollapse } {
      ExpandNTree $Canvas $NewNode
    } else {
      if { $State == $StateExpand } {
        CollapseNTree $Canvas $NewNode
      }
    }
  }

  # LeftClickOnIconObject procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc LeftClickOnIconObject { Canvas OldNode NewNode } {
    SwapSelectedObject $Canvas $OldNode $NewNode
  }

  # LeftClickOnTitleObject procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc LeftClickOnTitleObject { Canvas OldNode NewNode } {
    SwapSelectedObject $Canvas $OldNode $NewNode
  }

  # InitializeSelection procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc InitializeSelection { Canvas NTree } {
    variable NodeTextColorGreen
    set X [ lindex [ $Canvas coords Title$NTree ] 0 ]
    set Y [ lindex [ $Canvas coords Title$NTree ] 1 ]
    if { $::tcl_platform(platform) == "macintosh" } {
      set DeltaY 0
    } elseif { $::tcl_platform(platform) == "windows" } {
      set DeltaY 0
    } else {
      set DeltaY -2
    }
    $Canvas create image [ expr $X - 4 ] [ expr $Y + $DeltaY ] -anchor nw -image imgSelection -tags "Selection"
    $Canvas itemconfigure Title$NTree -fill $NodeTextColorGreen
    $Canvas lower Selection $NTree
  }

  # GetNumberDisplayedElements procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNumberDisplayedElements { Canvas } {
    set Count 0
    foreach Element [ $Canvas find withtag Icon ] {
      incr Count
    }
    return $Count
  }

  # GetNextGraphicNodeUpRecursive procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNextGraphicNodeUpRecursive { Node } {
    variable StateExpand
    if { [ GetState $Node ] == $StateExpand } {
      return [ GetNextGraphicNodeUpRecursive [ NTree::GetLastSon $Node ] ]
    } else {
      return $Node
    }
  }

  # GetNextGraphicNodeUp procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNextGraphicNodeUp { CurrentNode } {
    variable StateExpand
    set PreviousBrother [ NTree::GetPreviousBrother $CurrentNode ]
    if { ! [ NTree::Empty $PreviousBrother ] } {
      if { [ GetState $PreviousBrother ] == $StateExpand } {
        return [ GetNextGraphicNodeUpRecursive [ NTree::GetLastSon $PreviousBrother ] ]
      } else {
        return $PreviousBrother
      }
    } else {
      set Father [ NTree::GetFather $CurrentNode ]
      if { ! [ NTree::Empty $Father ] } {
        return $Father
      } else {
        return ""
      }
    }
  }

  # GetNextGraphicNodeDownRecursive procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNextGraphicNodeDownRecursive { Node } {
    set Father [ NTree::GetFather $Node ]
    if { ! [ NTree::Empty $Father ] } {
      set NextBrother [ NTree::GetNextBrother $Father ]
      if { ! [ NTree::Empty $NextBrother ] } {
        return $NextBrother
      } else {
        GetNextGraphicNodeDownRecursive $Father
      }
    } else {
      return ""
    }
  }

  # GetNextGraphicNodeDown procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc GetNextGraphicNodeDown { CurrentNode } {
    variable StateExpand
    if { [ GetState $CurrentNode ] == $StateExpand } {
      return [ NTree::GetFirstSon $CurrentNode ]
    } else {
      set NextBrother [ NTree::GetNextBrother $CurrentNode ]
      if { ! [ NTree::Empty $NextBrother ] } {
        return $NextBrother
      } else {
        return [ GetNextGraphicNodeDownRecursive $CurrentNode ]
      }
    }
  }

  # MoveCurrentNode procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc MoveCurrentNode { Keysym cvsCanvas CurrentNode Command } {
    switch $Keysym {
      Prior  {
               set NewNode [ GetNextGraphicNodeUp [ expr $$CurrentNode ] ]
               if { $NewNode != "" } {
                 SwapSelectedObject $cvsCanvas [ expr $$CurrentNode ] $NewNode
                 set $CurrentNode $NewNode
                 eval $Command
                 tkwait visibility $cvsCanvas
                 SeeCanvasNode $cvsCanvas [ expr $$CurrentNode ]
               }
             }
      Next   {
               set NewNode [ GetNextGraphicNodeDown [ expr $$CurrentNode ] ]
               if { $NewNode != "" } {
                 SwapSelectedObject $cvsCanvas [ expr $$CurrentNode ] $NewNode
                 set $CurrentNode $NewNode
                 eval $Command
                 tkwait visibility $cvsCanvas
                 SeeCanvasNode $cvsCanvas [ expr $$CurrentNode ]
               }
             }
    }
  }

  # Canvas_Bind_All_Button1 procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Canvas_Bind_All_Button1 { cvsCanvas CurrentNode Command } {
    set Node [ GetTagNode $cvsCanvas current ]
    set ObjectType [ GetTagObjectType $cvsCanvas current ]
    switch $ObjectType {
      State { 
              LeftClickOnStateObject $cvsCanvas [ expr $$CurrentNode ] $Node
              set $CurrentNode $Node
            }
      Icon  { 
              LeftClickOnIconObject $cvsCanvas [ expr $$CurrentNode ] $Node
              set $CurrentNode $Node
            }
      Title { 
              LeftClickOnTitleObject $cvsCanvas [ expr $$CurrentNode ] $Node
              set $CurrentNode $Node
            }
    }
    eval $Command
    SetCanvasScrollRegion $cvsCanvas
  }

  # Bind_Canvas_Configure procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Bind_Canvas_Configure { cvsCanvas CurrentNode } {
    SetCanvasScrollRegion $cvsCanvas
    if { ! [ NTree::Empty [ expr $$CurrentNode ] ] } {
      SeeCanvasNode $cvsCanvas [ expr $$CurrentNode ]
    }
  }

  # AddGraphicNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AddGraphicNTree { cvsCanvas NTreeFather NTreeSon } {
    variable StateCollapse
    variable StateExpand
    if { [ GetState $NTreeFather ] != $StateCollapse } {
      SetState $NTreeFather StateExpand
      CollapseNTree $cvsCanvas $NTreeFather
    }
    NTree::Add $NTreeFather $NTreeSon
    ExpandNTree $cvsCanvas $NTreeFather
  }

  # InsertGraphicNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc InsertGraphicNTree { cvsCanvas NTreeFather NTreeSon } {
    variable StateCollapse
    variable StateExpand
    if { [ GetState $NTreeFather ] != $StateCollapse } {
      SetState $NTreeFather StateExpand
      CollapseNTree $cvsCanvas $NTreeFather
    }
    NTree::Insert $NTreeFather $NTreeSon
    ExpandNTree $cvsCanvas $NTreeFather
  }

  # DeleteGraphicNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteGraphicNTree { cvsCanvas CurrentNode NTree Command } {
    set NTreeFather [ NTree::GetFather $NTree ]
    if { [ expr $$CurrentNode ] == $NTree } {
      SwapSelectedObject $cvsCanvas $NTree $NTreeFather
      set $CurrentNode $NTreeFather
    }
    CollapseNTree $cvsCanvas $NTreeFather
    NTree::DisAssociate $NTree
    NTree::Destroy $NTree
    if { [ NTree::GetNumberSons $NTreeFather ] != 0 } {
      ExpandNTree $cvsCanvas $NTreeFather
    } else {
      SetState $NTreeFather StateNone
      ChangeGraphicNodeState $cvsCanvas $NTreeFather StateNone
    }
    eval $Command
  }

  # DeleteCtrlGraphicNTree procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc DeleteCtrlGraphicNTree { cvsCanvas CurrentNode NTree Command } {
    set NTreeFather [ NTree::GetFather $NTree ]
    CollapseNTree $cvsCanvas $NTreeFather
    NTree::DisAssociate $NTree
    NTree::Destroy $NTree
    ExpandNTree $cvsCanvas $NTreeFather
    RobLab::ExpandNTree $PropertyPalette::cvsPP $PropertyPalette::frmPPTB PropertyPalette::CurrentNode
    eval $Command
  }

  # IsCommand procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc IsCommand { Node } {
    variable False
    variable True
    if { [ RobLab::IsInteger [ string range $Node 4 end ] ] == 0 } {
      return $False
    } elseif { [ info commands $Node ] == "" } {
      return $False
    }
    return True
  }

}

# Initialization of Tk objects

  # Initialization of fonts
  font create TkNTree_fntHelveticaBold -family Helvetica -size $Global_FontSize -weight bold -slant roman
  font create TkNTree_fntHelveticaNormal -family Helvetica -size $Global_FontSize -weight normal -slant roman
  font create TkNTree_fntHelveticaItalic -family Helvetica -size $Global_FontSize -weight normal -slant italic

  # Initialization of images, all images should be 17x17 pixels
  image create photo imgLine0 -file ./Images/TkNTree/Line0.gif
  image create photo imgLine1 -file ./Images/TkNTree/Line1.gif
  image create photo imgLine2 -file ./Images/TkNTree/Line2.gif
  image create photo imgState$TkNTree::StateCollapse -file ./Images/TkNTree/StateCollapse.gif
  image create photo imgState$TkNTree::StateExpand -file ./Images/TkNTree/StateExpand.gif
  image create photo imgState$TkNTree::StateNone -file ./Images/TkNTree/StateNone.gif
  image create photo imgSelection -file ./Images/TkNTree/Selection.gif

