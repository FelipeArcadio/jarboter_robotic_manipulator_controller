# --------------------------------------------------------------------------------------------------------------------------
#  RobLab.tcl
#		This is the main file for the ROBOMOSP Tcl/Tk Graphical User Interface.
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

 # Platform checking
  if { $::tcl_platform(platform) == "windows" } {
      set Global_FontSize 10
  } else {
     set Global_FontSize 10
  }

 namespace eval RobLab {

  global HOME
  # Initialization of namespace global variables
  variable ApplicationsTitle "ROBOMOSP - v1.4"

  # ROBOMOSP windows Geometry
  variable RL_Geometry

  # ObjectNavigator window geometry
  variable ON_Geometry

  # ConsoleWindow window geometry
  variable CW_Geometry

  # SocketsWindow window geometry
  variable SK_Geometry

 # MatrixDisplay window geometry
  variable MD_Geometry
  
  # FKine MatrixDisplay window geometry
  variable FKineMD_Geometry
  
  # Kine Vel MatrixDisplay window geometry
  variable VelMD_Geometry
  
  # Kine Accel MatrixDisplay window geometry
  variable AccelMD_Geometry
  
  # Kine Accel1 MatrixDisplay window geometry
  variable AccelMD1_Geometry
  
 # Configuration CheckList window geometry
  variable CfgCLW_Geometry
  
  # Teach pendant window geometry
  variable TeachW_Geometry
  
  # Kinematics CheckList window geometry
  variable KineCLW_Geometry
  
  # Forward Dynamics CheckList window geometry
  variable FwdDynCLW_Geometry
  
  # Inverse Dynamics CheckList window geometry
  variable InvDynCLW_Geometry
  
  # Jacobian window geometry
  variable JC_Geometry
  
  # Quaternion vector geometry
  variable Quat_Geometry

  # PropertyPalette window geometry
  variable PP_Geometry

  # MessagesWindow window geometry
  variable MW_Geometry

  # DisplayWindow window geometry
  variable DP_Geometry

  # Trajectories GNUPlot windows geometry
  variable GP_Geometry
  variable GPw1_Geometry
  variable GPw2_Geometry
  variable GPw3_Geometry
  variable GPw4_Geometry

  # Inverse Kinematics GNUPlot windows geometry
  variable InvKineGP_Geometry
  variable InvKineGPw1_Geometry
  variable InvKineGPw2_Geometry
  variable InvKineGPw3_Geometry
  
  # Forward Dynamics GNUPlot windows geometry
  variable FwdDynGP_Geometry
  variable FwdDynGPw1_Geometry
  variable FwdDynGPw2_Geometry
  variable FwdDynGPw3_Geometry
   
  # Inverse Dynamics GNUPlot windows geometry
  variable InvDynGP_Geometry
  variable InvDynGPw1_Geometry

  # Inverse Kinematics Simulation Toolbar Geometry
  variable InvKineTB_Geometry
  
  # Inverse Dynamics Simulation Toolbar Geometry
  variable InvDynTB_Geometry
  
  # Gravity Window
  variable GravityWindow_Geometry
  
  # Forward Dynamics Simulation Toolbar Geometry
  variable FwdDynTB_Geometry
  variable VerletFwdDynTB_Geometry

  # Arbitrary Axis Rotation Toolbar Geometry
  variable RotVecTB_Geometry

  variable VirtualGridSize 7
  variable WndMinSizeX [ expr $VirtualGridSize * 18 ]
  variable WndMinSizeY [ expr $VirtualGridSize * 3 ]
  variable WndMinSizeYRoot [ expr $VirtualGridSize * 2 ]
  variable CfgsFileWindowsGeometry "./CfgsFiles/WindowsGeometry.tcl"
  variable CfgsFileLighting "./CfgsFiles/Lighting.lgh"
  variable MaterialNumberFiles 15
  variable MultiplePurposeFlag
  
  #It states if the IRLWindow is active or not.
  variable IRLWindowInit 0
  
  #It states if the VelWindow is active or not.
  variable VelMatrixDisplayInit 0
  
  #Kinematics Simulation variables
  variable KinePosOrient 0
  variable KineVel 0
  variable KineAccel 0
  variable Jacobian 0
  variable IKineType Default
  
  #Dynamics Simulation variables
  variable IntegratorType Ode45
  
  #Main ToolBar Frame
  variable MainTFrame
  
  # OnApplicationsExit procedure --
  #
  #		This procedure closes the ROBOMOSP application whenever the
  #        Exit button is clicked.
  #
  # Arguments: None
  #
  # Results: Application Exit.
  #
  proc OnApplicationsExit {} {

    #Close connections if the SocketsWindow is active
     if { $SocketsWindow::SKWindowInit==1  } {
    	# Sockets: Close client connections
    	set ToolTip [ ToolBars::GetButtonToolTip $SocketsWindow::frmSKTB Port ]
    	if { $ToolTip == "RClose" } {
      		SocketsWindow::CommandClosePort
    	}
     }

     #Destroy MetricsUnits Lists.
     TclDestroyMetricUnitsLists

     # Geometry
     if { $::tcl_platform(platform) == "windows" } {
      #SaveWindowsGeometry
     }
     
      #End application.
     TclExit
  }

  # CreateWindow procedure --
  #
  #		This procedure is used to create all the windows of the ROBOMOSP
  #        Graphical User Interface.
  #
  # Arguments: None
  #
  # Results: Application Exit.
  #
  proc CreateWindow { Parent Name Title Geometry { MinSizeX 0 } { MinSizeY 0 } { Transient "" } { Overrideredirect 0 } { DeleteCommand {} } { VirtualGridSize 1 } } {
    set Window $Parent.$Name
    catch { destroy $Window }
    toplevel $Window
    wm title $Window "$Title"
    wm geometry $Window "$Geometry"
    wm minsize $Window $MinSizeX $MinSizeY
    if { $Transient != "" } {
      wm transient $Window $Transient
    }
    if { $DeleteCommand != {} } {
      wm protocol $Window WM_DELETE_WINDOW "$DeleteCommand"
    }
    wm overrideredirect $Window $Overrideredirect
    if { ( $::tcl_platform(platform) == "windows"  ) && ( $VirtualGridSize > 1 ) } {
      bind $Window <Configure> "RobLab::CheckWindowVirtualGridSize %W $Window $VirtualGridSize"
    }
    return $Window
  }

  # CreateWindowVirtualGridSize procedure --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CheckWindowVirtualGridSize { WindowReported Window VirtualGridSize } {
    variable Contador
    if { $WindowReported == $Window } {
      scan [ winfo geometry $Window ] "%dx%d+%d+%d" Width Height Left Top
      set NewWidth [ RoundToMultipleOf $Width $VirtualGridSize ]
      set NewHeight [ RoundToMultipleOf $Height $VirtualGridSize ]
      set NewLeft [ RoundToMultipleOf $Left $VirtualGridSize ]
      set NewTop [ RoundToMultipleOf $Top $VirtualGridSize ]
      if { ( $Width != $NewWidth ) || ( $Height != $NewHeight ) || ( $Left != $NewLeft ) || ( $Top != $NewTop ) } {
        wm geometry $Window "$NewWidth\x$NewHeight\+$NewLeft\+$NewTop"
      }
    }
  }

  # SaveWindowsGeometry procedure --
  #
  #		This procedure saves the geometry values for all the ROBOMOSP windows
  #		whenever the application is closed.
  #
  # Arguments: None
  #
  # Results:  The file ./CfgsFiles/WindowsGeometry.tcl is generated.
  #
  proc SaveWindowsGeometry {} {
    variable CfgsFileWindowsGeometry
    set Error [ catch {set FileId [ open "$CfgsFileWindowsGeometry" w ]} ErrorInfo ]
    if { $Error != 0 } {
      return $ErrorInfo
    }
    puts $FileId "if { \"\[ winfo screenwidth . \]x\[ winfo screenheight . \]\" == \"[ winfo screenwidth . ]x[ winfo screenheight . ]\" } {"
    puts $FileId "  set RobLab::RL_Geometry \"[ winfo geometry . ]\""
    puts $FileId "  set RobLab::ON_Geometry \"[ winfo geometry $ObjectNavigator::tplON ]\""
    puts $FileId "  set RobLab::CW_Geometry \"[ winfo geometry $ConsoleWindow::tplCW ]\""
    puts $FileId "  set RobLab::SK_Geometry \"[ winfo geometry $SocketsWindow::tplSK ]\""
    puts $FileId "  set RobLab::MD_Geometry \"[ winfo geometry $MatrixDisplay::tplMD ]\""
    puts $FileId "  set RobLab::PP_Geometry \"[ winfo geometry $PropertyPalette::tplPP ]\""
    puts $FileId "  set RobLab::MW_Geometry \"[ winfo geometry $MessagesWindow::tplMW ]\""
    puts $FileId "  set RobLab::DP_Geometry \"[ winfo geometry $DisplayWindow::tplDP ]\""
    puts $FileId "  set RobLab::GP_Geometry \"[ winfo geometry $GNUPlot::tplGP ]\""
    puts $FileId "} else {\n  set RobLab::MultiplePurposeFlag 1\n}"
    close $FileId
  }

  # LoadWindowsGeometry procedure --
  #
  #		This procedure loads the geometry values for all the ROBOMOSP windows
  #		whenever the application is started.
  #
  # Arguments: None
  #
  # Results:  The file ./CfgsFiles/WindowsGeometry.tcl is generated.
  #
  proc LoadWindowsGeometry {} {
    variable RL_Geometry
    variable ON_Geometry
    variable CW_Geometry
    variable SK_Geometry
    variable MD_Geometry
    variable FKineMD_Geometry
    variable VelMD_Geometry
    variable AccelMD_Geometry
    variable AccelMD1_Geometry
    variable JC_Geometry
    variable Quat_Geometry
    variable PP_Geometry
    variable MW_Geometry
    variable DP_Geometry
    variable GP_Geometry
    variable GPw1_Geometry
    variable GPw2_Geometry
    variable GPw3_Geometry
    variable GPw4_Geometry
    variable InvKineGP_Geometry
    variable InvKineGPw1_Geometry
    variable InvKineGPw2_Geometry
    variable InvKineGPw3_Geometry
    variable FwdDynGP_Geometry
    variable FwdDynGPw1_Geometry
    variable FwdDynGPw2_Geometry
    variable FwdDynGPw3_Geometry
    variable InvDynGP_Geometry
    variable InvDynGPw1_Geometry
    variable IRL_Geometry
    variable InvKineTB_Geometry
    variable InvDynTB_Geometry
    variable FwdDynTB_Geometry
    variable VerletFwdDynTB_Geometry
    variable CfgsFileWindowsGeometry
    variable RotVecTB_Geometry
    variable KineCLW_Geometry
    variable FwdDynCLW_Geometry
    variable CfgCLW_Geometry
    variable InvDynCLW_Geometry
    variable MultiplePurposeFlag
    variable GravityWindow_Geometry
    variable TeachW_Geometry
    if { $::tcl_platform(platform) == "windows" } {
      # Load window geometry if exist
      set MultiplePurposeFlag 0
      set FileExist [ file exist "$CfgsFileWindowsGeometry" ]
      if { $FileExist == 1 } {
        source "$CfgsFileWindowsGeometry"
        if { $MultiplePurposeFlag == 0 } {
          return
        }
      }
    }
    switch [ winfo screenwidth . ]x[ winfo screenheight . ] {
      640x480   {
                  set RL_Geometry "616x14+7+7"
                  set ON_Geometry "287x189+7+70"
                  set CW_Geometry "315x84+308+364"
                  set SK_Geometry "287x161+7+287"
                  set MD_Geometry "315x84+308+364"
                  set PP_Geometry "287x161+7+287"
                  set MW_Geometry "315x84+308+364"
                  set DP_Geometry "315x266+308+70"
                  set GP_Geometry "610x25+192+70"
                }
      800x600   {
                  set RL_Geometry "777x14+7+7"
                  set ON_Geometry "364x273+7+70"
                  set CW_Geometry "399x84+385+483"
                  set SK_Geometry "364x196+7+371"
                  set MD_Geometry "364x196+7+371"
                  set PP_Geometry "364x196+7+371"
                  set MW_Geometry "399x84+385+483"
                  set DP_Geometry "399x385+385+70"
                  set GP_Geometry "610x25+75+55"
                  set GPw1_Geometry "+75+110"
                  set GPw2_Geometry "+383+110"
                  set GPw3_Geometry "+75+345"
                  set GPw4_Geometry "+383+345"
                  set InvKineGP_Geometry "610x25+75+55"
                  set InvKineGPw1_Geometry "+75+110"
                  set InvKineGPw2_Geometry "+509+98"
                  set InvKineGPw3_Geometry "+295+288"
                  set FwdDynGP_Geometry "610x25+75+55"
                  set FwdDynGPw1_Geometry "+75+110"
                  set FwdDynGPw2_Geometry "+509+98"
                  set FwdDynGPw3_Geometry "+295+288"
                  set InvDynGP_Geometry "610x25+75+55"
                  set InvDynGPw1_Geometry "+75+110"
                  set KineCLW_Geometry "200x170+194+60"
                  set InvKineTB_Geometry "210x25+226+60"
                  set FKineMD_Geometry "370x84+7+640"
                  set JC_Geometry "585x165+7+371"
                  set RotVecTB_Geometry  "196x31+192+42"
                  set Quat_Geometry "455x24+20+240"
                  set IRL_Geometry "455x190+7+443"
                  set FwdDynTB_Geometry "230x235+200+60"
                  set VelMD_Geometry "370x100+152+270"
                  set AccelMD_Geometry "370x100+152+408"
                  set AccelMD1_Geometry "370x100+152+270"
                  set FwdDynCLW_Geometry "210x250+194+60"
                  set InvDynCLW_Geometry "205x150+194+60"
                  set InvDynTB_Geometry "210x25+226+60"
                }
      1024x768  {
                  if { $::tcl_platform(platform) == "windows" } {
                       set RL_Geometry "1001x35+7+2"
                       set ON_Geometry "455x320+7+85"
                       set CW_Geometry "455x170+7+443"
                       set SK_Geometry "455x170+7+443"
                       set MD_Geometry "455x84+7+620"
                       set PP_Geometry "455x150+7+442"
                       set MW_Geometry "532x84+476+620"
                       set DP_Geometry "532x505+476+85"
                       set GP_Geometry "610x25+192+62"
                       set GPw1_Geometry "+189+118"
                       set GPw2_Geometry "+497+118"
                       set GPw3_Geometry "+189+353"
                       set GPw4_Geometry "+497+353"
                       set InvKineGP_Geometry "610x25+192+42"
                       set InvKineGPw1_Geometry "+7+288"
                       set InvKineGPw2_Geometry "+342+288"
                       set InvKineGPw3_Geometry "+678+288"
                       set FwdDynGP_Geometry "610x25+192+42"
                       set FwdDynGPw1_Geometry "+7+288"
                       set FwdDynGPw2_Geometry "+342+288"
                       set FwdDynGPw3_Geometry "+678+288"
                       set InvDynGP_Geometry "610x25+192+62"
                       set InvDynGPw1_Geometry "+189+118"
                       set InvKineTB_Geometry "240x25+226+60"
                       set InvDynTB_Geometry "242x25+226+60"
                       set FKineMD_Geometry "370x84+7+640"
                       set JC_Geometry "585x165+7+442"
                       set RotVecTB_Geometry  "196x31+192+42"
                       set Quat_Geometry "455x24+20+240"
                       set IRL_Geometry "455x190+7+443"
                       set FwdDynTB_Geometry "230x235+200+85"
                       set VerletFwdDynTB_Geometry "230x265+230+85"
                       set KineCLW_Geometry "200x170+194+60"
                       set VelMD_Geometry "370x100+152+270"
                       set AccelMD_Geometry "370x100+152+408"
                       set AccelMD1_Geometry "370x100+152+270"
                       set FwdDynCLW_Geometry "210x250+194+60"
                       set InvDynCLW_Geometry "205x150+194+60"
                       set CfgCLW_Geometry "410x250+250+85"
                       set TeachW_Geometry "190x280+250+85"
                     } else {
                       set RL_Geometry "1001x30+7+2"
                       set ON_Geometry "455x330+7+90"
                       set DP_Geometry "532x527+476+90"
                       set PP_Geometry "455x160+7+454"
                       set MD_Geometry "455x95+7+648" 
                       set MW_Geometry "532x84+476+651"
                       set CW_Geometry "455x170+7+443"
                       set SK_Geometry "455x170+7+443"
                       set GP_Geometry "610x25+192+62"
                       set GPw1_Geometry "+192+128"
                       set GPw2_Geometry "+500+128"
                       set GPw3_Geometry "+192+363" 
                       set GPw4_Geometry "+500+363"
		           				 set InvKineGP_Geometry "610x25+192+62"
		           				 set InvKineGPw1_Geometry "+36+364"
		           				set InvKineGPw2_Geometry "+365+364"
		           				set InvKineGPw3_Geometry "+692+364"
                       set FwdDynGP_Geometry "610x25+192+62"
		           				set FwdDynGPw1_Geometry "+36+364"
		           				set FwdDynGPw2_Geometry "+365+364"
		           				set FwdDynGPw3_Geometry "+692+364"
                       set FwdDynTB_Geometry "230x235+258+90"
                       set VerletFwdDynTB_Geometry "247x265+258+90"
		           				set InvDynGP_Geometry "610x25+37+122"
                       set InvDynGPw1_Geometry "+37+120"
                       set InvKineTB_Geometry "250x25+226+62"
                       set InvDynTB_Geometry "242x25+226+60"
                       set GravityWindow_Geometry "126x170+270+90"
			     						set FKineMD_Geometry "370x100+152+130"
                       set JC_Geometry "585x165+7+442"
			     						set RotVecTB_Geometry  "196x31+192+62"
                       set Quat_Geometry "455x30+230+600"
                       set IRL_Geometry "455x190+7+443"                    
                       set KineCLW_Geometry "200x170+194+60"
                       set VelMD_Geometry "370x100+152+270"
                       set AccelMD_Geometry "370x100+152+408"
                       set AccelMD1_Geometry "370x100+152+270"
                       set CfgCLW_Geometry "410x250+250+60"
                       set FwdDynCLW_Geometry "210x350+250+60"
                       set InvDynCLW_Geometry "230x150+194+60"
                       set TeachW_Geometry "400x250+250+90"
                     }
                }
      1152x864  {
                  if { $::tcl_platform(platform) == "windows" } {
                       set RL_Geometry "1132x14+7+7"
                       set ON_Geometry "448x390+7+70"
                       set CW_Geometry "448x203+7+492"
		           set IRL_Geometry "448x203+7+492"
                       set SK_Geometry "448x203+7+492"
                       set MD_Geometry "448x95+7+723"
                       set Quat_Geometry "448x95+7+723"
                       set PP_Geometry "448x203+7+492"
                       set MW_Geometry "671x95+467+723"
                       set DP_Geometry "671x625+467+70"
                       set GP_Geometry "610x25+300+70"
                       set GPw1_Geometry "+300+125"
                       set GPw2_Geometry "+608+125"
                       set GPw3_Geometry "+300+360" 
                       set GPw4_Geometry "+608+360"
		           set InvKineGP_Geometry "610x25+300+70"
		           set InvKineGPw1_Geometry "+280+125"
		           set InvKineGPw2_Geometry "+615+125"
		           set InvKineGPw3_Geometry "+430+375"
                       set FwdDynGP_Geometry "610x25+300+70"
		           set FwdDynGPw1_Geometry "+280+125"
		           set FwdDynGPw2_Geometry "+615+125"
		           set FwdDynGPw3_Geometry "+430+375"
		           set InvDynGP_Geometry "610x25+300+70"
                       set InvDynGPw1_Geometry "+300+125"
			     set InvKineTB_Geometry "240x25+192+42"
                       set InvDynTB_Geometry "210x25+226+60"
		           set FKineMD_Geometry "370x100+152+130"
                       set JC_Geometry "585x165+7+442"
			     set RotVecTB_Geometry  "196x31+192+62"
			     set FwdDynTB_Geometry "230x200+192+42"
			     set KineCLW_Geometry "230x200+194+60"
                       set VelMD_Geometry "370x100+152+270"
                       set AccelMD_Geometry "370x100+152+408"
                       set AccelMD1_Geometry "370x100+152+270"
			     set FwdDynCLW_Geometry "210x250+194+60"
			     set InvDynCLW_Geometry "230x150+194+60"
                       set CfgCLW_Geometry "410x280+250+60"
                       set VerletFwdDynTB_Geometry "247x265+214+60"
                     } else {
                       set RL_Geometry "1132x14+7+7"
                       set ON_Geometry "448x390+7+70"
                       set CW_Geometry "448x203+7+492"
		     	     set IRL_Geometry "600x203+7+492"
                       set SK_Geometry "448x203+7+492"  
                       set MD_Geometry "448x95+7+728"
                       set Quat_Geometry "458x30+250+600"
                       set PP_Geometry "448x203+7+492" 
                       set MW_Geometry "678x95+460+728"
                       set DP_Geometry "678x625+460+70"
                       set GP_Geometry "610x25+300+70"
                       set GPw1_Geometry "+300+128"
                       set GPw2_Geometry "+608+128"
                       set GPw3_Geometry "+300+363"
                       set GPw4_Geometry "+608+363"
		           set InvKineGP_Geometry "610x25+300+70"
		           set InvKineGPw1_Geometry "+280+125"
		           set InvKineGPw2_Geometry "+615+125"
		           set InvKineGPw3_Geometry "+430+375"
                       set FwdDynGP_Geometry "610x25+300+70"
		           set FwdDynGPw1_Geometry "+280+125"
		           set FwdDynGPw2_Geometry "+615+125"
		           set FwdDynGPw3_Geometry "+430+375"
		           set InvDynGP_Geometry "610x25+300+70"
                       set InvDynGPw1_Geometry "+300+128"
	                 set InvKineTB_Geometry "210x25+275+70"
                       set InvDynTB_Geometry "210x25+226+60"
			     set FKineMD_Geometry "370x100+190+132"
                       set JC_Geometry "585x165+80+535"
			     set RotVecTB_Geometry  "196x31+229+72"
			     set FwdDynTB_Geometry "230x230+202+70"
                       set KineCLW_Geometry "230x200+200+71"
                       set VelMD_Geometry "370x100+190+266"
                       set AccelMD_Geometry "370x100+190+400"
                       set AccelMD1_Geometry "370x100+190+266"
                       set FwdDynCLW_Geometry "210x270+220+70"
			     set InvDynCLW_Geometry "230x170+200+70"
                     }
                }
      1280x1024 {
                  set RL_Geometry "1260x34+7+1"
                  set ON_Geometry "448x357+7+95"
                  set CW_Geometry "448x112+7+840"
                  set SK_Geometry "448x168+7+686"
                  set MD_Geometry "399x112+469+840"
                  set Quat_Geometry "399x84+469+910"
                  set PP_Geometry "448x300+7+495"
                  set MW_Geometry "385x112+882+840"
                  set DP_Geometry "798x700+469+95"
                  set GP_Geometry "610x25+300+70"
                  set GPw1_Geometry "+300+128"
                  set GPw2_Geometry "+608+128"
                  set GPw3_Geometry "+300+363"
                  set GPw4_Geometry "+608+363"
		      set InvKineGP_Geometry "610x25+192+62"
		      set InvKineGPw1_Geometry "+36+364"
		      set InvKineGPw2_Geometry "+365+364"
		      set InvKineGPw3_Geometry "+692+364"
                  set FwdDynGP_Geometry "610x25+192+62"
		      set FwdDynGPw1_Geometry "+36+364"
		      set FwdDynGPw2_Geometry "+365+364"
		      set FwdDynGPw3_Geometry "+692+364"                  
		      set InvDynGP_Geometry "610x25+300+70"
                  set InvDynGPw1_Geometry "+300+128"
			set RotVecTB_Geometry  "196x31+192+62"
                  set InvKineTB_Geometry "240x25+275+70"
                  set InvDynTB_Geometry "210x25+226+60"
			set FKineMD_Geometry "370x100+190+132"
                  set JC_Geometry "585x165+80+535"
			set RotVecTB_Geometry  "196x31+229+72"
			set FwdDynTB_Geometry "230x230+202+70"
                  set KineCLW_Geometry "230x200+200+71"
                  set VelMD_Geometry "370x100+190+266"
                  set AccelMD_Geometry "370x100+190+400"
                  set AccelMD1_Geometry "370x100+190+266"
                  set FwdDynCLW_Geometry "210x300+220+70"
			set InvDynCLW_Geometry "230x200+200+70"
                  set CfgCLW_Geometry "470x320+250+95"
                  set VerletFwdDynTB_Geometry "247x265+214+60"
                }
      default   {
                  # Defaults sets to 1024x768
                  set RL_Geometry "1001x14+7+7"
                  set ON_Geometry "455x350+7+70"
                  set CW_Geometry "455x175+7+448"
                  set SK_Geometry "455x175+7+448"
                  set MD_Geometry "455x84+7+651"
                  set Quat_Geometry "455x84+7+651"
                  set PP_Geometry "455x175+7+448"
                  set MW_Geometry "532x84+476+651"
                  set DP_Geometry "532x553+476+70"
                  set GP_Geometry "610x25+192+70"
                  set GPw1_Geometry "+192+128"
                  set GPw2_Geometry "+500+128"
                  set GPw3_Geometry "+192+363"
                  set GPw4_Geometry "+500+363"
		      set InvKineGP_Geometry "610x25+192+70"
		      set InvKineGPw1_Geometry "+169+98"
		      set InvKineGPw2_Geometry "+509+98"
		      set InvKineGPw3_Geometry "+295+288"
                  set FwdDynGP_Geometry "610x25+192+42"
                  set FwdDynGPw1_Geometry "+169+98"
                  set FwdDynGPw2_Geometry "+509+98"
                  set FwdDynGPw3_Geometry "+295+288"
		      set InvDynGP_Geometry "610x25+192+70"
                  set InvDynGPw1_Geometry "+192+128"
                }
    }
  }

  # StringRightJustify procedure --
  #
  #		This procedure right justifies a string in the ROBOMOSP GUI.
  #
  # Arguments:
  #
  # Results:
  #
  proc StringRightJustify { String Length } {
    set StringLength [ string length $String ]
    if { $StringLength < $Length } {
      set DummyString " "
      for { set i 0 } { $i < ( $Length - $StringLength - 1 ) } { incr i } {
        set DummyString "$DummyString "
      }
      return "$DummyString$String"
    }
    return "$String"
  }

  #  ExpandNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ExpandNTree { Canvas ToolBar CurrentNode } {
    ToolBars::DisableButton $ToolBar Expand
    ToolBars::EnableButton $ToolBar Collapse
    TkNTree::ExpandNTree $Canvas [ expr $$CurrentNode ]
  }

  #  CollapseNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CollapseNTree { Canvas ToolBar CurrentNode } {
    ToolBars::EnableButton $ToolBar Expand
    ToolBars::DisableButton $ToolBar Collapse
    TkNTree::CollapseNTree $Canvas [ expr $$CurrentNode ]
  }

  #  ExpandAllNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ExpandAllNTree { Canvas ToolBar CurrentNode } {
    ToolBars::DisableButton $ToolBar Expand
    ToolBars::EnableButton $ToolBar Collapse
    TkNTree::ExpandAllNTree $Canvas [ expr $$CurrentNode ]
  }

  #  CollapseAllNTree --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc CollapseAllNTree { Canvas ToolBar CurrentNode } {
    ToolBars::EnableButton $ToolBar Expand
    ToolBars::DisableButton $ToolBar Collapse
    TkNTree::CollapseAllNTree $Canvas [ expr $$CurrentNode ]
  }

  # IsInteger --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc IsInteger { String } {
    set Result [ scan $String "%d" Number ]
    if { $Result == 1 } {
      if { $String == $Number } {
        return 1
      } else {
        return 0
      }
    } else {
      return 0
    }
  }

  # IsReal --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc IsReal { String } {
    set Result [ scan $String "%f" Number ]
    if { $Result == 1 } {
      if { [ regexp {[\.]} $String ] == 0 } {
        set String "$String\.0"
      }
      if { $String == $Number } {
        return 1
      } else {
        return 0
      }
    } else {
      return 0
    }
  }
  
  #  RoundToMultipleOf  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc RoundToMultipleOf { Value MultipleOf } {
    set Mod [ expr int( fmod( $Value, $MultipleOf ) ) ]
    if { $Mod != 0 } {
      if { $Mod < [ expr $MultipleOf / 2.0 ] } {
        set Value [ expr $Value - $Mod ]
      } else {
        set Value [ expr $Value + ( $MultipleOf - $Mod ) ]
      }
    }
    return $Value
  }

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable ApplicationsTitle
    variable RL_Geometry
    variable VirtualGridSize
    variable WndMinSizeX
    variable WndMinSizeYRoot
    variable MainTFrame
    variable HOME
    wm title . $ApplicationsTitle
    wm geometry . "$RL_Geometry"
    wm minsize . $WndMinSizeX $WndMinSizeYRoot
    wm protocol . WM_DELETE_WINDOW "RobLab::OnApplicationsExit"
    if { ( $::tcl_platform(platform) == "windows"  ) && ( $VirtualGridSize > 1 ) } {
      bind . <Configure> "RobLab::CheckWindowVirtualGridSize %W . $VirtualGridSize"
    }
    Menu::Initialize
    #Create a horizontal toolbar frame...
    set MainTFrame [::toolbar::ToolbarFrame .toolbarFrame] 
    tkwait visibility .
    MessagesWindow::Initialize
    MatrixDisplay::Initialize
    PropertyPalette::Initialize
    ObjectNavigator::Initialize
    DisplayWindow::Initialize
    InvKineToolBar::Initialize
    GNUPlot::Initialize
    InvDynToolBar::Initialize
    focus $PropertyPalette::tplPP
    focus .
    # Load configurations files
    ObjectNavigator::LoadConfigurationsFiles
    
 }
}

set HOME [ pwd ]
if { $HOME == "/usr/lib/robomosp" } {
      set HOME "/home/amatta/robomosp"
}

#
# ROBOMOSP Windows initialization.
#
RobLab::LoadWindowsGeometry
source ./Tcl/MessagesWindow.tcl
source ./Tcl/MatrixDisplay.tcl
source ./Tcl/QuatDisplay.tcl
source ./Tcl/TkNTree.tcl
source ./Tcl/ToolBars.tcl
source ./Tcl/ConsoleWindow.tcl
source ./Tcl/IRL/IRLWindow.tcl
source ./Tcl/IOIC/IOICMain.tcl
source ./Tcl/SocketsWindow.tcl
source ./Tcl/PropertyPalette.tcl
source ./Tcl/ObjectNavigator/ObjNavMain.tcl
source ./Tcl/CfgCheckList.tcl
source ./Tcl/FwdDynSel.tcl
source ./Tcl/DisplayWindow.tcl
source ./Tcl/Menu.tcl
source ./Tcl/KineCheckList.tcl
source ./Tcl/FwdDynCheckList.tcl
source ./Tcl/InvDynCheckList.tcl
source ./Tcl/InvKineToolBar.tcl
source ./Tcl/InvDynToolBar.tcl
source ./Tcl/GravityWindow.tcl
source ./Tcl/FwdDynToolBar.tcl
source ./Tcl/VerletFwdDynToolBar.tcl
source ./Tcl/RotVecToolBar.tcl
source ./Tcl/FKineDisplay.tcl
source ./Tcl/JacobianDisplay.tcl
source ./Tcl/VelocityDisplay.tcl
source ./Tcl/AccelDisplay.tcl
source ./Tcl/PetasisToolbar.tcl
source ./Tcl/GNUPlot/TjcGNUPlot.tcl
source ./Tcl/GNUPlot/InvKineGNUPlot.tcl
source ./Tcl/GNUPlot/FwdDynGNUPlot.tcl
source ./Tcl/GNUPlot/InvDynGNUPlot.tcl
source ./Tcl/TeachPendantWindow.tcl
# ROBOMOSP Help
     if { $::tcl_platform(platform) != "windows" } {
      #Load ROBOMOSP Help
      source ./Tcl/Help/help.tcl
     }
RobLab::Initialize
