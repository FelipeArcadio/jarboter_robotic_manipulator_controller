#ifndef __GRIPPER_H__
#define __GRIPPER_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      gripper.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Efector_Final\C\gripper.c
//
// Proyecto: JARBOTER V2.1 Controlador Robótico de 5 grados de libertad.
//  
// Propósito General:	
// Estas funciones desarrollan operaciones para la manipulacion del gripper.
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	
/** \defgroup Efector_final EFECTOR FINAL
	\ingroup Rutinas_de_robotica
	\brief Estas funciones desarrollan operaciones para la manipulacion del gripper.
	
	\code #include <gripper.h> \endcode	*/

	/* @{ */
	
	/** \ingroup EFECTOR_FINAL
	\brief Esta función abre el gripper.*/
	void openGripper(void); 
	
	/** \ingroup EFECTOR_FINAL
	\brief Esta función abre el Gripper por un tiempo especificado.
	@param tiempo Tiempo en el cual el motor del gripper está en funcionamiento.*/
	void openGripper_time(int tiempo); 
	
	/** \ingroup EFECTOR_FINAL
	\brief Esta función cierra el gripper.*/
	void closeGripper(void);
	
	/** \ingroup EFECTOR_FINAL
	\brief  Esta función cierra el gripper por un tiempo especificado. 
	@param tiempo Tiempo en el cual el motor del gripper está en funcionamiento.*/
	void closeGripper_time(int tiempo);
	
	/** \ingroup EFECTOR_FINAL
	\brief  Determina el estado actual del gripper abierto / cerrado.
	@return Retorna 0 para abierto, 1 cerrado y -1 indeterminado.*/
	int8 Estado_gripper(void);
	
	/* @} */


#endif



