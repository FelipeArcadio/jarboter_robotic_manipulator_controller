#ifndef __PMDINTERRUPT_H__
#define __PMDINTERRUPT_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDinterrupt.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDinterrupt.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Funciones para la gestión de interrupciónes sobre los Chips PMDs.
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
	
/** \defgroup PMDinterrupt  MANEJADOR DE INTERRUPCIONES DE LOS PMDs
	\ingroup control_mov
	\brief Funciones para la gestión de interrupciónes sobre los Chips PMDs.
	
	\code #include <PMDinterrupt.h> \endcode	*/

	/* @{ */
	
	
	/** \ingroup PMDinterrupt
	\brief  Esta función deshabilita las interrupciónes que produce los controladores de motores (PMDs)
	hacia el microcontrolador.	*/	
	extern void HostInterruptDisable(void);
	
	/** \ingroup PMDinterrupt
	\brief  Esta función habilita las interrupciónes que produce los controladores de motores (PMDs)
	hacia el microcontrolador. Los Eventos que disparan una interrupción al microcontrolador son los siguientes:
	- 1. El eje ha completado un perfil de movimiento.
	- 2. En el eje se produjo un error de movimiento.
	- 3. El eje ha llegado al límite negativo.
	- 4. El eje ha llegado al límite positivo.
	- 5. Se produjo un error por ejecutar una instrucción no válida.*/	
	extern void HostInterruptEnable(void);
	
	/** \ingroup PMDinterrupt
	\brief Esta función realiza la(s) accion(es) necesaria(s) cuando se produce una interrupción 
	cuando el eje ha completado un perfil de movimiento.
	Las acciones son:
	- Imprime por consola un mensaje indicando al usuario que el eje a completado el movimiento.
	- Limpia el bit de estado correspondiente (PMDEventMotionCompleteMask).
	@param Axis Eje que produce la interrupción. */		
	extern void MotionCompletehandling(tAxis Axis);	
	
	/** \ingroup PMDinterrupt
	\brief Esta función realiza la(s) accion(es) necesaria(s) cuando se produce una interrupción 
	cuando el eje tiene un error de movimiento.
	Las acciones son:
	- Imprime tanto por consola como por el display un mensaje de error.
	- Limpia el bit de estado correspondiente (PMDEventMotionErrorMask).
	- Detiene suavemente el eje.
	@param Axis Eje que produce la interrupción. */		
	extern void MotionErrorhandling(tAxis Axis);	
	
	/** \ingroup PMDinterrupt
	\brief Esta función realiza la(s) accion(es) necesaria(s) cuando se produce una interrupción 
	cuando el eje ha llegado al límite positivo.
	Las acciones son:
	- Imprime por consola un mensaje indicando al usuario que el eje ha llegado al límite positivo.
	- Limpia el bit de estado correspondiente (PMDEventInPositiveLimitMask).
	- Mueve el eje hasta que desactive el interruptor de fin de carrera positivo.
	Nota: Para Pitch y Roll se realizan procedimientos especiales.
	@param Axis Eje que produce la interrupción. */		
	extern void Positivehandling(tAxis Axis);
	
	/** \ingroup PMDinterrupt
	\brief  Esta función realiza la(s) accion(es) necesaria(s) cuando se produce una interrupción 
	cuando el eje ha llegado al límite negativo.
	Las acciones son:
	- Imprime por consola un mensaje indicacando al usuario que el eje ha llegado al límite negativo.
	- Limpia el bit de estado correspondiente (PMDEventInNegativeLimitMask).
	- Mueve el eje hasta que desactive el interruptor de fin de carrera negativo.
	Nota: Para Pitch y Roll se realizan procedimientos especiales.	
	@param Axis Eje que produce la interrupción. */	
	extern void Negativehandling(tAxis Axis);	
	
	/** \ingroup PMDinterrupt
	\brief Esta función realiza la(s) accion(es) necesaria(s) cuando se produce una interrupción 
	cuando el PMD presenta un error de instrucción en un eje determinado.
	Las acciones son:
	- Imprime tanto por consola como por el display un mensaje de error.
	- Limpia el bit de estado correspondiente (PMDEventInstructionErrorMask).
	@param Axis Eje que produce la interrupción. */		
	extern void InstructionErrorhandling(tAxis Axis);
	
	/** \ingroup PMDinterrupt
	\brief Función que realiza la gestión de las interrupciones sobre un eje.
	- Halla la fuente de la interrupción. Por ejemplo: El eje llego al límite positivo.
	- Dependiendo de la interrupción realiza tareas a seguir. Por ejemplo: Si el eje llegó al límite
	  positivo, mover la articulación hasta que no haya evento de fin de carrera.
	- Limpia la bandera de interrupción con el fin de rehabilitar las interrupciones.		 
	@param Axis Eje que produce la interrupción. */		
	extern void interruptAxisHandling(tAxis Axis);	
	
	/** \ingroup PMDinterrupt
	\brief Esta función realiza la gestión de las interrupciones sobre el chipset número 1. Cada chipset soporte cuatro
	ejes. El sistema robótico tiene actualmente 2 chipsets.
	Es posible que sobre un mismo chipset varios ejes produzcan interrupciones al mismo tiempo. Esta función 
	detecta que ejes han producido interrupciones en un determinado chipset y las procesa una a una hasta que 
	no exista ninguna interrupción pendiente.*/	
	extern void InterruptHandling_chipset1(void); 
	
	
	/** \ingroup PMDinterrupt
	\brief Esta función realiza la gestión de las interrupciónes sobre el chipset número 2. Cada chipset soporte cuatro
	ejes. El sistema robótico tiene actualmente 2 chipsets.
	Es posible que sobre un mismo chipset varios ejes produzcan interrupciónes al mismo tiempo. Esta función 
	detecta que ejes han producido interrupciones en un determinado chipset y las procesa una a una hasta que 
	no exista ninguna interrupción pendiente.*/	
	extern void InterruptHandling_chipset2(void); 
	

	
	
	/* @} */
	


#endif