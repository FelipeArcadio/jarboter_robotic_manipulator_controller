#ifndef __MACROS_H__
#define __MACROS_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      macros.h
// Ubicación fuente: No tiene
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad.
//  
// Propósito General:	
// Declaración de definiciones Macro específicas para el Microcontrolador.
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

/**Macro definiciones interface uC_PMD */
#define		HostSlct1				PD4
#define		HostSlct2				PD5			
#define		HostSlct3				PD6			


#define		HostControl				PORTG
#define		HostCmd 				PG3
#define		HostWrite				PG4
#define		HostRead				PG5
#define		Reset					PE3

#define		HostData				PORTJ
#define		ReadData				PINJ
#define		Control_Port			DDRJ

/**Macro definiciones interface uC_PC(USB) */

#define		USB_EN					PH2			
#define		USB_WRITE				PH3
#define		USB_READ				PH4
#define		USB_FLAG_READ			PINH5
#define		USB_FLAG_WRITE			PINH6
#define		USB_READY				PINH7

#define		DATA_USB_WRITE_HIGH		PORTK 
#define		DATA_USB_READ_HIGH		PINK 
#define		CONTROL_USB_HIGH		DDRK

#define		DATA_USB_WRITE_LOW		PORTL
#define		DATA_USB_READ_LOW		PINL
#define		CONTROL_USB_LOW			DDRL

/** ALARMA AUDITIVA*/

#define		Alarma					PD7

/** ALARMA BOTON PARADA DE EMERGENCIA*/

#define		Estado_Boton_Emergencia		PIND2


/** PILOTOS DE VISUALIZACIÓN */


#define		ROJOb		  	 	PB7  //AMARILLO
#define		VERDEe				PE2  //VERDE
#define		AMARILLOe			PE7  //RoJO

#define		LED0         			AMARILLOe  //AMARILLO
#define		LED1					VERDEe  //VERDE
#define		LED2					ROJOb  //RoJO


/** GRIPPER */

#define     GRIPPERCTL    		  	PORTF
#define		DER				    	PF0
#define		IZQ						PF1
#define		FINDER					PINF6
#define		FINIZQ					PINF7



/* HD44780 LCD port connections */
#define 	HD44780_PORT 			B
#define 	HD44780_RS 				PORT6
#define 	HD44780_RW 				PORT4
#define 	HD44780_E  				PORT5
/* The data bits have to be in ascending order. */
#define 	HD44780_D4 				PORT0
#define 	HD44780_D5 				PORT1
#define 	HD44780_D6 				PORT2
#define 	HD44780_D7 				PORT3

#endif


