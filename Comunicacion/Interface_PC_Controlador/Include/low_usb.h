#ifndef __LOW_USB_H__
#define __LOW_USB_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      low_usb.h
// Ubicación fuente: C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Contorlador\C\low_usb.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Funciones que permiten la comunicación entre el microcontrolador ATMega1280 y el Módulo GPIF 
// del controlador de periféricos USB CY_68013, con el fin de enviar y recibir datos desde/hacia el PC.
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

#include "uC_setup.h"
#include "types.h"


/** \defgroup Interface_uC_USB_CY68013	COMUNICACIÓN MICROCONTROLADOR - USB
    \ingroup Sistema_de_Comunicación
	\brief Funciones que permiten la comunicación entre el microcontrolador ATMega1280 y el Módulo GPIF 
			del controlador de periféricos USB CY_68013, con el fin de enviar y recibir datos desde/hacia el PC.
	
	\code #include <low_usb.h> \endcode
*/
  
	/** \ingroup Interface_uC_USB_CY68013
    \brief Función que lee 16 bits de datos provenientes del Controlador de Periféricos USB. Estos Datos son leídos
	 a través de dos puertos del microcontrolador.
	@param datoh  Variable que guarda la parte alta de los 16 bits de datos.
	@param datol  Variable que guarda la parte baja de los 16 bits de datos.
	@return rc  Código de Error. rcOK ó rcTimeoutReadingData error de timeout.*/	
	int16 usb_read(uint8* datoh,uint8* datol);

	/** \ingroup Interface_uC_USB_CY68013
	\brief Función que realiza una operación de escritura de 16 bits en el Controlador de Periféricos USB. 
	@param datoh  Variable que guarda la parte alta de los 16 bits de datos.
	@param datol  Variable que guarda la parte baja de los 16 bits de datos.
	@return  rc  Código de Error. rcOK ó rcTimeoutSendingData error de timeout.*/
	int16 usb_write(uint8 datoh,uint8 datol);
	
	/** \ingroup Interface_uC_USB_CY68013
	\brief Función que envía los datos apuntados por Buffer al controlador de periféricos USB. El tamaño 
	de cada dato es de 8 bits y la cantidad total de los datos es size. 
	En el caso de que size sea impar, el último byte es llenado con ceros's.
	@param Buffer  Variable que apunta a la dirección donde están los datos.
	@param size  Tamaño total de datos.
	@return rc  Código de Error. rcOK ó rcTimeoutSendingData error de timeout. */
	int16 Envio_USB(uint8* Buffer,int16 size);

	/** \ingroup Interface_uC_USB_CY68013
	\brief Función que recibe los datos del Controlador de Periféricos USB y los almacena en Buffer. 
	El tamaño de cada dato es de 8 bits y la cantidad total de los datos es size. En el 
	caso de que size sea impar, el último byte que se recibe es NULL.
	@param Buffer  Variable que apunta a la dirección donde están los datos.
	@param size  Tamaño total de datos.
	@return rc  Código de Error. rcOK ó rcTimeoutReadingData error de timeout. */
	int16 Leo_USB(uint8* Buffer,int16 size);

  
#endif
