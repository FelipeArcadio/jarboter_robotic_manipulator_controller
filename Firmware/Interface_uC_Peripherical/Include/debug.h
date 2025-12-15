#ifndef __DEBUG_H__
#define __DEBUG_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      debug.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\debug.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Funciones de depuración que se utiliza para enviar cadenas de texto por
//	la interface serial y la pantalla de cristal líquido. 
//	Además estas funciones sirven para enviar mensajes de error audivisuales.
//	auditivos.
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




	
/** \defgroup debug FUNCIONES DE DEPURACIÓN
	\ingroup Programa_firmware
	\brief Funciones de depuración que se utiliza para enviar cadenas de texto por
	la interface serial y la pantalla de cristal líquido. 	
	Además estas funciones sirven para enviar mensajes de error audivisuales.*/
	
	
	/** \defgroup buzzer ALARMA AUDITIVA
	\ingroup debug  
	\brief Funciones para el manejo de la alarma auditiva que está conformada
	por un buzzer.
	\code #include <debug.h> \endcode 
	*/
	
	/** \defgroup led PILOTOS DE VISUALIZACIÓN
	\ingroup debug  
	\brief Funciones para el manejo de los tres pilotos de visualización LEDS. Los colores
	de los pilotos de visualización son: Rojo, Verde. Amarillo.
	\code #include <debug.h> \endcode 
	*/





/** \ingroup buzzer
 \brief Función de retardo de 1 segundo. Utilizada para propósitos generales. */
extern void delay_1s(void);

/** \ingroup led
\brief Función para depuración y diagnóstico. Prende un Led
 @param color El led que se prenderá.
				Verde, Rojo ó Amarillo.*/
extern void Led_on (tLeds color);

/** \ingroup led
 \brief Función para depuración y diagnóstico. Apaga un Led
 @param color El led que se apagará..
				Verde, Rojo ó Amarillo.*/
extern void Led_off (int16 color);

/** \ingroup buzzer
 \brief Función para depuración y diagnóstico. Emite un sonido
 @param valor_s Tiempo en segundos del sonido emitido */
 extern void buzzer(uint16 valor_s); 
 
 /** \ingroup buzzer
 \brief Función para depuración y diagnóstico. Emite un sonido
 @param valor_ms Tiempo en milisegundos en que durara el sonido */
 extern void buzzer_ms(uint32 valor_ms); 
 
 /** \ingroup buzzer
 \brief Función que inicia una secuencia de pruebas en el sistema*/
 extern void Test_perifericos(void);

#endif
