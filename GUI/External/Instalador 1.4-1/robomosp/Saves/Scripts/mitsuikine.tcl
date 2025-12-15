#----------------------------------------------------------------------------------------------------------------------------------
# IKineUserDefined.tcl
# 		This file implements the TCL code to solve the inverse kinematics problem through
#		the close geometric method for a Mitsubishi RV-M1 manipulator.
#
# Copyright (c) Automation and Robotics Group, Pontificia Universidad Javeriana - Cali.
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:		2006-04-25
#
# Input:	mDHp : RV-M1 DH matrix
#			mInHMT:	Homogeneus transformation matrix that represents a cartesian point.
#
# Ouput:	theta: list of joint angles (in radians).	
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------
proc IKineUserDefined { mDHp mInHMT } {
	set Pi 3.1415926535897932384626433832795
     
     	#250
     	set a2 [ lindex $mDHp  1 1 ]
     	#160
     	set a3 [ lindex $mDHp  2 1 ]
     	#300 (altura de base puede tomarse en 0)
     	set d1 [ lindex $mDHp  0 3 ]
	#dimensión efector final
     	set d5 [ lindex $mDHp  4 3 ]
     
	#n vector
	set n [ list { [ lindex $mInHMT  0 0 ] }  { [ lindex $mInHMT  1 0 ] }  { [ lindex $mInHMT  2 0 ] } ]
	#o vector
      set o [ list { [ lindex $mInHMT  0 1 ] }  { [ lindex $mInHMT  1 1 ] }  { [ lindex $mInHMT  2 1 ] } ]
	#a vector
      set a [ list { [ lindex $mInHMT  0 2 ] }  { [ lindex $mInHMT  1 2 ] }  { [ lindex $mInHMT  2 2 ] } ]
     	#position vector
      set p [ list { [ lindex $mInHMT  0 3 ] }  { [ lindex $mInHMT  1 3 ] }  { [ lindex $mInHMT  2 3 ] } ]
	
	#p(3)=p(3)-d1;
     	set p [ lreplace $p 2 2 [ expr [ lindex $p 2 ] - $d1 ] ]   
     	
      #P muñeca
      set a1 $a
     	set i 0
    	foreach { Value } $a1 {
      	set a1 [ lreplace $a1 $i $i [ expr $Value * $d5 ] ]
      	incr i
	}
     	set i 0
    	foreach { Value } $p {
      	set p [ lreplace $p $i $i [ expr $Value -   [ lindex $a1 $i ] ] ]
      	incr i
     	}
     
    	#Theta 1
	lappend theta [ expr atan2( [ lindex $p 1 ], [ lindex $p 0 ] ) ]
	#Theta 1 range chek
      if { ( [ lindex $theta 0] < -[ expr 150.0 / 180.0 * $Pi ] ) || ([ lindex $theta 0]  > [ expr 150.0 / 180.0 * $Pi ] )} {
      	MessagesWindow::Puts "Error: Theta 1 angle out of range.\n"
	}
      
	#Distancia muñeca hombro
     	set R [ expr sqrt( [expr [ expr pow ( [ lindex $p 0], 2 ) ] + [ expr pow ( [ lindex $p 1], 2 ) ] + [ expr pow ( [ lindex $p 2], 2 ) ] ] ) ]
      
	#Angulo hombro ( theta 2)
     	set beta [ expr atan2( [ lindex $p 2 ], [ expr sqrt( [ expr pow( [ lindex $p 0] , 2 ) + pow ( [ lindex $p 1] , 2 ) ]  ) ] ) ]
     	set term1 [ expr [ expr pow ($R, 2) ] + [ expr pow ( $a2, 2) ] - [ expr pow ($a3, 2) ]  ]
     	set term2 [ expr 2 * $a2 * $R ]
     	set sigma [ expr abs( [ expr acos ( [ expr $term1/$term2]  ) ]  ) ]
     	if { [ lindex $p 2 ] <= 0 } { 
      		lappend theta [ expr  $sigma - $beta]
	} else {
			lappend theta [ expr $beta + $sigma ]
	}
	#Theta 2 range chek
      if { ( [ lindex $theta 1 ] < - [ expr 30.0 / 180.0 * $Pi ] ) || ([ lindex $theta 1 ]  > [ expr 100.0 / 180.0 * $Pi ] )} {
      	MessagesWindow::Puts "Error: Theta 2 angle out of range.\n"
	}
       
	#Theta 3. This angle is always negative.
    	set term1 [ expr [ expr pow ($R, 2) ] - [ expr pow ( $a2, 2) ] - [ expr pow ($a3, 2) ]  ]
     	set term2 [ expr 2 * $a2 * $a3 ]
     	lappend theta [ expr -abs( [ expr acos ( [ expr $term1/$term2 ] )  ] ) ]
	#Theta 3 range chek
      if { [ lindex $theta 2 ] < - [ expr 110.0 / 180.0 * $Pi ] } {
      	MessagesWindow::Puts "Error: Theta 3 angle out of range.\n"
	}
      
	#Theta 4
      set T1 [ IOIC::FKine $mDHp $theta ]      
      set term1 [ expr [ expr [  lindex $a 0] * [ lindex $T1 0 0  ] ]  + [ expr [  lindex $a 1] * [ lindex $T1 1 0  ] ]  + [ expr [  lindex $a 2] * [ lindex $T1 2 0  ] ] ]
      set term2 -[ expr [ expr [  lindex $a 0] * [ lindex $T1 0 1  ] ]  + [ expr [  lindex $a 1] * [ lindex $T1 1 1  ] ]  + [ expr [  lindex $a 2] * [ lindex $T1 2 1  ] ] ]
      lappend theta [ expr atan2 ($term1, $term2)]       
     	if { ( [ lindex $theta 3 ] < - $Pi ) || ( [ lindex $theta 3 ] > $Pi )} {
		MessagesWindow::Puts "Error: Theta 4 angle out of range.\n"
	}
     
     #Theta 5
     	set T1 [ IOIC::FKine $mDHp $theta ]
     	set term1 [ expr [ expr [  lindex $n 0] * [ lindex $T1 0 1  ] ]  + [ expr [  lindex $n 1] * [ lindex $T1 1 1  ] ]  + [ expr [  lindex $n 2] * [ lindex $T1 2 1  ] ] ]
     	set term2 [ expr [ expr [  lindex $o 0] * [ lindex $T1 0 1  ] ]  + [ expr [  lindex $o 1] * [ lindex $T1 1 1  ] ]  + [ expr [  lindex $o 2] * [ lindex $T1 2 1  ] ] ]
     	lappend theta [ expr atan2( $term1, $term2) ]
      
      return $theta
}