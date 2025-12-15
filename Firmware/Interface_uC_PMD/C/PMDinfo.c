#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "c-motion.h"
#include "PMDsetup.h"
#include "messages.h"
#include "debug.h"

extern PMDAxisHandle hAxis[DOF]; 

//**********************************************************************************************

PMDresult PMDCheckEventStatus(int16 Axis, PMDint16 print)  
{
	PMDuint16 status;	
	
	PMDGetEventStatus(&hAxis[Axis],&status);
	//Se limpia el Event Status Register
	PMDResetEventStatus(&hAxis[Axis],~PMDEventStatusMask);
		
	printf("PMDCheckEventStatus=> El Event Status del eje %s es %X \n",AxisName(Axis),status);
	
	if(print==imprimir)
	{
    
		
		if ((status & PMDEventMotionCompleteMask) !=0) //Este eje está en movimiento		
			printf("PMDCheckEventStatus=> El movimiento  en el eje %s ha sido completado\n",AxisName(Axis));
		
		
		if ((status & PMDEventWrapAroundMask) !=0 ) 
			printf("PMDCheckEventStatus=> Desborde en la posicion actual del motor 7FFFFFFF O 80000000 eje %s \n",AxisName(Axis));
	 
		//Se ha activado el Breakpoint 1
		if ((status & PMDEventBreakpoint1Mask) !=0) 
			printf("PMDCheckEventStatus=> Se ha activado el Breakpoint 1 en el eje %s \n",AxisName(Axis));
		
		//Se activa cuando el hardware de alta velocidad actura un nuevo dato de posicion
		if ((status & PMDEventCaptureReceivedMask) !=0) 
			printf("PMDCheckEventStatus=> El Hardware de alta velocidad ha capturado una nueva posicion del eje %s \n",AxisName(Axis));
			
		//Cuando la posicion Actual difiere de la posicion commandada por una cantidad mayor 
		//que la expecificada por el maximo error de posicion
		if ((status & PMDEventMotionErrorMask)!=0) 
			printf("PMDCheckEventStatus=> El error del movimiento excede el limite maximo de error para el eje %s \n",AxisName(Axis));
		
		//Se llego al limite positivo
		if ((status & PMDEventInPositiveLimitMask) !=0) 
			printf("PMDCheckEventStatus=> Se ha llegado al limite Positivo en el eje %s\n",AxisName(Axis));
		
		//Se llego al limite negativo
		if ((status & PMDEventInNegativeLimitMask) !=0 ) 
			printf("PMDCheckEventStatus=> Se ha llegado al limite negativo en el eje %s\n",AxisName(Axis));
			
		//Error de instruccion
		if ((status & PMDEventInstructionErrorMask) !=0) 
			printf("PMDCheckEventStatus=> Ha ocurrido un error de Instruccion en el eje %s \n",AxisName(Axis));
		
		//Error de la comunicacion solo para (MC2300)
		if ((status & PMDEventCommutationErrorMask) !=0) 
			printf("PMDCheckEventStatus=> Error de comunicacion solo para MC2300 en el eje %s \n",AxisName(Axis));
		
		//Se ha activado el Breakpoint 2
		if ((status & PMDEventBreakpoint2Mask) !=0) 
			printf("PMDCheckEventStatus=> Se ha activado del Breakpoint2 en el eje %s \n",AxisName(Axis));
	}	
	
	return status;
  
}


/*****************************************************************************************/
PMDresult PMDCheckActivityStatus(int16 Axis, PMDint16 print)
{ 
	PMDuint16 status,profile;	
	
	PMDGetActivityStatus(&hAxis[Axis],&status);
	printf("PMDCheckActivityStatus=> El Activity status del eje %s es %X \n",AxisName(Axis),status);
	
	if(print==imprimir)
	{
		if ((status & PMDActivityAtMaximumVelocityMask) !=0 ) 
			printf("PMDCheckActivityStatus=> El eje %s esta A la maxima Velocidad  \n",AxisName(Axis));
		
		if ((status & PMDActivityTrackingMask) !=0) 
			printf("PMDCheckActivityStatus=> El eje %s esta dentro del tracking window  \n",AxisName(Axis));
		
		profile=(status & PMDActivityProfileModeMask) >> 3;
		switch (profile)
		{
			case (0):
			printf("PMDCheckActivityStatus=> El eje  %s tiene perfil TRAPEZOIDAL \n",AxisName(Axis));
			break;
			
			case (1):
			printf("PMDCheckActivityStatus=> El eje  %s tiene perfil de VELOCIDAD \n",AxisName(Axis));
			break;
			
			case (2):
			printf("PMDCheckActivityStatus=> El eje  %s tiene perfil CURVA-S \n",AxisName(Axis));
			break;
			
			case (3):
			printf("PMDCheckActivityStatus=> El eje  %s tiene perfil ELECTRONIC GEAR \n",AxisName(Axis));
			break;
		}
		
		if ((status & PMDActivityAxisSettledMask)!=0) 
			printf("PMDCheckActivityStatus=> El eje %s ha permanecido en el Settle Window por un periodo de tiempo N \n",AxisName(Axis));
		if ((status &  PMDActivityMotorOnMask) !=0) 
			printf("PMDCheckActivityStatus=> El eje %s esta en MOTOR MODE ON \n",AxisName(Axis));		
		if ((status & PMDActivityPositionCaptureMask) !=0 ) 
			printf("PMDCheckActivityStatus=> El eje %s tiene un nuevo valor de posicion disponible para leer \n",AxisName(Axis));		
		if ((status & PMDActivityInMotionMask) !=0) 
			printf("PMDCheckActivityStatus=> La posicion comandada esta cambiando en el eje %s\n",AxisName(Axis));		
		if ((status & PMDActivityInPositiveLimitMask) !=0) 
			printf("PMDCheckActivityStatus=> El eje %s  esta en el Limite Positivo \n",AxisName(Axis));		
		if ((status &  PMDActivityInNegativeLimitMask) !=0) 
			printf("PMDCheckActivityStatus=> El eje %s  esta en el Limite Negativo \n",AxisName(Axis));
	}

	return status;
}

/*****************************************************************************************/
//Si hay un 1 en la maskara SetSignalSense devuelve el invertido del valor electrico
//Si hay un 1 en la maskara el PMD interpreta que (0V SW oprimido) significa limite
//Si hay un 0 en la maskara SetSignalSense devuelve el el valor electrico
//Si hay un 0 en la maskara el PMD interpreta que (5V SW sin oprimir) significa limite
PMDresult PMDCheckSignalStatus(int16 Axis, PMDint16 print)
{

	PMDuint16 status,mask,status_char;
	char binario[20]="0";
	
	
	PMDGetSignalStatus(&hAxis[Axis],&status);
	//printf("El Signal status register del eje %s es %X \n",AxisName(Axis),status);
	
	status_char=status;
	status_char= status_char >> 3;
	
	itoa(status_char,binario,2);	
	
	printf("PMDCheckSignalStatus=> signal status = %s  \n",binario);
	
	
	if(print==imprimir)
	{
		PMDGetSignalSense(&hAxis[Axis],& mask);
		printf("PMDCheckSignalStatus=> La mascara de Signal sense del eje %s es %X \n",AxisName(Axis),mask);
	
		if ((status & PMDSignalEncoderAMask) !=0)
		{
			if((mask & PMDSignalEncoderAMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> El canal A = Real (0V) Valor  registro (5V) \n");		
		    else
			printf("PMDCheckSignalStatus=> El canal A = (5V) Valor Electrico Real =registro \n");
		}
		else
		{
			if((mask & PMDSignalEncoderAMask)!=0)  //el bit vale 1 en el signal sense
			printf("PMDCheckSignalStatus=> El canal A = Real (5V) Valor  Registro (0V) \n");
				
		    else
			printf("PMDCheckSignalStatus=> El canal A = (0V) Valor Electrico Real = Registro \n");	
		}
		
		//--------------------------------------------------------------------------------//
		if ((status & PMDSignalEncoderBMask) !=0)
		{
			if((mask & PMDSignalEncoderBMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> El canal B = Real (0V) Valor registro (5V) \n");		
		    else
			printf("PMDCheckSignalStatus=> El canal B = (5V) Valor Electrico Real =registro \n");
		}
		else
		{
			if((mask & PMDSignalEncoderBMask)!=0)  //el bit vale 1 en el signal sense
			printf("PMDCheckSignalStatus=> El canal B = Real (5V) Valor Registro (0V) \n");
				
		    else
			printf("PMDCheckSignalStatus=> El canal B = (0V) Valor Electrico Real= Registro \n");	
		}
		//--------------------------------------------------------------------------------//
		
		if ((status & PMDSignalEncoderIndexMask) !=0)
		{
			if((mask & PMDSignalEncoderIndexMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> Index = Real (0V) Valor registro (5V) \n");		
		    else
			printf("PMDCheckSignalStatus=> Index = (5V) Valor Electrico Real = registro \n");
		}
		else
		{
			if((mask & PMDSignalEncoderIndexMask)!=0)  //el bit vale 1 en el signal sense
			printf("PMDCheckSignalStatus=> Index = Real (5V) Valor Registro (0V) \n");
				
		    else
			printf("PMDCheckSignalStatus=> Index = (0V) Valor Electrico Real= Registro \n");	
		}
		
		
		//--------------------------------------------------------------------------------//
		
		if ((status & PMDSignalEncoderHomeMask) !=0)
		{
			if((mask & PMDSignalEncoderHomeMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> Home = Real (0V) Valor registro (5V) Captura Low to High \n");		
		    else
			printf("PMDCheckSignalStatus=> Home = (5V) Valor Electrico Real = registro Captura de High To Low \n");
		}
		else
		{
			if((mask & PMDSignalEncoderHomeMask)!=0)  //el bit vale 1 en el signal sense
			printf("PMDCheckSignalStatus=> Home = Real (5V) Valor Registro (0V) Captura Low to High \n");
				
		    else
			printf("PMDCheckSignalStatus=> Home = (0V) Valor Electrico Real = Registro  Captura de High To Low \n");	
		}
		
		//--------------------------------------------------------------------------------//
		if ((status & PMDSignalPositiveLimitMask) !=0)
		{
			if((mask & PMDSignalPositiveLimitMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> Limite Positivo = Real (0V) Valor registro (5V)  No hay Evento de Limite \n");		
		    else
			printf("PMDCheckSignalStatus=> Limite Positivo = (5V) Valor Electrico Real = registro No hay Evento de Limite \n");
		}
		else
		{
			if((mask & PMDSignalPositiveLimitMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> Limite Positivo = Real (5V) Valor Registro (0V) Hay Evento de Limite\n");
			else
			printf("PMDCheckSignalStatus=> Limite Positivo = (0V) Valor Electrico Real = Registro hay Evento de Limite \n");	
		}
		//--------------------------------------------------------------------------------//
		if ((status & PMDSignalNegativeLimitMask) !=0)
		{
			if((mask & PMDSignalNegativeLimitMask)!=0)  //el bit vale 1 en el signal sense
			
			printf("PMDCheckSignalStatus=> Limite Negativo = Real (0V) Valor registro (5V) No hay Evento de Limite \n");		
		    else
			printf("PMDCheckSignalStatus=> Limite Negativo = (5V) Valor Electrico Real= registro No hay Evento de Limite \n");
		}
		else
		{
			if((mask & PMDSignalNegativeLimitMask)!=0)  //el bit vale 1 en el signal sense
			printf("PMDCheckSignalStatus=> Limite Negativo = Real (5V) Valor Registro (0V) Hay Evento de Limite\n");
			else
			printf("PMDCheckSignalStatus=> Limite Negativo = (0V) Valor Electrico Real = Registro hay Evento de Limite \n");	
		}
		
		//--------------------------------------------------------------------------------//
		
		if ((status & PMDSignalStepOutputInvertMask) !=0) 
			printf("PMDCheckSignalStatus=> Step Output invert = + positivo\n");
		else
			printf("PMDCheckSignalStatus=> Step Output invert = - negativo\n");
		//--------------------------------------------------------------------------------//
		
		if ((status & PMDSignalMotorOutputInvertMask) !=0) 
			printf("PMDCheckSignalStatus=> Motor Output invert = + positivo\n");
		else
			printf("PMDCheckSignalStatus=> Motor Output invert = - negativo\n");
		//--------------------------------------------------------------------------------//
	}
	
	return status;

}
/**********************************************************************************************************************/
void infomov(int16 Axis, PMDint32* position, PMDint32* velocity, PMDuint32* acceleration, PMDuint32* deceleration,
			PMDint32* commandedposition,PMDint32* commandedvelocity, PMDint32* commandedacceleration,
			PMDint32* readencoder1, PMDint32* readerror,PMDuint16* encoderCounts, PMDuint16* steps)
{
	
	PMDGetPosition(&hAxis[Axis],position);
	PMDGetVelocity(&hAxis[Axis],velocity);
	PMDGetAcceleration(&hAxis[Axis],acceleration);
	PMDGetDeceleration(&hAxis[Axis],deceleration);
	
	PMDGetCommandedPosition(&hAxis[Axis],commandedposition);
	PMDGetCommandedVelocity(&hAxis[Axis],commandedvelocity);
	PMDGetCommandedAcceleration(&hAxis[Axis],commandedacceleration);
	
	
	//Funciones que hacen uso del Encoder
	PMDGetActualPosition(&hAxis[Axis],readencoder1);
	PMDGetPositionError(&hAxis[Axis], readerror);	
	PMDGetEncoderToStepRatio(&hAxis[Axis], encoderCounts,steps);
	
	printf("infomov=> La informacion correspondiente del movimiento del eje %s es :\n",AxisName(Axis));
	printf("infomov=> posicion=	%08ld\n",*position);
	printf("infomov=> velodidad	= 	%08ld\n",*velocity);
	printf("infomov=> aceleracion=	%08ld\n",*acceleration);
	printf("infomov=> desaceleracion=	%08ld\n",*deceleration);
	printf("infomov=> posicion instantanea=	%08ld\n",*commandedposition);
	printf("infomov=> velocidad instantanea=	%08ld\n",*commandedvelocity);
	printf("infomov=> aceleracion instantanea=	%08ld\n",*commandedacceleration);
	printf("infomov=> Lectura Encoder=	%08ld\n",*readencoder1);
	printf("infomov=> Error Posicion=	%08ld\n",*readerror);	
	//printf("infomov=> Cuentas de Encoder= %08d \n",*cuentas_encoder);
	//printf("infomov=> No. uPasos por vuelta= %08d \n",*steps);		

	
}

/**********************************************************************************************************************/

void Print_Handlesinfo(void)
{
  int16 axis;
  
	for(axis=0; axis < DOF; axis++) 
	{  
		printf("Print_Handlesinfo=> El Index del vector es %d \n",axis );
		printf("Print_Handlesinfo=> hAxis[%d].axis es igual a  %d  \n",axis,hAxis[axis].axis);
		printf("Print_Handlesinfo=> Corresponde a %s \n",AxisName(axis));
		printf("Print_Handlesinfo=> Es manejado por el chipset %d  \n",hAxis[axis].chipset);
	}
	
}
/**********************************************************************************************************************/
PMDuint16 getErrorHost(int16 axis)
{
    PMDuint16 ErrorPMD=0;
	 
	PMDGetHostIOError(&hAxis[axis],&ErrorPMD);
	
	return ErrorPMD;
}
/**********************************************************************************************************************/
int16 checkconnect(void)
{
    PMDuint16 signal_status;
	int16 Axis,Chipset,rc;
	
	rc=rcOK;
	
	/** SE observa el Estado del Boton de Parada de Emergecia **/
	
	if(bit_is_set(PIND, Estado_Boton_Emergencia))
	{
		buzzer(1);
		
		printf("checkconnect=> Boton de Parada de Emergencia Activo \n");
		fprintf_P(stderr,PSTR("Boton de Parada de Emergencia Activo \n"));
		
	}
	for(Axis=0; Axis < DOF; Axis++) 
	{
		
		PMDGetSignalStatus(&hAxis[Axis],&signal_status);	
		
		if ((signal_status & PMDSignalPositiveLimitMask) ==0) //Significa que esta activado el Limite Positivo
		{
			
			/*Verificar si los cables de Sensores estan conectados*/
			if((signal_status & PMDSignalNegativeLimitMask) ==0)
			{
				
				buzzer_ms(50);
				buzzer_ms(50);
				Chipset=Axis2PMDChipset(Axis)+1;
				printf("checkconnect=> Cable #%d de Sensores esta desconectado \n",Chipset);
				printf("checkconnect=> Los switch del eje de %s estan desconectados \n",AxisName(Axis));
				
				//impresion de caracteres Display
				if (Chipset==1)
					fprintf_P(stderr,PSTR("Cable #1 de sensores desconectado \n"));
				
				if (Chipset==2)
					fprintf_P(stderr,PSTR("Cable #2 de sensores desconectado \n"));
				
				rc=-1;
			}
		}
		
	}
	
	return rc;
}
