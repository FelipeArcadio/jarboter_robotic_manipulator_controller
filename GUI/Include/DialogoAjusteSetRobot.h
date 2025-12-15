
#ifndef __DIALOGOAJUSTESETROBOT_H__
#define __DIALOGOAJUSTESETROBOT_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      DialogoAjusteSetRobot.h
// Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\DialogoAjusteSetRobot.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// 	Conjunto de funciones que define el comportamiento del cuadro de diálogo "CONFIGURACION", que 
//	es accedido desde el menú principal, este cuadro de diálogo permite configurar los parámetros
//	del robot mediante la actualización de la variable global tAxisetting que al ser enviada a 
//	través del USB configura los PMDs (Tarjetas Controladoras de Motores) a través de Comandos 
//	preestablecidos para tal fin . De igual forma permite leer la configuración actual del controlador 
//  robótico.
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



/** \defgroup Cuadro_Dialogo_Configuración CUADRO DE DIÁLOGO DE CONFIGURACIÓN
	\ingroup GUI
	\brief Conjunto de funciones que define el comportamiento del cuadro de dialogo "CONFIGURACION", el cual 
	es accedido desde el menú principal. Este cuadro de dialogo permite configurar los parámetros
	del robot mediante la actualización de la variable global tAxisetting que al ser enviada a 
	través de la interfaz USB activa la configuración de los PMDs (Tarjetas Controladoras de Motores) por medio de Comandos 
	preestablecidos para tal fin. De igual forma, permite leer la configuración actual del controlador robótico.


	\code #include <DialogoAjusteSetRobot.h> \endcode 
*/
	/** \ingroup Cuadro_Dialogo_Configuración
		\brief Llena automáticamente los campos del cuadro de dialogo "CONFIGURACION"  con los parámetros por defecto 
		del controlador robótico.
		@param hDlg  Manejador de la ventana principal.
		@param uPasos  ID del Combo box uPasos. Este campo se llena con los posibles parámetros para configurar 
			   los uPasos del robot.
		@param MotorPower ID del Combo box Potencia. Este campo será llenado con el porcentaje de potencia entragado a 
				los moteres.
		@param Switch_Int ID del Check box Switch Int. Por defecto en este campo será colocado a verdadero, es decir
		que el robot toma por defecto los límites de carrera.
	*/
		void CBoxAJUSTE(HWND hDlg,int uPasos,int MotorPower,int Switch_Int);
	
	/** \ingroup Cuadro_Dialogo_Configuración
		\brief Usando la informacion de los campos del cuadro de dialogo "CONFIGURACION",  llena la estructura tAxisSettings.
		@param hDlg  Manejador de la ventana principal.
		@param tAxisSetting  Variable a llenar con los parámetros definidos por el Usuario en la inferface 
				"CONFIGURACION".
		@param uPasos ID del Combo box uPasos. Este Id sirve para acceder a la información de este campo para luego 
				copiarlo en el respectivo campo de la estructura tAxisSettings.
		@param MotorPower ID del Combo box Potencia. Se accede a la información para luego copiarla a la estructura 
				tAxisSettings.
		@param Switch_Int ID del Check box Switch Int. Sirve para determinar si está activado o no para luego copiarlo 
				a la estructura tAxisSettings.
		@param Encoder ID del Check box Encoder. Sirve para determinar su estado para luego copiarlo a la estructura
				tAxisSettings.
		@param ErrorLimit ID del Edit box ErrorLimit. Mediante el ID, se accede a la información para luego copiarlo 
			   a la estructura tAxisSettings.
		@param TrackWindow ID del Edit box TrackWindow. Mediante el ID, se accede a la información para luego copiarlo 
			   a la estructura tAxisSettings.
		@param SettleWindow ID del Edit box SettleWindow. Mediante el ID, se accede a la información para luego 
			   copiarlo a la estructura tAxisSettings.
		@param SettleTime ID del Edit box SettleTime. Mediante el ID, se accede a la información para luego copiarlo a 
			   la estructura tAxisSettings.
	*/
		void GetCBoxAJUSTE(HWND hDlg,tAxisSettings * tAxisSetting,int uPasos,int MotorPower,int Switch_Int,int Encoder,
		                   int ErrorLimit,int TrackWindow, int SettleWindow,int SettleTime);
	
	/** \ingroup Cuadro_Dialogo_Configuración
		\brief Usando la informacion de la estructura tAxisSettings tAxisSettingUSB llena el cuadro de dialogo  que 
				configura el robot, llamado  "CONFIGURACION".  La variable  tAxisSettingUSB contiene la configuración 
				actual del controlador  robótico.
		@param hDlg  Manejador de la ventana principal.
		@param tAxisSettingUSB  Variable que contiene los parámetros de configuración para actualizar el cuadro de diálogo.
		@param uPasos ID del Combo box uPasos. Este Id sirve actualizar su información con el valor del campo correspondiente  
		       en la estructura tAxisSettingUSB.
		@param MotorPower ID del Combo box Potencia. Este Id sirve para actualizar su información con el valor del campo 
			   correspondiente  en la estructura tAxisSettingUSB.
		@param Switch_Int ID del Check box Switch Int. Este Id sirve para actualizar su estado con el valor del campo 
		       correspondiente  en la estructura tAxisSettingUSB.
		@param Encoder ID del Check box Encoder. Este Id sirve para actualizar su estado con el valor del campo correspondiente  
		       en la estructura tAxisSettingUSB.
		@param ErrorLimit ID del Edit box ErrorLimit. Este Id sirve para actualizar su información con el valor del campo 
		       correspondiente  en la estructura tAxisSettingUSB.
		@param TrackWindow ID del Edit box TrackWindow. Este Id sirve para actualizar su información con el valor del campo 
			   correspondiente  en la estructura tAxisSettingUSB.
		@param SettleWindow ID del Edit box SettleWindow. Este Id sirve para actualizar su información con el valor del 
		       campo correspondiente  en la estructura tAxisSettingUSB.
		@param SettleTime ID del Edit box SettleTime. Este Id sirve para actualizar su información con el valor del campo 
		       correspondiente  en la estructura tAxisSettingUSB.
	*/
		void SetCBoxAJUSTE_USB(HWND hDlg,tAxisSettings * tAxisSettingUSB,int uPasos,int MotorPower,int Switch_Int,
		      int Encoder,int ErrorLimit,int TrackWindow, int SettleWindow,int SettleTime);
	
	/** \ingroup Cuadro_Dialogo_Configuración
		\brief Habilita o deshabilita los campos ErrorLimit, TrackWindow, SettleWindow,  SettleTime.
		@param hDlg  Manejador de la ventana principal.
		@param op ID del Check box Encoder.
		@param habilitar Esta variable cuando esta en 1, habilita los campos y con 0 los deshabilita.
		*/
		void Habilitar_camposSetRobot(HWND hDlg,int op,int habilitar);
	
	/** \ingroup Cuadro_Dialogo_Configuración
		\brief Comprueba si el  Check box Encoder esta activado, para luego Habilitar o deshabilitar sus respectivos 
		campos usando la función Habilitar_camposSetRobot.
		@param hDlg  Manejador de la ventana principal.
		@param op ID del Check box Encoder. Este ID sirve para detectar el estado del Check box. 
		*/
		void manejador_camposSetRobot(HWND hDlg,int op);



#if defined(__cplusplus)
}
#endif

#endif
