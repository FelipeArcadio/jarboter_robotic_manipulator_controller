#ifndef __DIALOGOTRACE_H__
#define __DIALOGOTRACE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      DialogoTRACE.h
// Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\DialogoTRACE.c
//
// Proyecto: JARBOTER V2.1.Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// 	Conjunto de funciones que define el comportamiento del cuadro de diálogo "TRAZADO". Este cuadro de diálogo 
//	permite configurar las características del muestreo del controlador robótico, tales como: el tipo de variable a
//	muestrear, el tiempo de muestreo, y la articulación a monitorear. 
//	Una vez el usuario llena los campos del cuadro de diálogo, la configuración es enviada hacia el controlador 
//	a través de la interfaz USB  con el fin de iniciar la adquisición de datos en la tarjeta controladora de motores 
//	almacenando los datos en su memoria RAM. 
//	Finalmente, mediante el botón de graficar del cuadro de diálogo, descarga los datos muestreados para graficarlos 
//	usando GNUPlot. 
//	
//	La interfaz permite configurar 8 variables de muestreo simultáneamente. Las articulaciones a muestrear son:
//	Cintura, Hombro, Codo y Pitch que pertenecen al PMD1 y Roll que pertenece al PMD2.
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
#include "mem.h"        
#include "resource.h"   
#include "packetIO.h"    
#include "messages.h"    
#include "conversion.h"  
#include <commctrl.h>     // namejo de controles graficos 




#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Cuadro_Dialogo_Trace CUADRO DE DIÁLOGO TRAZADO
	\ingroup GUI
	\brief Conjunto de funciones que define el comportamiento del cuadro de diálogo "TRAZADO". Este cuadro de diálogo 
	permite configurar las características del muestreo del controlador robótico, tales como: el tipo de variable a
	muestrear, el tiempo de muestreo, y la articulación a monitorear. 
	Una vez el usuario llena los campos del cuadro de diálogo, la configuración es enviada hacia el controlador 
	a través de la interfaz USB  con el fin de iniciar la adquisición de datos en la tarjeta controladora de motores 
	almacenando los datos en su memoria RAM. 
	Finalmente, mediante el botón de graficar del cuadro de diálogo, descarga los datos muestreados para graficarlos 
	usando GNUPlot. 
	
	La interfaz permite configurar 8 variables de muestreo simultáneamente. Las articulaciones a muestrear son:
	Cintura, Hombro, Codo y Pitch que pertenecen al PMD1 y Roll que pertenece al PMD2.
	\code #include <DialogoRTC.h> \endcode 

*/
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Coloca los nombres de los posibles ejes fuente (Cintura, Hombro, Codo, Pitch)  para las primeras cuatro 
		variables del PMD1 a muestrear.
		@param hDlg  Manejador de la ventana principal.
		@param nIDDlgItem Id del Combo box del eje fuente.
	*/
	void FillAxisTracePMD1(HWND hDlg,int nIDDlgItem);	
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Coloca la lista de los posibles tipos de variables a muestrear en el Combo box definido por nIDDlgItem
		@param hDlg  Manejador de la ventana principal.
		@param nIDDlgItem Id del Combo box donde se va colocar el listado de variables. 
	*/
	void CBoxTraceCommand(HWND hDlg,int nIDDlgItem);	
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Inicializa todos los campos del cuadro de diálogo "TRAZADO", tales como: El eje fuente para el muestreo, 
		el tiempo de muestreo por defecto y los tipos posibles de variables a muestrear. Apoyándose en las funciones 
		FillAxisTracePMD1 y CBoxTraceCommand.
		@param hDlg  Manejador de la ventana principal.
	*/	
	void CBoxTRACE(HWND hDlg);	
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Obtiene la información de los campos en el cuadro  de diálogo "TRAZADO" para actualizar las variables 
		de muestreo, que son: TraceTime, TraceTraj, Tracecomand[8];  campos de la estructura  tCommand y la variable 
		GraficSampletime utilizada para graficar.
		- Tracetime Almacena el tiempo de muestreo a configurar.
		- TraceTraj Indica si va a iniciar el muestreo cuando se ejecuta una trayectoria.
		- Tracecomand[8] Cada casilla tiene 2 campos adicionales: AxisTrace (Eje fuente) y TraceVar (Tipo de variable).
		- GraficSampletime  Tiempo de muestreo configurado por la aplicación.
		@param hDlg  Manejador de la ventana principal.
		@param ComandTrace Variable de tipo tCommand donde se guarda la configuración.
		@param GraficSampletime Apuntador al tiempo de muestreo configurado.
	*/
	void GetCBoxTRACE(HWND hDlg,tCommand * ComandTrace,double * GraficSampletime);
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Utilizando la información de configuración almacenada en ComandTrace, devuelve la cantidad de variables 
		configuradas en el PMD1 y en el PMD2.
		@param hDlg  Manejador de la ventana principal.
		@param ComandTrace Variable tipo tCommand que contiene la información de configuración.
		@param PMD1NVariables Apuntador  con el fin de actualizar con el numero de variables para PMD1
		@param PMD2NVariables Apuntador con el fin de actualizar con el numero de variables para PMD2
	*/
	int GetNvariables(HWND hDlg,tCommand ComandTrace,int * PMD1NVariables,int* PMD2NVariables);
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Habilita o deshabilita los campos: eje a monitorear y tipo de variable a trazar de la 
	    respectiva casilla de verificación identificada por op.
		@param hDlg  Manejador de la ventana principal.
		@param op ID de la Variable a Trazar.   
		@param habilitar 1 para Habilitar y 0 para deshabilitar.
	*/
	void Habilitar_camposSetTRACE(HWND hDlg,int op,int habilitar);
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Detecta si se ha seleccionado o no la casilla de verificación identificada por op de una  variable 
		de trazado,para habilitar o deshabilitar  sus respectivos campos de configuración. Para esto se apoya en la 
		función Habilitar_camposSetTRACE.
		@param hDlg  Manejador de la ventana principal.
		@param op Identificador de la casilla de verificación.	
	*/
	void manejador_camposSetTRACE(HWND hDlg,int op);
	
	/** \ingroup Cuadro_Dialogo_Trace
		\brief Reorganiza el vector  Tracecomand[8] de la estructura tCommand con el fin de no dejar campos vacíos 
		consecutivos y hacer más fácil el proceso de configuración del controlador Robótico.
		@param ComandTrace Variable tCommand Con la información de configuración del trazado.
	*/
	void FixVector(tCommand * ComandTrace);


#if defined(__cplusplus)
}
#endif

#endif

