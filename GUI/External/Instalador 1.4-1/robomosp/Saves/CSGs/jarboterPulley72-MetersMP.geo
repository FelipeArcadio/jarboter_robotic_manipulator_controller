algebraic3d
solid Solid1250 = cone (0.000000,0.000000,-0.005500;0.057960;0.000000,0.000000,0.005500;0.057960,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1292 = cone (0.000000,0.000000,0.005500;0.059500;0.000000,0.000000,0.007500;0.059500,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,0.005500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,0.007500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Solid1313 = cone (0.000000,0.000000,-0.007500;0.059500;0.000000,0.000000,-0.005500;0.059500,"./CfgsFiles/MtrDefault.mtr") 
	  and plane (0.000000,0.000000,-0.007500;0.000000,0.000000,-1.000000,"./CfgsFiles/MtrDefault.mtr") 
	 and plane (0.000000,0.000000,-0.005500;0.000000,0.000000,1.000000,"./CfgsFiles/MtrDefault.mtr") ;
solid Operator1235  = Solid1250  or Solid1292  or Solid1313 ; 
tlo Operator1235;