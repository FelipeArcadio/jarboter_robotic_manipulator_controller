#ifndef __DIALOGORTC_H__
#define __DIALOGORTC_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      DialogoRTC.h
// Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\DialogoRTC.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// 	Conjunto de funciones que define el comportamiento del cuadro de diálogo "TAREAS PROGRAMADAS", que 
//	es accedido desde el menú principal, este cuadro de diálogo permite configurar ó 
//	leer la  hora y realizar tareas programadas.
//
// Compilado en:
//   Microsoft Visual Studio 6.0 API 
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
#include "USBfunc.h" 
#include "mem.h"     
#include "resource.h" 
#include "packetIO.h" 
#include "messages.h" 
#include "conversion.h"
#include <commctrl.h>           // namejo de controles graficos 




#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Cuadro_Dialogo_RTC CUADRO DE DIÁLOGO TAREAS PROGRAMADAS
	\ingroup GUI
	\brief Conjunto de funciones que define el comportamiento del cuadro de diálogo "TAREAS PROGRAMADAS", el cual 
	es accedido desde el menú principal. Este cuadro de diálogo permite configurar ó 
	leer la  hora y  realizar tareas programadas en el controlador Robotico.
	
	\code #include <DialogoRTC.h> \endcode 

*/
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Carga los "bitmap" sobre los botones incluídos en el cuadro de diálogo "TAREAS PROGRAMADAS". 		
		@param hDlg  Manejador de la ventana principal.
		@param hInstance  Es la instancia del programa actual, es necesaria para cargar los bitmaps que 
		tiene este cuadro de diálogo
	*/
	void cargoBotonRTC(HWND hDlg,HINSTANCE hInstance);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Llena automáticamente las barras desplegables del cuadro de diálogo "TAREAS PROGRAMADAS", con los días 
		de la semana,meses y años posibles.
		@param hDlg  Manejador de la ventana principal.
	*/
	void CBoxRTC(HWND hDlg);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Obtiene la hora desde el cuadro de diálogo "TAREAS PROGRAMADAS" y actualiza la estructura Fecha. 		
		@param hDlg  Manejador de la ventana principal.
		@param Fecha Apuntador a estructura tFecha que se quiere actualizar	
	*/
	void SetFechaVisual(HWND hDlg,tFecha* Fecha);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Actualiza los campos del cuadro de diálogo  "TAREAS PROGRAMADAS" con rxFecha, hora leída desde el
        controlador robótico mediante el  USB. 
		@param hDlg  Manejador de la ventana principal.
	*/
	void getFechaVisual(HWND hDlg);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Lee  la Hora en  rxFecha que está en formato BCD y luego actualiza la cadena Horacompleta.
		@param Horacompleta Apuntador a vector tipo char que se quiere modificar
		@param rxFecha Estructura  que contiene la Hora.
	*/
	void SetFechaTXT(char* Horacompleta, tFecha rxFecha);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Obtiene la Hora desde el char Horatxt y la trasforma en formato BCD actualizando las variables 
				tipo int hh, mm,ss 
		@param Horatxt  Apuntador a la cadena donde se quiere leer la hora.
		@param hh Apuntador de la variable que se quiere modificar con la información de Horatxt
		@param mm Apuntador de la variable que se quiere modificar con la información de Horatxt
		@param ss Apuntador de la variable que se quiere modificar con la información de Horatxt	
	*/
	void GetHORATXT(char* Horatxt,int* hh,int* mm,int* ss);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Convierte de formato BCD a texto.
		@param BCD  Variable tipo entero (int) en formato BCD
		@param Cadena  Apuntador a la cadena que se quiere actualizar con la información de la variable int BCD.
	*/
	void BCDtoCHAR(int BCD,char* Cadena);
	
	/** \ingroup Cuadro_Dialogo_RTC
		\brief Lee los campos que configuran la alarma en el cuadro de diálogo "TAREAS PROGRAMADAS" para actualizar 
				la variable txAlarma.
		@param hDlg  Manejador de la ventana principal.
		@param txAlarma  Apuntador a la estructura tAlarm que se quiere actualizar.
	*/
	void SetAlarma(HWND hDlg,tAlarm * txAlarma);



#if defined(__cplusplus)
}
#endif

#endif