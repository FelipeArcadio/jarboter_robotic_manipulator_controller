algebraic3d
solid Solid511 = cone (-0.010000,0.000000,-0.000000;0.020000;0.010000,0.000000,0.000000;0.020000,"./Saves/Materials/jarboter_yellow.mtr") 
	  and plane (-0.010000,0.000000,-0.000000;-1.000000,0.000000,-0.000000,"./Saves/Materials/jarboter_yellow.mtr") 
	 and plane (0.010000,0.000000,0.000000;1.000000,0.000000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") ;
solid Solid532 = plane (-0.010000,0.013200,-0.014140;0.000000,0.000000,-1.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (-0.010000,0.013200,-0.014140;0.000000,-0.028280,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (-0.010000,0.013200,-0.014140;-0.028280,0.000000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.010000,0.028200,0.014140;0.000000,0.000000,1.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.010000,0.028200,0.014140;0.000000,0.028280,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.010000,0.028200,0.014140;0.028280,0.000000,0.000000,"./Saves/Materials/jarboter_yellow.mtr"); 
solid Operator496  = Solid511  or Solid532 ; 
tlo Operator496;