#ifndef PMD_CMotion
#define	PMD_CMotion


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      c-motion.h
// Ubicación fuente: C:\Jarboter_V_2_1\Firmware\Interface_uC_PMD\C\c-motion.c
//
// Proyecto: motion.h -- C-Motion API
//  
// Propósito General:
//Funciones que construyen y encapsula los comandos para el MC2400 (chip PMD)
//	para su ejecución, se apoya en PMDtrans.h, PMDpar.h y basicIO.h s para el envío de estos comandos
//	hacia las tarjetas controladoras de motores.
//	Tiene las siguientes características:
//		- Enfocado al eje (Axis), ya que en todas las funciones está como parámetro de entrada.
//		- Permite la comunicación con varios procesadores de movimiento.
//		- Soporta varios modos de comunicación paralela: 16/16, 8/16 y 8/8 .
//		- Soporta comunicación serial. 
//		- Fácil adaptación para cualquier código C de aplicación de usuario.//	
//	    Más información ver,
//			- Navigator Motion Processor - ISA Developpers Kit Manual, pag 31 .
//			- Navigator Motion Processor - Programmes´s Reference.
//
// Compilado en:
//    avr-gcc 4.3.3 
//
// Author: Performance Motion Devices, Inc.
//
// Version: 1.0 - Refer to "c-motion bug-change list.doc" for file history.
//  
// 
// Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define CMOTION_MAJOR_VERSION	1
#define CMOTION_MINOR_VERSION	5

#if defined(__cplusplus)
extern "C" {
#endif


#include "PMDpar.h"

/** \defgroup CMotion DEFINICIÓN/DECLARACIÓN COMANDOS PMDs
    \ingroup Interface_uC_PMD
	
	\brief  Funciones que construyen y encapsula los comandos para el MC2400 (chip PMD)
	para su ejecución, se apoya en PMDtrans.h, PMDpar.h y basicIO.h s para el envío de estos comandos
	hacia las tarjetas controladoras de motores.
	Tiene las siguientes características:
		- Enfocado al eje (Axis), ya que en todas las funciones está como parámetro de entrada.
		- Permite la comunicación con varios procesadores de movimiento.
		- Soporta varios modos de comunicación paralela: 16/16, 8/16 y 8/8 .
		- Soporta comunicación serial. 
		- Fácil adaptación para cualquier código C de aplicación de usuario.
	
	\see { Navigator Motion Processor - ISA Developpers Kit Manual, pag 31 }
	\see { Navigator Motion Processor - Programmes´s Reference }

	
	
	\code #include <c-motion.h> \endcode
*/

/* @{ */ 





// Profile Generation
extern PMDresult PMDSetProfileMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetProfileMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetPosition(PMDAxisHandle* axis_handle, PMDint32 position);
extern PMDresult PMDGetPosition(PMDAxisHandle* axis_handle, PMDint32* position);
extern PMDresult PMDSetVelocity(PMDAxisHandle* axis_handle, PMDint32 velocity);
extern PMDresult PMDGetVelocity(PMDAxisHandle* axis_handle, PMDint32* velocity);
extern PMDresult PMDSetStartVelocity(PMDAxisHandle* axis_handle, PMDuint32 velocity);
extern PMDresult PMDGetStartVelocity(PMDAxisHandle* axis_handle, PMDuint32* velocity);
extern PMDresult PMDSetAcceleration(PMDAxisHandle* axis_handle, PMDuint32 acceleration);
extern PMDresult PMDGetAcceleration(PMDAxisHandle* axis_handle, PMDuint32* acceleration);
extern PMDresult PMDSetDeceleration(PMDAxisHandle* axis_handle, PMDuint32 deceleration);
extern PMDresult PMDGetDeceleration(PMDAxisHandle* axis_handle, PMDuint32* deceleration);
extern PMDresult PMDSetJerk(PMDAxisHandle* axis_handle, PMDuint32 jerk);
extern PMDresult PMDGetJerk(PMDAxisHandle* axis_handle, PMDuint32* jerk);
extern PMDresult PMDSetGearRatio(PMDAxisHandle* axis_handle, PMDint32 ratio);
extern PMDresult PMDGetGearRatio(PMDAxisHandle* axis_handle, PMDint32* ratio);
extern PMDresult PMDSetGearMaster(PMDAxisHandle* axis_handle, PMDAxis masterAxis, PMDuint8 source);
extern PMDresult PMDGetGearMaster(PMDAxisHandle* axis_handle, PMDAxis* masterAxis, PMDuint8* source);
extern PMDresult PMDSetStopMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetStopMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDGetCommandedPosition(PMDAxisHandle* axis_handle, PMDint32* position);
extern PMDresult PMDGetCommandedVelocity(PMDAxisHandle* axis_handle, PMDint32* velocity);
extern PMDresult PMDGetCommandedAcceleration(PMDAxisHandle* axis_handle, PMDint32* acceleration);


// Servo Filter
extern PMDresult PMDSetKp(PMDAxisHandle* axis_handle, PMDuint16 kp);
extern PMDresult PMDGetKp(PMDAxisHandle* axis_handle, PMDuint16* kp);
extern PMDresult PMDSetKd(PMDAxisHandle* axis_handle, PMDuint16 kd);
extern PMDresult PMDGetKd(PMDAxisHandle* axis_handle, PMDuint16* kd);
extern PMDresult PMDSetKi(PMDAxisHandle* axis_handle, PMDuint16 ki);
extern PMDresult PMDGetKi(PMDAxisHandle* axis_handle, PMDuint16* ki);
extern PMDresult PMDSetKvff(PMDAxisHandle* axis_handle, PMDuint16 kvff);
extern PMDresult PMDGetKvff(PMDAxisHandle* axis_handle, PMDuint16* kvff);
extern PMDresult PMDSetKaff(PMDAxisHandle* axis_handle, PMDuint16 kaff);
extern PMDresult PMDGetKaff(PMDAxisHandle* axis_handle, PMDuint16* kaff);

extern PMDresult PMDSetKout(PMDAxisHandle* axis_handle, PMDuint16 kout);
extern PMDresult PMDGetKout(PMDAxisHandle* axis_handle, PMDuint16* kout);
extern PMDresult PMDSetIntegrationLimit(PMDAxisHandle* axis_handle, PMDuint32 limit);
extern PMDresult PMDGetIntegrationLimit(PMDAxisHandle* axis_handle, PMDuint32* limit);
extern PMDresult PMDSetLimitSwitchMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetLimitSwitchMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetMotorLimit(PMDAxisHandle* axis_handle, PMDuint16 limit);
extern PMDresult PMDGetMotorLimit(PMDAxisHandle* axis_handle, PMDuint16* limit);
extern PMDresult PMDSetMotorBias(PMDAxisHandle* axis_handle, PMDint16 bias);
extern PMDresult PMDGetMotorBias(PMDAxisHandle* axis_handle, PMDint16* bias);

extern PMDresult PMDSetPositionErrorLimit(PMDAxisHandle* axis_handle, PMDuint32 limit);
extern PMDresult PMDGetPositionErrorLimit(PMDAxisHandle* axis_handle, PMDuint32* limit);
extern PMDresult PMDSetAutoStopMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetAutoStopMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetDerivativeTime(PMDAxisHandle* axis_handle, PMDuint16 derivativeTime);
extern PMDresult PMDGetDerivativeTime(PMDAxisHandle* axis_handle, PMDuint16* derivativeTime);
extern PMDresult PMDSetSettleTime(PMDAxisHandle* axis_handle, PMDuint16 settleTime);
extern PMDresult PMDGetSettleTime(PMDAxisHandle* axis_handle, PMDuint16* settleTime);
extern PMDresult PMDSetSettleWindow(PMDAxisHandle* axis_handle, PMDuint16 settleWindow);
extern PMDresult PMDGetSettleWindow(PMDAxisHandle* axis_handle, PMDuint16* settleWindow);

extern PMDresult PMDSetTrackingWindow(PMDAxisHandle* axis_handle, PMDuint16 trackingWindow);
extern PMDresult PMDGetTrackingWindow(PMDAxisHandle* axis_handle, PMDuint16* trackingWindow);
extern PMDresult PMDSetMotionCompleteMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetMotionCompleteMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDClearPositionError(PMDAxisHandle* axis_handle);
extern PMDresult PMDGetDerivative(PMDAxisHandle* axis_handle, PMDint16* derivative);
extern PMDresult PMDGetIntegral(PMDAxisHandle* axis_handle, PMDint32* integral);
extern PMDresult PMDGetPositionError(PMDAxisHandle* axis_handle, PMDint32* positionError);
extern PMDresult PMDSetSampleTime(PMDAxisHandle* axis_handle, PMDuint16 sampleTime);
extern PMDresult PMDGetSampleTime(PMDAxisHandle* axis_handle, PMDuint16* sampleTime);


// Parameter Update & Breakpoints
extern PMDresult PMDSetBreakpoint(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, 
								  PMDAxis breakAxis, PMDuint8 action, PMDuint8 trigger);
extern PMDresult PMDGetBreakpoint(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, 
								  PMDAxis *breakAxis, PMDuint8* action, PMDuint8* trigger);
extern PMDresult PMDSetBreakpointValue(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, 
									   PMDint32 breakpointValue);
extern PMDresult PMDGetBreakpointValue(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, 
									   PMDint32* breakpointValue);
extern PMDresult PMDUpdate(PMDAxisHandle* axis_handle);
extern PMDresult PMDMultiUpdate(PMDAxisHandle* axis_handle, PMDuint16 axisMask);


// Interrupt Processing
extern PMDresult PMDSetInterruptMask(PMDAxisHandle* axis_handle, PMDuint16 interruptMask);
extern PMDresult PMDGetInterruptMask(PMDAxisHandle* axis_handle, PMDuint16* interruptMask);
extern PMDresult PMDClearInterrupt(PMDAxisHandle* axis_handle);
extern PMDresult PMDGetInterruptAxis(PMDAxisHandle* axis_handle, PMDuint16* interruptingAxisMask);


// Status Register Control
extern PMDresult PMDResetEventStatus(PMDAxisHandle* axis_handle, PMDuint16 eventStatus);
extern PMDresult PMDGetEventStatus(PMDAxisHandle* axis_handle, PMDuint16* eventStatus);
extern PMDresult PMDGetActivityStatus(PMDAxisHandle* axis_handle, PMDuint16* activityStatus);
extern PMDresult PMDSetSignalSense(PMDAxisHandle* axis_handle, PMDuint16 mask);
extern PMDresult PMDGetSignalSense(PMDAxisHandle* axis_handle, PMDuint16* mask);
extern PMDresult PMDGetSignalStatus(PMDAxisHandle* axis_handle, PMDuint16* signalStatus);


// Encoder
extern PMDresult PMDAdjustActualPosition(PMDAxisHandle* axis_handle, PMDint32 position);
extern PMDresult PMDSetActualPosition(PMDAxisHandle* axis_handle, PMDint32 position);
extern PMDresult PMDGetActualPosition(PMDAxisHandle* axis_handle, PMDint32* position);
extern PMDresult PMDSetActualPositionUnits(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetActualPositionUnits(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetCaptureSource(PMDAxisHandle* axis_handle, PMDuint16 captureSource);
extern PMDresult PMDGetCaptureSource(PMDAxisHandle* axis_handle, PMDuint16* captureSource);
extern PMDresult PMDSetEncoderSource(PMDAxisHandle* axis_handle, PMDuint16 encoderSource);
extern PMDresult PMDGetEncoderSource(PMDAxisHandle* axis_handle, PMDuint16* encoderSource);
extern PMDresult PMDSetEncoderModulus(PMDAxisHandle* axis_handle, PMDuint16 modulus);
extern PMDresult PMDGetEncoderModulus(PMDAxisHandle* axis_handle, PMDuint16* modulus);
extern PMDresult PMDSetEncoderToStepRatio(PMDAxisHandle* axis_handle, PMDuint16 encoderCounts, PMDuint16 steps);
extern PMDresult PMDGetEncoderToStepRatio(PMDAxisHandle* axis_handle, PMDuint16* encoderCounts, PMDuint16* steps);
extern PMDresult PMDGetActualVelocity(PMDAxisHandle* axis_handle, PMDint32* actualValue);
extern PMDresult PMDGetCaptureValue(PMDAxisHandle* axis_handle, PMDint32* captureValue);


// Motor
extern PMDresult PMDSetOutputMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetOutputMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetMotorMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetMotorMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetMotorCommand(PMDAxisHandle* axis_handle, PMDint16 motorCommand);
extern PMDresult PMDGetMotorCommand(PMDAxisHandle* axis_handle, PMDint16* motorCommand);
extern PMDresult PMDGetCurrentMotorCommand(PMDAxisHandle* axis_handle, PMDint16* motorCommand);


// Commutation
extern PMDresult PMDSetCommutationMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetCommutationMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetNumberPhases(PMDAxisHandle* axis_handle, PMDuint16 numberPhases);
extern PMDresult PMDGetNumberPhases(PMDAxisHandle* axis_handle, PMDuint16* numberPhases);
extern PMDresult PMDSetPhaseInitializeMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetPhaseInitializeMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetPhasePrescale(PMDAxisHandle* axis_handle, PMDuint16 phasePrescale);
extern PMDresult PMDGetPhasePrescale(PMDAxisHandle* axis_handle, PMDuint16* phasePrescale);
extern PMDresult PMDSetPhaseCounts(PMDAxisHandle* axis_handle, PMDuint16 phaseCounts);
extern PMDresult PMDGetPhaseCounts(PMDAxisHandle* axis_handle, PMDuint16* phaseCounts);
extern PMDresult PMDSetPhaseInitializeTime(PMDAxisHandle* axis_handle, PMDuint16 phaseInitTime);
extern PMDresult PMDGetPhaseInitializeTime(PMDAxisHandle* axis_handle, PMDuint16* phaseInitTime);
extern PMDresult PMDSetPhaseOffset(PMDAxisHandle* axis_handle, PMDuint16 phaseOffset);
extern PMDresult PMDGetPhaseOffset(PMDAxisHandle* axis_handle, PMDuint16* phaseOffset);
extern PMDresult PMDSetPhaseAngle(PMDAxisHandle* axis_handle, PMDuint16 phaseAngle);
extern PMDresult PMDGetPhaseAngle(PMDAxisHandle* axis_handle, PMDuint16* phaseAngle);
extern PMDresult PMDSetPhaseCorrectionMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetPhaseCorrectionMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDInitializePhase(PMDAxisHandle* axis_handle);
extern PMDresult PMDGetPhaseCommand(PMDAxisHandle* axis_handle, PMDuint16 phase, PMDint16* command);


// External Memory
extern PMDresult PMDSetBufferStart(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 bufferStart);
extern PMDresult PMDGetBufferStart(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* bufferStart);
extern PMDresult PMDSetBufferLength(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 bufferLength);
extern PMDresult PMDGetBufferLength(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* bufferLength);
extern PMDresult PMDSetBufferFunction(PMDAxisHandle* axis_handle, PMDuint16 function, PMDint16 bufferID);
extern PMDresult PMDGetBufferFunction(PMDAxisHandle* axis_handle, PMDuint16 function, PMDint16* bufferID);
extern PMDresult PMDWriteBuffer(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDint32 data);
extern PMDresult PMDReadBuffer(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDint32* data);
extern PMDresult PMDSetBufferWriteIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 writeIndex);
extern PMDresult PMDGetBufferWriteIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* writeIndex);
extern PMDresult PMDSetBufferReadIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 readIndex);
extern PMDresult PMDGetBufferReadIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* readIndex);


// Trace Operations
extern PMDresult PMDSetTraceMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetTraceMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetTracePeriod(PMDAxisHandle* axis_handle, PMDuint16 tracePeriod);
extern PMDresult PMDGetTracePeriod(PMDAxisHandle* axis_handle, PMDuint16 *tracePeriod);
extern PMDresult PMDSetTraceVariable(PMDAxisHandle* axis_handle, PMDuint16 traceID, PMDAxis traceAxis, PMDuint8 variable);
extern PMDresult PMDGetTraceVariable(PMDAxisHandle* axis_handle, PMDuint16 traceID, PMDAxis *traceAxis, PMDuint8* variable);
extern PMDresult PMDSetTraceStart(PMDAxisHandle* axis_handle, PMDAxis traceAxis, PMDuint8 condition, PMDuint8 bit, PMDuint8 state);
extern PMDresult PMDGetTraceStart(PMDAxisHandle* axis_handle, PMDAxis *traceAxis, PMDuint8* condition, PMDuint8* bit, PMDuint8* state);
extern PMDresult PMDSetTraceStop(PMDAxisHandle* axis_handle, PMDAxis traceAxis, PMDuint8 condition, PMDuint8 bit, PMDuint8 state);
extern PMDresult PMDGetTraceStop(PMDAxisHandle* axis_handle, PMDAxis *traceAxis, PMDuint8* condition, PMDuint8* bit, PMDuint8* state);
extern PMDresult PMDGetTraceStatus(PMDAxisHandle* axis_handle, PMDuint16* status);
extern PMDresult PMDGetTraceCount(PMDAxisHandle* axis_handle, PMDuint32* count);


// Miscellaneous
extern PMDresult PMDSetAxisMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetAxisMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDSetAxisOutSource(PMDAxisHandle* axis_handle, PMDAxis outAxis, PMDuint8 bit, PMDuint8 sourceRegister);
extern PMDresult PMDGetAxisOutSource(PMDAxisHandle* axis_handle, PMDAxis *outAxis, PMDuint8 *bit, PMDuint8* sourceRegister);
extern PMDresult PMDWriteIO(PMDAxisHandle* axis_handle, PMDuint16 address, PMDuint16 data);
extern PMDresult PMDReadIO(PMDAxisHandle* axis_handle, PMDuint16 address, PMDuint16 *data);
extern PMDresult PMDReadAnalog(PMDAxisHandle* axis_handle, PMDuint16 analogID, PMDuint16 *data);
extern PMDresult PMDReset(PMDAxisHandle* axis_handle);
extern PMDresult PMDNoOperation(PMDAxisHandle* axis_handle);
extern PMDresult PMDGetVersion(PMDAxisHandle* axis_handle, PMDuint16 *generation, PMDuint16 *motorType, 
						PMDuint16 *numberAxes, PMDuint16 *special,
						PMDuint16 *custom, PMDuint16 *major, PMDuint16 *minor);
extern PMDresult PMDGetHostIOError(PMDAxisHandle* axis_handle, PMDuint16 *hostIOError);

extern PMDresult PMDSetSerialPortMode(PMDAxisHandle* axis_handle, PMDuint8 baud, 
					PMDuint8 parity, PMDuint8 stopBits, 
					PMDuint8 protocol, PMDuint8 multiDropID);
extern PMDresult PMDGetSerialPortMode(PMDAxisHandle* axis_handle, PMDuint8* baud, 
					PMDuint8* parity, PMDuint8* stopBits, 
					PMDuint8* protocol, PMDuint8* multiDropID);

extern PMDresult PMDSetDiagnosticPortMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetDiagnosticPortMode(PMDAxisHandle* axis_handle, PMDuint16* mode);
extern PMDresult PMDGetTime(PMDAxisHandle* axis_handle, PMDuint32 *numberServoCycles);

extern PMDresult PMDGetChecksum(PMDAxisHandle* axis_handle, PMDuint32* checksum);

extern PMDresult PMDSetStepRange(PMDAxisHandle* axis_handle, PMDuint16 range);
extern PMDresult PMDGetStepRange(PMDAxisHandle* axis_handle, PMDuint16* range);
extern PMDresult PMDSetSynchronizationMode(PMDAxisHandle* axis_handle, PMDuint16 mode);
extern PMDresult PMDGetSynchronizationMode(PMDAxisHandle* axis_handle, PMDuint16* mode);


// C-Motion provided functions
extern PMDresult PMDGetStatus(PMDAxisHandle* axis_handle);
extern PMDresult PMDHasError(PMDAxisHandle* axis_handle);
extern PMDresult PMDIsReady(PMDAxisHandle* axis_handle);
extern PMDresult PMDHasInterrupt(PMDAxisHandle* axis_handle);
extern PMDresult PMDHardReset(PMDAxisHandle* axis_handle);
extern void PMDCloseAxisInterface(PMDAxisHandle* axis_handle);

extern void PMDGetCMotionVersion(PMDuint8* MajorVersion, PMDuint8* MinorVersion);

/* @} */

#if defined(__cplusplus)
}
#endif

#endif
