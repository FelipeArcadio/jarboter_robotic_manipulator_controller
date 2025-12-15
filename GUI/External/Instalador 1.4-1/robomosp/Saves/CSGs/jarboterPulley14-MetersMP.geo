algebraic3d
solid Solid944 = cone (0.000000,0.000000,-0.005500;0.011060;0.000000,0.000000,0.005500;0.011060,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid986 = cone (0.000000,0.000000,0.005500;0.014500;0.000000,0.000000,0.007500;0.014500,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.007500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1007 = cone (0.000000,0.000000,-0.007500;0.014500;0.000000,0.000000,-0.005500;0.014500,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.007500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator929  = Solid944  or Solid986  or Solid1007 ; 
tlo Operator929;