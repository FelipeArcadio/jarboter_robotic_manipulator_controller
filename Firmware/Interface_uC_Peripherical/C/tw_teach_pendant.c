#include <avr/io.h>
#include <stdint.h>
#include <util/twi.h>		
#include <stdio.h>
#include "types.h"
#include "tw_teach_pendant.h"
#include "mem.h"
#include "packetIO.h"
//#define DEBUGTWI
//#define DEBUGUSBLevel1


/** \ingroup Teach_Pendant
	\brief Define el numero maximo de Poses o Posturas del Robot que se pueden almacenar en la
	memoria EEPROM*/
	#define MAX_POSES			5696

	/** \ingroup Teach_Pendant
	\brief La Memoria EEPROM esta dividida en dos bloques de Memoria. ADD_NUM define la cantidad
	de bytes que se deben adicionar a la Primera Pose del segundo banco de la memoria para que
	quede en la primera posicion de memoria de esta*/
	#define	ADD_NUM				7

	/** \ingroup Teach_Pendant
	\brief Es un vector que contendra todos los ID (identificadores) de las Poses del Robot almacenados
	en la EEPROM*/
	uint16 *id_vector; 
	/*Esta trajectoria estara compuesta por las posiciones del ID vector*/
	tTrajectory  Trajectoryvector;
	/*Esta Trajectoria se usa para ejecutar una pose ya grabada*/
	tTrajectory  Trajectoryvectorpunto;
	/*Esta Bandera indica que se ejecutado una pose punto por primera vez*/
	int16 FlagTrayecPuntook=0;
	/*Utilizada para acceder a la informacion de moviento velocidades y aceleraciones max */
	extern PMDAxisHandle hAxis[DOF]; 

/*****************************************************************************************************/
int16 Get_Num_Poses(void)
{
	int16 Cpuntos=-1;
	int16 rv=-1;
   
	rv=ee24xx_read_bytes(0,sizeof(int16),(uint8 *)&Cpuntos);
	
	if(rv)
	{
		#ifdef DEBUGTWI
			printf("Get_Num_Poses=> Cpuntos= %d\n",Cpuntos);
		#endif
		if(Cpuntos< MAX_POSES)
			return Cpuntos;
	}

	return rv;
}
/*****************************************************************************************************/
int32 Found_Id(uint16 id)
{
	int16 id_actual=-1;
	int16 Num_Poses;
	int32 addr,rv=-1;
	int16 i=0;
	
	
	
	/*Se Determina la cantidad de Poses en EEPROM*/
	
	Num_Poses = Get_Num_Poses();
	
	if(Num_Poses >= 1)
	{
		for (i=0;i< Num_Poses ;i++)
	    {
			/*Direccion de Comienzo de cada Pose*/
			addr= (int32) ((i)*sizeof(PoseRobot)+sizeof(int16)); 
			
			if(i > (MAX_POSES/2))
				addr=addr+ ADD_NUM;
		 
			rv=ee24xx_read_bytes(addr,sizeof(uint16),(uint8*) &id_actual);
			#ifdef DEBUGTWI
				printf("Found_Id=> rv=%ld \n",rv);
				printf("Found_Id=> i=%d \n",i);
				printf("Found_Id=> addr=%ld \n",addr);
				printf("Found_Id=> id_actual=%d \n",id_actual);
			#endif
			
			if(rv == -1)
				return rv;								/* Se Produce un Error*/
			
			if(id==id_actual)			
				return addr;  							/* Encontro  la pose deseada*/
		}
	}
	return -1;	
	
}
/*****************************************************************************************************/
int16 Adicionar_PoseTWI(PoseRobot Pose)
{
	int16 rv=-1;
	int32 addr;
	int16 Num_Poses;
	
	
	
    /*Se obtiene el numero total de poses que hay en la memoria EEPROM*/
	Num_Poses=Get_Num_Poses();
	#ifdef DEBUGTWI
		printf("Adicionar_PoseTWI=> Numero Poses grabadas Adicionar_PoseTWI => %x\n",Num_Poses);
	#endif

	if(Num_Poses!=-1)
	{   
		/* Num_Poses Es el numero de Poses. Cada Pose Empieza en 1 hasta MAX_Poses*/
		/*La addr no depende del ID*/
		addr=(int32)(Num_Poses)*sizeof(PoseRobot)+sizeof(int16); 
		#ifdef DEBUGTWI
			printf("Adicionar_PoseTWI=> direccion actual en donde grabo la Pose => %lx\n",addr);
		#endif
			
		/*Pasar al Segundo Banco de Memoria */
		if(Num_Poses > (MAX_POSES/2))
			addr=addr+ADD_NUM;
		
		/* grabando pose*/
		rv=ee24xx_write_bytes(addr,sizeof(PoseRobot),(uint8 *)&Pose);	
		#ifdef DEBUGTWI
			printf("Adicionar_PoseTWI=> Numero de datos grabados corectamente al grabar Pose => %d\n",rv);
		#endif
		
		if(rv)
		{
			Num_Poses++;
			
			/*Se Actualiza el Numero de Poses Escritas en EEPROM*/
			rv=ee24xx_write_bytes(0,sizeof(int16),(uint8 *)&Num_Poses);	
			
			#ifdef DEBUGTWI
				printf("Adicionar_PoseTWI=> Numero de datos grabados corectamente al grabar Num_Poses => %d\n",rv);
				printf("Adicionar_PoseTWI=> Num_Poses = %x\n",Num_Poses); 
			#endif		
		}		
	}
	
	return rv;
}

/*****************************************************************************************************/
int16 Leer_PoseTWI(uint16 id, PoseRobot *Pose)
{
	int16 rv=-1;
	int32 addr; 	
	
	
    
	/* Se busca la direccion en donde se encuentra la Pose */	
	addr=Found_Id(id);
	
	
	#ifdef DEBUGTWI
		printf("Leer_PoseTWI=> addr en Leer_PoseTWI => %ld \n",addr);
	#endif	
	
	/* Las direcciones validas de las poses empiezan a partir de 2*/
	if(addr >= 2)									
	{
		rv=ee24xx_read_bytes(addr,sizeof(PoseRobot),(uint8*) Pose);
		#ifdef DEBUGTWI
			printf("Leer_PoseTWI=> La cantidad de datos leidos es en Leer_PoseTWI => %d \n",rv);
		#endif	
	}	
	
	
	
	return rv;
}
/*****************************************************************************************************/
int16 SobreEscribir_Pose(PoseRobot Pose)
{
	int16 rv=-1;
	int32 addr; 	
	
	
    
	/* Se busca la direccion en donde se encuentra la Pose*/	
	addr=Found_Id(Pose.id);
	
	
	#ifdef DEBUGTWI
		printf("SobreEscribir_Pose=> addr en SobreEscribir_Pose => %ld \n",addr);
	#endif	
	
	/* Las direcciones validas de las poses empiezan a partir de 2*/
	if(addr >= 2)									
	{
		rv=ee24xx_write_bytes(addr,sizeof(PoseRobot),(uint8*) &Pose);
		#ifdef DEBUGTWI
			printf("SobreEscribir_Pose=> La cantidad de datos escritos es en SobreEscribir_Pose => %d \n",rv);
		#endif	
	}	
	
	return rv;

}

/*****************************************************************************************************/
int16 return_id_vector(void)
{
	int16 Num_Poses;
	int16 id_actual=-1;
	int32 addr; 	
	int16 i,rv=-1;
  
	/*Se Determina la cantidad de Poses en EEPROM*/	
	Num_Poses = Get_Num_Poses();
	
	if (Num_Poses >=1)
	{	
		id_vector = declarar(sizeof(uint16)*(Num_Poses+1));
		id_vector[0]=Num_Poses+1;
		
		for (i=0;i<Num_Poses;i++)
		{
			
			/*Direccion de Comienzo de cada Pose*/
			addr= (int32) ((i)*sizeof(PoseRobot)+sizeof(int16)); 
			
			/*Pasar al Segundo Banco de Memoria */
			if(i > (MAX_POSES/2))
				addr=addr+ ADD_NUM;
		 
			rv=ee24xx_read_bytes(addr,sizeof(uint16),(uint8*) &id_actual);			
			
			if(rv)
			{
				id_vector[i+1]=id_actual;
				
				#ifdef DEBUGTWI
					printf("return_id_vector=> id_vector[%d] = %d \n",i+1,id_vector[i+1]);
				#endif	
			}
			else
				return rv; 								/* Error en la Lectura*/
		}
	}
	else
	{
		id_vector = declarar(sizeof(uint16)*(2));
		id_vector[0]=2;
		id_vector[1]=-1;
		return 0;										/* No hay Poses*/
	}

	return 1;	
}

/*****************************************************************************************************/
int16 BorrarEEPROM(void)
{
	uint16 Num_Poses;
	int16 rv;
   
	Num_Poses=0;
	
	/* actualizo Num_Poses */
	rv=ee24xx_write_bytes(0,sizeof(uint16),(uint8 *)&Num_Poses);	
	#ifdef DEBUGTWI
		printf("BorrarEEPROM=> Numero de BYTES grabados corectamente en BorrarEErom => %d\n",rv);
		rv=ee24xx_read_bytes(0,sizeof(uint16),(uint8 *)&Num_Poses);
		printf("BorrarEEPROM=> Numero de BYTES ledos corectamente en BorrarEErom => %d\n",rv);
		printf("BorrarEEPROM=> Num_Poses = %x\n",Num_Poses);
   #endif

return rv;
}
/*****************************************************************************************************/
/*-----------------------------------------------------------------------------------------*/
/*No podemos Usa rxTrajectory */
/*Crea un trayectory utilizando las posiciones del Vector de Ids*/
int16  Vector2Trajectory(uint16 *vector)
{	
	int16 num_datos,i,rv, axis;
	uint16 id_pose;
	PoseRobot Pose;	
	extern int16 FlagrxVectorok;
	
	num_datos=vector[0];								//numero de datos del vector
	if(FlagrxVectorok)	
		free(Trajectoryvector.point);
		
	Trajectoryvector.point= declarar(sizeof(tpoint)*num_datos);//asigno memoria a los puntos
	
	
	//lleno los campos de la trayectoria como size e invalid mask
	Trajectoryvector.size=	num_datos-1;
	Trajectoryvector.invalidmask= (WaistMask |ShoulderMask | ElbowMask | PitchMask| RollMask);
	
	printf("Vector2Trajectory=> Trajectoryvector.size= %d\n",Trajectoryvector.size);
	printf("Vector2Trajectory=> Trajectoryvector.invalidmask= = %d\n",Trajectoryvector.invalidmask);
	for (i=1;i<num_datos;i++)
	{
		id_pose= vector[i];								//Obtengo el Id de la Pose		
		rv=	Leer_PoseTWI(id_pose, &Pose);				//obtener la Pose del Robot almacenada en EEPROM
		#ifdef DEBUGUSBLevel1
		printf("Vector2Trajectory=> Pose.Pos_Wrist = %ld\n",Pose.Pos_Wrist);
		printf("Vector2Trajectory=> Pose.Pos_Sholder = %ld\n",Pose.Pos_Shoulder);
		printf("Vector2Trajectory=> Pose.Pos_Elbow = %ld\n",Pose.Pos_Elbow);
		printf("Vector2Trajectory=> Pose.Pos_Pitch = %ld\n",Pose.Pos_Pitch);
		printf("Vector2Trajectory=> Pose.Pos_Roll = %ld\n",Pose.Pos_Roll);
		printf("Vector2Trajectory=> Pose.id = %d\n",Pose.id);
		#endif
		if(rv>0)
		{
			//LLeno el Campo de Posicion de la Trayectoria
			Trajectoryvector.point[(i-1)].axis[AxisWaist].position=Pose.Pos_Waist;
			Trajectoryvector.point[(i-1)].axis[AxisShoulder].position=Pose.Pos_Shoulder;
			Trajectoryvector.point[(i-1)].axis[AxisElbow].position=Pose.Pos_Elbow;
			Trajectoryvector.point[(i-1)].axis[AxisPitch].position=Pose.Pos_Pitch;
			Trajectoryvector.point[(i-1)].axis[AxisRoll].position=Pose.Pos_Roll;
			Trajectoryvector.point[(i-1)].Gripper_Status=Pose.Gripper_Status;
			
			#ifdef DEBUGUSBLevel1
			printf("Vector2Trajectory=> Ha sido Llenado la estructura  Trajectoryvector\n");
			printf("Vector2Trajectory=> La posicion de %s es en packetIO.c => %ld\n",AxisName(AxisWaist),
				   Trajectoryvector.point[(i-1)].axis[AxisWaist].position);
			printf("Vector2Trajectory=> La posicion de %s es en packetIO.c => %ld\n",AxisName(AxisShoulder),
				   Trajectoryvector.point[(i-1)].axis[AxisShoulder].position);
			printf("Vector2Trajectory=> La posicion de %s es en packetIO.c => %ld\n",AxisName(AxisElbow),
				   Trajectoryvector.point[(i-1)].axis[AxisElbow].position);
			printf("Vector2Trajectory=> La posicion de %s es en packetIO.c => %ld\n",AxisName(AxisPitch),
				   Trajectoryvector.point[(i-1)].axis[AxisPitch].position);
			printf("Vector2Trajectory=> La posicion de %s es en packetIO.c => %ld\n",AxisName(AxisRoll),
				   Trajectoryvector->point[(i-1)].axis[AxisRoll].position);
			
			#endif
			//LLeno los demas Campos de la Trayectoria. Velocidad y Aceleracion maximos
			for(axis=0; axis < DOF; axis++) 
			{
				Trajectoryvector.point[(i-1)].axis[axis].velocity=hAxis[axis].axisSettings.homeVelocity;
				Trajectoryvector.point[(i-1)].axis[axis].acceleration=hAxis[axis].axisSettings.homeAcceleration;
			}	
		}
		else
			return -1;
		
	}	
	
	
	/*
	for (i=1;i<num_datos;i++)
	{
	for(axis=0; axis < DOF; axis++) 
		{
			switch (axis)
			{
				//Velocidades tomadas de homeVelocity, no se modifico tAxisSettings
				//para evitar problemas con la memoria.
				
				case (AxisWaist):
					Trajectoryvector.point[(i-1)].axis[AxisWaist].velocity=//66834;
					hAxis[AxisWaist].axisSettings.homeVelocity;
					Trajectoryvector.point[(i-1)].axis[AxisWaist].acceleration=//40;
					hAxis[AxisWaist].axisSettings.homeAcceleration;
				break;
				case (AxisShoulder):
					Trajectoryvector.point[(i-1)].axis[AxisShoulder].velocity=//841238;
					hAxis[AxisShoulder].axisSettings.homeVelocity;
					Trajectoryvector.point[(i-1)].axis[AxisShoulder].acceleration=//202;
					hAxis[AxisShoulder].axisSettings.homeAcceleration;
				break;
				case (AxisElbow):
					Trajectoryvector.point[(i-1)].axis[AxisElbow].velocity=//838511;
					hAxis[AxisElbow].axisSettings.homeVelocity;
					Trajectoryvector.point[(i-1)].axis[AxisElbow].acceleration=//202;
					hAxis[AxisElbow].axisSettings.homeAcceleration;
				break;
				case (AxisPitch):
					Trajectoryvector.point[(i-1)].axis[AxisPitch].velocity=//108158;
					hAxis[AxisPitch].axisSettings.homeVelocity;
					Trajectoryvector.point[(i-1)].axis[AxisPitch].acceleration=//49;
					hAxis[AxisPitch].axisSettings.homeAcceleration;
				break;
				case (AxisRoll):
					Trajectoryvector.point[(i-1)].axis[AxisRoll].velocity=//111316;
					hAxis[AxisRoll].axisSettings.homeVelocity;
					Trajectoryvector.point[(i-1)].axis[AxisRoll].acceleration=//33;
					hAxis[AxisRoll].axisSettings.homeAcceleration;
				break;
			}
		}
	}*/
	return 0;
}

/*****************************************************************************************************/
/*Llena una trayectoria de un punto, para luego poder ejecutarla con teach_pendant_point() */
void Pose2rxTrayectoria(PoseRobot* EEpromPose)
{

	
	if(FlagTrayecPuntook)
		free(Trajectoryvectorpunto.point);
	
	Trajectoryvectorpunto.point=(tpoint*)declarar(sizeof(tpoint));
	Trajectoryvectorpunto.size=1;
	/* se habilitan todos los ejes para el movimiento*/
	Trajectoryvectorpunto.invalidmask=0x1F; 
	
	/*Velocidades tomadas de homeVelocity, no se modifico tAxisSettings
		para evitar problemas con la memoria.*/
	Trajectoryvectorpunto.point[0].axis[AxisWaist].position=EEpromPose->Pos_Waist;	
	Trajectoryvectorpunto.point[0].axis[AxisWaist].velocity=//155945;//70°/seg;
	hAxis[AxisWaist].axisSettings.homeVelocity;
	Trajectoryvectorpunto.point[0].axis[AxisWaist].acceleration=//80;//60
	hAxis[AxisWaist].axisSettings.homeAcceleration;
	
	Trajectoryvectorpunto.point[0].axis[AxisShoulder].position=EEpromPose->Pos_Shoulder;
	Trajectoryvectorpunto.point[0].axis[AxisShoulder].velocity=//1151798; //7°/seg
	hAxis[AxisShoulder].axisSettings.homeVelocity;
	Trajectoryvectorpunto.point[0].axis[AxisShoulder].acceleration=//395; //4
	hAxis[AxisShoulder].axisSettings.homeAcceleration;
	
	Trajectoryvectorpunto.point[0].axis[AxisElbow].position=EEpromPose->Pos_Elbow;
	Trajectoryvectorpunto.point[0].axis[AxisElbow].velocity=//1151798;
	hAxis[AxisElbow].axisSettings.homeVelocity;
	Trajectoryvectorpunto.point[0].axis[AxisElbow].acceleration=//395;
	hAxis[AxisElbow].axisSettings.homeAcceleration;
	
	Trajectoryvectorpunto.point[0].axis[AxisPitch].position=EEpromPose->Pos_Pitch;
	Trajectoryvectorpunto.point[0].axis[AxisPitch].velocity=//108158;
	hAxis[AxisWaist].axisSettings.homeVelocity;
	Trajectoryvectorpunto.point[0].axis[AxisPitch].acceleration=//49;
	hAxis[AxisWaist].axisSettings.homeAcceleration;
	
	Trajectoryvectorpunto.point[0].axis[AxisRoll].position=EEpromPose->Pos_Roll;
	Trajectoryvectorpunto.point[0].axis[AxisRoll].velocity=//111316;   // 150000
	hAxis[AxisRoll].axisSettings.homeVelocity;
	Trajectoryvectorpunto.point[0].axis[AxisRoll].acceleration=//33;  // 100
	hAxis[AxisRoll].axisSettings.homeAcceleration;
	
	FlagTrayecPuntook=1;
	
}

