#ifndef __PMDTRACE_H__
#define __PMDTRACE_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDtrace.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDtrace.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Funciones que permiten configurar varios parámetros y registros de los Chips PMDs 
//	para capturar o monitorear continuamente variables del movimiento y finalmente almacenar estas
//	variables en memoria RAM externa.  La configuración del trazado es enviada al controlador 
//	robótico a través del campo tTrace de la estructura tCommand. Se creó el vector Tracecomand[8] 
//	de tipo tTrace tal y como se observa en la figura de abajo. Si existen variables sin configurar 
//	se inicializa a -1 dicha casilla del vector.
//
//	
//	
//	
//	|---------------------------------------------------------------------------------------|
//	|                                      Tracecomand[8]                                   |
//	|---------------------------------------------------------------------------------------|
//	| TRAZADO1 | TRAZADO2 | TRAZADO3 | TRAZADO4 | TRAZADO5 | TRAZADO6 | TRAZADO7 | TRAZADO8 | INDEX    
//	|----------|----------|----------|----------|----------|----------|----------|----------|     
//	| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| EJE FUENTE
//	|----------|----------|----------|----------|----------|----------|----------|----------|     
//	| TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TIPO DE VARIABLE
//	|----------|----------|----------|----------|----------|----------|----------|----------|     
//	
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

	/** \defgroup PMDTrace FUNCIONES DE TRAZADO DE VARIABLES
	\ingroup control_mov
	\brief Funciones que permiten configurar varios parámetros y registros de los Chips PMDs 
	para capturar o monitorear continuamente variables del movimiento y finalmente almacenar estas
	variables en memoria RAM externa.  La configuración del trazado es enviada al controlador 
	robótico a través del campo tTrace de la estructura tCommand. Se creó el vector Tracecomand[8] 
	de tipo tTrace tal y como se observa en la figura de abajo. Si existen variables sin configurar 
	se inicializa a -1 dicha casilla del vector.

	
	
	\verbatim
	|---------------------------------------------------------------------------------------|
	|                                      Tracecomand[8]                                   |
	|---------------------------------------------------------------------------------------|
	| TRAZADO1 | TRAZADO2 | TRAZADO3 | TRAZADO4 | TRAZADO5 | TRAZADO6 | TRAZADO7 | TRAZADO8 | INDEX    
	|----------|----------|----------|----------|----------|----------|----------|----------|     
	| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| AxisTrace| EJE FUENTE
	|----------|----------|----------|----------|----------|----------|----------|----------|     
	| TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TraceVar | TIPO DE VARIABLE
	|----------|----------|----------|----------|----------|----------|----------|----------|     
	\endverbatim			
			
			
	\code #include <PMDtrace.h> \endcode */
		
	/* @{ */
	
	/** \ingroup PMDTRACE 
	\brief El PMD organiza la memoria externa en buffers de datos. Cada buffer de datos tiene	
	un identificador numérico (ID). Para el trazado de variables el buffer de datos debe
	tener un ID=0.*/
	#define BUFFER_ID			0
	
	/** \ingroup PMDTRACE
	\brief Dirección BASE del buffer de datos. */
	#define BUFFER_START	0x200
	
	/** \ingroup PMDTRACE
	\brief Longitud del buffer de datos. El buffer de datos contiene 32760 campos de 16 bits que 	
	corresponden a un total de 16380 (0x3FFC) muestras de 32 bits.
	El PMD puede hacer trazos o muestras de hasta 4 Variables. Tanto la longitud del 
	buffer de datos como la cantidad de variables a trazar afectan directamente la cantidad de muestras
	que pueden ser almacenadas en la memoria RAM externa así:
	- 1 Variable					16380 Muestras de 32 Bits cada Una
	- 2 Variables					 8192 Muestras de 32 Bits cada una
	- 3 Variables    				 5460 Muestras de 32 Bits cada una
	- 4 Variables					 4095 Muestras de 32 Bits cada una*/
	#define BUFFER_LENGTH	0x3FFC    //32760 bytes--Multiplo de 12  
		
	/** \ingroup PMDTRACE
	\brief Esta función configura los bloques de Memoria RAM tanto para el Chipset1 como para el Chipset2 */
	void Memoryconfig(void);
	
	/** \ingroup PMDTRACE
	\brief Esta función configura y realiza el monitoreo de algunas variables de un eje específico.
	@param Axis Indica el eje al cual se va a monitorear*/
	void SetupSingleAxisTrace(tAxis Axis);

	/** \ingroup PMDTRACE
	\brief Esta función envía por la interfaz RS-232 los datos almacenados en la memoria RAM de los PMDs.
	@param  Chipset  Selecciona cual chipset (PMDs) se desea adquirir los datos, 1=> (PMD1) 2 => (PMD2).
	@param  nVariables Número de variables de trazado configuradas en el Chipset.
	*/
	void DisplayTraceResults(int Chipset ,PMDuint8 nVariables);
	
	
	/** \ingroup PMDTRACE
	\brief Esta función configura el trazado para cada eje del PMD. Esta configuración incluye 
	las variables a muestrear y tiempo de muestreo.
	@param  TraceComand Comando que contiene la información para configurar el trazado en los PMDs.
	*/	
	void SetupAxisTrace(tCommand TraceComand);
	
	
	/** \ingroup PMDTRACE
	\brief Esta función solo es utilizada por la función SetupAxisTrace y se encarga de devolver el ID
    de la variable para cada PMD al momento de su  configuración . Por ejemplo se  ha configurado
	las variables a trazar de la siguiente forma:
	    
	\verbatim
	|------------------------------------------------------------------------|
	|                         Tracecomand[8]                                 |
	|------------------------------------------------------------------------|
	| 		0         |        1        |       2        | 3 | 4 | 5 | 6 | 7 | TracevarIndex
	|-----------------|-----------------|----------------|---|---|---|---|---|     
	| AxisWaist (PMD1)| AxisElbow (PMD1)| AxisRoll (PMD2)| -1| -1| -1| -1| -1| Eje fuente 
	|-----------------|-----------------|----------------|---|---|---|---|---|     
	| 		1         |        3        |       1        | -1| -1| -1| -1| -1| Tipo de variable
	|-----------------|-----------------|----------------|---|---|---|---|---|     
	\endverbatim			
	Si TracevarIndex es igual a 1 tenemos que el ID de la variable de trazado es 1, pero
	si TracevarIndex es igual a 2 y la variable a trazar pertenece al PMD 2 entonces el ID de la 
	variable es 0 ya que es la primera variable configurada en el PMD 2.
	
	@param  VisualTrace Estructura que contiene la información de configuración  de las variables de trazado.
	@param  TracevarIndex Número que representa la cantidad de variables configuradas en SetupAxisTrace.
	*/
	int8 Tracevar2PMDVar(tCommand VisualTrace,int8 TracevarIndex);

	
	/** \ingroup PMDTRACE
	\brief Esta función envía por la interfaz USB los datos almacenados en la memoria RAM de los PMDs.
	@param   N_traceVariable Total de variables configuradas en el controlador.
	*/
	void EnvioTrazado2PC(uint8 N_traceVariable);
	
	
	/** \ingroup PMDTRACE
	\brief Deshabilita todas las variables de trazado para que sea posible una re-configuración de las mismas.
	*/
	void DisableVariable(void);
	/* @} */

#endif
