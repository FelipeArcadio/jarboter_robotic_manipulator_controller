#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include "c-motion.h"
#include "PMDsetup.h"
#include "PMDrobotics.h"
#include "PMDinterrupt.h"
#include "robotData.h"
#include "debug.h"
#include "messages.h"
#include "mem.h"
#include "packetIO.h"
#include "tw_teach_pendant.h"
#include "PMDtrace.h"

//#define DEBUGSETUP


extern PMDAxisHandle hAxis[DOF]; 
/*Variable que almacenara la reconfiguracion por parte del usuario*/
extern tAxisSettings EEMEM EEprom_axisSettings[DOF];
/*Bandera que indica reconfiguracion por parte del Usuario*/
extern int16 EEMEM EEprom_Reconf_Indicator;


/******************************DEFINICION DE LAS FUNCIONES***********************************/

PMDresult initAxisHandles(void) 
{
	int16 Axis,rc;
	int16 Reconf_Enable;

	for(Axis=0; Axis<DOF; Axis++) 
	{  
		if(hAxis[Axis].transport_data == NULL) //Si no ha sido definida
		{		
			rc = PMDSetupAxisInterface_Parallel(&hAxis[Axis], Axis2PMDAxis(Axis), Axis2PMDChipset(Axis));
			if(rc != rcOK) 
				return rc; //no memory / rc ok
		}
	}	
   
	
	/* Leer el indicado de reconfiguracion*/
	eeprom_read_block((void*)&Reconf_Enable,(const void*)&EEprom_Reconf_Indicator,sizeof(int16));
	
     
	for(Axis=0; Axis<DOF; Axis++)
	{
		//Configura la structura tAxisSettings
		//limites de velocidad entre ostras cosas definidas en robotdata.c 
		/*Si no he reconfigurado utilizsr por defecto*/
                if(!Reconf_Enable)
			defaultAxisSettings(Axis,&hAxis[Axis].axisSettings); 		
		else
		{
			printf("initAxisHandles=> Configurando usando la eeprom\n");
			//Configuro Usando la eeprom
			//Actualizo rxAxissetting a la estructura PMDAxisHandle hAxis	
			eeprom_read_block((void*)&hAxis[Axis].axisSettings,(const void*)&EEprom_axisSettings[Axis],sizeof(tAxisSettings));
		}
	}

	
	return rcOK;
} 

/*********************************************************************************************/
PMDresult PMDChipsetReset(PMDAxisHandle* handle)  //Queda igual
{
	PMDuint16 result;
	PMDuint16 status;
	 
   // reset the PMD chip set that this axis resides on
	// if more than one chip set is present, all of them should be reset here
	result = PMDReset(handle);
	// with the parallel interface the result code will always be
	// PMD_ERR_CommandError since that bit is set whenever a reset
	// occurs.  If it ISN'T set then there is some other error
	if ( result == PMD_ERR_CommandError)
	{
		result = PMDGetHostIOError(handle, &status);
	   if ((result == PMD_ERR_OK) && (status == PMD_ERR_ChipsetReset))			 
		{
			printf("PMDChipsetReset => PMDreset ejecutado correctamente\n\n");
			return rcOK;
		}
	}		 
	printf("PMDChipsetReset => PMDreset ejecutado INCORECTAMENTE!\n\n");
  	return rcPMDChipsetResetError; //al main principal USAMOS rc........
} // PMDChipsetReset	

/*********************************************************************************************/
PMDresult PMDchipsetOK(PMDuint16 PMDChipSet) // Le quite el parametro PMDAxisHandle* hAxis
{
   PMDuint32 checksum=0;
   PMDresult result;
   int16 axis,axistemp=0;  
  
	// before reading from the PMD, we must reset the chipsets  
  
	for(axis=0; axis < DOF; axis++) 
	{

      if(hAxis[axis].chipset == PMDChipSet) //Buscamos un AxisHandle para ese chipset
	   {  
			result = PMDChipsetReset(&hAxis[axis]);
	      if(result != rcOK)
			{
				printf("PMDchipsetOK => No se pudo resetear el chipset %i con Axis %d\n",PMDChipSet,axis);
				return result;
			}
		  else
		   { 
				axistemp=axis;			 
				printf("PMDchipsetOK => Se ha reseteado el chipset %i con Axis %d\n",PMDChipSet,axis);	 
				axis=DOF; //Nos salimos del for
		   }
      }
	}
  
	// Leer el checksum del Chipset y comparar con 0x12345678
	printf("PMDchipsetOK =>Comprobando Checksum...\n");	
	result=PMDGetChecksum(&hAxis[axistemp],&checksum); 
	if(checksum != 0x12345678) //Esto esta definido en el manual de programacion .
	{
		printf("PMDchipsetOK =>PMD %i PMDchipset checksum test error: 0x12345678=>%lx\n",PMDChipSet+1,checksum);
		return rcPMDchecksumError;  // we got a wrong answer
	}
	printf("PMDchipsetOK =>PMD %i test succeeded %lx \n",PMDChipSet+1,checksum);
	return rcOK;
} 

/*********************************************************************************************/
PMDresult selftestPMD(void)  //Le quite el parametro PMDAxisHandle* hAxis
{
	int16 rc1= rcOK;                // return code
	int16 rc2 = rcOK; 
	int16 rc;
   
   rc1 = initAxisHandles();        //inicializamos todos los ejes hAxis
   if(rc1!=rcOK)
   { //Enviar Codigo de ERRoR por Led´s , por Buzzer y Por Display.
		buzzer(2);
		delay_1s();
		buzzer(1); 
		
	}    
	
	else
	{
		
		printf("selftestPMD => Hard reset\n");	
		PMDHardReset(&hAxis[0]); 			//hacemos reset por hardware a PMD1 y PMD2
		rc1 = PMDchipsetOK(PMDChipSet1); 	//COmprobamos el Checksum después de reset
		if(rc1 != rcOK)						// Error de Checksum para Chipset 1
		{
			Led_on(Amarillo);    
			buzzer(1);
			fprintf_P(stderr,PSTR("Error en Controlador 1 \n"));
			fprintf_P(stdout,PSTR("Error en el Controlador de motores No. 1 \n"));			
		}
		// Chipset1 OK
		
		rc2 = PMDchipsetOK(PMDChipSet2);//COmprobamos el Checksum despues de reset
		if(rc2 != rcOK)					 // Error de Checksum para Chipset 2
		{ //Enviar Codigo de ERRoR por Led´s , por Buzzer y por Display.y salir del programa
		  
			Led_on(Rojo);    //
			buzzer(1);
			fprintf_P(stderr,PSTR("Error en Controlador 2 \n"));
			fprintf_P(stdout,PSTR("Error en el Controlador de motores No. 2 \n"));			
		}
		
		if((rc1 == rcOK) && (rc2 == rcOK))		//  Exito en ambos Checksum
		{
			
			Led_on(Verde);							
		} 		
	}

	rc= rc1 || rc2;
	
	return rc;

}
/*********************************************************************************************/
PMDresult PMDsetupChipSets(PMDProfileMode Profile) 
{
	uint8_t axis;
	PMDuint16 PMDrc,EncodertoStepRatio;

  // look for a handle of every chipset to reset it...look for axis=1
  for(axis=0; axis < DOF; axis++)
	{
		if(hAxis[axis].axis == 0)  
		{
			PMDChipsetReset(&hAxis[axis]);	
			/* "calibrate" the velocity and acceleration settings: adjust the sampling time (cycle time)
			minimum 4axis*154us/axis, max. value 65536us */
			PMDSetSampleTime(&hAxis[axis],4*150);			
			#ifdef DEBUGSETUP
			printf("PMDsetupChipsets_dummy=> Run Reset, Sampletime para eje %d\n",axis);
			#endif
		}
		
	}	
	
	for(axis=0; axis < DOF; axis++)
	{
		
		//FIja la unidad del registo Actual position a uPasos
		PMDrc = PMDSetActualPositionUnits(&hAxis[axis],PMDSteps);     
		if(PMDrc != PMD_ERR_OK) 
		{
			return rcHardwareFailure;  // PMD didn't answer... comment 
										// out if UC should continue!		
		}		
		
		switch (axis)
		{
			case (AxisWaist):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalPositiveLimitMask |PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisShoulder):
				PMDSetSignalSense(&hAxis[axis],
						PMDSignalEncoderHomeMask | PMDSignalPositiveLimitMask |
						PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisElbow):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalEncoderHomeMask |PMDSignalPositiveLimitMask |
						PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisPitch):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalEncoderHomeMask |PMDSignalPositiveLimitMask |
						PMDSignalNegativeLimitMask);
			break;
			
			case (AxisRoll):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalPositiveLimitMask |PMDSignalNegativeLimitMask);
			break;
			
			default:
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalPositiveLimitMask |PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
		}	
		
		
		
		if((hAxis[axis].axisSettings.switchEnable == TRUE) && 
		(hAxis[axis].axisSettings.axisEnabled==TRUE))
		{
			//Se Habilitan los Switches de Fin de Carrera
			PMDSetLimitSwitchMode(&hAxis[axis],PMDLimitEnabled);
		}
		else
		{
			////Se deshabilitan los Switches de Fin de Carrera
			PMDSetLimitSwitchMode(&hAxis[axis],PMDLimitDisabled);
		}
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config switch %d\n",axis);
		#endif

		// Tipo de salida PWM 50/50
		PMDSetOutputMode(&hAxis[axis],PMDMotorOutputPWM5050Magnitude);  
		//numero de fasese del motor 3
		PMDSetNumberPhases(&hAxis[axis],hAxis[axis].axisSettings.numberPhases);
		//numero de pasos por upaso, esta funcion esta en Counts. 1 Counts = 4 uPasos
		PMDSetPhaseCounts(&hAxis[axis],(4*hAxis[axis].axisSettings.ustepsPerStep)); 
		//configuracion de Encoder
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> uPasos = %d \n",hAxis[axis].axisSettings.ustepsPerStep);
		#endif
		
		PMDSetEncoderSource(&hAxis[axis],PMDEncoderSourceIncremental);  // incremental quadrature encoder
			//(pulses per round), (microsteps per step)*(steps per round)
		
		EncodertoStepRatio=hAxis[axis].axisSettings.ustepsPerStep*
										hAxis[axis].axisSettings.motorStepsPerRevolution;
		
			PMDSetEncoderToStepRatio(&hAxis[axis],hAxis[axis].axisSettings.encoderStepsPerRevolution
									,EncodertoStepRatio);
	 	
		PMDSetTrackingWindow(&hAxis[axis], hAxis[axis].axisSettings.TrackingWindow);

		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config Outputmode, #Phases, PhaseCounts,Encoder Source, EncoderRatio %d\n",axis);
		#endif

		
		//se habilitan las funciones de encoder
		if((hAxis[axis].axisSettings.encoderEnable == TRUE) && 
			(hAxis[axis].axisSettings.axisEnabled==TRUE))
		{
			PMDSetSettleWindow(&hAxis[axis], hAxis[axis].axisSettings.SettleWindow);
			PMDSetSettleTime(&hAxis[axis], hAxis[axis].axisSettings.SettleTime);
		    PMDSetMotionCompleteMode(&hAxis[axis],PMDMotionCompleteActualPosition); //  automatic correction of errors
			//PMDSetPositionErrorLimit(&hAxis[axis],(30*hAxis[axis].axisSettings.ustepsPerStep
			//					 *hAxis[axis].axisSettings.motorStepsPerRevolution)/
			//					 hAxis[axis].axisSettings.encoderStepsPerRevolution);
			PMDSetPositionErrorLimit(&hAxis[axis],hAxis[axis].axisSettings.ErrorLimit);
			
			PMDClearPositionError(&hAxis[axis]);
		}	
		else
		{
			PMDSetMotionCompleteMode(&hAxis[axis],PMDMotionCompleteCommandedPosition); // no automatic correction of errors
			
		}
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config SetSeetWindows,Settle time, Motion Complete,Pos. Error limit, Pos. error %d\n",axis);
		#endif
		if(hAxis[axis].axisSettings.axisEnabled==TRUE) 
		{
			PMDSetAxisMode(&hAxis[axis],PMDAxisOn);              // enable Axis
			PMDSetMotorMode(&hAxis[axis],PMDMotorOn);            // enable trajectory generator
		} 
		else 
		{
			PMDSetAxisMode(&hAxis[axis],PMDAxisOff);             // disable Axis
			PMDSetMotorMode(&hAxis[axis],PMDMotorOff);           // disable trajectory generator
		}
		//Fija la potencia del motor o torque
		PMDSetMotorCommand(&hAxis[axis],(hAxis[axis].axisSettings.motorPower*32768)/100);
		//Se selecciona un tipo de perfil
		PMDSetProfileMode(&hAxis[axis],Profile);   // data interpretation mode
		//Detiene el motor cuando se produce un error en el movimiento (Motion Error).
		PMDSetAutoStopMode(&hAxis[axis],PMDAutoStopDisabled); 		
		
		PMDResetEventStatus(&hAxis[axis],~PMDEventStatusMask);
		PMDClearInterrupt(&hAxis[axis]);
		PMDUpdate(&hAxis[axis]);
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config AxisMode,Motor Mode, Motor power,Auto Stop, Reset Event, Clear INT,Update  %d\n",axis);
		#endif		
	}	
	return rcOK;
}

/*********************************************************************************************/
//En PMDsetupChipsets_USB no se ejecutan los siguientes Comandos:
//-PMDChipsetReset
//-PMDSetSampleTime
//-PMDSetOutputMode
//-PMDSetNumberPhases
//-PMDSetEncoderSource
//-PMDSetEncoderToStepRatio
//-PMDSetAxisMode
//-PMDSetMotorMode
PMDresult PMDsetupChipsets_USB(PMDProfileMode Profile) 
{
	uint8_t axis;
	PMDuint16 PMDrc;

  // look for a handle of every chipset to reset it...look for axis=1
	
	for(axis=0; axis < DOF; axis++)
	{
		//FIja la unidad del registo Actual position a uPasos
		PMDrc = PMDSetActualPositionUnits(&hAxis[axis],PMDSteps);     
		if(PMDrc != PMD_ERR_OK) 
		{
			return rcHardwareFailure;  // PMD didn't answer... comment 
										// out if UC should continue!		
		}		
		
		switch (axis)
		{
			case (AxisWaist):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalPositiveLimitMask |PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisShoulder):
				PMDSetSignalSense(&hAxis[axis],
						PMDSignalEncoderHomeMask | PMDSignalPositiveLimitMask |
						PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisElbow):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalEncoderHomeMask |PMDSignalPositiveLimitMask |
						PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisPitch):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalEncoderHomeMask | PMDSignalPositiveLimitMask |
						PMDSignalNegativeLimitMask |PMDSignalMotorOutputInvertMask);
			break;
			
			case (AxisRoll):
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalPositiveLimitMask |PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
			default:
				 PMDSetSignalSense(&hAxis[axis],
						PMDSignalPositiveLimitMask |PMDSignalNegativeLimitMask | PMDSignalMotorOutputInvertMask);
			break;
			
		}			
		if((hAxis[axis].axisSettings.switchEnable == TRUE) && 
		(hAxis[axis].axisSettings.axisEnabled==TRUE))
		{
			//Se Habilitan los Switches de Fin de Carrera
			PMDSetLimitSwitchMode(&hAxis[axis],PMDLimitEnabled);
		}
		else
		{
			////Se deshabilitan los Switches de Fin de Carrera
			PMDSetLimitSwitchMode(&hAxis[axis],PMDLimitDisabled);
		}
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config switch %d\n",axis);
		#endif
		PMDSetPhaseCounts(&hAxis[axis],(4*hAxis[axis].axisSettings.ustepsPerStep)); 
		//configuracion de Encoder
		PMDSetTrackingWindow(&hAxis[axis], hAxis[axis].axisSettings.TrackingWindow);

		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config Outputmode, #Phases, PhaseCounts,Encoder Source, EncoderRatio %d\n",axis);
		printf("PMDsetupChipsets_dummy=> hAxis[%d].axisSettings.ustepsPerStep= %d\n",axis,hAxis[axis].axisSettings.ustepsPerStep);
		#endif

		
		//se habilitan las funciones de encoder
		if((hAxis[axis].axisSettings.encoderEnable == TRUE) && 
                   (hAxis[axis].axisSettings.axisEnabled==TRUE))
		{
			PMDSetSettleWindow(&hAxis[axis], hAxis[axis].axisSettings.SettleWindow);
			PMDSetSettleTime(&hAxis[axis], hAxis[axis].axisSettings.SettleTime);
		    PMDSetMotionCompleteMode(&hAxis[axis],PMDMotionCompleteActualPosition); //  automatic correction of errors
			//PMDSetPositionErrorLimit(&hAxis[axis],(30*hAxis[axis].axisSettings.ustepsPerStep
			//					 *hAxis[axis].axisSettings.motorStepsPerRevolution)/
			//					 hAxis[axis].axisSettings.encoderStepsPerRevolution);
			PMDSetPositionErrorLimit(&hAxis[axis],hAxis[axis].axisSettings.ErrorLimit);
			
			PMDClearPositionError(&hAxis[axis]);
		}	
		else
		{
			PMDSetMotionCompleteMode(&hAxis[axis],PMDMotionCompleteCommandedPosition); // no automatic correction of errors
			
		}
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config SetSeetWindows,Settle time, Motion Complete,Pos. Error limit, Pos. error %d\n",axis);
		#endif
		//Fija la potencia del motor o torque
		PMDSetMotorCommand(&hAxis[axis],(hAxis[axis].axisSettings.motorPower*32768)/100);
		//Se selecciona un tipo de perfil
		PMDSetProfileMode(&hAxis[axis],Profile);   // data interpretation mode
		//Detiene el motor cuando se produce un error en el movimiento (Motion Error).
		PMDSetAutoStopMode(&hAxis[axis],PMDAutoStopDisabled); 		
		
		PMDResetEventStatus(&hAxis[axis],~PMDEventStatusMask);
		PMDClearInterrupt(&hAxis[axis]);
		PMDUpdate(&hAxis[axis]);
		#ifdef DEBUGSETUP
		printf("PMDsetupChipsets_dummy=> config AxisMode,Motor Mode, Motor power,Auto Stop, Reset Event, Clear INT,Update  %d\n",axis);
		#endif		
	}	
	return rcOK;
}

/**********************************************************************************************************************/
void Reset_Even_Status_Register(void)
{
	int16 axis;
	
	for(axis=0; axis < DOF; axis++) 
	{
		if((hAxis[axis].axisSettings.axisEnabled == TRUE))
			PMDResetEventStatus(&hAxis[axis],~PMDEventStatusMask); 
	}	
}

/**********************************************************************************************************************/
void Clear_Position_Error(void)
{
	int16 axis;
	
	for(axis=0; axis < DOF; axis++) 
	{
		if((hAxis[axis].axisSettings.axisEnabled == TRUE))
			PMDClearPositionError(&hAxis[axis]);
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


/**********************************************************************************************************************/
void ajustar_posicion(int16 axis, PMDint32 posicion)
{


	PMDAdjustActualPosition(&hAxis[axis],posicion);

}

/**********************************************************************************************************************/
void cargar_encoder(int16 axis, PMDint32 posicion)
{


	PMDSetActualPosition(&hAxis[axis],posicion);

}


/**********************************************************************************************************************/
void  Config_BreakPoint(int16 Axis, int16 breakpointID,int16 breakAxis, int16 action, int16 trigger, PMDint32 breakpointvalue)
{

	PMDSetBreakpointValue(&hAxis[Axis], (PMDuint16)breakpointID, breakpointvalue);
	
	PMDSetBreakpoint(&hAxis[Axis],(PMDuint16)breakpointID,(PMDAxis)breakAxis,(PMDuint8)action,(PMDuint8)trigger);

}
/**********************************************************************************************************************/
int Axissetting2hAxisHandle(uint8* rxAxissetting)
{
	int16 i,Buffersize,Axis,Diferentes,Ram_Reconf_Indicator;
	uint8* pdataEEprom;
	tAxisSettings Ram_axisSettings[DOF];
	tAxisSettings * prxAxissetting;


	Diferentes=0;
	
	#ifdef DEBUGSETUP
	printf("CopiartohAxissetting=> Iniciando la Comparacion\n");
	#endif	
	for(Axis=0;Axis<DOF;Axis++) 
	{
		/* Leer la configuracion anterior para no reconfigurar lo mismo*/
		//Destino, Fuente.
		eeprom_read_block((void*)&Ram_axisSettings[Axis],(const void*)&EEprom_axisSettings[Axis],sizeof(tAxisSettings));
		pdataEEprom=(uint8*)&Ram_axisSettings[Axis];
		#ifdef DEBUGSETUP
		printf("CopiartohAxissetting=> Configuracion en EEprom del Eje %d\n",Axis);
		
		PrintAxissetting(Ram_axisSettings[Axis]);
		#endif
		
		//Comparo las configuraciones por eje
		for(i=0;i<sizeof(tAxisSettings);i++) 
		{
			//Utilizo el apuntador a bytes!
                        /*(sizeof(tAxisSettings)*Axis) significa la posicion inicial de cada eje*/
			if(pdataEEprom[i]!=rxAxissetting[(sizeof(tAxisSettings)*Axis)+i])
			{
				Diferentes=1;
				#ifdef DEBUGSETUP
				printf("CopiartohAxissetting=> Son diferentes\n");
				#endif	
                                /*Para salirce de las dos instrucciones for */
				Axis=DOF;
				i=sizeof(tAxisSettings);
			}
		}
		
		
	}
	
	
	if(Diferentes==1)
	{
		//Activo el indicador de configuracion por EEprom pero primero leo el mismo
		//para ver si esta activado y no sobre escribirlo inecesariamente.
		eeprom_read_block((void*)&Ram_Reconf_Indicator,(const void*)&EEprom_Reconf_Indicator,sizeof(int16));
		if(!Ram_Reconf_Indicator)
		{
			Ram_Reconf_Indicator=1;
			//Actualizo a el indicador
			//Fuente,Destino
			eeprom_write_block((const void*)&Ram_Reconf_Indicator,(void*)&EEprom_Reconf_Indicator,sizeof(int16));
		}
		
		//Reconfiguro  el apuntador rxAxissetting para poder trabajar como apuntador a tAxisSettings
		prxAxissetting=(tAxisSettings*)rxAxissetting;
		
		Buffersize=sizeof(tAxisSettings);
		#ifdef DEBUGSETUP
		printf("CopiartohAxissetting=> Buffersize= %d\n",Buffersize);
		#endif	
		/*Copio la nueva configuracion a al eeprom y actualizo la configuracion en hAxis[Axis]..axisSetting */
		for(Axis=0;Axis<DOF;Axis++) 
		{
			
			#ifdef DEBUGSETUP
			printf("CopiartohAxissetting=> Copiando la informacion en eeprom\n");
			#endif	
			//Copio la configuracion de ejes a la eepron
			eeprom_write_block((const void*)&prxAxissetting[Axis],(void*)&EEprom_axisSettings[Axis],sizeof(tAxisSettings));
			
			//Actualizo hAxis[Axis]..axisSettings a la estructura axisSettings en EEprom
			eeprom_read_block((void*)&hAxis[Axis].axisSettings,(const void*)&EEprom_axisSettings[Axis],sizeof(tAxisSettings));
		}
		
		#ifdef DEBUGSETUP
		//Imprimo el resultado
		//PrinthAxis(hAxis);
		#endif		
		
	}
	
	return Diferentes;
}
/**********************************************************************************************************************/
void GetAxissetting(uint8* TXAxissetting)
{
	int i,Buffersize,Axis,fin;
	int offset=0;
	uint8* pdata;

	
	/*Obtengo el apuntador al primer dato, datos secuenciales, pero no son entre Axis*/
    
	Buffersize=sizeof(tAxisSettings);
	
    for(Axis=0;Axis<DOF;Axis++) 
	{
	    
		offset=Buffersize*Axis;
		#ifdef DEBUGSETUP
		printf("GethAxissetting=> Actualizando Axis %d con offset = %d\n",Axis,offset);
		#endif	
		pdata=(uint8*)&hAxis[Axis].axisSettings;
		fin=Buffersize+offset;
		for(i=offset;i<fin;i++) 
			TXAxissetting[i]=pdata[i-offset];  //desde pdata[0] hasta pdata[buffersize]
	}

}



void PrintAxissetting(tAxisSettings axisSettings)
{
	int test;
	
		printf("PrintAxissetting=> En  axisSettings.maxAxisAngle= %d \n",axisSettings.maxAxisAngle);
		printf("PrintAxissetting=> En  axisSettings.minAxisAngle= %d \n",axisSettings.minAxisAngle);
		printf("PrintAxissetting=> En  axisSettings.axisEnabled= %d \n",axisSettings.axisEnabled);
		printf("PrintAxissetting=> En  axisSettings.interruptEnable= %d \n",axisSettings.interruptEnable);
		printf("PrintAxissetting=> En  axisSettings.switchEnable= %d \n",axisSettings.switchEnable);
		printf("PrintAxissetting=> En  axisSettings.encoderEnable= %d \n",axisSettings.encoderEnable);
		printf("PrintAxissetting=> En  axisSettings.ustepsPerStep= %d \n",axisSettings.ustepsPerStep);
		printf("PrintAxissetting=> En  axisSettings.motorStepsPerRevolution= %d \n",axisSettings.motorStepsPerRevolution);
		printf("PrintAxissetting=> En  axisSettings.encoderStepsPerRevolution= %d \n",axisSettings.encoderStepsPerRevolution);
		printf("PrintAxissetting=> En  axisSettings.TrackingWindow= %d \n",axisSettings.TrackingWindow);
		printf("PrintAxissetting=> En  axisSettings.SettleWindow= %d \n",axisSettings.SettleWindow);
		printf("PrintAxissetting=> En  axisSettings.SettleTime= %d \n",axisSettings.SettleTime);
		printf("PrintAxissetting=> En  axisSettings.motorPower= %d \n",axisSettings.motorPower);
		printf("PrintAxissetting=> En  axisSettings.numberPhases= %d \n",axisSettings.numberPhases);
		printf("PrintAxissetting=> En  axisSettings.homeSwitchHandling= %d \n",axisSettings.homeSwitchHandling);
		printf("PrintAxissetting=> En  axisSettings.gearNominator= %d \n",axisSettings.gearNominator);
		printf("PrintAxissetting=> En  axisSettings.gearDenominator= %d \n",axisSettings.gearDenominator);
		printf("PrintAxissetting=> En  axisSettings.ErrorLimit= %ld \n",axisSettings.ErrorLimit);
		printf("PrintAxissetting=> En  axisSettings.limitAcceleration= %ld \n",axisSettings.limitAcceleration);	  
		printf("PrintAxissetting=> En  axisSettings.limitVelocity= %ld \n",axisSettings.limitVelocity);
		printf("PrintAxissetting=> En  axisSettings.limitJerk= %ld \n",axisSettings.limitJerk);
		printf("PrintAxissetting=> En  axisSettings.maxAcceleration= %ld \n",axisSettings.maxAcceleration);	 
		printf("PrintAxissetting=> En  axisSettings.maxVelocity= %ld \n",axisSettings.maxVelocity);
		printf("PrintAxissetting=> En  axisSettings.minVelocity= %ld \n",axisSettings.minVelocity);	  
		scanf("%d",&test);
	
}



/*
void PrinthAxis(PMDAxisHandle* hAxis)
{
	int test;
	
	printf("PrinthAxis=> Comprobando copia\n");
	
		for(int Axis=0;Axis<DOF;Axis++) 
		{
			printf("PrinthAxis=> En hAxis[%d].axisSettings.maxAxisAngle= %d \n",Axis,hAxis[Axis].axisSettings.maxAxisAngle);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.minAxisAngle= %d \n",Axis,hAxis[Axis].axisSettings.minAxisAngle);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.axisEnabled= %d \n",Axis,hAxis[Axis].axisSettings.axisEnabled);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.interruptEnable= %d \n",Axis,hAxis[Axis].axisSettings.interruptEnable);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.switchEnable= %d \n",Axis,hAxis[Axis].axisSettings.switchEnable);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.encoderEnable= %d \n",Axis,hAxis[Axis].axisSettings.encoderEnable);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.ustepsPerStep= %d \n",Axis,hAxis[Axis].axisSettings.ustepsPerStep);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.motorStepsPerRevolution= %d \n",Axis,hAxis[Axis].axisSettings.motorStepsPerRevolution);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.encoderStepsPerRevolution= %d \n",Axis,hAxis[Axis].axisSettings.encoderStepsPerRevolution);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.TrackingWindow= %d \n",Axis,hAxis[Axis].axisSettings.TrackingWindow);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.SettleWindow= %d \n",Axis,hAxis[Axis].axisSettings.SettleWindow);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.SettleTime= %d \n",Axis,hAxis[Axis].axisSettings.SettleTime);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.motorPower= %d \n",Axis,hAxis[Axis].axisSettings.motorPower);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.numberPhases= %d \n",Axis,hAxis[Axis].axisSettings.numberPhases);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.homeSwitchHandling= %d \n",Axis,hAxis[Axis].axisSettings.homeSwitchHandling);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.gearNominator= %d \n",Axis,hAxis[Axis].axisSettings.gearNominator);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.gearDenominator= %d \n",Axis,hAxis[Axis].axisSettings.gearDenominator);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.ErrorLimit= %ld \n",Axis,hAxis[Axis].axisSettings.ErrorLimit);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.limitAcceleration= %ld \n",Axis,hAxis[Axis].axisSettings.limitAcceleration);	  
			printf("PrinthAxis=> En hAxis[%d].axisSettings.limitVelocity= %ld \n",Axis,hAxis[Axis].axisSettings.limitVelocity);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.limitJerk= %ld \n",Axis,hAxis[Axis].axisSettings.limitJerk);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.maxAcceleration= %ld \n",Axis,hAxis[Axis].axisSettings.maxAcceleration);	 
			printf("PrinthAxis=> En hAxis[%d].axisSettings.maxVelocity= %ld \n",Axis,hAxis[Axis].axisSettings.maxVelocity);
			printf("PrinthAxis=> En hAxis[%d].axisSettings.minVelocity= %ld \n",Axis,hAxis[Axis].axisSettings.minVelocity);	  
			scanf("%d",&test);
		}

}





*/

