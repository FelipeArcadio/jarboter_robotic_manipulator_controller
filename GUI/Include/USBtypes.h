#ifndef __USBTYPES_H__
#define __USBTYPES_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      USBtypes.h
//  Ubicación fuente: No tiene
//
//  Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
//  Propósito General:	
//  Definición de la estructura encargada de controlar los procesos (hilos) que ejecutan las transferencias sobre 
//	el bus USB.
//
//  Compilado en:
//  Microsoft Visual Studio 6.0 API 
//
//  Author: Felipe Hurtado
//          David Osorio
//
//  Version: 1.0
//  
// 
//  Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>			//funciones basicas de ventanas graficas
#include <windowsx.h>			//funciones extendidas de ventanas graficas


#define MAX_DRIVER_NAME 64



/** 
	\ingroup tipo_datos_pc 
	\brief Definición de la estructura encargada de controlar los procesos (hilos) que ejecutan las transferencias 
			sobre el bus USB.

*/ 

typedef struct _THREAD_CONTROL
{
   HANDLE hDevice;			/*!< Manejador Del driver USB */
	
   DWORD Ioctl;				/*!< Código IO que indica que proceso ejecutar */				
   PVOID InBuffer;			/*!< Buffer de entrada de parámetros */
   DWORD InBufferSize;		/*!< Tamaño del buffer de entrada */
   PVOID OutBuffer;			/*!< Buffer de salida ó respuesta */
   DWORD OutBufferSize;		/*!< Tamaño del buffer de salida */
   DWORD BytesReturned;		/*!< Cantidad de bytes retornados */

   HANDLE completionEvent;  /*!< Indica cuando el proceso se completado */
   BOOL  status;			/*!< Indica el estado final del proceso */
} THREAD_CONTROL, *PTHREAD_CONTROL;


#endif