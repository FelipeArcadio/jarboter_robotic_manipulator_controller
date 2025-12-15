#ifndef __HD44780_H__
#define __HD44780_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      hd44780.h
// Ubicacion fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\hd44780.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Manejador de la pantalla de cristal líquido con el controlador HD44780.
//
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
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "hd44780.h"
 
 /** \defgroup lcd PANTALLA DE CRISTAL LÍQUIDO
	\ingroup Interface_uC_Peripherical  
	\brief Estos módulos describen el controlador para la pantalla de cristal líquido y las funciones
	desarrolladas para el displiegue de mensajes en el LCD (Pantalla de cristal líquido).
	*/
 
 
 /** \defgroup LCD_Driver  CONTROLADOR
	\ingroup lcd
	\brief Manejador de la pantalla de cristal líquido con el controlador HD44780..
	
	\code #include <hd44780.h> \endcode */
 
 	

	/** \ingroup LCD_Driver
	\brief Envía un byte hacia el LCD (Pantalla de cristal líquido).
	@param b Byte que será enviado.
	@param rs Es la señal RS (Register Select). Rs= 0 Selecciona Instruccion Register
												 RS= 1 Selecciona Data Register.*/
	void	hd44780_outbyte(uint8_t b, uint8_t rs);


	/** \ingroup LCD_Driver
	
	\brief Lee un byte desde el controlador del LCD (Pantalla de cristal líquido).
	@param rs Es la señal RS (Register Select). Rs= 0 Selecciona busy flag (bit 7) y address counter,
												 RS= 1 Selecciona Data Register.*/
	uint8_t	hd44780_inbyte(uint8_t rs);


	/** \ingroup LCD_Driver
	\brief Espera a que el busy flag (bit 7) sea cero. */	
	void	hd44780_wait_ready(void);

	/** \ingroup LCD_Driver
	\brief Inicializa el controlador del LCD (Pantalla de cristal líquido).*/
	void	hd44780_init(void);

	/** \ingroup LCD_Driver
	\brief Envía un comando al controlador LCD (Pantalla de cristal líquido).*/
	#define hd44780_outcmd(n)	hd44780_outbyte((n), 0)

	/** \ingroup LCD_Driver
	\brief Envía un byte de datos al controlador LCD (Pantalla de cristal líquido).*/
	#define hd44780_outdata(n)	hd44780_outbyte((n), 1)

	/** \ingroup LCD_Driver
	\brief Lee el Address counter y el busy flag desde el controlador LCD (Pantalla de cristal líquido).*/
	#define hd44780_incmd()		hd44780_inbyte(0)

	/** \ingroup LCD_Driver
	\brief Lee el dato actual del LCD (Pantalla de cristal líquido).*/
	#define hd44780_indata()	hd44780_inbyte(1)

	/** \ingroup LCD_Driver
	\brief Define el comando de clear. */
	#define HD44780_CLR			0x01

	/** \ingroup LCD_Driver
	\brief Define el comando de home.*/
	#define HD44780_HOME		0x02

	/** \ingroup LCD_Driver
	\brief Selecciona el modo de entrada (entry mode). inc determina si el address counter se autoincrementa.
	shift selecciona corrimiento automático del display.*/
	#define HD44780_ENTMODE(inc, shift)	\
	(0x04 | ((inc)? 0x02: 0) | ((shift)? 1: 0))

	/** \ingroup LCD_Driver
	\brief Selecciona display on/off, cursor on/off, cursor blinking on/off.*/	
	#define HD44780_DISPCTL(disp, cursor, blink) \
	(0x08 | ((disp)? 0x04: 0) | ((cursor)? 0x02: 0) | ((blink)? 1: 0))

	/** \ingroup LCD_Driver
	\brief Habilita corrimiento a la izquierda o derecha del cursor o del display.	
	con shift = 1, corrimiento del  display izquierda o derecha.
	con shift = 0, Mueve el cursor a la izquierda o derecha.*/
	#define HD44780_SHIFT(shift, right) \
	(0x10 | ((shift)? 0x08: 0) | ((right)? 0x04: 0))

	/** \ingroup LCD_Driver
	\brief Función de configuración.	
	if8bit selecciona entre datos de 8 bits o 4 bits.
	Twoline selecciona si el display funcionará con dos líneas o una línea.
	font5x10 selecciona la resolución de la fuente entre 5x10 puntos o 5x8 puntos.*/ 
	#define HD44780_FNSET(if8bit, twoline, font5x10) \
	(0x20 | ((if8bit)? 0x10: 0) | ((twoline)? 0x08: 0) | \
	((font5x10)? 0x04: 0))

	/** \ingroup LCD_Driver
	\brief Fija la dirección del generador de caracteres a addr. */
	#define HD44780_CGADDR(addr) \
	(0x40 | ((addr) & 0x3f))

	/** \ingroup LCD_Driver
	\brief Fija la  próxima dirección del display (RAM) en addr.*/
	#define HD44780_DDADDR(addr) \
	(0x80 | ((addr) & 0x7f))
	
	
#endif
