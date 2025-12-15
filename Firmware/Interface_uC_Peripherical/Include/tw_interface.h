#ifndef __TW_INTERFACE_H__
#define __TW_INTERFACE_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      tw_interface.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_Peripherical\Include\C\tw_interface.c
//
// Proyecto: JAROBTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Estas funciones desarrollan el protocolo de comunicación en el bus I2C.
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


/**	\defgroup Interface_uC_Peripherical   PERIFÉRICOS
	\ingroup Programa_firmware
	\brief Se encuentran las funciones de interface entre el microcontrolador y los periféricos tales como el 
	reloj de tiempo real, pantalla de cristal líquido, UART, leds de visualizacion, alarma auditiva, 
	memoria EEPROM externa entre otros. */
	
	
/** \defgroup Protocolo_I2C CONTROLADOR PROTOCOLO I2C
	\ingroup Programa_firmware
	\brief Estas funciones desarrollan el protocolo de comunicación en el bus I2C.
	
	\code #include <tw_interface.h> \endcode */

	/* @{ */
	
	/** \ingroup Protocolo_I2C
	\brief Esta función inicializa la frecuencia del bus I2C a 400KHz*/
	void TWI_init(void);
	
	/** \ingroup Protocolo_I2C
	\brief Esta función envía un mensaje por el serial del Valor del registro de estado del TWI (bus I2C) para conocer
	el motivo de error.	*/
	void error(void);
		
	/** \ingroup Protocolo_I2C
	\brief Esta Función  Lee "len" bytes desde un dispositivo esclavo comenzando en "eeaddr" y las almacena
	posteriormente dentro de "buf".
	
	Esta función requiere dos ciclos: Durante el primer ciclo, el dispositivo deberá ser seleccionado
	(Modo maestro transmisor), y la dirección debe ser transmitida "sla".
	
	El segundo ciclo el dispositivo se reselecciona (Condición de reenvío de STAR llevando al Modo de maestro receptor),
	y transfiere los datos hacia el maestro.
	
	Múltiples datos pueden ser transferidos enviando una señal de ACK. En el último dato se envía NACK, por lo 
	que se le informa al dispositivo que no envíe más datos.
	
	@param sla Dirección del dispositivo esclavo.
	@param eeaddr Dirección de inicio de los datos a leer.
	@param len Número de bytes a leer.
	@param buf Arreglo en donde se almacenan los datos leídos.
	@return rv Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos leídos.
	*/
	int16 read_bytes_I2C(uint8 sla, uint32 eeaddr, int16 len, uint8 *buf);
	
	
	
	/** \ingroup Protocolo_I2C
	\brief Esta función escribe "len" bytes en un dispositivo esclavo comenzando en "eeaddr" desde "buf".
	
	Tanto la dirección y los datos son transimitos en modo Maestro Transmisor, así que no se necesita reseleccionar 
	el dispositivo.
	Sin embargo, en las memorias EEPROM sólo se puede escribir una "página" a la vez, así que se debe tener cuidado 
	de no curzar el límite de la página en un ciclo de escritura.
	
	La función termina cuando se ha escrito una "página", retornando el número
	actual de datos escritos.
	
	@param PAGE_SIZE Tamaño de la página.
	@param sla Dirección del dispositivo esclavo.
	@param eeaddr Dirección de inicio en donde se escribirán los datos.
	@param len Número de bytes a escribir.
	@param buf Arreglo en donde se almacenan los datos a escribir en el dispositivo esclavo.
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos escritos.
	*/
	int16 write_page_I2C(uint8 PAGE_SIZE,uint8 sla, uint32 eeaddr, int16 len, uint8 *buf);
	
	/** \ingroup Protocolo_I2C
	\brief Esta función llama recursivamente a write_page_I2C() hasta que todos los datos se escriban ó
	se produzca un error.
	 
	@param PAGE_SIZE Tamaño de la página. Debe ser una potencia de 2.
	@param sla Dirección del dispositivo esclavo.
	@param eeaddr Dirección de inicio en donde se escribirán los datos.
	@param len Número de bytes de datos a escribir.
	@param buf Arreglo en donde se almacenan los datos a escribir en el dispositivo esclavo.
	@return rv. Si hay un error en la transmisión rv=-1 de lo contrario retorna la cantidad de
			datos escritos.
	*/
	int16 write_bytes_I2C(uint8 PAGE_SIZE,uint8 sla, uint32 eeaddr, int16 len, uint8 *buf);
	
	
	/* @} */
	
#endif

