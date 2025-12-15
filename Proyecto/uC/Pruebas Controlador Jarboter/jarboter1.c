#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <inttypes.h>

#include "c-motion.h"
#include "tw_teach_pendant.h"
#include "tw_rtc.h"
#include "uart.h"
#include "lcd.h"


#include "PMDsetup.h"
#include "PMDrobotics.h"
#include "PMDtrace.h"
#include "PMDinfo.h"
#include "PMDinterrupt.h"
#include "gripper.h"


#include "debug.h"
#include "messages.h"
#include "packetIO.h"

#define		DEBUGAVR
//#define		DEBUG
#define		TRAPEZOIDAL



void Init_EXRAM (void) 
{ 
	XMCRA		=	_BV(SRE) | _BV(SRW11) | _BV(SRW10);	
	XMCRB 		= 	0x00; 
}

void Init_EXRAM (void) __attribute__ ((naked)) \
__attribute__ ((section (".init3")));

//Variable Timer
	volatile uint16 conta=0; //No estaba voalite, Modificado por david 2014
	volatile uint16 tiempo_motor_stop;
	
//Variables Fecha
	tFecha Fecha;
	uint8 SR_RTC;
	tcontrol_RTC ctr;
	tAlarm alarma;
	int16 sbib,bsw,dtr,atr,al,im,ask,scn;
	int16 activo_alarma,cont_alarma=0;

ISR(INT2_vect)
{
	TIM16_ReadTCNTn(); //coloco a cero el timer 1
	abruptstop();      //Paro los motores
	tiempo_motor_stop=TIM16_ReadTCNTn(); //Retorno el tiempo 
	buzzer(1);	
	printf("Se ha efectuadado una parada Abrupta en los motores. \n");
	printf("Registro Timer=%x Tiempo=%f seg.\n",tiempo_motor_stop,(double)tiempo_motor_stop*0.000128);
	fprintf(stderr,"Se ha efectuadado una parada Abrupta en los motores. \n"); 
	exit(1);
}


ISR(INT4_vect)
{
	printf("PMD1 PRODUJO INTERRUPCION \n"); 
	InterruptHandling_chipset1();
}

ISR(INT5_vect)
{
	printf("PMD2 PRODUJO INTERRUPCION \n"); 
	InterruptHandling_chipset2();
}

ISR(TIMER1_OVF_vect)
{
	conta++;
	
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

ISR(TIMER3_OVF_vect)
{
	
	if (activo_alarma)
	{
		
		cont_alarma++;
		
		if(cont_alarma==3) //30 seg aprox
		{
			cont_alarma=0;
			Read_SR(&SR_RTC);
			if((SR_RTC & AL0Mask)!=0)
			{
				activo_alarma=0;
				buzzer(1); 
				fprintf(stderr,"Se produjo Alarma \n");	
				Read_Fecha(&Fecha);
				if((Fecha.hr & 0x20) != 0)	
					fprintf(stderr,"a las %x:%x:%x PM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
				else 
					fprintf(stderr,"a las %x:%x:%x AM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
			}
		}	
	}

}
	


FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

extern uint16 *id_vector; 
	


int
main(void)
{
	//Variables Menu
	int16 menu,articulacion,submenu,stop;	
	//Variables de Movimiento
	PMDint32 position,velocity,aceleration;	
	//Variables Informacion de Movimiento
	PMDint32 position1, velocity1, commandedposition, commandedvelocity,commandedacceleration, readencoder1, readerror;
 	PMDuint32 acceleration1, deceleration;
	PMDuint16 cuentas_encoder, micropasos;
	int16 P_ajuste;	
	//Variables Calibracion-Ajuste-Breakpoint	
	PMDint32 relacion;
	int16 eje_break,Breakpoint_id,source_Axis,accion,trigger;
	PMDint32 value;
	//Variable para la prueba funcion Search Positive Roll Home
	PMDint32 home;

	
	
	
	//Variables EEPROM
	int16 memory[10]={2,4,6,8,10,12,14,16,18,20};
	int16 memory1[10],i;
	uint16 id,id_pose;
	int16 Addr;
	
	//Variables Pose Robot
	PoseRobot Pose;
	int16 rv,num_datos;
	
	//Variables Prueba Get_Trajectory
	
	//uint16 id_vector_PC[5] ={5,2,3,7,8};
	
	initCard();							//inicializa el Atmega 1280.
	uart_init();						//Inicializa la UART a 9600 Bd, tx/rx, 8N1.
	lcd_init();
	rtc_init();							//Inicializacion RTC	
	stdout = stdin = &uart_str;
	stderr = &lcd_str;
	
	Led_on(Amarillo);	
	delay_1s();	
	Led_on(Verde);	
	delay_1s();	
	Led_on(Rojo);	
	delay_1s();	
	Led_off(Rojo);
	Led_off(Verde);
	Led_off(Amarillo);
	
	
	
	if(selftestPMD()!=rcOK)
	{
		fprintf_P(stderr,PSTR("Apague el controlador \n"));
		exit(0);
	}
	
	
	
	#ifdef TRAPEZOIDAL
		PMDsetupChipSets(PMDTrapezoidalProfile);
	#else
		PMDsetupChipSets(PMDVelocityContouringProfile);
	#endif
	
#ifdef TRAPEZOIDAL
	Memoryconfig();
	checkconnect();	
	go_home();
	
   EIFR=0xFF;   //Borro flasas interrupnes al inizializar PMDs, ver pag 79 del manual atemega1280
	             //Para evitar el problema de la interrupcion al inicializar el controlador
					 
/**	 enable interrupts 	*/

	HostInterruptEnable();
	sei();
	
	fprintf(stderr, "Robot Jarboter Controller.\n");


	//Se resetea el Event Status Register en todos los ejes y se borra el Error
	
	
	Clear_Position_Error();	
	Reset_Even_Status_Register();
	_delay_ms(100);
	Reset_Even_Status_Register();
	
	
	for (;;)
	{  
		fprintf_P(stdout,PSTR("/n /n PROGRAMA DE PRUEBAS Y DEPURACION .\n"));
		fprintf_P(stdout,PSTR("Tesis de Grado JARBOTER V2.1. \n \n"));
		
		fprintf_P(stdout,PSTR("0.Funciones de Movimiento\n"));
		fprintf_P(stdout,PSTR("1.Funciones de informacion de Movimiento\n"));
		fprintf_P(stdout,PSTR("2.Funciones de Calibracion-Ajuste-Breakpoint\n"));
		fprintf_P(stdout,PSTR("3.Funciones del Reloj de Tiempo Real \n"));
		fprintf_P(stdout,PSTR("4.Funciones de Manejo De EEPROM\n"));
		fprintf_P(stdout,PSTR("5.Funciones de Trazo e Impresion de Variables \n"));
		fprintf_P(stdout,PSTR("6.Reset Event Status Register \n"));
		fprintf_P(stdout,PSTR("7.Funciones de Gripper \n"));
		fprintf_P(stdout,PSTR("8.Funciones para DEBUG \n"));
		fprintf_P(stdout,PSTR("Otra Opcion: Parada Suave \n"));	
		scanf("%d",&menu);		
		
		switch (menu)
		{
			case (0)://FUNCIONES DE MOVIMIENTO
				fprintf_P(stdout,PSTR("Digite Articulacion a mover \n"));
				fprintf_P(stdout,PSTR("0. Cintura. \n"));
				fprintf_P(stdout,PSTR("1. Hombro. \n"));
				fprintf_P(stdout,PSTR("2. Codo. \n"));
				fprintf_P(stdout,PSTR("3. Pitch. \n"));
				fprintf_P(stdout,PSTR("4.Roll. \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&articulacion);
				
				if(articulacion >=0 && articulacion <=4)
				{				
					fprintf_P(stdout,PSTR("Digite Posicion? \n"));
					scanf("%ld", &position);				
					fprintf_P(stdout,PSTR("Digite Velocidad? \n"));
					scanf("%ld", &velocity);			
					fprintf_P(stdout,PSTR("Digite aceleracion? \n"));
					scanf("%ld", &aceleration);
				}
				
				switch (articulacion)
				{
					case (0):
				    mov_axis(articulacion,position,velocity,aceleration,update);
				    break;
					
					case (1):
				    mov_axis(articulacion,position,velocity,aceleration,update);
				    break;
					
					case (2):
				    mov_axis(articulacion,position,velocity,aceleration,update);
				    break;
					
					case (3):
				    pitch(position,velocity,aceleration,update,MOVIMIENTO_NORMAL);
				    break;
					
				    case (4):
				    roll(position,velocity,aceleration,update,MOVIMIENTO_NORMAL);
				    break;
					
				    default:
				    break;
				}
				
				/*do
				{
					error_posicion=readError(a);
					status=PMDCheckEventStatus(a,no_imprimir);
					printf("El movimiento no ha sido considerado completo \n");
					printf("Error Posicion=	%08ld\n",error_posicion);			
				}
				while ((status & PMDEventMotionCompleteMask)==0);
				printf("El movimiento se completo \n");*/
				
				motioncomplete(articulacion);					
			break;
			
		    case (1)://FUNCIONES DE INFORMACION DE MOVIMIENTO
				fprintf_P(stdout,PSTR("0. Informacion Movimiento\n"));
				fprintf_P(stdout,PSTR("1. Lectura del Event Status Register \n"));
				fprintf_P(stdout,PSTR("2. Lectura del Activity Status Register \n"));
				fprintf_P(stdout,PSTR("3. Signal Status \n"));
				fprintf_P(stdout,PSTR("4. Ajustar Posicion \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);
				
				if(submenu >=0 && submenu<=4)
				{
					fprintf_P(stdout,PSTR("Digite la articulacion que quiere Leer o Ajustar\n"));
					fprintf_P(stdout,PSTR("0. Cintura. \n"));
					fprintf_P(stdout,PSTR("1. Hombro. \n"));
					fprintf_P(stdout,PSTR("2. Codo. \n"));
					fprintf_P(stdout,PSTR("3. Pitch. \n"));		
					fprintf_P(stdout,PSTR("4.Roll. \n"));
					scanf("%d",&articulacion);
				}
				
				switch (submenu)
				{
					case (0):
						infomov(articulacion,&position1, &velocity1, &acceleration1, &deceleration, 
								&commandedposition,	&commandedvelocity, &commandedacceleration, 
								&readencoder1, &readerror,&cuentas_encoder,&micropasos);
						printf("la informacion correspondiente del movimiento del eje %s es :\n",AxisName(articulacion));
						printf("posicion=	%08ld\n",position1);
						printf("velodidad	= 	%08ld\n",velocity1);
						printf("aceleracion=	%08ld\n",acceleration1);
						printf("desaceleracion=	%08ld\n",deceleration);
						printf("posicion instantanea=	%08ld\n",commandedposition);
						printf("velodidad instantanea=	%08ld\n",commandedvelocity);
						printf("aceleracion instantanea=	%08ld\n",commandedacceleration);
						printf("Lectura Encoder=	%08ld\n",readencoder1);
						printf("Error Posicion=	%08ld\n",readerror);	
						printf("Cuentas de Encoder= %08d \n",cuentas_encoder);
						printf("No. uPasos por vuelta= %08d \n",micropasos);	
						scanf("%d",&stop);		
								
								
				    break;
					
				    case (1):
						PMDCheckEventStatus(articulacion,imprimir);
						scanf("%d",&stop);	
				    break;
					
					case (2):
						PMDCheckActivityStatus(articulacion,imprimir);
						scanf("%d",&stop);	
				    break;
					
					case (3):
						PMDCheckSignalStatus(articulacion,imprimir);
						scanf("%d",&stop);	
				    break;
					
					case (4):
						fprintf_P(stdout,PSTR("Digite el valor de la posicion a ajustar \n"));
						scanf("%d",&P_ajuste);
						ajustar_posicion(articulacion,P_ajuste);
				    break;
					
					default:
					break;
					
				}		
				
				
			
			break;
			
			case (2)://FUNCIONES DE CALIBRACION - AJUSTE - BREAKPOINT
				fprintf_P(stdout,PSTR("0. Realizar Proceso de Calibracion \n"));
				fprintf_P(stdout,PSTR("1. Hallar Nest y max y min posicion \n"));
				fprintf_P(stdout,PSTR("2. Configurar BreakPoint \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);
				
				if(submenu >=0 && submenu<=1)
				{				
					fprintf_P(stdout,PSTR("Digite la articulacion a configurar \n"));
					fprintf_P(stdout,PSTR("0. Cintura. \n"));
					fprintf_P(stdout,PSTR("1. Hombro. \n"));
					fprintf_P(stdout,PSTR("2. Codo. \n"));
					fprintf_P(stdout,PSTR("3. Pitch. \n"));		
					fprintf_P(stdout,PSTR("4.Roll. \n"));
					scanf("%d",&articulacion);
				}
				
				switch (submenu)
				{
					case (0):
						relacion=calibration(articulacion);
						printf("la relacion de pasos a motor en el eje %s es= %08ld \n",AxisName(articulacion),relacion);		
				    break;
					
				    case (1):
						initializeAxis(articulacion);
				    break;
					
					case (2):						
						fprintf_P(stdout,PSTR("Digite el eje el cual configurara el BreakPoint \n"));
						fprintf_P(stdout,PSTR("0. Cintura. \n"));
						fprintf_P(stdout,PSTR("1. Hombro. \n"));
						fprintf_P(stdout,PSTR("2. Codo. \n"));
						fprintf_P(stdout,PSTR("3. Pitch. \n"));		
						fprintf_P(stdout,PSTR("4.Roll. \n"));
						scanf("%d",&eje_break);			
						
						fprintf_P(stdout,PSTR("Digite el  BreakPoint_ID \n"));
						fprintf_P(stdout,PSTR("0. BreakPoint1. \n"));
						fprintf_P(stdout,PSTR("1. BreakPoint2. \n"));
						scanf("%d",&Breakpoint_id);			
						
						fprintf_P(stdout,PSTR("Digite el Eje Fuente (source) del BreakPoint \n"));
						fprintf_P(stdout,PSTR("0. Cintura. \n"));
						fprintf_P(stdout,PSTR("1. Hombro. \n"));
						fprintf_P(stdout,PSTR("2. Codo. \n"));
						fprintf_P(stdout,PSTR("3. Pitch. \n"));		
						fprintf_P(stdout,PSTR("4.Roll. \n"));
						scanf("%d",&source_Axis);			
						
						fprintf_P(stdout,PSTR("Digite la accion del BreakPoint \n"));
						fprintf_P(stdout,PSTR("0.Ninguna Accion. \n"));
						fprintf_P(stdout,PSTR("1. Update. \n"));
						fprintf_P(stdout,PSTR("2. AbruptStop. \n"));
						fprintf_P(stdout,PSTR("3. SmoothStop. \n"));
						fprintf_P(stdout,PSTR("4.Motor_OFF. \n"));
						scanf("%d",&accion);			
						
						fprintf_P(stdout,PSTR("Digite el TRIGGER del BreakPoint \n"));
						fprintf_P(stdout,PSTR("0. Ninguno. \n"));
						fprintf_P(stdout,PSTR("1. GreaterOrEqualCommandedPosition. \n"));
						fprintf_P(stdout,PSTR("2. LesserOrEqualCommandedPosition. \n"));
						fprintf_P(stdout,PSTR("3. GreaterOrEqualActualPosition. \n"));
						fprintf_P(stdout,PSTR("4. LesserOrEqualActualPosition. \n"));
						fprintf_P(stdout,PSTR("5. CommandedPositionCrossed. \n"));
						fprintf_P(stdout,PSTR("6. ActualPositionCrossed. \n"));
						fprintf_P(stdout,PSTR("7. Time. \n"));
						fprintf_P(stdout,PSTR("8. EventStatus. \n"));
						fprintf_P(stdout,PSTR("9. ActivityStatus. \n"));
						fprintf_P(stdout,PSTR("10.SignalStatus. \n"));
						scanf("%d",&trigger);			
						
						fprintf_P(stdout,PSTR("Digite el valor del Trigger \n"));
						scanf("%ld",&value);			
						
						Config_BreakPoint(eje_break, Breakpoint_id, source_Axis, accion, trigger,value);
					break;
						
					default:
					break;
				}	
			break;
			
			case (3)://FUNCIONES DEL RELOJ DE TIEMPO REAL
				fprintf_P(stdout,PSTR("0. Escribir Fecha \n"));
				fprintf_P(stdout,PSTR("1. Escribir en el Control de Registros del RTC \n"));
				fprintf_P(stdout,PSTR("2. Configurar Alarma \n"));
				fprintf_P(stdout,PSTR("3. Leer Fecha \n"));
				fprintf_P(stdout,PSTR("4. Leer El Status Register del RTC \n"));
				fprintf_P(stdout,PSTR("5. Leer el Control de Registros del RTC \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);
				
				switch (submenu)
				{
					case (0):
						//Configuro el ano 20 (para 2000)
						Fecha.y2k=0x20;			
						fprintf_P(stdout,PSTR("Digite el Valor en Segundos. 0-59 \n"));
						scanf("%x",(int16 *)&Fecha.sc);			
						fprintf_P(stdout,PSTR("Digite el Valor en Minutos. 0-59 \n"));
						scanf("%x",(int16 *)&Fecha.mn);			
						fprintf_P(stdout,PSTR("Digite el Valor de la Hora  \n"));
						scanf("%x",(int16 *)&Fecha.hr);			
						fprintf_P(stdout,PSTR("0.AM  \n"));
						fprintf_P(stdout,PSTR("1.PM  \n"));
						scanf("%d",&ask);			
						if(ask==1)
							Fecha.hr= Fecha.hr | 0x20;			
						fprintf_P(stdout,PSTR("Digite el Dia. 1-31 \n"));
						scanf("%x",(int16 *)&Fecha.dt);			
						fprintf_P(stdout,PSTR("Digite el Mes. 1-12 \n"));
						scanf("%x",(int16 *)&Fecha.mo);			
						fprintf_P(stdout,PSTR("Digite el Ano. 0-99 \n"));
						scanf("%x",(int16 *)&Fecha.yr);			
						fprintf_P(stdout,PSTR("Digite el dia de la Semana. \n"));
						fprintf_P(stdout,PSTR("0. Domingo \n"));
						fprintf_P(stdout,PSTR("1. Lunes \n"));
						fprintf_P(stdout,PSTR("2. Martes \n"));
						fprintf_P(stdout,PSTR("3. Miercoles \n"));
						fprintf_P(stdout,PSTR("4. Jueves \n"));
						fprintf_P(stdout,PSTR("5. Viernes \n"));
						fprintf_P(stdout,PSTR("6. Sabado \n"));
						scanf("%x",(int16 *)&Fecha.dw);			
						write_Fecha(Fecha);
				    break;
					
					case (1):
						fprintf_P(stdout,PSTR("Digite si Habilitara la Interface Serial en Modo de Battery Backup. \n"));
						fprintf_P(stdout,PSTR("0. Habilitar. \n"));
						fprintf_P(stdout,PSTR("1. Deshabilitar. \n"));
						scanf("%d", &sbib);			
						fprintf_P(stdout,PSTR("Digite la condicion de cambio entre VDD y Battery Backup. \n"));
						fprintf_P(stdout,PSTR("0. Standard. \n"));
						fprintf_P(stdout,PSTR("1. Legacy/Defecto. \n"));
						scanf("%d", &bsw);			
						fprintf_P(stdout,PSTR("Digite un Valor del Digital TRimming Register. \n"));
						fprintf_P(stdout,PSTR("0. Frecuencia Estimada (PPM) = 0 \n"));
						fprintf_P(stdout,PSTR("1. Frecuencia Estimada (PPM) = +20 \n"));
						fprintf_P(stdout,PSTR("2. Frecuencia Estimada (PPM) = +10 \n"));
						fprintf_P(stdout,PSTR("3. Frecuencia Estimada (PPM) = +30 \n"));
						fprintf_P(stdout,PSTR("5. Frecuencia Estimada (PPM) = -20 \n"));
						fprintf_P(stdout,PSTR("6. Frecuencia Estimada (PPM) = -10 \n"));
						fprintf_P(stdout,PSTR("7. Frecuencia Estimada (PPM) = -30 \n"));			
						scanf("%x", &dtr);			
						fprintf_P(stdout,PSTR("Digite un Valor para la compensacion de la capacitancia. \n"));
						scanf("%d", &atr);			
						fprintf_P(stdout,PSTR("Habilitar Alarma. \n"));
						fprintf_P(stdout,PSTR("1. Habilitar. \n"));
						fprintf_P(stdout,PSTR("0. Deshabilitar. \n"));
						scanf("%d", &al);			
						fprintf_P(stdout,PSTR("Alarma Periodica. \n"));
						fprintf_P(stdout,PSTR("0. No Periodica. \n"));
						fprintf_P(stdout,PSTR("1. Periodica. \n"));
						scanf("%d", &im);			
						ctr.PWR=RTC_PWR(sbib,bsw);
						ctr.DTR=dtr;
						ctr.ATR=atr;
						ctr.INT=RTC_INT(al,im);
						ctr.BL =0x18;
						write_control(ctr);
				    break;
					
					case (2):
						fprintf_P(stdout,PSTR("Desea Habiliar un Valor del Mes para la Alarma?. \n"));			
						fprintf_P(stdout,PSTR("0. NO \n"));
						fprintf_P(stdout,PSTR("1. SI \n"));
						scanf("%d",&ask);						
						if(ask==1)
						{
							fprintf_P(stdout,PSTR("Digite el Mes. 1-12 \n"));
							scanf("%x",&scn);
							alarma.MOA0= (uint8)scn;
							alarma.MOA0=alarma.MOA0 | 0x80;						//0x80 habilita el mes
						}
						else
							alarma.MOA0=0x00;				
						fprintf_P(stdout,PSTR("Desea Habiliar un Valor del DIA para la Alarma?. \n"));			
						fprintf_P(stdout,PSTR("0. NO \n"));
						fprintf_P(stdout,PSTR("1. SI \n"));
						scanf("%d",&ask);
						if(ask==1)
						{
							fprintf_P(stdout,PSTR("Digite el Dia. 1-31 \n"));
							scanf("%x",&scn);
							alarma.DTA0=(uint8)scn;
							alarma.DTA0=alarma.DTA0 | 0x80;						//0x80 habilita el dia
						}
						else
							alarma.DTA0=0x00;							
						fprintf_P(stdout,PSTR("Desea Habiliar el valor de la Hora para la Alarma?. \n"));			
						fprintf_P(stdout,PSTR("0. NO \n"));
						fprintf_P(stdout,PSTR("1. SI \n"));
						scanf("%d",&ask);
						if(ask==1)
						{
							fprintf_P(stdout,PSTR("Digite el Valor de la Hora \n"));
							scanf("%x",&scn);
							alarma.HRA0=(uint8)scn;
							alarma.HRA0=alarma.HRA0 | 0x80;						//0x80 habilita la Hora
							
							fprintf_P(stdout,PSTR("0.AM  \n"));
							fprintf_P(stdout,PSTR("1.PM  \n"));
							scanf("%d",&ask);
							
							if(ask==1)
								alarma.HRA0= alarma.HRA0 | 0x20;
						}
						else
							alarma.HRA0=0x00;						
						fprintf_P(stdout,PSTR("Desea Habiliar el valor de los minutos para la Alarma?. \n"));			
						fprintf_P(stdout,PSTR("0. NO \n"));
						fprintf_P(stdout,PSTR("1. SI \n"));
						scanf("%d",&ask);
						if(ask==1)
						{
							fprintf_P(stdout,PSTR("Digite el Valor de los Minutos. 0-59 \n"));
							scanf("%x",&scn);
							alarma.MNA0=(uint8)scn;
							alarma.MNA0=alarma.MNA0 | 0x80;						//0x80 habilita Los minutos
						}
						else
							alarma.MNA0=0x00;							
						fprintf_P(stdout,PSTR("Desea Habiliar el valor de los segundos para la Alarma?. \n"));			
						fprintf_P(stdout,PSTR("0. NO \n"));
						fprintf_P(stdout,PSTR("1. SI \n"));
						scanf("%d",&ask);
						if(ask==1)
						{
							fprintf_P(stdout,PSTR("Digite el Valor en segundos. 0-59 \n"));
							scanf("%x",&scn);
							alarma.SCA0=(uint8)scn;
							alarma.SCA0=alarma.SCA0 | 0x80;						//0x80 habilita Los segundos
						}
						else
							alarma.SCA0= 0x00;							
						write_alarm(alarma);
						_delay_ms(20);
						
						read_control(&ctr);
						_delay_ms(20);
						
						fprintf_P(stdout,PSTR("Alarma Periodica. \n"));
						fprintf_P(stdout,PSTR("0. No Periodica. \n"));
						fprintf_P(stdout,PSTR("1. Periodica. \n"));
						scanf("%d", &ask);
						
						if(ask==1)
						{
							ctr.INT=RTC_INT(1,1);
							activo_alarma=1;
						}
						else
						{	
							ctr.INT=RTC_INT(1,0);
							activo_alarma=1;
						}							
						write_control(ctr);
						_delay_ms(20);
						
						read_control(&ctr);
						printf("PWR = %x \n",ctr.PWR);
						printf("DTR = %x \n",ctr.DTR);
						printf("ATR = %x \n",ctr.ATR);
						printf("INT = %x \n",ctr.INT);
						printf("BL  = %x \n",ctr.BL);
						scanf("%d",&stop);		
						
						read_alarm(&alarma);
						printf("MES = %x \n",alarma.MOA0);
						printf("DIA = %x \n",alarma.DTA0);
						printf("HORA = %x \n",alarma.HRA0);
						printf("MIN = %x \n",alarma.MNA0);
						printf("SEC = %x \n",alarma.SCA0);
						
						scanf("%d",&stop);	
				    break;
					
					case (3):
						Read_Fecha(&Fecha);
						printf("Fecha = Hoy es el dia %s %x de %s de 200%x. \n",Dayname(Fecha.dw),Fecha.dt,Monthname(Fecha.mo),Fecha.yr);						
						if((Fecha.hr & 0x20) != 0)			
							printf("Son las %x:%x:%x PM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
						else 
							printf("Son las %x:%x:%x AM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
						scanf("%d",&stop);  
				    break;
					
					case (4):
						Read_SR(&SR_RTC);
							if((SR_RTC & BATMask) != 0)
								fprintf_P(stdout,PSTR("BAT = 1 \n"));
							else
								fprintf_P(stdout,PSTR("BAT = 0 \n"));								
							
							if((SR_RTC & AL1Mask) != 0)
								fprintf_P(stdout,PSTR("AL1 = 1 \n"));
							else
								fprintf_P(stdout,PSTR("AL1 = 0 \n"));								
							
							if((SR_RTC & AL0Mask) != 0)
								fprintf_P(stdout,PSTR("Al0 = 1 \n"));
							else
								fprintf_P(stdout,PSTR("Al0 = 0 \n"));								
							
							if((SR_RTC & OSCFMask) != 0)
								fprintf_P(stdout,PSTR("OSCF = 1 \n"));
							else
								fprintf_P(stdout,PSTR("OSCF = 0 \n"));
								
							if((SR_RTC & RWELMask) != 0)
								fprintf_P(stdout,PSTR("RWEL = 1 \n"));
							else
								fprintf_P(stdout,PSTR("RWEL = 0 \n"));								
								
							if((SR_RTC & WELMask) != 0)
								fprintf_P(stdout,PSTR("WEL = 1 \n"));
							else
								fprintf_P(stdout,PSTR("WEL = 0 \n"));								
								
							if((SR_RTC & RTCFMask) != 0)
								fprintf_P(stdout,PSTR("RTCF = 1 \n"));
							else
								fprintf_P(stdout,PSTR("RTCF = 0 \n"));							
						
						printf("Status Register = %#X \n",SR_RTC);
						scanf("%d",&stop);		
				    break;
					
					case (5):
						read_control(&ctr);
						printf("PWR = %x \n",ctr.PWR);
						printf("DTR = %x \n",ctr.DTR);
						printf("ATR = %x \n",ctr.ATR);
						printf("INT = %x \n",ctr.INT);
						printf("BL  = %x \n",ctr.BL);
						scanf("%d",&stop);		
				    break;
					
					default:
					break;				    
				}				
			break;			
			
			case (4)://FUNCIONES DE MANEJO DE EEPROM Y TEACH PENDANT
				fprintf_P(stdout,PSTR("0. Escribir en la EEPROM \n"));
				fprintf_P(stdout,PSTR("1. Leer la EEPROM \n"));
				fprintf_P(stdout,PSTR("2. Encontrar Direccion de ID \n"));
				fprintf_P(stdout,PSTR("3. Borrar EEPROM \n"));
				fprintf_P(stdout,PSTR("4. Adicionar Pose en EEPROM \n"));
				fprintf_P(stdout,PSTR("5. Sobreescribir Pose \n"));
				fprintf_P(stdout,PSTR("6. Leer Pose almacenada en EEPROM \n"));				
				fprintf_P(stdout,PSTR("7. Retornar Vector de Ids \n"));
				fprintf_P(stdout,PSTR("8. Obtener Trayectoria de un Vector de IDs \n"));
				fprintf_P(stdout,PSTR("9. Imprimir Datos de Trayectoria \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);
				
				if(submenu==4 || submenu==5)
				{
					fprintf_P(stdout,PSTR("Digite el ID de la Pose que desea grabar . \n"));
					fprintf_P(stdout,PSTR("Digite el ID de la Pose que desea grabar . \n"));
					scanf("%d", &id_pose);			
					Pose.id=id_pose;			
					fprintf_P(stdout,PSTR("Digite la posicion de cintura . \n"));
					scanf("%ld", &position);
					Pose.Pos_Waist=position;			
					fprintf_P(stdout,PSTR("Digite la posicion de Hombro . \n"));
					scanf("%ld", &position);
					Pose.Pos_Shoulder=position;			
					fprintf_P(stdout,PSTR("Digite la posicion de Codo . \n"));
					scanf("%ld", &position);
					Pose.Pos_Elbow=position;			
					fprintf_P(stdout,PSTR("Digite la posicion de Pitch . \n"));
					scanf("%ld", &position);
					Pose.Pos_Pitch=position;			
					fprintf_P(stdout,PSTR("Digite la posicion de Roll . \n"));
					scanf("%ld", &position);
					Pose.Pos_Roll=position;				
				}
				
				switch (submenu)
				{
					case (0):
						ee24xx_write_bytes(0, sizeof(int16)*10, (uint8 *)memory);
						for (i=0; i< 10;i++)
						{
							printf(" El valor %d de Memory es = %d\n",i,memory[i]);							
						}
						scanf("%d",&stop);
				    break;
					
					case (1):
						ee24xx_read_bytes(0, sizeof(int16)*10, (uint8 *)memory1);
						for (i=0; i< 10;i++)
						{
							printf(" El valor %d de Memory1 es = %d\n",i,memory1[i]);
							
						}
						scanf("%d",&stop);
				    break;
					
					case (2):
						fprintf_P(stdout,PSTR("Digite el ID de la Pose que Desea Encontrar. \n"));
						scanf("%d", &id);
						Addr= Found_Id (id);
						if(Addr>=0)
							printf("La pose %d tiene una direccion = %d \n",id,Addr);
						else
							printf("La pose %d No existe \n",id);
						scanf("%d",&stop);
				    break;
					
					case (3):
						BorrarEEPROM();
				    break;
					
					case (4):						
						Adicionar_PoseTWI(Pose);				    
				    break;
					
					case (5):
						SobreEscribir_Pose(Pose);				    
				    break;
					
					case (6):
						fprintf_P(stdout,PSTR("Digite el ID de la Pose que desea Leer . \n"));
						scanf("%d", &id_pose);
						rv=Leer_PoseTWI(id_pose, &Pose);
						if(rv > 0)
						{
							printf("Cintura	= %ld \n",Pose.Pos_Waist);
							printf("Hombro 	= %ld \n",Pose.Pos_Shoulder);
							printf("Codo 	= %ld \n",Pose.Pos_Elbow);
							printf("Pitch 	= %ld \n",Pose.Pos_Pitch);
							printf("Roll 	= %ld \n",Pose.Pos_Roll);
						}
						else
							fprintf_P(stdout,PSTR("La Pose no Existe \n"));
							scanf("%d",&stop);				
				    break;
					
					case (7):
						rv= return_id_vector();
						if(rv==1)
						{
							num_datos=id_vector[0];
							
							for(i=0;i<num_datos;i++)
							{
								printf("id_vector[%d] en Jarboter.c => %d \n",i,id_vector[i]);
							}
						}
						if(rv==0)
							printf("No Existen Poses en jarboter.c \n");
						
						if(rv==-1)
							fprintf_P(stdout,PSTR("Error en la Transmision I2C en Jarboter.c \n"));
							scanf("%d",&stop);	
				    break;					
					
					case (8):
						//GetTrajectory(id_vector_PC);
				    break;
					
					case (9):
						//Print_Trajectory();
						//scanf("%d",&stop);	
				    break;
						
					default:
					break;
				}			
			break;			
			
			case (5)://FUNCIONES DE TRAZO E IMPRESION DE VARIABLES
				fprintf_P(stdout,PSTR("0. Configurar Trazo \n"));
				fprintf_P(stdout,PSTR("1. Imprimir Trazo \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);
				
				if(submenu >=0 && submenu<=1)
				{				
					fprintf_P(stdout,PSTR("Digite la articulacion \n"));
					fprintf_P(stdout,PSTR("0. Cintura. \n"));
					fprintf_P(stdout,PSTR("1. Hombro. \n"));
					fprintf_P(stdout,PSTR("2. Codo. \n"));
					fprintf_P(stdout,PSTR("3. Pitch. \n"));		
					fprintf_P(stdout,PSTR("4.Roll. \n"));
					scanf("%d",&articulacion);
				}	
				
				
				switch (submenu)
				{
					case (0):
						SetupSingleAxisTrace(articulacion);
				    break;	
					
					case (1):
						DisplayTraceResults(articulacion,4);
				    break;
					
					default:
					break;
				}				
			break;
			
			case(6)://RESET EVENT STATUS REGISTER DE TODOS LOS EJES
				Reset_Even_Status_Register();
			break;
			
			case(7)://FUNCIONES DE GRIPPER
				fprintf_P(stdout,PSTR("0. Abrir Gripper \n"));
				fprintf_P(stdout,PSTR("1. Cerrar Gripper \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);
				
				switch (submenu)
				{
					case (0):
						openGripper(); 
				    break;	
					
					case (1):
						closeGripper();
				    break;
					
					default:
					break;
				}				
			break;
			
			case(8)://Funciones para DEBUG
				fprintf_P(stdout,PSTR("0. Debug Signal Status \n"));
				fprintf_P(stdout,PSTR("1. Hallar Limites (grados) del Robot \n"));
				fprintf_P(stdout,PSTR("2. Hallar en Numero de Pasos por Revolucion del Motor \n"));
				fprintf_P(stdout,PSTR("3. Search Home Roll Positive \n"));
				fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
				scanf("%d",&submenu);		
				
				if(submenu >=0 && submenu<=2)
				{	
					fprintf_P(stdout,PSTR("Digite Articulacion : \n"));
					fprintf_P(stdout,PSTR("0. Cintura. \n"));
					fprintf_P(stdout,PSTR("1. Hombro. \n"));
					fprintf_P(stdout,PSTR("2. Codo. \n"));
					fprintf_P(stdout,PSTR("3. Pitch. \n"));
					fprintf_P(stdout,PSTR("4.Roll. \n"));
					fprintf_P(stdout,PSTR("Otra Opcion: Salir SubMenu \n"));
					scanf("%d",&articulacion);
				}				
				
				switch (submenu)
				{
					case (0):
						while(1)
						{
							PMDCheckSignalStatus(articulacion,no_imprimir);
						}
				    break;
					
				    case (1):
						while(1)
						{
							rangosrobot(articulacion);
						}
				    break;
					
					case (2):
						StepCalibration(articulacion);
				    break;
					
					case (3):
						search_home_roll_positive(&home);
				    break;		
					default:
					break;
				}
			break;
			
			default://PARADA SUAVE DE TODOS LOS EJES
				sofstop();
				fprintf_P(stdout,PSTR("Se ha realizado una parada suave en todos los Ejes \n"));
			break; 
			
			
			
		}			
	}

#else

  for (;;)
   {	

		#ifdef DEBUGAVR 
		printf("TEACH PENDANT BASICO .\n");
		printf("Tesis de Grado JARBOTER V2.1. \n");
		printf("0. Movimiento de Cintura Positivo. \n");
		printf("1. Movimiento de Cintura Negativo. \n");
		printf("2. Movimiento de Hombro Positivo. \n");
		printf("3. Movimiento de Hombro Negativo. \n");
		printf("4. Movimiento de Codo Positivo. \n");
		printf("5. Movimiento de Codo Negativo. \n");
		printf("6. Movimiento de Pitch Positivo. \n");
		printf("7. Movimiento de Pitch Negativo. \n");
		printf("8. Movimiento de Roll Positivo. \n");
		printf("9. Movimiento de Roll Negativo. \n");
		printf("10. Movimiento Motor Pitch Positivo. \n");
		printf("11. Movimiento Motor Roll Negativo. \n");
		printf("12. Ver el Status register. \n");
		printf("13. Ver el signal status . \n");
		printf("14. ver el Activity Status. \n");
		printf("15. ver el Handle info. \n");
		printf("16. Go Home -Inicializar Chipsets-. \n");
		printf("17. Leer Fecha \n");
		printf("Otra Opcion: Parada Suave \n");	
		#endif		
		
		#ifdef DEBUGAVR
		int16 a;
		scanf("%d",&a);
		#endif
		
		
		
		switch (a)
		{
			case (0):
			mov_axis(AxisWaist,none,50000,100,update);
			break;
			case (1):
			mov_axis(AxisWaist,none,-50000,100,update);
			break;
			case (2):
			mov_axis(AxisShoulder,none,350000,100,update);
			break;
			case (3):
			mov_axis(AxisShoulder,none,-350000,100,update);
			break;
			case (4):
			mov_axis(AxisElbow,none,350000,100,update);
			break;
			case (5):
			mov_axis(AxisElbow,none,-350000,100,update);
			break;
			case (6):
			pitch(none,30000,100,update);
			break;
			case (7):
			pitch(none,-30000,100,update);
			break;
			case (8):
			roll(none,-25000,100,update);
			break;
			case (9):
			roll(none,25000,100,update);
			break;
			case (10):
			mov_axis(AxisPitch,none,40000,100,update);
			break;			
			case (11):
			mov_axis(AxisRoll,none,-30000,100,update);
			break;
			
			case (17):
			Read_Fecha(&Fecha);
			printf("Fecha = Hoy es el dia %s %x de %s de 200%x. \n",Dayname(Fecha.dw),Fecha.dt,Monthname(Fecha.mo),Fecha.yr);
			
			if((Fecha.hr & 0x20) != 0)			
				printf("Son las %x:%x:%x PM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
			else 
				printf("Son las %x:%x:%x AM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
			scanf("%d",&stop);		
			break;
			
			default:
			sofstop();
			break;
		}
		
		
		
	}
#endif	

  return 0;
}

