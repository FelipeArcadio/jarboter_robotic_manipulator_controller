#ifndef __MESSAGES_H__
#define __MESSAGES_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      messages.h
// Ubicación fuente: C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Contorlador\C\messages.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Funciones que envían mensajes de depuración por la interface serial ó por la pantalla de cristal
//	líquido LCD.
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

#include "types.h" 

#if defined(__cplusplus)
extern "C" {
#endif



/** \defgroup Mensajes	MENSAJES
    \ingroup debug
	
	\brief Funciones que envían mensajes de depuración por la interface serial ó por la pantalla de cristal
	líquido LCD.
	
	\code #include <messages.h> \endcode
*/



	/** \ingroup Mensajes
    \brief Función que relaciona un eje (axis) representado con un entero por una trama que contiene  en ASCII 
	el respectivo nombre de la articulación.
	@param Axis Eje a evaluar. 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *AxisName(tAxis Axis);
	
	/** \ingroup Mensajes
    \brief Función que relaciona el campo CommandOP de la estructura tCommand por una trama que contiene en ASCII 
	el respectivo nombre del comando. Los nombres del comando se encuentran en tEnumCommand.
	@param command Estructura tCommand que contiene el campo a evaluar.
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *CommandMsg(tCommand command);
	
	/** \ingroup Mensajes
    \brief Función que relaciona packet por una trama que contiene en ASCII el respectivo nombre del paquete. 
	El nombre de la trama se encuentra en tEnumPacket.
	@param packet Paquete  a evaluar. 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *PacketMsg(tPacket packet);
	
	/** \ingroup Mensajes
    \brief Función que relaciona el código de error rc representado con un entero por una trama que 
	contiene en ASCII el respectivo nombre del  error.
	@param rc Código de error  a evaluar. 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *rcMsg(tReturnCode rc);
	
	/** \ingroup Mensajes
    \brief Función que relaciona la variable día  por una trama que contiene en ASCII el respectivo nombre del día.
	@param dia Día a evaluar. 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *Dayname(tdia dia);
	
	/** \ingroup Mensajes
    \brief Función que relaciona la variable mes por una trama que contiene en ASCII el respectivo nombre del mes.
	@param mes Mes a evaluar. 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *Monthname(tmes mes);
	
	/** \ingroup Mensajes
	\brief Función que relaciona la  variable Trace por una trama que contiene en ASCII el nombre de la 
	variable a muestrear por el controlador.  
	@param Trace Variable a evaluar .
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 * TraceName(PMDTraceVariable Trace);
	
	/** \ingroup Mensajes
	\brief Función que relaciona la variable Trace por una trama que contiene en ASCII el nombre del tipo de variable a 
	muestrear por el controlador.  
	@param Trace Tipo de variable a evaluar 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 * Tracetype(PMDTraceVariable Trace);



	// PMD specific messages
	#ifdef _AVR_IO_H_
	/** \ingroup Mensajes
	\brief Función que relaciona la variable opCode por una trama que contiene en ASCII el nombre del comando.  
	@param opCode Variable a evaluar. 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *PMDGetOpcodeText(PMDuint16 opCode);
	
	
	/** \ingroup Mensajes
	\brief Función que relaciona la variable errorCode por una trama que contiene en ASCII el nombre el error
	retornado por el controlador.  
	@param errorCode Variable a evaluar 
	@return uint8 * Apuntador a la cadena de respuesta.*/
	uint8 *PMDGetErrorMessage(PMDuint16 errorCode);
	#endif



#if defined(__cplusplus)
}
#endif

#endif
