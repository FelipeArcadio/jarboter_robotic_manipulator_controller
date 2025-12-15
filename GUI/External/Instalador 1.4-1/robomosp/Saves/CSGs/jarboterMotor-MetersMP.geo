algebraic3d
solid Solid567 = cone (0.000000,0.000000,0.000000;0.025000;0.000000,0.000000,0.070000;0.025000,"./Saves/Materials/jarboter_black.mtr") 
	  and plane (0.000000,0.000000,0.000000;0.000000,0.000000,-1.000000,"./Saves/Materials/jarboter_black.mtr") 
	 and plane (0.000000,0.000000,0.070000;0.000000,0.000000,1.000000,"./Saves/Materials/jarboter_black.mtr") ;
solid Solid588 = cone (0.000000,0.000000,0.069950;0.017500;0.000000,0.000000,0.074450;0.017500,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,0.069950;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.074450;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid609 = cone (0.000000,0.000000,-0.005000;0.003175;0.000000,0.000000,0.095000;0.003175,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.005000;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.095000;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator552  = Solid567  or Solid588  or Solid609 ; 
tlo Operator552;