#ifndef __UCTYPES_H__
#define __UCTYPES_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      UCtypes.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\UCtypes.c
//
// Proyecto: UCtypes.h -- C-Motion API
//  
// Propósito General:
// Definición de los tipos de datos utilizados para la Interface microcontrolador-PMD.
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/** \defgroup UCtypes TIPOS DE DATOS DE LA INTERFACE UC-PMD
    \ingroup Interface_uC_PMD
	\brief Definición de los tipos de datos utilizados para la Interface microcontrolador-PMD.
    \code #include <UCtypes.h> \endcode */

/* @{ */

	/** 

	Tipos Enteros Standart. Librería que es recomendada por AVR*/

	#include <stdint.h>
	#include "types.h"                 

	/**  Declaraciones generales  */
	/** \ingroup UCtypes 
	\brief Valor de TRUE=1*/

	#ifndef TRUE
		#define TRUE 1
	#endif
	
	/** \ingroup UCtypes 
	\brief Definición de Valor para FALSE = 0*/
	#ifndef FALSE
		#define FALSE 0
	#endif

	


	
	/** \ingroup UCtypes 
	\brief Valor de la posición de home (después de calibración). */
	#define HOME 0    

	/** \ingroup UCtypes 
	\brief Límite valor absoluto de la posición 2^31-1 */
	#define INF 2147483647
	
	
	/** \ingroup UCtypes 
	\brief Definición de PMDnull*/
	#define PMDnull 			0
	
	/** \ingroup UCtypes 
	\brief Definición para none*/
	#define none 				-15
	
	/** \ingroup UCtypes 
	\brief Definición de update*/
	#define update	 			-16
	
	/** \ingroup UCtypes 
	\brief Definición de no_update*/
	#define no_update 			-17
	
	/** \ingroup UCtypes 
	\brief Definición de imprimir*/
	#define imprimir 			-18
	
	/** \ingroup UCtypes 
	\brief Definición de No_imprimir*/
	#define no_imprimir			-19
	
	
	/** \ingroup UCtypes   */
	#define		PMDint32		int32_t
	#define		PMDuint32		uint32_t
	#define		PMDlong32		int32_t
	#define		PMDint16		int16_t
	#define		PMDuint16		uint16_t
	#define		PMDuint8		uint8_t

	#define		uint8			uint8_t		
	#define		int8			int8_t
	#define		int16			int16_t	
	#define		uint16			uint16_t
	#define		int32			int32_t
	#define		uint32			uint32_t



	typedef float PMDfloat32;			//No se encuentra en la declaración original de PMD

	typedef PMDuint16 PMDAxis;
	typedef PMDuint16 PMDresult;


	/** \ingroup UCtypes 
	\brief Definición de máscaras para datos.*/
	enum 
	{	
		nibbleMask=0x0F,								/**< NibbleMask=0x0F*/ 
		byteMask=0xFF									/**< byteMask=FF*/
	};								

	/** \ingroup UCtypes 
	\brief Modo de comunicación de los Chipset con el HOST (uC)*/
	enum 
	{
		PMDParallelIOMode_16_16=0, 						/**< Modo de Comunicacion con el host*/
		PMDParallelIOMode_8_16,							/**< Este es el modo actual implementado */ 
		PMDParallelIOMode_8_8							/**< Generalmente este modo no se utiliza*/
	};


	/** \ingroup UCtypes 
	\brief Definición de nombres simbólicos  para los Chipset. El Sistema implementado soporta 3 
	Chipset MC2440*/
	typedef enum 
	{
        PMDChipSet1=0,
		PMDChipSet2=1,
		PMDChipSet3=2
	} tChipsetEnum;


	/** \ingroup UCtypes 
	\brief Definición de nombres simbólicos  para los ejes. Cada Chipset de la serie MC2440 soporta cuatro
	ejes o articulaciones (motores)*/
	enum 
	{
        PMDAxis1=0,
        PMDAxis2,
        PMDAxis3,
        PMDAxis4
	};

	/** \ingroup UCtypes 
	\brief Definición de máscaras para los Ejes. Cada Chipset de la serie MC2440 soporta cuatro
	ejes o articulaciones (motores)*/
	typedef enum 
	{
		PMDNoAxisMask=0x00,
        PMDAxis1Mask=0x01,
        PMDAxis2Mask=0x02,
        PMDAxis3Mask=0x04,
        PMDAxis4Mask=0x08
	} PMDAxisMask;


	/** \ingroup UCtypes 
	\brief Definición de nombres simbolicos  para los pilotos de visualizacion (LEDs). El sistema 
	implementado contiene 3 LEDs.*/
	typedef enum 
	{
        Verde=0,
        Rojo=1,
		Amarillo=2
	} tLeds;

enum {
        PMDDisable=0,
        PMDEnable=1
}; // generic enable switch								//definicion de PMD

// Profile Generation

	/** \ingroup UCtypes 
	\brief Definición de nombres simbolicos  para el tipo de perfil de velociadad implementado.*/
	typedef enum {
        PMDTrapezoidalProfile=0,
        PMDVelocityContouringProfile=1,
        PMDSCurveProfile=2,
        PMDElectronicGearProfile=3,
        PMDExternalProfile=4
	} PMDProfileMode;

typedef enum {
        PMDPositionSourceActual=0, 
        PMDPositionSourceCommanded=1
} PMDPositionSource;

typedef enum {
        PMDNoStopMode=0, 
        PMDAbruptStopMode=1, 
        PMDSmoothStopMode=2
} PMDStopMode;


// Servo Filter
typedef enum {
        PMDLimitDisabled=0, 
        PMDLimitEnabled=1
} PMDLimitMode;

typedef enum {
        PMDAutoStopDisabled=0, 
        PMDAutoStopEnabled=1
} PMDAutoStopMode;

typedef enum {
        PMDMotionCompleteCommandedPosition=0, 
        PMDMotionCompleteActualPosition=1
} PMDMotionCompleteMode;

typedef enum {
        PMDBiQuadFilterFilterEnable=0, 
        PMDBiQuadFilterCooefficientB0=1,
        PMDBiQuadFilterCooefficientB1=2,
        PMDBiQuadFilterCooefficientB2=3,
        PMDBiQuadFilterCooefficientA1=4,
        PMDBiQuadFilterCooefficientA2=5,
        PMDBiQuadFilterCooefficientK=6
} PMDBiQuadFilter;


// Parameter Update & Breakpoints
typedef enum {
        PMDBreakpoint1=0,
        PMDBreakpoint2=1
} PMDBreakpoint;

typedef enum {
        PMDBreakpointDisable=0,
        PMDBreakpointGreaterOrEqualCommandedPosition=1,
        PMDBreakpointLessOrEqualCommandedPosition=2,
        PMDBreakpointGreaterOrEqualActualPosition=3,
        PMDBreakpointLessOrEqualActualPosition=4,
        PMDBreakpointCommandedPositionCrossed=5,
        PMDBreakpointActualPositionCrossed=6,
        PMDBreakpointTime=7,
        PMDBreakpointEventStatus=8,
        PMDBreakpointActivityStatus=9,
        PMDBreakpointSignalStatus=10
} PMDBreakpointTrigger;

typedef enum {
        PMDBreakpointNoAction=0,
        PMDBreakpointActionUpdate=1,
        PMDBreakpointActionAbruptStop=2,
        PMDBreakpointActionSmoothStop=3,
        PMDBreakpointActionMotorOff=4
} PMDBreakpointAction;



// Status Register Control
enum {
        PMDActivityPhasingInitializedMask=0x0001,
        PMDActivityAtMaximumVelocityMask=0x0002,
        PMDActivityTrackingMask=0x0004,
        PMDActivityProfileModeMask=0x0038,
        PMDActivityAxisSettledMask=0x0080,
        PMDActivityMotorOnMask=0x0100,
        PMDActivityPositionCaptureMask=0x0200,
        PMDActivityInMotionMask=0x0400,
        PMDActivityInPositiveLimitMask=0x0800,
        PMDActivityInNegativeLimitMask=0x1000,
        PMDActivityProfileSegmentMask=0xE000,
        PMDActivityStatusMask=0x1FBF
};

//Máscaras para la interupccion
enum {
        PMDEventMotionCompleteMask=0x0001,
        PMDEventWrapAroundMask=0x0002,
        PMDEventBreakpoint1Mask=0x0004,
        PMDEventCaptureReceivedMask=0x0008,
        PMDEventMotionErrorMask=0x0010,
        PMDEventInPositiveLimitMask=0x0020,
        PMDEventInNegativeLimitMask=0x0040,
        PMDEventInstructionErrorMask=0x0080,
        PMDEventCommutationErrorMask=0x0800,
        PMDEventBreakpoint2Mask=0x4000,
        PMDEventStatusMask=0x48FF
};

enum {
        PMDSignalEncoderAMask=0x0001,
        PMDSignalEncoderBMask=0x0002,
        PMDSignalEncoderIndexMask=0x0004,
        PMDSignalEncoderHomeMask=0x0008,
        PMDSignalPositiveLimitMask=0x0010,
        PMDSignalNegativeLimitMask=0x0020,
        PMDSignalAxisInMask=0x0040,
        PMDSignalHallAMask=0x0080,
        PMDSignalHallBMask=0x0100,
        PMDSignalHallCMask=0x0200,
        PMDSignalAxisOutMask=0x0400,
        PMDSignalStepOutputInvertMask=0x0800,
        PMDSignalMotorOutputInvertMask=0x1000,
        PMDSignalMask=0x07FF
};

// Encoder
typedef enum {
        PMDCaptureSourceIndex=0,
        PMDCaptureSourceHome=1
} PMDCaptureSource;

typedef enum {
        PMDEncoderSourceIncremental=0,
        PMDEncoderSourceParallel=1
} PMDEncoderSource;

// Motor
typedef enum {
        PMDMotorOutputDAC=0,
        PMDMotorOutputPWMSignMagnitude=1,
        PMDMotorOutputPWM5050Magnitude=2
} PMDMotorOutputMode;

typedef enum {
        PMDMotorOff=0,
        PMDMotorOn=1
} PMDMotorMode;

// Commutation
typedef enum {
        PMDCommutationModeSinusoidal=0,
        PMDCommutationModeHallBased=1,
        PMDCommutationModeMicrostepping=2
} PMDCommutationMode;

typedef enum {
        PMDPhaseInitAlgorithmic=0,
        PMDPhaseInitHallBased=1
} PMDPhaseInitializeMode;

typedef enum {
        PMDPhaseCorrectionDisabled=0,
        PMDPhaseCorrectionEnabled=1
} PMDPhaseCorrectionMode;

typedef enum {
        PMDPhasePrescaleOff=0,
        PMDPhasePrescaleOn=1
} PMDPhasePrescaleMode;

typedef enum {
        PMDPhaseA=0,
        PMDPhaseB=1,
        PMDPhaseC=2
} PMDPhaseNumber;

// Trace Operations
typedef enum {
        PMDTrace1=0,
        PMDTrace2=1,
        PMDTrace3=2,
        PMDTrace4=3
} PMDTraceNumber;

typedef enum {
        PMDTraceOneTime=0,
        PMDTraceRollingBuffer=1
} PMDTraceMode;


typedef enum {
        PMDTraceConditionImmediate=0,
        PMDTraceConditionUpdate=1,
        PMDTraceConditionEventStatus=2,
        PMDTraceConditionActivityStatus=3,
        PMDTraceConditionSignalStatus=4
} PMDTraceCondition;

typedef enum {
        PMDTraceTriggerStateLow=0,
        PMDTraceTriggerStateHigh=1
} PMDTraceTriggerState;

enum {
        PMDTraceStatusMode=0x0001,
        PMDTraceStatusActivity=0x0002,
        PMDTraceStatusDataWrap=0x0004,
        PMDTraceStatusMask=0x07
};

// Miscellaneous
enum {
        PMDActivityPhasingInitializedBit=0,
        PMDActivityAtMaximumVelocityBit=1,
        PMDActivityTrackingBit=2,
        PMDActivityAxisSettledBit=7,
        PMDActivityMotorOnBit=8,
        PMDActivityPositionCaptureBit=9,
        PMDActivityInMotionBit=10,
        PMDActivityInPositiveLimitBit=11,
        PMDActivityInNegitiveLimitBit=12,
};

enum {
        PMDEventMotionCompleteBit=0,
        PMDEventWrapAroundBit=1,
        PMDEventBreakpoint1Bit=2,
        PMDEventCaptureReceivedBit=3,
        PMDEventMotionErrorBit=4,
        PMDEventInPositiveLimitBit=5,
        PMDEventInNegativeLimitBit=6,
        PMDEventInstructionErrorBit=7,
        PMDEventCommutationErrorBit=11,
        PMDEventBreakpoint2Bit=14,
};

enum {
        PMDSignalEncoderABit=0,
        PMDSignalEncoderBBit=1,
        PMDSignalEncoderIndexBit=2,
        PMDSignalEncoderHomeBit=3,
        PMDSignalPositiveLimitBit=4,
        PMDSignalNegativeLimitBit=5,
        PMDSignalAxisInBit=6,
        PMDSignalHallABit=7,
        PMDSignalHallBBit=8,
        PMDSignalHallCBit=9,
        PMDSignalAxisOutBit=10,
        PMDSignalMotorOutputInvertBit=12,
};

typedef enum {
        PMDAxisOutSourceNone=0,
        PMDAxisOutSourceEventStatus=1,
        PMDAxisOutSourceActivityStatus=2,
        PMDAxisOutSourceSignalStatus=3
} PMDAxisOutSource;

typedef enum {
        PMDDiagnosticPortModeLimited=0,
        PMDDiagnosticPortModeFull=1
} PMDDiagnosticPortMode;

typedef enum {
        PMDAxisOff=0,
        PMDAxisOn=1
} PMDAxisMode;

typedef enum {
        PMDSerialBaud1200=0,
        PMDSerialBaud2400=1,
        PMDSerialBaud9600=2,
        PMDSerialBaud19200=3,
        PMDSerialBaud57600=4,
        PMDSerialBaud115200=5,
        PMDSerialBaud250000=6,
        PMDSerialBaud416667=7
} PMDSerialBaud;

typedef enum {
        PMDSerial1StopBit=0,
        PMDSerial2StopBits=1
} PMDSerialStopBits;

typedef enum {
        PMDSerialProtocolPoint2Point=0,
        PMDSerialProtocolMultiDropUsingAddressBit=2,
        PMDSerialProtocolMultiDropUsingIdleLineDetection=3
} PMDSerialProtocol;

typedef enum {
        PMDSerialParityNone=0,
        PMDSerialParityOdd=1,
        PMDSerialParityEven=2
} PMDSerialParity;

typedef enum {
        PMDBrushedServo=1,
        PMDBrushlessServo=3,
        PMDMicroStepping=4,
        PMDStepping=5,
        PMDMixedMotor=8
} PMDMotorTypeVersion;

typedef enum {
        PMDSynchDisabled=0, 
        PMDSynchMasterNode, 
        PMDSynchSlaveNode
} PMDSynch;

typedef enum {
        PMDBufferFunctionNone=-1,
        PMDBufferFunctionPosition=0,
        PMDBufferFunctionVelocity,
        PMDBufferFunctionAcceleration,
        PMDBufferFunctionJerk,
        PMDBufferFunctionTime
} PMDBufferFunction;

typedef enum {
		PMDCounts = 0, 
		PMDSteps = 1 
} PMDPositionUnits;

typedef enum PMDErrorCodesEnum {

	// MC2400 Chipset error codes
	PMD_NOERROR                     = 0,
	PMD_ERR_OK                      = 0,
	PMD_ERR_ChipsetReset=0x01,
	//PMD_ERR_ChipsetReset1=0x10,
	//PMD_ERR_ChipsetReset2=0x11,
	PMD_ERR_InvalidInstruction      = 0x02,
	PMD_ERR_InvalidAxis             = 0x03,
	PMD_ERR_InvalidParameter        = 0x04,
	PMD_ERR_TraceRunning            = 0x05,
	PMD_ERR_Flash                   = 0x06,
	PMD_ERR_BlockOutOfBounds        = 0x07,
	PMD_ERR_TraceBufferZero         = 0x08,
	PMD_ERR_BadSerialChecksum       = 0x09,
	PMD_ERR_NotPrimaryPort          = 0x0A,
	PMD_ERR_InvalidNegativeValue    = 0x0B,
	PMD_ERR_InvalidParameterChange  = 0x0C,
	PMD_ERR_LimitEventPending       = 0x0D,
	PMD_ERR_InvalidMoveIntoLimit    = 0x0E,
	PMD_ERR_InvalidClearInterrupt = 0x0F,
	
	
		// DK2000 initialization errors
	PMD_ERR_InvalidOperation        = 0x7FD0,
	PMD_ERR_NotConnected            = 0x7FD1,
	PMD_ERR_ChipsetNotResponding    = 0x7FD2,
	PMD_ERR_CommPortRead            = 0x7FD3,
	PMD_ERR_CommPortWrite           = 0x7FD4,
	PMD_ERR_InvalidCommPortAddress  = 0x7FD5,
	PMD_ERR_InvalidCommPortBaudRate = 0x7FD6,
	PMD_ERR_InvalidCommPortParity   = 0x7FD7,
	PMD_ERR_InvalidCommPortStopBits = 0x7FD8,
	PMD_ERR_InvalidCommPortProtocol = 0x7FD9,
	PMD_ERR_InvalidCommPortMDropID  = 0x7FDA,
	PMD_ERR_InvalidSerialPort       = 0x7FDB,
	PMD_ERR_InvalidDK2000IOBase     = 0x7FDC,
	PMD_ERR_InvalidDK2000IRQ        = 0x7FDD,
	PMD_ERR_InvalidDK2000IOMode     = 0x7FDE,
	PMD_ERR_PMDNotInitialized		  = 0x7FDF,
	PMD_ERR_OpeningWindowsDriver    = 0x7FE0,

	// run-time errors
	PMD_ERR_Driver                  = 0x7FE1,
	PMD_ERR_CommunicationsError     = 0x7FFC,
	PMD_ERR_CommTimeoutError        = 0x7FFD,
	PMD_ERR_ChecksumError           = 0x7FFE,
	PMD_ERR_CommandError            = 0x7FFF
} PMD_ResultCodes;


//This is the list with operation codes for each command, necessary to built the command in PMD chipset

enum {

	PMDOPNoOperation=					0x00,
	PMDOPSetMotorLimit=					0x06,
	PMDOPGetMotorLimit=					0x07,
	PMDOPSetMotorBias=					0x0F,

	PMDOPSetPosition=					0x10,
	PMDOPSetVelocity=					0x11,
	PMDOPSetJerk=						0x13,
	PMDOPSetGearRatio=					0x14,
	PMDOPUpdate=						0x1A,
	PMDOPGetCommandedPosition=			0x1D,
	PMDOPGetCommandedVelocity=			0x1E,

	PMDOPSetKp=							0x25,
	PMDOPSetKi=							0x26,
	PMDOPSetKd=							0x27,
	PMDOPSetKvff=						0x2B,
	PMDOPGetPhaseAngle=					0x2C,
	PMDOPGetMotorBias=					0x2D,
	PMDOPSetInterruptMask=				0x2F,
	
	PMDOPGetEventStatus=				0x31,
	PMDOPResetEventStatus=				0x34,
	PMDOPGetCaptureValue=				0x36,
	PMDOPGetActualPosition=				0x37,
	PMDOPSetSampleTime=					0x38,
	PMDOPReset=							0x39,
	PMDOPGetCurrentMotorCommand=		0x3A,
	PMDOPGetTime=						0x3E,

	PMDOPClearPositionError=			0x47,
	PMDOPGetPosition=					0x4A,
	PMDOPGetVelocity=					0x4B,
	PMDOPGetAcceleration=				0x4C,
	PMDOPSetActualPosition=				0x4D,

	PMDOPGetKp=							0x50,
	PMDOPGetKi=							0x51,
	PMDOPGetKd=							0x52,
	PMDOPGetKvff=						0x54,
	PMDOPGetInterruptMask=				0x56,
	PMDOPGetJerk=						0x58,
	PMDOPGetGearRatio=					0x59,
	PMDOPMultiUpdate=					0x5B,

	PMDOPGetSampleTime=					0x61,
	PMDOPGetMotorCommand=				0x69,
	PMDOPSetStartVelocity=				0x6A,
	PMDOPGetStartVelocity=				0x6B,
	PMDOPGetOutputMode=					0x6E,

	PMDOPSetPhaseInitializeTime=		0x72,
	PMDOPSetPhaseCounts=				0x75,
	PMDOPSetPhaseOffset=				0x76,
	PMDOPSetMotorCommand=				0x77,
	PMDOPInitializePhase=				0x7A,
	PMDOPGetPhaseOffset=				0x7B,
	PMDOPGetPhaseInitializeTime=		0x7C,
	PMDOPGetPhaseCounts=				0x7D,

	PMDOPSetLimitSwitchMode=			0x80,
	PMDOPGetLimitSwitchMode=			0x81,
	PMDOPWriteIO=						0x82,
	PMDOPReadIO=						0x83,
	PMDOPSetPhaseAngle=					0x84,
	PMDOPSetNumberPhases=				0x85,
	PMDOPGetNumberPhases=				0x86,
	PMDOPSetAxisMode=					0x87,
	PMDOPGetAxisMode=					0x88,
	PMDOPSetDiagnosticPortMode=			0x89,
	PMDOPGetDiagnosticPortMode=			0x8A,
	PMDOPSetSerialPortMode=				0x8B,
	PMDOPGetSerialPortMode=				0x8C,
	PMDOPSetEncoderModulus=				0x8D,
	PMDOPGetEncoderModulus=				0x8E,
	PMDOPGetVersion=					0x8F,
	
	PMDOPSetAcceleration=				0x90,
	PMDOPSetDeceleration=				0x91,
	PMDOPGetDeceleration=				0x92,
	PMDOPSetKaff=						0x93,
	PMDOPGetKaff=						0x94,
	PMDOPSetIntegrationLimit=			0x95,
	PMDOPGetIntegrationLimit=			0x96,
	PMDOPSetPositionErrorLimit=			0x97,
	PMDOPGetPositionErrorLimit=			0x98,
	PMDOPGetPositionError=				0x99,
	PMDOPGetIntegral=					0x9A,
	PMDOPGetDerivative=					0x9B,
	PMDOPSetDerivativeTime=				0x9C,
	PMDOPGetDerivativeTime=				0x9D,
	PMDOPSetKout=						0x9E,
	PMDOPGetKout=						0x9F,

	PMDOPSetProfileMode=				0xA0,
	PMDOPGetProfileMode=				0xA1,
	PMDOPSetSignalSense=				0xA2,
	PMDOPGetSignalSense=				0xA3,
	PMDOPGetSignalStatus=				0xA4,
	PMDOPGetHostIOError=				0xA5,
	PMDOPGetActivityStatus=				0xA6,
	PMDOPGetCommandedAcceleration=		0xA7,
	PMDOPSetTrackingWindow=				0xA8,
	PMDOPGetTrackingWindow=				0xA9,
	PMDOPSetSettleTime=					0xAA,
	PMDOPGetSettleTime=					0xAB,
	PMDOPClearInterrupt=				0xAC,
	PMDOPGetActualVelocity=				0xAD,
	PMDOPSetGearMaster=					0xAE,
	PMDOPGetGearMaster=					0xAF,

	PMDOPSetTraceMode=					0xB0,
	PMDOPGetTraceMode=					0xB1,
	PMDOPSetTraceStart=					0xB2,
	PMDOPGetTraceStart=					0xB3,
	PMDOPSetTraceStop=					0xB4,
	PMDOPGetTraceStop=					0xB5,
	PMDOPSetTraceVariable=				0xB6,
	PMDOPGetTraceVariable=				0xB7,
	PMDOPSetTracePeriod=				0xB8,
	PMDOPGetTracePeriod=				0xB9,
	PMDOPGetTraceStatus=				0xBA,
	PMDOPGetTraceCount=					0xBB,
	PMDOPSetSettleWindow=				0xBC,
	PMDOPGetSettleWindow=				0xBD,
	PMDOPSetActualPositionUnits=		0xBE,
	PMDOPGetActualPositionUnits=		0xBF,

	PMDOPSetBufferStart=				0xC0,
	PMDOPGetBufferStart=				0xC1,
	PMDOPSetBufferLength=				0xC2,
	PMDOPGetBufferLength=				0xC3,
	PMDOPSetBufferWriteIndex=			0xC4,
	PMDOPGetBufferWriteIndex=			0xC5,
	PMDOPSetBufferReadIndex=			0xC6,
	PMDOPGetBufferReadIndex=			0xC7,
	PMDOPWriteBuffer=					0xC8,
	PMDOPReadBuffer=					0xC9,
	PMDOPSetBufferFunction=				0xCA,
	PMDOPGetBufferFunction=				0xCB,
	PMDOPGetStepRange=					0xCE,
	PMDOPSetStepRange=					0xCF,

	PMDOPSetStopMode=					0xD0,
	PMDOPGetStopMode=					0xD1,
	PMDOPSetAutoStopMode=				0xD2,
	PMDOPGetAutoStopMode=				0xD3,
	PMDOPSetBreakpoint=					0xD4,
	PMDOPGetBreakpoint=					0xD5,
	PMDOPSetBreakpointValue=			0xD6,
	PMDOPGetBreakpointValue=			0xD7,
	PMDOPSetCaptureSource=				0xD8,
	PMDOPGetCaptureSource=				0xD9,
	PMDOPSetEncoderSource=				0xDA,
	PMDOPGetEncoderSource=				0xDB,
	PMDOPSetMotorMode=					0xDC,
	PMDOPGetMotorMode=					0xDD,
	PMDOPSetEncoderToStepRatio=			0xDE,
	PMDOPGetEncoderToStepRatio=			0xDF,

	PMDOPSetOutputMode=					0xE0,
	PMDOPGetInterruptAxis=				0xE1,
	PMDOPSetCommutationMode=			0xE2,
	PMDOPGetCommutationMode=			0xE3,
	PMDOPSetPhaseInitializeMode=		0xE4,
	PMDOPGetPhaseInitializeMode=		0xE5,
	PMDOPSetPhasePrescale=				0xE6,
	PMDOPGetPhasePrescale=				0xE7,
	PMDOPSetPhaseCorrectionMode=		0xE8,
	PMDOPGetPhaseCorrectionMode=		0xE9,
	PMDOPGetPhaseCommand=				0xEA,
	PMDOPSetMotionCompleteMode=			0xEB,
	PMDOPGetMotionCompleteMode=			0xEC,
	PMDOPSetAxisOutSource=				0xED,
	PMDOPGetAxisOutSource=				0xEE,
	PMDOPReadAnalog=					0xEF,

	PMDOPSetSynchronizationMode=		0xF2,
	PMDOPGetSynchronizationMode=		0xF3,
	PMDOPAdjustActualPosition=			0xF5,
	PMDOPGetChecksum=					0xF8,

};






//took this definition from original PMDpar.h:      Felipe

typedef struct tagPMDParallelIOTransportData {

	PMDuint16 dcs_Port; // Port to write or read data, commands and status -> here: chipset number
	PMDuint16 readyMask;    // InPStatus bit mask       
	PMDuint16 readyValue;
	PMDuint16 hostInterruptMask;
	PMDuint16 hostInterruptValue;
	PMDuint16 commandStatusMask;
	PMDuint16 commandStatusValue;

	PMDuint16 bVerifyChecksum;
	PMDuint16 bDiagnostics;
	
	void (*OutPData)(PMDuint16 port, PMDuint16 dataword);
	void (*OutPCmd)(PMDuint16 port, PMDuint16 dataword);
	void (*Hard)(void);	//PMDuint16 port
	PMDuint16 (*InPData)(PMDuint16 port);
	PMDuint16 (*InPStatus)(PMDuint16 port);

} PMDParallelIOTransportData;

//took this definition from original PMDtrans.h:      Felipe

typedef struct tagPMDIOTransport {

// these have not yet been defined in any interface classes
//	void (*DisableInterrupt)(void*);
//	void (*EnableInterrupt)(void*);

	PMDresult (*SendCommand)(PMDParallelIOTransportData* transport_data, PMDuint8 xCt, 
	PMDuint16* xDat, PMDuint8 rCt, PMDuint16* rDat);

	void (*Close)(void*);

	PMDuint16 (*GetStatus)(void*);
	PMDuint16 (*IsReady)(void*);
	PMDuint16 (*HasInterrupt)(void*);
	PMDuint16 (*HasError)(void*);
	PMDuint16 (*HardReset)(void*);

} PMDIOTransport;

typedef struct tagPMDAxisHandle {
	PMDuint16 chipset;
	PMDAxis	axis;
        tAxisSettings axisSettings; // holds info about microstepping resolution, encoder and motor resolution, ...
        tAxisLimits axisLimits;     // holds info about the absolute limits of the position range (as found in initialization)
	PMDIOTransport transport;
	void* transport_data;
	//Modificado por David
	//PMDresult result;
} PMDAxisHandle;


 /* @} */

#endif

















