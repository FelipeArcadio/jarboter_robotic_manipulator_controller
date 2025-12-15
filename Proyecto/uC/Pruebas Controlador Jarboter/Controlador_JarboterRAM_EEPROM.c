#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>    		//salida estandar
#include <inttypes.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "mem.h"
#include "c-motion.h"
#include "PMDsetup.h"
#include "PMDtrace.h"
#include "PMDrobotics.h"
#include "PMDinfo.h"
#include "PMDinterrupt.h"
#include "uart.h"
#include "messages.h"
#include "debug.h"
#include "packetIO.h"
#include "lcd.h"
#include "tw_teach_pendant.h"
#include "tw_rtc.h"
#include "gripper.h"
#include "conversion.h"

void Init_EXRAM (void) __attribute__ ((naked)) \
__attribute__ ((section (".init3")));

/** Inicializacion de la memoria externa */
/** Inicializacion de la memoria externa con dos ciclos de espera para Lectura/Escritura y un ciclo de espera antes de sacar el 
dato. La memoria externa es manejada como un solo sector. */

void Init_EXRAM (void) 
{ 


XMCRA		=	_BV(SRE) | _BV(SRW11) | _BV(SRW10);	
XMCRB = 0x00; 
}

//#define	  DEBUGUSB
#define     DEBUGPUNTO
#define     DEBUGPOSE
//#define     DEBUGFECHA

void work_in(void);
void work_out(void);

volatile int16 		 activo_alarma=0; 		/*Activo alarma mediante un paquete alarma en Work_out*/
volatile int16 		 ATENDER_INT_IN=1; 		/*Se utiliza para no interupir dos veces*/
static 	 tReturnCode rc; 					/*Global para work_in work_out*/
extern   uint16      *id_vector; 			/*Vector de los IDs que se crea mediante return_id_vector(); usado en work_in y work_out */
extern   tAlarm      rxAlarma;   			/*En interrupcion y work_out*/
extern   PMDlong32   *Tracebufferdata; 		/*Usado en work_in y work_out*/
tFecha 				 txFecha; 				/*Actualizada en work_out para luego ser enviada por work_in*/
tAxisSettings 		 TxAxisSettings[DOF]; 	/*Actualizado en work_out y enviado por work_in */
int16                TRACETrajFlag;         /*Le indica a PMDprocessTrajectory si se esta trabajando con Muestreo*/
int8				 GetBufferFlag;         
uint16               TXPoseActual=0;
uint16               GripperActual=0;

/*Para borrar*/
int TEST_EJE=3;


PMDint32 position1, velocity1, commandedposition, commandedvelocity;
PMDint32 commandedacceleration, readencoder1, readerror;
PMDuint32 acceleration1, deceleration;
PMDuint16 cuentas_encoder, micropasos;

extern PMDAxisHandle hAxis[DOF];	


volatile struct
{

  uint8_t USB_int			: 1;			//Bandera cuando interrumpe el USB al uC.
  uint8_t flag_read		    : 1;
  uint8_t flag_write		: 1;
  uint8_t Timer1_int	    : 1;
  uint8_t Timer3_int	    : 1;
}intflags;

//Interrupcion Causada por el Boton de Parada de Emergencia
ISR(INT2_vect)
{
	abruptstop(); 
	buzzer(1); 
	Led_on(Rojo);
	printf("SE ha activo el Boton de Parada de Emergencia. \n");
	fprintf_P(stderr,PSTR("Boton de Parada de Emergencia Activo \n"));
	exit(1);
}

//Interrupcion Causada por La interfaz USB
ISR(INT3_vect)
{	  
    //Solo se pueden atender nuevas interrupciones IN/OUT/PARADA, 
	//si no hay e curso un proceso de comunicacion de datos IN.
	if(ATENDER_INT_IN==1)
	{
		buzzer_ms(50); 
		intflags.USB_int	= 0;
		
		//Comando de Parada de Emergencia		
		if (bit_is_clear(PINH, USB_FLAG_WRITE) && bit_is_clear(PINH, USB_FLAG_READ))
		{			
			abruptstop();
			buzzer(1); 
			printf("SE ha efectudado una parada Abrupta en los motores. \n");
			fprintf(stderr,"Se ha efectudado una parada Abrupta en los motores. \n"); 
			exit(1);			
		}	
		
		//Comando de Lectura al uControlador
		
		if (bit_is_clear(PINH, USB_FLAG_READ))
		{
			intflags.flag_read=0;		
		}
		
		//Comando de Escritura Sobre el uContralador
		
		if (bit_is_clear(PINH, USB_FLAG_WRITE))
		{
			intflags.flag_write=0;			
		}		
	}
}

//Interrupcion Causada por el PMD1
ISR(INT4_vect)
{
	printf("PMD1 PRODUJO INTERRUPCION \n");
	InterruptHandling_chipset1(); 
}

//Interrupcion Causada por el PMD2
ISR(INT5_vect)
{
	printf("PMD2 PRODUJO INTERRUPCION \n"); 
	InterruptHandling_chipset2(); 
}

//Interrupcion Causada por el Timer1
ISR(TIMER1_OVF_vect)
{
	
	intflags.Timer1_int=0;
}

//Interrupcion Causada por el Timer3
ISR(TIMER3_OVF_vect)
{
	intflags.Timer3_int=0;
}
	

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);


int main(void)
{
	int16 numdata,i;
	//Variable Timer
	uint16 conta=0;
	//Variables Fecha
	tFecha Fecha;
	uint8 SR_RTC;
	int16 cont_alarma=0;
	//Inicializacion de las Banderas de Interrupcion;
	intflags.USB_int		=	1;
	intflags.flag_read		=	1;
	intflags.flag_write		=	1;
	intflags.Timer1_int	    =   1;
	intflags.Timer3_int	    =   1;
	
	
	initCard();							//inicializa el Atmega 1280.
	uart_init();						//Inicializa la UART a 9600 Bd, tx/rx, 8N1.	
	lcd_init();
	rtc_init();							//Inicializacion RTC
	stdout = stdin = &uart_str;
	stderr = &lcd_str;
	//Secuencia de pruebas
	//Test_perifericos();
	
	//Led
	Led_on(Amarillo);
	delay_1s();	
	Led_on(Verde);	
	delay_1s();	
	Led_on(Rojo);	
	delay_1s();	
	Led_off(Rojo);
	Led_off(Verde);
	Led_off(Amarillo);
	
	//Verificacion Tarjetas Controladores de Motores.
	
	fprintf_P(stderr,PSTR("Verificando Tarjetas PMDs \n"));
	if(selftestPMD()!=rcOK)
	{
		fprintf_P(stderr,PSTR("Apague el controlador \n"));
		exit(0);
	}
	
	//Configuracion de los PMDs con Valores de Defecto.
	PMDsetupChipSets(PMDTrapezoidalProfile);
	Led_off(Verde);	
	
	//Configuracion de la Memoria RAM externa de los PMDs
	//Memoryconfig();
	
	
	//Verificacion de los Cables de los Sensores y del cable
	//del Boton de Parada de Emergencia
	Led_on(Amarillo);
	fprintf_P(stderr,PSTR("Verificando Conexiones de Cables \n"));
	if(checkconnect()!=rcOK)
	{
		fprintf_P(stderr,PSTR("Apague el controlador \n"));
		exit(0);
	}	
	Led_off(Amarillo);
	
	Led_on(Rojo);
	fprintf_P(stderr,PSTR("Secuencia de HOME \n"));
	//go_home();
	Led_off(Rojo);
	return_id_vector(); /*Obtengo las Poses de la eeprom*/
    numdata=id_vector[0];
    if(numdata)
    {	
		for(i=0;i<numdata;i++)
			printf("main=> vector de poses, id_vector[%d] = %d \n",i,id_vector[i]);		  
	}
	
	EIFR=0xFF;  /*Borro falsas interrupciones al inizializar PMDs, ver pag 79 del manual atemega1280*/   
	HostInterruptEnable();

	sei();		/* enable interrupts */									
	printf("main=> Programa Jarboter Controller. \n");
	fprintf(stderr, "Robot Jarboter Controller.\n");
	Led_on(Verde);
	
	
	for (;;)
	{		
		if(!intflags.USB_int)
		{
		    
			intflags.USB_int	= 1;                     	
			
			if(!intflags.flag_read)			/*llego dato hacia el uC*/
			{
				intflags.flag_read	= 1;	
				Led_off(Verde);								
				Led_on(Amarillo);			/*Amarillo Prendido*/										
			   	work_out(); 
				Led_off(Amarillo);								
				Led_on(Verde);								
				
			}
			if(!intflags.flag_write)		/*El PC pide datos del uC*/						
			{
				intflags.flag_write	= 1;						
				Led_off(Verde);								
				Led_on(Rojo);				/*Rojo Prendido*/
				work_in(); 
				delay_1s();
				Led_off(Rojo);
				Led_on(Verde);								
			}
		}
		if(!intflags.Timer1_int)
		{
			intflags.Timer1_int=1;	
			conta=conta+1;
			if (conta==35)  //5min
			{
				fprintf(stderr, "Robot Jarboter Controller.\n");
				Read_Fecha(&Fecha);
				if((Fecha.hr & 0x20) != 0)	
					fprintf(stderr,"Son las %x:%x:%x PM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
				else 
					fprintf(stderr,"Son las %x:%x:%x AM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
				
				conta=0;
			}
			
		}
		
		if(!intflags.Timer3_int)
		{
			intflags.Timer3_int=1;
			
			if (activo_alarma)
			{
				
				cont_alarma++;
				
				if(cont_alarma==3) //25.165 seg
				{
					cont_alarma=0;
					Read_SR(&SR_RTC);
					if((SR_RTC & AL0Mask)!=0)
					{
						Read_Fecha(&Fecha);						
						activo_alarma=0;
						buzzer(1); 
						printf("main=> Se produjo Alarma=> Ejecutando Tarea programada\n");
						RCT_task(rxAlarma.TASK);												
						fprintf(stderr,"Se produjo Alarma \n");							
						if((Fecha.hr & 0x20) != 0)	
							fprintf(stderr,"a las %x:%x:%x PM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
						else 
							fprintf(stderr,"a las %x:%x:%x AM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
							
					}
				}	
			}
			
		}	
		
	}
	return 0;
}


/************************CUERPO DE LAS FUNCIONES*******************************************/
void work_out(void) 
{
	#ifdef DEBUGPUNTO
	int16   eje,punto;
	#endif

	extern  tFecha        	rxFecha;				/*Contiene la Fecha enviada por el USB*/
	extern  tPacket         rxPacket; 				/*Tiene el Paquete recivido por el USB*/
	extern  tPacketHeader   rxHeader;				/*El header de ese paquete*/	
	extern 	tCommand        rxCommand;				/*Comando enviado por el USB*/
	extern  uint16          *rxvector;				/*Es utilizada para generar una trayectoria mediante IDs*/
	extern  tAxisSettings   rxAxisSettings[DOF];	/*Configuracio enviada por el USB*/
	extern  tTrajectory  	Trajectoryvector;		/*Contiene una trajectoria generada por Ids*/
	extern  tTrajectory 	Trajectoryvectorpunto;	/*Utilizada cuando deseo mover el robot con una pose en memoria*/
	extern  tTrajectory   	rxTrajectory;       	/*trayectoria que llega por USB*/
	static  PoseRobot    	Pose;					/*Estuctura pose que graba los puntos en eeprom*/
	static  int8 			Num_Variables_Trazar=0;	/*Numero de variable a trazar en visual*/
	int16					i,maxdata;				/*Para imprimir el vector de IDs*/
	tcontrol_RTC 			ctr;					/*Manejo del RTC*/
	double grados_Pose;                             /*Almacena pose en grados*/
	
	rc = receiptPacket();
	if(rc != rcOK) 					//Existe algun error?
	{   			
		printf("work_out=> Codigo de retorno al REcibir los Datos %d = %s \n",rc,rcMsg(rc));
		printf("work_out=> Paquete recibido es %s \n",  PacketMsg(rxPacket));
		printf("work_out=> La cantidad de datos  del header.datalength %x \n", rxHeader.dataLength);
		printf("work_out=> dataChecksum  es %x \n",rxHeader.dataChecksum);
		printf("work_out=> headerCheksum %x \n",rxHeader.headerChecksum);
	}
	switch(rxPacket) 
	{   
		case PacketTeachPendant :
		{
		    #ifdef DEBUGPUNTO
			for(eje=0;eje<DOF;eje++)
			{
		        printf("work_out=> El eje es  %s y su pos es %ld \n" ,AxisName((tAxis)eje),rxTrajectory.point[0].axis[eje].position);
			    printf("work_out=> vel  %ld  y su ac  %ld \n" ,rxTrajectory.point[0].axis[eje].velocity,rxTrajectory.point[0].axis[eje].acceleration);
		    }
			printf("work_out=>Invalida Mask= %X \n", rxTrajectory.invalidmask);
			printf("work_out=>Tamano %X \n", rxTrajectory.size);
		    #endif	
			/*Coloco la pose a 0, pues se a movido una poses nueva*/
			TXPoseActual=0;
			
			printf("work_out=> Efectuando funcion teach_pendant_point en PMDrobotics.c  \n");		
		    teach_pendant_point(&rxTrajectory,0);	
			/*Copia la posicion de los MOTORES a la estructura pose en ram*/
			ObtenerPose(&Pose);			    
		break;
		}
		case PacketCommand:
		{
			#ifdef DEBUGUSB 
		    printf("work_out=> el comando es %d => %s \n",rxCommand.CommandOP,CommandMsg(rxCommand));	
			#endif	
			if(rxCommand.CommandOP==CommandOpenGripper)
			{	
				openGripper(); 
				
			
			}
			if(rxCommand.CommandOP==CommandCloseGripper)
			{	
				closeGripper(); 
				
			
			}
			/*Graba la pose en eeprom*/
		    if(rxCommand.CommandOP==CommandSetPoseEEPROM)  			
			{  
			    
				Pose.id=rxCommand.Dato;
				TXPoseActual=Pose.id;
				#ifdef DEBUGPOSE
				printf("work_out=> ID de Pose = %ld \n",rxCommand.Dato);	
			    printf("work_out=> Pose->Pos_num = %d   \n",Pose.id);	
	            printf("work_out=> Pose->Pos_Waist = %ld  \n ",Pose.Pos_Waist);	
	            printf("work_out=> Pose->Pos_Shoulder = %ld  \n ",Pose.Pos_Shoulder);	
	            printf("work_out=> Pose->Pos_Elbow = %ld  \n ",Pose.Pos_Elbow);	
	            printf("work_out=> Pose->Pos_Pitch = %ld  \n ",Pose.Pos_Pitch);	
	            printf("work_out=> Pose->Pos_Roll= %ld  \n ",Pose.Pos_Roll);	
				printf("work_out=> Pose->Gripper_Status =%d \n",Pose.Gripper_Status);
				#endif	
				Pose.Gripper_Status=Estado_gripper();
				//No es posible determinar el estado del gripper por interuptores 
				//Suponer cerrado con pieza..
				if(Pose.Gripper_Status==-1)
				{
					Pose.Gripper_Status=1;
					
				}
				Adicionar_PoseTWI(Pose); 
			}
			/*Sobreescribe una poses existente*/
		    if(rxCommand.CommandOP==CommandReSetPoseEEPROM)  			
			{  
			    
				Pose.id=rxCommand.Dato;
				TXPoseActual=Pose.id;
				#ifdef DEBUGPOSE
				printf("work_out=> ID de Pose = %ld \n",rxCommand.Dato);	
			    printf("work_out=> Pose->Pos_num = %d   \n",Pose.id);	
	            printf("work_out=> Pose->Pos_Waist = %ld  \n ",Pose.Pos_Waist);	
	            printf("work_out=> Pose->Pos_Shoulder = %ld  \n ",Pose.Pos_Shoulder);	
	            printf("work_out=> Pose->Pos_Elbow = %ld  \n ",Pose.Pos_Elbow);	
	            printf("work_out=> Pose->Pos_Pitch = %ld  \n ",Pose.Pos_Pitch);	
	            printf("work_out=> Pose->Pos_Roll= %ld  \n ",Pose.Pos_Roll);
				printf("work_out=> Pose->Gripper_Status =%d \n",Pose.Gripper_Status);
				#endif	
				Pose.Gripper_Status=Estado_gripper();
				//No es posible determinar el estado del gripper por interuptores 
				//suponer con pieza
				if(Pose.Gripper_Status==-1)
				{
					Pose.Gripper_Status=1;
				}
				SobreEscribir_Pose(Pose);
				
			}
			/*Lee la Pose y Se mueve el robot*/
			if(rxCommand.CommandOP==CommandGetPoseEEPROM)
			{
                Leer_PoseTWI(rxCommand.Dato,&Pose);
				#ifdef DEBUGPOSE
				printf("work_out=> Poseleida->Pos_num = %d \n",Pose.id);	
				//cintura
				grados_Pose=uPasosTOgradros(&hAxis[AxisWaist].axisSettings,Pose.Pos_Waist,POSICION_TYPE);
	            printf("work_out=> Poseleida->Pos_Waist = %ld => %f \n ",Pose.Pos_Waist,grados_Pose);
				//hombro
				grados_Pose=uPasosTOgradros(&hAxis[AxisShoulder].axisSettings,Pose.Pos_Shoulder,POSICION_TYPE)-140;
				printf("work_out=> Poseleida->Pos_Shoulder = %ld => %f \n ",Pose.Pos_Shoulder,grados_Pose);		
				//codo
				grados_Pose=uPasosTOgradros(&hAxis[AxisElbow].axisSettings,Pose.Pos_Elbow,POSICION_TYPE)+124;
	            printf("work_out=> Poseleida->Pos_Elbow = %ld => %f \n ",Pose.Pos_Elbow,grados_Pose);		
				//pitch
				grados_Pose=uPasosTOgradros(&hAxis[AxisPitch].axisSettings,Pose.Pos_Pitch,POSICION_TYPE)+170;
				printf("work_out=> Poseleida->Pos_Pitch = %ld => %f \n ",Pose.Pos_Pitch,grados_Pose);		
	            //roll
				grados_Pose=uPasosTOgradros(&hAxis[AxisRoll].axisSettings,Pose.Pos_Roll,POSICION_TYPE);
				printf("work_out=> Poseleida->Pos_Roll= %ld => %f \n ",Pose.Pos_Roll,grados_Pose);		
				#endif
				TXPoseActual=Pose.id;
				Pose2rxTrayectoria(&Pose);
				#ifdef DEBUGPUNTO
				for(eje=0; eje<DOF; eje++)
				{
					printf("work_out=>  El eje es  %s y su pos es %ld \n" ,AxisName((tReturnCode)eje),Trajectoryvectorpunto.point[0].axis[eje].position);
					printf("work_out=>  vel  %ld  y su ac  %ld \n" ,Trajectoryvectorpunto.point[0].axis[eje].velocity,Trajectoryvectorpunto.point[0].axis[eje].acceleration);
				}
				printf("work_out=> Invalida Mask= %X \n", Trajectoryvectorpunto.invalidmask);
				printf("work_out=> Tamano %X \n", Trajectoryvectorpunto.size);
				#endif
				teach_pendant_point(&Trajectoryvectorpunto,0);
				//Nuevo gripper
				if(Pose.Gripper_Status==1)
				{
					if(GripperActual!=1)
					{
						closeGripper();
						GripperActual=1;
					}
					
					//#ifdef DEBUGPOSE
					printf("work_out=> Pose con gripper cerrado\n");	
					//#endif
				}
				if(Pose.Gripper_Status==0)
				{
					openGripper();
					GripperActual=0;
					//#ifdef DEBUGPOSE
					printf("work_out=> Pose con gripper abierto\n");	
					//#endif
				}
			}
			if(rxCommand.CommandOP==CommandEraseEEPROM)
			{
			    printf("work_out=> Borrando memoria eeprom   \n");	
				BorrarEEPROM();
				return_id_vector(); //Actualizo el vector
			}
			if(rxCommand.CommandOP==CommandGetPosesID)
			{				
				return_id_vector();
				#ifdef DEBUGUSB
				printf("work_out=> En el proximo IN mando el  vector de IDs\n");	
				#endif	
			}			
			if(rxCommand.CommandOP==CommandGetPoseActual)
			{				
				#ifdef DEBUGUSB
				printf("work_out=> En el proximo IN mando la poses actual, Poseactual = %d\n",TXPoseActual);	
				#endif	
				
				
			}	
			if(rxCommand.CommandOP==CommandGetFecha)
			{
				#ifdef DEBUGFECHA
			    printf("work_out=> Obtenido Fecha en el uC\n");
				#endif
                Read_Fecha(&txFecha);
				#ifdef DEBUGFECHA
				printf("work_out=> txFecha.sc %x\n",txFecha.sc);
				printf("work_out=> txFecha.mn %x\n",txFecha.mn);
				printf("work_out=> txFecha.hr %x\n",txFecha.hr);
				printf("work_out=> txFecha.mo %x\n",txFecha.mo);
				printf("work_out=> txFecha.yr %x\n",txFecha.yr);
				printf("work_out=> txFecha.dw %x\n",txFecha.dw);
				printf("work_out=> txFecha.y2k %x\n",rxFecha.y2k);
				#endif
				printf("work_out=> Fecha = Hoy es el dia %s %x de %s de 200%x. \n",Dayname(txFecha.dw),txFecha.dt,Monthname(txFecha.mo),txFecha.yr);						
				if((txFecha.hr & 0x20) != 0)			
				    printf("work_out=> Son las %x:%x:%x PM \n",(txFecha.hr & 0x1F),txFecha.mn,txFecha.sc);
				else 
					printf("work_out=> Son las %x:%x:%x AM \n",(txFecha.hr & 0x1F),txFecha.mn,txFecha.sc);
				#ifdef DEBUGUSB
				printf("work_out=> En el proximo IN mando esta Fecha hacia el PC\n");
				#endif		
			    
			}
			if(rxCommand.CommandOP==CommandGetAxisSettings)
			{
			    printf("work_out=> Obteniendo configuracion del robot\n");
				GetAxissetting((uint8*)TxAxisSettings);
				#ifdef DEBUGUSB
				printf("work_out=> En el proximo IN mando la configuracion del robot hacia el PC\n");
				#endif						
			}
			if(rxCommand.CommandOP==CommandSetPMDTrace)
			{
				/*rxCommand.Dato tiene la cantidad de variables a trazar*/
				/*Num_Variables_Trazar Sera igual a rxCommand.Dato si cumple con la restricciones*/
				/*Obtenemos la cantidad de puntos configurados*/
			    printf("work_out=> Configuracion el trazado del robot en los ejes: %d, %d, %d, %d, %d, %d, %d, %d\n",
				rxCommand.Tracecomand[0].AxisTrace,rxCommand.Tracecomand[1].AxisTrace,
				rxCommand.Tracecomand[2].AxisTrace,rxCommand.Tracecomand[3].AxisTrace,				
				rxCommand.Tracecomand[4].AxisTrace,rxCommand.Tracecomand[5].AxisTrace,			
				rxCommand.Tracecomand[6].AxisTrace,rxCommand.Tracecomand[7].AxisTrace);					
				Num_Variables_Trazar=rxCommand.Dato;
				SetupAxisTrace(rxCommand);
				//Bandera si se desea realizar el trazado sobre una trayectoria.
				TRACETrajFlag=rxCommand.TraceTraj;
				GetBufferFlag=1;
				printf("work_out=> Despues de configurar Num_Variables_Trazar = %d\n",Num_Variables_Trazar);
				printf("work_out=> Deseo Trazar Una Trajectoria = %d\n",TRACETrajFlag);
				
			}
			if(rxCommand.CommandOP==CommandGetPMDTrace)
			{   
			    printf("work_out=> Numero Variables a Trazar = %ld\n",rxCommand.Dato);
				/*Copia la informacion a Tracebufferdata*/
				//DisplayTraceResults(AxisWaist,rxCommand.Dato);
				//TraceResultstoBuffer(rxCommand,&GetBufferFlag);
				
			}			
			if(rxCommand.CommandOP==CommandGetMemory)
			{
				i=maxavail();
				printf("work_out=> La cantidad de memoria ram disponible %d \n",i);
				//TEST_EJE=rxCommand.Dato;
			}
			if(rxCommand.CommandOP==CommandGoHome)
			{
				printf("work_out=> Ejecutando Nest \n");
				go_home();
			}
			//---NUEVO--- MAY 28 2010//
			if(rxCommand.CommandOP==CommandGetSensor)
			{
				printf("work_out=> Ejecutando Check_signal_Status \n");		
				printf("work_out=> En el eje %s el estado de los sensores es:\n" ,AxisName((tReturnCode)rxCommand.Axis));
				PMDCheckSignalStatus(rxCommand.Axis,imprimir);  
			}
			if(rxCommand.CommandOP==CommandGetEventStatus)
			{
				printf("work_out=> Ejecutando Check_Event_Status \n");
				printf("work_out=> En el eje %s Event_status es:\n" ,AxisName((tReturnCode)rxCommand.Axis));
				PMDCheckEventStatus(rxCommand.Axis,imprimir);  
			}
			if(rxCommand.CommandOP==CommandGetAvtivityStatus)
			{
				printf("work_out=> Ejecutando Check_Activity_Status \n");
				printf("work_out=> En el eje %s el Activity_Status es:\n" ,AxisName((tReturnCode)rxCommand.Axis));
				PMDCheckActivityStatus(rxCommand.Axis,imprimir);  
				
			}		
			if(rxCommand.CommandOP==CommandGetinfomov)
			{
				infomov(rxCommand.Axis,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
				&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
				&micropasos);	
			}	
			//---NUEVO--- //
		break;
		}
        case PacketTrajectory:
		{
		    #ifdef DEBUGPUNTO
			printf("work_out=> Tamano %X \n", rxTrajectory.size);
			printf("work_out=> Invalida Mask= %X \n", rxTrajectory.invalidmask);
			for(punto=0;punto<rxTrajectory.size;punto++)		  		
			{
				printf("work_out=> Imprimiendo el punto %d \n" ,punto);
		         for(eje=0;eje<DOF;eje++)
		        {	
		         printf("work_out=>  El eje es  %s y su pos es %ld \n" ,AxisName((tReturnCode)eje),rxTrajectory.point[punto].axis[eje].position);
			     printf("work_out=>  vel  %ld  y su ac  %ld \n" ,rxTrajectory.point[punto].axis[eje].velocity,rxTrajectory.point[punto].axis[eje].acceleration);
		        }
				if(punto>10)
				break;
			}
			#endif
			printf("work_out=> Procesando la trayectoria recibida desde robomosp \n");
			PMDprocessTrajectory(&rxTrajectory,TRACETrajFlag);		  
		break;
		}
		case PacketPosesvector:
		{	
			maxdata=rxvector[0];
			#ifdef DEBUGPOSE
			printf("work_out=> maxdata=> %d\n",maxdata);
			
			for(i=0;i<maxdata;i++)
				printf("work_out=> rxvector[%d]=> %d\n",i,rxvector[i]);
			#endif
			Vector2Trajectory(rxvector);
			
			printf("work_out=> Procesando la trayectoria recivida con rxvector \n");
			PMDprocessTrajectory(&Trajectoryvector,TRACETrajFlag);			
		break;
		}
		case PacketFecha:
		{
			#ifdef DEBUGFECHA
			printf("work_out=> rxFecha.sc %x \n",rxFecha.sc);
			printf("work_out=> rxFecha.mn %x \n",rxFecha.mn);
			printf("work_out=> rxFecha.hr %x \n",rxFecha.hr);
			printf("work_out=> rxFecha.dt %x \n",rxFecha.dt);
			printf("work_out=> rxFecha.mo %x \n",rxFecha.mo);
			printf("work_out=> rxFecha.yr %x \n",rxFecha.yr);
			printf("work_out=> rxFecha.dw %x \n",rxFecha.dw);
			printf("work_out=> rxFecha.y2k %x \n",rxFecha.y2k);
			#endif
			printf("work_out=> Ejecutando  write_Fecha \n");			
		    write_Fecha(rxFecha);
			printf("work_out=> Fecha enviada = Hoy es el dia %s %x de %s de 200%x. \n",Dayname(rxFecha.dw),rxFecha.dt,Monthname(rxFecha.mo),rxFecha.yr);						
			if((rxFecha.hr & 0x20) != 0)			
				printf("work_out=> Son las %x:%x:%x PM \n",(rxFecha.hr & 0x1F),rxFecha.mn,rxFecha.sc);
			else 
				printf("work_out=> Son las %x:%x:%x AM \n",(rxFecha.hr & 0x1F),rxFecha.mn,rxFecha.sc);
		break;
		}
		case PacketAlarma:
		{
			#ifdef DEBUGFECHA
			printf("work_out=> rxAlarma.SCA0 %x \n",rxAlarma.SCA0);
			printf("work_out=> rxAlarma.MNA0 %x \n",rxAlarma.MNA0);
			printf("work_out=> rxAlarma.HRA0 %x \n",rxAlarma.HRA0);
			printf("work_out=> rxAlarma.DTA0 %x \n",rxAlarma.DTA0);
			printf("work_out=> rxAlarma.MOA0 %x \n",rxAlarma.MOA0);
			printf("work_out=> rxAlarma.PER %x \n",rxAlarma.PER);
			#endif
			printf("work_out=> Tarea programada => rxAlarma.TASK = %x \n",rxAlarma.TASK);
		    write_alarm(rxAlarma);
			_delay_ms(20);
			read_control(&ctr);
			_delay_ms(20);
			if(rxAlarma.PER)
			{
				ctr.INT=RTC_INT(1,1);//PERIODICA
				activo_alarma=1;
			}
			else
			{	
				ctr.INT=RTC_INT(1,0);//nO PERIODICA
				activo_alarma=1;
			}	
			write_control(ctr);
		break;
		}
		case PacketAxisSettings:
		{	//Debo de reconfigurar los PMDs usando la variable rxAxisSettings[DOF]
			//rxAxisSettings tiene las configuraciones de default con algunos campos cambiados 
			//Segun el usuario
			//Tengo que copiar rxAxisSettings a hAxis[0].axisSettings
			for(i=0; i<DOF; i++) 
				printf("work_out=>  rxAxisSettings[%d].ustepsPerStep= %d \n",i,rxAxisSettings[i].ustepsPerStep);
			//Copio la informacion a la eeprom si es diferente y luego la copia a  hAxis[0].axisSettings
			if(Axissetting2hAxisHandle((uint8*)rxAxisSettings))
			{
				//Reconfiguro con la nueva informacion 
			PMDsetupChipsets_USB(PMDTrapezoidalProfile); 
			}
		break;
		}
	} // switch rxPacket

} // work


void work_in(void)
{
    extern 	tCommand        rxCommand;	
	
	

	
	if(ATENDER_INT_IN==1)
	{
	  
       if(rc != rcOK) 					//Existe algun error?
	    {   
		
		ATENDER_INT_IN=0;  //Para poder imprimier bien tengo que deshabilitar las interupciones por USB.
		printf("work_in=> ERROR\n");
		// Volver a atender la interupciones
		ATENDER_INT_IN=1;  
	    }
	    else
	    {	
			if(rxCommand.CommandOP==CommandGetPosesID)
			{
				ATENDER_INT_IN=0;
				printf("work_in=> Estoy mandando el vector de IDs\n");	
				rc=dispatchPacket(PacketPosesvector,(uint8*)id_vector);  
				printf("work_in=> Salgo del dispatchPacket\n ");			
				ATENDER_INT_IN=1;		
			}
			if(rxCommand.CommandOP==CommandGetFecha)
			{
				ATENDER_INT_IN=0;
				printf("work_in=> Estoy mandando la fecha\n");	
				rc=dispatchPacket(PacketFecha,(uint8*)&txFecha);  
				printf("work_in=> Salgo del dispatchPacket\n ");			
				ATENDER_INT_IN=1;		
			}
			if(rxCommand.CommandOP==CommandGetAxisSettings)
			{
				ATENDER_INT_IN=0;
				printf("work_in=> Estoy mandando la configuracion del robot\n");	
				rc=dispatchPacket(PacketAxisSettings,(uint8*)TxAxisSettings);  
				printf("work_in=> Salgo del dispatchPacket\n ");			
				ATENDER_INT_IN=1;		
			}
			if(rxCommand.CommandOP==CommandGetPMDTrace)
			{
				ATENDER_INT_IN=0;
				//rc=dispatchPacket(PacketTrace,(uint8*)Tracebufferdata);  
				EnvioTrazado2PC(rxCommand.Dato);
				printf("work_in=> Salgo del dispatchPacket\n ");			
				ATENDER_INT_IN=1;		
			}
			if(rxCommand.CommandOP==CommandGetPoseActual)
			{
				ATENDER_INT_IN=0;
				printf("work_in=> Estoy mandando la poses actual, Poseactual = %d\n",TXPoseActual);	
				rc=dispatchPacket(Packetuint16,(uint8*)&TXPoseActual);  
				printf("work_in=> Salgo del dispatchPacket\n ");			
				ATENDER_INT_IN=1;		
			}
	   }
	}
	printf("work_in=> Salgo del IN\n ");			
}


