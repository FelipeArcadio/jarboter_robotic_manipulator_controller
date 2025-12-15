#ifndef __DIALOGOTRAJECENVIA_H__
#define __DIALOGOTRAJECENVIA_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      DialogoTrajecEnvia.h
//  Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\DialogoTrajecEnvia.c
//
//  Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
//  Propósito General:	
// 	Conjunto de funciones que define el comportamiento del cuadro de dialogo "TRAYECTORIA", el cual se
//  encarga de abrir un  archivo de extensión .PlOT generado por el programa ROBOMOSP.
//	Este archivo  contiene la información de posición, velocidad y aceleración para cada eje del robot en cada 
//	punto de la trayectoria diseñada por el usuario. 
//	Luego envía ésta información hacia el controlador Robótico con el fin de ejecutar la trayectoria. 
//	Si la trayectoria es enviada con éxito el cuadro de diálogo despliega la información enviada.
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


/** \defgroup Cuadro_Dialogo_Enviar_Archivo CUADRO DE DIÁLOGO DE ENVÍO DE TRAYECTORIAS
	\ingroup GUI
	\brief Conjunto de funciones que define el comportamiento del cuadro de dialogo "TRAYECTORIA", el cual se
	encarga de abrir un  archivo de extensión .PlOT generado por el programa ROBOMOSP.
	Este archivo  contiene la información de posición, velocidad y aceleración para cada eje del robot en cada 
	punto de la trayectoria diseñada por el usuario. 
	Luego envía ésta información hacia el controlador Robótico con el fin de ejecutar la trayectoria. 
	Si la trayectoria es enviada con éxito el cuadro de diálogo despliega la información enviada.
	
	\code #include <DialogoTrajecEnvia.h> \endcode 

*/

	/** \ingroup Cuadro_Dialogo_Enviar_Archivo
		\brief Imprime la trayectoria en el cuadro de diálogo "TRAYECTORIA".
		@param hDlg  Manejador de la ventana principal.
		@param Pptrajectory Apuntador a la trayectoria que se desea imprimir.
		@param nIDDlgItem ID de la ventana en donde se imprime la trayectoria.
	*/
	void DisplayTrayectInfo(HWND hDlg,tTrajectory * Pptrajectory,int nIDDlgItem);
	
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendal
		\brief Llena la estructura tTrajectory con el flujo de datos provenientes del aarchivo de extensión .PLOT
		usando la información de configuración mecánica especificada por la variable axisSettings.
		@param ptrajectory Apuntador a la estructura trayectoria que será actualizada.
		@param axisSettings Apuntador a la estructura que contiene la configuración del robot.
		@param dimem Estructura que contiene información relaciona con el archivo, como son: Número  de puntos, datos, 
		ejes del archivo leído.
		@param datos Apuntador a una cadena que contiene los datos del archivo.
		@param SET_ROBOT Una Bandera que indica si se ha reconfigurado el robot.
		@return rcFileOpenError, Si archivo contiene más grados de libertar que los del Jarboter no existe.
		@return rcOK, Si Trayectoria es llenada con éxito.
	*/
	int Llenar_trayectoria(tTrajectory *ptrajectory,tAxisSettings *axisSettings,pDIMEN dimem,double* datos,
							int SET_ROBOT);

#if defined(__cplusplus)
}
#endif

#endif
