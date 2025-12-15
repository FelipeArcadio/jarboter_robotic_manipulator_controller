//  c-motion.c -- C-Motion API implementation
//
//  Performance Motion Devices, Inc.
//

#include <avr/io.h>
#include "c-motion.h"



// Profile Generation
PMDresult PMDSetProfileMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetProfileMode, mode);
}

PMDresult PMDGetProfileMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetProfileMode, mode);
}

PMDresult PMDSetPosition(PMDAxisHandle* axis_handle, PMDint32 position)
{
	return SendCommandLong(axis_handle, PMDOPSetPosition, position);
}

PMDresult PMDGetPosition(PMDAxisHandle* axis_handle, PMDint32* position)
{
	return SendCommandGetLong(axis_handle, PMDOPGetPosition, (PMDuint32*)position);
}

PMDresult PMDSetVelocity(PMDAxisHandle* axis_handle, PMDint32 velocity)
{
	return SendCommandLong(axis_handle, PMDOPSetVelocity, velocity);
}

PMDresult PMDGetVelocity(PMDAxisHandle* axis_handle, PMDint32* velocity)
{
	return SendCommandGetLong(axis_handle, PMDOPGetVelocity, (PMDuint32*)velocity);
}

PMDresult PMDSetStartVelocity(PMDAxisHandle* axis_handle, PMDuint32 velocity)
{
	return SendCommandLong(axis_handle, PMDOPSetStartVelocity, velocity);
}

PMDresult PMDGetStartVelocity(PMDAxisHandle* axis_handle, PMDuint32* velocity)
{
	return SendCommandGetLong(axis_handle, PMDOPGetStartVelocity, velocity);
}

PMDresult PMDSetAcceleration(PMDAxisHandle* axis_handle, PMDuint32 acceleration)
{
	return SendCommandLong(axis_handle, PMDOPSetAcceleration, acceleration);
}

PMDresult PMDGetAcceleration(PMDAxisHandle* axis_handle, PMDuint32* acceleration)
{
	return SendCommandGetLong(axis_handle, PMDOPGetAcceleration, acceleration);
}

PMDresult PMDSetDeceleration(PMDAxisHandle* axis_handle, PMDuint32 deceleration)
{
	return SendCommandLong(axis_handle, PMDOPSetDeceleration, deceleration);
}

PMDresult PMDGetDeceleration(PMDAxisHandle* axis_handle, PMDuint32* deceleration)
{
	return SendCommandGetLong(axis_handle, PMDOPGetDeceleration, deceleration);
}

PMDresult PMDSetJerk(PMDAxisHandle* axis_handle, PMDuint32 jerk)
{
	return SendCommandLong(axis_handle, PMDOPSetJerk, jerk);
}

PMDresult PMDGetJerk(PMDAxisHandle* axis_handle, PMDuint32* jerk)
{
	return SendCommandGetLong(axis_handle, PMDOPGetJerk, jerk);
}

PMDresult PMDSetGearRatio(PMDAxisHandle* axis_handle, PMDint32 ratio)
{
	return SendCommandLong(axis_handle, PMDOPSetGearRatio, (PMDuint32)ratio);
}

PMDresult PMDGetGearRatio(PMDAxisHandle* axis_handle, PMDint32* ratio)
{
	return SendCommandGetLong(axis_handle, PMDOPGetGearRatio, (PMDuint32*)ratio);
}

PMDresult PMDSetGearMaster(PMDAxisHandle* axis_handle, PMDAxis masterAxis, PMDuint8 source)
{
	PMDuint16 value = (PMDuint16)((masterAxis & nibbleMask) | ((source & nibbleMask) << 8));
	return SendCommandWord(axis_handle, PMDOPSetGearMaster, value);
}

PMDresult PMDGetGearMaster(PMDAxisHandle* axis_handle, PMDAxis* masterAxis, PMDuint8* source)
{
	PMDuint16 result;
	PMDuint16 value;

	result = SendCommandGetWord(axis_handle, PMDOPGetGearMaster, &value);
	*masterAxis = (PMDAxis)(value & nibbleMask);
	*source = (PMDuint8)((value >> 8) & nibbleMask);

	return result;
}

PMDresult PMDSetStopMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetStopMode, mode);
}

PMDresult PMDGetStopMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetStopMode, mode);
}

PMDresult PMDGetCommandedPosition(PMDAxisHandle* axis_handle, PMDint32* position)
{
	return SendCommandGetLong(axis_handle, PMDOPGetCommandedPosition, (PMDuint32*)position);
}

PMDresult PMDGetCommandedVelocity(PMDAxisHandle* axis_handle, PMDint32* velocity)
{
	return SendCommandGetLong(axis_handle, PMDOPGetCommandedVelocity, (PMDuint32*)velocity);
}

PMDresult PMDGetCommandedAcceleration(PMDAxisHandle* axis_handle, PMDint32 *acceleration)
{
	return SendCommandGetLong(axis_handle, PMDOPGetCommandedAcceleration, (PMDuint32*)acceleration);
}


// Servo Filter
PMDresult PMDSetKp(PMDAxisHandle* axis_handle, PMDuint16 kp)
{
	return SendCommandWord(axis_handle, PMDOPSetKp, kp);
}

PMDresult PMDGetKp(PMDAxisHandle* axis_handle, PMDuint16* kp)
{
	return SendCommandGetWord(axis_handle, PMDOPGetKp, kp);
}

PMDresult PMDSetKd(PMDAxisHandle* axis_handle, PMDuint16 kd)
{
	return SendCommandWord(axis_handle, PMDOPSetKd, kd);
}

PMDresult PMDGetKd(PMDAxisHandle* axis_handle, PMDuint16* kd)
{
	return SendCommandGetWord(axis_handle, PMDOPGetKd, kd);
}

PMDresult PMDSetKi(PMDAxisHandle* axis_handle, PMDuint16 ki)
{
	return SendCommandWord(axis_handle, PMDOPSetKi, ki);
}

PMDresult PMDGetKi(PMDAxisHandle* axis_handle, PMDuint16* ki)
{
	return SendCommandGetWord(axis_handle, PMDOPGetKi, ki);
}

PMDresult PMDSetKvff(PMDAxisHandle* axis_handle, PMDuint16 kvff)
{
	return SendCommandWord(axis_handle, PMDOPSetKvff, kvff);
}

PMDresult PMDGetKvff(PMDAxisHandle* axis_handle, PMDuint16* kvff)
{
	return SendCommandGetWord(axis_handle, PMDOPGetKvff, kvff);
}

PMDresult PMDSetKaff(PMDAxisHandle* axis_handle, PMDuint16 kaff)
{
	return SendCommandWord(axis_handle, PMDOPSetKaff, kaff);
}

PMDresult PMDGetKaff(PMDAxisHandle* axis_handle, PMDuint16* kaff)
{
	return SendCommandGetWord(axis_handle, PMDOPGetKaff, kaff);
}

PMDresult PMDSetKout(PMDAxisHandle* axis_handle, PMDuint16 kout)
{
	return SendCommandWord(axis_handle, PMDOPSetKout, kout);
}

PMDresult PMDGetKout(PMDAxisHandle* axis_handle, PMDuint16* kout)
{
	return SendCommandGetWord(axis_handle, PMDOPGetKout, kout);
}

PMDresult PMDSetIntegrationLimit(PMDAxisHandle* axis_handle, PMDuint32 limit)
{
	return SendCommandLong(axis_handle, PMDOPSetIntegrationLimit, limit);
}

PMDresult PMDGetIntegrationLimit(PMDAxisHandle* axis_handle, PMDuint32* limit)
{
	return SendCommandGetLong(axis_handle, PMDOPGetIntegrationLimit, limit);
}

PMDresult PMDSetLimitSwitchMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetLimitSwitchMode, mode);
}

PMDresult PMDGetLimitSwitchMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetLimitSwitchMode, mode);
}

PMDresult PMDSetMotorLimit(PMDAxisHandle* axis_handle, PMDuint16 limit)
{
	return SendCommandWord(axis_handle, PMDOPSetMotorLimit, limit);
}

PMDresult PMDGetMotorLimit(PMDAxisHandle* axis_handle, PMDuint16* limit)
{
	return SendCommandGetWord(axis_handle, PMDOPGetMotorLimit, limit);
}

PMDresult PMDSetMotorBias(PMDAxisHandle* axis_handle, PMDint16 bias)
{
	return SendCommandWord(axis_handle, PMDOPSetMotorBias, (PMDuint16)bias);
}

PMDresult PMDGetMotorBias(PMDAxisHandle* axis_handle, PMDint16* bias)
{
	return SendCommandGetWord(axis_handle, PMDOPGetMotorBias, (PMDuint16*)bias);
}

PMDresult PMDSetPositionErrorLimit(PMDAxisHandle* axis_handle, PMDuint32 limit)
{
	return SendCommandLong(axis_handle, PMDOPSetPositionErrorLimit, limit);
}

PMDresult PMDGetPositionErrorLimit(PMDAxisHandle* axis_handle, PMDuint32* limit)
{
	return SendCommandGetLong(axis_handle, PMDOPGetPositionErrorLimit, limit);
}

PMDresult PMDSetAutoStopMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetAutoStopMode, mode);
}

PMDresult PMDGetAutoStopMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetAutoStopMode, mode);
}

PMDresult PMDSetDerivativeTime(PMDAxisHandle* axis_handle, PMDuint16 derivativeTime)
{
	return SendCommandWord(axis_handle, PMDOPSetDerivativeTime, derivativeTime);
}

PMDresult PMDGetDerivativeTime(PMDAxisHandle* axis_handle, PMDuint16* derivativeTime)
{
	return SendCommandGetWord(axis_handle, PMDOPGetDerivativeTime, derivativeTime);
}

PMDresult PMDSetSettleTime(PMDAxisHandle* axis_handle, PMDuint16 settleTime)
{
	return SendCommandWord(axis_handle, PMDOPSetSettleTime, settleTime);
}

PMDresult PMDGetSettleTime(PMDAxisHandle* axis_handle, PMDuint16* settleTime)
{
	return SendCommandGetWord(axis_handle, PMDOPGetSettleTime, settleTime);
}

PMDresult PMDSetSettleWindow(PMDAxisHandle* axis_handle, PMDuint16 settleWindow)
{
	return SendCommandWord(axis_handle, PMDOPSetSettleWindow, settleWindow);
}

PMDresult PMDGetSettleWindow(PMDAxisHandle* axis_handle, PMDuint16* settleWindow)
{
	return SendCommandGetWord(axis_handle, PMDOPGetSettleWindow, settleWindow);
}

PMDresult PMDSetTrackingWindow(PMDAxisHandle* axis_handle, PMDuint16 trackingWindow)
{
	return SendCommandWord(axis_handle, PMDOPSetTrackingWindow, trackingWindow);
}

PMDresult PMDGetTrackingWindow(PMDAxisHandle* axis_handle, PMDuint16* trackingWindow)
{
	return SendCommandGetWord(axis_handle, PMDOPGetTrackingWindow, trackingWindow);
}

PMDresult PMDSetMotionCompleteMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetMotionCompleteMode, mode);
}

PMDresult PMDGetMotionCompleteMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetMotionCompleteMode, mode);
}

PMDresult PMDClearPositionError(PMDAxisHandle* axis_handle)
{
	return SendCommand(axis_handle, PMDOPClearPositionError);
}

PMDresult PMDGetDerivative(PMDAxisHandle* axis_handle, PMDint16* derivative)
{
	return SendCommandGetWord(axis_handle, PMDOPGetDerivative, (PMDuint16*)derivative);
}

PMDresult PMDGetIntegral(PMDAxisHandle* axis_handle, PMDint32* integral)
{
	return SendCommandGetLong(axis_handle, PMDOPGetIntegral, (PMDuint32*)integral);
}

PMDresult PMDGetPositionError(PMDAxisHandle* axis_handle, PMDint32* error)
{
	return SendCommandGetLong(axis_handle, PMDOPGetPositionError, (PMDuint32*)error);
}

PMDresult PMDSetSampleTime(PMDAxisHandle* axis_handle, PMDuint16 sampleTime)
{
	return SendCommandWord(axis_handle, PMDOPSetSampleTime, sampleTime);
}

PMDresult PMDGetSampleTime(PMDAxisHandle* axis_handle, PMDuint16* sampleTime)
{
	return SendCommandGetWord(axis_handle, PMDOPGetSampleTime, sampleTime);
}


// Parameter Update & Breakpoints
PMDresult PMDSetBreakpoint(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, 
						   PMDAxis breakAxis, PMDuint8 action, PMDuint8 trigger)
{
	PMDuint16 value = (PMDuint16)((breakAxis & nibbleMask) | 
		((action & nibbleMask) << 4) | ((trigger & byteMask) << 8));
	return SendCommandWordWord(axis_handle, PMDOPSetBreakpoint, breakpointID, value);
}

PMDresult PMDGetBreakpoint(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, 
						   PMDAxis* breakAxis, PMDuint8* action, PMDuint8* trigger)
{
	PMDuint16 result;
	PMDuint16 value;

	result = SendCommandWordGetWord(axis_handle, PMDOPGetBreakpoint, breakpointID, &value);

	*breakAxis = (PMDAxis)(value & nibbleMask);
	*action = (PMDuint8)((value >> 4) & nibbleMask);
	*trigger = (PMDuint8)((value >> 8) & byteMask);

	return result;
}

PMDresult PMDSetBreakpointValue(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, PMDint32 breakpointValue)
{
	return SendCommandWordLong(axis_handle, PMDOPSetBreakpointValue, breakpointID, (PMDuint32)breakpointValue);
}

PMDresult PMDGetBreakpointValue(PMDAxisHandle* axis_handle, PMDuint16 breakpointID, PMDint32* breakpointValue)
{
	return SendCommandWordGetLong(axis_handle, PMDOPGetBreakpointValue, breakpointID, (PMDuint32*)breakpointValue);
}

PMDresult PMDUpdate(PMDAxisHandle* axis_handle)
{
	return SendCommand(axis_handle, PMDOPUpdate);
}

PMDresult PMDMultiUpdate(PMDAxisHandle* axis_handle, PMDuint16 axisMask)
{
	return SendCommandWord(axis_handle, PMDOPMultiUpdate, axisMask);
}


// Interrupt Processing
PMDresult PMDSetInterruptMask(PMDAxisHandle* axis_handle, PMDuint16 interruptMask)
{
	return SendCommandWord(axis_handle, PMDOPSetInterruptMask, interruptMask);
}

PMDresult PMDGetInterruptMask(PMDAxisHandle* axis_handle, PMDuint16* interruptMask)
{
	return SendCommandGetWord(axis_handle, PMDOPGetInterruptMask, interruptMask);
}

PMDresult PMDClearInterrupt(PMDAxisHandle* axis_handle)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// for the ClearInterrupt command, the axis number MUST be zero
	axis_handle->axis = 0;

	result = SendCommand(axis_handle, PMDOPClearInterrupt);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetInterruptAxis(PMDAxisHandle* axis_handle, PMDuint16* interruptingAxisMask)
{
	return SendCommandGetWord(axis_handle, PMDOPGetInterruptAxis, interruptingAxisMask);
}


// Status Register Control
PMDresult PMDResetEventStatus(PMDAxisHandle* axis_handle, PMDuint16 eventStatus)
{
	return SendCommandWord(axis_handle, PMDOPResetEventStatus, eventStatus);
}

PMDresult PMDGetEventStatus(PMDAxisHandle* axis_handle, PMDuint16* eventStatus)
{
	return SendCommandGetWord(axis_handle, PMDOPGetEventStatus, eventStatus);
}

PMDresult PMDGetActivityStatus(PMDAxisHandle* axis_handle, PMDuint16* activityStatus)
{
	return SendCommandGetWord(axis_handle, PMDOPGetActivityStatus, activityStatus);
}

PMDresult PMDSetSignalSense(PMDAxisHandle* axis_handle, PMDuint16 mask)
{
	return SendCommandWord(axis_handle, PMDOPSetSignalSense, mask);
}

PMDresult PMDGetSignalSense(PMDAxisHandle* axis_handle, PMDuint16* mask)
{
	return SendCommandGetWord(axis_handle, PMDOPGetSignalSense, mask);
}

PMDresult PMDGetSignalStatus(PMDAxisHandle* axis_handle, PMDuint16* signalStatus)
{
	return SendCommandGetWord(axis_handle, PMDOPGetSignalStatus, signalStatus);
}


// Encoder
PMDresult PMDAdjustActualPosition(PMDAxisHandle* axis_handle, PMDint32 position)
{
	return SendCommandLong(axis_handle, PMDOPAdjustActualPosition, (PMDuint32)position);
}

PMDresult PMDSetActualPosition(PMDAxisHandle* axis_handle, PMDint32 position)
{
	return SendCommandLong(axis_handle, PMDOPSetActualPosition, (PMDuint32)position);
}

PMDresult PMDGetActualPosition(PMDAxisHandle* axis_handle, PMDint32* position)
{
	return SendCommandGetLong(axis_handle, PMDOPGetActualPosition, (PMDuint32*)position);
}

PMDresult PMDSetActualPositionUnits(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetActualPositionUnits, mode);
}

PMDresult PMDGetActualPositionUnits(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetActualPositionUnits, mode);
}

PMDresult PMDSetCaptureSource(PMDAxisHandle* axis_handle, PMDuint16 captureSource)
{
	return SendCommandWord(axis_handle, PMDOPSetCaptureSource, captureSource);
}

PMDresult PMDGetCaptureSource(PMDAxisHandle* axis_handle, PMDuint16* captureSource)
{
	return SendCommandGetWord(axis_handle, PMDOPGetCaptureSource, captureSource);
}

PMDresult PMDSetEncoderSource(PMDAxisHandle* axis_handle, PMDuint16 encoderSource)
{
	return SendCommandWord(axis_handle, PMDOPSetEncoderSource, encoderSource);
}

PMDresult PMDGetEncoderSource(PMDAxisHandle* axis_handle, PMDuint16* encoderSource)
{
	return SendCommandGetWord(axis_handle, PMDOPGetEncoderSource, encoderSource);
}

PMDresult PMDSetEncoderModulus(PMDAxisHandle* axis_handle, PMDuint16 modulus)
{
	return SendCommandWord(axis_handle, PMDOPSetEncoderModulus, modulus);
}

PMDresult PMDGetEncoderModulus(PMDAxisHandle* axis_handle, PMDuint16* modulus)
{
	return SendCommandGetWord(axis_handle, PMDOPGetEncoderModulus, modulus);
}

PMDresult PMDSetEncoderToStepRatio(PMDAxisHandle* axis_handle, PMDuint16 encoderCounts, PMDuint16 steps)
{
	return SendCommandWordWord(axis_handle, PMDOPSetEncoderToStepRatio, encoderCounts, steps);
}

PMDresult PMDGetEncoderToStepRatio(PMDAxisHandle* axis_handle, PMDuint16* encoderCounts, PMDuint16* steps)
{
	return SendCommandGetWordWord(axis_handle, PMDOPGetEncoderToStepRatio, encoderCounts, steps);
}

PMDresult PMDGetActualVelocity(PMDAxisHandle* axis_handle, PMDint32* actualValue)
{
	return SendCommandGetLong(axis_handle, PMDOPGetActualVelocity, (PMDuint32*)actualValue);
}

PMDresult PMDGetCaptureValue(PMDAxisHandle* axis_handle, PMDint32* captureValue)
{
	return SendCommandGetLong(axis_handle, PMDOPGetCaptureValue, (PMDuint32*)captureValue);
}


// Motor
PMDresult PMDSetOutputMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetOutputMode, mode);
}

PMDresult PMDGetOutputMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetOutputMode, mode);
}

PMDresult PMDSetMotorMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetMotorMode, mode);
}

PMDresult PMDGetMotorMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetMotorMode, mode);
}

PMDresult PMDSetMotorCommand(PMDAxisHandle* axis_handle, PMDint16 motorCommand)
{
	return SendCommandWord(axis_handle, PMDOPSetMotorCommand, (PMDuint16)motorCommand);
}

PMDresult PMDGetMotorCommand(PMDAxisHandle* axis_handle, PMDint16* motorCommand)
{
	return SendCommandGetWord(axis_handle, PMDOPGetMotorCommand, (PMDuint16*)motorCommand);
}

PMDresult PMDGetCurrentMotorCommand(PMDAxisHandle* axis_handle, PMDint16* motorCommand)
{
	return SendCommandGetWord(axis_handle, PMDOPGetCurrentMotorCommand, (PMDuint16*)motorCommand);
}


// Commutation
PMDresult PMDSetCommutationMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetCommutationMode, mode);
}

PMDresult PMDGetCommutationMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetCommutationMode, mode);
}

PMDresult PMDSetNumberPhases(PMDAxisHandle* axis_handle, PMDuint16 numberPhases)
{
	return SendCommandWord(axis_handle, PMDOPSetNumberPhases, numberPhases);
}

PMDresult PMDGetNumberPhases(PMDAxisHandle* axis_handle, PMDuint16* numberPhases)
{
	return SendCommandGetWord(axis_handle, PMDOPGetNumberPhases, numberPhases);
}

PMDresult PMDSetPhaseInitializeMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetPhaseInitializeMode, mode);
}

PMDresult PMDGetPhaseInitializeMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhaseInitializeMode, mode);
}

PMDresult PMDSetPhasePrescale(PMDAxisHandle* axis_handle, PMDuint16 phasePrescale)
{
	return SendCommandWord(axis_handle, PMDOPSetPhasePrescale, phasePrescale);
}

PMDresult PMDGetPhasePrescale(PMDAxisHandle* axis_handle, PMDuint16* phasePrescale)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhasePrescale, phasePrescale);
}

PMDresult PMDSetPhaseCounts(PMDAxisHandle* axis_handle, PMDuint16 phaseCounts)
{
	return SendCommandWord(axis_handle, PMDOPSetPhaseCounts, phaseCounts);
}

PMDresult PMDGetPhaseCounts(PMDAxisHandle* axis_handle, PMDuint16* phaseCounts)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhaseCounts, phaseCounts);
}

PMDresult PMDSetPhaseInitializeTime(PMDAxisHandle* axis_handle, PMDuint16 phaseInitTime)
{
	return SendCommandWord(axis_handle, PMDOPSetPhaseInitializeTime, phaseInitTime);
}

PMDresult PMDGetPhaseInitializeTime(PMDAxisHandle* axis_handle, PMDuint16* phaseInitTime)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhaseInitializeTime, phaseInitTime);
}

PMDresult PMDSetPhaseOffset(PMDAxisHandle* axis_handle, PMDuint16 phaseOffset)
{
	return SendCommandWord(axis_handle, PMDOPSetPhaseOffset, phaseOffset);
}

PMDresult PMDGetPhaseOffset(PMDAxisHandle* axis_handle, PMDuint16* phaseOffset)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhaseOffset, phaseOffset);
}

PMDresult PMDSetPhaseAngle(PMDAxisHandle* axis_handle, PMDuint16 phaseAngle)
{
	return SendCommandWord(axis_handle, PMDOPSetPhaseAngle, phaseAngle);
}

PMDresult PMDGetPhaseAngle(PMDAxisHandle* axis_handle, PMDuint16* phaseAngle)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhaseAngle, phaseAngle);
}

PMDresult PMDSetPhaseCorrectionMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetPhaseCorrectionMode, mode);
}

PMDresult PMDGetPhaseCorrectionMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetPhaseCorrectionMode, mode);
}

PMDresult PMDInitializePhase(PMDAxisHandle* axis_handle)
{
	return SendCommand(axis_handle, PMDOPInitializePhase);
}

PMDresult PMDGetPhaseCommand(PMDAxisHandle* axis_handle, PMDuint16 phase, PMDint16* command)
{
	return SendCommandWordGetWord(axis_handle, PMDOPGetPhaseCommand, phase, (PMDuint16*)command);
}


// External Memory
PMDresult PMDSetBufferStart(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 bufferStart)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordLong(axis_handle, PMDOPSetBufferStart, bufferID, bufferStart);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetBufferStart(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* bufferStart)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordGetLong(axis_handle, PMDOPGetBufferStart, bufferID, bufferStart);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetBufferLength(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 bufferLength)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordLong(axis_handle, PMDOPSetBufferLength, bufferID, bufferLength);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetBufferLength(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* bufferLength)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordGetLong(axis_handle, PMDOPGetBufferLength, bufferID, bufferLength);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetBufferFunction(PMDAxisHandle* axis_handle, PMDuint16 function, PMDint16 bufferID)
{
	return SendCommandWordWord(axis_handle, PMDOPSetBufferFunction, function, (PMDuint16)bufferID);
}

PMDresult PMDGetBufferFunction(PMDAxisHandle* axis_handle, PMDuint16 function, PMDint16* bufferID)
{
	return SendCommandWordGetWord(axis_handle, PMDOPGetBufferFunction, function, (PMDuint16*)bufferID);
}

PMDresult PMDWriteBuffer(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDint32 data)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordLong(axis_handle, PMDOPWriteBuffer, bufferID, (PMDuint32)data);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDReadBuffer(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDint32* data)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordGetLong(axis_handle, PMDOPReadBuffer, bufferID, (PMDuint32*)data);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetBufferWriteIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 writeIndex)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordLong(axis_handle, PMDOPSetBufferWriteIndex, bufferID, writeIndex);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetBufferWriteIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* writeIndex)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordGetLong(axis_handle, PMDOPGetBufferWriteIndex, bufferID, writeIndex);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetBufferReadIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32 readIndex)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordLong(axis_handle, PMDOPSetBufferReadIndex, bufferID, readIndex);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetBufferReadIndex(PMDAxisHandle* axis_handle, PMDuint16 bufferID, PMDuint32* readIndex)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordGetLong(axis_handle, PMDOPGetBufferReadIndex, bufferID, readIndex);

	axis_handle->axis = axis;

	return result;
}


// Trace Operations
PMDresult PMDSetTraceMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWord(axis_handle, PMDOPSetTraceMode, mode);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetTraceMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandGetWord(axis_handle, PMDOPGetTraceMode, mode);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetTracePeriod(PMDAxisHandle* axis_handle, PMDuint16 tracePeriod)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWord(axis_handle, PMDOPSetTracePeriod, tracePeriod);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetTracePeriod(PMDAxisHandle* axis_handle, PMDuint16* tracePeriod)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandGetWord(axis_handle, PMDOPGetTracePeriod, tracePeriod);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetTraceVariable(PMDAxisHandle* axis_handle, PMDuint16 traceID, PMDAxis traceAxis, PMDuint8 variable)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;
	PMDuint16 value;

	// set the axis number to zero
	axis_handle->axis = 0;

	value = (PMDuint16)((traceAxis & nibbleMask) | ((variable & byteMask) << 8));
	result = SendCommandWordWord(axis_handle, PMDOPSetTraceVariable, traceID, value);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetTraceVariable(PMDAxisHandle* axis_handle, PMDuint16 traceID, PMDAxis *traceAxis, PMDuint8* variable)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;
	PMDuint16 value;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandWordGetWord(axis_handle, PMDOPGetTraceVariable, traceID, &value);
	*traceAxis = (PMDAxis)(value & nibbleMask);
	*variable = (PMDuint8)((value >> 8) & byteMask);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetTraceStart(PMDAxisHandle* axis_handle, PMDAxis traceAxis, PMDuint8 condition, PMDuint8 bit, PMDuint8 state)
{
	PMDuint16 value = (PMDuint16)((traceAxis & nibbleMask) | ((condition & nibbleMask) << 4));
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;
	
	switch(condition)
	{
	case PMDTraceConditionEventStatus:
	case PMDTraceConditionActivityStatus:
	case PMDTraceConditionSignalStatus:
		value |= ((bit & nibbleMask) << 8) | ((state & nibbleMask) << 12);
		break;
	default:
		break;
	}
	result = SendCommandWord(axis_handle, PMDOPSetTraceStart, value);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetTraceStart(PMDAxisHandle* axis_handle, PMDAxis *traceAxis, PMDuint8* condition, PMDuint8* bit, PMDuint8* state)
{
	PMDuint16 result;
	PMDuint16 value;
	PMDAxis axis = axis_handle->axis;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandGetWord(axis_handle, PMDOPGetTraceStart, &value);
	*traceAxis = (PMDAxis)((value & nibbleMask));
	*condition = (PMDuint8)((value >> 4) & nibbleMask);
	*bit = (PMDuint8)((value >> 8) & nibbleMask);
	*state = (PMDuint8)((value >> 12) & nibbleMask);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDSetTraceStop(PMDAxisHandle* axis_handle, PMDAxis traceAxis, PMDuint8 condition, PMDuint8 bit, PMDuint8 state)
{
	PMDuint16 value = (PMDuint16)((traceAxis & nibbleMask) | ((condition & nibbleMask) << 4));
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;
	
	switch(condition)
	{
	case PMDTraceConditionEventStatus:
	case PMDTraceConditionActivityStatus:
	case PMDTraceConditionSignalStatus:
		value |= ((bit & nibbleMask) << 8) | ((state & nibbleMask) << 12);
		break;
	default:
		break;
	}
	result = SendCommandWord(axis_handle, PMDOPSetTraceStop, value);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetTraceStop(PMDAxisHandle* axis_handle, PMDAxis *traceAxis, PMDuint8* condition, PMDuint8* bit, PMDuint8* state)
{
	PMDuint16 result;
	PMDuint16 value;
	PMDAxis axis = axis_handle->axis;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandGetWord(axis_handle, PMDOPGetTraceStop, &value);
	*traceAxis = (PMDAxis)((value & nibbleMask));
	*condition = (PMDuint8)((value >> 4) & nibbleMask);
	*bit = (PMDuint8)((value >> 8) & nibbleMask);
	*state = (PMDuint8)((value >> 12) & nibbleMask);

	axis_handle->axis = axis;

	return result;
}


PMDresult PMDGetTraceStatus(PMDAxisHandle* axis_handle, PMDuint16* status)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandGetWord(axis_handle, PMDOPGetTraceStatus, status);

	axis_handle->axis = axis;

	return result;
}

PMDresult PMDGetTraceCount(PMDAxisHandle* axis_handle, PMDuint32* count)
{
	PMDAxis axis = axis_handle->axis;
	PMDuint16 result;

	// set the axis number to zero
	axis_handle->axis = 0;

	result = SendCommandGetLong(axis_handle, PMDOPGetTraceCount, count);

	axis_handle->axis = axis;

	return result;
}


// Miscellaneous
PMDresult PMDSetAxisMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetAxisMode, mode);
}

PMDresult PMDGetAxisMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetAxisMode, mode);
}

PMDresult PMDSetAxisOutSource(PMDAxisHandle* axis_handle, PMDAxis sourceAxis, PMDuint8 bit, PMDuint8 sourceRegister)
{
	PMDuint16 value = (PMDuint16)((sourceAxis & nibbleMask) | ((bit & nibbleMask) << 4) 
		| ((sourceRegister & nibbleMask) << 8));
		
	return SendCommandWord(axis_handle, PMDOPSetAxisOutSource, value);
}

PMDresult PMDGetAxisOutSource(PMDAxisHandle* axis_handle, PMDAxis* sourceAxis, PMDuint8* bit, PMDuint8* sourceRegister)
{
	PMDuint16 result;
	PMDuint16 value;

	result = SendCommandGetWord(axis_handle, PMDOPGetAxisOutSource, &value);

	*sourceAxis = (PMDAxis)(value & nibbleMask);

	value = (PMDuint16)(value >> 4);
	*bit = (PMDuint8)(value & nibbleMask);

	value = (PMDuint16)(value >> 4);
	*sourceRegister = (PMDuint8)(value & nibbleMask);

	return result;
}

PMDresult PMDWriteIO(PMDAxisHandle* axis_handle, PMDuint16 address, PMDuint16 data)
{
	return SendCommandWordWord(axis_handle, PMDOPWriteIO, address, data);
}

PMDresult PMDReadIO(PMDAxisHandle* axis_handle, PMDuint16 address, PMDuint16* data)
{
	return SendCommandWordGetWord(axis_handle, PMDOPReadIO, address, data);
}

PMDresult PMDReadAnalog(PMDAxisHandle* axis_handle, PMDuint16 analogID, PMDuint16* data)
{
	return SendCommandWordGetWord(axis_handle, PMDOPReadAnalog, analogID, data);
}

PMDresult PMDReset(PMDAxisHandle* axis_handle)
{
    PMDAxis	axis = axis_handle->axis;
	PMDuint16 result;

	// for the reset command, the axis number MUST be zero
	axis_handle->axis = 0;

	result = SendCommand(axis_handle, PMDOPReset);

	axis_handle->axis = axis;

	return result;
	
}

PMDresult PMDNoOperation(PMDAxisHandle* axis_handle)
{
	return SendCommand(axis_handle, PMDOPNoOperation);
}

PMDresult PMDGetVersion(PMDAxisHandle* axis_handle, PMDuint16 *generation, PMDuint16 *motorType, 
						PMDuint16 *numberAxes, PMDuint16 *special,
						PMDuint16 *custom, PMDuint16 *major, PMDuint16 *minor)
{
	PMDuint16 chip = 0, software = 0;
	PMDuint16 result;

	result = SendCommandGetWordWord(axis_handle, PMDOPGetVersion, &chip, &software);

	*generation = (PMDuint16)((chip & 0xF000) >> 12);
	*motorType = (PMDuint16)((chip & 0x0F00) >> 8);
	*numberAxes = (PMDuint16)((chip & 0x00F0) >> 4);
	*special = (PMDuint16)(chip & 0x000F);

	*custom = (PMDuint16)((software & 0xFF00) >> 8);
	*major = (PMDuint16)((software & 0x00F0) >> 4);
	*minor = (PMDuint16)(software & 0x000F);

	return result;
}

PMDresult PMDGetHostIOError(PMDAxisHandle* axis_handle, PMDuint16 *hostIOError)
{
	return SendCommandGetWord(axis_handle, PMDOPGetHostIOError, hostIOError);
}

PMDresult PMDSetSerialPortMode(PMDAxisHandle* axis_handle, PMDuint8 baud, PMDuint8 parity, 
					PMDuint8 stopBits, PMDuint8 protocol, PMDuint8 multiDropID)
{
	PMDuint16 serialData;

	serialData = (PMDuint16)(baud | (parity << 4) | (stopBits << 6) | 
		(protocol << 7) | 
		// bits 9 and 10 are resereved for future use
		(multiDropID << 11));
	return SendCommandWord(axis_handle, PMDOPSetSerialPortMode, serialData);
}

PMDresult PMDGetSerialPortMode(PMDAxisHandle* axis_handle, PMDuint8* baud, PMDuint8* parity, 
					PMDuint8* stopBits, PMDuint8* protocol, PMDuint8* multiDropID)
{
	PMDuint16 serialData;
	PMDuint16 result;

	result = SendCommandGetWord(axis_handle, PMDOPGetSerialPortMode, &serialData);

	*baud = (PMDuint8)(serialData & 0x000F);
	*parity = (PMDuint8)((serialData & 0x0030) >> 4);
	*stopBits = (PMDuint8)((serialData & 0x0040) >> 6);
	*protocol = (PMDuint8)((serialData & 0x0180) >> 7);
	*multiDropID = (PMDuint8)((serialData & 0xF800) >> 11);

	return result;
}

PMDresult PMDSetDiagnosticPortMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetDiagnosticPortMode, mode);
}

PMDresult PMDGetDiagnosticPortMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetDiagnosticPortMode, mode);
}

PMDresult PMDGetTime(PMDAxisHandle* axis_handle, PMDuint32* numberChipCycles)
{
	return SendCommandGetLong(axis_handle, PMDOPGetTime, numberChipCycles);
}

PMDresult PMDGetChecksum(PMDAxisHandle* axis_handle, PMDuint32* checksum)
{
	return SendCommandGetLong(axis_handle, PMDOPGetChecksum, checksum);
}

PMDresult PMDSetStepRange(PMDAxisHandle* axis_handle, PMDuint16 range)
{
	return SendCommandWord(axis_handle, PMDOPSetStepRange, range);
}

PMDresult PMDGetStepRange(PMDAxisHandle* axis_handle, PMDuint16* range)
{
	return SendCommandGetWord(axis_handle, PMDOPGetStepRange, range);
}

PMDresult PMDSetSynchronizationMode(PMDAxisHandle* axis_handle, PMDuint16 mode)
{
	return SendCommandWord(axis_handle, PMDOPSetSynchronizationMode, mode);
}

PMDresult PMDGetSynchronizationMode(PMDAxisHandle* axis_handle, PMDuint16* mode)
{
	return SendCommandGetWord(axis_handle, PMDOPGetSynchronizationMode, mode);
}


// C-Motion provided functions

PMDresult PMDGetStatus(PMDAxisHandle* axis_handle)
{
	return axis_handle->transport.GetStatus(axis_handle->transport_data);
}

PMDresult PMDHasError(PMDAxisHandle* axis_handle)
{
	return axis_handle->transport.HasError(axis_handle->transport_data);
}

PMDresult PMDIsReady(PMDAxisHandle* axis_handle)
{
	return axis_handle->transport.IsReady(axis_handle->transport_data);
}

PMDresult PMDHasInterrupt(PMDAxisHandle* axis_handle)
{
	return axis_handle->transport.HasInterrupt(axis_handle->transport_data);
}

PMDresult PMDHardReset(PMDAxisHandle* axis_handle)
{  //Se invoca la funcion asociada a axis_handle->transport.HardReset
   //com parametros axis_handle->transport_data
   //que es PMDParallel_HardReset (void* transport_data)
   //luego esta funcion toma transport_data y ejecuta una de las funciones asocioadas
   //mediante PIOtransport_data->Hard();
   //que si se ha sido corretamente inicializada sera OutP8BitHard(void)
   //esta llama writePMDreset() en basicio.c
   //la cual finalmente hace el reset por hardaware y mana comandos de reset sin esperar PMDready.
	return axis_handle->transport.HardReset(axis_handle->transport_data);
}

void PMDCloseAxisInterface(PMDAxisHandle* axis_handle)
{
	axis_handle->transport.Close(axis_handle->transport_data);
}

void PMDGetCMotionVersion(PMDuint8* MajorVersion, PMDuint8* MinorVersion)
{
	*MajorVersion = CMOTION_MAJOR_VERSION;
	*MinorVersion = CMOTION_MINOR_VERSION;
}

