#ifndef __MENUFUNC_H__
#define __MENUFUNC_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      menuFunc.h
//  Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\menuFunc.c
//
//  Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
//  Próposito General:	
//  Conjunto de funciones que crean el menú principal del programa (INTERFAZ DE CONTROL).
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <windows.h>			//funciones basicas de ventanas graficas
#include <windowsx.h>			//funciones extendidas de ventanas graficas
#include "COMMONtypes.h" 
#include <stdio.h>
#include "resource.h"
#include "winuser.h"


#if defined(__cplusplus)
extern "C" {
#endif
/** \defgroup Funciones_de_menu MENU
	\ingroup GUI
	\brief Conjunto de funciones que crean el menú principal del programa (INTERFAZ DE CONTROL).
	\code #include <menuFunc.h> \endcode 

*/
	/** \ingroup Funciones_de_menu
		\brief Función que adiciona un botón con su respectivo mapa de bits, título  e Id (identificador cuando 
		se pulsa el botón), a un menú previamente definido.
		@param hmenu Manejador del menú donde se va a adicionar el botón.
		@param hInstance Apuntador a la memoria donde está corriendo el programa principal (instancia).
		@param hwnd Manejador de la ventana principal donde se quiere desplegar el menú.
		@param wInteger Identificador del mapa de bits (bitmap)  que se quiere cargar.
		@param uID Identificador del texto título, el cual se coloca debajo del dibujo (bitmap) y centrado.
		@param Item  identificador asigndado al botón cuando es pulsado.
		
	*/
	void InsertarBotonBMP(HMENU hmenu,HINSTANCE hInstance, HWND hwnd,WORD wInteger,UINT uID,UINT Item);
	
	
	/** \ingroup Funciones_de_menu
		\brief Función que construye un menú de seis botones utilizando la función InsertarBotonBMP,
		este menú corresponde al menú de inicio del programa principal (INTERFAZ DE CONTROL).
		@param  hwnd  Es el manejador de la ventana en donde se quiere insertar el menú.
		@param  hInstance  Apuntador a la memoria donde está corriendo el programa principal (instancia).
	*/
	void InsertarMenu( HWND hwnd,HINSTANCE hInstance);


#if defined(__cplusplus)
}
#endif

#endif