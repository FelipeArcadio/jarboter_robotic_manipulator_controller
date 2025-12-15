#----------------------------------------------------------------------------------------------------------------------------------
# Test.tcl
# 		This file implements the TCL code for testing the ROBOMOSP application.
#
# Copyright (c) Robotics and Automation Group, Pontificia Universidad Javeriana - Cali.
#                     Antonio Alejandro Matta Gómez amatta@puj.edu.co
# See the file "license.terms" for information on usage and redistribution of this file, and for a
# DISCLAIMER OF ALL WARRANTIES.
#
# Date:                  2005-08-19
#
# Requirements:   
#
# SCCS: %Z% %M% %I% %E% %U%
#------------------------------------------------------------------------------------------------------------------------------------

#CSGs testing
proc ChangeSphere { NTree iter } {
  $NTree ModifyProperty vPO1 $iter 
  $NTree ModifyProperty vPO2 $iter
  $NTree ModifyProperty vPO3 $iter
  $NTree ModifyProperty vPO4 [expr {$iter*90}]
  $NTree ModifyProperty vPO5 [expr {$iter*90}]
  $NTree ModifyProperty vPO6 [expr {$iter*90}]
  $NTree ModifyProperty SphereRadius  [expr {$iter+0.1}]
  $NTree ModifyProperty SphereSlices 15
  $NTree ModifyProperty SphereStacks 15
}

proc ChangeCylinder { NTree iter } {
  $NTree ModifyProperty vPO1 $iter
  $NTree ModifyProperty vPO2 $iter
  $NTree ModifyProperty vPO3 $iter
  $NTree ModifyProperty vPO4 [expr {$iter*90}]
  $NTree ModifyProperty vPO5 [expr {$iter*45}]
  $NTree ModifyProperty vPO6 [expr {$iter*180}]
  $NTree ModifyProperty CylinderBaseRadius $iter
  $NTree ModifyProperty CylinderTopRadius $iter
  $NTree ModifyProperty CylinderHeigth [expr {$iter+0.3}]
}

proc ChangeCube { NTree iter } {
  $NTree ModifyProperty vPO1 $iter
  $NTree ModifyProperty vPO2 $iter
  $NTree ModifyProperty vPO3 $iter
  $NTree ModifyProperty vPO4 [expr {$iter*180}]
  $NTree ModifyProperty vPO5 [expr {$iter*45}]
  $NTree ModifyProperty vPO6 [expr {$iter*90}]
  $NTree ModifyProperty CubeBaseWidth [expr {$iter+0.2}]
  $NTree ModifyProperty CubeBaseDepth [expr {$iter+0.2}] 
  $NTree ModifyProperty CubeTopWidth [expr {$iter+0.2}]
  $NTree ModifyProperty CubeTopDepth [expr {$iter+0.2}]
  $NTree ModifyProperty CubeHeigth [expr {$iter+0.2}]
}

proc ChangePyramid { NTree iter } {
  $NTree ModifyProperty vPO1 $iter
  $NTree ModifyProperty vPO2 $iter
  $NTree ModifyProperty vPO3 $iter
  $NTree ModifyProperty vPO4 [expr {$iter*45}]
  $NTree ModifyProperty vPO5 [expr {$iter*180}]
  $NTree ModifyProperty vPO6 [expr {$iter*45}]
  $NTree ModifyProperty PyramidBase [expr {$iter+0.1}]
  $NTree ModifyProperty PyramidHeigth [expr {$iter+0.3}]
}

proc ChangeCone { NTree iter } {
  $NTree ModifyProperty vPO1 $iter
  $NTree ModifyProperty vPO2 $iter
  $NTree ModifyProperty vPO3 $iter
  $NTree ModifyProperty vPO4 [expr {$iter*360}]
  $NTree ModifyProperty vPO5 [expr {$iter*180}]
  $NTree ModifyProperty vPO6 [expr {$iter*180}]
  $NTree ModifyProperty ConeBase [expr {$iter+0.1}]
  $NTree ModifyProperty ConeHeigth [expr {$iter+0.1}]
}

proc ChangeSolidsProperties { NTree num iter } {
    switch $num {
        0  { 
                ChangeSphere  $NTree $iter
            }
       1  { 
                ChangeCylinder $NTree $iter
           }
       2  { 
                ChangeCube  $NTree $iter
            }
        3  { 
                ChangePyramid  $NTree $iter
            }
        4  { 
                ChangeCone  $NTree $iter
            }    
    }   
}

proc DefineSolids { num num1 num2 num3 num4 iter } {
  set NTree [ ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstSolidTypes $num ] ] ]
  ChangeSolidsProperties $NTree $num $iter
  set NTree [ ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstSolidTypes $num1 ] ] ]
  ChangeSolidsProperties $NTree $num1 $iter
  set NTree [ ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstSolidTypes $num2 ] ] ]
  ChangeSolidsProperties $NTree $num2 $iter
  set NTree [ ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstSolidTypes $num3 ] ] ]
  ChangeSolidsProperties $NTree $num3 $iter
  set NTree [ ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstSolidTypes $num4 ] ] ]
  ChangeSolidsProperties $NTree $num4 $iter
}

proc DefineOperation { num num1 num2 iter} {
    ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON $ObjectNavigator::CSGsNode
    ObjectNavigator::CreateNode
    ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstOperatorTypes $num ] ]
    ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetFirstSon $ObjectNavigator::CurrentNode ] 1
    ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstOperatorTypes $num1 ] ]
    ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetFirstSon $ObjectNavigator::CurrentNode ] 1
    switch $num {
        0  {   
              DefineSolids $num [expr {$num+1}] [expr {$num+2}]  [expr {$num+3}]  [expr {$num+4}] $iter
            }
        1  {   
              DefineSolids $num [expr {$num-1}] [expr {$num+1}]  [expr {$num+2}]  [expr {$num+3}] $iter
            }
       2  {   
              DefineSolids $num [expr {$num-2}] [expr {$num-1}]  [expr {$num+1}]  [expr {$num+2}] $iter
           }
    } 
    ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetFather $ObjectNavigator::CurrentNode ] 1
    ObjectNavigator::CSGToolBar [ list [ lindex $ObjectNavigator::LstOperatorTypes $num2 ] ]
    ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetFirstSon $ObjectNavigator::CurrentNode ] 1  
    ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetNextBrother $ObjectNavigator::CurrentNode ] 1
      switch $num {
        0  {   
              DefineSolids [expr {$num+4}] [expr {$num+3}] [expr {$num+2}] [expr {$num+1}] $num $iter
            }
        1  {   
              DefineSolids [expr {$num+3}] [expr {$num+2}] [expr {$num+1}] [expr {$num-1}] $num $iter
            }
       2  {   
              DefineSolids [expr {$num+2}] [expr {$num+1}] [expr {$num-1}] [expr {$num-2}] $num $iter
          }
      }         
}

#--------------------------------------------------------------
# Start test.
#
#
#-------------------------------------------------------------


#Only primitives.
#set Solidslength [ llength $ObjectNavigator::LstSolidTypes ]
#for {set i 0 } { $i< $Solidslength } {incr i} {
#   CreateSolid $i
#}

for {set i 0 } { $i<3} {incr i} {
  switch $i {
    0  {   
            DefineOperation $i [expr {$i+1}] [expr {$i+2}] 0
            DefineOperation $i [expr {$i+2}]  [expr {$i+1}] 0.1
        }
    1  {   
            DefineOperation $i [expr {$i-1}] [expr {$i+1}] 0.2
            DefineOperation $i [expr {$i+1}] [expr {$i-1}] 0.3
        }
    2  {  
            DefineOperation $i [expr {$i-2}] [expr {$i-1}] 0.4
            DefineOperation $i [expr {$i-1}] [expr {$i-2}] 0.5
        }   
  }       
}  
ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON $ObjectNavigator::CSGsNode
ObjectNavigator::SwapSelectedObject $ObjectNavigator::cvsON [ NTree::GetFirstSon $ObjectNavigator::CurrentNode ] 1