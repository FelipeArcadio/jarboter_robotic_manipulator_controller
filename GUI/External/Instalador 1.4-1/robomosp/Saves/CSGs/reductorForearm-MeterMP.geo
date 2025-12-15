algebraic3d
solid Solid914 = plane (-0.034000,-0.031750,-0.007000;0.000000,0.000000,-1.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (-0.034000,-0.031750,-0.007000;0.000000,-0.014000,0.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (-0.034000,-0.031750,-0.007000;-0.014000,0.000000,0.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (0.034000,0.031750,0.007000;0.000000,0.000000,1.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (0.034000,0.031750,0.007000;0.000000,0.014000,0.000000,"./Saves/Materials/jarboter_black.mtr") and 
	plane (0.034000,0.031750,0.007000;0.014000,0.000000,0.000000,"./Saves/Materials/jarboter_black.mtr"); 
solid Solid935 = cone (-0.011000,0.000000,0.007050;0.004000;-0.011000,0.000000,0.030550;0.004000,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (-0.011000,0.000000,0.007050;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (-0.011000,0.000000,0.030550;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator899  = Solid914  or Solid935 ; 
tlo Operator899;