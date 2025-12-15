algebraic3d
solid Solid2461 = plane (-0.059500,-0.034875,-0.011000;0.000000,0.000000,-1.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (-0.059500,-0.034875,-0.011000;0.000000,-0.022000,0.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (-0.059500,-0.034875,-0.011000;-0.022000,0.000000,0.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (0.059500,0.034875,0.011000;0.000000,0.000000,1.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (0.059500,0.034875,0.011000;0.000000,0.022000,0.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (0.059500,0.034875,0.011000;0.022000,0.000000,0.000000,"./Saves/Materials/jarboter_black.mtr"); 
solid Solid2482 = cone (0.029750,0.000000,-0.023250;0.004750;0.029750,0.000000,0.043250;0.004750,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.029750,0.000000,-0.023250;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.029750,0.000000,0.043250;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator2446  = Solid2461  or Solid2482 ; 
tlo Operator2446;