algebraic3d
solid Solid1148 = cone (0.000000,0.000000,-0.005500;0.048260;0.000000,0.000000,0.005500;0.048260,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1190 = cone (0.000000,0.000000,0.005500;0.049760;0.000000,0.000000,0.007500;0.049760,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.007500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1211 = cone (0.000000,0.000000,-0.007500;0.049760;0.000000,0.000000,-0.005500;0.049760,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.007500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator1133  = Solid1148  or Solid1190  or Solid1211 ; 
tlo Operator1133;