#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "c-motion.h"
#include "PMDsetup.h"
#include "PMDinterrupt.h"
#include "packetIO.h"
#include "messages.h"
#include "debug.h"
#include "conversion.h"
#include "PMDinfo.h"
#include "PMDrobotics.h"
#include "gripper.h"


//Ajustes en grados para cada articulacion
#define	AJUSTE_CINTURA_HOME_POSI	0.5 //Segundo movimiento OK
#define	AJUSTE_CINTURA_HOME_NEGA	-0.6//-1.3 //primer movimiento (demasiado hacia el bano)
#define	AJUSTE_HOMBRO_HOME_NEGA	    1.8 
#define	AJUSTE_CODO_HOME_POSI	    -2.5 
#define	AJUSTE_PITCH_HOME_POSI	    -15.5
#define	AJUSTE_ROLL_HOME_POSI	    -8
#define	AJUSTE_ROLL_HOME_POSI_TIME	-7
#define	AJUSTE_ROLL_HOME_NEGA	    20



//#define DEBUGROBOTICS
#define DEBUGROBOTICS_HOME

extern PMDAxisHandle hAxis[DOF]; 


//Variables en EEPROM en donde se almacenan los movimientos de Pitch y Roll
PMDint32 EEMEM Mov_Roll_Anterior=0;
PMDint32 EEMEM Mov_Pitch_Anterior=0;
//Bandera que se activa si se esta en el processo de home del robot.
int HomePitch_process_active=0;
int HomeRoll_process_active=0;

 

//*********************************************************************************************/
void mov_axis(tAxis Axis,PMDint32 position, PMDint32 velocity, 
					PMDuint32 acceleration, PMDint16 actualizar)
{ 
	#ifdef DEBUGROBOTICS
	PMDint32 posicion_origen;
	#endif
	
	if(position!=none)
	{
		#ifdef DEBUGROBOTICS
		PMDGetCommandedPosition(&hAxis[Axis],&posicion_origen);
		printf("mov_axis=> registro de posicion inicial en el motor %s =  %ld\n ",AxisName(Axis), posicion_origen);
		printf("mov_axis=> registro de posicion final en el motor %s =  %ld\n ",AxisName(Axis), position);
		#endif
		PMDSetPosition(&hAxis[Axis],position);
	}
	if(velocity!=none)
	{
		#ifdef DEBUGROBOTICS
		printf("mov_axis=> registro de velocidad en el motor %s =  %ld\n ",AxisName(Axis), velocity);
		#endif
		PMDSetVelocity(&hAxis[Axis],velocity); //limite permitido 1310720 unidades en up/Sampletime	
	}
	if(acceleration!=none)
	{
		#ifdef DEBUGROBOTICS
		printf("mov_axis=> registro de aceleracion en el motor %s =  %ld\n ",AxisName(Axis), acceleration);
		#endif
		PMDSetAcceleration(&hAxis[Axis],acceleration); //limite permitido 786 uP/Sampleª	
	}
	if(actualizar==update)
	{
		PMDUpdate(&hAxis[Axis]);
	}
	else
	{
		#ifdef DEBUGROBOTICS
		printf("mov_axis=> No Actualizando..\n ");
		#endif
	}
}

void inipitch_roll(void)
{
	PMDint32 position=0;
	
	eeprom_write_block((const void*)&position,(void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
	eeprom_write_block((const void*)&position,(void*)&Mov_Roll_Anterior,sizeof(PMDint32));
}


//**********************************************************************************************
void pitch(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration,
					PMDint16 actualizar,PMDint16 modo_movimiento)
{
   
    PMDint32 Mov_Pitch_Anterior_RAM,temp;
	
	#ifdef DEBUGROBOTICS
	PMDint32 posicion_actual_motor_pitch, 
	posicion_actual_motor_roll;
	#endif
   
   /*habilitando eventos de limite para realizar movimientos de PiTch*/
	PMDSetLimitSwitchMode(&hAxis[AxisRoll],PMDLimitEnabled);
	PMDSetLimitSwitchMode(&hAxis[AxisPitch],PMDLimitEnabled);
	
	/*Realizo el Movimiento en el motor de Pitch */	
	#ifdef DEBUGROBOTICS
	PMDGetActualPosition(&hAxis[AxisPitch],&posicion_actual_motor_pitch);
	PMDGetActualPosition(&hAxis[AxisRoll],&posicion_actual_motor_roll);
	printf("pitch=> Posicion encoder actual del motor de pitch %ld\n",posicion_actual_motor_pitch);
	printf("pitch=> Posicion encoder actual del motor de roll %ld\n",posicion_actual_motor_roll);
	#endif
	
	switch (modo_movimiento)
	{ 
		case (MOVIMIENTO_NORMAL):
			if(position!=none)
			{
				//Leer el movimiento anterior de Pitch que esta almacenado en EEPROM
				eeprom_read_block((void*)&Mov_Pitch_Anterior_RAM,(const void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
			//	#ifdef DEBUGROBOTICS
				printf("pitch=> Movimiento Anterior de pitch EEPROM de %s = %ld\n",AxisName(AxisPitch),Mov_Pitch_Anterior_RAM);
			//	#endif
				//Es un movimiento normal se debe de actualizar la eeprom.
			//#ifdef DEBUGROBOTICS
				printf("pitch=> Actualizando ActualPosition motor de  PITCH y ROLL en pitch();! \n");
			//	#endif
				PMDSetActualPosition(&hAxis[AxisPitch],Mov_Pitch_Anterior_RAM);
				PMDSetActualPosition(&hAxis[AxisRoll],-(Mov_Pitch_Anterior_RAM));
				//Actualizo el movimiento anterior con el actual, para el proximo movimiento.
				eeprom_write_block((const void*)&position,(void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
				#ifdef DEBUGROBOTICS
				PMDGetActualPosition(&hAxis[AxisPitch],&posicion_actual_motor_pitch);
				PMDGetActualPosition(&hAxis[AxisRoll],&posicion_actual_motor_roll);
				printf("pitch=> Posicion encoder despues de actualizar del motor de pitch %ld\n",posicion_actual_motor_pitch);
				printf("pitch=> Posicion encoder despues de actualizar del motor de roll %ld\n",posicion_actual_motor_roll);
				#endif
			}
	    break;	
	    case (MOVIMIENTO_HACIA_HOME):
			//Me estoy moviendo hacia home por primera vez, no necesito ejecutar PMDSetActualPosition
			//ni tampoco actualizar la eeprom ya que ha sido inicializada en INICIANDO_HOME_PITCH_ROLL.
			//No tengo que hacer nada.
	    break;	
	    case (EN_CALIBRACION_PROCESOHOME):
			//Estoy buscando home o en algun otro proceso
			//No tengo que hacer nada
	    break;
	    case (INICIANDO_HOME_PITCH_ROLL):
			//Estoy iniciando la busqueda de home para pitch o roll
			//Debo reinicar el movimiento anterior a cero. Para evitar descuadre del encoder cuando: 
			//-Hago HOME luego me muevo hacia una posicion X y luego hago home.
			//-Apago el controlador y se mueve por error los motores de pitch y roll.
			#ifdef DEBUGROBOTICS
				printf("pitch=> Iniciando variable Mov_Pitch_Anterior a Cero.! \n");
			#endif
			temp=0;
			eeprom_write_block((const void*)&temp,(void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
			PMDSetActualPosition(&hAxis[AxisPitch],0);
			PMDSetActualPosition(&hAxis[AxisRoll],0);
	    break;	 
	}
	
	
	/*Realizo el Movimiento en el motor de Pitch */	
	
	
	
	mov_axis(AxisPitch, position, velocity, acceleration, actualizar);
	/*none es utilizado cuando se utiliza un perfil de velocidad constante.*/
	if (position == none)
	{
		
		/*Realizo el Movimiento en el motor de roll */	
		mov_axis(AxisRoll, position, -(velocity), acceleration, actualizar);
	}
	else
	{
		/*Realizo el Movimiento en el motor de roll */	
		mov_axis(AxisRoll, -position, (velocity), acceleration, actualizar);
		
		
	}
	
	
}




//**********************************************************************************************
void roll(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration,
					PMDint16 actualizar,PMDint16 modo_movimiento)
{
    PMDint32 Mov_Roll_Anterior_RAM, temp;
	#ifdef DEBUGROBOTICS
	PMDint32  posicion_actual_motor_pitch,posicion_actual_motor_roll;
	#endif
	
	/*deshabilitando eventos de limite porque Roll no tiene interruptores 
	de fin de carrera*/
	PMDSetLimitSwitchMode(&hAxis[AxisRoll],PMDLimitDisabled);
	PMDSetLimitSwitchMode(&hAxis[AxisPitch],PMDLimitDisabled);
	
	
	#ifdef DEBUGROBOTICS
	PMDGetActualPosition(&hAxis[AxisPitch],&posicion_actual_motor_pitch);
	PMDGetActualPosition(&hAxis[AxisRoll],&posicion_actual_motor_roll);
	printf("roll=> Posicion encoder actual del motor de pitch %ld\n",posicion_actual_motor_pitch);
	printf("roll=> Posicion encoder actual del motor de roll %ld\n",posicion_actual_motor_roll);
	#endif
	
	switch (modo_movimiento)
	{ 
		case (MOVIMIENTO_NORMAL):
			/* Leer el movimiento anterior de Roll que esta almacenado en EEPROM*/
			eeprom_read_block((void*)&Mov_Roll_Anterior_RAM,(const void*)&Mov_Roll_Anterior,sizeof(PMDint32));
			//#ifdef DEBUGROBOTICS
				printf("roll => Movimiento de roll Anterior EEPROM de %s = %ld\n",AxisName(AxisRoll),Mov_Roll_Anterior_RAM);
				printf("roll => Actualizando ActualPosition motor de  PITCH y ROLL en roll();! \n");
			//#endif	
			/* Carga el valor de movimiento anterior de Roll a los encoders de los motores de pitch y roll*/
			PMDSetActualPosition(&hAxis[AxisPitch],Mov_Roll_Anterior_RAM);
			PMDSetActualPosition(&hAxis[AxisRoll],Mov_Roll_Anterior_RAM);
			//Es un movimiento normal se debe de actualizar la posicion eeprom.
			eeprom_write_block((const void*)&position,(void*)&Mov_Roll_Anterior,sizeof(PMDint32));
	    break;	
	    case (MOVIMIENTO_HACIA_HOME):
			//Me estoy moviendo hacia home por primera vez, no necesito ejecutar PMDSetActualPosition
			//ni tampoco actualizar la eeprom ya que ha sido inicializada en INICIANDO_HOME_PITCH_ROLL.
			//No tengo que hacer nada.
	    break;	
	    case (EN_CALIBRACION_PROCESOHOME):
			//Estoy buscando home o en algun otro proceso
			//No tengo que hacer nada
	    break;
	    case (INICIANDO_HOME_PITCH_ROLL):
			//Debo reinicar el movimiento anterior a cero. Para evitar descuadre del encoder cuando: 
			//-Hago HOME luego me muevo hacia una posicion X y luego hago home.
			//-Apago el controlador y se mueve por error el los motores de pitch y roll.
			temp=0;
			eeprom_write_block((const void*)&temp,(void*)&Mov_Roll_Anterior,sizeof(PMDint32));
			//reinicio encoders de pitch y roll en cero
			printf("roll => iniciando encodificadores a cero. \n");
			PMDSetActualPosition(&hAxis[AxisPitch],0);
			PMDSetActualPosition(&hAxis[AxisRoll],0);
	    break;	 
	}
	
	
	#ifdef DEBUGROBOTICS
	PMDGetActualPosition(&hAxis[AxisPitch],&posicion_actual_motor_pitch);
	PMDGetActualPosition(&hAxis[AxisRoll],&posicion_actual_motor_roll);
	printf("roll => Posicion encoder actual del motor de pitch %ld\n",posicion_actual_motor_pitch);
	printf("roll => Posicion encoder actual del motor de roll %ld\n",posicion_actual_motor_roll);
	#endif
	
		
	/*Realizo el Movimiento de Roll */	
	mov_axis(AxisPitch, position, velocity, acceleration, actualizar);
	mov_axis(AxisRoll, position, (velocity), acceleration, actualizar);


}




//**********************************************************************************************
//Solo carga los registros no efectua movimiento.
//Al mover el EJe1 se tiene que mover proporcionalmente el Eje2
void mov_correlacionado( tTrajectory   *TrajectoryPOINT, int16 pointIndex,
						tAxis Eje1, tAxis Eje2, double FACTOR_CORRELACION)
{
 
	PMDint32 position_Eje1;
	PMDuint32 Acceleration_Eje1,Velocity_Eje1;
	
	PMDint32 position_Eje2_de_Correlacion,position_Eje2;
	PMDuint32 Acceleration_Eje2,Velocity_Eje2;
	#ifdef DEBUGROBOTICS
	PMDint32 position_commanded_Eje2,position_commanded_Eje1;
	int temp;
	#endif
	
	
	
	//Leyendo datos de la posicion final para el eje 1
	Velocity_Eje1=TrajectoryPOINT->point[pointIndex].axis[Eje1].velocity;
	Acceleration_Eje1=TrajectoryPOINT->point[pointIndex].axis[Eje1].acceleration;
	position_Eje1 = TrajectoryPOINT->point[pointIndex].axis[Eje1].position;
	
	//Leyendo datos de la posicion final para el eje 2
	position_Eje2 = TrajectoryPOINT->point[pointIndex].axis[Eje2].position;
	Velocity_Eje2 = TrajectoryPOINT->point[pointIndex].axis[Eje2].velocity;
	Acceleration_Eje2 = TrajectoryPOINT->point[pointIndex].axis[Eje2].acceleration;
	
	/*La Velocidad del Eje2 es igual a la del Registro del Eje2 + la velocidad del Eje1 X factor de correlacion*/
	if(Velocity_Eje1 != 0)
	{
		Velocity_Eje2 = (Velocity_Eje1*FACTOR_CORRELACION);
		
	}
	
	if(Acceleration_Eje1 != 0)
	{
		Acceleration_Eje2 =  (Acceleration_Eje1*FACTOR_CORRELACION);
		
	}
	
	/*La Aceleracion del Eje2 es igual a la del Registro  del Eje 2 + la aceleracion del Eje1 X factor de correlacion*/
	
	
	//INTERACCION Eje1-Eje2
	
	/*La Posicion del Eje2 es igual al valor almacenado en el registro del Eje2 + la poscion
	del Eje1*FaCTOR DE CORRELACION*/
	position_Eje2_de_Correlacion = position_Eje2+(position_Eje1 * FACTOR_CORRELACION);	
	
	
	
	#ifdef DEBUGROBOTICS				
		/*Leo la posicion instantanea de Eje1*/
		PMDGetCommandedPosition(&hAxis[Eje1],&position_commanded_Eje1);
		/*Obtengo la posicion instantanea del Eje2 para fines de debug*/
		PMDGetCommandedPosition(&hAxis[Eje2],&position_commanded_Eje2);		
		/*Imprimo para debug*/ 	 
		printf("mov_correlacionado => Velocidad Eje %s = %ld\n",AxisName(Eje1),Velocity_Eje1);
		printf("mov_correlacionado => Posicion Instantanea Eje %s = %ld\n",AxisName(Eje1),position_commanded_Eje1);	
		
		printf("mov_correlacionado => Velocidad Eje %s = %ld\n",AxisName(Eje2),Velocity_Eje2);	
		printf("mov_correlacionado => Aceleracion Eje %s = %ld\n",AxisName(Eje2),Acceleration_Eje2);	
		printf("mov_correlacionado => position inicial de Eje %s = %ld\n",AxisName(Eje2),position_Eje2);
		printf("mov_correlacionado => position Instantanea Eje %s = %ld\n",AxisName(Eje2),position_commanded_Eje2);
		printf("mov_correlacionado => position Eje %s deseada(Formula)= %ld\n",AxisName(Eje2),position_Eje2_de_Correlacion);
	
	 #endif
	
	/*Muevo el Eje2 con esos datos */
	if (Eje2==AxisPitch)
	{	
		/*Almaceno la posicion final del pitch, es necesaria para 
                reconfiguracion del movimiento*/
                TrajectoryPOINT->point[pointIndex].axis[Eje2].position=position_Eje2_de_Correlacion;
		pitch(position_Eje2_de_Correlacion,Velocity_Eje2,Acceleration_Eje2,no_update,MOVIMIENTO_NORMAL );
	}
	else
	{
		mov_axis(Eje2,position_Eje2_de_Correlacion,Velocity_Eje2,Acceleration_Eje2,no_update);	
	}			
	
	
	
	
	if(Eje1==AxisShoulder)
	{
		#ifdef DEBUGROBOTICS				
		printf("mov_correlacionado =>  Actualizando valores de codo \n");
		#endif
		TrajectoryPOINT->point[pointIndex].axis[Eje2].position=position_Eje2_de_Correlacion;
		TrajectoryPOINT->point[pointIndex].axis[Eje2].velocity=Velocity_Eje2;
		TrajectoryPOINT->point[pointIndex].axis[Eje2].acceleration=Acceleration_Eje2;
	}
	
	#ifdef DEBUGROBOTICS				
	//scanf("%d",&temp);
	#endif
		
	
}



//Solo carga los registros no efectua movimiento.
//Al mover el EJe1 se tiene que mover proporcionalmente el Eje2
void mov_correlacionado_HOME(tAxis Eje2, PMDint32 Posicion,
										 PMDint32 Velocity_Eje1, PMDint32 *Velocity_Eje2,
						                 PMDuint32 Acceleration_Eje1, PMDuint32 *Acceleration_Eje2, 
										 double FACTOR_CORRELACION)
{

	
	
	/*La Velocidad del Eje2 es igual a la del Registro del Eje2 + la velocidad del Eje1 X factor de correlacion*/
	*Velocity_Eje2 = (Velocity_Eje1*FACTOR_CORRELACION);
	/*La Aceleracion del Eje2 es igual a la del Registro  del Eje 2 + la aceleracion del Eje1 X factor de correlacion*/
	*Acceleration_Eje2 =  (Acceleration_Eje1*FACTOR_CORRELACION);
	
	/*Imprimo para debug*/ 	 
	printf("mov_correlacionado_HOME=> Velocidad Eje %s = %ld\n",AxisName(Eje2),*Velocity_Eje2);	
	printf("mov_correlacionado_HOME=> Aceleracion Eje %s = %ld\n",AxisName(Eje2),*Acceleration_Eje2);	
	
	
	/*Muevo el Eje2 con esos datos */
	if (Eje2==AxisPitch)
	{		
		pitch(Posicion,*Velocity_Eje2,*Acceleration_Eje2,no_update,EN_CALIBRACION_PROCESOHOME);
	}
	else
	{
		mov_axis(Eje2,Posicion,*Velocity_Eje2,*Acceleration_Eje2,no_update);	
	}			
		
}



//*******************************************************************************************/
void sofstop(void) 
{
	int16 axis;
	
	for(axis=0; axis < DOF; axis++)
	{
		PMDSetStopMode(&hAxis[axis],PMDSmoothStopMode);
	}
		
	for(axis=0; axis < DOF; axis++) 
	{ // for every chipset (all axis with axis number=1), call MultiUpdate once
		if(hAxis[axis].axis == 0) 
		{
			// Stop the robot (all axis in parallel)
			PMDMultiUpdate(&hAxis[axis],PMDAxis1Mask|PMDAxis2Mask|PMDAxis3Mask|PMDAxis4Mask);    
		} 
	}
}
//*******************************************************************************************/
void abruptstop(void) 
{
	int16 axis;
	
	for(axis=0; axis < DOF; axis++)
	{
		PMDSetStopMode(&hAxis[axis],PMDAbruptStopMode);
	}
		
	for(axis=0; axis < DOF; axis++) 
	{ // for every chipset (all axis with axis number=1), call MultiUpdate once
		if(hAxis[axis].axis == 0) 
		{
			// Stop the robot (all axis in parallel)
			PMDMultiUpdate(&hAxis[axis],PMDAxis1Mask|PMDAxis2Mask|PMDAxis3Mask|PMDAxis4Mask);    
		} 
	}

}

/*********************************************************************************************/

PMDresult PMDcheckIdle(int16 mask)
{
		
	PMDuint16 status;
	int16 axis;
	
  
	for(axis=0; axis < DOF; axis++) 
	{
		if((hAxis[axis].axisSettings.axisEnabled == TRUE)&&
		  ((mask & (Axis2AxisMask(axis))) >0)) 
		{		
			PMDGetEventStatus(&hAxis[axis],&status);
			
			
			if ((status & PMDEventMotionCompleteMask) == 0)
			{//Este eje está en movimiento			 
				//#ifdef DEBUGROBOTICS
				//printf("PMDcheckIdle=> FALSE eje %s en movimiento\n",AxisName(axis));
				//#endif			
				return FALSE;			
			}
		}
	}
  return TRUE;
} // PMDcheckIdle

/**********************************************************************************************************************/
void motioncomplete(int16 axis)
{
	
	//Habilitar todos los ejes para el proximo movimiento limpiando  el bit
	//de Motion Complete ( Bit 0 del Event Status Register).
	
		if((hAxis[axis].axisSettings.axisEnabled == TRUE))
			PMDResetEventStatus(&hAxis[axis],~ PMDEventMotionCompleteMask);	
		
}

/**********************************************************************************************************************/
void Enable_Mov(void)
{
	int16 axis;
	PMDuint16 status;

	for(axis=0; axis < DOF; axis++) 
	{
		if(hAxis[axis].axisSettings.axisEnabled == TRUE)
		{		
			PMDGetEventStatus(&hAxis[axis],&status);
			if ((status & PMDEventMotionCompleteMask) > 0) //Eje detenido, movimiento completado 
			{
				motioncomplete(axis);
				
			}
		}
	}
	
}

//**********************************************************************************************
void PMDprocessTrajectory(tTrajectory* Trajectory, int16 TraceOP)
{
	int16 pointIndex=0;
	int16 mask;

	
	
    mask= Trajectory->invalidmask;
	
	#ifdef DEBUGROBOTICS
	printf("PMDprocessTrajectory=> El Numero de puntos de la trayectoria es en PMDfunctions.c => %d\n",Trajectory->size);
	printf("PMDprocessTrajectory=> invalid mask en PMDfunctions.c => %X\n",Trajectory->invalidmask);
	#endif
	do
	{  
		printf("PMDprocessTrajectory=> Procesado el  punto %d \n",pointIndex);
		#ifdef DEBUGROBOTICS
		printf("PMDprocessTrajectory=> La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisWaist),
				Trajectory->point[pointIndex].axis[AxisWaist].position);
		printf("PMDprocessTrajectory=> La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisShoulder),
				Trajectory->point[pointIndex].axis[AxisShoulder].position);
		printf("PMDprocessTrajectory=> La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisElbow),
				Trajectory->point[pointIndex].axis[AxisElbow].position);
		printf("PMDprocessTrajectory=> La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisPitch),
				Trajectory->point[pointIndex].axis[AxisPitch].position);
		printf("PMDprocessTrajectory=> La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisRoll),
				Trajectory->point[pointIndex].axis[AxisRoll].position);
		#endif
		//Cargo la posicion, velocidad, aceleracion con pointIndex tomando encuenta 
		//La interaccion de los ejes, espera a que termine el movimiento de cintura, 
		//hombro, codo y pich para finalmente  ejecutar roll
		teach_pendant_point(Trajectory,pointIndex);
		/*Ejecuto movimiento del gripper*/
		if(Trajectory->point[pointIndex].Gripper_Status==1)
		{
			closeGripper();
			#ifdef DEBUGROBOTICS
			printf("PMDprocessTrajectory=> Pose con gripper cerrado\n");	
			#endif
		}
		if(Trajectory->point[pointIndex].Gripper_Status==0)
		{
			openGripper();
			#ifdef DEBUGROBOTICS
			printf("PMDprocessTrajectory=> Pose con gripper abierto\n");	
			#endif
		}
		pointIndex++;				
	}
	while (pointIndex <  (Trajectory->size)); //pointIndex es la variable que marca el punto en ejecucion 
    printf("PMDprocessTrajectory=>  Saliendo de PMDprocessTrajectory\n");
	//He configurado trazar una trajectoria
	if(TraceOP==1)
	{
		//Parao el trazado para PMD1
		PMDSetTraceStop(&hAxis[AxisWaist],AxisWaist,PMDTraceConditionImmediate,0,0);
		//PMDSetTraceStop(&hAxis[axis], axis,PMDTraceConditionImmediate,0,0);
	}

	
} 



/**********************************************************************************************************************/
void search_home_negative(int16 Axis,PMDint32 *home,PMDint32 *posicion,PMDint16 rangos)
{
	PMDuint16 status,find_home_out;
	PMDint32 temp;
	PMDint32 Velocidad_eje2_temp,Velocidad_final_pitch;
	PMDuint32 Aceleracion_eje2_temp, Aceleracion_final_pitch;
	int Axis_Update;
	#ifdef DEBUGROBOTICS_HOME
	PMDuint16 test;
	#endif
	
	find_home_out=1;
	

	
	#ifdef DEBUGROBOTICS_HOME
	PMDGetHostIOError(&hAxis[Axis], &test);
	printf("search_home_negative=> HostIOError = %x para el eje %s\n ",test,AxisName(Axis));
	#endif
	
	No_limit_switch(Axis);
	//Limpia el BiT de Position Capture Indicator en el Status Register
	PMDResetEventStatus(&hAxis[Axis],~PMDEventCaptureReceivedMask);	
	//La senal de HOME sera utilizada como Disparo para la Captura de la Posicion
	PMDSetCaptureSource(&hAxis[Axis],PMDCaptureSourceHome);
	//Leyendo el registro de Captura de Posicion Causa "rearmar" 
	//el disparador con el fin de permitir mas capturas.
	PMDGetCaptureValue(&hAxis[Axis],&temp); 
	
	//#ifdef DEBUGROBOTICS_HOME
	printf("search_home_negative=> Ejecutado No_limit_switch\n");
	printf("search_home_negative=> Busqueda home en dir neg. del eje %s.\n ",AxisName(Axis));
	//#endif

	
	switch (Axis)
	{
		
		case (AxisShoulder):
		{
			printf("caso AxisShoulder\n");
			//No codo depende de nadie va directo...  
			//(Siempre entra aqui cuando se usa con una trayectoria )..
			//NO OLVIDAR CONFIGURAR LOS DEMAS COMO VELOCIDAD CONTINUA 
			mov_axis(Axis,-INF, 
			hAxis[Axis].axisSettings.homeVelocity, 
			hAxis[Axis].axisSettings.homeAcceleration,
			no_update);
			/*mov_axis(AxisElbow,-INF, 
			hAxis[Axis].axisSettings.homeVelocity*CORRELACION_HOMBRO_CODO, 
			hAxis[Axis].axisSettings.homeAcceleration*CORRELACION_HOMBRO_CODO
			,no_update);
			pitch(-INF,
			hAxis[Axis].axisSettings.homeVelocity*CORRELACION_HOMBRO_CODO*CORRELACION_CODO_PITCH,
			100,
			no_update,EN_CALIBRACION_PROCESOHOME );
			*/
			
			//Configurando los registros de codo, codo esta invertido invertido.
			mov_correlacionado_HOME(AxisElbow,-INF,
												hAxis[Axis].axisSettings.homeVelocity,
												&Velocidad_eje2_temp,hAxis[Axis].axisSettings.homeAcceleration,
												&Aceleracion_eje2_temp,CORRELACION_HOMBRO_CODO);		
			//Configurando los registros de pitch, el pitch esta en el mismo sentido que el codo.
			mov_correlacionado_HOME(AxisPitch,-INF,
												Velocidad_eje2_temp,
												&Velocidad_final_pitch,Aceleracion_eje2_temp,
												&Aceleracion_final_pitch,
												CORRELACION_CODO_PITCH);
			
			printf("search_home_negative=> Update AxisShoulder \n ");
			PMDUpdate(&hAxis[AxisShoulder]);
			printf("search_home_negative=> Update AxisElbow \n ");
			PMDUpdate(&hAxis[AxisElbow]);	
			printf("search_home_negative=> Update AxisPitch \n ");
			PMDUpdate(&hAxis[AxisPitch]);	
			printf("search_home_negative=> Update AxisRoll \n ");
			PMDUpdate(&hAxis[AxisRoll]);
		}
		break;
		case (AxisElbow):
		{
			mov_axis(Axis,-INF, 
			hAxis[Axis].axisSettings.homeVelocity, 
			hAxis[Axis].axisSettings.homeAcceleration,
			no_update);
			
			/*pitch(-INF,
			hAxis[Axis].axisSettings.homeVelocity*0.0166,
			300,
			no_update,EN_CALIBRACION_PROCESOHOME );
			
			*/
			
			mov_correlacionado_HOME(AxisPitch,-INF,
												hAxis[Axis].axisSettings.homeVelocity ,
												&Velocidad_final_pitch,hAxis[Axis].axisSettings.homeAcceleration,
												&Aceleracion_final_pitch,
												CORRELACION_CODO_PITCH);	
			printf("search_home_negative=> Update AxisElbow \n ");
			PMDUpdate(&hAxis[AxisElbow]);
			printf("search_home_negative=> Update AxisPitch \n ");
			PMDUpdate(&hAxis[AxisPitch]);	
			printf("search_home_negative=> Update AxisRoll \n ");
			PMDUpdate(&hAxis[AxisRoll]);
		}
		break;					
		case (AxisPitch):
		{
			pitch(-INF, hAxis[Axis].axisSettings.homeVelocity,
			hAxis[Axis].axisSettings.homeAcceleration,update,INICIANDO_HOME_PITCH_ROLL );
			
			
			#ifdef DEBUGROBOTICS_HOME
			PMDGetHostIOError(&hAxis[AxisRoll], &test);
			printf("search_home_negative=> HostIOError = %x del eje ROLL despues del update.\n ",test);
			#endif
		}
	    break;	
		case (AxisRoll):		
		{
			roll(-INF, hAxis[Axis].axisSettings.homeVelocity,
			hAxis[Axis].axisSettings.homeAcceleration,update,INICIANDO_HOME_PITCH_ROLL );
			
			
			#ifdef DEBUGROBOTICS_HOME
			PMDGetHostIOError(&hAxis[AxisPitch], &test);
			printf("search_home_negative=> HostIOError = %x del eje PITCH despues del update.\n ",test);
			#endif
		}
	    break;
		//Solo cintura
		default:
			mov_axis(Axis,-INF,hAxis[Axis].axisSettings.homeVelocity,
			hAxis[Axis].axisSettings.homeAcceleration,update);
	    break;
	}
	
	
	#ifdef DEBUGROBOTICS_HOME
	PMDGetHostIOError(&hAxis[Axis], &test);
	printf("search_home_negative=> HostIOError = %x del eje %s despues del Update.\n ",test,AxisName(Axis));
	printf("search_home_negative=> Iniciando busqueda de home en el sentido negativo.\n");	
	#endif

	do 
	{ 
		PMDGetEventStatus(&hAxis[Axis],&status);            
		
		if((status & PMDEventCaptureReceivedMask) != 0) 
		{   
			#ifdef DEBUGROBOTICS_HOME
			printf("search_home_negative=>  Captura de HOME realizada. \n ");			
			#endif
			PMDGetCaptureValue(&hAxis[Axis],home);
			PMDResetEventStatus(&hAxis[Axis],~PMDEventCaptureReceivedMask);
			
			if(rangos==FALSE) //se Hace Caputura de HOME y se quiere detener el motor
			{	
				
				//se Cambia la variable a 0 para salir del ciclo.
				find_home_out=0;
				//detener suavemente el movimiento	
				switch (Axis)
				{
				    case (AxisWaist):
						PMDSetStopMode(&hAxis[Axis], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[Axis]);
				    break;
					case (AxisRoll):
						PMDSetStopMode(&hAxis[Axis], PMDAbruptStopMode ); 
						PMDSetStopMode(&hAxis[AxisPitch], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[Axis]);
						PMDUpdate(&hAxis[AxisPitch]);
						
					case (AxisPitch):
						PMDSetStopMode(&hAxis[AxisRoll], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[AxisRoll]);
				    break;
				    default:
				    	for(Axis_Update=Axis+1; Axis_Update < DOF; Axis_Update++) 
						{ 
							PMDSetStopMode(&hAxis[Axis_Update], PMDAbruptStopMode ); 
							PMDUpdate(&hAxis[Axis_Update]);
						}
				    break;
				}
				
			}
			
		}			
		#ifdef DEBUGROBOTICS_HOME
		//printf(" Moviendo en direccion negativa. \n ");
			//printf("El Event status register del eje %s es %X \n",AxisName(Axis),status);			
		#endif
		
	}		
	while (((status & PMDEventInNegativeLimitMask) == 0) && (find_home_out==1));	
	
	
	#ifdef DEBUGROBOTICS_HOME
	PMDGetHostIOError(&hAxis[Axis], &test);
	printf("search_home_negative=> Error en el eje %s despues del movimiento %x\n ",AxisName(Axis),test);
	PMDGetEventStatus(&hAxis[Axis],&status);  
	printf("search_home_negative=> El Event status register antes de limpiar del eje %s es %X \n",AxisName(Axis),status);	
	#endif	
	

	
	/*Se limpia el Event Status Register, borrando las interrupciones generadas: Evento de Limite
	Negativo y Fin de movimiento*/	
	Reset_Even_Status_Register();
	
	
	/* Se obtiene la posicion Actual del encoder*/
	PMDGetActualPosition(&hAxis[Axis],posicion);
	
	
	#ifdef DEBUGROBOTICS_HOME
	PMDGetEventStatus(&hAxis[Axis],&status);  
	printf("search_home_negative=> El Event status register del eje %s despues de limpiar es %X \n ",AxisName(Axis),status);	
	#endif	
	
	

}
void search_home_positive(int16 Axis,PMDint32 *home,PMDint32 *posicion,PMDint16 rangos)
{
	PMDuint16 status,find_home_out,test;
	PMDint32 temp;
	PMDint32 Velocidad_eje2_temp,Velocidad_final_pitch;
	PMDuint32 Aceleracion_eje2_temp, Aceleracion_final_pitch;
	int Axis_Update;

	find_home_out=1;
	
	

	#ifdef DEBUGROBOTICS_HOME
	PMDGetHostIOError(&hAxis[Axis], &test);
	printf("search_home_positive=> HostIOError = %x para el eje %s\n ",test,AxisName(Axis));
	#endif
	//Se pueden produccir multiples eventos de limites
	//No basta con limpiar la bandera
	// hay que mover el motor en la direccion contraria
	//Por ejemplo, situacion de error 
	//En search_home_positive estoy moviendo el hombro
	//Por casualidad el codo o pitch esta tocando el interuptor positivo
	//por lo tanto antes de mover nada hay que desactivarlos.
	No_limit_switch(Axis);
	

	PMDGetHostIOError(&hAxis[Axis], &test);
	//Limpia el BiT de Position Capture Indicator en el Status Register
	PMDResetEventStatus(&hAxis[Axis],~PMDEventCaptureReceivedMask);	
	//La senal de HOME sera utilizada como Disparo para la Captura de la Posicion
	PMDSetCaptureSource(&hAxis[Axis],PMDCaptureSourceHome);
	//Leyendo el registro de Captura de Posicion Causa "rearmar" 
	//el disparador con el fin de permitir mas capturas.
	PMDGetCaptureValue(&hAxis[Axis],&temp); 
	

	#ifdef DEBUGROBOTICS_HOME
	printf("search_home_positive=> Ejecutado No_limit_switch\n");
	printf("search_home_positive=> Busqueda home en dir Posi. del eje %s.\n ",AxisName(Axis));
	#endif

	switch (Axis)
	{
		case (AxisShoulder):
		{
			//No codo depende de nadie va directo...  
			//(Siempre entra aqui cuando se usa con una trayectoria )..
			mov_axis(Axis,INF, 
			hAxis[Axis].axisSettings.homeVelocity, 
			hAxis[Axis].axisSettings.homeAcceleration,
			no_update);
			/*
			mov_axis(AxisElbow,INF, 
			hAxis[Axis].axisSettings.homeVelocity*CORRELACION_HOMBRO_CODO, 
			hAxis[Axis].axisSettings.homeAcceleration*CORRELACION_HOMBRO_CODO
			,no_update);
			pitch(INF,
			hAxis[Axis].axisSettings.homeVelocity*CORRELACION_HOMBRO_CODO*CORRELACION_CODO_PITCH,
			100,
			no_update,EN_CALIBRACION_PROCESOHOME );*/
			//Configurando los registros de codo, codo esta invertido invertido.
			
			mov_correlacionado_HOME(AxisElbow,INF,
												hAxis[Axis].axisSettings.homeVelocity,
												&Velocidad_eje2_temp,hAxis[Axis].axisSettings.homeAcceleration,
												&Aceleracion_eje2_temp,CORRELACION_HOMBRO_CODO);		
			//Configurando los registros de pitch, el pitch esta en el mismo sentido que el codo.
			mov_correlacionado_HOME(AxisPitch,INF,
												Velocidad_eje2_temp,
												&Velocidad_final_pitch,Aceleracion_eje2_temp,
												&Aceleracion_final_pitch,
												CORRELACION_CODO_PITCH);
			
	
			printf("search_home_positive=>  Update AxisShoulder \n ");
			PMDUpdate(&hAxis[AxisShoulder]);
			printf("search_home_positive=>  Update AxisElbow \n ");
			PMDUpdate(&hAxis[AxisElbow]);	
			printf("search_home_positive=>  Update AxisPitch \n ");
			PMDUpdate(&hAxis[AxisPitch]);	
			printf("search_home_positive=>  Update AxisRoll \n ");
			PMDUpdate(&hAxis[AxisRoll]);
		}	
		break;
		case (AxisElbow):
		{
			mov_axis(Axis,INF, 
			hAxis[Axis].axisSettings.homeVelocity, 
			hAxis[Axis].axisSettings.homeAcceleration,
			no_update);
			/*
			pitch(INF,
			hAxis[Axis].axisSettings.homeVelocity*0.0166,
			300,
			no_update,EN_CALIBRACION_PROCESOHOME );
			*/
			
			mov_correlacionado_HOME(AxisPitch,INF,
												hAxis[Axis].axisSettings.homeVelocity ,
												&Velocidad_final_pitch,hAxis[Axis].axisSettings.homeAcceleration,
												&Aceleracion_final_pitch,
												CORRELACION_CODO_PITCH);
			
	
			printf("search_home_positive=>  Update AxisElbow \n ");
			PMDUpdate(&hAxis[AxisElbow]);	
			printf("search_home_positive=>  Update AxisPitch \n ");
			PMDUpdate(&hAxis[AxisPitch]);
			printf("search_home_positive=>  Update AxisRoll \n ");
			PMDUpdate(&hAxis[AxisRoll]);
		}
		break;
		case (AxisPitch):
		{
			pitch(INF, (hAxis[Axis].axisSettings.homeVelocity)/2,
			hAxis[Axis].axisSettings.homeAcceleration, update,INICIANDO_HOME_PITCH_ROLL);
			#ifdef DEBUGROBOTICS_HOME
			PMDGetHostIOError(&hAxis[AxisRoll], &test);
			printf("search_home_positive=> HostIOError = %x del eje ROLL despues del update.\n ",test);
			#endif
		}
	    break;
		
		case (AxisRoll):		
			roll(INF, hAxis[Axis].axisSettings.homeVelocity,
			hAxis[Axis].axisSettings.homeAcceleration, update,INICIANDO_HOME_PITCH_ROLL );
			#ifdef DEBUGROBOTICS_HOME
			PMDGetHostIOError(&hAxis[AxisRoll], &test);
			printf("search_home_positive=> HostIOError = %x del eje pitch despues del update.\n ",test);
			#endif
	
	    break;
		
		default:
			mov_axis(Axis,INF, hAxis[Axis].axisSettings.homeVelocity,
			hAxis[Axis].axisSettings.homeAcceleration,update);
	    break;
	}
		
	

	#ifdef DEBUGROBOTICS_HOME
	PMDGetHostIOError(&hAxis[Axis], &test);
	printf("search_home_positive=> HostIOError = %x del eje %s despues del Update.\n ",test,AxisName(Axis));
	printf("search_home_positive=> Iniciando busqueda de home en el sentido positivo.\n");	
	#endif	
	
	
	do 
	{ 
		PMDGetEventStatus(&hAxis[Axis],&status);
		
		if((status & PMDEventCaptureReceivedMask) != 0) 
		{   
			#ifdef DEBUGROBOTICS_HOME
			printf("search_home_positive=>  Captura de HOME realizada. \n ");			
			#endif
			PMDGetCaptureValue(&hAxis[Axis],home);
			PMDResetEventStatus(&hAxis[Axis],~PMDEventCaptureReceivedMask);
			
			if(rangos==FALSE) //se Hace Caputura de HOME y se quiere detener el motor
			{	
				
				//se Cambia la variable a 0 para salir del ciclo.
				find_home_out=0;
				
				//Detener suavemente el movimiento de los ejes implicados
				//Se supone que como ninguno esta tocando algun limites, excepto
				//del eje donde se esta buscando HOME.
				//Ejempo si estoy buscando el home de hombro hay que detener codo y pitch
				//Ya que Home se detiene automaticamente. 
				//Tambien hay que tomar en cuenta los casos especiales de Cintura y roll 
				//Que no se detiene al llegar el home.
				switch (Axis)
				{
				    case (AxisWaist):
						PMDSetStopMode(&hAxis[Axis], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[Axis]);
				    break;
					case (AxisRoll):
						PMDSetStopMode(&hAxis[Axis], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[Axis]);
						PMDSetStopMode(&hAxis[AxisPitch], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[AxisPitch]);
					case (AxisPitch):
						PMDSetStopMode(&hAxis[AxisRoll], PMDAbruptStopMode ); 
						PMDUpdate(&hAxis[AxisRoll]);
				    break;
				    default:
						
				    	for(Axis_Update=Axis+1; Axis_Update < DOF; Axis_Update++) 
						{ 
							PMDSetStopMode(&hAxis[Axis_Update], PMDAbruptStopMode ); 
							PMDUpdate(&hAxis[Axis_Update]);
						}
				    break;
				}
			}			
		}
		
		#ifdef DEBUGROBOTICS_HOME
		//	printf("Moviendo en direccion positiva. \n ");
		//	printf("El Event status register del eje %s es %X \n",AxisName(Axis),status);	
		#endif
	} 
	while (((status & PMDEventInPositiveLimitMask) ==0) && (find_home_out==1));

	
	#ifdef DEBUGROBOTICS_HOME
	PMDGetHostIOError(&hAxis[Axis], &test);
	printf("search_home_positive=> Error en el eje %s despues del movimiento %x\n ",AxisName(Axis),test);
	PMDGetEventStatus(&hAxis[Axis],&status);  
	printf("search_home_positive=> El Event status register antes de limpiar del eje %s es %X \n",AxisName(Axis),status);	
	#endif	

	
	/*Se limpia el Event Status Register, borrando las interrupciones generadas: Evento de Limite
	Positivo y Fin de movimiento*/	
	Reset_Even_Status_Register();
	
	
	/* Se obtiene la posicion Actual del encoder*/
	PMDGetActualPosition(&hAxis[Axis],posicion);
	
	#ifdef DEBUGROBOTICS_HOME
	PMDGetEventStatus(&hAxis[Axis],&status);  
	printf("search_home_positive=> El Event status register del eje %s despues de limpiar es %X \n ",AxisName(Axis),status);	
	#endif
}
/**********************************************************************************************************************/
void search_home_roll_positive(PMDint32 *home)
{
	PMDuint16 status,find_home_out, timer;
	PMDint32 temp,Correcion;
	double rad;

  	

	find_home_out=1;
	
	No_limit_switch(AxisPitch);
	//Se cambia el perfil movimiento a perfil de  Velocidad	
	PMDSetProfileMode(&hAxis[AxisRoll],  PMDVelocityContouringProfile );
	PMDSetProfileMode(&hAxis[AxisPitch],  PMDVelocityContouringProfile );
	//Limpia el BiT de Position Capture Indicator en el Status Register
	PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventCaptureReceivedMask);	
	//La senal de HOME sera utilizada como Disparo para la Captura de la Posicion
	PMDSetCaptureSource(&hAxis[AxisRoll],PMDCaptureSourceHome);
	//Leyendo el registro de Captura de Posicion Causa "rearmar" 
	//el disparador con el fin de permitir mas capturas.
	PMDGetCaptureValue(&hAxis[AxisRoll],&temp); 

	#ifdef DEBUGROBOTICS_HOME
	printf("search_home_roll_positive=> Ejecutado No_limit_switch\n");
	printf("search_home_roll_positive=> Moviendo Roll en direccion Positiva para Hallar HOME \n ");
	#endif
	roll(none,0.7*(hAxis[AxisRoll].axisSettings.homeVelocity),
				hAxis[AxisRoll].axisSettings.homeAcceleration, update,INICIANDO_HOME_PITCH_ROLL);
	   
	
	timer = 50;//50; 	//Este valor garantiza un movimiento de 45 grados en la direccion positiva
	
	do 
	{ 
		PMDGetEventStatus(&hAxis[AxisRoll],&status);
		
		if((status & PMDEventCaptureReceivedMask) != 0) // se ha capturado home
		{   
			#ifdef DEBUGROBOTICS_HOME
				printf("search_home_roll_positive=> Captura de HOME realizada. \n ");			
			#endif
			
			PMDGetCaptureValue(&hAxis[AxisRoll],home);
			PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventCaptureReceivedMask);
			
			#ifdef DEBUGROBOTICS_HOME
				printf("search_home_roll_positive=> Parando Motor... \n ");
			#endif
				
			//se Cambia la variable a 0 para salir del ciclo.
			find_home_out=0;
				
			//detener el movimiento
			PMDSetStopMode(&hAxis[AxisRoll], PMDAbruptStopMode ); 
			PMDUpdate(&hAxis[AxisRoll]);
			PMDSetStopMode(&hAxis[AxisPitch], PMDAbruptStopMode ); 
			PMDUpdate(&hAxis[AxisPitch]);	
			//Adicionando correccion
			rad=grados2rad(AJUSTE_ROLL_HOME_POSI_TIME);
			Correcion=rad2usteps(&hAxis[AxisPitch].axisSettings,rad);
			printf("search_home_roll_positive=> Factor de correcion de roll (search_home_positive) en uPasos = %ld\n",Correcion);
			printf("search_home_roll_positive=> home = %ld\n",*home);
			*home = *home+Correcion;	
			printf("search_home_roll_positive=> home + Correcion = %ld\n",*home);	
			
		}
			
		timer --;
		
		#ifdef DEBUGROBOTICS_HOME		
		if(timer==0)
			printf("search_home_roll_positive=> Time OUT . \n ");				
		printf("search_home_roll_positive=> Moviendo en direccion positiva. \n ");
		#endif
	} 
	while  ((find_home_out==1) && (timer !=0));//Mientras encuentre HOME o no  se cumpla el timer

	
	#ifdef DEBUGROBOTICS_HOME		
		printf("search_home_roll_positive=> El Valor del timer es  %d \n",timer);	
	#endif	
	
	//detener  el movimiento
	PMDSetStopMode(&hAxis[AxisRoll], PMDAbruptStopMode ); 
	PMDUpdate(&hAxis[AxisRoll]);
	PMDSetStopMode(&hAxis[AxisPitch], PMDAbruptStopMode ); 
	PMDUpdate(&hAxis[AxisPitch]);
	
	/*Se cambia el perfil movimiento a perfil trapezoidal*/ 
	PMDSetProfileMode(&hAxis[AxisRoll], PMDTrapezoidalProfile);
	PMDSetProfileMode(&hAxis[AxisPitch], PMDTrapezoidalProfile);	
	
	

	
	
}

/*****************************************************************************************/ 
PMDresult initializeAxis(int16 Axis) //falta realizar para Pitch & Roll y enviar o guardar los Datos encontrados!!
{
	PMDuint16 rc = rcOK;
	
	PMDint32 home=0,home1=INF,home2=INF,minPos,maxPos;
	
	
	
	//utilizada para hacer un scanf para leer el dato
	#ifdef DEBUGROBOTICS
		int16 temp1;
	#endif		


    // Deshabilitada las interrupciones hacia el uControlador
	HostInterruptDisable();							
	//Limpia la Posicion de Error. Posicion_Comandada=Posicion_Acutal
	PMDClearPositionError(&hAxis[Axis]); 			
	PMDUpdate(&hAxis[Axis]);

	printf("initializeAxis=> Inicializado eje %s\n  ",AxisName(Axis));
	#ifdef DEBUGROBOTICS
	//	scanf("%d",&temp1);
	#endif

	switch(Axis) 
	{
		case  (AxisPitch) :
		case  (AxisElbow) : 					
			
			//Si el eje es Pitch se debe limpiar la posicion de error para el motor Roll.
			if(Axis==AxisPitch)
			{
				PMDClearPositionError(&hAxis[AxisRoll]); 			
				PMDUpdate(&hAxis[AxisRoll]);
			}
			
		    //Mover el Eje en Direccion Positiva
			search_home_positive(Axis,&home1,&maxPos,TRUE);
			#ifdef DEBUGROBOTICS
				printf("initializeAxis=> El valor home1 del eje %s = %ld. \n ",AxisName(Axis),home1);
				printf("initializeAxis=> El valor maxPos del eje %s = %ld. \n ",AxisName(Axis),maxPos);
			//	scanf("%d",&temp1);
			#endif			
			
			//Mover el Eje en Direccion Negativa
			search_home_negative(Axis,&home2,&minPos,TRUE);
			#ifdef DEBUGROBOTICS
				printf("initializeAxis=> El valor home2 del eje %s = %ld. \n ",AxisName(Axis),home2);
				printf("initializeAxis=> El valor minPos del eje %s = %ld. \n ",AxisName(Axis),minPos);
			//	scanf("%d",&temp1);
			#endif			
        break;
		default : 					 // just use neg. limit
			//Mover el Eje en Direccion Negativa
			search_home_negative(Axis,&home1,&minPos,TRUE);
			#ifdef DEBUGROBOTICS
				printf("initializeAxis=> El valor home1 del eje %s = %ld. \n ",AxisName(Axis),home1);
				printf("initializeAxis=> El valor minPos del eje %s = %ld. \n ",AxisName(Axis),minPos);
				//scanf("%d",&temp1);
			#endif
			//PMDResetEventStatus(&hAxis[Axis],~(PMDEventMotionCompleteMask | PMDEventInNegativeLimitMask));	
			//PMDSetMotorMode(&hAxis[Axis],PMDMotorOn); 
					 
			//Mover el Eje en Direccion Positiva
			search_home_positive(Axis,&home2,&maxPos,TRUE);
			#ifdef DEBUGROBOTICS
				printf("initializeAxis=> El valor home2 del eje %s = %ld. \n ",AxisName(Axis),home2);
				printf("initializeAxis=> El valor maxPos del eje %s = %ld. \n ",AxisName(Axis),maxPos);
				//scanf("%d",&temp1);
			#endif	
		break;
	}
	
	
	//------------- depending on the axis configuration, calibrate the HOME value.-----------------
	switch(hAxis[Axis].axisSettings.homeSwitchHandling) 
	{
		case -1 : 
		case  1 : 
			home = home2;            // just use right limit
		break;
				
		case  0 : 
			if(home1==INF)
				{	
			//		home=home2+(AJUSTE_HOME1*(hAxis[Axis].axisSettings.ustepsPerStep));
				}
				else					
			//		home = ((home1+home2)/2)-(AJUSTE_HOME2*(hAxis[Axis].axisSettings.ustepsPerStep)); // use medium position
        break;
	}	
	
	hAxis[Axis].axisLimits.minPosition = minPos-home;
	hAxis[Axis].axisLimits.maxPosition = maxPos-home;
	PMDAdjustActualPosition(&hAxis[Axis],-home); 
	

	
	//-------------------------Solo para pruebas---------------------------------------------------	
    #ifdef DEBUGROBOTICS
		printf("initializeAxis=> El valor de home del eje %s = %ld. \n ",AxisName(Axis),home);
		printf("initializeAxis=> Valor final para maxPos del eje %s = %ld. \n ",AxisName(Axis),hAxis[Axis].axisLimits.maxPosition);
		printf("initializeAxis=> Valor final para minPos del eje %s = %ld. \n ",AxisName(Axis),hAxis[Axis].axisLimits.minPosition);
		printf("initializeAxis=> Moviendo hacia Home \n ");
	#endif
	
	switch (Axis)
	{
		case (AxisPitch):
			//Se coloca home positivo ya que el motor de roll esta invertido con respecto al motor de pitch
			PMDAdjustActualPosition(&hAxis[AxisRoll],home);
			
			
			pitch(HOME,hAxis[Axis].axisSettings.homeVelocity,
						hAxis[Axis].axisSettings.homeAcceleration,update,EN_CALIBRACION_PROCESOHOME ); 
		break;
		
		default:
			mov_axis(Axis,HOME,hAxis[Axis].axisSettings.homeVelocity,
						hAxis[Axis].axisSettings.homeAcceleration,update); 
	    break;
	}

	//habilitada las interrupciones hacia el uControlador
	HostInterruptEnable();
	return rc;
	
}
/*****************************************************************************************/
void teach_pendant_point(tTrajectory   *TrajectoryPOINT,int16 pointIndex )
{
	
	
	PMDint32 position,velocity, acceleration;
	PMDint32 velocity_elbow_usr,velocity_pich_usr;
	PMDint32 acceleration_elbow_usr,acceleration_pich_usr;
	PMDint32 actual_shoulder_position, actual_elbow_position,actual_position;
	int16 mask;
	uint16 status;
	int16 reconf_Elbow,reconf_Pitch;
	int16 test8,axis;
	
	#ifdef DEBUGROBOTICS
	PMDint32	actual_pitch_position;
	int16 temp;
	#endif
	
	position=0;
	velocity=0;
	acceleration=0;
	reconf_Elbow=0;
	reconf_Pitch=0;
	Enable_Mov();		
	mask= TrajectoryPOINT->invalidmask;
	
	
	/*Posicion Actual de Hombro y codo y Pitch*/
	PMDGetPosition(&hAxis[AxisShoulder],&actual_shoulder_position);
	PMDGetPosition(&hAxis[AxisElbow],&actual_elbow_position);
	
	
	printf("teach_pendant_point=> Ejecutando movimiento o pose asignada\n");
	#ifdef DEBUGROBOTICS
		eeprom_read_block((void*)&actual_pitch_position,(const void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
		printf("teach_pendant_point=> actual_shoulder_position = %ld\n",actual_shoulder_position);
		printf("teach_pendant_point=> actual_Elbow_position = %ld\n",actual_elbow_position);	
		printf("teach_pendant_point=> actual_pitch_position = %ld\n",actual_pitch_position);	
	#endif
	//Configuro condiciones iniciales (Caso en el eje no se selecciona para el movimiento, 
	//por tal motivo debe mantener el valor en donde se encuentra)
	for(axis=0; axis < DOF; axis++) 
	{  
		
		if ( hAxis[axis].axisSettings.axisEnabled == TRUE) 		
		{	
			 // El eje no esta habilitado para el movimiento, igual tengo que mantener la posicion actual
			if( ( mask & Axis2AxisMask(axis) )== 0 )     
			{
				//Obtengo la posicion ANTES de actualizar!
				PMDGetPosition(&hAxis[axis],&actual_position);	
				
				switch (axis)
				{
					
					case (AxisShoulder):
						actual_position=actual_shoulder_position;
					break;
					case (AxisElbow):
						/*Se tiene la siguiente formula:
						Pos_Codo= Pos_Codo_inicial + PosHombro*(R_Codo/R_Hombro)
						Como es necesario la posicion inicial del codo se despeja, Pos_Codo_inicial.*/
						actual_position=(actual_position)-(actual_shoulder_position*CORRELACION_HOMBRO_CODO);					
					break;
					case (AxisPitch):
						/* Leer el movimiento anterior de Pitch que esta almacenado en EEPROM*/
						eeprom_read_block((void*)&actual_position,(const void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
						actual_position = (actual_position)-(actual_elbow_position*CORRELACION_CODO_PITCH);
					break;
					case (AxisRoll):
						/* Leer el movimiento anterior de Roll que esta almacenado en EEPROM*/
						eeprom_read_block((void*)&actual_position,(const void*)&Mov_Roll_Anterior,sizeof(PMDint32));
					break;								
					
					default:
						//Aplica solo  para cintura.
						
					break;
				}				
				TrajectoryPOINT->point[pointIndex].axis[axis].position= actual_position;
				#ifdef DEBUGROBOTICS				
					printf("teach_pendant_point=> Posicion anterior %s = %ld\n",AxisName(axis),actual_position);						
					
				#endif
			}
		}
	}
	/*Salvo la velocidad y aceleracion de usuario de pitch y codo */
	velocity_elbow_usr=TrajectoryPOINT->point[pointIndex].axis[AxisElbow].velocity;
	acceleration_elbow_usr=TrajectoryPOINT->point[pointIndex].axis[AxisElbow].acceleration;
	velocity_pich_usr=TrajectoryPOINT->point[pointIndex].axis[AxisPitch].velocity;
	acceleration_pich_usr=TrajectoryPOINT->point[pointIndex].axis[AxisPitch].acceleration;
	//Busco el primer eje seleccionado y configuro el resto, es decir que si encuentro que hombro es el primero, 
	//configuro hombro, codo, pitch y salgo imediatamente del for para no reconfigurar con datos erroneos los demas ejes
	for(axis=0; axis < DOF; axis++) 
	{  
		
		if ( hAxis[axis].axisSettings.axisEnabled == TRUE) 		
		{	
			 // El eje esta habilitado para el movimiento,
			if( ( mask & Axis2AxisMask(axis) )!= 0 )     
			{	
				switch (axis)
				{
					case (AxisShoulder):
						//No depende de nadie va directo...  
						//(Siempre entra aqui cuando se usa con una trayectoria )..
						mov_axis(AxisShoulder,TrajectoryPOINT->point[pointIndex].axis[axis].position, 
								TrajectoryPOINT->point[pointIndex].axis[axis].velocity,
								TrajectoryPOINT->point[pointIndex].axis[axis].acceleration, 
								no_update);									
						
						mov_correlacionado(TrajectoryPOINT,pointIndex,AxisShoulder,AxisElbow,CORRELACION_HOMBRO_CODO);		
						
						mov_correlacionado(TrajectoryPOINT,pointIndex,AxisElbow,AxisPitch,CORRELACION_CODO_PITCH);
						//Se coloca Axis=AxisPitch para que no vuelva a cargar datos de movimiento en los demás ejes y salte a cargar
						//el eje de roll.
						axis=AxisPitch;
					break;
					case (AxisElbow):
							//Si el hombro se ha movido tengo que tomarlo en cuenta para calcular la 
							//posicion del codo
							mov_correlacionado(TrajectoryPOINT,pointIndex,AxisShoulder,AxisElbow,CORRELACION_HOMBRO_CODO);				
							mov_correlacionado(TrajectoryPOINT,pointIndex,AxisElbow,AxisPitch,CORRELACION_CODO_PITCH);
							//Se coloca Axis=AxisPitch para que no vuelva a cargar datos de movimiento en los demás ejes y salte a cargar
							//el eje de roll.
							axis=AxisPitch;
					break;					
					case (AxisPitch):
						#ifdef DEBUGROBOTICS				
							printf("teach_pendant_point=> cargando valores de Pitch\n");
							//scanf("%d",&temp);
						#endif
						mov_correlacionado(TrajectoryPOINT,pointIndex,AxisShoulder,AxisElbow,CORRELACION_HOMBRO_CODO);
						mov_correlacionado(TrajectoryPOINT,pointIndex,AxisElbow,AxisPitch,CORRELACION_CODO_PITCH);					
					break;
					case (AxisRoll):
						#ifdef DEBUGROBOTICS				
							printf("teach_pendant_point=> cargando valores de Roll\n");
						//	scanf("%d",&temp);
						#endif
						position     = TrajectoryPOINT->point[pointIndex].axis[axis].position;
						velocity     = TrajectoryPOINT->point[pointIndex].axis[axis].velocity;
						acceleration = TrajectoryPOINT->point[pointIndex].axis[axis].acceleration;
						
					break;								
					default:
						//Aplican para cintura
						mov_axis(axis,TrajectoryPOINT->point[pointIndex].axis[axis].position, 
								TrajectoryPOINT->point[pointIndex].axis[axis].velocity,
								TrajectoryPOINT->point[pointIndex].axis[axis].acceleration, 
								no_update);	
					break;
				}
				
			}
		}
	}
	
	
	/*Se realiza el movimiento simultaneo de las articulaciones*/
	for(axis=0; axis < DOF; axis++) 
	{ 
		if(hAxis[axis].axis == 0) 
		{
			PMDMultiUpdate(&hAxis[axis],PMDAxis1Mask|PMDAxis2Mask|PMDAxis3Mask|PMDAxis4Mask);    // move the robot (all axis in parallel)
		} 
	}
	
	/*Espero un segundo a que los ejes comiencen a moverse*/
	for(test8=0; test8 < 10; test8++) 
	{ 
		_delay_ms(100);
		
	}
	
	do
	{
		//Hallo el El registro de actividad en el hombro
		PMDGetActivityStatus(&hAxis[AxisShoulder],&status);
		//Si hombro se mueve
		if ((status & PMDActivityInMotionMask) !=0)  
		{
			//tengo que mover el codo con la misma velocidad para no chocar	
		}
		else
		{	//Ya me puedo mover a la velocidad escogida por el usuario.
			if(reconf_Elbow==0)
			{
				#ifdef DEBUGROBOTICS
				printf("teach_pendant_point=> Reconfigurando Codo\n");
				printf("teach_pendant_point=> Velocidad codo usuario = %ld\n",velocity_elbow_usr);	
				printf("teach_pendant_point=> Aceleracion codo usuario = %ld\n",acceleration_elbow_usr);	
				#endif
				reconf_Elbow=1;
				PMDSetVelocity(&hAxis[AxisElbow],velocity_elbow_usr); 
				PMDSetAcceleration(&hAxis[AxisElbow],acceleration_elbow_usr); 
				PMDUpdate(&hAxis[AxisElbow]);
			}
			//Hallo el El registro de actividad en el codo
			PMDGetActivityStatus(&hAxis[AxisElbow],&status);
			//Si codo se mueve
			if ((status & PMDActivityInMotionMask) !=0)
			{
				//tengo que mover el pitch con la misma velocidad para no chocar	
			}
			else
			{
				//Ya me puedo mover a la velocidad escogida por el usuario.
				if(reconf_Pitch==0)
				{
					reconf_Pitch=1;
					if(velocity_pich_usr>0)
					{
						#ifdef DEBUGROBOTICS
						printf("teach_pendant_point=> Reconfigurando Pitch\n");
						printf("teach_pendant_point=> Velocidad Pitch usuario = %ld\n",velocity_pich_usr);	
						printf("teach_pendant_point=> Aceleracion Pitch usuario = %ld\n",acceleration_pich_usr);	
						printf("teach_pendant_point=> Posicion final de  Pitch= %ld\n",TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position);	
						#endif
						//No puede usar la funcion pitch porque se reinicia los encoderes..
						mov_axis(AxisPitch, TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position, 
						velocity_pich_usr, acceleration_pich_usr, update);
						mov_axis(AxisRoll, -TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position,
						(velocity_pich_usr), acceleration_pich_usr, update);
					}
				}
			}
		}
		
	}	
	while( PMDcheckIdle(mask & ~Axis2AxisMask(AxisRoll))==FALSE);
	
	
	
	/* Si esta activado encoderEnable, el movimiento solo termina cuando se cumple:
		-La generacion del profile a terminado.
		-La diferencia entre la posiscion comandada y actual es menor a SettleWindow.
		-Las condiciones se debe cumplir por lo menos un tiempo especificado por SetSettleTime.
	
	*/


	
	//Una vez el robot este totalmente detenido se empieza a efectur el movimiento de Roll 
	//(si existe) para eso se debe habiliar los ejes de pitch and Roll limpiando  el bit
	//de Motion Complete ( Bit 0 del Event Status Register).	
	if((hAxis[AxisRoll].axisSettings.axisEnabled == TRUE) && 
		((mask & (Axis2AxisMask(AxisRoll))) >0  )) 
	{ 
		Enable_Mov();
		roll(position,velocity,acceleration,update,MOVIMIENTO_NORMAL );			//Efectuo movimiento de Roll.
	}

	while( PMDcheckIdle(mask & Axis2AxisMask(AxisRoll))==FALSE)	
	{	

		printf("teach_pendant_point=> Efectuando Movimiento de Roll.\n");

	}

	//Habilitar todos los ejes para el proximo movimiento limpiando la el bit
	//de Motion Complete ( Bit 0 del Event Status Register).*/

	Enable_Mov();
	
	printf("teach_pendant_point=> Fin del Movimiento \n");
	
	
}

/**********************************************************************************************************************/
PMDint32 max(PMDint32 a, PMDint32 b)
{
	if (a > b) 
		return a;
	else
		return b;
}

/**********************************************************************************************************************/
PMDint32 min(PMDint32 a, PMDint32 b)
{
	if (a < b) 
		return a;
	else
		return b;
}

/**********************************************************************************************************************/
PMDint32 calibration(int16 Axis)
{
	PMDint32 max_read, min_read,read_encoder,sum,read;
	PMDuint16 signal_status;
	int16 i=0,Cont=0;
	

	//Elimina el Error.
	PMDClearPositionError(&hAxis[Axis]);
	sum=0;
	max_read= -INF;
	min_read=  INF;

	
	for (i=0; i<5; i++)
	{
		//Mover el eje en la direccion positiva del movimiento. Hasta el que llegue al limite de
		//fin de carrera
		
		switch (Axis)
		{
			
			case (AxisWaist):
		      mov_axis(AxisWaist,INF,60000,100,update);
		   break;
			
		   case (AxisShoulder):
		      mov_axis(AxisShoulder,INF,30000,100,update);
		   break;
			
			case (AxisElbow):
		      mov_axis(AxisElbow,INF,120000,100,update);
		   break;
			
			case (AxisPitch):
		      pitch(INF,35000,100,update,EN_CALIBRACION_PROCESOHOME);
		   break;
			
		   case (AxisRoll):
		      roll(INF,35000,100,update,EN_CALIBRACION_PROCESOHOME );
		   break;
			
		}
		
		
		if(Axis==AxisRoll)
		{
		    delay_1s();
			delay_1s();
			do 
			{
				PMDGetSignalStatus(&hAxis[Axis], &signal_status);				
			} 
			while ((signal_status & PMDSignalEncoderHomeMask)!=0);
			
			//Detener el Eje.(Ya que el Eje Roll no tiene Limites de fin de carrera.
			PMDSetStopMode(&hAxis[Axis], PMDAbruptStopMode); 
			PMDSetStopMode(&hAxis[AxisPitch], PMDAbruptStopMode); 
			PMDUpdate(&hAxis[Axis]);	
			PMDUpdate(&hAxis[AxisPitch]);	
			
		}
		else
		{
			do 
			{			
				PMDGetSignalStatus(&hAxis[Axis], &signal_status);				
				printf("calibration=> El Signal status register al moverse en + del eje %s es %X \n",AxisName(Axis),signal_status);					
			} 
			while ((signal_status & PMDSignalPositiveLimitMask)!=0);
		}
		
		// En este punto el Eje se Detiene automaticamente.
		
		//Leer Registro del Encoder
		PMDGetActualPosition(&hAxis[Axis],&read_encoder);
		
		
			printf("calibration=> La Lectura del valor No %d del encoder del eje %s es : %08ld \n ",(2*i+1),AxisName(Axis),read_encoder);
		  //  scanf("%d",&Cont);


		
		//Sacar Maximos y Minimos
		max_read = max(max_read, read_encoder);
		min_read = min(min_read, read_encoder);
		
		//sumar el valor leido
		sum=sum+read_encoder;
		
		//Borrar el registro del Encoder
		PMDAdjustActualPosition(&hAxis[Axis],-read_encoder);
		
		//Se limpia el Event Status Register, borrando la interrupcion 		
		PMDResetEventStatus(&hAxis[Axis],~PMDEventInPositiveLimitMask);
		
		//Se habilita el Generador de Trayectoria.
		PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);  		
		
		if(Axis==AxisPitch)
		{
			PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventInNegativeLimitMask);	
			PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn); 
		}	
		
		read_encoder=0;
		
	
	//mover en la direccion negativa del movimiento. Hasta el que llegue al limite de
	//fin de carrera
		#ifdef DEBUGROBOTICS
		printf("calibration=> Empezar Movimiento en direccion  negativa \n ");			
		#endif
		
		switch (Axis)
		{
			
			case (AxisWaist):
		      mov_axis(AxisWaist,-INF,60000,100,update);
		   break;
			
		   case (AxisShoulder):
		      mov_axis(AxisShoulder,-INF,30000,100,update);
		   break;
			
			case (AxisElbow):
		      mov_axis(AxisElbow,-INF,120000,100,update);
		   break;
			
			case (AxisPitch):
		      pitch(-INF,35000,100,update,EN_CALIBRACION_PROCESOHOME);
		   break;
			
		   case (AxisRoll):
		      roll(-INF,35000,100,update,EN_CALIBRACION_PROCESOHOME );
		   break;
		   
		}
		
		if(Axis==AxisRoll)
		{
		 //Modificado por david
		    delay_1s();
			delay_1s();
			
		 	
			do 
			{
				PMDGetSignalStatus(&hAxis[Axis], &signal_status);				
			} 
			while ((signal_status & PMDSignalEncoderHomeMask)!=0);
			
			//Detener el Eje.(Ya que el Eje Roll no tiene Limites de fin de carrera.
			PMDSetStopMode(&hAxis[Axis], PMDAbruptStopMode); 
			PMDSetStopMode(&hAxis[AxisPitch], PMDAbruptStopMode); 
			PMDUpdate(&hAxis[Axis]);	
			PMDUpdate(&hAxis[AxisPitch]);			
		}
		else
		{
			do 
			{				
				PMDGetSignalStatus(&hAxis[Axis], &signal_status);					
				printf("calibration=> El Signal status register del eje %s al moverse en - es %X \n",AxisName(Axis),signal_status);					
			} 
			while ((signal_status & PMDSignalNegativeLimitMask)!=0);
		}		
		
		// En este punto el Eje se Detiene automaticamente.
		
		//Leer Registro del Encoder
		PMDGetActualPosition(&hAxis[Axis],&read_encoder);
		printf("calibration=> La Lectura del valor No %d del encoder del eje %s es : %08ld \n ",(2*i+2),AxisName(Axis),read_encoder);
		//printf("La Lectura del valor del encoder del eje %s es : %08ld \n ",AxisName(Axis),read_encoder);
		//scanf("%d",&Cont);
		
		//Sacar Maximos y Minimos
		max_read = max(max_read, -read_encoder);
		min_read = min(min_read, -read_encoder);
		
		//sumar el valor leido
		sum=sum-read_encoder;
		
		//Borrar el registro del Encoder
		PMDAdjustActualPosition(&hAxis[Axis],-read_encoder);
		
		//Se limpia el Event Status Register, borrando la interrupcion 		
		PMDResetEventStatus(&hAxis[Axis],~PMDEventInNegativeLimitMask);
		
		//Se habilita el Generador de Trayectoria.
		PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);  		
		
		if(Axis==AxisPitch)
		{
			PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventInPositiveLimitMask);	
			PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn); 
		}
		read_encoder=0;
	}
	
	printf("calibration=> Los Valores maximo y minimo son= %08ld y %08ld  \n ",max_read,min_read);
	printf("calibration=> La sumatoria de los valores leidos es : %08ld \n",sum);
	read = (sum-(max_read + min_read))/(2*(i-1));
	
	return read;
}



/**********************************************************************************************************************/
void ObtenerPose(PoseRobot *Pose)
{

	PMDint32 Mov_Pitch_Anterior_RAM,Mov_Roll_Anterior_RAM,Pos_Elbow_corelacionada;

	//Carga posicion de cintura
	PMDGetPosition(&hAxis[AxisWaist],&Pose->Pos_Waist);
	
	//Carga poscion de hombro
	PMDGetPosition(&hAxis[AxisShoulder],&Pose->Pos_Shoulder);
	
	
	
	//Carga poscion de codo
	PMDGetPosition(&hAxis[AxisElbow],&Pos_Elbow_corelacionada);
	printf("ObtenerPose=> Pose->Pos_Elbow con correlacionado = %ld\n",Pos_Elbow_corelacionada);
	Pose->Pos_Elbow = Pos_Elbow_corelacionada - (Pose->Pos_Shoulder*CORRELACION_HOMBRO_CODO);
	printf("ObtenerPose=> Pose->Pos_Elbow = %ld\n",Pose->Pos_Elbow);
	
	
	
	//Carga poscion de pitch
	eeprom_read_block((void*)&Mov_Pitch_Anterior_RAM,(const void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
	//#ifdef DEBUGROBOTICS
	printf("ObtenerPose=> Mov_Pitch_Anterior_RAM = %ld\n",Mov_Pitch_Anterior_RAM);
	//#endif
	Pose->Pos_Pitch=Mov_Pitch_Anterior_RAM - (Pos_Elbow_corelacionada *CORRELACION_CODO_PITCH) ;
	//#ifdef DEBUGROBOTICS
	printf("ObtenerPose=> Pose->Pos_Pitch = %ld\n",Pose->Pos_Pitch);
	//#endif
	
	
	
	//Carga poscion de roll
	eeprom_read_block((void*)&Mov_Roll_Anterior_RAM,(const void*)&Mov_Roll_Anterior,sizeof(PMDint32));
	//#ifdef DEBUGROBOTICS
	printf("ObtenerPose=> Mov_Roll_Anterior_RAM =  %ld\n",Mov_Roll_Anterior_RAM);
	//#endif
	Pose->Pos_Roll=Mov_Roll_Anterior_RAM;

}
/**********************************************************************************************************************/
void rangosrobot(tAxis Axis)
{
    PMDint32 position;
	double rad;
	double grad;
	
	
	
	
	PMDGetActualPosition(&hAxis[Axis],&position);
	
	rad=ustep2rad(&hAxis[Axis].axisSettings, position);
	grad=rad2grados(rad);
	
	printf("rangosrobot=> %s posicion = %ld \n",AxisName(Axis),position);	
	printf("rangosrobot=> %s grados = %f \n \n",AxisName(Axis),grad);

	
}
/**********************************************************************************************************************/
void StepCalibration(tAxis Axis)
{


	PMDint32 position,encoder;
  
	PMDGetCommandedPosition(&hAxis[Axis],&position);
	printf("StepCalibration=> Posicion Instantanea antes del ajuste %ld\n",position);
	PMDGetActualPosition(&hAxis[Axis],&encoder);
	printf("StepCalibration=> Lectura del encoder %ld\n",encoder);
	
	printf("StepCalibration=> Ajustando...\n");	

	PMDAdjustActualPosition(&hAxis[Axis],-encoder); 
	PMDGetCommandedPosition(&hAxis[Axis],&position);
	printf("StepCalibration=> Posicion Instantanea despues del ajuste %ld\n",position);
	PMDGetActualPosition(&hAxis[Axis],&encoder);
	printf("StepCalibration=> Lectura encoder Despues de ajustar %ld\n",encoder);	
	mov_axis(Axis,INF,300000,100,update);
	
	while(encoder < 480)
	{
     
	PMDGetActualPosition(&hAxis[Axis],&encoder);
	//printf("Encoder = %ld\n",encoder);	
	 
	}
	PMDSetStopMode(&hAxis[Axis],PMDAbruptStopMode);
	PMDUpdate(&hAxis[Axis]);
	PMDGetCommandedPosition(&hAxis[Axis],&position);
	
	printf("StepCalibration=> Posicion Instantanea al completar 480 pulsos del encoder => %ld\n",position);  
	printf("Encoder = %ld\n",encoder);	

}
/**********************************************************************************************************************/
void go_home(void)
{
	PMDint32 home=0,home1=INF,home2=INF,minPos,maxPos;
	PMDint32 readerror,encoder, Correcion;
	int16 Axis;	
	PMDuint16 signal_status, activity_status;
	PMDint32 Mov_Roll_Anterior_RAM;
	double rad=0;
	// informacion del movimiento
	#ifdef DEBUGROBOTICS_HOME
	PMDint32 position1, velocity1, commandedposition, commandedvelocity;
	PMDint32 commandedacceleration, readencoder1;
	PMDuint32 acceleration1, deceleration;
	PMDuint16 cuentas_encoder, micropasos,test;
	#endif
		
	/* Desactivar las interrupciones generadas por los PMDs*/
	HostInterruptDisable();	
	
	
	
	for(Axis=AxisShoulder; Axis <= DOF; Axis++) 
	{	
		if(Axis==DOF)
		{
			Axis=AxisWaist;
		}
		
		switch (Axis)
		{
			case (AxisWaist):
		    //fprintf_P(stderr,PSTR("Secuencia de HOME Cintura \n"));
		    break;
			
			case (AxisShoulder):
		    //fprintf_P(stderr,PSTR("Secuencia de HOME Hombro \n"));
		    break;
			
			case (AxisElbow):
		   // fprintf_P(stderr,PSTR("Secuencia de HOME Codo \n"));
		    break;
			
			case (AxisPitch):
		  //  fprintf_P(stderr,PSTR("Secuencia de HOME Pitch \n"));
		    break;
			
			case (AxisRoll):
		    //fprintf_P(stderr,PSTR("Secuencia de HOME Roll \n"));  ;
		    break;	
		  
		}		
		printf("go_home=> --INICIO DE SECUENCIA DE HOME EJE %s--- \n ",AxisName(Axis));
		printf("Saltar eje, teclee 1\n ");
		
		#ifdef DEBUGROBOTICS_HOME
		scanf("%d",&test);
		#endif
		
		if(test!=1)
		{
			//-----------------------------------------------------------------------------------------------------
			// Busqueda el home.
			//-----------------------------------------------------------------------------------------------------
			switch(hAxis[Axis].axisSettings.homeSwitchHandling) 
			{
				case -1 : //Hombro
				{
					//Como el interuptor es desactivo por No_limit_switch(), 
					//ya no hay necesidad de entrar a el limite para volver a salir.
					//Mover el Eje en Direccion Negativa llegando al Limite Negativo
					search_home_negative(Axis,&home1,&minPos,FALSE);
					//Mover el Eje en Direccion Positiva parando cuando encuentre el HOME
					//Nunca lo va a encontrar porque al ejecutar No_limit_switch() no esta oprimido el interuptor.
					//search_home_positive(Axis,&home2,&maxPos,FALSE);	
					rad=grados2rad(AJUSTE_HOMBRO_HOME_NEGA);
					Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
					printf("go_home=> Factor de correcion %s (search_home_negative) en uPasos = %ld\n",AxisName(Axis),Correcion);
					printf("go_home=> home1 = %ld\n",home1);
					//8300
					home = home1+Correcion;
				}
				break;
				case  1 : //Codo y Pitch
				{
					//Mover el Eje en Direccion Positiva hasta llegar al Limite Positivo.
					search_home_positive(Axis,&home1,&maxPos,FALSE);
					//Mover el Eje en Direccion Negativa parando cuando encuentre el HOME
					//Nunca lo va a encontrar porque al ejecutar No_limit_switch() no esta oprimido el interuptor.
					//search_home_negative(Axis,&home2,&minPos,FALSE);				
					//home = home1;            // just use right limit
					if(Axis==AxisElbow)
					{
						//-9700
						//home=home-9700;
						rad=grados2rad(AJUSTE_CODO_HOME_POSI);
						
					}	
					if(Axis==AxisPitch)
					{
						//-1100
						//home=home-1100;
						rad=grados2rad(AJUSTE_PITCH_HOME_POSI);
					}
					
					Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
					printf("go_home=> Factor de correcion %s (search_home_positive) en uPasos = %ld\n",AxisName(Axis),Correcion);
					printf("go_home=> home1 = %ld\n",home1);
					//home=home-9700;
					home = home1+Correcion;
				}
				break;		
				case  0 : //cintura, Roll (CASO ESPECIAL)
				{
					/*si las articulaciones (cintura) esta en home entonces no se realiza
					ningun procedimiento de busqueda de home y se asigna como home
					su posicion actual.*/
					
					if (Axis==AxisWaist)
					{
						
						PMDGetSignalStatus(&hAxis[Axis], &signal_status);				
						if((signal_status & PMDSignalEncoderHomeMask)==0) //si la articulacion esta en HOME
						{
							PMDGetActualPosition(&hAxis[Axis],&home);
						}
						else//si la articulacion no esta en HOME
						{
							//Mover el Eje en Direccion Negativa llegando al Limite Negativo
							search_home_negative(Axis,&home1,&minPos,FALSE);
							//Si no se encuentra home
							if(home1==INF)
							{	
								//Mover el Eje en Direccion Positiva hasta llegar al Limite Positivo
								search_home_positive(Axis,&home2,&maxPos,FALSE);
								rad=grados2rad(AJUSTE_CINTURA_HOME_POSI); //ok
								Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
								printf("go_home=> Factor de correcion %s (search_home_positive) en uPasos = %ld\n",AxisName(Axis),Correcion);
								printf("go_home=> home2  = %ld\n",home2);
								home = home2+Correcion;
							}
							else	
							{		
								rad=grados2rad(AJUSTE_CINTURA_HOME_NEGA);
								Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
								printf("go_home=> Factor de correcion %s (search_home_negative) en uPasos = %ld\n",AxisName(Axis),Correcion);
								printf("go_home=> home1 = %ld\n",home1);
								home = home1+Correcion;		
							}
							
						}
					}		
					if(Axis==AxisRoll)
					{
						
						// Leer el movimiento anterior de Roll que esta almacenado en EEPROM
						eeprom_read_block((void*)&Mov_Roll_Anterior_RAM,(const void*)&Mov_Roll_Anterior,sizeof(PMDint32));
						#ifdef DEBUGROBOTICS_HOME
							printf("go_home=> El valor anterior de Roll = %ld\n",Mov_Roll_Anterior_RAM);		
						#endif
						
						PMDGetSignalStatus(&hAxis[Axis], &signal_status);				
						//si la articulacion esta en HOME 
						if((signal_status & PMDSignalEncoderHomeMask)!=0)	 
						{
							//Debo mover el roll para ajustar home con search_home_positive.
							roll(-15, hAxis[AxisRoll].axisSettings.homeVelocity,
							hAxis[AxisRoll].axisSettings.homeAcceleration, update,EN_CALIBRACION_PROCESOHOME);
							Mov_Roll_Anterior_RAM=0;
						}
						#ifdef DEBUGROBOTICS_HOME
							printf("go_home=>  El eje de Roll No esta en HOME\n");		
						#endif
						//Caso Inicializacion (HOME ejecutado o reprogramacion)
						if(Mov_Roll_Anterior_RAM==0)
						{
							search_home_roll_positive(&home1);
							if(home1==INF) //que se cumplio el timer y no encontro home
							{
								printf("go_home=> caso home1==INF\n");
								search_home_negative(Axis,&home1,&minPos,FALSE);//mover en direcion negativa y hallar Home
								rad=grados2rad(AJUSTE_ROLL_HOME_NEGA);
								Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
								printf("go_home=> Factor de correcion %s (search_home_negative) en uPasos = %ld\n",AxisName(Axis),Correcion);
								printf("go_home=> home1 = %ld\n",home1);
								home = home1+Correcion;		
							}
							else
							{
								home = home1;
							}
						}
						
						//Verifico el signo del movimiento anterior
						if (Mov_Roll_Anterior_RAM > 0)// Positivo
						{
							printf("go_home=> caso Mov_Roll_Anterior_RAM > 0\n");
							search_home_negative(Axis,&home1,&minPos,FALSE);
							rad=grados2rad(AJUSTE_ROLL_HOME_NEGA);
							Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
							printf("go_home=> Factor de correcion %s (search_home_negative) en uPasos = %ld\n",AxisName(Axis),Correcion);
							printf("go_home=> home1 = %ld\n",home1);
							home = home1-Correcion;		
							
						}
						
						if (Mov_Roll_Anterior_RAM < 0) // Negativo
						{
							printf("go_home=> caso Mov_Roll_Anterior_RAM < 0\n");
							search_home_positive(Axis,&home1,&maxPos,FALSE);
							rad=grados2rad(AJUSTE_ROLL_HOME_POSI);
							Correcion=rad2usteps(&hAxis[Axis].axisSettings,rad);
							printf("go_home=> Factor de correcion %s (search_home_positive) en uPasos = %ld\n",AxisName(Axis),Correcion);
							printf("go_home=> home1 = %ld\n",home1);
							home = home1+Correcion;		
						}
					}
				}
				break;
			}	
		}
		//-----------------------------------------------------------------------------------------------------
		//Mover el eje al home encontrado + la correcion.
		//Necesitamos que el concodificador tenga el valor de home para los calculos de PMDAdjustActualPosition.
		//-----------------------------------------------------------------------------------------------------
		#ifdef DEBUGROBOTICS_HOME
		printf("go_home=> El valor de home encontrado! del eje %s = %ld. \n ",AxisName(Axis),home);
		printf("go_home=> Moviendo el Eje %s hacia home \n ",AxisName(Axis));
		//scanf("%d",&test);
		#endif
		if(home!=INF)
		{
			switch (Axis)
			{
				case (AxisPitch):	
					pitch(home,hAxis[Axis].axisSettings.homeVelocity,
						hAxis[Axis].axisSettings.homeAcceleration,update,MOVIMIENTO_HACIA_HOME ); 
				break;
				case (AxisRoll):
					roll(home,hAxis[Axis].axisSettings.homeVelocity,
						hAxis[Axis].axisSettings.homeAcceleration,update,MOVIMIENTO_HACIA_HOME );	
				break;
				default:
					mov_axis(Axis,home,hAxis[Axis].axisSettings.homeVelocity,
							hAxis[Axis].axisSettings.homeAcceleration,update); 
				break;
			}
		}	
		// se reinician las variables de Busqueda home1 y home2
		home1=INF;
		home2=INF;
		
		
		//-----------------------------------------------------------------------------------------------------
		//Informacion del movimiento al llegar home + correccion.
		//-----------------------------------------------------------------------------------------------------
		#ifdef DEBUGROBOTICS_HOME
		printf("go_home=> Informacion del movimiento antes de AJUSTAR! \n ");
		infomov(Axis,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
				&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
				&micropasos);
		if(Axis==AxisRoll)
		{
			infomov(AxisPitch,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
			&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
			&micropasos);PMDGetPositionError(&hAxis[AxisPitch], &readerror);
		}
		if(Axis==AxisPitch)
		{
			infomov(AxisRoll,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
			&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
			&micropasos);PMDGetPositionError(&hAxis[AxisPitch], &readerror);
		}
		#endif
		
		
		//-----------------------------------------------------------------------------------------------------
		//Espero a que los motores de detengan
		//-----------------------------------------------------------------------------------------------------
		do
		{
			//Hallo el El registro de actividad del eje
			PMDGetActivityStatus(&hAxis[Axis],&activity_status);
		}
		while ((activity_status & PMDActivityInMotionMask) !=0);
		
		
		//-----------------------------------------------------------------------------------------------------
		//Adjustando el Valor home encontrado a Cero
		//-----------------------------------------------------------------------------------------------------
		switch (Axis)
		{
			case (AxisPitch):	
				//Se coloca home positivo ya que el motor de roll esta invertido con respecto al motor de pitch
				PMDAdjustActualPosition(&hAxis[AxisRoll],home);	
				#ifdef DEBUGROBOTICS_HOME
				printf("Informacion del movimiento despues de AJUSTAR AxisRoll! \n");
				infomov(AxisRoll,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
				&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
				&micropasos);
			//	scanf("%d",&test);
				#endif
			break;
			case (AxisRoll):
				PMDAdjustActualPosition(&hAxis[AxisPitch],-home);
				#ifdef DEBUGROBOTICS_HOME
				printf("Informacion del movimiento despues de AJUSTAR AxisPitch! \n");
				infomov(AxisPitch,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
				&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
				&micropasos);
				//scanf("%d",&test);
				#endif
			break;
			default:
				PMDAdjustActualPosition(&hAxis[Axis],-home); 		
			break;
		}
		
		
		//-----------------------------------------------------------------------------------------------------
		//Imprimiendo estado del eje despues del ajuste
		//-----------------------------------------------------------------------------------------------------
		#ifdef DEBUGROBOTICS_HOME
		printf("go_home=> Informacion del movimiento despues de AJUSTAR! \n ");
		infomov(Axis,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
				&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
				&micropasos);
		#endif
		//-----------------------------------------------------------------------------------------------------
		//Caso especial de Axis == Cintura indica que se acabo el proceso.
		//-----------------------------------------------------------------------------------------------------
		if(Axis==AxisWaist)
		{
			break;
		}
	}	
	


	

	
	for(Axis=0; Axis < DOF; Axis++) 
	{	
		PMDGetActualPosition(&hAxis[Axis],&encoder);
		
		#ifdef DEBUGROBOTICS
			printf("go_home=> Posicion Encoder despues del proceso de home del Eje %s es= %ld\n",AxisName(Axis),encoder);		
			PMDGetPositionError(&hAxis[Axis], &readerror);
			printf("go_home=> Error de posicion del Eje %s despues del proceso de home = %ld\n",AxisName(Axis),readerror);		
		#endif
		
		PMDAdjustActualPosition(&hAxis[Axis],-encoder); 		
		PMDGetPositionError(&hAxis[Axis], &readerror);
		printf("go_home=> Error de posicion del Eje %s al finalizar el proceso de HOME = %ld\n",AxisName(Axis),readerror);		
	}
	//habilitada las interrupciones hacia el uControlador
	
	HostInterruptEnable();
	
}

/**********************************************************************************************************************/
void No_limit_switch(int Ini_Axis)
{
	PMDuint16 Axis,Limit_switch;
	
	//No se toma en cuenta roll ya que carese de sentido
	if(Ini_Axis!=AxisRoll)
	{
		for(Axis=(Ini_Axis);Axis<DOF;Axis++)
		{
			PMDGetSignalStatus(&hAxis[Axis],&Limit_switch); 
			printf("No_limit_switch=> sensando de limites eje %s  en No_limit_switch = 0x%x\n ",AxisName(Axis),Limit_switch);
			if(!((Limit_switch & PMDSignalPositiveLimitMask)!=0))//El límite positivo está activo (presionado).
			{
				if(Axis!=AxisRoll)
				{
					printf("No_limit_switch=> Limites Positivo activado por el  eje %s\n ",AxisName(Axis));
					Positivehandling(Axis);	
				}
				else
				{	
					//Tiene que estar desconectado o hubo un error con el movimiento de pitch, pues pitch tiene los
					//mismo interuptores.
					printf("No_limit_switch=> Cable #2 de Sensores esta DESCONECTADO! limite positivo activado \n");
				}
			}		
			if(!((Limit_switch & PMDSignalNegativeLimitMask)!=0))
			{
				if(Axis!=AxisRoll)
				{
					printf("No_limit_switch=> Limites Negativo activado por el  eje %s\n ",AxisName(Axis));
					Negativehandling(Axis);	
				}
				else
				{
					//Tiene que estar desconectado o hubo un error con el movimiento de pitch, pues pitch tiene los
					//mismo interuptores.
					printf("No_limit_switch=> Cable #2 de Sensores esta DESCONECTADO! limite negativo activado \n");
				}
				
			}
		}
	}
	
	printf("No_limit_switch=> END! \n");
} 
/**********************************************************************************************************************/
PMDresult Status_Axis(tAxis Axis) 
{

	uint16 status;
	
	
	PMDGetActivityStatus(&hAxis[Axis],&status);
	
	if ((status & PMDActivityInMotionMask) !=0)  
	{
		return rcPMDProfileactivo;
	}
	else
	{	
		if(!PMDcheckIdle(Axis2AxisMask(Axis)))
		{
			/*Hay correcion de errores*/
			return rcPMDAxisPresicionError;
		}
		/*Movimiento completado con presicion ya sea con o sin encoderes*/
		return rcPMDAxisPresicionMotionComplete;
	}
}
 
/**********************************************************************************************************************/
void SetPosicionActual(tAxis Axis, PMDint32 AJUSTE)
{
	if(AJUSTE >=  0)								
	{
		
		PMDSetActualPosition(&hAxis[Axis],AJUSTE);
	}
	else //Valor a cargar es negativo.
	{
		//Cargar el registro de posición actual con cero.
		PMDSetActualPosition(&hAxis[Axis],0);
		
		//Ajustar el registro de posición actual con el valor de AJUSTE.
		PMDAdjustActualPosition(&hAxis[Axis],AJUSTE);		
		
	}
	
	
}

/**********************************************************************************************************************/
void AjustarPosicionActual(tAxis Axis, PMDint32 AJUSTE)
{
	PMDint32 registro_posicion, registro_encoder, AJUSTE_MODIFICADO;
	
	PMDGetPosition(&hAxis[Axis], &registro_posicion);
	//Se halla el nuevo valor de AJUSTE.
	PMDGetActualPosition(&hAxis[Axis],&registro_encoder);
	
	if(registro_posicion >  0)	  //no pasa nada							
	{
		
		//Ajustar el registro de posición actual con el valor de AJUSTE.
		PMDAdjustActualPosition(&hAxis[Axis],AJUSTE);
	}
	else //El movimiento anterior fué negativo.
	{
		//Cargar el registro de posición actual con cero.
		PMDSetActualPosition(&hAxis[Axis],0);
		
		
		AJUSTE_MODIFICADO = AJUSTE + registro_encoder;
		
		
		//Ajustar el registro de posición actual con el valor de AJUSTE_MODIFICADO.
		PMDAdjustActualPosition(&hAxis[Axis],AJUSTE_MODIFICADO);		
		
	}
	
	
}

/* Con deteccion de errores


	do
	{
		//Cintura desligada
		if(Status_Axis(AxisWaist)==rcPMDAxisPresicionError)
		{
			//Realizo correccion
			if(!activada_correcion_AxisWaist)
			{
				activada_correcion_AxisWaist=1;
				#ifdef DEBUGROBOTICS
				printf("Error de presicion en el %s \n",AxisName(AxisWaist));	
				#endif
			}
		}		
		//Hombro - codo - pich ligados
		if(Status_Axis(AxisShoulder)==rcPMDAxisPresicionError)
		{
			//Realizo correccion
			if(!activada_correcion_AxisShoulder)
			{
				activada_correcion_AxisShoulder=1;
				#ifdef DEBUGROBOTICS
				printf("Error de presicion en el %s \n",AxisName(AxisShoulder));	
				#endif
			}
		}
		//Complete el movimiento con presicion
		if(Status_Axis(AxisShoulder)==rcPMDAxisPresicionMotionComplete)
		{
			//El hombro no se esta moviendo, ya me puedo mover a la velocidad escogida 
			//por el usuario para codo.
			if(reconf_Elbow==0)
			{
				#ifdef DEBUGROBOTICS
				printf("Reconfigurando Codo\n");
				printf("Velocidad codo usuario = %ld\n",velocity_elbow_usr);	
				printf("Aceleracion codo usuario = %ld\n",acceleration_elbow_usr);	
				#endif
				reconf_Elbow=1;
				PMDSetVelocity(&hAxis[AxisElbow],velocity_elbow_usr); 
				PMDSetAcceleration(&hAxis[AxisElbow],acceleration_elbow_usr); 
				PMDUpdate(&hAxis[AxisElbow]);
			}
			if(Status_Axis(AxisElbow)==rcPMDAxisPresicionError)
			{
				//Realizo correccion
				if(!activada_correcion_AxisElbow)
				{
					activada_correcion_AxisElbow=1;
					#ifdef DEBUGROBOTICS
					printf("Error de presicion en el %s \n",AxisName(AxisElbow));	
					#endif
				}
			}
			if(Status_Axis(AxisElbow)==rcPMDAxisPresicionMotionComplete)
			{
				//El codo no se esta moviendo, ya me puedo mover a la velocidad escogida 
				//por el usuario para pitch.
				if(reconf_Pitch==0)
				{
					#ifdef DEBUGROBOTICS
					printf("Reconfigurando Pitch\n");
					printf("Velocidad Pitch usuario = %ld\n",velocity_pich_usr);	
					printf("Aceleracion Pitch usuario = %ld\n",acceleration_pich_usr);							printf("Posicion final de  Pitch= %ld\n",TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position);	
					#endif
					reconf_Pitch=1;
					//No puede usar la funcion pitch porque se reinicia los encoderes..
					mov_axis(AxisPitch, TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position, 					velocity_pich_usr, acceleration_pich_usr, update);
					mov_axis(AxisRoll, -TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position,
					(velocity_pich_usr), acceleration_pich_usr, update);
				}
				if(Status_Axis(AxisPitch)==rcPMDAxisPresicionError)
				{
					//Realizo correccion
					//En este momento deberia de activarse motion complete y salir de loop
					if(!activada_correcion_AxisPitch)
					{
						activada_correcion_AxisPitch=1;
						#ifdef DEBUGROBOTICS
						printf("Error de presicion en el %s \n",AxisName(AxisPitch));	
						#endif
					}
				}
				
			}
		}
	}	
	//Se toma en cuenta todos los ejes nenos el axis roll
	while( PMDcheckIdle(mask & ~Axis2AxisMask(AxisRoll))==FALSE);


*/






/*Sin deteccion de errores
	do
	{
		//Hallo el El registro de actividad en el hombro
		PMDGetActivityStatus(&hAxis[AxisShoulder],&status);
		//Si hombro se mueve
		if ((status & PMDActivityInMotionMask) !=0)  
		{
			//tengo que mover el codo con la misma velocidad para no chocar	
		}
		else
		{	//Ya me puedo mover a la velocidad escogida por el usuario.
			if(reconf_Elbow==0)
			{
				#ifdef DEBUGROBOTICS
				printf("Reconfigurando Codo\n");
				printf("Velocidad codo usuario = %ld\n",velocity_elbow_usr);	
				printf("Aceleracion codo usuario = %ld\n",acceleration_elbow_usr);	
				#endif
				reconf_Elbow=1;
				PMDSetVelocity(&hAxis[AxisElbow],velocity_elbow_usr); 
				PMDSetAcceleration(&hAxis[AxisElbow],acceleration_elbow_usr); 
				PMDUpdate(&hAxis[AxisElbow]);
			}
			//Hallo el El registro de actividad en el codo
			PMDGetActivityStatus(&hAxis[AxisElbow],&status);
			//Si codo se mueve
			if ((status & PMDActivityInMotionMask) !=0)
			{
				//tengo que mover el pitch con la misma velocidad para no chocar	
			}
			else
			{
				//Ya me puedo mover a la velocidad escogida por el usuario.
				if(reconf_Pitch==0)
				{
					reconf_Pitch=1;
					if(velocity_pich_usr>0)
					{
						#ifdef DEBUGROBOTICS
						printf("Reconfigurando Pitch\n");
						printf("Velocidad Pitch usuario = %ld\n",velocity_pich_usr);	
						printf("Aceleracion Pitch usuario = %ld\n",acceleration_pich_usr);	
						printf("Posicion final de  Pitch= %ld\n",TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position);	
						#endif
						//No puede usar la funcion pitch porque se reinicia los encoderes..
						mov_axis(AxisPitch, TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position, 
						velocity_pich_usr, acceleration_pich_usr, update);
						mov_axis(AxisRoll, -TrajectoryPOINT->point[pointIndex].axis[AxisPitch].position,
						(velocity_pich_usr), acceleration_pich_usr, update);
					}
				}
			}
		}
		
	}	
	while( PMDcheckIdle(mask & ~Axis2AxisMask(AxisRoll))==FALSE);



*/











/*

00330 void Motion_error(PMDAxisHandle *axis_handle)
00331 {
00332       PMDAxisHandle hAxis0;
00333       PMDint32 Error, ActualPosition, Position, pos;
00334       PMDuint32 ErrorLimit;
00335       PMDint32 *temp, temp1, *temp2, temp3, *temp4, temp5;
00336 
00337         PMDGetPositionError(axis_handle, &Error);
00338         PMDGetPositionErrorLimit(axis_handle, &ErrorLimit);
00339         PMDGetActualPosition(axis_handle, &ActualPosition);
00340         PMDGetPosition(axis_handle, &Position);
00341         temp= &Error;
00342         temp1= *temp;
00343         temp2= &Position;
00344         temp3= *temp2;
00345         temp4= &ActualPosition;
00346         temp5= *temp4;
00347         pos=temp5-temp3;
00348         PMDSetActualPosition (axis_handle, pos);
00349         PMDSetMotorMode(axis_handle, PMDMotorOn);
00350         PMDUpdate(axis_handle);
00351         PMDResetEventStatus(axis_handle, PMDEventMotionErrorMask);
00352         PMDClearInterrupt(axis_handle);
00353                   
00354         return;
00355 }











*/


/* //Home por interuptorres
		if(Axis==AxisShoulder)
		{
			//El motor de pitch toco interuptor.
			
			PMDGetSignalStatus(&hAxis[AxisPitch],&status_ligado); 
			printf("sensando pitch en search_home_negative= %x... \n ",status_ligado);
			if(!((status_ligado & PMDSignalPositiveLimitMask)!=0))
			{
				// Pare el motor de codo
				
				printf("Parando por colicion... \n ");
				//PMDSetStopMode(&hAxis[AxisShoulder], PMDSmoothStopMode ); 
				//PMDUpdate(&hAxis[AxisShoulder]);
				//Ejecuto movimiento de recuperacion en la articulacion pitch
				printf("Limite positivo pitch en search_home_negative para el eje %s. \n ",AxisName(Axis));
			    //Positivehandling(AxisPitch);	
				//Se rehabilita el movimiento en el Eje.
				//PMDSetMotorMode(&hAxis[AxisShoulder],PMDMotorOn);
				//mov_axis(AxisShoulder,none, -hAxis[AxisShoulder].axisSettings.homeVelocity,
				//hAxis[AxisShoulder].axisSettings.homeAcceleration, update);
				buzzer(1);
			}		
			if(!((status_ligado & PMDSignalNegativeLimitMask)!=0))
			{
				printf("Parando por colicion... \n ");
				//PMDSetStopMode(&hAxis[AxisShoulder], PMDSmoothStopMode ); 
				//PMDUpdate(&hAxis[AxisShoulder]);
				printf("Limite negativo pitch en search_home_negative para el eje %s. \n ",AxisName(Axis));
				//Negativehandling(AxisPitch);
				// Se rehabilita el movimiento en el Eje 
				//PMDSetMotorMode(&hAxis[AxisShoulder],PMDMotorOn);
				//mov_axis(AxisShoulder,none, -hAxis[AxisShoulder].axisSettings.homeVelocity,
				//hAxis[AxisShoulder].axisSettings.homeAcceleration, update);
				buzzer(1);
			}
			
			//El eje de codo toco interuptor
			PMDGetSignalStatus(&hAxis[AxisElbow],&status_ligado);
			printf("sensando codo en search_home_negative= %x... \n ",status_ligado);
			if(!((status_ligado & PMDSignalPositiveLimitMask)!=0))
			{
				printf("Parando por colicion... \n ");
				//PMDSetStopMode(&hAxis[AxisShoulder], PMDSmoothStopMode ); 
				//PMDUpdate(&hAxis[AxisShoulder]);
				printf("Limite positivo codo en search_home_negative para el eje %s. \n ",AxisName(Axis));
				//Positivehandling(AxisElbow);
				// Se rehabilita el movimiento en el Eje 
				//PMDSetMotorMode(&hAxis[AxisShoulder],PMDMotorOn);
				//mov_axis(AxisShoulder,none, -hAxis[AxisShoulder].axisSettings.homeVelocity,
				//hAxis[AxisShoulder].axisSettings.homeAcceleration, update);
				buzzer(1);
			}		
			if(!((status_ligado & PMDSignalNegativeLimitMask)!=0))
			{
				printf("Parando por colicion... \n ");
				//PMDSetStopMode(&hAxis[AxisShoulder], PMDSmoothStopMode ); 
				//PMDUpdate(&hAxis[AxisShoulder]);
				printf("Limite negativo codo en search_home_negative para el eje %s. \n ",AxisName(Axis));
				//Negativehandling(AxisElbow);	
				// Se rehabilita el movimiento en el Eje 
				//PMDSetMotorMode(&hAxis[AxisShoulder],PMDMotorOn);
				//mov_axis(AxisShoulder,none, -hAxis[AxisShoulder].axisSettings.homeVelocity,
				//hAxis[AxisShoulder].axisSettings.homeAcceleration, update);
				buzzer(1);
			}	
			
*/








/* Basura_esta_mal
void Mov_to_no_colicion(void)
{
	PMDint32 max_read, min_read,center_pos;
	//PMDint32 home1;
	
	PMDuint16 status;
	
	
	
	//MOVIENDO PITCH A NO COLICION
	
	PMDSetProfileMode(&hAxis[AxisPitch],  PMDVelocityContouringProfile );		
	PMDSetProfileMode(&hAxis[AxisRoll],  PMDVelocityContouringProfile );
	
	
	
	mov_axis(AxisPitch, none, -(hAxis[AxisPitch].axisSettings.homeVelocity), 
		hAxis[AxisPitch].axisSettings.homeAcceleration, update);
	mov_axis(AxisRoll,none, hAxis[AxisPitch].axisSettings.homeVelocity, 
		hAxis[AxisPitch].axisSettings.homeAcceleration, update);
	
	do 
	{ 
		PMDGetEventStatus(&hAxis[AxisPitch],&status);
	} 
	while ((status & PMDEventInNegativeLimitMask ) ==0);

	#ifdef DEBUGROBOTICS
		printf(" Llega a el limite negativo parando Motor... \n ");
	#endif
				
	//detener suavemente el movimiento
	PMDSetStopMode(&hAxis[AxisPitch], PMDSmoothStopMode ); 
	PMDUpdate(&hAxis[AxisPitch]);				
	PMDSetStopMode(&hAxis[AxisRoll], PMDSmoothStopMode ); 
	PMDUpdate(&hAxis[AxisRoll]);
					
	
	PMDResetEventStatus(&hAxis[AxisPitch],~(PMDEventMotionCompleteMask | PMDEventInNegativeLimitMask ));
	PMDSetMotorMode(&hAxis[AxisPitch],PMDMotorOn); 
	PMDResetEventStatus(&hAxis[AxisRoll],~(PMDEventMotionCompleteMask | PMDEventInPositiveLimitMask));
	PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn);

	PMDGetActualPosition(&hAxis[AxisPitch],&min_read);

	//Movimiento al positivo
	mov_axis(AxisPitch, none, hAxis[AxisPitch].axisSettings.homeVelocity, 
		hAxis[AxisPitch].axisSettings.homeAcceleration, update);
	mov_axis(AxisRoll,none, -(hAxis[AxisPitch].axisSettings.homeVelocity), 
		hAxis[AxisPitch].axisSettings.homeAcceleration, update);
	
	
	do 
	{ 
		PMDGetEventStatus(&hAxis[AxisPitch],&status);
	} 
	while ((status & PMDEventInPositiveLimitMask) ==0);

	#ifdef DEBUGROBOTICS
		printf(" Llega a el limite positivo parando Motor... \n ");
	#endif
	
	
	//detener suavemente el movimiento
	PMDSetStopMode(&hAxis[AxisPitch], PMDSmoothStopMode ); 
	PMDUpdate(&hAxis[AxisPitch]);				
	PMDSetStopMode(&hAxis[AxisRoll], PMDSmoothStopMode ); 
	PMDUpdate(&hAxis[AxisRoll]);
	
	PMDResetEventStatus(&hAxis[AxisPitch],~(PMDEventMotionCompleteMask | PMDEventInPositiveLimitMask));
	PMDSetMotorMode(&hAxis[AxisPitch],PMDMotorOn); 
	PMDResetEventStatus(&hAxis[AxisRoll],~(PMDEventMotionCompleteMask | PMDEventInNegativeLimitMask));
	PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn);
	
	PMDGetActualPosition(&hAxis[AxisPitch],&max_read);
	
	//Se cambia el perfil movimiento a perfil trapezoidal
	PMDSetProfileMode(&hAxis[AxisPitch], PMDTrapezoidalProfile);	
	PMDSetProfileMode(&hAxis[AxisRoll], PMDTrapezoidalProfile);	
	

	
	printf("min_read = %ld \n",min_read);
	printf("max_read = %ld \n",max_read);
	center_pos = (max_read + min_read)/2;
	
	printf("center_pos = %ld \n",center_pos);
	//Movimiento hacia el centro
	mov_axis(AxisPitch, center_pos, 
	hAxis[AxisPitch].axisSettings.homeVelocity, 
	hAxis[AxisPitch].axisSettings.homeAcceleration, update);
	mov_axis(AxisRoll, -center_pos,
	hAxis[AxisPitch].axisSettings.homeVelocity,
	hAxis[AxisPitch].axisSettings.homeAcceleration, update);
	
	//No sirve choca con la base  cuando esta en origen..
	
	search_home_positive(AxisElbow,&home1,&max_read,TRUE);
	
	PMDAdjustActualPosition(&hAxis[AxisElbow],-home1); 
	
	mov_axis(AxisElbow,-142709,
	hAxis[AxisElbow].axisSettings.homeVelocity,
	hAxis[AxisElbow].axisSettings.homeAcceleration, update);
	


}*/


/*
void PMDprocessTrajectory(tTrajectory* Trajectory, int16 Naxis,int16 TraceOP)
{
	int16 axis,pointIndex=0;
	PMDuint16 status;
	int16 mask;
	static int16 updateMask = 0; 
	PMDint32 position_comanded_shoulder,position_Elbow, velocity_Elbow;
	
	
    mask= Trajectory->invalidmask;
	
	#ifdef DEBUGROBOTICS
	printf("El Numero de puntos de la trayectoria es en PMDfunctions.c => %d\n",Trajectory->size);
	printf("invalid mask en PMDfunctions.c => %X\n",Trajectory->invalidmask);
	#endif
	do
	{   
		if(updateMask==0) //No he configurado el updatemask o ya se ha procesado un punto
		{   
		    //Configuro updatemask
			for(axis=0; axis < Naxis; axis++) 
			{ 
			    if((hAxis[axis].axisSettings.axisEnabled == TRUE) && 		
					((mask & (Axis2AxisMask(axis))) >0)) 
					{
				       // PMDClearPositionError(&hAxis[axis]); //Poner los encoder a cero	
				        if(hAxis[axis].axisSettings.axisEnabled == TRUE) 
				        updateMask = updateMask | (1<<axis);	 
				    }
			}
			//Cargo la posicion, velocidad, aceleracion con pointIndex
			for(axis=0; axis < Naxis; axis++) 
			{
				if((hAxis[axis].axisSettings.axisEnabled == TRUE) && 		
					((mask & (Axis2AxisMask(axis))) >0)) 
				{						
					switch (axis)
					{
					    case (AxisPitch):
						break;
						case (AxisRoll):
						break;								
						default:
							mov_axis(axis,
							Trajectory->point[pointIndex].axis[axis].position,
							Trajectory->point[pointIndex].axis[axis].velocity,
						    Trajectory->point[pointIndex].axis[axis].acceleration,
							no_update);
							break;
						}						
					}
				} 	
				//Obtengo la velocidad del hombro para mover propocionalmente el codo
				PMDGetVelocity(&hAxis[AxisShoulder],&velocity_Elbow); 
				velocity_Elbow=velocity_Elbow*0.21;
				#ifdef DEBUGROBOTICS
				printf("velocidad codo %ld\n",velocity_Elbow);	
				printf("Velocidad cintura en PMDprocessTrajectory => %ld\n",
				   Trajectory->point[pointIndex].axis[AxisWaist].velocity);
				#endif
				//scanf("%d",&stop);
			//Cargo el breakpoint con mismo punto hacia donde me voy a mover 
			for(axis=0; axis < Naxis; axis++)
			{ 
				if((hAxis[axis].axisSettings.axisEnabled == TRUE) && 		
					((mask & (Axis2AxisMask(axis))) >0)) 
				{   
					if(axis==AxisElbow)
						PMDSetBreakpointValue(&hAxis[axis],PMDBreakpoint2,
						(Trajectory->point[pointIndex].axis[axis].position+(Trajectory->point[pointIndex].axis[AxisShoulder].position)*0.21));
					else
						PMDSetBreakpointValue(&hAxis[axis],PMDBreakpoint2,Trajectory->point[pointIndex].axis[axis].position);
						//No puedo Hacer un Update porque tengo que esperar a que todas
						//Las articulaciones terminen su movimiento 
						//asi que simplemente no hay que hacer nada y 
						//el breakpoint sirve para indicarle a el programa que le eje
						//en cuestion ha llegado a el punto	
					PMDSetBreakpoint(&hAxis[axis],PMDBreakpoint2,hAxis[axis].axis,PMDBreakpointNoAction,PMDBreakpointCommandedPositionCrossed);
					//PMDSetBreakpoint(&hAxis[axis],PMDBreakpoint2,hAxis[axis].axis,PMDBreakpointNoAction,PMDBreakpointGreaterOrEqualCommandedPosition);
				}
			}
			//Hago Update de todos los ejes al mismo tiempo
			switch (Naxis)
			{
					case (1): //Cintura
					    PMDUpdate(&hAxis[0]);
					    break;
					case (2): //Cintura y Hombro
					    PMDMultiUpdate(&hAxis[0],PMDAxis1Mask|PMDAxis2Mask);    // move the robot (all axis in parallel)					};
					    break;
				    case (3):	
					    PMDMultiUpdate(&hAxis[0],PMDAxis1Mask|PMDAxis2Mask|PMDAxis3Mask);    // move the robot (all axis in parallel)					};
					    break;
					case (4):
					    PMDMultiUpdate(&hAxis[0],PMDAxis1Mask|PMDAxis2Mask|PMDAxis3Mask|PMDAxis4Mask);    // move the robot (all axis in parallel)					};;
					    break;			
		    }
		}
		//Estoy procesando un punto y alguna articulacion falta por completar el movimiento 
		else 
		{  
		    //Hago correcion de Hombro_Codo y  busco los breakpoint 
			//Si encuentro un breakpoint pongo a cero su respectiva casilla en updatemask
			//Ej solo el eje cintura llego al el breakpoint, updatemask=11110
			for(axis=0; axis < Naxis; axis++) 
			{    
				if((hAxis[axis].axisSettings.axisEnabled == TRUE) && 		
					((mask & (Axis2AxisMask(axis))) >0)) 
				{
					PMDGetCommandedPosition(&hAxis[AxisShoulder],&position_comanded_shoulder);
					position_Elbow=(Trajectory->point[pointIndex].axis[AxisElbow].position)+(position_comanded_shoulder*0.21);		
					mov_axis(AxisElbow, position_Elbow, Trajectory->point[pointIndex].axis[axis].velocity,Trajectory->point[pointIndex].axis[axis].acceleration,update);
					//mov_axis(AxisElbow, position_Elbow,velocity_Elbow,100,update);
					PMDGetEventStatus(&hAxis[axis],&status);
					if((status & PMDEventBreakpoint2Mask) != 0) 
					{					    
						printf(" BreakPonint encontrado en eje %s\n",AxisName((tReturnCode)axis));
						updateMask = updateMask & ~(1<<axis);
						PMDResetEventStatus(&hAxis[axis],~PMDEventBreakpoint2Mask); // clear just this event
						PMDResetEventStatus(&hAxis[axis],~ PMDEventMotionCompleteMask);	
					}
				}
			} // for
			// this move is completed, we can proced to the next point
			if(updateMask==0) 
			{   printf("Procesado el  punto %d con los siguientes Datos:\n",pointIndex);
			    #ifdef DEBUGROBOTICS
				printf("La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisWaist),
				   Trajectory->point[pointIndex].axis[AxisWaist].position);
				printf("La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisShoulder),
				   Trajectory->point[pointIndex].axis[AxisShoulder].position);
				printf("La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisElbow),
				   Trajectory->point[pointIndex].axis[AxisElbow].position);
				printf("La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisPitch),
				   Trajectory->point[pointIndex].axis[AxisPitch].position);
				printf("La posicion de %s es en PMDprocessTrajectoryProcesse => %ld\n",AxisName(AxisRoll),
				   Trajectory->point[pointIndex].axis[AxisRoll].position);
				//scanf("%d",&stop);
				#endif
				pointIndex++;				
			}
		} // if else updatemask
	}
	while (pointIndex <  (Trajectory->size)); //pointIndex es la variable que marca el punto en ejecucion 
    printf("Saliendo de PMDprocessTrajectory\n");
	He configurado trazar una trajectoria
	if(TraceOP==1)
	{
		//Parao el trazado para PMD1
		PMDSetTraceStop(&hAxis[AxisWaist],AxisWaist,PMDTraceConditionImmediate,0,0);
		//PMDSetTraceStop(&hAxis[axis], axis,PMDTraceConditionImmediate,0,0);
	}

	//Muevo roll
} // PMDprocessTrajectory
*/




//Pitch v1

/*

void pitch(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration,
					PMDint16 actualizar)
{
   
    PMDint32 Mov_Pitch_Anterior_RAM, posicion_actual_motor_pitch, 
	posicion_actual_motor_roll,temp;
   
	//habilitando eventos de limite para realizar movimientos de PiTch
	PMDSetLimitSwitchMode(&hAxis[AxisRoll],PMDLimitEnabled);
	PMDSetLimitSwitchMode(&hAxis[AxisPitch],PMDLimitEnabled);
	
	
	if(position!=INF && position!=-INF )	
	{
		if(position!=none)
		{
			//Estoy ejecutando un movimiento trapezoidal.
			// Leer el movimiento anterior de Pitch que esta almacenado en EEPROM
			eeprom_read_block((void*)&Mov_Pitch_Anterior_RAM,(const void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
			#ifdef DEBUGROBOTICS
			printf("Movimiento Anterior de pitch EEPROM de %s = %ld\n",AxisName(AxisPitch),Mov_Pitch_Anterior_RAM);
			#endif
			//Si no me estoy moviendo hacia el HOME por primera vez.
			if(HomePitch_process_active==0)
			{
				//Es un movimiento normal se debe de actualizar la eeprom.
				eeprom_write_block((const void*)&position,(void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
				#ifdef DEBUGROBOTICS
				printf("Actualizando ActualPosition motor de  PITCH y ROLL en pitch();! \n");
				#endif
				PMDSetActualPosition(&hAxis[AxisPitch],Mov_Pitch_Anterior_RAM);
				PMDSetActualPosition(&hAxis[AxisRoll],-(Mov_Pitch_Anterior_RAM));
			}
			//He finalizado el proceso de Home ó es un moviento normal de pitch.
			HomePitch_process_active=0;
		}
		
	}
	else
	{
		//Si Estoy iniciando el proceso de Home
		if(HomePitch_process_active==0)
		{
			//Debo reinicar el movimiento anterior a cero. Para evitar descuadre del encoder cuando: 
			//-Hago HOME luego me muevo hacia una posicion X y luego hago home.
			//-Apago el controlador y se mueve por error los motores de pitch y roll.
			#ifdef DEBUGROBOTICS
				printf("Iniciando variable Mov_Pitch_Anterior a Cero.! \n");
			#endif
			temp=0;
			eeprom_write_block((const void*)&temp,(void*)&Mov_Pitch_Anterior,sizeof(PMDint32));
		}
		//He iniciado o estoy en el proceso de HOME
		HomePitch_process_active=1;
		
	}
	
	//Realizo el Movimiento en el motor de Pitch 
	#ifdef DEBUGROBOTICS
	PMDGetActualPosition(&hAxis[AxisPitch],&posicion_actual_motor_pitch);
	PMDGetActualPosition(&hAxis[AxisRoll],&posicion_actual_motor_roll);
	printf("Posicion encoder actual del motor de pitch %ld\n",posicion_actual_motor_pitch);
	printf("Posicion encoder actual del motor de roll %ld\n",posicion_actual_motor_roll);
	#endif
	
	
	
	mov_axis(AxisPitch, position, velocity, acceleration, actualizar);
	//none es utilizado cuando se utiliza un perfil de velocidad constante.
	if (position == none)
	{
		
		//Realizo el Movimiento en el motor de roll
		mov_axis(AxisRoll, position, -(velocity), acceleration, actualizar);
	}
	else
	{
		//Realizo el Movimiento en el motor de roll 
		mov_axis(AxisRoll, -position, (velocity), acceleration, actualizar);
		
		
	}
	
	
	
}*/



//Roll V1

/*
void roll(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration,
					PMDint16 actualizar,PMDint16 modo_movimento)
{
    PMDint32 Mov_Roll_Anterior_RAM,temp, posicion_actual_motor_pitch,
	posicion_actual_motor_roll;
	
	//deshabilitando eventos de limite porque Roll no tiene interruptores 
	de fin de carrera
	PMDSetLimitSwitchMode(&hAxis[AxisRoll],PMDLimitDisabled);
	PMDSetLimitSwitchMode(&hAxis[AxisPitch],PMDLimitDisabled);
	
	
	
	
	if(modo_movimento==MOVIMIENTO_NORMAL_ROLL)	
	{
		//Para roll siempre estoy ejecutando un movimiento trapezoidal.
		// Leer el movimiento anterior de Roll que esta almacenado en EEPROM
		eeprom_read_block((void*)&Mov_Roll_Anterior_RAM,(const void*)&Mov_Roll_Anterior,sizeof(PMDint32));
		#ifdef DEBUGROBOTICS
			printf("Movimiento de roll Anterior EEPROM de %s = %ld\n",AxisName(AxisRoll),Mov_Roll_Anterior_RAM);
		#endif
			//Es un movimiento normal se debe de actualizar la posicion eeprom.
			eeprom_write_block((const void*)&position,(void*)&Mov_Roll_Anterior,sizeof(PMDint32));
			#ifdef DEBUGROBOTICS
				printf("Actualizando ActualPosition motor de  PITCH y ROLL en roll();! \n");
			#endif	
			// Carga el valor de movimiento anterior de Roll a los encoders de los motores de pitch y roll
			PMDSetActualPosition(&hAxis[AxisPitch],Mov_Roll_Anterior_RAM);
			PMDSetActualPosition(&hAxis[AxisRoll],Mov_Roll_Anterior_RAM);
	}
	else
	{
		//Si Estoy iniciando el proceso de Home
		if(HomeRoll_process_active==0)
		{
			//Debo reinicar el movimiento anterior a cero. Para evitar descuadre del encoder cuando: 
			//-Hago HOME luego me muevo hacia una posicion X y luego hago home.
			//-Apago el controlador y se mueve por error el los motores de pitch y roll.
			temp=0;
			eeprom_write_block((const void*)&temp,(void*)&Mov_Roll_Anterior,sizeof(PMDint32));
		}
		//He iniciado o estoy en el proceso de HOME
		HomeRoll_process_active=1;
	}
	
	#ifdef DEBUGROBOTICS
	PMDGetActualPosition(&hAxis[AxisPitch],&posicion_actual_motor_pitch);
	PMDGetActualPosition(&hAxis[AxisRoll],&posicion_actual_motor_roll);
	printf("Posicion encoder actual del motor de pitch %ld\n",posicion_actual_motor_pitch);
	printf("Posicion encoder actual del motor de roll %ld\n",posicion_actual_motor_roll);
	#endif
	
		
	//Realizo el Movimiento de Roll
	mov_axis(AxisPitch, position, velocity, acceleration, actualizar);
	mov_axis(AxisRoll, position, (velocity), acceleration, actualizar);	
}*/
