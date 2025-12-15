#include <math.h>      
#include "conversion.h"
#ifdef _WIN32
	#define M_PI 3.141592653589793238462643	
#endif


/*****************************************************************************************/
int32 positionPCtoUC(double f) 
{
	return (int32)floor(0.50+f);  // 32.0 format
} // positionPCtoUC

/*****************************************************************************************/
// convert also from seconds to cycles
int32 velocityPCtoUC(double f) 
{
	double temp=0;
	
	temp=f*SAMPLETIME; //De segundos a sampletime 
	temp=0.50+65536.0*temp;
	temp=floor(temp);
	return (int32)temp;  // 16.16 format
} // velocityPCtoUC



/*****************************************************************************************/
// convert also from seconds to cycles
int32 accelerationPCtoUC(double f) 
{
	double temp=0;
	
	temp=f*SAMPLETIME*SAMPLETIME; //De segundos a sampletime 
	temp=0.50+(65536.0*temp);
    temp=floor(temp);
	return (int32)temp;  // 16.16 format
} // accelerationPCtoUC



/*****************************************************************************************/
// convert angle in rad into microsteps
double rad2usteps(tAxisSettings *paxisSettings, double rad) 
{
	double resul,x,y,z;
	
	x=(double) paxisSettings->gearNominator/(double)paxisSettings->gearDenominator;
	y=(rad/(2*M_PI));
	z=(double)paxisSettings->motorStepsPerRevolution* (double)paxisSettings->ustepsPerStep;
	resul= x*y*z;
	return resul; 
} // rad2usteps

/*****************************************************************************************/
// convert microsteps into angle in rad
double ustep2rad(tAxisSettings *paxisSettings, double usteps) 
{
	double resul,x,y;

	x=(double)paxisSettings->gearDenominator/(double)paxisSettings->gearNominator;
	y=1/( (double)paxisSettings->ustepsPerStep*(double)paxisSettings->motorStepsPerRevolution );
	resul=usteps*2*M_PI*x*y;
	return resul;
} // ustep2rad

/*****************************************************************************************/
double grados2rad(double deg)
{
	double rad;

	rad=deg*(M_PI/180);
	return rad;
}

/*****************************************************************************************/
double rad2grados(double rad)
{
	double grad;

	grad=rad/(M_PI/180);
	return grad;
}

/*****************************************************************************************/
uint8 bcd2int(uint8 bcd_format)
{
	uint8 integer;
	
	integer= (((bcd_format >>4)*10) + (bcd_format%16));
	
	return integer;
}

/*****************************************************************************************/
uint8 int2bcd(uint8 integer)
{
	uint8 bcd_format;
	
	bcd_format= ((integer/10)<<4) + (integer%10);
	
	return bcd_format;
}

/*
int32 gradosMotortouPasos(double grados) 
{
	
	double resul,y;

	
	y=(double)paxisSettings->ustepsPerStep*(double)paxisSettings->motorStepsPerRevolution;
	resul=y/360;
	return (int32)floor(0.50+resul);  // 32.0 format
} 
*/

//paxisSettings apuntador a las caracteristicas de la acticulacion a convertir. 

/*****************************************************************************************/
double uPasosTOgradros(tAxisSettings *paxisSettings,int32 Valor,int tipo)
{

	double result,Data;

	result=-1;
	
	switch(tipo) 
	{
		case POSICION_TYPE:
			Data=Valor;
			/*De uPasos A radianes*/
			result=ustep2rad(paxisSettings,(double)Data);
			/*De Radianes A grados*/
			result=rad2grados((double)result);
		break;
		case VELOCIDAD_TYPE:
			Data=Valor-0.5;
			Data=Data/65536.0;
			Data=Data/SAMPLETIME;
			/*De uPasos A radianes*/
			result=ustep2rad(paxisSettings,(double)Data);
			/*De Radianes A grados*/
			result=rad2grados((double)result);
		break;
		case ACELERACION_TYPE:
			Data=Valor-0.5;
			Data=Data/65536.0;
			Data=Data/(SAMPLETIME*SAMPLETIME);
			/*De uPasos A radianes*/
			result=ustep2rad(paxisSettings,(double)Data);
			/*De Radianes A grados*/
			result=rad2grados((double)result);
		break;
	}
 
	return result;
}


int32 gradrosTOuPasos(tAxisSettings *paxisSettings,double angulo,int tipo)
{

	double Radianes,uPasos;
	int32 result;

	result=-1;
	
	Radianes=grados2rad(angulo);
	uPasos=rad2usteps(paxisSettings,Radianes);
	
	switch(tipo) 
	{
		case POSICION_TYPE:		
			result=positionPCtoUC(uPasos);  
		break;
		case VELOCIDAD_TYPE:
			result=velocityPCtoUC(uPasos);  
		break;
		case ACELERACION_TYPE:
			result=accelerationPCtoUC(uPasos);  
		break;
	}
 
	return result;
}


#ifdef _WIN32

/*****************************************************************************************/
double uPasosTOgradroGraficos(tAxisSettings *paxisSettings,int32 BufferData,int TraceVar)
{

	double result,Data;

	switch(TraceVar) 
	{
		case (PMDTracePositionError): 		
		case (PMDTraceCommandedPosition): 	
		case (PMDTraceActualPosition): 
				/*De uPasos A radianes*/
				result=ustep2rad(paxisSettings,(double)BufferData);
				/*De Radianes A grados*/
				result=rad2grados((double)result);
		break;
		case (PMDTraceCommandedVelocity): 
		case (PMDTraceActualVelocity):
			Data=BufferData-0.5;
			Data=Data/65536.0;
			Data=Data/SAMPLETIME;
			/*De uPasos A radianes*/
			result=ustep2rad(paxisSettings,(double)Data);
			/*De Radianes A grados*/
			result=rad2grados((double)result);
        break;
		case (PMDTraceCommandedAcceleration): 
			Data=BufferData-0.5;
			Data=Data/65536.0;
			Data=Data/(SAMPLETIME*SAMPLETIME);
			/*De uPasos A radianes*/
			result=ustep2rad(paxisSettings,(double)Data);
			/*De Radianes A grados*/
			result=rad2grados((double)result);
		break;
		case (PMDTraceTime): 				
		case (PMDTraceCaptureValue): 			
		case (PMDTraceCurrentMotorCommand): 		
		case (PMDTraceEventStatus): 				
		case (PMDTraceActivityStatus): 			
		case (PMDTraceSignalStatus): 			
				result=BufferData;
		break;
	}
 
	return result;
}


#endif
/*****************************************************************************************/

