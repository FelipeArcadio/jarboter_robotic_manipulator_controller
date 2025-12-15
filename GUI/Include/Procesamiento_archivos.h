#ifndef __PROCESAMIENTO_ARCHIVOS_H__
#define __PROCESAMIENTO_ARCHIVOS_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      Procesamiento_archivos.h
// Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\Procesamiento_archivos.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// 	Conjunto de funciones que se encargan de abrir, leer y analizar los  archivos de extensión .PLOT.
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

#include "types.h"            //contiene varibles que solo le inportan a el PC
#include <stdio.h>				//Entrada y salida estandar
#include <stdlib.h>     
#include <string.h>				//manejo de caracteres


#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Procesamiento_Archivos  MÓDULO PARA EL MANEJO DE ARCHIVOS
	\ingroup Cuadro_Dialogo_Enviar_Archivo
	\brief Conjunto de funciones que se encargan de abrir, leer y analizar los  archivos de extensión .PLOT.
	\code #include <Procesamiento_archivos.h> \endcode 
*/
	/** \ingroup Procesamiento_Archivos
		\brief Función que obtiene el número de puntos, número de datos, número de columnas, número de grados de 
				libertad del archivo trayectoria.
		@param NArchivo [] Nombre del archivo a leer, con la ubicación del archivo incluído.
		@param tdimen Estructura donde se almacena las características del archivo. 
		
	*/
	void Get_dimension_trayec(char NArchivo [], pDIMEN tdimen);
	
	
	/** \ingroup Procesamiento_Archivos
		\brief Función que extrae los datos del archivos de extensión .PLOT y los almacena en el vector numeros[]. 
		@param hDlg Manejador de la ventana.
		@param NArchivo [] Nombre del archivo a leer, con la ubicación del archivo incluído.
		@param numeros Apuntador al vector en donde se almacenarán los datos.
		@param Dimen Estructura que contiene las características del archivo.
		@return rcFileReadError Cuando el archivo no se puede abrir o leer. 
		@return rcFileReadOK Cuando es leído con éxito 
	*/
	int Obtener_datosFILE(HWND hDlg,char NArchivo [],double* numeros,pDIMEN Dimen);
	
	
	/** \ingroup Procesamiento_Archivos
		\brief Función que obtiene la posición, velocidad y aceleración de un eje determinado para todos los puntos 
				de la trayectoria del archivo de extensión  .PLOT, almacenándolos en un vector de tipo pPROFILE 
				llamado  datosJoint[nundatos]. 
		@param datosJoint Apuntador a profile, vector donde se almacenarán los datos.
		@param Joint Nombre de la articulación de la cual se quiere extraer los datos de posición, velocidad y 
				aceleración.
		@param dimem Estructura que contiene las características del archivo leído.
		@param numeros Apuntador al vector que contiene los datos leídos del archivo de extensión .PLOT.
	*/
	void ObtenerProfile(pPROFILE datosJoint,int Joint,const pDIMEN dimem,double* numeros);
	
	
	/** \ingroup Procesamiento_Archivos
		\brief Función que comprueba si el archivo que se está tratando de abrir existe en disco.
		@param hDlg Manejador de la ventana.
		@param NArchivo [] Nombre del archivo a leer incluyendo la ubicación.
	*/
	int Existe_Arhivo(HWND hDlg,char NArchivo []);  


#if defined(__cplusplus)
}
#endif

#endif