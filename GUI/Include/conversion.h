#ifndef __CONVERSION_H__
#define __CONVERSION_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      conversion.h
//  Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\conversion.c
//
//  Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
//  Propósito General:	
//  Conjunto de funciones que facilitan las conversiones de unidades, necesarias para una 
//	correcta comunicación entre el controlador Robótico y el PC
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
#include "mem.h"
#include "robotData.h"

#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Conversiones CONVERSIONES
	\ingroup Cuadro_Dialogo_Tech_Pendant
	\brief Conjunto de funciones que facilitan las conversiones de unidades, necesarias para una 
	correcta comunicación entre el controlador Robótico y el PC 
	\code #include <conversion.h> \endcode 

*/
	/** \ingroup Conversiones
		\brief 	Tiempo de muestreo de los PMD, para la conversión desde segundos a ciclos. 
	*/
	#define SAMPLETIME (4.0*150e-6)    
	
	
	/** \ingroup Conversiones
		\brief Función que permite convertir un número real que representa los micropasos a un número en la notación 
		punto fijo en el formato 32.0 (para mayor información consultar el Navigator Motion Processor User´s Guide); 
		esto quiere decir 32 bits se usan para la parte entera y 0 bits se usan para la parte decimal, 
		lo que representa un número entero. 
		@param f Número a convertir.
	*/
	int32 positionPCtoUC(double f);

	
	/** \ingroup Conversiones
		\brief Función que convierte un número real  que representa la velocidad deseada a un número en la notación 
		punto fijo en el formato 16.16 (para mayor información consultar el Navigator Motion Processor User´s Guide);
		esto quiere decir 16 bits se usan  para la parte entera y 16 bits para la parte decimal.
		@param f Número a convertir.
	*/
	int32 velocityPCtoUC(double f);


	/** \ingroup Conversiones
		\brief Función que convierte un número real  que representa la aceleración deseada a un número en la notación 
		punto fijo en el formato 16.16 (para mayor información consultar el Navigator Motion Processor User´s Guide);
		esto quiere decir 16 bits se usan  para la parte entera y 16 bits para la parte decimal.
		@param f Numero a convertir.

	*/
	int32 accelerationPCtoUC(double f); 

	
	/** \ingroup Conversiones
		\brief Función que utilizando los parámetros mecánicos del motor que están en paxisSettings convierte 
		un ángulo en radianes a micropasos.
		@param paxisSettings Estructura que contiene los parámetros del motor.
		@param rad Número a convertir.
	*/
	double rad2usteps(tAxisSettings *paxisSettings, double rad);
	
	
	/** \ingroup Conversiones
		\brief Función que utilizando los parámetros del motor que estan en paxisSettings convierte  los micropasos a un 
		ángulo en radianes 		
		@param paxisSettings Estructura que contiene los parámetros del motor.
		@param usteps Número a convertir.
	*/
	double ustep2rad(tAxisSettings *paxisSettings, double usteps);
	
	
	/** \ingroup Conversiones
		\brief Función que convierte un ángulo representado en grados a radianes.
		@param grad Ángulo a convertir.
	*/
	double grados2rad(double grad);
	
	
	/** \ingroup Conversiones
		\brief Función que convierte un ángulo representado en radianes a grados.
		@param rad  Ángulo  a convertir.
	*/
	double rad2grados(double rad);
	
	
	/** \ingroup Conversiones
		\brief Función que convierte un número representado en la notación BCD a una representación decimal.
		@param bcd_format Número en notación BCD a convertir.
	*/
	uint8 bcd2int(uint8 bcd_format);
	
	
	/** \ingroup Conversiones
		\brief Función que convierte un número en representación decimal en la notación BCD.
		@param integer Número decimal a convertir.
	*/
	uint8 int2bcd(uint8 integer);
	
	/** \ingroup Conversiones
		\brief Devuelve la cantidad de uP que representa un número deterniando de grados en el motor.
		@param grados Número de grados a convertir.
	*/
	int32 gradosMotortouPasos(double grados); 
	
	
	double uPasosTOgradros(tAxisSettings *paxisSettings,int32 Valor,int tipo);
	int32 gradrosTOuPasos(tAxisSettings *paxisSettings,double angulo,int tipo);
	#ifdef _WIN32
		/** \ ingroup Conversiones
		\brief Función que convierte de:
		- micropasos a grados.
		- micropasos/SAMPLE_TIME  a grados/seg.
		- microspaos/SAMPLE_TIME^2 a grados/seg^2.
		
		@param paxisSettings Estructura que contiene los parámetros del motor.
		@param BufferData  Dato muestreado.
		@param TraceVar Tipo de variable (posición, velocidad, aceleración, registros de PMD, entre otra más).
	*/
	double uPasosTOgradroGraficos(tAxisSettings *paxisSettings,int32 BufferData,int TraceVar);
	#endif
#if defined(__cplusplus)
}
#endif

#endif