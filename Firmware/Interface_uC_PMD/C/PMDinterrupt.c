#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "c-motion.h"
#include "PMDsetup.h"
#include "PMDrobotics.h"
#include "messages.h"
#include "robotData.h"
#include "debug.h"
#include "packetIO.h"

//#define DEBUGINTERRUPT


extern PMDAxisHandle hAxis[DOF]; 



/*******************************************************************************************/
void HostInterruptDisable(void)
{
	int16 axis;
	
	for(axis=0; axis < DOF; axis++)
	{
		if((hAxis[axis].axisSettings.axisEnabled == TRUE) &&
			(hAxis[axis].axisSettings.interruptEnable==TRUE))
		{
			PMDSetInterruptMask(&hAxis[axis],0);
		}
	}
} 
/*****************************************************************************************/
void HostInterruptEnable(void)
{
	int16 axis;
	
	for(axis=0; axis < DOF; axis++)
	{
		if((hAxis[axis].axisSettings.axisEnabled == TRUE) &&
			(hAxis[axis].axisSettings.interruptEnable==TRUE))
		{
			PMDSetInterruptMask(&hAxis[axis],	PMDEventMotionErrorMask		|
												PMDEventInPositiveLimitMask | 
												PMDEventInNegativeLimitMask	|
												PMDEventInstructionErrorMask);
		}
	}
}

/*****************************************************************************************/
//Funcion NO UTILIZADA
void MotionCompletehandling(tAxis Axis)
{
	#ifdef DEBUGINTERRUPT
	printf("MotionCompletehandling=> El eje de %s ha completado el Movimiento \n", AxisName(Axis));
	#endif
	/*Se limpia el Event Status Register, borrando la interrupcion
	should clear the motion complete bit, enabling the bit to indicate 
	the end of motion for the next move*/	
	PMDResetEventStatus(&hAxis[Axis],~PMDEventMotionCompleteMask);
	
	if(Axis==AxisPitch)
	{	
		PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventMotionCompleteMask);		
	}
	
	if(Axis==AxisRoll)
	{	
		PMDResetEventStatus(&hAxis[AxisPitch],~PMDEventMotionCompleteMask);		
	}
	
}

/*****************************************************************************************/
void MotionErrorhandling(tAxis Axis)
{
	PMDuint16 mode;
	
	/* Imprimir mensajes de Error*/	
	printf("MotionErrorhandling=> El eje de %s Produjo un Error de Movimiento \n", AxisName(Axis));
	fprintf_P(stderr,PSTR(" Error de Movimiento \n"));
	/* se enciende el LED Rojo*/
	Led_on(Rojo);
	
	
	/*Se limpia el Event Status Register, borrando la interrupcion 	*/	
	PMDResetEventStatus(&hAxis[Axis],~PMDEventMotionErrorMask);
	
	// reactivar el movimiento dependiendo de si existe parada automatica
	PMDGetAutoStopMode(&hAxis[Axis], &mode);
			
	if (mode==PMDEnable)
	{
		PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);
		if(Axis==AxisPitch)
		{				
			PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn); 		
		}		
	}
	
}

/*****************************************************************************************/
void Positivehandling(tAxis Axis)
{
	PMDuint16  signal_status, Profile, Profile_roll, mask; 
	
	/* Imprimir mensajes de Error*/	
	#ifdef DEBUGINTERRUPT
	printf("Positivehandling=> El eje de %s ha llegado al Limite Positivo \n", AxisName(Axis));
	#endif
	
	//fprintf_P(stderr,PSTR("Se produjo un Evento de Limite Positivo \n"));
	
	/*Salvar el Perfil de Velocidad para no alterarlo.*/
	PMDGetProfileMode(&hAxis[Axis],&Profile);	
	
	/*Se limpia el Event Status Register, borrando la interrupcion 
	The limit switch bit(s) in the event status register should be cleared
	by issuing the ResetEventStatus command. No motion is possible in any 
	direction while either of the limit switch bits in the event status register
	is set.*/
	
	PMDResetEventStatus(&hAxis[Axis],~PMDEventInPositiveLimitMask);		
		
	// Mover el eje en la direccion opuesta del Límite Positivo.
		
	switch (Axis)
	{
		case (AxisPitch):
		
			/*Si se produce un evento de limite positivo en el eje de pitch
			entonces el eje roll producira un evento de Limite Negativo.*/
			PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventInNegativeLimitMask);	
				
			/*Se realiza un movimiento de pitch en la direccion negativa*/
			PMDGetProfileMode(&hAxis[AxisRoll],&Profile_roll);
			PMDSetProfileMode(&hAxis[Axis],  PMDVelocityContouringProfile ); 
			PMDSetProfileMode(&hAxis[AxisRoll],  PMDVelocityContouringProfile ); 
			PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);  	
			PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn); 
			pitch(none,-hAxis[Axis].axisSettings.homeVelocity,
				hAxis[Axis].axisSettings.homeAcceleration, update,MOVIMIENTO_NORMAL );					
		break;
		
		case (AxisRoll):
		break;
			
		default:				
			PMDSetProfileMode(&hAxis[Axis],  PMDVelocityContouringProfile ); 
			PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);  				
			mov_axis(Axis,none, -hAxis[Axis].axisSettings.homeVelocity,
				hAxis[Axis].axisSettings.homeAcceleration, update);		
		break;
	}		
		
	PMDGetSignalStatus(&hAxis[Axis], &signal_status);
		
	do 
	{
		PMDGetSignalStatus(&hAxis[Axis], &signal_status);
		mask=signal_status& PMDSignalPositiveLimitMask;
		#ifdef DEBUGINTERRUPT
			printf("Positivehandling=> El Estado de la senal del Switch Positivo del eje %s es : %#X \n ",
					AxisName(Axis),mask);
		#endif
			
	} 
	while ((signal_status & PMDSignalPositiveLimitMask)==0);
	

	//---------------- Detener suavemente el movimiento.	
	
	PMDSetStopMode(&hAxis[Axis], PMDSmoothStopMode ); 
	PMDUpdate(&hAxis[Axis]);
	
	if(Axis==AxisPitch)
	{
		PMDSetStopMode(&hAxis[AxisRoll], PMDSmoothStopMode ); 
		PMDUpdate(&hAxis[AxisRoll]);
		PMDSetProfileMode(&hAxis[AxisRoll],Profile_roll);
		PMDUpdate(&hAxis[AxisRoll]);
		
	}	
	
	//Se restaura el Valor inicial del Perfil de la trayectoria.
	PMDSetProfileMode(&hAxis[Axis],Profile);
	PMDUpdate(&hAxis[Axis]);	
	
}

/*****************************************************************************************/
void Negativehandling(tAxis Axis)
{
	PMDuint16  signal_status, Profile, Profile_roll, mask; 
	
	/* Imprimir mensajes de Error*/
	//#ifdef DEBUGINTERRUPT
	printf("Negativehandling=> El eje de %s ha llegado al Limite Negativo \n", AxisName(Axis));
	//#endif
	//fprintf_P(stderr,PSTR("Se produjo un Evento de Limite Negativo \n"));


	/*Salvar el Perfil de Velocidad para no alterarlo.*/
	PMDGetProfileMode(&hAxis[Axis],&Profile);
	
	
	/*Se limpia el Event Status Register, borrando la interrupcion 
	The limit switch bit(s) in the event status register should be cleared
	by issuing the ResetEventStatus command. No motion is possible in any 
	direction while either of the limit switch bits in the event status register
	is set.*/
	
	PMDResetEventStatus(&hAxis[Axis],~PMDEventInNegativeLimitMask);		
		
	//-----Mover el eje en la direccion opuesta del Límite Negativo.
		
	switch (Axis)
	{
		case (AxisPitch):
				
			/*Si se produce un evento de limite negativo en el eje de pitch
			entonces el eje roll producira un evento de Limite Positivo.*/				
			PMDResetEventStatus(&hAxis[AxisRoll],~PMDEventInPositiveLimitMask);	
				
			/*Se realiza un movimiento de pitch en la direccion positiva*/			
			PMDGetProfileMode(&hAxis[AxisRoll],&Profile_roll);
			PMDSetProfileMode(&hAxis[Axis],  PMDVelocityContouringProfile ); 
			PMDSetProfileMode(&hAxis[AxisRoll],  PMDVelocityContouringProfile ); 
			PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);  	
			PMDSetMotorMode(&hAxis[AxisRoll],PMDMotorOn); 
			pitch(none,hAxis[Axis].axisSettings.homeVelocity,
				hAxis[Axis].axisSettings.homeAcceleration, update,MOVIMIENTO_NORMAL );					
		break;
		case (AxisRoll):		
		break;
			
		default:
			PMDSetProfileMode(&hAxis[Axis],  PMDVelocityContouringProfile ); 
			PMDSetMotorMode(&hAxis[Axis],PMDMotorOn);  				
			mov_axis(Axis,none,hAxis[Axis].axisSettings.homeVelocity,
				hAxis[Axis].axisSettings.homeAcceleration, update);	
		break;
	}		

	
	PMDGetSignalStatus(&hAxis[Axis], &signal_status);
	
	do 
	{
		PMDGetSignalStatus(&hAxis[Axis], &signal_status);
		mask=signal_status& PMDSignalNegativeLimitMask;
		//#ifdef DEBUGINTERRUPT
			printf("Negativehandling=> El Estado de la senal del Switch Negativo del eje %s es : %#X \n ",
					AxisName(Axis),mask);
		//#endif
	} 
	while ((signal_status & PMDSignalNegativeLimitMask)==0);



	//---------------- Detener suavemente el movimiento.------------------------------	
	
	PMDSetStopMode(&hAxis[Axis], PMDSmoothStopMode ); 
	PMDUpdate(&hAxis[Axis]);
	
	if(Axis==AxisPitch)
	{
		PMDSetStopMode(&hAxis[AxisRoll], PMDSmoothStopMode ); 	//detiene el Motor de Roll
		PMDUpdate(&hAxis[AxisRoll]);
		PMDSetProfileMode(&hAxis[AxisRoll],Profile_roll);			//restaura el perfil de moviemiento de Roll
		PMDUpdate(&hAxis[AxisRoll]);
		
	}

	//Se restaura el Valor inicial del Perfil de la trayectoria.
	PMDSetProfileMode(&hAxis[Axis],Profile);
	PMDUpdate(&hAxis[Axis]);	
	
}

/*****************************************************************************************/
void InstructionErrorhandling(tAxis Axis)
{
	
	
	/* Imprimir mensajes de Error*/	
	printf("Negativehandling=> El eje de %s Produjo un Error de Instruccion \n", AxisName(Axis));
	fprintf_P(stderr,PSTR("Error de Instruccion \n"));
	/* se enciende el LED Rojo*/
	Led_on(Rojo);
	
	
	
	/*Se limpia el Event Status Register, borrando la interrupcion 	*/	
	PMDResetEventStatus(&hAxis[Axis],~PMDEventInstructionErrorMask);
	
	
	
}

/*****************************************************************************************/
void interruptAxisHandling(tAxis Axis)
{
	PMDuint16 status;
	
	
	/* ----Verificar La causa de la Interrupcion a traves de Event Status REgister*/
	
	PMDGetEventStatus(&hAxis[Axis],&status);
	#ifdef DEBUGINTERRUPT
	printf("interruptAxisHandling=> El Event Status Register del eje %s despues de la interupccion es %X \n "
				,AxisName(Axis),status);
	#endif	
	
	
	/*--------La causa de la Interrupcion es un Error de Movimiento */
	
	if((status & PMDEventMotionErrorMask)!=0)
	{
		buzzer_ms(500);
		MotionErrorhandling(Axis);		
	}	
	
	/*--------La causa de la Interrupcion es el Evento de Limite Positivo */
	
	if((status & PMDEventInPositiveLimitMask)!=0)
	{
		buzzer_ms(100);
		Positivehandling(Axis);		
	}		
	
	/*--------La causa de la Interrupcion es el Evento de Limite Negativo */
	
	if((status & PMDEventInNegativeLimitMask)!=0)
	{
		buzzer_ms(100);
		Negativehandling(Axis);		
	}
	
	
	
	/*--------La causa de la Interrupcion es un Error de Instruccion */
	
	if((status & PMDEventInstructionErrorMask)!=0)
	{
		buzzer_ms(500);
		InstructionErrorhandling(Axis);		
	}	


	
}

/*****************************************************************************************/
void InterruptHandling_chipset1(void)
{
	PMDuint16 interruptingAxisMask1;
	PMDresult rc;	
	interruptingAxisMask1=0;
	
	

	
	//Verificar que eje produjo la Interrupcion

	do
	{
		rc=PMDGetInterruptAxis(&hAxis[AxisWaist],  &interruptingAxisMask1);
	}
	while (rc!=PMD_ERR_OK);		
	
	
		
	#ifdef DEBUGINTERRUPT
		printf("InterruptHandling_chipset1=> La mascara 1 de interrupcion es %X \n",interruptingAxisMask1 );
	#endif
	
	
	
	if ((interruptingAxisMask1 & PMDAxis1Mask)!=0)					//Eje de cintura Causo la Interrupcion?
	{
		interruptAxisHandling(AxisWaist);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask1 = interruptingAxisMask1 & ~(PMDAxis1Mask);
		#ifdef DEBUGINTERRUPT
		printf("InterruptHandling_chipset1=> La mascara 1 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask1 );		
		#endif
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisWaist]);	
		}
		while (rc!=PMD_ERR_OK);			
		
		return;
	}
	
	
	if ((interruptingAxisMask1 & PMDAxis2Mask)!=0)					//Eje de HOmbro Causo la Interrupcion?
	{
		interruptAxisHandling(AxisShoulder);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask1 = interruptingAxisMask1 & ~(PMDAxis2Mask);
		#ifdef DEBUGINTERRUPT
		printf("InterruptHandling_chipset1=> La mascara 1 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask1 );
		#endif
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisShoulder]);	
		}
		while (rc!=PMD_ERR_OK);	
		
		return;
	}

	if ((interruptingAxisMask1 & PMDAxis3Mask)!=0)					//Eje de Codo Causo la Interrupcion?
	{
		interruptAxisHandling(AxisElbow);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask1 = interruptingAxisMask1 & ~(PMDAxis3Mask);
		#ifdef DEBUGINTERRUPT
		printf("InterruptHandling_chipset1=> La mascara 1 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask1 );
		#endif
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisElbow]);	
		}
		while (rc!=PMD_ERR_OK);	
		
		return;
	}

	if ((interruptingAxisMask1 & PMDAxis4Mask)!=0)					//Eje de Pitch Causo la Interrupcion?
	{
		interruptAxisHandling(AxisPitch);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask1 = interruptingAxisMask1 & ~(PMDAxis4Mask);
		#ifdef DEBUGINTERRUPT
		printf("InterruptHandling_chipset1=> La mascara 1 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask1 );
		#endif
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisPitch]);	
		}
		while (rc!=PMD_ERR_OK);	
		
		return;
	}
	
}
	

/*****************************************************************************************/
void InterruptHandling_chipset2(void)
{
	PMDuint16 interruptingAxisMask2;
	PMDresult rc;
	
	interruptingAxisMask2=0;
	
	//Verificar que eje produjo la Interrupcion	
	do
	{
		rc=PMDGetInterruptAxis(&hAxis[AxisRoll],  &interruptingAxisMask2);
	}
	while (rc!=PMD_ERR_OK);			
	
	
	
	#ifdef DEBUGINTERRUPT
		printf("InterruptHandling_chipset2=> La mascara 2 de interrupcion es %X \n",interruptingAxisMask2 );
	#endif
	
	
	if ((interruptingAxisMask2 & PMDAxis1Mask)!=0)					//Eje de Roll Causo la Interrupcion?
	{
		interruptAxisHandling(AxisRoll);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask2 = interruptingAxisMask2 & ~(PMDAxis1Mask);
		
		#ifdef DEBUGINTERRUPT
			printf("InterruptHandling_chipset2=> La mascara 2 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask2 );
		#endif	
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisRoll]);	
		}
		while (rc!=PMD_ERR_OK);	
		
		return;
	}
		
	if ((interruptingAxisMask2 & PMDAxis2Mask)!=0)					//Eje Aux1 Causo la Interrupcion?
	{
		interruptAxisHandling(AxisAux1);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask2 = interruptingAxisMask2 & ~(PMDAxis2Mask);
		
		#ifdef DEBUGINTERRUPT
			printf("InterruptHandling_chipset2=> La mascara 2 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask2 );
		#endif	
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisAux1]);	
		}
		while (rc!=PMD_ERR_OK);		
		
		return;
	}
		
	if ((interruptingAxisMask2 & PMDAxis3Mask)!=0)					//Eje Aux2 Causo la Interrupcion?
	{
		interruptAxisHandling(AxisAux2);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask2 = interruptingAxisMask2 & ~(PMDAxis3Mask);
		
		#ifdef DEBUGINTERRUPT
			printf("InterruptHandling_chipset2=> La mascara 2 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask2 );
		#endif	
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisAux2]);	
		}
		while (rc!=PMD_ERR_OK);	
		
		return;
	}
		
	if ((interruptingAxisMask2 & PMDAxis4Mask)!=0)					//Eje Aux3 Causo la Interrupcion?
	{
		interruptAxisHandling(AxisAux3);
		//Limpio el bit correspondiente con el eje
		interruptingAxisMask2 = interruptingAxisMask2 & ~(PMDAxis4Mask);
		
		#ifdef DEBUGINTERRUPT
			printf("InterruptHandling_chipset2=> La mascara 2 de interrupcion despues de limpiar es  %X \n",interruptingAxisMask2 );
		#endif	
		
		/* resets the HostInterrupt signal to its inactive state*/	 
		do
		{
			rc=PMDClearInterrupt(&hAxis[AxisAux3]);	
		}
		while (rc!=PMD_ERR_OK);	
		
		
		return;
	}
	
	
}