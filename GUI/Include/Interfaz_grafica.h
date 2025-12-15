#ifndef __INTERFAZGRAFICA_H__
#define __INTERFAZGRAFICA_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      Interfaz_grafica.h
// Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\Interfaz_grafica.cpp
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// 	Declara e inicializa la clase ventana, la cuál permite crear el menú principal, 
//  para que el usuario pueda abrir los diferentes cuadros de diálogo del programa, 
//  evualuando los eventos del ratón mediante la función WindowProcedure. 
//  
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





#include "COMMONtypes.h"


/*! \mainpage Documentación del software de la tesis de grado JARBOTER V2.1
 
   \section intro_sec INTRODUCCIÓN

	En las siguientes páginas se encuentra la documentación del código fuente que se utilizó en la 
	implementación de la tesis de grado JARBOTER V2.1. Controlador para un robot de 5 grados de libertad con
	su respectivo efector final.
	
	La documentación del código fuente se dividió en tres módulos principales:
	- Interfaz gráfica de usuario.
	- Sistema de comunicación entre el PC y el controlador robótico.
	- Firmware del controlador robótico.
	
	\subsection interfaz_grafica INTERFAZ GRÁFICA DE USUARIO
	La interfaz gráfica permite al usuario un total dominio sobre el controlador robótico. Esta Interfaz fué 
	desarrollada utilizando las librerías de la API de WINDOWS y el compilador Visual C++ 6.0.  Se puede 
	acceder al proyecto a través de la siguiente ruta: 
	C:\Jarboter_V_2_1\Proyecto\VisualC++\RoboticController\RoboticController.dsw.
	
	Al compilar produce el archivo ejecutable PMDControler.exe. Este archivo se accede a través del menú Windows opción
	USB Controler del programa de simulación ROBOMOSP.

	
	La interfaz gráfica del controlador robótico se divió en los siguientes módulos:
	
	- \ref tipo_datos_pc
	- \ref Funciones_de_menu
	- \ref Cuadro_Dialogo_Enviar_Archivo
		- \ref Procesamiento_Archivos
	- \ref Cuadro_Dialogo_Tech_Pendant
		- \ref Conversiones
	- \ref Cuadro_Dialogo_Trace
	- \ref Cuadro_Dialogo_RTC
	- \ref Cuadro_Dialogo_Configuración
	- \ref Interface_uC_USB_CY68013
	
	
	\subsection comunicacion SISTEMA DE COMUNCIACIÓN ENTRE EL PC Y EL CONTROLADOR ROBÓTICO
	
	El sistema de comunicación de datos tiene dos funciones:
	1.) Mantener un flujo de comunicación bidireccional constante entre el PC y el microcontrolador ATMega 1280. 
	Este se realiza	a través del controlador de periféricos USB CY-68013. En este controlador de periféricos se encuentran
	los archivos que realizan ésta tarea estos son (firmware del controlador de periféricos USB):
		- USBTrans.c
		- GPIF.c
		- FW.c
		- dscr.a51
	
	Estos archivos fueron compilados en el programa uVision2  V2.10 utlilizando el compilador C51 V6.10.
	Al compilar estos archivos se produce el archivo USBTrans.iic que es cargado en la memoria EEPROM utilizando
	el programo EZ-USB Control Panel. 
	
	
	2.) La segunda funcionalidad es la de establecer un protocolo de comunicación de paquetes de datos tanto en el PC
	como en el microcontrolador.
	Los módulos que realizan ésta tarea son:
	- \ref Protocolo_comunicacion_PC
		- \ref com_types COMMONtypes.h
		- \ref Transmision
		- \ref Recepcion
		- \ref Checksum
		- \ref Tools
		- \ref Manejador_RAM

		
	Estos módulos son compilados en ambos ambientes (PC, Microcontrolador). Para ello se declararon las definiciones
	_#ifdef _WIN32  e #ifdef _AVR_IO_H  para seleccionar las funciones específicas  para cada	ambiente. 
	
	Para el envío y recepción de datos desde el PC se utliza el siguiente módulo (el cual es compilado sólo en el PC):
	- \ref Funciones_USB
	
	El archivo USBfunc.h utiliza la API de WINDOWS que a su vez se comunica con el driver del dispositivo, el cual
	fué suministrado por la empresa CYPRESS.	
	- \ref driver_usb
	
	Por último para el envío y recepción de datos desde el microcontrolador se utilizó en siguiente módulo (el cual es
	compilador sólo en el microcontroador):	
	- \ref Interface_uC_USB_CY68013
	
	
	\subsection controlador_robotico FIRMWARE DEL CONTROLADOR ROBÓTICO
	
	El firmware del controlador robótico fué  desarrolloado en el lenguaje ansi C 
	y compilado utilizando avr-gcc  4.3.3. Al compilar el archivo principal <contralador_Jarboter.c>
	se generan dos archivos:
	
	- <contralador_Jarboter.hex>.
	- <contralador_Jarboter.eep>. 
	
	Estos archivos se cargan en la memoria flash y en la memoria EEPROM del microcontrolador ATMEGA1280 respectivamente. 
	Para realizar esta carga se utilizó el programa AVR Studio 4.0 que se puede descargar de la página de ATMEL 
	<A HREF="http://www.atmel.com/dyn/products/tools_card.asp?tool_id=2725&category_id=163&family_id=607&subfamily_id=760"> http://www.atmel.com</A>
	
	El firmware del controlador robótico se divió en los siguientes módulos:
	- \ref uC_Setup
	- \ref Protocolo_I2C
	- \ref debug
		- \ref buzzer
		- \ref led
	- \ref Interface_uC_Peripherical
		- \ref lcd
			\ref LCD_Driver
			\ref funciones_lcd
		- \ref EEPROM
		- \ref RTC_ISL1207
		- \ref Uart
	- \ref control_mov
		- \ref Interface_uC_PMD
			- \ref UCtypes
			- \ref basicIO
			- \ref PMDpar
			- \ref PMDtrans
			- \ref CMotion			
		- \ref PMDsetup
		- \ref PMDinfo
		- \ref PMDinterrupt
		- \ref PMDTrace
		- \ref Rutinas_de_robotica
			- \ref PMDrobotics
			- \ref Efector_final
	- \ref Teach_Pendant	
	
	

*/


/** \defgroup GUI  INTERFAZ GRÁFICA DE USUARIO
	\brief Funciones que se ejecutan en el PC para la implementación de la interfaz gráfica.

*/
	/** \ingroup GUI
	\brief Función Principal del programa,  ésta es una función propia de WINDOWS que 
	se  encarga de configurar la ventana principal, atender  los mensajes de 
	consola y cargar el Menú de la aplicación.
	*/ 
	int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpszArgument,
                    int nFunsterStil);
					
	/** \ingroup GUI
	\brief Función invocada por DispatchMessage(&mensaje) (función de la API WINDOWS) 
	para el procesamiento de los mensajes.
	En WINDOWS un mensaje puede ser un click del mouse, una entrada por teclado,
	una selección del menú, entre otrás más. La función  WindowProcedure llama a otras funciones
	de diálogo dependiendo del mensaje que se recibe.
	*/ 
	LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

	/** \ingroup GUI
	\brief Cuadro de diálogo encargado de enviar el archivo *.pot de la trayectoria al controlador 
    robótico  utilizando la interface USB. Si la trayectoria  es enviada con éxito se imprime la
	información del archivo en el cuadro de diálogo, en caso de algún error aborta la transferencia. 	
	*/ 
	BOOL CALLBACK Envia(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	/** \ingroup GUI
	\brief Función del cuadro de diálogo del teach pendant.
	*/ 	
	BOOL CALLBACK TechBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	
	
	/** \ingroup GUI
	\brief Función de cuadro de diálogo que aparece cuando el usuario desea sobreescribir una pose.
	*/ 	
	BOOL CALLBACK RSETPOSE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	
	/** \ingroup GUI
	\brief Función de cuadro de diálogo encargada de extraer las poses almacenadas en la memoria EEROM 
	del controlador con el fin de construir una trayectoria.
	*/ 	
	BOOL CALLBACK TechTracj(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	/** \ingroup GUI
	\brief Cuadro de diálogo encargado de reconfigurar parámetros del robot. Ej: Potencia de motor, 
	número de micropasos por paso, activación de encodificadores entre otros más.
	*/ 		
	BOOL CALLBACK SETRobot(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	/** \ingroup GUI
	\brief cuadro de diálogo encargado de leer y reconfigurar la hora y tares preprogramadas 
	en el controlador.
	*/ 	
	BOOL CALLBACK SETRTC(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


	/** \ingroup GUI
	\brief Cuadro de diálogo encargado configurar el trazado de parámetros del robot.
	*/ 	
	BOOL CALLBACK SETTRACE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	/** \ingroup GUI
	\brief Cuadro de diálogo que imprime la matriz homogénea de la posición y orientación del robot.
	*/ 	
	BOOL CALLBACK DISPLAYMATIZ(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

	/** \ingroup GUI
	\brief Función que se encarga en mostrar en pantalla las gráficas del trazado del controlador
	robótico.
	*/ 	
	void GraficTraceData(int32 log,int NVariablesGraficar,int * TraceINDEX,int PMDId);


#endif