algebraic3d
solid Solid1046 = cone (0.000000,0.000000,-0.005500;0.021120;0.000000,0.000000,0.005500;0.021120,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1088 = cone (0.000000,0.000000,0.005500;0.026000;0.000000,0.000000,0.007500;0.026000,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.007500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1109 = cone (0.000000,0.000000,-0.007500;0.026000;0.000000,0.000000,-0.005500;0.026000,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.007500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator1031  = Solid1046  or Solid1088  or Solid1109 ; 
tlo Operator1031;