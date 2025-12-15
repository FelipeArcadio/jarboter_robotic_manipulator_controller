
#ifndef PMD_Parallel__
#define	PMD_Parallel__

#include "basicIO.h"
#include "PMDtrans.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDpar.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDpar.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Funciones que permiten la comunicación paralela con los controladores de ejes (PMDs).
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


	/** \defgroup  PMDpar INTERFACE  DE COMUNICACIÓN PARALELA 
	\ingroup Interface_uC_PMD
	\brief Funciones que permiten la comunicación paralela con los controladores de ejes (PMDs).
	\code #include <PMDpar.h> \endcode */
	
	/* @{ */

	/** \ingroup PMDpar
	\brief Esta función realiza el mapeo de los ejes con el fin de encontrar a que chipset (Chipset 1 ó Chipset 2) 
	pertenecen.

	@param Axis	Corresponde al eje (es un elemento de la estructura tAxis). 
	@see tAxis estructura.
	@return Retorna el PMDChipSet1, PMDChipSet2 ó PMDChipSet3 según sea el caso. */
	extern int16 Axis2PMDChipset(tAxis Axis);

	/** \ingroup PMDpar
	\brief Esta función realiza el mapeo de los ejes de la estructura tAxis con el fin de hallar el número de eje al
	que pertenece dentro del chipset.
	
	@param Axis	Corresponde al eje (es un elemento de la estructura tAxis).
	@see tAxis estructura.
	@return Retorna PMDAxis1, PMDAxis2, PMDAxis3 ó PMDAxis4 según sea el caso. */
	extern int16 Axis2PMDAxis(tAxis Axis);

	/** \ingroup PMDpar
	\brief Esta función configura los valores y máscaras de las banderas dependiendo del tipo de interface
	utilizada.
	@param transport_data Estructura que contiene la configuración del puerto de comunicación.
	@param IOMode Tipo de interface paralela utiliza (8/16).*/						  
	extern void PMDParallel_InitData(PMDParallelIOTransportData* transport_data, int16 IOMode);

	/**	\ingroup PMDpar
	\brief Función que asigna apuntadores a funciones.
	@param transport Inicializa los campos de la estructura a los apuntadores de las funciones que utiliza la interface.
	*/
	extern void PMDParallel_Init(PMDIOTransport* transport);

	/**	\ingroup PMDpar
	\brief Se llama a esta función una vez para cada chipset y eje, para configuración de punteros y máscaras.
	@param axis_handle Manejador de ejes.
	@param axis_number Eje a asignar. 
	@param PMDChipSet Chipset donde se le asigna el eje.
	*/
	extern PMDuint16 PMDSetupAxisInterface_Parallel(PMDAxisHandle* axis_handle, 
												 PMDAxis axis_number, PMDuint16 PMDChipSet);	
											 
	/* @} */
#endif

