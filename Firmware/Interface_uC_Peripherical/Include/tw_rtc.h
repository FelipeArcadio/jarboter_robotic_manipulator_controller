#ifndef __TW_RTC_H__
#define __TW_RTC_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      tw_rtc.h
// Ubicacion fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\tw_rtc.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Estas funciones desarrollan operaciones de lectura y escritura sobre los registros de un
//	RTC de referencia ISL12027
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

#include "tw_interface.h"

	
	
/** \defgroup RTC_ISL1207 CONTROLADOR DEL RELOJ DE TIEMPO REAL
	\ingroup Interface_uC_Peripherical
	\brief Estas funciones desarrollan operaciones de lectura y escritura sobre los registros de un
	reloj de tiempo real de referencia ISL12027.
	
	\code #include <tw_rtc.h> \endcode	*/

	/* @{ */
	
	/** \ingroup RTC_ISL1207
	\brief TWI address for ISL12027 Real Time Clock 
	
	1 1 0 1 1 1 1 R/~W	 access The Control Clock Registers (CCR)*/
	#define TWI_SLA_RTC			0xDE

	/** \ingroup RTC_ISL1207
	\brief Define la dirección del Status Register dentro de los registros de control del 
	rejoj (CCR)*/
	#define SR_CCR				0x003F	
	
	
	/** \ingroup RTC_ISL1207
	\brief Define el Registro de PWR de Control del RTC.
	Si SBIB es 1 deshabilita el bus serial en modo de Battery Backup (Alimentación por batería)
	BSW determina las condiciones para cambiar entre VDD y Battery Backup. Hay dos opciones:
	
	Opcion 1. Standard: BSW = 0,
	Opcion 2. Legacy/Modo por defecto: BSW=1
	*/	
	#define RTC_PWR(SBIB,BSW) (0x00 | ((SBIB)? 0x80: 0) | ((BSW)? 0x40: 0))
	
	/** \ingroup RTC_ISL1207
	\brief Configuración de la Alarma.
	AL0E Habilita la alarma. IM define si la Alarma es periódica o no.
	*/
	#define RTC_INT(AL0E,IM) (0x00 | ((AL0E)? 0x20: 0) | ((IM)? 0x80: 0))
	
	/** \ingroup RTC_ISL1207
	\brief Esta función realiza los pasos preliminares que se deben hacer para escribir en los registros del 
	RTC.
	Antes de escribir en los registros se debe realizar los siguientes pasos:
	1. Escribir 02h al Status Register. Esto coloca en "1" el Write Enable Latch bit (WEL)
	2. Escribir 06h al Status Register. Esto coloca en "1" al Register Write Enable Latch
	(RWEL) y el (WEL) bit.
	
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos escritos.	*/
	int16 isl1207_write_CCR(void);
		
	/** \ingroup RTC_ISL1207
	\brief Esta función escribe en los registros de reloj y control (CCR) del Reloj de Tiempo Real (RTC) 
	una estructura tipo Fecha.
	@param Fecha Estructura Tipo Fecha que se escribirá en el RTC
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos escritos.	*/
	int16 write_Fecha(tFecha Fecha);
		
	/** \ingroup IRTC_ISL1207
	\brief Esta función retorna la fecha leída por el RTC.
	@param Fecha Puntero a una estructura tipo Fecha en donde se almacenan los datos leídos 
	del RTC.
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos leídos.	*/
	int16 Read_Fecha(tFecha *Fecha);
	
	/** \ingroup IRTC_ISL1207
	\brief Esta función retorna el Registro de Estado (Status Register) del RTC.
	@param SR Variable donde se almacena el valor del registro de estado.
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos leídos.	*/
	int16 Read_SR(uint8* SR);	
	
	/** \ingroup RTC_ISL1207
	\brief Esta funcion escribe en los registros control  del Reloj de Tiempo Real (RTC) una estructura
	de tipo control_RTC. En la estructura control se configuran los siguientes parámetros del RTC:
	\verbatim
	-Compensación del Oscilador ( Tanto análogo como digital).
	-Habilitación o deshabilitación de la Interface Serial en Modo de Backup.
	-Determinación de las condiciones de cambio entre el modo de Backup y Vdd.
	-Selección del nivel de Voltaje de umbral del Reset.
	-Configuración de la Alarma.
	\endverbatim
	
	@param ctr Estructura de Tipo Control que se escribe en el RTC.
	@return rv Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos escritos.	*/
	int16 write_control(tcontrol_RTC ctr);
	
	/** \ingroup IRTC_ISL1207
	\brief Esta funcion retorna los registros de control del RTC.
	@param ctr Puntero a una estructura tipo control_RTC en donde se almacenan los registros
	de control del RTC.
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos leídos.	*/
	int16 read_control(tcontrol_RTC *ctr);
	
	/** \ingroup IRTC_ISL1207
	\brief Esta funcion configura la Fecha y/u hora para efectuar una alarma.
	@param alarma Estructura tipo Alarma.
	@return rv Si hay un error en la transmisi'on rv=-1 de lo contrario retorna la cantidad de
			datos escritos.	*/
	int16 write_alarm(tAlarm alarma);
	
	/** \ingroup IRTC_ISL1207
	\brief Esta funcion retorna la fecha y/u hora de configuracion de la Alarma.
	@param alarma Puntero a una estructura tipo alarma en donde se almacenaran los registros
	de alarma del RTC
	@return rv Si hay un error en la transmisi'on rv=-1 de lo contrario retorna la cantidad de
			datos le'idos.	*/
	int16 read_alarm(tAlarm *alarma);
	
	/** \ingroup IRTC_ISL1207
	\brief Esta funcion configura el reloj de la siguiente manera:
	\verbatim
	1.Es deshabilitado el bus Serial cuando el RTC se encuentra alimentado por la batería. 
	2.Las condiciones
	de cambio entre el modo de Bateráa (Battery Backup-VBAT) y el Modo Normal (VDD) es la opcion 2: Legacy que
	consiste en comparar los voltajes (VDD y VBAT) y el RTC operara con el que presenta mayor voltaje.
	3.DTR (Digital Trimming Register)=0.
	4.ATR (Analog Trimming Register)=0 lo que permite una Capacitancia de 12.5pF
	5.La Alarma es deshabilitada inicialmente.
	6. Watchdog Timer es deshabilitado y no hay protección de la EEPROM interna
	(esta EEPROM no se utiliza)
	
	\endverbatim
	*/
	void rtc_init(void);
	
	
	
	/** \ingroup IRTC_ISL1207
	\brief Esta función ejecuta una tarea determinada cuando se cumple un evento de alarma.
	@param TASK Tarea a Realizar ej: Cerrar gripper, abrir gripper, ejecutar trayectoria generada
	por robomosp, ejecutar trayectoria generada por teach pendant (Poses)
	*/
	void RCT_task(uint8 TASK);
	
	
	/* @} */
	


#endif

