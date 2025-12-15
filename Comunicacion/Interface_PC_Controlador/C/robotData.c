
#include "robotData.h"
#include "types.h"
#include "conversion.h"
#if !defined(_AVR_IO_H_)
#include <windows.h>			//funciones basicas de ventanas graficas
#endif

#if defined(_AVR_IO_H_)
#include <avr/eeprom.h>



//Variable en eeprom que contiene la configuracion 
tAxisSettings EEMEM EEprom_axisSettings[DOF];
//Le indica al el programa si utilizar la configuracion por defecto o la reconfigurada.
int16 EEMEM EEprom_Reconf_Indicator=0;

#endif


 void defaultAxisSettings(int16 axis, tAxisSettings *paxisSettings)
{
//General Settings:
  paxisSettings->axisEnabled = TRUE;      
  paxisSettings->interruptEnable=TRUE;
  paxisSettings->switchEnable=TRUE;
  paxisSettings->encoderEnable=FALSE; 
  paxisSettings->ustepsPerStep = 32; //Quiere decir que 32 uP hacen un paso
  paxisSettings->motorStepsPerRevolution = 200;//El angulo de paso es 15ª 360/15 = 24, oseo que con 24 pasos se tiene una vuelta 
  paxisSettings->encoderStepsPerRevolution = 480;
  paxisSettings->ErrorLimit=200;
  paxisSettings->TrackingWindow	= 30;
  paxisSettings->SettleWindow = 22; //
  paxisSettings->SettleTime	= 833; // 0.5 seg  
  paxisSettings->motorPower = 98; // % of full power
  paxisSettings->numberPhases = 2;
  paxisSettings->limitVelocity = 1310720; //20*65536;    // (2500rpm/60s)*48spr*16usps/1623cycle/s) !! 2500 rmp is close to the limit of the numerical range
  paxisSettings->limitAcceleration = 786; //0.012*65536;  // absolute maximum values which the hardware can do
  paxisSettings->limitJerk = 128849; //0.00003*4294967296;  // default value for S-curve profile...400 acceleration cycles (0.25s)
  paxisSettings->maxVelocity = 327680; //0.25*paxisSettings->limitVelocity;  // 25% of maximum
  paxisSettings->minVelocity = 13107; //0.01*paxisSettings->limitVelocity;  // 1%, for initial finding of the limit switches and home position
  paxisSettings->maxAcceleration = 197; //0.25*paxisSettings->limitAcceleration; // 25% of maximum
  paxisSettings->homeSwitchHandling = 0; // 3 switches: for neg. limit, pos. limit and home
  paxisSettings->gearNominator = 1;      // PC needs that to change rad into usteps
  paxisSettings->gearDenominator = 1;    // gear=Nominator/Denominator, step=rad*gear*other_stuff
  // specific settings:
  switch(axis) 
{
   case AxisWaist:
		paxisSettings->motorStepsPerRevolution = 32;
		paxisSettings->numberPhases = 3;
		paxisSettings->gearNominator = 9959;      // PC needs that to change rad into usteps
		paxisSettings->gearDenominator = 1000;	  
		paxisSettings->maxVelocity = gradrosTOuPasos(paxisSettings,71.819,VELOCIDAD_TYPE);//80000; 
		paxisSettings->minVelocity = gradrosTOuPasos(paxisSettings,53.864,VELOCIDAD_TYPE);//60000; 
		paxisSettings->homeVelocity = gradrosTOuPasos(paxisSettings,70,VELOCIDAD_TYPE);//  89111?
		paxisSettings->homeAcceleration= gradrosTOuPasos(paxisSettings,59.101,ACELERACION_TYPE); //40; 
		paxisSettings->maxAxisAngle=122;
		paxisSettings->minAxisAngle=-136;

    break;
    case AxisShoulder://HOMBRO
		paxisSettings->homeSwitchHandling = -1; // home = neg. limit
		paxisSettings->gearNominator = 11769;
		paxisSettings->gearDenominator = 100;
		paxisSettings->maxVelocity = gradrosTOuPasos(paxisSettings,2.6102,VELOCIDAD_TYPE);//214748; 
		paxisSettings->minVelocity = gradrosTOuPasos(paxisSettings,0.8496,VELOCIDAD_TYPE);//69905; 
	    paxisSettings->homeVelocity = gradrosTOuPasos(paxisSettings,20,VELOCIDAD_TYPE);//2468138?; 
		paxisSettings->homeAcceleration= gradrosTOuPasos(paxisSettings,9.9974,ACELERACION_TYPE);//494;  
		paxisSettings->maxAxisAngle=10;
		paxisSettings->minAxisAngle=-140;
		
    break;
    case AxisElbow://CODO
		paxisSettings->homeSwitchHandling = 1; // home = rigth. limit
		paxisSettings->gearNominator = 11805;//11805//11768;//11955;  //12105;
		paxisSettings->gearDenominator = 100;
		paxisSettings->maxVelocity = gradrosTOuPasos(paxisSettings,1.2117,VELOCIDAD_TYPE);//100000; 
		paxisSettings->minVelocity = gradrosTOuPasos(paxisSettings,0.5965,VELOCIDAD_TYPE);//49231; 
		paxisSettings->homeVelocity = gradrosTOuPasos(paxisSettings,20,VELOCIDAD_TYPE);//2475688?; 
		paxisSettings->homeAcceleration= gradrosTOuPasos(paxisSettings,9.9871,ACELERACION_TYPE);//495;  
		paxisSettings->maxAxisAngle=120;
		paxisSettings->minAxisAngle=-126;
		//paxisSettings->ustepsPerStep = 128;
		
    break;
    case AxisPitch://PITCH
		paxisSettings->homeSwitchHandling = 1; // home = rigth. limit
		paxisSettings->gearNominator = 20256;
		paxisSettings->gearDenominator = 10000;// PC needs that to change rad into usteps
		paxisSettings->homeVelocity = gradrosTOuPasos(paxisSettings,76.3825,VELOCIDAD_TYPE);//108158; // 20 deg/seg
		paxisSettings->homeAcceleration= gradrosTOuPasos(paxisSettings,57.0858,ACELERACION_TYPE);//49;  // 30 deg/seg2
		paxisSettings->maxAxisAngle=174;
		paxisSettings->minAxisAngle=-20;
    break;
    case AxisRoll:
     	paxisSettings->interruptEnable= FALSE;
		paxisSettings->gearNominator = 1327;
		paxisSettings->gearDenominator = 1000;// PC needs that to change rad into usteps
		paxisSettings->homeVelocity = gradrosTOuPasos(paxisSettings,119.998,VELOCIDAD_TYPE);//111316; // 30 deg/seg
		paxisSettings->homeAcceleration= gradrosTOuPasos(paxisSettings,58.3918,ACELERACION_TYPE);//33;  // 30 deg/seg2
		paxisSettings->maxAxisAngle=360;
		paxisSettings->minAxisAngle=0;
    break;    
    case AxisAux1:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
    case AxisAux2:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
	case AxisAux3:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
    case AxisAux4:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
	case AxisAux5:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
    case AxisAux6:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
    case AxisAux7:
		paxisSettings->axisEnabled = FALSE;
		paxisSettings->interruptEnable= FALSE;
		paxisSettings->encoderEnable=FALSE; 
    break;
  } // switch
}// defaultAxisSettings

/*****************************************************************************************/
void setupAxisSettings(tAxisSettings *axisSettings) 
{
  int16 i;

  for(i=0; i<DOF; i++) 
  {
    defaultAxisSettings(i,&(axisSettings[i]));
  }
} // setupAxisHandles
