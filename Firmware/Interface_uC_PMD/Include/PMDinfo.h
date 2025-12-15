#ifndef __PMDinfo_H__
#define __PMDinfo_H__


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDinfo.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDinfo.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Estas funciones se utilizan para monitorear el estado del movimiento del robot a través
//	de la lectura de los registros de estado de los chips PMDs
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


	
/** \defgroup PMDinfo FUNCIONES DE ESTADO DEL MOVIMIENTO 
	\ingroup control_mov
	\brief Estas funciones se utilizan para monitorear el estado del movimiento del robot a través
	de la lectura de los registros de estado de los chips PMDs
	
	\code #include <PMDinfo.h> \endcode	*/

	/* @{ */
	
	/** \ingroup PMDinfo
	\brief Esta Función realiza la lectura del registro de estado de eventos de un determinado eje
	del chip PMD y los imprime por la consola.
	@param Axis	Eje del que se quiere leer la información.
	@param print  toma dos valores "imprimir" y "no_imprimir".
	@return Retorna 2 bytes que contiene el Valor del registro de estado de eventos del eje correspondiente.
	Este registro se define en la siguiente tabla:

	\verbatim	
Bit	 Name 				Description 	 
0 	Motion complete 	Set when a trajectory profile completes. The motion being 
						considered complete may be based on the commanded position or
						the actual encoder position. See section 6.3 for more details.
1 	Position wraparound	Set when the actual motor position exceeds 7FFFFFFFh 
						(the most positive position) and wraps to 80000000h 
						(the most negative position), or vice versa. 	 
2 	Breakpoint 1		Set when breakpoint #1 is triggered. 	 
3	Capture received 	Set when the high-speed position capture hardware acquires a new
						position value.	 
4	Motion error 		Set when the actual position differs from the commanded position
						by an amount more then the specified maximum position error. 	 
5	Positive limit 		Set when a positive limit switch event occurs. 	 
6	Negative limit 		Set when a negative limit switch event occurs. 	 
7	Instruction error 	Set when an instruction error occurs. 	 
8 	Reserved 			May contain 1 or 0.
9	Reserved 			May contain 1 or 0. 
10	Reserved 			May contain 1 or 0.	 
11 	Commutation error 	Set when a commutation error occurs (MC2300 series only). 	 
1	2Reserved 			May contain 1 or 0. 
13 	Reserved 			May contain 1 or 0. 	 
14 	Breakpoint 2 		Set when breakpoint #2 is triggered. 	 
15 	Reserved 			May contain 0 or 1. 	 
	\endverbatim */ 
	extern PMDresult PMDCheckEventStatus(int16 Axis,PMDint16 print);

	/** \ingroup PMDinfo 
	\brief Esta Función realiza la lectura del registro de estado de actividad de un determinado eje
	del chip PMD y los imprime por la consola.
	@param Axis	Eje del que se quiere leer la información.
	@param print- toma dos valores "imprimir" y "no_imprimir".
	@return Retorna 2 bytes que contiene el Valor del registro de estado de actividad del eje correspondiente.
	Este registro se define en la siguiente tabla:
	
	\verbatim	
Bit Name 					Description 	 
0 	Phasing initialized 	Set (1) when the motor’s commutation hardware has been 
							initialized. Cleared (0) ifnot yet initialized. 
							Only valid for MC2300 series chipsets. 	 
1 	At maximum velocity 	Set (1) when the commanded velocity is equal to the maximum 
							velocity specified by the host. Cleared (0) if it is not. 
							This bit functions only when the profile mode is trapezoidal,
							velocity contouring, or S-curve. It will not function
							when the chipset is in electronic gearing mode. 	 
2 	Position tracking 		Set (1) when the servo is keeping the axis within the 
							Tracking Window. Cleared (0) when it is not. See Section 6.2. 	 
3-5 Current profile mode 	These bits indicate the profile mode currently in effect, 
							which might be different than the value set using the 
							SetProfileModecommand if an Update command has not yet been 
							issued. The 3 bits define the current profile mode as follows: 
							bit 5 	bit 4 	bit 3 	Profile Mode 
							0 			0 			0 			trapezoidal 
							0			0 			1 			velocity contouring 
							0 			1 			0 			S-curve 
							0 			1 			1 			electronic gear	 
6 	Reserved 				May contain 0 or 1. 	 
7 	Axis settled 			Set (1) when the axis has remained within the Settle 
							Window for a specified periodof time. 
							Cleared (0) if it has not. See Section 7.5.
8 	Motor mode 				Set (1) when the motor is "on", cleared (0) when the motor 
							is "off."
9 	Position capture 		Set (1) when a new position value is available to read from 
							the high speed capture hardware. Cleared (0) when a new value 
							is not yet been captured. While this bit is set, no new values 
							will be captured. The command GetCaptureValue retrieves a 
							captured position value and clears this bit, allowing 
							additional captures to occur. 	 
10 In-motion indicator 		Set (1) when the trajectory profile commanded position is 
							changing. Cleared (0) when the commanded position is 
							not changing. The value of this bit may or may not correspond 
							to the value of the motion complete bit of the event status register 
							depending on whether the motion complete mode has been set 
							to commanded or actual. 	 
11 In positive limit		Set (1) when the motor is in a positive limit condition. 
							Cleared (0) when it is not.	 
12 In negative limit 		Set (1) when the motor is in a negative limit condition. 
							Cleared (0) when it is not.	 
13-15 Profile segment 		Indicates the S-curve segment number, 1-7, as shown in 
							the S-curve trajectory section of this manual. A value of 0 
							in this field indicates the trajectory is not in motion. 
							When using the External profile mode, bit 13 is 1 when the 
							trajectory generator is processing data, and 0 when stopped. 
							This field is undefined for other profile modes and may contain 
							0's or 1's. 	 

	\endverbatim */ 
	extern PMDresult PMDCheckActivityStatus(int16 Axis,PMDint16 print);

	/** \ingroup PMDinfo 
	\brief Esta Función realiza la lectura del registro de estado de señales de un determinado eje
	del chip PMD y los imprime por la consola. Este registro muestra los niveles de la señal en 
	tiempo real de varios pines MC2440 I/O.
	@param Axis	Eje del que se quiere leer la información.
	@param print toma dos valores "imprimir" y "no_imprimir".
	@return Retorna 2 bytes que contiene el valor del Registro de Estado de señales del eje correspondiente.
	Este registro se define en la siguiente tabla:
	
	\verbatim

Bit Name 			Description
0	A encoder 		A signal of quadrature encoder input.
1 	B encoder 		B signal of quadrature encoder input.
2 	Index encoder 	Index signal of quadrature encoder input.
3 	Home 			Home position capture input.
4 	Positive limit 	Positive limit switch input.
5 	Negative limit 	Negative limit switch input.
6 	AxisIn 			Generic axis input signal.
7 	Hall 1 			Hall effect sensor input number 1 (MC2300 chipset only).
8 	Hall 2 			Hall effect sensor input number 2 (MC2300 chipset only).
9 	Hall 3 			Hall effect sensor input number 3 (MC2300 chipset only).
10 	AxisOut 		Programmable axis output signal.
11-15 	Reserved

	\endverbatim */

	extern PMDresult PMDCheckSignalStatus(int16 Axis, PMDint16 print);

	/** \ingroup PMDinfo 
	\brief Esta Función realiza la lectura de algunos registros de movimiento de los chips PDMs.
	@param Axis	 Eje del que se quiere leer la información.
	@param position  Puntero para almacenar la posición enviada al eje.
	@param velocity  Puntero para almacenar la velocidad enviada al eje.
	@param acceleration  Puntero para almacenar la aceleración enviada al eje.
	@param deceleration  Puntero para almacenar la desaceleración enviada al eje.
	@param commandedposition  Puntero para almacenar la posición instantánea en el generador 
								de trayectorias.
	@param commandedvelocity  Puntero para almacenar la velocidad instantánea en el generador 
										de trayectorias.
	@param commandedacceleration  Puntero para almacenar la aceleración instántanea en el 
											generador de trayectorias.
	@param readencoder1 Puntero para almacenar la posición real leída por el Encoder.
	@param readerror Puntero para almacenar el error de posición. 
	@param encoderCounts Puntero para almacenar las cuentas del encodificador óptico.
	@param steps Puntero para almacenar los micropasos.
	*/
	extern void infomov(int16 Axis, PMDint32* position, PMDint32* velocity, PMDuint32* acceleration, 
						PMDuint32* deceleration, PMDint32* commandedposition,PMDint32* commandedvelocity, 
						PMDint32* commandedacceleration, PMDint32* readencoder1, PMDint32* readerror,
						PMDuint16* encoderCounts, PMDuint16* steps);

		
	/** \ingroup PMDinfo
	\brief Esta función retorna el código de error y limpia el error a Cero. 
	@param axis  El eje que se quiere leer el error
	@return Código de error:

- No error								0
- Processor Reset						1
- Invalid instruction					2
- Invalid axis							3
- Invalid parameter						4
- Trace running							5
- reserved								6
- Block out of bounds					7
- Trace buffer zero						8
- Bad serial checksum					9	
- Not primary port						Ah
- Invalid negative value				Bh
- Invalid parameter change				Ch
- Invalid move after limit condition	Dh
- Invalid move into limit				Eh

	 */
	extern PMDuint16 getErrorHost(int16 axis);
	
	/** \ingroup PMDinfo 
	\brief Esta función verifica la correcta conexión de los cables de sensores y del botón de parada
	de emergencia.
	@return -1, si alguno de los cables de los sensores está desconectado.
			0. Todos los cables están conectados.*/
	extern int16 checkconnect(void);
	
	/* @} */
	


#endif

