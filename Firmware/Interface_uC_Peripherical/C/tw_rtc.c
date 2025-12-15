#include <avr/io.h>
#include <stdint.h>
#include <util/twi.h>		
#include <stdio.h>
#include "types.h"
#include "tw_rtc.h"
#include "gripper.h"
#include "packetIO.h"
#include "PMDrobotics.h"
#include "tw_teach_pendant.h"
//#define DEBUG_TAREAS_PROG



/** \ingroup Interface_uC_Peripherical
	\brief Arreglo que contiene los valores que se deben escribir al SR (Status Register) 
	con el fin de realizar una operacion de escritura sobre los registros de control del reloj
	(CCR)*/
	uint8 Conf_SR[2]={0x02,0x06};


/*****************************************************************************************************/
int16 isl1207_write_Conf_SR(void)
{
	uint8 sla,PAGE_SIZE;
	int16 rv;
	uint8 *pWSR;
	
	pWSR= Conf_SR;									/* Apunta a la direccion de la Variable Conf_SR */

	PAGE_SIZE=8;									/* El tamano de Pagina en los registros de Control*/
													/* CCR es de 8 bytes*/
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/
	
	/* Escribir 02h al Status Register */
	rv= write_bytes_I2C(PAGE_SIZE,sla, SR_CCR, 1, pWSR);
	
	if(rv)
	{
		/* Escribir 06h al Status Register */
		pWSR++;
		rv=write_bytes_I2C(PAGE_SIZE,sla, SR_CCR, 1, pWSR);
	}

	return rv;
}

/*****************************************************************************************************/
int16 write_Fecha(tFecha Fecha)
{
	uint8 sla,PAGE_SIZE;
	int16 rv;
	
	PAGE_SIZE=8;									/* El tamano de Pagina en los registros de Control*/
													/* CCR es de 8 bytes*/
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/
	
	rv=isl1207_write_Conf_SR();
	
	if(rv)
	{
		/* 0x30 es el comienzo de la direccion en donde estasn los Registros del RTC*/
		rv= write_bytes_I2C(PAGE_SIZE,sla, 0x30, sizeof(tFecha),(uint8 *)&Fecha);
	}

	return rv;
}
/*****************************************************************************************************/
int16 Read_Fecha(tFecha *Fecha)
{
	uint8 sla;
	int16 rv;
	
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/
	
	/* 0x30 es el comienzo de la direccion en donde estasn los Registros del RTC*/
	rv= read_bytes_I2C(sla, 0x30, sizeof(tFecha), (uint8 *)Fecha);
	
	return rv;
}

/*****************************************************************************************************/
int16 Read_SR(uint8* SR)
{
	uint8 sla;
	int16 rv;
	
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/

	rv= read_bytes_I2C(sla, SR_CCR, sizeof(uint8), (uint8 *)SR);
	
	return rv;
}
/*****************************************************************************************************/
int16 write_control(tcontrol_RTC ctr)
{
	uint8 sla,PAGE_SIZE;
	int16 rv;
	
	PAGE_SIZE=8;									/* El tamano de Pagina en los registros de Control*/
													/* CCR es de 8 bytes*/
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/
	
	rv=isl1207_write_Conf_SR();
	
	if(rv)
	{
		/* 0x10 es el comienzo de la direccion en donde estasn los Registros de control del RTC*/
		rv= write_bytes_I2C(PAGE_SIZE,sla, 0x10, sizeof(tcontrol_RTC),(uint8 *)&ctr);
	}

	return rv;
}
/*****************************************************************************************************/
int16 read_control(tcontrol_RTC *ctr)
{
	uint8 sla;
	int16 rv;
	
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/

	/* 0x10 es el comienzo de la direccion en donde estasn los Registros de control del RTC*/
	rv= read_bytes_I2C(sla, 0x10, sizeof(tcontrol_RTC), (uint8 *)ctr);
	
	return rv;
}
/*****************************************************************************************************/
int16 write_alarm(tAlarm alarma)
{
	uint8 sla,PAGE_SIZE;
	int16 rv,buffersizertc;
	
	PAGE_SIZE=16;									/* El tamano de Pagina en los registros de Alarma*/
													/* es de 16 bytes*/
	sla= TWI_SLA_RTC;								/* Direccion del Dispositivo Esclavo*/
	
	rv=isl1207_write_Conf_SR();
/*Esto es  para guardar solo hasta el campo uint8 MOA0;*/
	buffersizertc=sizeof(tAlarm)-sizeof(uint8)-sizeof(uint8);
	
	if(rv)
	{
		/* 0x00 es el comienzo de la direccion en donde estan los Registros de configuracion de Alarma*/
		rv= write_bytes_I2C(PAGE_SIZE,sla, 0x00, buffersizertc,(uint8 *)&alarma);
	}

	return rv;
}
/*****************************************************************************************************/
int16 read_alarm(tAlarm *alarma)
{
	uint8 sla;
	int16 rv,buffersizertc;
	
	sla= TWI_SLA_RTC;
	buffersizertc=sizeof(tAlarm)-sizeof(uint8)-sizeof(uint8);								/* Direccion del Dispositivo Esclavo*/

	/* 0x00 es el comienzo de la direccion en donde estasn los Registros de configuracion de Alarma*/
	rv= read_bytes_I2C(sla, 0x00, buffersizertc, (uint8 *)alarma);
	
	return rv;
}

/*****************************************************************************************************/
void rtc_init(void)
{
	tcontrol_RTC ctr;
	
	ctr.PWR=RTC_PWR(1,1);
	ctr.DTR=0;
	ctr.ATR=0;
	ctr.INT=RTC_INT(0,0);
	ctr.BL =0x18;
	write_control(ctr);
	
}
/*****************************************************************************************************/
void RCT_task(uint8 TASK)
{
	extern int16   FlagrxVectorok;
	extern uint16  *rxvector;
	extern tTrajectory  Trajectoryvector;
	int maxdata;
	#ifdef DEBUG_TAREAS_PROG
	int i;
	#endif
  
	switch (TASK)
	{
		case (TAST_Opengripper):
			openGripper();  
		break;
		
		case (TAST_Closegripper):
			closeGripper();
		break;
		
		case (TAST_IDVectorTrajectory):
			#ifdef DEBUG_TAREAS_PROG
			printf("RCT_task=> FlagrxVectorok %d\n",FlagrxVectorok);
			#endif
			if(FlagrxVectorok)
			{
				maxdata=rxvector[0];
				#ifdef DEBUG_TAREAS_PROG				
				printf("RCT_task=>  maxdata= %d\n",maxdata);
				for(i=0;i<maxdata;i++)
				
					printf("RCT_task=> rxvector[%d]= %d\n",i,rxvector[i]);
				#endif
				printf("RCT_task=> Ejecutando TAST_IDVectorTrajectory\n");
				
				/*Copiar el rxvector que se mando en la ultima transferencia
				utilizando los datos de la eeprom*/
				Vector2Trajectory(rxvector);
				
				/*utilizo esos datos para ejecutar la trajectoria*/
				PMDprocessTrajectory(&Trajectoryvector,0);
			}
			else 
				printf("RCT_task=> No hay una trajectoryVector Valido\n");
			
		break;
		
		case (TAST_RunlasrxrTrajectory):
		//Falta ver que hacer por que estamos borrando rxtrayectoria despues de utilizarlar.    
		break;
		
		default:  
		break;
	}
}


