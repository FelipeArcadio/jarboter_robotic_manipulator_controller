#ifndef __MEM_H__
#define __MEM_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      mem.h
// Ubicación fuente: C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Contorlador\C\mem.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Funciones que manejan la memoria RAM, reservando espacio de direccionamiento o calculando 
// la cantidad de memoria disponible.
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined _WIN32
#include <avr/io.h>
#include <types.h>
#endif

#include <stdlib.h> 
    
#ifndef _AVR_IO_H_
	#include <stdio.h>				//Entrada y salida estandar
	#include <string.h>				//manejo de caracteres
	#include <malloc.h>
	#include <memory.h>
	#include <windows.h>			//funciones basicas de ventanas graficas
	#include <windowsx.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Manejador_RAM	GESTIÓN DE MEMORIA
    \ingroup Protocolo_comunicacion_PC
	
	\brief Funciones que manejan la memoria RAM, reservando espacio de direccionamiento o calculando 
			la cantidad de memoria disponible.
	
	\code #include <men.h> \endcode
*/

/* @{ */ 

	/** \ingroup Manejador_RAM
    \brief Permite reservar memoria RAM dinámicamente.
	@param size_t x  Indica la cantidad en bytes de memoria a reservar.*/
	
	void*  declarar(size_t x);
	
	#ifdef _AVR_IO_H_
	/** \ingroup Manejador_RAM
	\brief Calcula la cantidad de memoria disponible, en tiempo de ejecución. 
	@return rc  Número entero que representa la cantidad de bytes disponibles en un instante determinado.*/
	uint16 maxavail(void);
	#endif
/* @} */

#if defined(__cplusplus)
}
#endif

#endif