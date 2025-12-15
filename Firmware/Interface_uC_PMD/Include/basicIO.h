#ifndef __BASICIO_H__
#define __BASICIO_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      basicIO.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\basicIO.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Este módulo contiene las funciones de entrada y salida  de bajo nivel de la 	comunicación con 
//	los Controladores de motores (PMD´s) y el microcontrolador en Modo 8/16 tal y como se describe 
//	en el manual de especificaciones técnicas de los PMD´s.  	
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

#include "uC_setup.h"
#include "types.h"




/** \defgroup control_mov	SISTEMA DE CONTROL DE MOVIMIENTO
	\ingroup Programa_firmware
	\brief Este grupo contiene los módulos con los cuales se efectúa movimiento en el manipulador
	robótico utilizando para ello las tarjetas controladoras de motores (PMDs).
*/

/** \defgroup Interface_uC_PMD	INTERFACE MICROCONTROLADOR-PMD
	\ingroup control_mov
	\brief Este módulo contiene las funciones con las cuáles se puede efectuar una comunicación 
	bidireccional con las tarjetas controladoras de motores (PMDs) con el fin de poder hacer uso 
	de la librería C-motion.
*/

	/** \defgroup basicIO DRIVER PMDs
	\ingroup Interface_uC_PMD
	\brief Este módulo contiene las funciones de entrada y salida  de bajo nivel de la 	comunicación con 
	los Controladores de motores (PMD´s) y el microcontrolador en Modo 8/16.
	\see {Navigator Motion Processor - MC2400 Series Technical Specifications for Microstepping Motion Control, pag 22}
  	
	\code #include <basicIO.h> \endcode */   

	/* @{ */
 
	/** \ingroup basicIO
	\brief Escribe un comando de 16 bits al chip PMD.
	 @param PMDChipSet Corresponde al chip PMD con el cual nos comunicaremos:
			PMDChipset1, PMDChipset2 Y PMDChipset3.
	 @param dataword- Corresponde al comando que se va a enviar al chip PMD.*/
	void writePMDcommand(PMDuint16 PMDChipSet, PMDuint16 dataword);


	/** \ingroup basicIO
    \brief Escribe un dato de 16 bits al chip PMD.
	@param PMDChipSet Corresponde al chip PMD con el cual nos comunicaremos
		   PMDChipset1, PMDChipset2 Y PMDChipset3.
	@param dataword- Corresponde al dato que se va a enviar al chip PMD. */
	void writePMDdata(PMDuint16 PMDChipSet, PMDuint16 dataword);

	/** \ingroup basicIO
	\brief Lectura de un dato de 16 bits provenientes del Chip PMD.
	@param PMDChipSet Corresponde al chip PMD con el cual nos comunicaremos
		   PMDChipset1, PMDChipset2 Y PMDChipset3.
	@return Retorna un dato de 16 Bits. */
	PMDuint16 readPMDdata(PMDuint16 PMDChipSet);


	/**  \ingroup basicIO
	\brief Lectura del registro del Estado del chip PMD.
	Permite al usuario determinar el estado de algunas señales de la interface del microcontrolador con 
	el chipset sin tener que desarrollar una lógica de decodificación especial.
	 
	Una operación de Status Read se desarrolla asertando  ~HostRead y ~HostSlct bajo, HostCmd alto, y 
	leyendo el bus de datos. 
	 
	@param PMDChipSet Corresponde al chip PMD con el cual nos comunicaremos
			PMDChipset1, PMDChipset2 Y PMDChipset3.
	@return El resultado de la palabra enviada por el chipset contiene
	la siguiente información:
	\verbatim
	Bit number                Description
		0-12	Sin usar, fijadas en cero.
		13		Mantiene el valor de la señal de HostIOError.
				(Ver sección 10.3.3 para mayor información sobre esta señal.).
		14		Mantiene el Varlor de la señal de ~HostIntrpt. 
				Un 1 indica que el nivel de la señal es bajo.
		15		Mantiene el valor de la señal de HostRdy. 
				1 indica que el nivel de la señal es alto.
	\endverbatim */	
	PMDuint16 readPMDstatus(PMDuint16 PMDChipSet);

	/** \ingroup basicIO
	\brief Ejecuta un Reset hacia los PMD´s por hardware, utilizando el pin 3 del puerto E  
	del microcontrolador.*/
	void writePMDreset(void);

	/* @} */
 
  
#endif
