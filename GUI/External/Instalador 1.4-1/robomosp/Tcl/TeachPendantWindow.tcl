#----------------------------------------------------------------------------------------------------------------------------------
# TeachPendantWindow.tcl
# 		This file implements the TCL code for creating and managing the Teach Pendant 
#		Window for the ROBOMOSP application.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-06-09
#
# Requirements:   RobLab.tcl
#			              Menu.tcl
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

# Initialization of Tk objects

  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
namespace eval TeachPendantWindow {

  # Initialization of global variables
  variable tplTeach .wndTeachWindow                   #Main Teach Pendant Window.
  variable frmTeachAll $tplTeach.frmTeachAll        #Main Teach Pendant frame.
  variable TeachWindowInit 0
  variable WorldId
  variable PRobotId
  variable DOF
  variable NumPTrajectories
  variable Index
  variable Value
  
	# UpdateJointsValues procedure --
	#
	#
	# Arguments:
	#
	# Results:
	#
  	proc UpdateJointsValues { JointValues } {
      	variable tplTeach
  		variable frmTeachAll
             variable DOF
              
    		 for {set I 1 } { $I <= $DOF } {incr I} {
             	set Value [ lindex $JointValues [ expr { $I-1 } ] ]
				eval $frmTeachAll.frmjoints.frmview.values.$I.entCell_$I delete 0 end
    			if { $Value != "." } {
				$frmTeachAll.frmjoints.frmview.values.$I.entCell_$I insert 0 $Value
			}
    		}       
	}
  
  #  Delete  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Delete {} {
    variable tplTeach
    variable TeachWindowInit
    set TeachWindowInit 0 
    wm withdraw $tplTeach
  }
  
  #  Nest  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Nest { } {
      variable tplTeach
  	variable frmTeachAll
      variable PRobotId

	# Constants
	set Pi 3.1415926535897932384626433832795
	set Degrees [ expr 180.0 / $Pi ]
      
	# mNestAngles
	set mNestAngles [ IOIC::PRobot $PRobotId LLRL mGetNestAngles ]
	#mDNestAngles
	set mDNestAngles [ lindex $mNestAngles 0 ]
	set i 0
	foreach { Value } $mDNestAngles {
		set mDNestAngles [ lreplace $mDNestAngles $i $i [ expr $Value * $Degrees ] ]
		incr i
	}
      eval "IOIC::PRobot $PRobotId SetLinks $mDNestAngles"
      UpdateJointsValues $mDNestAngles
 }
 
	#  Origin  --
	#
	#
	# Arguments:
	#
	# Results:
	#
  	proc Origin { } {
      variable tplTeach
  	variable frmTeachAll
      variable PRobotId

	# Constants
	set Pi 3.1415926535897932384626433832795
	set Degrees [ expr 180.0 / $Pi ]
      
	# mOriginAngles
	set mOriginAngles [ IOIC::PRobot $PRobotId LLRL mGetOriginAngles ]
	#mDOriginAngles
	set mDOriginAngles [ lindex $mOriginAngles 0 ]
	set i 0
	foreach { Value } $mDOriginAngles {
		set mDOriginAngles [ lreplace $mDOriginAngles $i $i [ expr $Value * $Degrees ] ]
		incr i
	}
      eval "IOIC::PRobot $PRobotId SetLinks $mDOriginAngles"
      UpdateJointsValues $mDOriginAngles
	}
	  
  #  ChangeScaleValues  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc ChangeScaleValues { W } {
      variable tplTeach
  	variable frmTeachAll
      variable PRobotId
      variable Index
      variable Value
  	set Index [ $W cget -textvariable ]
      set Index [ string index $Index 6 ]
      
      set Value [ $W get ]
      IOIC::PRobot $PRobotId Link $Index Set $Value
      
       set MinAngle [ IOIC::PRobot $PRobotId Link $Index GetMinAngle ]
       set MaxAngle [ IOIC::PRobot $PRobotId Link $Index GetMaxAngle ]
       
      $frmTeachAll.frmjoints.frmview.scl configure -variable Joint_$Index -from $MinAngle -to $MaxAngle -resolution 0.01
      
  }
  
#  ChangePropertyValue  --
#
#
# Arguments:
#
# Results:
#
  proc ChangePropertyValue { W } {
      variable tplTeach
  	variable frmTeachAll
      variable PRobotId
      variable Index
      variable Value
  	
      set Value [ $W get ] 
      if { [ RobLab::IsInteger $Index ] == 0 } {
           set Index 1                      
	}
      IOIC::PRobot $PRobotId Link $Index Set $Value
}
  
  #  Initialize  --
  #
  #
  # Arguments:
  #
  # Results:
  #
  proc Initialize {} {
    variable tplTeach
    variable TeachWindowInit
    variable frmTeachAll 
    variable WorldId
    variable PRobotId
    variable DOF
    variable NumPTrajectories
    variable Index
    
    set TeachWindowInit 1
    set CellWidth 15
    
	# Error Message if NumWorlds is 0.
    	if { [ llength [ IOIC::Worlds LsWorlds ]  ] == 0 } {
      	MessagesWindow::Puts "Error:  Not a world has been loaded. Please load a World definition file.\n"
	} else { 
	# Get WorldId
      	set Index $ObjectNavigator::WorldNodeId
      	set WorldIndex [ IOIC::Worlds GetWorldIndex $Index ]
      	set WorldId [ IOIC::Worlds GetWorldId $WorldIndex ]
		# Error Message if NumPRobots is 0.
      	if {  [ llength [ IOIC::World $WorldId LsPRobots ] ] == 0 } {
        		error "Error: There are not robots in current world, please load a Robot definition file."
		}
		#Getting PRobot DOF
    		set PRobotId [ IOIC::World $WorldId GetPRobotId 1 ]
    		set DOF [ IOIC::PRobot $PRobotId GetDOF ] 
    
		# Initialization of Teach Pendant window
    		RobLab::CreateWindow "" wndTeachWindow "Teach Pendant" "$RobLab::TeachW_Geometry" $RobLab::WndMinSizeX $RobLab::WndMinSizeY . 0 { TeachPendantWindow::Delete } $RobLab::VirtualGridSize
  
		#Left frame
  		frame $frmTeachAll
			#Joints Frame
  			frame $frmTeachAll.frmjoints -relief groove -borderwidth 2
       			frame $frmTeachAll.frmjoints.frmview 
          				label $frmTeachAll.frmjoints.frmview.title -text "Joints:"  -font "-weight bold -size 12" -anchor w
                        	frame $frmTeachAll.frmjoints.frmview.values  
						for {set I 1 } { $I <= $DOF} {incr I} {
							#global variables
        						global Joint_$I
        						set Joint_$I [ IOIC::PRobot $PRobotId Link $I GetAngle ]
							# Row i
                              		frame $frmTeachAll.frmjoints.frmview.values.$I -relief raised -borderwidth 1
								label  $frmTeachAll.frmjoints.frmview.values.$I.frmlabel$I -text "Joint $I" -font "-weight bold -size 10"
                              			pack   $frmTeachAll.frmjoints.frmview.values.$I.frmlabel$I -side left -fill both -expand true -anchor w 
								entry  $frmTeachAll.frmjoints.frmview.values.$I.entCell_$I -textvariable Joint_$I -width $CellWidth -borderwidth 1 -relief sunken -background #fffffffff -justify left
                              			pack  $frmTeachAll.frmjoints.frmview.values.$I.entCell_$I -side left -fill both -expand true -anchor w
                                    		bind $frmTeachAll.frmjoints.frmview.values.$I.entCell_$I <ButtonPress> {  TeachPendantWindow::ChangeScaleValues %W }
                                                bind $frmTeachAll.frmjoints.frmview.values.$I.entCell_$I <Return> {  TeachPendantWindow::ChangeScaleValues %W }
                              		pack  $frmTeachAll.frmjoints.frmview.values.$I -side top -fill both -expand true -anchor w                           
						}
                        	scale $frmTeachAll.frmjoints.frmview.scl -showvalue 0 -variable Joint_$I -orient horizontal -from $ObjectNavigator::ScaleAngleDegreeMinimum -to $ObjectNavigator::ScaleAngleDegreeMaximum -resolution $ObjectNavigator::ScaleMaxAngleFactor
                        	bind $frmTeachAll.frmjoints.frmview.scl <B1-Motion> { TeachPendantWindow::ChangePropertyValue %W }
				pack  $frmTeachAll.frmjoints.frmview $frmTeachAll.frmjoints.frmview.title $frmTeachAll.frmjoints.frmview.values $frmTeachAll.frmjoints.frmview.scl -anchor w -expand true -fill both
  			pack  $frmTeachAll.frmjoints -expand true -fill both -side left -padx 2 -pady 2
  
			#Right frame
			frame $frmTeachAll.frmctrl -relief groove -borderwidth 2
				#Pose buttons frame.
				frame $frmTeachAll.frmctrl.frmpose
					label $frmTeachAll.frmctrl.frmpose.text -text "Poses:"  -font "-weight bold -size 12" -justify left
                              
					#Go to pose buttons
                               frame $frmTeachAll.frmctrl.frmpose.gotobtns -relief groove -borderwidth 2
        					button  $frmTeachAll.frmctrl.frmpose.gotobtns.save1 -text "Go to Pose 1" -command {  }
        					pack     $frmTeachAll.frmctrl.frmpose.gotobtns.save1 -side left -fill both -expand true
        					button  $frmTeachAll.frmctrl.frmpose.gotobtns.save2 -text "Go to Pose 2" -command {   }
        					pack     $frmTeachAll.frmctrl.frmpose.gotobtns.save2 -side left -fill both -expand true
      				pack  $frmTeachAll.frmctrl.frmpose.gotobtns -side bottom -fill both -expand true -padx 2 -pady 2   
					
					#Save Pose buttons
                        	frame $frmTeachAll.frmctrl.frmpose.savebtns -relief groove -borderwidth 2
        					button  $frmTeachAll.frmctrl.frmpose.savebtns.save1 -text "Save Pose 1" -command {  }
        					pack     $frmTeachAll.frmctrl.frmpose.savebtns.save1 -side left -fill both -expand true
        					button  $frmTeachAll.frmctrl.frmpose.savebtns.save2 -text "Save Pose 2" -command {   }
        					pack     $frmTeachAll.frmctrl.frmpose.savebtns.save2 -side left -fill both -expand true
      				pack  $frmTeachAll.frmctrl.frmpose.savebtns -side bottom -fill both -expand true -padx 2 -pady 2
                                                  
				pack  $frmTeachAll.frmctrl.frmpose $frmTeachAll.frmctrl.frmpose.text $frmTeachAll.frmctrl.frmpose.savebtns $frmTeachAll.frmctrl.frmpose.gotobtns -anchor w
                        
			pack  $frmTeachAll.frmctrl -expand true -fill both -side left -padx 2 -pady 2   
                        
 	pack  $frmTeachAll -side top -expand true -fill both 
  
		#Bottom Frame
     		frame $tplTeach.frmStateButtons -relief groove -borderwidth 2
        		button  $tplTeach.frmStateButtons.cancel -text "Cancel" -command { TeachPendantWindow::Delete }
        		pack     $tplTeach.frmStateButtons.cancel -side left -fill both -expand true
        		button  $tplTeach.frmStateButtons.reset -text "Nest" -command {  TeachPendantWindow::Nest }
        		pack     $tplTeach.frmStateButtons.reset -side left -fill both -expand true
        		button  $tplTeach.frmStateButtons.ok -text "Origin" -command {  TeachPendantWindow::Origin }
        		pack  $tplTeach.frmStateButtons.ok  -side left -fill both -expand true
      	pack  $tplTeach.frmStateButtons -side bottom -fill both -expand true -padx 2 -pady 2
	}
  }
}  

# --------------------------------------------------------------------------------------------------------------------------
# --------------------------------------------------------------------------------------------------------------------------
