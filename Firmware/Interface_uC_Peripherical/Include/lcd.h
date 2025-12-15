#ifndef __LCD_H__
#define __LCD_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      lcd.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\lcd.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Capa superior del controlador LCD. Funciones que inicializan el LCD (Pantalla de cristal líquido) y envío de 
// mensajes para su visualización.
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

#include "lcd.h"

 
 /**\defgroup funciones_lcd FUNCIONES
	\ingroup lcd
	\brief Capa superior del controlador LCD. Funciones que inicializan el LCD (Pantalla de cristal líquido) y envío de 
	mensajes para su visualización.
	\code #include <lcd.h> \endcode */
 
 /* @{ */



 /**\ingroup funciones_lcd
	\brief Inicializa el LCD (Pantalla de cristal líquido) y realiza reset por Software .*/
	void	lcd_init(void);


 /** \ingroup funciones_lcd
	\brief Envía un caracter por el LCD .
	@param c Caracter que se desea colocar en el LCD.
	@param *stream Apuntador al archivo. 
	@return Retorna 0 Cuando es ejecutado corectamente. */
	int	lcd_putchar(char c, FILE *stream);



 /* @} */
#endif