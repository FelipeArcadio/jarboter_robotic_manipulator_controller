#ifndef __UART_H__
#define __UART_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      uart.h
// Ubicaci�n fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\uart.c
//
// Proyecto: JARBOTER V.2.1. Controlador Rob�tico de 5 grados de libertad
//  
// Prop�sito General:	
// Estas funciones desarrollan la comunicaci�n con la UART integrada en el microcontrolador ATMega1280.
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

#include "uart.h"

/** \defgroup Uart CONTROLADOR INTERFAZ RS232
	\ingroup Interface_uC_Peripherical
	\brief Estas funciones desarrollan la comunicaci�n con la UART integrada en el microcontrolador ATMega1280.
	
	
	\code #include <uart.h> \endcode */

/* @{ */


/** \ingroup Uart
	\brief UART baud rate.*/
#define UART_BAUD  9600


/** \ingroup Uart
	\brief Funci�n  que desarrolla la inicializaci�n de la UART. inicializa la UART a 9600 Baudios, tx/rx, 8N1. */
void uart_init(void);


/** \ingroup Uart
	\brief Env�a un caracter a la UART.
	@param c Caracter que se desea colocar en la UART.
	@param *stream Apuntador al Archivo. 
	@return Retorna 0 Cuando es ejecutado corectamente. */
int	uart_putchar(char c, FILE *stream);


/** \ingroup Uart
	 \brief Tama�o del buffer utilizado por la funci�n uart_getchar().*/ 
#define RX_BUFSIZE 80

/** \ingroup Uart
	\brief  Recepci�n de un caracter desde la UART. La recepci�n actual	es line-buffered, y un caracter se retorna 
	desde el buffer en cada invocaci�n.*/
int	uart_getchar(FILE *stream);

#endif

/* @} */
