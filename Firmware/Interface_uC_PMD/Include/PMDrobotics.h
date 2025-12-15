#ifndef __PMDROBOTICS_H__
#define __PMDROBOTICS_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PMDrobotics.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\PMDrobotics.c
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Funciones de Robótica sobre los chips PMDs 	que realizan movimientos de perfil trapezoidal en los
//	motores, procesamiento de trayectorias entre otras más.
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

	//#define CORRELACION_HOMBRO_CODO 1.047
	#define CORRELACION_HOMBRO_CODO 0.82950//0.862703
	/*Si se  baja el valor, aumenta el angulo entre codo y pitch*/
	#define CORRELACION_CODO_PITCH  0.01728//0.01650//0.01588//0.01500//0.01677//0.01728
	//#define CORRELACION_CODO_PITCH  1.0111

	//Definiciones para los movimientos de pitch y roll
	//Actualiza los encoderes con Mov_Anterior_RAM y la ram a Posicion
	#define MOVIMIENTO_NORMAL 1  
	//No actualiza ni encoderes, ni ram 
	#define MOVIMIENTO_HACIA_HOME 2 
	//No actualiza ni encoderes, ni ram 
	#define EN_CALIBRACION_PROCESOHOME 3
	//Iniciaza a cero Mov_Anterior_RAM y encoderes
	#define INICIANDO_HOME_PITCH_ROLL 4

/** \defgroup Rutinas_de_robotica FUNCIONES DE ROBÓTICA
	\ingroup control_mov
	\brief Módulo que contiene funciones de róbotica como efectuar el seguimiento de una trayectoria,
	ir a una pose determinada, abrir o cerrar el gripper entre otras más.
*/


	
/** \defgroup PMDrobotics PMDROBOTICS
	\ingroup Rutinas_de_robotica
	\brief Funciones de Robótica sobre los chips PMDs 	que realizan movimientos de perfil trapezoidal en los
	motores, procesamiento de trayectorias entre otras más.
	
	\code #include <PMDrobotics.h> \endcode	
*/

/* @{ */
	
	/** \ingroup PMDrobotics
	\brief Esta función realiza un movimiento en un eje determinado.
	@param Axis Indica el eje a mover
	@param position Define la posición deseada. Si el perfil usado es velocidad este campo es ignorado por los PMD.
	@param velocity Define la velocidad deseada para el movimiento.
	@param acceleration Define la aceleración del movimiento. 
	@param actualizar Define si se desea realizar el movimiento o simplemente cargar los registros. 
	*/
	extern void mov_axis(tAxis Axis,PMDint32 position, PMDint32 velocity, 
								PMDuint32 acceleration, PMDint16 actualizar);

	
	
	/** \ingroup PMDrobotics
	\brief Esta función coloca en cero las variables de la memoria EEPROM que definen la posición anterior de 
	los ejes Pitch y Roll.*/
	extern void inipitch_roll(void);
	
	
	/** \ingroup PMDrobotics
	\brief Esta función realiza un movimiento de pitch (Cabeceo) en la muñeca.
	@param position Define la posición deseada. Si el perfil usado es velocidad este campo es ignorado por los PMD.
	@param velocity Define la velocidad deseada para el movimiento. 
	@param acceleration Define la aceleración del movimiento.
	@param actualizar Define si se desea realizar el movimiento o simplemente cargar los registros. 
	*/
	extern void pitch(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration,
							PMDint16 actualizar,PMDint16 modo_movimiento);

	/** \ingroup PMDrobotics
	\brief Esta función realiza un movimiento de roll (Giro) en la muñeca.
	@param position Define la posición deseada. Si el perfil usado es velocidad este campo es ignorado por los PMD. 
	@param velocity Define la velocidad deseada para el movimiento. 
	@param acceleration Define la aceleración del movimiento. 
	@param actualizar Define si se desea realizar el movimiento o simplemente cargar los registros. 
	@param modo_movimento Indica si se esta haciendo un movimiento para buscar HOME.
	*/
void roll(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration,
					PMDint16 actualizar,PMDint16 modo_movimiento);
					
	/** \ingroup PMDrobotics
	\brief Esta función mueve el Eje 2 proporcionalmente al movimiento del Eje1, esta proporción  está definida
	 por FACTOR_CORRELACION.
	@param TrajectoryPOINT Contiene la información del movimiento.
	@param pointIndex Punto a de la trayectoria a leer.
	@param Eje1 Primer eje que interviene el movimiento.
	@param Eje2 Segundo eje que interviene el movimiento.
	@param FACTOR_CORRELACION Factor de relación entre el eje 1 y el eje 2.
	*/
	//extern void mov_elbow(PMDint32 position, PMDint32 velocity, PMDuint32 acceleration);
	void mov_correlacionado(tTrajectory   *TrajectoryPOINT, int16 pointIndex, 
							tAxis Eje1, tAxis Eje2, double FACTOR_CORRELACION);
	
	/** \ingroup PMDrobotics
	\brief Esta Función realiza una parada suave en cada uno de los ejes. Es decir los ejes van 
	desacelerando hasta que la velocidad sea 0.*/
	extern void sofstop(void);

	/** \ingroup PMDrobotics
	\brief Esta Función realiza una parada abrupta en cada uno de los ejes*/
	extern void abruptstop(void);
	
	/** \ingroup PMDrobotics
	\brief Esta función chequea si algún eje está en movimiento.
	@param mask Indica los ejes que se toman en cuenta para determinar si el PMD está ocupado.
	@return 	TRUE- Ningún eje se encuentra en movimiento.
				FALSE-Algún eje está en movimiento. */
	extern PMDresult PMDcheckIdle(int16 mask);	
 
	
	/**\ingroup PMDrobotics
	\brief Función que limpia la bandera de Motion Complete del Event Status Register para un eje determinado 
	con el fin de habilitar el eje para el próximo movimiento.
	@param axis El eje que se desea habilitar.*/
	extern void motioncomplete(int16 axis);
	
	/**\ingroup PMDrobotics
	\brief Esta función limpia la bandera de Motion Complete del Event Status Register de todos los ejes,
	esto con el fin de habilitarlos para el próximo movimiento.*/
	extern void Enable_Mov(void);
	
	/**\ingroup PMDrobotics
	\brief Esta función realiza un movimiento en la dirección negativa con el fin de encontrar el
	HOME de la articulación y el límite máximo negativo.
	@param Axis El eje.
	@param home  Retorna el valor del registro de captura de posición de alta Velocidad.
	@param posicion  Retorna el valor actual de posición.
	@param rangos  Esta variable es una variable Booleana que toma dos valores TRUE o FALSE.
	Si esta Variables es FALSE significa que una vez es encontrado el HOME el eje se detiene 
	inmediatamente  y no se halla el límite máximo negativo.*/
	extern void search_home_negative(int16 Axis,PMDint32 *home,PMDint32 *posicion,PMDint16 rangos);
	
	/**\ingroup PMDrobotics
	\brief Esta función realiza un movimiento en la dirección positiva con el fin de encontrar el
	HOME de la articulación y el límite máximo positivo.
	@param Axis El eje.
	@param home  Retorna el valor del registro de captura de posición de alta Velocidad.
	@param posicion  Retorna el valor actual de posición.
	@param rangos  Esta variable es una variable Booleana que toma dos valores TRUE o FALSE.
	Si esta Variables es FALSE significa que una vez es encontrado el HOME el eje se detiene 
	inmediatamente  y no se halla el límite máximo positivo.*/
	extern void search_home_positive(int16 Axis,PMDint32 *home,PMDint32 *posicion,PMDint16 rangos);
	
	/**\ingroup PMDrobotics
	\brief Esta función realiza un movimiento en la dirección positiva con el fin de encontrar el
	HOME de roll.
	@param home  Apuntador para poder almacenar el home encontrado.
	*/
	extern void search_home_roll_positive(PMDint32 *home);

	/** \ingroup PMDrobotics
	\brief Esta función realiza el seguimiento de una trayectoria.
	@param Trajectory Contiene la informacion de la trayectoria.	
	@param TraceOP	Indica si se desea trazar el movimiento.
	*/
	extern void PMDprocessTrajectory(tTrajectory* Trajectory, int16 TraceOP);	
	
	/** \ingroup PMDrobotics
	\brief Esta función calcula los rangos articulares de cada eje	exceptuando el eje de Roll. 
	Esta función mueve  el eje a sus límites articulares, encuentra el home, hace home=0, retorna el eje  al home.
	@param Axis	El eje.*/
	extern PMDresult initializeAxis(int16 Axis); 
	
	/** \ingroup PMDrobotics
	\brief Esta función inicializa mecánicamente todos los ejes y debe ser ejecutada cuando se 
	reinicia el sistema. Esta función mueve  los ejes a sus límites articulares y encuentra el home.
	*/
	void go_home(void);
	
	/** \ingroup PMDrobotics
	\brief Esta función mueve todos las articulaciones del robot utilizando la información almacenada en 
	TrajectoryPOINT (Posición, velocidad, aceleración). 
	@param TrajectoryPOINT Estructara tipo trajectoria que tiene la información del movimiento.*/	
	extern void teach_pendant_point(tTrajectory   *TrajectoryPOINT, int16 pointIndex);
	
	/** \ingroup PMDrobotics
	\brief Esta función retorna el máximo valor entre a y b.
	@param a	Variable a comparar.
	@param b	Variable a comparar.
	*/
	extern PMDint32 max(PMDint32 a, PMDint32 b);
	
	/** \ingroup PMDrobotics
	\brief Esta función retorna el mínimo valor entre a y b.
	@param a	Variable a comparar.
	@param b    Variable a comparar.
	*/
	extern PMDint32 min(PMDint32 a, PMDint32 b);
	
	/** \ingroup PMDrobotics
	\brief Permite conocer la relación mecánica al devolver el recorrido en micropasos de una 
	articulación (Axis) conociendo de antemano el barrido en ángulo para este mismo eje.
	@param Axis	eje donde se desea conocer su relación mecánica.*/
	extern PMDint32 calibration(int16 Axis);
	
	/** \ingroup PMDrobotics
	\brief Carga directamente los registros de posición del PMD con la información de la POSE, sin ejecutar update.
	@param Pose	Estructura tipo POSE.*/
	extern void ObtenerPose(PoseRobot *Pose);
	
	/** \ingroup PMDrobotics
	\brief Imprime los rangos en grados de un eje correctamente configurado.
	@param Axis	eje donde se desea imprimir sus rangos.
	*/
	extern void rangosrobot(tAxis Axis);
	
	/** \ingroup PMDrobotics
	\brief Permite determinar si un eje está bien calibrado, al ejecutar un movimiento de una 
	revolución del encodificador óptico.
	@param Axis	eje a probar.
	*/
	void StepCalibration(tAxis Axis);

	
	
	/*En desarrolo*/	
	/** \ingroup PMDrobotics
	\brief Carga los registro de las articulaciones utilizando el factor correlacion
		   y un perfil de velocidad continia.
	*/
	
	void mov_correlacionado_HOME(tAxis Eje2, PMDint32 Posicion,
										 PMDint32 Velocity_Eje1, PMDint32 *Velocity_Eje2,
						                 PMDuint32 Acceleration_Eje1, PMDuint32 *Acceleration_Eje2, 
										 double FACTOR_CORRELACION);
	/*En desarrolo*/	
	/** \ingroup PMDrobotics
	\brief Coloca los ejes en una posicion donde no esten activos los switch para que no se produca 
	errores de moviemiento en el proceso de busqueda del home.
	@param Ini_Axis	eje inicial donde se comienza a probar esta condicion. 
	Siempre se finaliza en el eje pitch.
	*/
	void No_limit_switch(int Ini_Axis);
	
	
	/*En desarrolo*/	
	/** \ingroup PMDrobotics
	\brief Retorna el estatus del eje determinado en movimiento.
	@param Axis	eje a probar.
	@return rcPMDAxisPresicionMotionComplete- Movimiento completado con presicion.
			rcPMDAxisPresicionError-Movimiento completado pero con error de presicion.
			rcPMDNoProfileactivo-Indica generacion profile inactiva.
	*/
	PMDresult Status_Axis(tAxis Axis);


	/** \ingroup PMDrobotics
	\brief Carga el registro de la posición actual (posición del encoder) para un eje determinado. Realiza la
	corrección cuando el valor a cargar es negativo.
	@param Axis	eje al que se le ajustará el registro de posición actual (posición del encoder)
	@param AJUSTE es el valor que se le cargará al registro del enconder.*/
	extern void SetPosicionActual(tAxis Axis, PMDint32 AJUSTE);
	
	
		/** \ingroup PMDrobotics
	\brief Realiza la corrección a la función PMDAdjustActualPosition cuando cuando el movimiento anterior fué
	negativo.
	@param Axis	eje al que se le ajustará el registro de posición actual (posición del encoder).
	@param AJUSTE es el valor que se le ajustará al registro del enconder.*/
	extern void AjustarPosicionActual(tAxis Axis, PMDint32 AJUSTE);
	
	/* @} */
	


#endif
