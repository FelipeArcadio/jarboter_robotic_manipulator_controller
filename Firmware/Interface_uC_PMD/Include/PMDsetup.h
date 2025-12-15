#ifndef __PMDSETUP_H__
#define __PMDSETUP_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDsetup.h
// Ubicaciön fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDsetup.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Funciones de configuración y chequeo de las tarjetas controladoras de motores (PMDs).
//
// Compilado en:
//    avr-gcc 4.3.3 
//
// Author: Felipe Hurtado
//         David Osorio
//
// Version: 1.0
//  
// 
// Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** \defgroup PMDsetup  CONFIGURACIÓN DE LOS PMDs
	\ingroup control_mov
	\brief Funciones de configuración y chequeo de las tarjetas controladoras de motores (PMDs).
	\code #include <PMDsetup.h> \endcode */
		
	/* @{ */


	/** \ingroup PMDsetup
	\brief Arreglo de manejadores globales para todos los ejes del Robot. */
	PMDAxisHandle hAxis[DOF];  
	
	/**\ingroup PMDsetup 
	\brief Función que configura los manejadores de ejes (axis Handle). LLamar esta función una vez antes de 
	utilizar un manejador de eje.*/
	PMDresult initAxisHandles(void); 

	/**\ingroup PMDsetup 
	\brief Función que reinicia el Chipset PMD. Retorna códigos de error de PMD. Después del reset, limpia el bit de
	CommandError, situación que es causada por ejecutar ésta función.
	@param handle  Corresponde a un manejador de eje.
	@return Retorna un código de error de 16 bits (PMD Error Messages).*/
	PMDresult PMDChipsetReset(PMDAxisHandle* handle);

	/** \ingroup PMDsetup
	\brief Función básica que prueba la Conectividad, líneas de estado, checksum del chip PMD.
	Esta Función reinicia el Chipset y luego realiza un chequeo del checksum.	 
	@param PMDChipSet  Corresponde al chip PMD a probar:PMDChipset1, PMDChipset2 Y PMDChipset3.
	@return Retorna rcOK o un código de error.*/
	PMDresult PMDchipsetOK(PMDuint16 PMDChipSet); 	

	/** \ingroup PMDsetup
	\brief Comprueba la comunicación de las dos tarjetas controladoras de motores PMD. En caso de algún 
	error emite una alarma y una señal por los pilotos de visualización.
	*/	
	extern PMDresult selftestPMD(void); 
	
	/** \ingroup PMDsetup
	\brief Esta función realiza lo siguiente:
	Reinicia el conjunto de integrados PMD`s y calibra el tiempo de muestreo que se utiliza para los cálculos
	de velocidad y aceleración.
	Para Cada eje configura lo siguiente:
	- 1. Las unidades utilizadas para la trayectoria en micropasos.
	- 2. Las señales de los encodificadores ´ópticos ( A, B e INDEX) activas en alto.
	- 3. Se habilitan los interrruptores de fin de carrera. Esto significa que cuando ocurre un evento de 
		 fin de carrera el chipset generara una parada abrupta deteniendo el motor.
	- 4. El modo de parada del motor (suave, abrupta o ninguna).
	- 5. Habilita o desabilita el eje según la configuración de la estructura en tAxisSettings.
	- 6. Hablita o deshablilita el generador de trayectorias según la configuaración del eje en la estructura
	     tAxisSettings.
	- 7. Perfil de la Trayectoria (Trapezoidal). 
	- 8. Formato de salida para el manejo del motor (PWM 50/50).
	- 9. Número de fases  del motor según la configuración en la estructura tAxisSettings.
	- 10. Potencia de salida para el motor según la configuracion del porcentaje en la estructura tAxisSettings.
	- 11. El número de micropasos por paso según la configuración en la estructura tAxisSettings.
	- 12. Fija el tipo de encodificador óptico a utilizar por el eje (Incremental).
	- 13. Fija la relación del número de pulsos del encodificador por micropasos
	- 14. El valor absoluto del error de posición.(8 micropasos/Pulso de encoder).
	- 15. Limpia el error de posición.
	- 16. La habilitación de parada del motor cuando ocurre un error de movimiento.
	- 17. La fuente de comparación para determinar el fin del movimiento. En este caso se basa en los registros del 
		  generador de perfil de trayectoria, pudiendo configurarse con el valor del encodificador.
	- 18. La máxima aceleración, desaceleración, mínima velocidad y jerk según la configuración en la estructura
			tAxisSettings.
	- 19. Cual de las señales (INDEX O HOME) se utiliza para el disparo de captura de posición. Se utilizará la señal de
		HOME.
	- 20. Limpia el Event Status Register.
	- 21. Captura la posición en una variable temporal permitiendo que ocurra otra captura.
	- 22. Realiza un Update.
	
	@param Profile	Perfil de movimiento de los ejes. Puede ser: Trapezoidal Punto-a-Punto (PTP), Curva-S (PTP),
					Velocidad.
	@return Retorna rcOK o un código de error.
	*/
	extern PMDresult PMDsetupChipSets(PMDProfileMode Profile); 
	
	
	/** \ingroup PMDsetup
	\brief Reconfigura el PMD desde el USB. Esta función es como PMDsetupChipSets, pero sin volver a 
	ejecutar los siguientes comandos:
	- PMDChipsetReset.
	- PMDSetSampleTime.
	- PMDSetOutputMode.
	- PMDSetNumberPhases.
	- PMDSetEncoderSource.
	- PMDSetEncoderToStepRatio.
	- PMDSetAxisMode.
	- PMDSetMotorMode.
	@param Profile	Perfil de movimiento de los ejes. Puede ser: Trapezoidal Punto-a-Punto (PTP), Curva-S (PTP),
					Velocidad.
	@return Retorna rcOK o un código de error.*/
	extern PMDresult PMDsetupChipsets_USB(PMDProfileMode Profile);
	
	/**\ingroup PMDsetup 
	\brief Función que limpia los bits del Event Status Register de todos los ejes.*/
	extern void Reset_Even_Status_Register(void);
	
	/**\ingroup PMDsetup 
	\brief Función que limpia el error de posición de todos los ejes.*/
	extern void Clear_Position_Error(void);
		
	/**\ingroup PMDsetup 
	\brief Función que suma o sustrae un OFFSET a la posicion actual (encoder). Utilizada para
	procedimientos de Homming.
	@param axis 		El eje al cual se le ajustará la posición.
	@param posicion 	El valor de la posición de ajuste.*/
	extern void ajustar_posicion(int16 axis, PMDint32 posicion);
	
	/**\ingroup PMDsetup 
	\brief Función que suma o sustrae un OFFSET a la posicion actual (encoder). Utilizada para
	procedimientos de Homming.
	@param axis 		El encoder del eje al que se le ajustará la posición.
	@param posicion 	El valor de la posición de ajuste.*/
	extern void cargar_encoder(int16 axis, PMDint32 posicion);
	
	/**\ingroup PMDsetup 
	\brief Función que configura un breakPoint en un eje.  Para mayor información ver la seccion 5.2 del 
	Manual de Usuario de PMDs.
	@param Axis 			El eje al cual se le configura el breakpoint.
	@param breakpointID 	Puede ser 1 ó 2.
	@param breakAxis		El eje fuente del breakPoint.
	@param action			Acción del breakpoint, puede ser parada abrupta, parada suave.
	@param trigger			Es el disparo del breakpoint.
	@param breakpointvalue  Configura el umbral del disparo.*/
	extern void  Config_BreakPoint( int16 Axis, int16 breakpointID,int16 breakAxis, 
									int16 action, int16 trigger, PMDint32 breakpointvalue);
									
	/**\ingroup PMDsetup 
	\brief Copia la información contenida en el vector rxAxissetting[DOF] al campo  axisSettings de 
	los  manejadores de eje (hAxis[DOF]), con el fin de poder reconfigurar los PMDs utilizando 
	posteriormente la funcion PMDsetupChipSets. 
	@param rxAxissetting Apuntador al vector que contiene la información a ser copiada.
	@return Retorna 1 si la configuración hecha por el usuario es diferenta a la almacenada en 
        memoria EEPROM.*/
       
	extern int Axissetting2hAxisHandle(uint8* rxAxissetting);
	
	/**\ingroup PMDsetup 
	\brief Copia la información del campo axisSettings de los  manejadores de eje (hAxis[DOF]) y 
	la almacena en el vector TXAxissetting[DOF], por lo tanto la configuración actual del PMD queda 
	en TXAxissetting[DOF].
	@param TXAxissetting Apuntador al vector que se quiere actualizar.
	*/
	extern void GetAxissetting(uint8* TXAxissetting);
	
	/**\ingroup PMDsetup 
	\brief Envía por serial la información almacenada en la estructura tAxisSettings.
	@param axissetting estructura a imprimir.
	*/
	void PrintAxissetting(tAxisSettings axissetting);
	
	/**\ingroup PMDsetup 
	\brief Imprime el manejador de eje para fines de depuración.
	@param hAxis Manejador a imprimir.
	*/
	void PrinthAxis(PMDAxisHandle* hAxis);
	
	/* @} */

#endif
