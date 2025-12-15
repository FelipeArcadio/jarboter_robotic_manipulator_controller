#ifndef __UC_SETUP_H__
#define __UC_SETUP_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      uC_setup.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Configuracion_uC\C\uC_setup.c
//
// Proyecto: JARBOER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Esta función realiza la configuración de los puertos (entrada ó salida)	
//	y periféricos del microcontralador	ATMEGA1280. Además configura el modo en que 
//	se activan las interrupciones externas y los temporizadores (timer1 y timer2).
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

#include "macros.h"

/** \defgroup Programa_firmware	FIRMWARE DEL CONTROLADOR ROBÓTICO
	
	\brief Corresponde al código que corre en el microcontrolador ATmega 1280.
*/


/**	\defgroup uC_Setup   CONFIGURACIÓN MICROCONTROLADOR
	\ingroup Programa_firmware
	\brief Esta función realiza la configuración de los puertos (entrada ó salida)	
	y periféricos del microcontralador	ATMEGA1280. Además configura el modo en que 
	se activan las interrupciones externas y los temporizadores (timer1 y timer2).	
	
	\code #include <uC_setup.h> \endcode */
	
	
	/** \ingroup uC_Setup
	\brief Función que configura los puertos y periféricos del microcontrolador.*/
	void initCard(void); 
	
	unsigned int TIM16_ReadTCNTn(void);
	
	
#endif
