#include "messages.h"
#include "types.h"

#if !defined _WIN32
#include <avr/io.h>
#endif


/*****************************************************************************************/
uint8 * AxisName(tAxis Axis)
{

   switch(Axis) 
  {
    case (AxisWaist) 					: return (uint8*)"Cintura"; break;
    case (AxisShoulder) 				: return (uint8*)"Hombro"; break;
    case (AxisElbow) 					: return (uint8*)"Codo"; break;
    case (AxisPitch) 					: return (uint8*)"Pitch"; break;
    case (AxisRoll) 					: return (uint8*)"Roll"; break;
    case (AxisAux1) 					: return (uint8*)"aux1"; break;
    case (AxisAux2) 					: return (uint8*)"aux2"; break;
    case (AxisAux3) 					: return (uint8*)"aux3"; break;
    case (AxisAux4) 					: return (uint8*)"aux4"; break;
	case (AxisAux5) 					: return (uint8*)"aux5"; break;
    case (AxisAux6) 					: return (uint8*)"aux6"; break;
    case (AxisAux7) 					: return (uint8*)"aux7"; break;    
    
	default 							: return (uint8*)"invalid axis";
  }
} // AxisName

/*****************************************************************************************/
uint8 * TraceName(PMDTraceVariable Trace)
{

	switch(Trace) 
	{
    case (PMDTracePositionError) 			: return (uint8*)"Error de Posición"; break;
    case (PMDTraceCommandedPosition) 		: return (uint8*)"Posicion Comandada"; break;
    case (PMDTraceCommandedVelocity) 		: return (uint8*)"Velocidad Comandada"; break;
    case (PMDTraceCommandedAcceleration) 	: return (uint8*)"Acceleracion Comandada"; break;
    case (PMDTraceActualPosition) 			: return (uint8*)"Posicion Censada"; break;
    case (PMDTraceActualVelocity) 			: return (uint8*)"Velocidad Censada"; break;
    case (PMDTraceCurrentMotorCommand) 		: return (uint8*)"Registro MotorCommand"; break;
    case (PMDTraceTime) 					: return (uint8*)"Tiempo de Muestreo"; break;
	case (PMDTraceCaptureValue) 			: return (uint8*)"Registro de Captura"; break;
    case (PMDTraceActivityStatus) 			: return (uint8*)"Registro ActivityStatus"; break;
    case (PMDTraceSignalStatus) 			: return (uint8*)"Registro SignalStatus"; break;
	case (PMDTraceEventStatus) 				: return (uint8*)"Registro EventStatus"; break;
    default 								: return (uint8*)"invalid Var";
  }
} // AxisName

/*****************************************************************************************/
uint8 * Tracetype(PMDTraceVariable Trace)
{
	switch(Trace) 
	{

		case (PMDTracePositionError): 		
		case (PMDTraceCommandedPosition): 	
		case (PMDTraceActualPosition) 			: return (uint8*)"Grados"; break;
		case (PMDTraceCommandedVelocity): 
		case (PMDTraceActualVelocity) 			: return (uint8*)"Grados/s"; break;
		case (PMDTraceCommandedAcceleration) 	: return (uint8*)"Grados/s^2"; break;
		case (PMDTraceTime) 					: return (uint8*)"ms"; break;
		case (PMDTraceCaptureValue): 			
		case (PMDTraceCurrentMotorCommand): 		
		case (PMDTraceEventStatus): 				
		case (PMDTraceActivityStatus): 			
		case (PMDTraceSignalStatus) 			: return (uint8*)"HEX"; break;
		default 								: return (uint8*)"invalid";
	}
  }

/*****************************************************************************************/
uint8 *CommandMsg(tCommand command)
 {
  switch(command.CommandOP)
 {
 
    case (CommandNOP) 					: return (uint8*)"no command"; break;
	case (CommandGoHome)			 	: return (uint8*)"go home cmd"; break;
	case (CommandGetAxisSettings) 		: return (uint8*)"get axis settings"; break;
    case (CommandOpenGripper) 			: return (uint8*)"open gripper cmd"; break;
    case (CommandCloseGripper) 			: return (uint8*)"close gripper cmd"; break;
	case (CommandSetPoseEEPROM)			: return (uint8*)"Set Pose EEPROM cmd"; break;
	case (CommandReSetPoseEEPROM)		: return (uint8*)"Reset Pose EEPROM cmd"; break;
	case (CommandGetPoseEEPROM)			: return (uint8*)"Get Pose EEPROM cmd"; break;
	case (CommandGetPoseActual)			: return (uint8*)"Get Pose Actual cmd"; break;
	case (CommandEraseEEPROM)			: return (uint8*)"Erase EEPROM cmd"; break;
	case (CommandGetPosesID)			: return (uint8*)"Get Poses ID cmd"; break;
	case (CommandSetPMDTrace)			: return (uint8*)"Set PMD trace"; break;
	case (CommandGetPMDTrace)			: return (uint8*)"Get PMD Trace"; break;
	case (CommandGetFecha)				: return (uint8*)"Get Fecha"; break;
	case (CommandGetMemory)				: return (uint8*)"Get Memory"; break;
	case (CommandGetSensor)				: return (uint8*)"Get PMD Sensor"; break;
	case (CommandGetEventStatus)		: return (uint8*)"Get PMD EventStatus"; break;
	case (CommandGetAvtivityStatus)		: return (uint8*)"Get PMD AvtivityStatus"; break;
	case (CommandGetinfomov)			: return (uint8*)"Get PMD infomov"; break;
	default 							: return (uint8*)"unknown cmd";
  }

//  return (uint8*)"CommandMsg";   // avoid compiler warning
} // CommandMsg

/*****************************************************************************************/
uint8 *PacketMsg(tPacket packet) 
{

  switch(packet) {
    case (PacketNOP) 					:  return (uint8*)"no packet"; break;
    case (PacketCommand)				:  return (uint8*)"cmd packet"; break;
    case (PacketVector) 				:  return (uint8*)"vector packet"; break;
    case (PacketTrajectory) 			:  return (uint8*)"trajectory packet"; break;
	case (PacketTeachPendant) 			:  return (uint8*)"TeachPendant packet"; break;
    case (PacketString) 				:  return (uint8*)"string packet"; break;
    case (PacketAxisSettings) 			: return (uint8*)"axis settings packet"; break;
    case (PacketAxisLimits) 			: return (uint8*)"axis limits packet"; break;
	case (PacketHeader) 				: return (uint8*)"packet Header"; break;
    
	default : return (uint8*)"unknown packet"; break;
  }

//  return (uint8*)"PacketMsg";    // avoid compiler warning
} // PacketMsg

/*****************************************************************************************/
uint8 *rcMsg(tReturnCode rc) 
{

  switch(rc) {
    case (rcOK) 						: return (uint8*)"all ok"; break;

// packet related:
    case (rcInvalidHeader) 				: return (uint8*)"invalid Header"; break;
    case (rcInvalidData) 				: return (uint8*)"invalid data"; break;
    case (rcInvalidPacket) 				: return (uint8*)"invalid Packet"; break;
    case (rcInvalidCommand) 			: return (uint8*)"invalid Command"; break;
 //   case (rcInvalidError) 				: return (uint8*)"invalid Error"; break;
    case (rcInvalidChecksum) 			: return (uint8*)"invalidChecksum"; break;
    case (rcInvalidHeaderChecksum) 		: return (uint8*)"invalidHeaderChecksum"; break;
    case (rcTooFewData) 				: return (uint8*)"too few data"; break;
//    case (rcTooMuchData) 				: return (uint8*)"too much data"; break;
    case (rcNoMemory) 					: return (uint8*)"no memory"; break;

// USB communication related:   
    case (rcTimeoutSendingData) 		: return (uint8*)"timeout sending data"; break;
    case (rcTimeoutReadingData) 		: return (uint8*)"timeout reading data"; break;
    case (rcCommunicationError) 		: return (uint8*)"communication error"; break;
//    case (rcUnexpectedResponse) 		: return (uint8*)"unexpected response"; break;
    case (rcUnexpectedState) 			: return (uint8*)"unexpected ISA interface state"; break;
//    case (rcSendInterrupted) 			: return (uint8*)"incoming data while sending"; break;
	
// microcontroller system related:
//    case (rcUCisBusy) 					: return (uint8*)"uC is busy"; break;
//    case (rcUCNeedsReset) 				: return (uint8*)"uC needs reset"; break;
//    case (rcUCNeedsInitialization) 		: return (uint8*)"uC needs initialisation"; break;

// robot related:
    case (rcHardwareFailure) 			: return (uint8*)"hardware failure"; break;
//    case (rcVelocityLimitExceeded) 		: return (uint8*)"velocity too high"; break;
 //   case (rcAccelerationLimitExceeded) 	: return (uint8*)"acceleration too high"; break;
	
// for the PMD reset:
    case (rcPMDChipsetResetError) 		: return (uint8*)"PMD chipset reset error"; break;
//    case (rcPMDCommandError) 			: return (uint8*)"PMD cmd error"; break;
    case (rcPMDchecksumError) 			: return (uint8*)"PMD checksum wrong"; break;
//    case (rcPMDstuckBit0) 				: return (uint8*)"PMD data line stuck at 0"; break;
//    case (rcPMDstuckBit1) 				: return (uint8*)"PMD data line stuck at 1"; break;
 //   case (rcPMDtimeout) 				: return (uint8*)"PMD communication timeout"; break;
//    case (rcOtherError) 				: return (uint8*)"unspecified rc code"; break;
	
// file procecing related	
	case (rcFileOpenError) 				: return (uint8*)"Open File Error"; break;
	case (rcFileOpenOK) 				: return (uint8*)"Open File OK"; break;
	case (rcFileReadOK) 				: return (uint8*)"Read File OK"; break;
	case (rcFileReadError) 				: return (uint8*)"Read File Error"; break;
    
	default : return (uint8*)"unknown rc code"; break;
  }

 // return (uint8*)"rcMsg";    // avoid compiler warning
} // rcMsg



// PMD specific messages



#ifdef _AVR_IO_H_
//*****************************************************************************
uint8 *PMDGetOpcodeText(PMDuint16 opCode)
{
	opCode=opCode & 0xff;

	switch(opCode) {
	
	case (PMDOPNoOperation)				: return (uint8*)"Nop"; break;
	case (PMDOPSetMotorLimit) 			: return (uint8*)"SetMotorLimit"; break;
	case (PMDOPGetMotorLimit) 			: return (uint8*)"GetMotorLimit"; break;
	case (PMDOPSetMotorBias) 			: return (uint8*)"SetMotorBias"	; break;
	case (PMDOPSetPosition) 			: return (uint8*)"SetPosition"	; break;
	case (PMDOPSetVelocity) 			: return (uint8*)"SetVelocity"; break;
	case (PMDOPSetJerk) 				: return (uint8*)"SetJerk"	; break;
	case (PMDOPSetGearRatio) 			: return (uint8*)"SetGearRatio"	; break;
	case (PMDOPUpdate) 					: return (uint8*)"Update"; break;
	case (PMDOPGetCommandedPosition)	: return (uint8*)"GetCommandedPosition"	; break;
	case (PMDOPGetCommandedVelocity) 	: return (uint8*)"GetCommandedVelocity"	; break;
	case (PMDOPSetKp) 					: return (uint8*)"SetKp"; break;
	case (PMDOPSetKi) 					: return (uint8*)"SetKi"; break;
	case (PMDOPSetKd) 					: return (uint8*)"SetKd"; break;
	case (PMDOPSetKvff) 				: return (uint8*)"SetKvff"; break;
	case (PMDOPGetPhaseAngle) 			: return (uint8*)"GetPhaseAngle"; break;
	case (PMDOPGetMotorBias) 			: return (uint8*)"GetMotorBias"	; break;
	case (PMDOPSetInterruptMask) 		: return (uint8*)"SetInterruptMask"	; break;
	case (PMDOPGetEventStatus) 			: return (uint8*)"GetEventStatus"; break;
	case (PMDOPResetEventStatus) 		: return (uint8*)"ResetEventStatus"	; break;
	case (PMDOPGetCaptureValue) 		: return (uint8*)"GetCaptureValue"; break;
	case (PMDOPGetActualPosition) 		: return (uint8*)"GetActualPosition"; break;
	case (PMDOPSetSampleTime) 			: return (uint8*)"SetSampleTime"; break;
	case (PMDOPReset) 					: return (uint8*)"Reset"; break;
	case (PMDOPGetCurrentMotorCommand)	: return (uint8*)"GetCurrentMotorCommand"; break;
	case (PMDOPGetTime) 				: return (uint8*)"GetTime"; break;
	case (PMDOPClearPositionError) 		: return (uint8*)"ClearPositionError"; break;
	case (PMDOPGetPosition) 			: return (uint8*)"GetPosition"; break;
	case (PMDOPGetVelocity) 			: return (uint8*)"GetVelocity"; break;
	case (PMDOPGetAcceleration) 		: return (uint8*)"GetAcceleration"; break;
	case (PMDOPSetActualPosition) 		: return (uint8*)"SetActualPosition"; break;
	case (PMDOPGetKp) 					: return (uint8*)"GetKp"; break;
	case (PMDOPGetKi) 					: return (uint8*)"GetKi"; break;
	case (PMDOPGetKd) 					: return (uint8*)"GetKd"; break;
	case (PMDOPGetKvff) 				: return (uint8*)"GetKvff"; break;
	case (PMDOPGetInterruptMask)		: return (uint8*)"GetInterruptMask"	; break;
	case (PMDOPGetJerk) 				: return (uint8*)"GetJerk"; break;
	case (PMDOPGetGearRatio)			: return (uint8*)"GetGearRatio"	; break;
	case (PMDOPMultiUpdate) 			: return (uint8*)"MultiUpdate"; break;
	case (PMDOPGetSampleTime) 			: return (uint8*)"GetSampleTime"; break;
	case (PMDOPGetMotorCommand) 		: return (uint8*)"GetMotorCommand"; break;
	case (PMDOPSetStartVelocity) 		: return (uint8*)"SetStartVelocity"; break;
	case (PMDOPGetStartVelocity) 		: return (uint8*)"GetStartVelocity"; break;
	case (PMDOPGetOutputMode) 			: return (uint8*)"GetOutputMode"; break;
	case (PMDOPSetPhaseInitializeTime)	: return (uint8*)"SetPhaseInitializeTime"; break;
	case (PMDOPSetPhaseCounts) 			: return (uint8*)"SetPhaseCounts"; break;
	case (PMDOPSetPhaseOffset) 			: return (uint8*)"SetPhaseOffset"; break;
	case (PMDOPSetMotorCommand) 		: return (uint8*)"SetMotorCommand"; break;
	case (PMDOPInitializePhase) 		: return (uint8*)"InitializePhase"; break;
	case (PMDOPGetPhaseOffset) 			: return (uint8*)"GetPhaseOffset"; break;
	case (PMDOPGetPhaseInitializeTime)	: return (uint8*)"GetPhaseInitializeTime"; break;
	case (PMDOPGetPhaseCounts) 			: return (uint8*)"GetPhaseCounts"; break;
	case (PMDOPSetLimitSwitchMode) 		: return (uint8*)"SetLimitSwitchMode"; break;
	case (PMDOPGetLimitSwitchMode) 		: return (uint8*)"GetLimitSwitchMode"; break;
	case (PMDOPWriteIO) 				: return (uint8*)"WriteIO"; break;
	case (PMDOPReadIO) 					: return (uint8*)"ReadIO"; break;
	case (PMDOPSetPhaseAngle) 			: return (uint8*)"SetPhaseAngle"; break;
	case (PMDOPSetNumberPhases) 		: return (uint8*)"SetNumberPhases"; break;
	case (PMDOPGetNumberPhases) 		: return (uint8*)"GetNumberPhases"; break;
	case (PMDOPSetAxisMode) 			: return (uint8*)"SetAxisMode"	; break;
	case (PMDOPGetAxisMode) 			: return (uint8*)"GetAxisMode"; break;
	case (PMDOPSetDiagnosticPortMode)	: return (uint8*)"SetDiagnosticPortMode"; break;
	case (PMDOPGetDiagnosticPortMode)	: return (uint8*)"GetDiagnosticPortMode"; break;
	case (PMDOPSetSerialPortMode) 		: return (uint8*)"SetSerialPortMode"; break;
	case (PMDOPGetSerialPortMode)		: return (uint8*)"GetSerialPortMode"; break;
	case (PMDOPSetEncoderModulus) 		: return (uint8*)"SetEncoderModulus"; break;
	case (PMDOPGetEncoderModulus) 		: return (uint8*)"GetEncoderModulus"; break;
	case (PMDOPGetVersion) 				: return (uint8*)"GetVersion"; break;
	case (PMDOPSetAcceleration)			: return (uint8*)"SetAcceleration"; break;
	case (PMDOPSetDeceleration) 		: return (uint8*)"SetDeceleration"; break;
	case (PMDOPGetDeceleration) 		: return (uint8*)"GetDeceleration"; break;
	case (PMDOPSetKaff) 				: return (uint8*)"SetKaff"; break;
	case (PMDOPGetKaff) 				: return (uint8*)"GetKaff"; break;
	case (PMDOPSetIntegrationLimit)		: return (uint8*)"SetIntegrationLimit"; break;
	case (PMDOPGetIntegrationLimit) 	: return (uint8*)"GetIntegrationLimit"; break;
	case (PMDOPSetPositionErrorLimit) 	: return (uint8*)"SetPositionErrorLimit"; break;
	case (PMDOPGetPositionErrorLimit)	: return (uint8*)"GetPositionErrorLimit"; break;
	case (PMDOPGetPositionError) 		: return (uint8*)"GetPositionError"	; break;
	case (PMDOPGetIntegral) 			: return (uint8*)"GetIntegral"; break;
	case (PMDOPGetDerivative) 			: return (uint8*)"GetDerivative"; break;
	case (PMDOPSetDerivativeTime) 		: return (uint8*)"SetDerivativeTime"; break;
	case (PMDOPGetDerivativeTime) 		: return (uint8*)"GetDerivativeTime"; break;
	case (PMDOPSetKout) 				: return (uint8*)"SetKout"; break;
	case (PMDOPGetKout) 				: return (uint8*)"GetKout"; break;
	case (PMDOPSetProfileMode)			: return (uint8*)"SetProfileMode"; break;
	case (PMDOPGetProfileMode) 			: return (uint8*)"GetProfileMode"; break;
	case (PMDOPSetSignalSense) 			: return (uint8*)"SetSignalSense"; break;
	case (PMDOPGetSignalSense) 			: return (uint8*)"GetSignalSense"; break;
	case (PMDOPGetSignalStatus) 		: return (uint8*)"GetSignalStatus"; break;
	case (PMDOPGetHostIOError) 			: return (uint8*)"GetHostIOError"; break;
	case (PMDOPGetActivityStatus) 		: return (uint8*)"GetActivityStatus"; break;
	case (PMDOPGetCommandedAcceleration): return (uint8*)"GetCommandedAcceleration"; break;
	case (PMDOPSetTrackingWindow) 		: return (uint8*)"SetTrackingWindow"; break;
	case (PMDOPGetTrackingWindow) 		: return (uint8*)"GetTrackingWindow"; break;
	case (PMDOPSetSettleTime) 			: return (uint8*)"SetSettleTime"; break;
	case (PMDOPGetSettleTime) 			: return (uint8*)"GetSettleTime"; break;
	case (PMDOPClearInterrupt) 			: return (uint8*)"ClearInterrupt"; break;
	case (PMDOPGetActualVelocity) 		: return (uint8*)"GetActualVelocity"; break;
	case (PMDOPSetGearMaster) 			: return (uint8*)"SetGearMaster"; break;
	case (PMDOPGetGearMaster) 			: return (uint8*)"GetGearMaster"; break;
	case (PMDOPSetTraceMode) 			: return (uint8*)"SetTraceMode"	; break;
	case (PMDOPGetTraceMode) 			: return (uint8*)"GetTraceMode"	; break;
	case (PMDOPSetTraceStart) 			: return (uint8*)"SetTraceStart"; break;
	case (PMDOPGetTraceStart) 			: return (uint8*)"GetTraceStart"; break;
	case (PMDOPSetTraceStop) 			: return (uint8*)"SetTraceStop"	; break;
	case (PMDOPGetTraceStop) 			: return (uint8*)"GetTraceStop"	; break;
	case (PMDOPSetTraceVariable) 		: return (uint8*)"SetTraceVariable"; break;
	case (PMDOPGetTraceVariable) 		: return (uint8*)"GetTraceVariable"; break;
	case (PMDOPSetTracePeriod) 			: return (uint8*)"SetTracePeriod"; break;
	case (PMDOPGetTracePeriod) 			: return (uint8*)"GetTracePeriod"; break;
	case (PMDOPGetTraceStatus) 			: return (uint8*)"GetTraceStatus"; break;
	case (PMDOPGetTraceCount) 			: return (uint8*)"GetTraceCount"; break;
	case (PMDOPSetSettleWindow) 		: return (uint8*)"SetSettleWindow"; break;
	case (PMDOPGetSettleWindow) 		: return (uint8*)"GetSettleWindow"; break;
	case (PMDOPSetActualPositionUnits)	: return (uint8*)"SetActualPositionUnits"; break;
	case (PMDOPGetActualPositionUnits)	: return (uint8*)"GetActualPositionUnits"; break;
	case (PMDOPSetBufferStart) 			: return (uint8*)"SetBufferStart"; break;
	case (PMDOPGetBufferStart) 			: return (uint8*)"GetBufferStart"; break;
	case (PMDOPSetBufferLength) 		: return (uint8*)"SetBufferLength"; break;
	case (PMDOPGetBufferLength) 		: return (uint8*)"GetBufferLength"; break;
	case (PMDOPSetBufferWriteIndex) 	: return (uint8*)"SetBufferWriteIndex"; break;
	case (PMDOPGetBufferWriteIndex) 	: return (uint8*)"GetBufferWriteIndex"; break;
	case (PMDOPSetBufferReadIndex) 		: return (uint8*)"SetBufferReadIndex"; break;
	case (PMDOPGetBufferReadIndex) 		: return (uint8*)"GetBufferReadIndex"; break;
	case (PMDOPWriteBuffer) 			: return (uint8*)"WriteBuffer"; break;
	case (PMDOPReadBuffer) 				: return (uint8*)"ReadBuffer"; break;
	case (PMDOPSetBufferFunction) 		: return (uint8*)"SetBufferFunction"; break;
	case (PMDOPGetBufferFunction) 		: return (uint8*)"GetBufferFunction"; break;
	case (PMDOPGetStepRange) 			: return (uint8*)"GetStepRange"	; break;
	case (PMDOPSetStepRange) 			: return (uint8*)"SetStepRange"; break;
	case (PMDOPSetStopMode) 			: return (uint8*)"SetStopMode"; break;
	case (PMDOPGetStopMode) 			: return (uint8*)"GetStopMode"; break;
	case (PMDOPSetAutoStopMode) 		: return (uint8*)"SetAutoStopMode"; break;
	case (PMDOPGetAutoStopMode) 		: return (uint8*)"GetAutoStopMode"; break;
	case (PMDOPSetBreakpoint) 			: return (uint8*)"SetBreakpoint"; break;
	case (PMDOPGetBreakpoint) 			: return (uint8*)"GetBreakpoint"; break;
	case (PMDOPSetBreakpointValue) 		: return (uint8*)"SetBreakpointValue"; break;
	case (PMDOPGetBreakpointValue) 		: return (uint8*)"GetBreakpointValue"; break;
	case (PMDOPSetCaptureSource) 		: return (uint8*)"SetCaptureSource"	; break;
	case (PMDOPGetCaptureSource) 		: return (uint8*)"GetCaptureSource"	; break;
	case (PMDOPSetEncoderSource) 		: return (uint8*)"SetEncoderSource"	; break;
	case (PMDOPGetEncoderSource) 		: return (uint8*)"GetEncoderSource"	; break;
	case (PMDOPSetMotorMode) 			: return (uint8*)"SetMotorMode"	; break;
	case (PMDOPGetMotorMode) 			: return (uint8*)"GetMotorMode"	; break;
	case (PMDOPSetEncoderToStepRatio) 	: return (uint8*)"SetEncoderToStepRatio"; break;
	case (PMDOPGetEncoderToStepRatio) 	: return (uint8*)"GetEncoderToStepRatio"; break;
	case (PMDOPSetOutputMode) 			: return (uint8*)"SetOutputMode"; break;
	case (PMDOPGetInterruptAxis) 		: return (uint8*)"GetInterruptAxis"	; break;
	case (PMDOPSetCommutationMode) 		: return (uint8*)"SetCommutationMode"; break;
	case (PMDOPGetCommutationMode) 		: return (uint8*)"GetCommutationMode"; break;
	case (PMDOPSetPhaseInitializeMode) 	: return (uint8*)"SetPhaseInitializeMode"; break;
	case (PMDOPGetPhaseInitializeMode) 	: return (uint8*)"GetPhaseInitializeMode"; break;
	case (PMDOPSetPhasePrescale) 		: return (uint8*)"SetPhasePrescale"; break;
	case (PMDOPGetPhasePrescale) 		: return (uint8*)"GetPhasePrescale"; break;
	case (PMDOPSetPhaseCorrectionMode) 	: return (uint8*)"SetPhaseCorrectionMode"; break;
	case (PMDOPGetPhaseCorrectionMode) 	: return (uint8*)"GetPhaseCorrectionMode"; break;
	case (PMDOPGetPhaseCommand) 		: return (uint8*)"GetPhaseCommand"; break;
	case (PMDOPSetMotionCompleteMode) 	: return (uint8*)"SetMotionCompleteMode"; break;
	case (PMDOPGetMotionCompleteMode) 	: return (uint8*)"GetMotionCompleteMode"; break;
	case (PMDOPSetAxisOutSource) 		: return (uint8*)"SetAxisOutSource"	; break;
	case (PMDOPGetAxisOutSource) 		: return (uint8*)"GetAxisOutSource"	; break;
	case (PMDOPReadAnalog) 				: return (uint8*)"ReadAnalog"; break;
	case (PMDOPAdjustActualPosition)	: return (uint8*)"AdjustActualPosition"; break;
	case (PMDOPGetChecksum) 			: return (uint8*)"GetChecksum"; break;
	
	default 							: return (uint8*)"Invalid opcode!"; break;
	}
}

/*****************************************************************************************/
uint8 *PMDGetErrorMessage(PMDuint16 errorCode) 
{

  switch(errorCode) {
    
	// MC2400 Chipset error codes
	
	case ( PMD_ERR_OK) 						: return (uint8*)  "NO Error"; break;
	case ( PMD_ERR_ChipsetReset)  			: return (uint8*)  "Chipset Reset "; break;
    case ( PMD_ERR_InvalidInstruction)  	: return (uint8*)  "Invalid Instruction"; break;
    case ( PMD_ERR_InvalidAxis)  			: return (uint8*)  "Invalid Axis"; break;
    case ( PMD_ERR_InvalidParameter)  		: return (uint8*)  "Invalid Data Parameter"; break;
	case ( PMD_ERR_TraceRunning)			: return (uint8*)  "Trace Currently Running" ;break;
	case ( PMD_ERR_Flash)					: return (uint8*)  "Flash Error" ;break;
	case ( PMD_ERR_BlockOutOfBounds)		: return (uint8*)  "Block Out of Bounds" ;break;
	case ( PMD_ERR_TraceBufferZero)			: return (uint8*)  "Zero Length Trace Buffer" ;break;
	case ( PMD_ERR_BadSerialChecksum)		: return (uint8*)  "Invalid Checksum" ;break;
	case ( PMD_ERR_NotPrimaryPort)			: return (uint8*)  "Invalid Instruction For Diagnostic Port" ;break;
	case ( PMD_ERR_InvalidNegativeValue)	: return (uint8*)  "Invalid Negative Value for Profile Mode" ;break;
	case ( PMD_ERR_InvalidParameterChange) 	: return (uint8*)  "Invalid Parameter Change for Profile Mode" ;break;
	case ( PMD_ERR_LimitEventPending)		: return (uint8*)  "Invalid Move with Limit Event Pending" ;break;
	case ( PMD_ERR_InvalidMoveIntoLimit)	: return (uint8*)  "Invalid Move into Limit" ;break;
    case ( PMD_ERR_InvalidClearInterrupt)	: return (uint8*)  "Invalid Clear Interrupt" ;break;
	
	// DK2000 initialization errors
	case ( PMD_ERR_InvalidOperation)		: return (uint8*)  "Invalid Operation" ;break;
	case ( PMD_ERR_NotConnected)			: return (uint8*)  "Not Connected" ;break;
	case ( PMD_ERR_ChipsetNotResponding)	: return (uint8*)  "Chipset Not Responding" ;break;
	case ( PMD_ERR_CommPortRead)			: return (uint8*)  "Comm Port Read" ;break;
	case ( PMD_ERR_CommPortWrite)			: return (uint8*)  "Comm Port Write" ;break;
	case ( PMD_ERR_InvalidSerialPort)		: return (uint8*)  "Invalid Serial Port" ;break;
	case ( PMD_ERR_InvalidDK2000IOBase)		: return (uint8*)  "Invalid DK2000 IO Base" ;break;
	case ( PMD_ERR_InvalidDK2000IRQ)		: return (uint8*)  "Invalid DK2000 IRQ" ;break;
	case ( PMD_ERR_InvalidDK2000IOMode)		: return (uint8*)  "Invalid DK2000 IO Mode" ;break;
	case ( PMD_ERR_PMDNotInitialized)		: return (uint8*)  "PMD Not Initialized" ;break;
	case ( PMD_ERR_OpeningWindowsDriver)	: return (uint8*)  "Cannot Open Windows Driver" ;break;
	
	// run-time errors
	case ( PMD_ERR_Driver)					: return (uint8*)  "Windows Driver Error" ;break;
	case ( PMD_ERR_CommunicationsError)		: return (uint8*)  "Communication Error" ;break;
	case ( PMD_ERR_CommTimeoutError)		: return (uint8*)  "Communication Timeout Error" ;break;
	case ( PMD_ERR_ChecksumError)			: return (uint8*)  "Checksum Error" ;break;
	case ( PMD_ERR_CommandError)			: return (uint8*)  "Command Error" ;break;
	
	
	
    
	default: return (uint8*)"Undefined error occurred";
  }

 // return (uint8*)"Undefined error occurred";    // avoid compiler warning
} // PMDGetErrorMessage
#endif

/*****************************************************************************************/
uint8 *Dayname(tdia dia)
{

	switch(dia) 
	{
		case (Domingo) 					: return (uint8*)"Domingo"; break;
		case (Lunes) 					: return (uint8*)"Lunes"; break;
		case (Martes) 					: return (uint8*)"Martes"; break;
		case (Miercoles) 				: return (uint8*)"Miercoles"; break;
		case (Jueves) 					: return (uint8*)"Jueves"; break;
		case (Viernes) 					: return (uint8*)"Viernes"; break;
		case (Sabado) 					: return (uint8*)"Sabado"; break;
		   
		default 						: return (uint8*)"invalid day";
	}

} // Dayname


/*****************************************************************************************/
uint8 *Monthname(tmes mes)
{

	switch(mes) 
	{
		case (Enero) 					: return (uint8*)"Enero"; break;
		case (Febrero) 					: return (uint8*)"Febrero"; break;
		case (Marzo) 					: return (uint8*)"Marzo"; break;
		case (Abril) 					: return (uint8*)"Abril"; break;
		case (Mayo) 					: return (uint8*)"Mayo"; break;
		case (Junio) 					: return (uint8*)"Junio"; break;
		case (Julio) 					: return (uint8*)"Julio"; break;
		case (Agosto) 					: return (uint8*)"Agosto"; break;
		case (Septiembre) 				: return (uint8*)"Septiembre"; break;
		case (Octubre) 					: return (uint8*)"Octubre"; break;
		case (Noviembre) 				: return (uint8*)"Noviembre"; break;
		case (Diciembre) 				: return (uint8*)"Diciembre"; break;
		
		default 						: return (uint8*)"invalid month";
	}

  //return (uint8*)"Monthname";  // avoid compiler warning
} //Monthname