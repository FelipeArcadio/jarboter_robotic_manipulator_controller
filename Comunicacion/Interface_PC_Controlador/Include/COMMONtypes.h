#ifndef __COMMONTYPES_H__
#define __COMMONTYPES_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      COMMONtypes.h
// Ubicación fuente: No tiene
//
// Proyecto: Controlador Robótico de 5 grados de libertad
//  
// Propósito General:
// Declaraciones de estructuras y variables del sistema de comunicación  que son comunes  tanto en el PC como 
//	en el microcontrolador.
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
// Copyright (c) 2011
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DOF 5         /**< Grados de libertada del robot.*/
#define MAXAXIS 12    /**< Máxima cantidad de grados de libertad, 4 ejes  por 3 tarjetas controladoras de motores.*/



/** \defgroup com_types TIPOS DE DATOS DEL PROTOCOLO DE COMUNICACIÓN
	\ingroup Protocolo_comunicacion_PC
	\brief Declaraciones de estructuras y variables del sistema de comunicación  que son comunes  tanto en el PC como 
	en el microcontrolador.
	
	\code #include <Commontypes.h> \endcode
*/

/* @{ */
	
/**	\ingroup com_types
*	\brief Estandarización de los tipos de datos con el fin de utiliar los mismos tamaños tanto en el compilador de PC 
	como el compilador GCC del microcontrolador. 
*/	

/** \ingroup com_types
* @{ */
typedef unsigned char   		uint8;           /**< Definición variable unsigned char como un entero sin signo de 8 bits*/
typedef signed char        		int8;            /**< Definición variable signed char   como un entero de 8 bits*/
typedef signed short int   		int16;           /**< Definición variable signed short int  como un entero de 16 bits*/
typedef unsigned short int		uint16;			 /**< Definición variable unsigned short int como un entero sin signo de 16 bits*/
typedef signed long int    		int32;           /**< Definición variable signed long int  como un entero de 32 bits*/
typedef unsigned long int  		uint32;          /**< Definición variable unsigned long int como un entero sin signo de 16 bits*/
typedef unsigned char      		tString[256];    /**< OJO ERROR REDIFINIICION*/
/** @} */

//----------------------------------------------------------------------------


/** \ingroup com_types 
\brief Definición de los tipos de datos para conversion*/
#define POSICION_TYPE 0
#define VELOCIDAD_TYPE 1
#define ACELERACION_TYPE 2






/** \ingroup com_types
*/
typedef  int16 tAxis;   /**< Definición del tipo tAxis como un entero  de 16 bits*/

/** \ingroup com_types
*   
*   \enum tEnumtAxis
*	\brief Valores áalidos que puede tomar el tipo de dato tAxis
*/
enum tEnumtAxis{ 
  AxisWaist,        	/**< El código válido para AxisWaist, es 0*/
  AxisShoulder,     	/**< El código válido para AxisShoulder, es 1*/
  AxisElbow,        	/**< El código válido para AxisElbow, es 2*/
  AxisPitch,        	/**< El código válido para AxisPitch, es 3*/
  AxisRoll,           	/**< El código válido para AxisRoll, es 4*/
  AxisAux1,           	/**< El código válido para AxisAux1, es 5*/
  AxisAux2,          	/**< El código válido para AxisAux2, es 6*/
  AxisAux3,           	/**< El código válido para AxisAux3, es 7*/
  AxisAux4,           	/**< El código válido para AxisAux4, es 8*/
  AxisAux5,           	/**< El código válido para AxisAux5, es 9*/
  AxisAux6,           	/**< El código válido para AxisAux6, es 10*/
  AxisAux7,     		/**< El código válido para AxisAux7, es 11*/
  AxisInvalid           /**< Cualquier código mayor o igual a AxisInvalid es considerado como error*/
  };


//----------------------------------------------------------------------------

/** \ingroup com_types
*/
typedef int16 AxisMask; /**< Definición del tipo AxisMask como un entero  de 16 bits*/

/** \ingroup com_types
*   @{ 
*   \enum tEnumAxisMask
*	\brief Se utiliza para enviar o recibir en forma de código que articulación esta activa o habilitada para 
		el movimiento.
*/
enum tEnumAxisMask {  
  NOAxisMask=0x0000,    /**< Código que indica que no hay ejes activados */
  WaistMask=0x0001,     /**< Código que indica que Cintura está activada */
  ShoulderMask=0x0002,  /**< Código que indica que Hombro está activada */
  ElbowMask=0x0004,     /**< Código que indica que Codo está activada */
  PitchMask=0x0008,     /**< Código que indica que Pitch está activada */
  RollMask=0x0010,      /**< Código que indica que Roll está activada */
  Aux1Mask=0x0020,      /**< Código que indica que EjeAux1 está activada */
  Aux2Mask=0x0040,      /**< Código que indica que EjeAux2 está activada */
  Aux3Mask=0x0080       /**< Código que indica que EjeAux3 está activada */
}; 
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct taxisData
*	\brief Estructura que almacena la información de posición, velocidad y aceleración de un eje, en un 
			determinado punto sobre una trayectoria.
*/
typedef struct {	                    
    int32 position; 	/**< Variable de 32bits que almacena la posición de la articulación */
    int32 velocity;		/**< Variable de 32bits que almacena la velocidad de la articulación */
    int32 acceleration; /**< Variable de 32bits que almacena la aceleración de la articulación */
  } taxisData;
/** @} */ 
//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct tpoint
*	\brief Estructura que  define un punto de la trayectoria para todos los ejes del robot.
*/
typedef struct {
    taxisData axis[DOF];  /**< Estructura que  corresponde a un punto sobre una trayectoria. DOF es el número de ejes 
								y cada casilla tiene tres campos: posición, velocidad y aceleración*/ 
	int8  Gripper_Status;	
  } tpoint;
/** @} */ 

//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct tTrajectory
*	\brief Estructura que define una trayectoria. Un ejemplo para el acceso a la información  de esta 
*	estructura sería: trajectory[Número del punto].axis[Eje].acceleration
*/
typedef struct {
    int16 size;         /**< Cantidad de puntos de la trayectoria. */ 
    int16 invalidmask;  /**<  Máscara que índica que  eje es válido o inválido.*/ 
    tpoint *point;      /**< Apunta al vector que contiene la información de la trayectoria para todos los ejes. */ 
  } tTrajectory;
/** @} */ 

//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct PoseRobot
*	\brief Estructura que define una pose del Robot, incluyendo el estado del efector final
*/
typedef struct{
  	uint16 id;							/**< Etiqueta o Nombre de la Pose. Es decir Pose 1, Pose 2...etc.*/
	int32 Pos_Waist;					/**< Posición Cintura */
	int32 Pos_Shoulder;					/**< Posición Hombro */
	int32 Pos_Elbow;					/**< Posición Codo */
	int32 Pos_Pitch;					/**< Posición Pitch */ 
	int32 Pos_Roll;						/**< Position Roll */
	int8  Gripper_Status;				/**< El estado del Gripper: Abierto (1) o Cerrado (0) */
}PoseRobot;
/** @} */ 

//----------------------------------------------------------------------------

/** \ingroup com_types
*   @{ 
*   \struct tAxisSettings
*	\brief Estructura que contiene los parámetros de configuración de un eje determinado.
*/
typedef struct {
	int8  axisEnabled;                 /**< TRUE o FALSE, eje habilitado o no*/ 
    int8  interruptEnable;			   /**< TRUE or FALSE, eje es habilitado o no para generar una Int de un evento */ 
	int16 switchEnable;				   /**< TRUE or FALSE, habilita el evento de limite de carrera */
	int16 encoderEnable;			   /**< TRUE or FALSE, permite o no la retroalimentación por encodificador óptico */
	int16 maxAxisAngle;				   /**< Máximo ángulo de desplazamiento */
	int16 minAxisAngle;				   /**< Mínimo ángulo de desplazamiento */
	int16 ustepsPerStep;               /**< Configura el número de pasos por micropaso del motor, 1..256 usteps/step */
    int16 motorStepsPerRevolution;     /**< Configura el número de pasos por revolución del motor */
    int16 encoderStepsPerRevolution;   /**< Configura el número de pulsos  del encodificador óptico por revolución */
	int16 TrackingWindow;			   /**< Configura el tamaño de la ventana de monitoreo */
	int16 SettleWindow;				   /**< Configura el fin del movimiento basándose en un rango dentro de 
											la ventana de monitoreo */
	int16 SettleTime;				   /**< Tiempo que indica el fin de movimiento */
    int16 motorPower;                  /**< Porcentaje de la potencia del motor */ 
    int16 numberPhases;                /**< Numero de fases del motor */
	int16 homeSwitchHandling;          /**< Indica que si el HOME es el limite positivo o el limite negativo  */
    int16 gearNominator;               /**< Numerador de la relación mecánica del eje motor::articulación */ 
    int16 gearDenominator;             /**< Denominador de la relación mecánica del eje motor::articulación del robot.  
											micropasos = (gearNominator/gearDenominator)*(delta(radians)/motorStepsPerRevolution)*ustepsPerStep*/ 
	int32 ErrorLimit;				   /**< Máximo error de posición permitido */
    int32 limitAcceleration;           /**< Limite de aceleración permitido para el eje */
    int32 limitVelocity;               /**< Límite de velocidad permitido para el eje */
    int32 limitJerk;                   /**< Limite de máximo valor de Jerk  */
    int32 maxAcceleration;             /**< Máxima aceleración para la articulación*/
    int32 maxVelocity;                 /**< Máxima velocidad para la articulación */
    int32 minVelocity;                 /**< Mínima velocidad para la articulación */
	int32 homeVelocity;					/**< Velocidad para ejecución de del procedimiento de HOMe en la articulación */
	int32 homeAcceleration;				/**< Aceleración  de home en la articulación */
  } tAxisSettings; 
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct tAxisLimits
*	\brief Almacena la posición máxima y mínima para un eje determinado
*/
typedef struct {
    int32 minPosition;       /**< Posición mínima de la articulación */
    int32 maxPosition;       /**< Posición máxima de la articulación */
  } tAxisLimits;
/** @} */



//----------------------------------------------------------------------------
// the packet header definition:
//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \brief Definición de los tamaños de las variables que construyen la trama del protocolo de comunicación, 
*   para asegurarse que tanto el uC y PC manejen los mismos tipos de datos.
*/

typedef int16 tDataType;
typedef int16 tDataLength;
typedef int16 tDataChecksum;      
typedef int16 tHeaderChecksum;   
/** @} */

//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct tPacketHeader
*	\brief Estructura que define la cabecera en el protocolo de comunicación.
*/
typedef struct {
    tDataType dataType;         	/**< Campo donde se almacena el tipo de dato que contiene la trama */
    tDataLength dataLength;    		/**< Indica cuantos bytes tiene la trama */
    tDataChecksum dataChecksum;		/**< Almacena el cálculo del Checksum de los datos */ 
    tHeaderChecksum headerChecksum; /**< Almacena el cálculo de Checksum de la cabecera */
  } tPacketHeader;
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct tTrace
*	\brief Estructura que contiene la información para ejecutar el trazado de un eje.
*/
typedef struct {
	int8 AxisTrace;             /**< Indica en que eje se va a ejecutar el trazado */
	int8 TraceVar;              /**< Indica que variable se va a monitorear */
}tTrace;
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \struct tCommand
*	\brief Estructura que contiene la configuración del comando para ser ejecutado por el controlador.
*/
typedef struct {
	int16  CommandOP;          /**< Código de comando a ejecutar */
    int16  Axis;               /**< Sobre que eje se realiza la acción */
    int32  Dato;               /**< Información adicional que necesite el comando */
	uint16 TraceTime;          /**< Indica el tiempo de monitoreo, en caso de utilizar el comando CommandSetPMDTrace */
	int8   TraceTraj;          /**< Indica que se quiere monitorear una trayectoria */
	tTrace Tracecomand[8];     /**< Almacena la configuración para ejecutar el trazado para cada PMD*/
}tCommand;
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \enum tEnumCommand
*	\brief Posibles valores que puede tomar tCommand.CommandOP
*/

enum tEnumCommand {
	CommandNOP,                /**< Indica no acción */
	CommandGoHome,             /**< Ordena al robot ir a la posición de descanso */
	CommandGetAxisSettings,    /**< Pide la configuración de un eje al controlador */
	CommandOpenGripper,        /**< Ordena abrir el gripper */
	CommandCloseGripper,       /**< Ordena cerrar el gripper */
	CommandSetPoseEEPROM,      /**< Graba una pose en EEPROM */
	CommandReSetPoseEEPROM,    /**< Sobrescribe una pose en EEPROM  */
	CommandGetPoseEEPROM,      /**< Lee una pose del controlador */
	CommandGetPoseActual,      /**< Pregunta sobre la pose actual */
	CommandEraseEEPROM,        /**< Borra la totalidad de la EEPROM */
	CommandGetPosesID,         /**< Retorna al PC un vector que contiene los IDs de las poses en el sistema */
	CommandSetPMDTrace,   	   /**< Configura el trazado para todos los ejes */
	CommandGetPMDTrace,   	   /**< Envía la información del trazado hacia el PC */
	CommandGetFecha,	       /**< Envía la fecha actual del sistema según el RTC */
	CommandGetMemory,	       /**< Retorna la cantidad de memoria disponible */
	CommandGetSensor,	       /**< Muestra por serial el estado de todos los sensores */
	CommandGetEventStatus,     /**< Muestra por serial el estado de EventStatus */
	CommandGetAvtivityStatus,  /**< Muestra por serial el estado de AvtivityStatus */
	CommandGetinfomov          /**< Obtiene la información del movimiento. */
};

#define lastCommand CommandGetinfomov   // for error checking
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*/
typedef int16 tPacket; /**< Definición del tipo tPacket como un entero  de 16 bits*/

/** \ingroup com_types
*   @{ 
*   \enum tEnumPacket
*	\brief Declara todo los paquetes posibles que se pueden enviar o recibir utilizando el protocolo de comunicación.
*/
enum tEnumPacket {
  PacketNOP,          /**< Indica paquete no válido */
  PacketCommand,      /**< Indica paquete tipo comando, utilizado para realizar una acción */
  PacketVector,       /**< Indica paquete tipo vector, permite enviar vectores*/
  PacketTrajectory,   /**< Indica paquete tipo trayectoria*/
  PacketTeachPendant, /**< Indica paquete tipo teach pendant, indica que el paquete solo tiene un punto de la trayectoria*/
  PacketString,       /**< Indica paquete tipo String, utilizado para enviar cadenas*/
  PacketAxisSettings, /**< Indica paquete tipo AxisSettings, permite leer o enviar la información de la configuración del eje*/
  PacketAxisLimits,   /**< Indica paquete tipo AxisLimits*/
  PacketHeader,       /**< Indica paquete tipo PacketHeader, permite enviar primero la cabecera para luego leer los datos*/ 
  PacketPosesvector,  /**< Indica paquete tipo Posesvector, utilizado para leer las poses almacenadas en el controlador*/ 
  PacketFecha,		  /**< Indica paquete tipo Fecha, utilizado para configurar o leer la fecha/hora del RTC*/
  PacketAlarma,	      /**< Indica paquete tipo Alarma, permite configurar una tarea programada en el sistema*/
  PacketTrace,        /**< Indica paquete tipo Trace, utilizado para enviar la información del trazado al PC*/ 
  Packetuint16        /**< Indica paquete tipo int16, un dato genérico de 16 bits*/ 
  }; // tPacket
#define lastPacket Packetuint16  // for error checking
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types	
*/
typedef int16 tReturnCode; /**< Definición del tipo tReturnCode como un entero  de 16 bits*/

/** \ingroup Enumeraciones	
*   @{ 
*   \enum tEnumReturnCode
*	\brief Códigos de retorno del protocolo de comunicación.
*/
enum tEnumReturnCode {
  rcOK = 0,                     /**< Indica éxito */ 
  // data packet related:       
  rcInvalidHeader,              	/**< Indica error al recibir el encabezado*/
  rcInvalidData,               		 /**< Indica error en los datos, datos vacíos */ 
  rcInvalidPacket,             		 /**< Indica tipo de paquete fuera del rango permitido*/ 
  rcInvalidCommand,            		 /**< Indica tipo de comando desconocido*/ 
  rcInvalidChecksum,           		 /**< Indica error en el cálculo del checksum de los datos*/ 
  rcInvalidHeaderChecksum,     		 /**< Indica error en el cálculo del checksum del encabezado */ 
  rcTooFewData,                		 /**< Indica error en la cantidad de datos */ 
  rcNoMemory,                   	/**< Indica que no tiene memoria suficiente para almacenar los datos */ 
  // USB communication related:
  rcTimeoutSendingData,         	/**< Indica error al mandar información por el USB */
  rcTimeoutReadingData,				/**< Indica error al recibir información por el USB */
  rcCommunicationError,   			/**< Indica error de comunicación */
  rcUnexpectedState,				/**< Indica driver USB ocupado */
  // robot related
  rcHardwareFailure,				/**< Indica error en el hardware */
  // PMD chipset related:
  rcPMDChipsetResetError,  			/**< Indica error al ejecutar un reset*/
  rcPMDchecksumError,   			/**< Indica error de Checksum para los PMDs*/
  rcPMDAxisPresicionMotionComplete, /**< Indica movimiento terminado tomando en cuenta la presicion */
  rcPMDAxisPresicionError,   		/**< Indica movimiento terminado pero con error de presicion */
  rcPMDProfileactivo,   	    	/**< Indica generacion profile activo */
  // file procecing related
  rcFileOpenError,			    	/**< Indica error al abrir el archivo, inexistente o no válido */
  rcFileOpenOK,						/**< Indica que se ha abierto el archivo con éxito */
  rcFileReadOK,						/**< Indica que fué leído el archivo abierto  */
  rcFileReadError  					/**< Indica que ocurrió un error de lectura del archivo */
  }; // tReturnCode
#define lastRC rcFileReadError  
/** @} */


//----------------------------------------------------------------------------
/** \ingroup com_types
*   @{ 
*   \enum tEnumRTC_TASTCode
*	\brief Indica qué tipo de tarea se va a ejecutar cuando ocurra un evento de alarma (tarea programada).
*/
typedef int16 tRTC_TASTCode;
enum tEnumRTC_TASTCode {
	TAST_Opengripper,         /**< Abrir el gripper */
	TAST_Closegripper,        /**< Cerrar el gripper */
	TAST_IDVectorTrajectory,  /**< Ejecutar una trayectoria de poses */
	TAST_RunlasrxrTrajectory  /**< Ejecutar la última trayectoria que fué descargada al controlador. */
  }; // tRTC_TASTCode
#define tRTC_TASTCode TAST_RunlasrxrTrajectory 
/** @} */

//Fecha y Hora
//----------------------------------------------------------------------------
/** \ingroup com_types
*   \enum tdia
*	\brief Define valores numéricos para los días de la Semana .*/	
typedef enum 
{
	Domingo=0,				/**< Valor numérico para el día Domingo*/
	Lunes=1,				/**< Valor numérico para el día Lunes*/
	Martes=2,				/**< Valor numérico para el día Martes*/
	Miercoles=3,			/**< Valor numérico para el día Miercoles*/
	Jueves=4,				/**< Valor numérico para el día Jueves*/
	Viernes=5,				/**< Valor numérico para el día Viernes*/
	Sabado=6				/**< Valor numérico para el día Sabado*/
} tdia;


//----------------------------------------------------------------------------
/** \ingroup com_types
*   \enum tmes
*	\brief Define los valores numéricos en formato BCD para los meses del año.*/	
typedef enum 
{
	Enero=0x01,				/**< Valor numérico para el Mes de Enero*/
	Febrero=0x02,			/**< Valor numérico para el Mes de Febrero*/
	Marzo=0x03,				/**< Valor numérico para el Mes de Marzo*/
	Abril=0x04,				/**< Valor numérico para el Mes de Abril*/
	Mayo=0x05,				/**< Valor numérico para el Mes de Mayo*/
	Junio=0x06,				/**< Valor numérico para el Mes de Junio*/
	Julio=0x07,				/**< Valor numérico para el Mes de Julio*/
	Agosto=0x08,			/**< Valor numérico para el Mes de Agosto*/
	Septiembre=0x09,		/**< Valor numérico para el Mes de Septiembre*/
	Octubre=0x10,			/**< Valor numérico para el Mes de Octubre*/
	Noviembre=0x11,			/**< Valor numérico para el Mes de Noviembre*/
	Diciembre=0x12			/**< Valor numérico para el Mes de Diciembre*/
} tmes;


//----------------------------------------------------------------------------
/** \ingroup com_types
*   \struct tFecha
*	\brief estructura tipo Fecha
*/	
typedef struct{	

	uint8 sc;					/**< Seconds.It can be 0 to 59.*/
	uint8 mn;					/**< Minutes. It can be 0 to 59.*/
	uint8 hr;					/**< Hour. It can be 0 to 12 with AM or PM indicator (H21 bit) 
									 or 0 to 23 (with MIL=1). */
	uint8 dt;					/**< Date. It can be 1 to 31*/
	uint8 mo;					/**< Month. It can be 1 to 12.*/
	uint8 yr;					/**< year. It can be 0 to 99*/
	uint8 dw;					/**< Day of the Week Register. It can be 0 to 6*/
	uint8 y2k;					/**< It can be 19 o 20.*/
}tFecha;

//----------------------------------------------------------------------------
/** \ingroup com_types
*   \struct tcontrol_RTC
*	\brief estructura tipo control_RTC*/	
typedef struct{	

	uint8 BL;					/**< it defines The Watchdog Timer and the Block Protect Bits*/ 
	uint8 INT;					/**< Registro de activación de alarma y configuración de alarma
									 periodica*/
	uint8 ATR;					/**< Analog Trimming Register*/
	uint8 DTR;					/**< Digital Trimming Register*/
	uint8 PWR;					/**< Power Register*/
}tcontrol_RTC;


//----------------------------------------------------------------------------
/** \ingroup com_types
*   \struct tAlarm
	\brief estructura tipo Alarma*/	
typedef struct{	
	uint8 SCA0;					/**< Seconds.It can be 0 to 59.*/
	uint8 MNA0;					/**< Minutes. It can be 0 to 59.*/
	uint8 HRA0;					/**< Hour. It can be 0 to 12 with AM or PM indicator (H21 bit) 
									or 0 to 23 (with MIL=1).*/
	uint8 DTA0;					/**< Date. It can be 1 to 31*/
	uint8 MOA0;					/**< Month. It can be 1 to 12.*/
	uint8 PER;                  /**< */
	uint8 TASK;                 /**<  */
}tAlarm;

//----------------------------------------------------------------------------
/** \ingroup com_types
	\brief define máscaras para los BITS en el registro de Estado (Status Register) del RTC:
*/	
	enum 
	{
		RTCFMask	= 0x01,		/**< Máscara para el Bit RTCF del Status Register del RTC*/
		WELMask		= 0x02,		/**< Máscara para el Bit WEL del Status Register del RTC*/
		RWELMask	= 0x04,		/**< Máscara para el Bit RWEL del Status Register del RTC*/
		OSCFMask	= 0x10,		/**< Máscara para el Bit OSCF del Status Register del RTC*/
		AL0Mask		= 0x20,		/**< Máscara para el Bit AL0 del Status Register del RTC*/
		AL1Mask		= 0x40,		/**< Máscara para el Bit AL1 del Status Register del RTC*/
		BATMask		= 0x80		/**< Máscara para el Bit BAT del Status Register del RTC*/
	};


//----------------------------------------------------------------------------
/** \ingroup com_types
	\brief Define las posibles variables a trazar o monitorear por el PMD*/	
typedef enum {
	PMDTraceNoVariable=0,           /**< No Monitoreo */
	PMDTracePositionError=1,        /**< Muestreo del error de posición, diferencia en entre la posición comandada 
											y la posición actual */
	PMDTraceCommandedPosition=2,    /**< Muestreo de la posición instantánea  comandada*/
	PMDTraceCommandedVelocity=3,    /**< Muestreo de la velocidad instantánea  comandada*/
	PMDTraceCommandedAcceleration=4,/**< Muestreo de la acelaración  instantánea  comandada*/
	PMDTraceActualPosition=5,       /**< Muestreo de la posición actual*/
	PMDTraceActualVelocity=6,       /**< Muestreo de la velocidad actual*/
	PMDTraceCurrentMotorCommand=7,  /**< Muestreo del registro motor command */
	PMDTraceTime=8,                 /**< Muestreo del tiempo del chipset */
	PMDTraceCaptureValue=9,         /**< Devuelve el contenido del registro de captura rápida */
	PMDTraceIntegral=10,            /**< No aplica para MC2400 */
	PMDTraceDerivative=11,          /**< No aplica para MC2400 */
	PMDTraceEventStatus=12,         /**< Muestreo del registro de estatus de los eventos  */
	PMDTraceActivityStatus=13,      /**< Muestreo del registro de actividad */
	PMDTraceSignalStatus=14,        /**< Muestreo del registro de sensores */
	PMDTracePhaseAngle=15,          /**< No aplica para MC2400 */
	PMDTracePhaseOffset=16,         /**< No aplica para MC2400 */
	PMDTracePhaseACommand=17,       /**< No aplica para MC2400 */
	PMDTracePhaseBCommand=18,       /**< No aplica para MC2400 */
	PMDTracePhaseCCommand=19,       /**< No aplica para MC2400 */
	PMDTraceAnalogInput1=20,        /**< Muestreo de la entrada analógica 1*/
	PMDTraceAnalogInput2=21,        /**< Muestreo de la entrada analógica 2*/
	PMDTraceAnalogInput3=22,        /**< Muestreo de la entrada analógica 3*/
	PMDTraceAnalogInput4=23,        /**< Muestreo de la entrada analógica 4*/
	PMDTraceAnalogInput5=24,        /**< Muestreo de la entrada analógica 5*/
	PMDTraceAnalogInput6=25,        /**< Muestreo de la entrada analógica 6*/
	PMDTraceAnalogInput7=26,        /**< Muestreo de la entrada analógica 7*/
	PMDTraceAnalogInput8=27,        /**< Muestreo de la entrada analógica 8*/
	PMDPIDPositionError=28          /**< No aplica para MC2400  */
} PMDTraceVariable;



#endif

/* @} */
