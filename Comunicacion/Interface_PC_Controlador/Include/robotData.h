#ifndef __ROBOTDATA_H__
#define __ROBOTDATA_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      robotData.h
// Ubicación fuente: C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Contorlador\C\robotData.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Funciones que permiten inicializar con los parámetros por defecto la estructura tAxisSettings, 
// la cual tiene la configuración de cada eje del controlador robótico.
//
// Compilado en:
//    avr-gcc 4.3.3 
//
// Author: Felipe Hurtado
//         David Osorio
//
// Version: 1.0
//  
// 
// Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** \defgroup RobotData		CONFIGURACIÓN DEL ROBOT
    \ingroup Sistema_de_Comunicación
	
	\brief Funciones que permiten inicializar con los parámetros por defecto la estructura tAxisSettings, 
	la cual tiene la configuración de cada eje del controlador robótico.
	
	\code #include <robotData.h> \endcode */
	
	

#include "types.h"

#if defined(__cplusplus)
extern "C" {
#endif

	
/** \ingroup RobotData
	\brief Esta función configura  un eje a sus valores por defecto.
	@param axis Es el eje que se configura.
	@param *paxisSettings Apuntador a una estructura de configuración de ejes.
*/
	void defaultAxisSettings(int16 axis, tAxisSettings *paxisSettings);

/** \ingroup RobotData
	\brief Esta función configura todos los ejes del controlador robótico a sus valores por defecto.
	@param *axisSettings Apuntador a la estructura a configurar.
*/
	void setupAxisSettings(tAxisSettings *axisSettings);
	
	
#if defined(__cplusplus)
}
#endif

#endif
