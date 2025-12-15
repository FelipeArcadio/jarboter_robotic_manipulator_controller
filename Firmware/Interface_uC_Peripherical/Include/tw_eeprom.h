#ifndef __TW_EEPROM_H__
#define __TW_EEPROM_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      tw_eeprom.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\tw_eeprom.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Estas funciones desarrollan operaciones de lectura y escritura en una memoria EEPROM 24LC1025.
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

#include "tw_interface.h"

	
/** \defgroup EEPROM CONTROLADOR MEMORIA EEPROM
	\ingroup Interface_uC_Peripherical
	\brief Estas funciones desarrollan operaciones de lectura y escritura en una memoria EEPROM de
	referencia 24LC1025.
	
	\code #include <tw_eeprom.h> \endcode */

	/* @{ */
	
	/** \ingroup EEPROM 
	\brief Dirección para la memoria EEPROM  24LC1025 EEPROM  en el estándar I2C:
	\verbatim
 
	1 0 1 0 B0 A1 A0 R/~W	
  
	A1 Y A0: Bits de selección. Estos bits están configurados en  cero.
	BO: Block Select Bit (A16).
	\endverbatim */
	#define TWI_SLA_24LC1025	0xA0

	
	/** \ingroup EEPROM 
	\brief Esta función  Lee "len" bytes desde una memoria EEPROM de referencia 24LC1025
	comenzando en "eeaddr" y las almacena posteriormente dentro de "buf". 
 
	\see {int16 read_bytes_I2C(uint8 sla, uint32 eeaddr, int16 len, uint8 *buf) }
	
	@param eeaddr Dirección de inicio de los datos a leer.
	@param len Número de bytes a leer.
	@param buf Arreglo en donde se almacena los datos leídos.
	@return rv Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos leídos.	*/
	int16 ee24xx_read_bytes(uint32 eeaddr, int16 len, uint8 *buf);
		
	/** \ingroup EEPROM 
	\brief Esta función escribe "len" bytes en una memoria EEPROM de referencia 24LC1025 comenzando en "eeaddr"
	los datos que se encuentran en  "buf".
	
	
	\see {int16 write_page_I2C(uint8 PAGE_SIZE,uint8 sla, uint32 eeaddr, int16 len, uint8 *buf) }
	\see {int16 write_bytes_I2C(uint8 PAGE_SIZE,uint8 sla, uint32 eeaddr, int16 len, uint8 *buf) }
	
	@param eeaddr Dirección de inicio en donde se escribirán los datos.
	@param len Número de bytes a escribir.
	@param buf Arreglo en donde se almacena los datos a escribir en EEPROM.
	@return rv Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos escritos.	*/
	int16 ee24xx_write_bytes(uint32 eeaddr, int16 len, uint8 *buf);	
	/* @} */


#endif