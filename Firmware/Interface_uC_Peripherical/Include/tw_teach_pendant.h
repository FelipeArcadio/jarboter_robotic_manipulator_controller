#ifndef __TW_TEACH_PENDANT_H__
#define __TW_TEACH_PENDANT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      tw_teach_pendant.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\tw_teach_pendant.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Estas funciones desarrollan operaciones de lectura y escritura sobre una Pose del Robot en Memoria EEPROM
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
	
#include "tw_eeprom.h"

/** \defgroup Teach_Pendant FUNCIONES PARA EL MANEJO DE POSES
	\ingroup Programa_firmware
	\brief Estas funciones desarrollan operaciones de lectura y escritura sobre una Pose del Robot 
	en Memoria EEPROM.
	
	\code #include <tw_teach_pendant.h> \endcode */

	/* @{ */
	
	/** \ingroup Teach_Pendant
	\brief Esta Función retorna el número total de Poses del Robot que se encuentran almacenadas en la memoria	EEPROM.	
	@return -1.Si Existe un error, de lo contrario un valor de tipo entero que contiene el número total de poses.
	*/
	int16 Get_Num_Poses(void);
	
	/** \ingroup Teach_Pendant
	\brief Esta Función retorna la dirección de memoria (EEPROM) en donde se encuentra ubicada la Pose 	cuya etiqueta 
	o identificación sea id.
	@param	id Etiqueta de la Pose que se quiere ubicar en memoria.
	@return	. -1 si hay un error de lo contrario retorna la dirección de memoria de la Pose.*/
	int32 Found_Id(uint16_t id);
	
	/** \ingroup Teach_Pendant
	\brief Esta función almacena una Pose del Robot en la memoria EEPROM.
	@param  Pose Estructura tipo Pose que se almacenará en la memoria EEPROM.
	@return rv Si hay un error en la transmisión rv=-1.*/
	int16 Adicionar_PoseTWI(PoseRobot Pose);
		
	/** \ingroup Teach_Pendant
	\brief Esta función Lee una Pose del Robot almacenada en EEPROM cuya etiqueta o nombre sea id.
	@param id Etiqueta de la Pose del Robot.
	@param Pose Estructura en donde se almacenarán los datos leídos desde la memoria EEPROM.
	@return rv Si hay un error en la transmisión o la pose no existe rv=-1 de lo contrario retorna la cantidad de
			datos leidos.	*/
	int16 Leer_PoseTWI(uint16_t id, PoseRobot *Pose);	

	/** \ingroup Teach_Pendant
	\brief Esta función sobreescribe una Pose que está almacenada en memoria EEPROM. Por Ej, Sobreescribir la Pose
	cuaya identificación sea 24.
	@param Estructura tipo Pose que se almacenará en memoria EEPROM.
	@return Si hay un error en la transmisión rv=-1.	*/	
	int16 SobreEscribir_Pose(PoseRobot Pose);	
	
	
	/** \ingroup Teach_Pendant
	\brief Esta función retorna todos los Id's (etiquetas) de las Poses que están almacenadas en la memoria EEPROM.
	
	@return	. retorna 1 cuando existen poses en la memoria EEPROM y no hay error.
					  0 Cuando no existen Poses en la memoria EEPROM.
					 -1 Cuando hay en error en la lectura.*/	
	int16 return_id_vector(void);
		
	/** \ingroup Teach_Pendant	
	\brief Esta función borra todos los datos almacenados en la memoria EEPROM. Para borrar la memoria EEPROM no es 
	necesario llenar toda la memoria con ceros, basta con poner a cero el número de Poses, así se permite la 
	sobreescritura de los datos como si la memoria no tuviera nada grabado.
	
	@return -1 Si hay un error en la transmisión sobre el bus I2C. De lo contrario retorna 2 que es 
				el número de bytes escritos que es la cantidad ocupada por la variable Num_pos.*/
	int16 BorrarEEPROM(void);	
	
	/** \ingroup Teach_Pendant	
	\brief Esta función construye una trayectoria a partir un vector de etiquetas (ID´s) de poses. Mediante este 
	vector la función busca en la memoria EEPROM la información de la pose para luego copiarla a la 
	trayectoria en cada punto.
	
	@return -1 si hay un error, vector de ID´s vacío .*/
	int16  Vector2Trajectory(uint16 *vector);
	
	/** \ingroup Teach_Pendant	
	\brief Esta función transfiere la información de una estructura tipo Pose a un estructura tipo Trayectoria.	*/
	void Pose2rxTrayectoria(PoseRobot* EEpromPose);
	
	/* @} */

	


#endif
