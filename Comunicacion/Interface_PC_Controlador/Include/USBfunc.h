#ifndef __USBFUNC_H__
#define __USBFUNC_H__

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      USBfunc.h
//  Ubicación fuente: C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Contorlador\C\USBfunc.c
//
//  Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
//  Propósito General:	
//	Estas funciones permiten cofigurar e iniciar trasferencias de datos	sobre el bus USB, utilizando 
//	la API de WINDOWS, con el fin de gestionar la comunicacion con el driver del dispositivo, ezusbsys.h, 
//	mediante peticiones que utilizan códigos IOCTL. Las funciones utilizadas de la API fueron CreateFile
//	y DeviceIoControl.
//	para las transferencias se utilizó el esquema de  thread (Hilos) de WINDOWS, es	decir que al iniciar 
//	una transferencia se crea un subproceso que encarga de ésta.
//	
//	Mediante estas funciones se puede:
//	- Abrir el dispositivo.
//	- Leer y escribir en los endpoint de datos, EP2 (OUT) EP8 (IN).
//	- Leer el endpoint 0 (Setup).
//	- Enviar peticiones tipo vendor.
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
#include "types.h"
#include "C:\Jarboter_V_2_1\Comunicacion\Driver_USB\ezusbdrv\ezusbsys.h"  //el driver como tal			


#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Funciones_USB FUNCIONES USB
	\ingroup Sistema_de_Comunicación
	\brief Estas funciones permiten cofigurar e iniciar trasferencias de datos	sobre el bus USB, utilizando 
	la API de WINDOWS, con el fin de gestionar la comunicacion con el driver del dispositivo, ezusbsys.h, 
	mediante peticiones que utilizan códigos IOCTL. Las funciones utilizadas de la API fueron CreateFile
	y DeviceIoControl.
	para las transferencias se utilizó el esquema de  thread (Hilos) de WINDOWS, es	decir que al iniciar 
	una transferencia se crea un subproceso que encarga de ésta.
	
	Mediante estas funciones se puede:
	- Abrir el dispositivo.
	- Leer y escribir en los endpoint de datos, EP2 (OUT) EP8 (IN).
	- Leer el endpoint 0 (Setup).
	- Enviar peticiones tipo vendor.

	\code #include <USBfunc.h> \endcode 

*/
	/** \ingroup Funciones_USB
		\brief Función que crea un camino de entrada y salida  entre el sistema operativo y el hardware para iniciar 
		cualquier tipo de comunicación.		
		@param hDlg  Manejador de la ventana principal.
		@param phDeviceHandle Apuntador hacia el dispositivo USB, será actualizado si se puede crear el camino de 
				comunicación.
		@param devname Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	BOOLEAN bOpenDriver(HWND hDlg,HANDLE * phDeviceHandle, PCHAR devname);
	
	
	/** \ingroup Funciones_USB
		\brief Función que configura los campos de la estructura ThreadControl. Esta estructura contiene los parámetros
		para poder ejecutar una transferencia de escritura o lectura en un endponit de datos.
		
		@param BulkControl Apuntador a la estructura BULK_TRANSFER_CONTROL. En esta estructura sólo se modifica el 
				número del endpoint.
		@param ThreadControl Apuntador a la estructura THREAD_CONTROL (Esta estructura maneja los hilos). 
		@param PipeNum Número que representa el endponint asociado del dispositivo USB. EP2=>0 y EP8=>1.
		@param hDevice Manejador del dispositivo USB.
		@param inBuffer Apuntador a los datos de la transferencia. 
		@param TransferSize Cantidad de datos en la transferencia
		@param CompleteEvent Manejador del evento, Ocurre un evento cuando el Hilo termina.   
		@param tipo Bandera que indica el tipo de transferencia a ejecutar, 1 para escribir y 0 para leer.

	*/
	void Ini_estruc_USB(BULK_TRANSFER_CONTROL* BulkControl,THREAD_CONTROL* ThreadControl, 
					ULONG PipeNum, HANDLE hDevice,PVOID inBuffer, ULONG TransferSize, 
					HANDLE CompleteEvent,int tipo );
	
	
	/** \ingroup Funciones_USB
		\brief Función que permite comunicarse directamente con el driver para iniciar una transferencia de datos,
				utilizando los parámetros de la estructura threadControl y la función de WINDOWS DeviceIoControl.
		@param threadControl Parámetros para la comunicación con el driver.
	*/
	void TransferThread(PTHREAD_CONTROL threadControl);
	
	
	/** \ingroup Funciones_USB
		Función que inicia un hilo (thread) para la transferencia de datos desde el PC  hacia el controlador robótico
        y espera que ocurra su finalización. Para Configurar el hilo se utilizan las funciones Ini_estruc_USB 
		y TransferThread.
		@param hDlg  Manejador de la ventana principal.
		@param pipeNun Identificador del pipe de salida, en este caso siempre será 0 (referenciando al Endpoint 2).
		@param datos Apuntador a los datos a enviar.
		@param NUndata Número de datos a enviar.
		@param DeviceName Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	int Mandar_datos_Por_USB(HWND hDlg,ULONG pipeNun,PVOID datos,int NUndata,char* DeviceName);
	
	
	/** \ingroup Funciones_USB	
		Función que inicia  un hilo (thread) para la transferencia de datos desde controlador robótico hacia el PC y 
		espera que ocurra su finalización. Para Configurar el hilo se utilizan las funciones Ini_estruc_USB y 
		TransferThread.
		@param hDlg  Manejador de la ventana principal.
		@param pipeNun Identificador del pipe de entrada, en este caso siempre será 1 (referenciando al Endpoint 8).
		@param datos Apuntador a los datos a recibir.
		@param NUndata Número de datos a recibir.
		@param DeviceName Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	int Leer_datos_Por_USB(HWND hDlg,ULONG pipeNun,PVOID datos,int NUndata,char* DeviceName);

	
	/** \ingroup Funciones_USB
		Función que envía una petición (request) tipo vendor, definida por UCHAR request. Estas peticiones 
		fueron desarrolladas en el presente proyecto.
		@param hDlg  Manejador de la ventana principal.
		@param request Petición tipo vendor a enviar.
		@param Buffer Datos recibos en respuesta a la petición enviada.
		@param DeviceName Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	int Mandar_verdorR(HWND hDlg, UCHAR request,PCHAR Buffer,char * DeviceName);

	
	/** \ingroup Funciones_USB
		Función que cancela un transferencia en progreso, en caso de algún error en ésta.
		@param hDlg  Manejador de la ventana principal.
		@param pipeNun Identificador del pipe donde se inició la transferencia.
		@param DeviceName Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	int Abortar(HWND hDlg,ULONG pipeNun,char* DeviceName );
	
	
	/** \ingroup Funciones_USB
		Función que informa si el periférico USB está ocupado en alguna transferencia desde o hacia el controlador.
		@param hDlg  Manejador de la ventana principal.
		@param DeviceName Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	int UsbGpifIDEL(HWND hDlg,char* DeviceName);
	
	
	/** \ingroup Funciones_USB
		Función que informa si el dispositivo USB está conectado al computador.
		@param hDlg  Manejador de la ventana principal.
		@param DeviceName Nombre del dispositivo especificado en el archivo ezusbw2k.INF.
	*/
	BOOLEAN Esta_Conectado_USB(HWND hDlg,char* DeviceName);





#if defined(__cplusplus)
}
#endif


#endif