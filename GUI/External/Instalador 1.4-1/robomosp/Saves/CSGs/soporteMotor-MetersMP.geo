algebraic3d
solid Solid1016 = cone (-0.003000,0.000000,0.025000;0.025000;0.003000,0.000000,0.025000;0.025000,"./Saves/Materials/jarboter_yellow.mtr") 
	  and plane (-0.003000,0.000000,0.025000;-1.000000,0.000000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") 
	 and plane (0.003000,0.000000,0.025000;1.000000,0.000000,-0.000000,"./Saves/Materials/jarboter_yellow.mtr") ;
solid Solid1037 = plane (-0.003000,-0.025000,0.025000;-1.000000,0.000000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (-0.003000,-0.025000,0.025000;0.000000,-0.006000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (-0.003000,-0.025000,0.025000;0.000000,0.000000,0.006000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.003000,0.025000,0.000000;1.000000,0.000000,-0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.003000,0.025000,0.000000;0.000000,0.006000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.003000,0.025000,0.000000;-0.000000,0.000000,-0.006000,"./Saves/Materials/jarboter_yellow.mtr"); 
solid Solid1058 = plane (0.003000,-0.025000,0.000000;-1.000000,-0.000000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.003000,-0.025000,0.000000;-0.000000,-0.000000,-0.024000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.003000,-0.025000,0.000000;0.000000,-0.024000,0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.027000,0.025000,0.005000;1.000000,0.000000,-0.000000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.027000,0.025000,0.005000;0.000000,0.000000,0.024000,"./Saves/Materials/jarboter_yellow.mtr") and 
	plane (0.027000,0.025000,0.005000;-0.000000,0.024000,0.000000,"./Saves/Materials/jarboter_yellow.mtr"); 
solid Operator1001  = Solid1016  or Solid1037  or Solid1058 ; 
tlo Operator1001;