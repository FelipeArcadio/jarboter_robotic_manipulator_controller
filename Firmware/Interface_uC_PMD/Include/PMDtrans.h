#ifndef __PMDTRANS_H__
#define __PMDTRANS_H__

// .h part

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDtrans.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDtrans.c
//
// Proyecto: PMDtrans.h -- C-Motion API
//  
// Propósito General:
//  Definen el formato de los paquetes de datos para construir comandos y enviarlos hacia la tarjeta
//	controladora de motores (PMDs).  
//
//	Ver Navigator Motion Processor - ISA Developpers Kit Manual, pag 31.
//  ver Navigator Motion Processor - Programmes´s Reference.
//
//	
//		Todos los comandos con datos asociados tiene 1,2,3 palabras de datos, 
//		su formato es:
//			
//|-------------------------------------------|---------------|---------------|
//|               (Comando)                   | 1er dato(W/R) | 2do Dato(W/R) |        
//|-------------|-----------|-----------------|---------------|---------------|
//| 15 14 13 12 | 11 10 9 8 | 7 6 5 4 3 2 1 0 |31          16 | 15          0 |
//|-------------|-----------|-----------------|---------------|---------------|
//|       0     |   Axis    |    Código del   |               |               |
//|             |           |      Comando    |               |               |
//|-------------|-----------|-----------------|---------------|---------------|
//	
// Compilado en:
//    avr-gcc 4.3.3 
//
// Author: Performance Motion Devices, Inc.
//
// Version: 1.0
//  
// 
// Copyright (c) 2010
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "types.h"



/** \defgroup PMDtrans FUNCIONES DE TRANSPORTE
    \ingroup Interface_uC_PMD	
	\brief  Definen el formato de los paquetes de datos para construir comandos y enviarlos hacia la tarjeta
	controladora de motores (PMDs). 

	\see { Navigator Motion Processor - ISA Developpers Kit Manual, pag 31 }
	\see { Navigator Motion Processor - Programmes´s Reference }

	\verbatim
		Todos los comandos con datos asociados tiene 1,2,3 palabras de datos, 
		su formato es:
			
|-------------------------------------------|---------------|---------------|
|               (Comando)                   | 1er dato(W/R) | 2do Dato(W/R) |        
|-------------|-----------|-----------------|---------------|---------------|
| 15 14 13 12 | 11 10 9 8 | 7 6 5 4 3 2 1 0 |31          16 | 15          0 |
|-------------|-----------|-----------------|---------------|---------------|
|       0     |   Axis    |    Código del   |               |               |
|             |           |      Comando    |               |               |
|-------------|-----------|-----------------|---------------|---------------|
 
	\endverbatim		
	
	
	\code #include <c-motion.h> \endcode */
	
/* @{ */ 




extern PMDuint16 BuildCommand(PMDuint16 OPCode, PMDAxis axis);
extern PMDuint16 SendCommand(PMDAxisHandle* axis_handle, PMDuint16 OPCode);
extern PMDuint16 SendCommandWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1);
extern PMDuint16 SendCommandWordWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint16 data2);
extern PMDuint16 SendCommandLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint32 data1);
extern PMDuint16 SendCommandWordLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint32 data2);
extern PMDuint16 SendCommandGetWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 *data1);
extern PMDuint16 SendCommandGetWordWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 *data1, PMDuint16 *data2);
extern PMDuint16 SendCommandGetLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint32 *data1);
extern PMDuint16 SendCommandWordGetWord(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint16 *data2);
extern PMDuint16 SendCommandWordGetLong(PMDAxisHandle* axis_handle, PMDuint16 OPCode, PMDuint16 data1, PMDuint32 *data2);

/* @} */
    
#endif


