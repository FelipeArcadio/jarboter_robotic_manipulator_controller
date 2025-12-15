# --------------------------------------------------------------------------------------------------------------------------
#  File Name    : SocketsWindow.tcl
#  Date         : 2002-04-07
#  Description  : Implementation of sockets window
#  Requirements :
#  Copyright    : Wilber Perea Castro wpcmmx@hotmail.com
#                 Antonio Alejandro Matta Gomez, amatta@atlas.puj.edu.co
# --------------------------------------------------------------------------------------------------------------------------

# --------------------------------------------------------------------------------------------------------------------------
#  SocketsWindow.tcl
#		This file implements the ROBOMOSP sockets window.
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

  # Images of object navigator canvas
  image create photo  imgCVSIcon25 -file ./Images/SocketsWindow/Icon25.gif
  image create photo  imgCVSIcon26 -file ./Images/SocketsWindow/Icon26.gif
  image create photo  imgCVSIcon27 -file ./Images/SocketsWindow/Icon27.gif
  image create photo  imgCVSIcon28 -file ./Images/SocketsWindow/Icon28.gif

  # Images of object navigator tool bar
  image create photo imgSKTBOpen -file ./Images/SocketsWindowToolBar/Open.gif
  image create photo imgSKTBClose -file ./Images/SocketsWindowToolBar/Close.gif
  image create photo imgSKTBDisconnect -file ./Images/SocketsWindowToolBar/Disconnect.gif

namespace eval SocketsWindow {

  # Procedures :
  # proc SocketInfo { SocketId Type }
  # proc SearchSocketNTree { ClientSocketId }
  # proc ReceiveMessage { ClientSocketId }
  # proc AcceptConnections { ClientSocketId ClientIP ClientPort }
  # proc CommandOpenPort {}
  # proc CommandClosePort {}
  # proc CommandSetPort {}
  # proc CommandDisconnectClientConnection {}
  # proc ValidateToolBarButtons {}
  # proc Bind_entSK_Return {}
  # proc CreateClientsNode { ClientSocketId }
  # proc Initialize {}

  # Object Types :
  # Sockets
  # -- Port
  # -- Connections
  #   -- Client

  # Initialization of global variables
  variable tplSK .wndSocketsWindow
  variable cvsSK $tplSK.frmSK_A.cvsSocketsWindow
  variable entSK $tplSK.frmSK_A.frmSK_C.entPropertyValue
  variable frmSKTB $tplSK.frmToolBar
  variable CurrentNode $NTree::Null
  variable PortNode $NTree::Null
  variable ConnectionsNode $NTree::Null
  variable SocketId
  variable ReservedMessages
  variable SKWindowInit 0

  # SocketInfo  procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SocketInfo { SocketId Type } {
    set Info [ fconfigure $SocketId -$Type ]
    return "( Host:[ lindex $Info 1 ], IP:[ lindex $Info 0 ], Port:[ lindex $Info 2 ] )"
  }

  #SearchSocketNTree   procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc SearchSocketNTree { ClientSocketId } {
    variable ConnectionsNode
    set TemporaryNTree [ NTree::GetFirstSon $ConnectionsNode ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      set TemporaryClientSocketId [ ObjectNavigator::GetFileName $TemporaryNTree ]
      if { $ClientSocketId == $TemporaryClientSocketId } {
        return $TemporaryNTree
      }
      set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
    }
    return $NTree::Null
  }

  #ReceiveMessage   procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ReceiveMessage { ClientSocketId } {
    variable cvsSK
    variable ReservedMessages
    set Message [ read $ClientSocketId ]
    if { $Message == $ReservedMessages(EOC) } {
      MessagesWindow::Puts "The [ SocketInfo $ClientSocketId peername ] client has been disconnected.\n\n"
      close $ClientSocketId
      # Delete graphic node
      set NTreeToDelete [ SearchSocketNTree $ClientSocketId ]
      TkNTree::DeleteGraphicNTree $cvsSK SocketsWindow::CurrentNode $NTreeToDelete SocketsWindow::ValidateToolBarButtons
    } else {
      set Error [ catch {set Result [ uplevel #0 $Message ]} ErrorInfo ]
      if { $Error != 0 } {
        puts -nonewline $ClientSocketId $ErrorInfo
      } else {
        if { $Result != "" } {
          puts -nonewline $ClientSocketId $Result
        } else {
          puts -nonewline $ClientSocketId $ReservedMessages(ER)
        }
      }
      flush $ClientSocketId
    }
  }

  #AcceptConnections procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc AcceptConnections { ClientSocketId ClientIP ClientPort } {
    variable cvsSK
    variable ConnectionsNode
    fconfigure $ClientSocketId -blocking false
    fileevent $ClientSocketId readable "SocketsWindow::ReceiveMessage $ClientSocketId"
    MessagesWindow::Puts "The [ SocketInfo $ClientSocketId peername ] client has been connected.\n\n"
    TkNTree::AddGraphicNTree $cvsSK $ConnectionsNode [ CreateClientsNode $ClientSocketId ]
  }

  #CommandOpenPort   procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandOpenPort {} {
    variable entSK
    variable SocketId
    set Error [ catch "set SocketId [ socket -server { SocketsWindow::AcceptConnections } [ $entSK get ] ]" ErrorInfo ]
    if { $Error == 0 } {
      MessagesWindow::Puts "Connection opened at [ SocketInfo $SocketId sockname ].\n\n"
    } else {
      MessagesWindow::Puts "Error : $ErrorInfo.\n\n"
    }
  }

  #CommandClosePort procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandClosePort {} {
    variable cvsSK
    variable CurrentNode
    variable ConnectionsNode
    variable SocketId
    variable ReservedMessages
    set TemporaryNTree [ NTree::GetFirstSon $ConnectionsNode ]
    while { ! [ NTree::Empty $TemporaryNTree ] } {
      set ClientSocketId [ ObjectNavigator::GetFileName $TemporaryNTree ]
      puts -nonewline $ClientSocketId $ReservedMessages(EOC)
      flush $ClientSocketId
      close $ClientSocketId
      set NTreeToDelete $TemporaryNTree
      set TemporaryNTree [ NTree::GetNextBrother $TemporaryNTree ]
      TkNTree::DeleteGraphicNTree $cvsSK SocketsWindow::CurrentNode $NTreeToDelete SocketsWindow::ValidateToolBarButtons
    }
    MessagesWindow::Puts "Connection closed at [ SocketInfo $SocketId sockname ].\n\n"    
    close $SocketId
  }

  #CommandSetPort procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandSetPort {} {
    variable cvsSK
    variable entSK
    variable frmSKTB
    variable CurrentNode
    variable PortNode
    set ToolTip [ ToolBars::GetButtonToolTip $frmSKTB Port ]
    if { $ToolTip == "RClose" } {
      ToolBars::SetButtonToolTip $frmSKTB Port "ROpen"
      ToolBars::SetButtonImage $frmSKTB Port imgSKTBOpen
      ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmSKTB Port ] "ROpen"
      TkNTree::SetUpdateAllowed $PortNode True
      if { $CurrentNode == $PortNode } {
        pack $entSK -side top -fill x
      }
      CommandClosePort
    } else {
      Bind_entSK_Return
      set PropertyValue [ $entSK get ]
      if { $PropertyValue != "" } {
        ToolBars::SetButtonToolTip $frmSKTB Port "RClose"
        ToolBars::SetButtonImage $frmSKTB Port imgSKTBClose
        ToolTip::SetToolTipText [ ToolBars::GetButtonWidget $frmSKTB Port ] "RClose"
        TkNTree::SetUpdateAllowed $PortNode False
        if { $CurrentNode == $PortNode } {
          pack forget $entSK
        }
        CommandOpenPort
      } else {
        MessagesWindow::Puts "Error : Can not open a null port.\n\n"
      }
    }
    TkNTree::TurnGraphicNodeAllowed $cvsSK $CurrentNode $PortNode
  }

  #CommandDisconnectClientConnection --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CommandDisconnectClientConnection {} {
    variable cvsSK
    variable CurrentNode
    variable ReservedMessages
    set ClientSocketId [ ObjectNavigator::GetFileName $CurrentNode ]
    puts -nonewline $ClientSocketId $ReservedMessages(EOC)
    flush $ClientSocketId
    close $ClientSocketId
    TkNTree::DeleteGraphicNTree $cvsSK SocketsWindow::CurrentNode $CurrentNode SocketsWindow::ValidateToolBarButtons
  }

  #ValidateToolBarButtons --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ValidateToolBarButtons {} {
    variable cvsSK
    variable entSK
    variable frmSKTB
    variable CurrentNode
    if { ! [ NTree::Empty $CurrentNode ] } {
      set CurrentNodeState [ TkNTree::GetState $CurrentNode ]
      if { $CurrentNodeState == $TkNTree::StateNone } {
        ToolBars::DisableButton $frmSKTB Expand
        ToolBars::DisableButton $frmSKTB Collapse
        ToolBars::DisableButton $frmSKTB ExpandAll
        ToolBars::DisableButton $frmSKTB CollapseAll
      } else {
        ToolBars::EnableButton $frmSKTB ExpandAll
        ToolBars::EnableButton $frmSKTB CollapseAll
        if { $CurrentNodeState == $TkNTree::StateExpand } {
          ToolBars::DisableButton $frmSKTB Expand
          ToolBars::EnableButton $frmSKTB Collapse
        } else {
          ToolBars::EnableButton $frmSKTB Expand
          ToolBars::DisableButton $frmSKTB Collapse
        }
      }
      TkNTree::SeeCanvasNode $cvsSK $CurrentNode
    } else {
      ToolBars::DisableButton $frmSKTB Expand
      ToolBars::DisableButton $frmSKTB Collapse
      ToolBars::DisableButton $frmSKTB ExpandAll
      ToolBars::DisableButton $frmSKTB CollapseAll
    }
    pack forget $entSK
    set ObjectType [ ObjectNavigator::GetObjectType $CurrentNode ]
    switch $ObjectType {
      Port                        {
                                    if { [ TkNTree::GetUpdateAllowed $CurrentNode ] == $TkNTree::True } {
                                      pack $entSK -side top -fill x
                                      focus $entSK
                                      $entSK selection range 0 end
                                    }
                                    ToolBars::DisableButton $frmSKTB Disconnect
                                  }
      Connections                 {
                                    ToolBars::DisableButton $frmSKTB Disconnect
                                  }
      Client                      {
                                    ToolBars::EnableButton $frmSKTB Disconnect
                                  }
    }
  }

  #Bind_entSK_Return  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Bind_entSK_Return {} {
    variable cvsSK
    variable entSK
    variable PortNode
    set PropertyValue [ $entSK get ]
    if { [ RobLab::IsInteger $PropertyValue ] == 0 } {
      MessagesWindow::Puts "Error : Can not set value, \"$PropertyValue\" is not a number.\n\n"
      set PropertyValue ""
      $entSK delete 0 end
    } else {
      if { ( $PropertyValue < 3000 ) || ( $PropertyValue > 9000 ) } {
        MessagesWindow::Puts "Error : Can not set value, \"$PropertyValue\" is not a number between 3000 and 9000.\n\n"
        set PropertyValue ""
        $entSK delete 0 end
      }
    }
    TkNTree::SetGraphicTitle $cvsSK $PortNode "Port = $PropertyValue"
  }

  #CreateClientsNode   --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CreateClientsNode { ClientSocketId } {
    set NodeNTreeData [ list Client $NTree::Null $ClientSocketId ]
    set TemporaryNTreeA [ TkNTree::Set "[ SocketInfo $ClientSocketId peername ]" StateNone 28 False False True $NodeNTreeData ]
    return $TemporaryNTreeA
  }

  #Initialize   --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplSK
    variable cvsSK
    variable entSK
    variable frmSKTB
    variable CurrentNode
    variable PortNode
    variable ConnectionsNode
    variable ReservedMessages
    global SKWindowInit
    set SKWindowInit 1
    # Initialize the reserved messages array
    # EOC = End Of Communication
    # ER  = Empty Result
    array set ReservedMessages {EOC "@$%&" ER "&%$@"}
    # Initialization of sockets window
    RobLab::CreateWindow "" wndSocketsWindow "Sockets" "$RobLab::SK_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { wm withdraw $SocketsWindow::tplSK } $RobLab::VirtualGridSize
    frame $frmSKTB -width 26
    frame $tplSK.frmSK_A
    frame $tplSK.frmSK_B -borderwidth 1 -width 13 -height 13
    frame $tplSK.frmSK_A.frmSK_C -borderwidth 1 -relief sunken -width 13 -height 17
    scrollbar $tplSK.frmSK_A.scbHorizontal -borderwidth 1 -orient horiz -command "$cvsSK xview"
    scrollbar $tplSK.scbVertical -borderwidth 1 -orient verti -command "$cvsSK yview"
    canvas $cvsSK -borderwidth 1 -relief sunken -width 17 -height 17 -background #fffffffff -xscrollcommand "$tplSK.frmSK_A.scbHorizontal set" -yscrollcommand "$tplSK.scbVertical set" -xscrollincrement 17 -yscrollincrement 17 -confine true -scrollregion { 0 0 0 0 } -highlightthickness 0
    entry $entSK -borderwidth 0 -background #fffffffff -foreground #000000000
    $entSK insert 0 "3000"
    pack $frmSKTB -side left -fill y
    pack $tplSK.frmSK_A -side left -fill both -expand true
    pack $tplSK.frmSK_A.scbHorizontal -side bottom -fill x
    pack $tplSK.frmSK_A.frmSK_C -side bottom -fill x -padx 2 -pady 1
    pack $cvsSK -side top -fill both -expand true
    pack $tplSK.frmSK_B -side bottom
    pack $tplSK.scbVertical -side left -fill y
    # Initialization of NTree
    set NodeNTreeData [ list Sockets $NTree::Null "" ]
    set ApplicationNTree [ TkNTree::Set "Sockets" StateCollapse 25 False False Undefined $NodeNTreeData ]
    # Port
    set NodeNTreeData [ list Port $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Port = 3000" StateNone 26 False False True $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set PortNode $TemporaryNTreeA
    # Connections
    set NodeNTreeData [ list Connections $NTree::Null "" ]
    set TemporaryNTreeA [ TkNTree::Set "Connections" StateNone 27 False False Undefined $NodeNTreeData ]
    NTree::Add $ApplicationNTree $TemporaryNTreeA
    set ConnectionsNode $TemporaryNTreeA
    set CurrentNode $ApplicationNTree
    TkNTree::AddGraphicNode $cvsSK $ApplicationNTree 0 0
    TkNTree::ExpandNTree $cvsSK $ApplicationNTree
    TkNTree::InitializeSelection $cvsSK $ApplicationNTree
    # Initialization of binds
    bind $tplSK <KeyPress> "TkNTree::MoveCurrentNode %K $cvsSK SocketsWindow::CurrentNode SocketsWindow::ValidateToolBarButtons"
    $cvsSK bind all <Button-1> "TkNTree::Canvas_Bind_All_Button1 $cvsSK SocketsWindow::CurrentNode SocketsWindow::ValidateToolBarButtons"
    bind $cvsSK <Configure> "TkNTree::Bind_Canvas_Configure $cvsSK SocketsWindow::CurrentNode"
    bind $entSK <Return> { SocketsWindow::Bind_entSK_Return }
    # Initialization of sockets window tool bar
    set SKToolBar  { {          Port              "ROpen"           imgSKTBOpen    normal  { SocketsWindow::CommandSetPort } }
                     {    Disconnect        "RDisconnect"     imgSKTBDisconnect  disabled  { SocketsWindow::CommandDisconnectClientConnection } }
                     { "" }
                     {        Expand            "RExpand"         imgONTBExpand  disabled  {} }
                     {      Collapse          "RCollapse"       imgONTBCollapse    normal  {} }
                     {     ExpandAll        "RExpand All"      imgONTBExpandAll    normal  {} }
                     {   CollapseAll      "RCollapse All"    imgONTBCollapseAll    normal  {} } }
    ToolBars::Create $frmSKTB $SKToolBar Vertical
    ToolBars::SetButtonCommand $frmSKTB      Expand "RobLab::ExpandNTree $cvsSK $frmSKTB SocketsWindow::CurrentNode"
    ToolBars::SetButtonCommand $frmSKTB    Collapse "RobLab::CollapseNTree $cvsSK $frmSKTB SocketsWindow::CurrentNode"
    ToolBars::SetButtonCommand $frmSKTB   ExpandAll "RobLab::ExpandAllNTree $cvsSK $frmSKTB SocketsWindow::CurrentNode"
    ToolBars::SetButtonCommand $frmSKTB CollapseAll "RobLab::CollapseAllNTree $cvsSK $frmSKTB SocketsWindow::CurrentNode"
  }

}

