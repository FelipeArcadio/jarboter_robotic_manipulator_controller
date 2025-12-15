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


#include "debug.h"
#include "messages.h"
#include "packetIO.h"

#define		DEBUGAVR
//#define		DEBUG
#define		TRAPEZOIDAL
#define		CALIBRACION


void Init_EXRAM (void) 
{ 
	XMCRA		=	_BV(SRE) | _BV(SRW11) | _BV(SRW10);	
	XMCRB 		= 	0x00; 
}

void Init_EXRAM (void) __attribute__ ((naked)) \
__attribute__ ((section (".init3")));




 
 /*
 * Bits that are set inside interrupt routines, and watched outside in
 * the program's main loop.
 */


volatile struct
{
  uint8 PMD1_int: 1;
  uint8 PMD2_int: 1;}
intflags;


//Variable Timer
	uint16 conta=0;
	
	//Variables Fecha
	tFecha Fecha;
	uint8 SR_RTC;
	tcontrol_RTC ctr;
	tAlarm alarma;
	int16 sbib,bsw,dtr,atr,al,im,ask,scn;
	int16 activo_alarma,cont_alarma=0;

ISR(INT2_vect)
{
	abruptstop(); 
	buzzer(1);
	printf("Se ha efectudado una parada Abrupta en los motores. \n");
	fprintf(stderr,"Se ha efectudado una parada Abrupta en los motores. \n"); 
	exit(1);
}


ISR(INT4_vect)
{
//	intflags.PMD1_int=0;				
 
	printf("PMD1 PRODUJO INTERRUPCION \n"); 
	InterruptHandling_chipset1();

}

ISR(INT5_vect)
{
//	intflags.PMD2_int=0;	
 
	printf("PMD2 PRODUJO INTERRUPCION \n"); 
	InterruptHandling_chipset2();

}

ISR(TIMER1_OVF_vect)
{
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

ISR(TIMER3_OVF_vect)
{
	
	if (activo_alarma)
	{
		
		cont_alarma++;
		
		if(cont_alarma==3)
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

	PMDint32 position,velocity,aceleration, error_posicion,relacion;
	PMDresult status;
	PMDuint16 cuentas_encoder, micropasos;
	
	PMDint32 position1, velocity1, commandedposition, commandedvelocity, commandedacceleration, readencoder1, readerror;
	PMDuint32 acceleration1, deceleration;
	int16 a,eje,b,P_ajuste;
	
	//Variables de BreakPoint
	int16 eje_break,Breakpoint_id,source_Axis,accion,trigger;
	PMDint32 value;
	
	
	
	//Variables EEPROM
	int16 memory[10]={2,4,6,8,10,12,14,16,18,20};
	int16 memory1[10],i,stop;
	uint16 id,id_pose;
	int16 Addr;
	
	//Variables Pose Robot
	PoseRobot Pose;
	int16 rv,num_datos;
	
	//Variables Prueba Get_Trajectory
	
	uint16 id_vector_PC[5] ={5,2,3,7,8};
	


	

	
	intflags.PMD1_int=1;
	intflags.PMD2_int = 1;
	
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
	
	checkconnect();
	
	#ifdef TRAPEZOIDAL
		PMDsetupChipsets_dummy(PMDTrapezoidalProfile);
	#else
		PMDsetupChipsets_dummy(PMDVelocityContouringProfile);
	#endif
	
	/** SE observa el Estado del Boton de Parada de Emergecia **/
	
	if(bit_is_set(PIND, Estado_Boton_Emergencia))
	{
		buzzer(1);
		printf("Boton de Parada de Emergencia Activo \n");
		fprintf_P(stderr,PSTR("Boton de Parada de Emergencia Activo \n"));	
	}
	
	No_swicht();
	
   EIFR=0xFF;   //Borro flasas interrupnes al inizializar PMDs, ver pag 79 del manual atemega1280
	             //Para evitar el problema de la interrupcion al inicializar el controlador
					 
/**	 enable interrupts 	*/


	
	#ifdef CALIBRACION
		HostInterruptEnable();
	#endif
	sei();
	
	fprintf(stderr, "Robot Jarboter Controller.\n");

	
	
	
	

#ifdef TRAPEZOIDAL


	//Se resetea el Event Status Register en todos los ejes y se borra el Error
	
	
	Clear_Position_Error();	
	Reset_Even_Status_Register();
	_delay_ms(100);
	Reset_Even_Status_Register();
	
	
	for (;;)
   {
	
		
	
	
		#ifdef DEBUGAVR 		
		fprintf_P(stdout,PSTR("PROGRAMA DE PRUEBAS Y DEPURACION .\n"));
		fprintf_P(stdout,PSTR("Tesis de Grado JARBOTER V2.1. \n"));
		fprintf_P(stdout,PSTR("Digite Articulacion a mover \n"));
		fprintf_P(stdout,PSTR("0. Cintura. \n"));
		fprintf_P(stdout,PSTR("1. Hombro. \n"));
		fprintf_P(stdout,PSTR("2. Codo. \n"));
		fprintf_P(stdout,PSTR("3. Pitch. \n"));
		fprintf_P(stdout,PSTR("4.Roll. \n"));
		fprintf_P(stdout,PSTR("5. Informacion Movimiento\n"));
		fprintf_P(stdout,PSTR("6. Lectura del Event Status Register \n"));
		fprintf_P(stdout,PSTR("7. Lectura del Activity Status Register \n"));
		fprintf_P(stdout,PSTR("8. Signal Status \n"));
		fprintf_P(stdout,PSTR("9. Ajustar Posicion \n"));
		fprintf_P(stdout,PSTR("10. Realizar Proceso de Calibracion \n"));
		fprintf_P(stdout,PSTR("11. Hallar Nest y max y min posicion \n"));
		fprintf_P(stdout,PSTR("13. Configurar BreakPoint \n"));
		fprintf_P(stdout,PSTR("14. Escribir Fecha \n"));
		fprintf_P(stdout,PSTR("15. Leer Fecha \n"));
		fprintf_P(stdout,PSTR("16. Leer El Status Register del RTC \n"));
		fprintf_P(stdout,PSTR("17. Escribir en la EEPROM \n"));
		fprintf_P(stdout,PSTR("18. Leer la EEPROM \n"));
		fprintf_P(stdout,PSTR("19. Encontrar Direccion de ID \n"));
		fprintf_P(stdout,PSTR("20. Borrar EEPROM \n"));
		fprintf_P(stdout,PSTR("21. Adicionar Pose en EEPROM \n"));
		fprintf_P(stdout,PSTR("22. Leer Pose almacenada en EEPROM \n"));
		fprintf_P(stdout,PSTR("23. Sobreescribir Pose \n"));
		fprintf_P(stdout,PSTR("24. Retornar Vector de Ids \n"));
		fprintf_P(stdout,PSTR("25. Obtener Trayectoria de un Vector de IDs \n"));
		fprintf_P(stdout,PSTR("26. Imprimir Datos de Trayectoria \n"));
		fprintf_P(stdout,PSTR("27. Escribir en el Control de Registros del RTC \n"));
		fprintf_P(stdout,PSTR("28. Leer el Control de Registros del RTC \n"));
		fprintf_P(stdout,PSTR("29. Configurar Alarma \n"));
		fprintf_P(stdout,PSTR("30. Configurar Trazo \n"));
		fprintf_P(stdout,PSTR("31. Imprimir Trazo \n"));
		fprintf_P(stdout,PSTR("32. Reset Event Status Register \n"));
		fprintf_P(stdout,PSTR("Otra Opcion: Parada Suave \n"));		
		#endif	
		
		
		#ifdef DEBUGAVR
		scanf("%d",&a);
		#endif
		
		if ((a>=0)&&(a<=4))
		{
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite Posicion? \n"));
			scanf("%ld", &position);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite Velocidad? \n"));
			scanf("%ld", &velocity);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite aceleracion? \n"));
			scanf("%ld", &aceleration);
			#endif
			
		}
		
		if ( ((a>=5)&&(a<=11))  || ( (a>=30)&&(a<=31) ) )
		{
			#ifdef DEBUGAVR 
			fprintf_P(stdout,PSTR("Digite la articulacion que quiere Leer\n"));
			fprintf_P(stdout,PSTR("0. Cintura. \n"));
			fprintf_P(stdout,PSTR("1. Hombro. \n"));
			fprintf_P(stdout,PSTR("2. Codo. \n"));
			fprintf_P(stdout,PSTR("3. Pitch. \n"));		
			fprintf_P(stdout,PSTR("4.Roll. \n"));
			scanf("%d",&b);
			
			#endif
		}
			
		if(a == 21 || a==23)
		{
			fprintf_P(stdout,PSTR("Digite el ID de la Pose que desea grabar . \n"));
			scanf("%d", &id_pose);			
			Pose.id=id_pose;
			
			//ObtenerPose(&Pose);
			
			
			fprintf_P(stdout,PSTR("Digite la posicion de cintura . \n"));
			scanf("%ld", &position);
			Pose.Pos_Wrist=position;
			
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
		
		
		switch (a)
		{
			case (0):
		   mov_axis(AxisWaist,position,velocity,aceleration,update);
			break;
			
			case (1):
		   mov_axis(AxisShoulder,position,velocity,aceleration,update);
		   break;
			
		   case (2):
		   mov_axis(AxisElbow,position,velocity,aceleration,update);
		   break;
			
			case (3):
		   pitch(position,velocity,aceleration,update);
		   break;
			
			case (4):
		   roll(position,velocity,aceleration,update);
		   break;			
			
			case (5):
		   infomov(b,&position1, &velocity1, &acceleration1, &deceleration, &commandedposition,
			&commandedvelocity, &commandedacceleration, &readencoder1, &readerror,&cuentas_encoder,
			&micropasos);
			printf("la informacion correspondiente del movimiento del eje %s es :\n",AxisName(3));
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
			
			case (6):
			PMDCheckEventStatus(b,imprimir);
			scanf("%d",&stop);	
			break;
			
			case (7):
			PMDCheckActivityStatus(b,imprimir);
			scanf("%d",&stop);	
			break;
			
			case (8):
			PMDCheckSignalStatus(b,imprimir);
			scanf("%d",&stop);	
			break;
			
			case (9):
			fprintf_P(stdout,PSTR("Digite el valor de la posicion a ajustar \n"));
			scanf("%d",&P_ajuste);
			ajustar_posicion(b,P_ajuste);
			break;
			
			case (10):
			relacion=calibration(b);
			printf("la relacion de pasos a motor en el eje %s es= %08ld \n",AxisName(b),relacion);				
			break;
			
			case (11):
			initializeAxis(b);
			break;
			
			case (12):
			//prueba_waist_home();
			break;
			
			case (13):
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el eje el cual configurara el BreakPoint \n"));
			fprintf_P(stdout,PSTR("0. Cintura. \n"));
			fprintf_P(stdout,PSTR("1. Hombro. \n"));
			fprintf_P(stdout,PSTR("2. Codo. \n"));
			fprintf_P(stdout,PSTR("3. Pitch. \n"));		
			fprintf_P(stdout,PSTR("4.Roll. \n"));
			scanf("%d",&eje_break);
			#endif	
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el  BreakPoint_ID \n"));
			fprintf_P(stdout,PSTR("0. BreakPoint1. \n"));
			fprintf_P(stdout,PSTR("1. BreakPoint2. \n"));
			scanf("%d",&Breakpoint_id);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Eje Fuente (source) del BreakPoint \n"));
			fprintf_P(stdout,PSTR("0. Cintura. \n"));
			fprintf_P(stdout,PSTR("1. Hombro. \n"));
			fprintf_P(stdout,PSTR("2. Codo. \n"));
			fprintf_P(stdout,PSTR("3. Pitch. \n"));		
			fprintf_P(stdout,PSTR("4.Roll. \n"));
			scanf("%d",&source_Axis);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite la accion del BreakPoint \n"));
			fprintf_P(stdout,PSTR("0.Ninguna Accion. \n"));
			fprintf_P(stdout,PSTR("1. Update. \n"));
			fprintf_P(stdout,PSTR("2. AbruptStop. \n"));
			fprintf_P(stdout,PSTR("3. SmoothStop. \n"));
			fprintf_P(stdout,PSTR("4.Motor_OFF. \n"));
			scanf("%d",&accion);
			#endif
			
			#ifdef DEBUGAVR
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
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el valor del Trigger \n"));
			scanf("%ld",&value);
			#endif
			
			Config_BreakPoint(eje_break, Breakpoint_id, source_Axis, accion, trigger,value);
			break;
			
			case (14):
			//Configuro el ano 20 (para 2000)
			Fecha.y2k=0x20;
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Valor en Segundos. 0-59 \n"));
			scanf("%x",(int16 *)&Fecha.sc);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Valor en Minutos. 0-59 \n"));
			scanf("%x",(int16 *)&Fecha.mn);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Valor de la Hora  \n"));
			scanf("%x",(int16 *)&Fecha.hr);
			#endif
			
			fprintf_P(stdout,PSTR("0.AM  \n"));
			fprintf_P(stdout,PSTR("1.PM  \n"));
			scanf("%d",&ask);
			
			if(ask==1)
				Fecha.hr= Fecha.hr | 0x20;
			
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Dia. 1-31 \n"));
			scanf("%x",(int16 *)&Fecha.dt);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Mes. 1-12 \n"));
			scanf("%x",(int16 *)&Fecha.mo);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el Ano. 0-99 \n"));
			scanf("%x",(int16 *)&Fecha.yr);
			#endif
			
			#ifdef DEBUGAVR
			fprintf_P(stdout,PSTR("Digite el dia de la Semana. \n"));
			fprintf_P(stdout,PSTR("0. Domingo \n"));
			fprintf_P(stdout,PSTR("1. Lunes \n"));
			fprintf_P(stdout,PSTR("2. Martes \n"));
			fprintf_P(stdout,PSTR("3. Miercoles \n"));
			fprintf_P(stdout,PSTR("4. Jueves \n"));
			fprintf_P(stdout,PSTR("5. Viernes \n"));
			fprintf_P(stdout,PSTR("6. Sabado \n"));
			scanf("%x",(int16 *)&Fecha.dw);
			#endif
			
			write_Fecha(Fecha);
			break;
			
			case (15):
			Read_Fecha(&Fecha);
			printf("Fecha = Hoy es el dia %s %x de %s de 200%x. \n",Dayname(Fecha.dw),Fecha.dt,Monthname(Fecha.mo),Fecha.yr);
			
			if((Fecha.hr & 0x20) != 0)			
				printf("Son las %x:%x:%x PM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
			else 
				printf("Son las %x:%x:%x AM \n",(Fecha.hr & 0x1F),Fecha.mn,Fecha.sc);
			scanf("%d",&stop);		
			break;
			
			case (16):
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
			
			case (17):
			ee24xx_write_bytes(0, sizeof(int16)*10, (uint8 *)memory);
			for (i=0; i< 10;i++)
			{
				printf(" El valor %d de Memory es = %d\n",i,memory[i]);
				scanf("%d",&stop);
			}
			break;
			
			case (18):
			ee24xx_read_bytes(0, sizeof(int16)*10, (uint8 *)memory1);
			for (i=0; i< 10;i++)
			{
				printf(" El valor %d de Memory1 es = %d\n",i,memory1[i]);
				scanf("%d",&stop);
			}			
			break;
			
			case (19):
			fprintf_P(stdout,PSTR("Digite el ID de la Pose que Desea Encontrar. \n"));
			scanf("%d", &id);
			Addr= Found_Id (id);
			if(Addr>=0)
				printf("La pose %d tiene una direccion = %d \n",id,Addr);
			else
				printf("La pose %d No existe \n",id);
			scanf("%d",&stop);
			break;
			
			case (20):
			BorrarEEPROM();
			break;
			
			case (21):			
			Adicionar_PoseTWI(Pose);
			break;
			
			case (22):			
			fprintf_P(stdout,PSTR("Digite el ID de la Pose que desea Leer . \n"));
			scanf("%d", &id_pose);
			rv=Leer_PoseTWI(id_pose, &Pose);
			if(rv > 0)
			{
				printf("Cintura	= %ld \n",Pose.Pos_Wrist);
				printf("Hombro 	= %ld \n",Pose.Pos_Shoulder);
				printf("Codo 	= %ld \n",Pose.Pos_Elbow);
				printf("Pitch 	= %ld \n",Pose.Pos_Pitch);
				printf("Roll 	= %ld \n",Pose.Pos_Roll);
			}
			else
				fprintf_P(stdout,PSTR("La Pose no Existe \n"));
				scanf("%d",&stop);				
			break;
			
			case (23):			
			SobreEscribir_Pose(Pose);
			break;
			
			case (24):			
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
			
			case (25):			
			//GetTrajectory(id_vector_PC);
			break;
			
			case(26):
			//Print_Trajectory();
			//scanf("%d",&stop);		
			break;
			
			case(27):
			
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
			
			case(28):
			read_control(&ctr);
			printf("PWR = %x \n",ctr.PWR);
			printf("DTR = %x \n",ctr.DTR);
			printf("ATR = %x \n",ctr.ATR);
			printf("INT = %x \n",ctr.INT);
			printf("BL  = %x \n",ctr.BL);
			scanf("%d",&stop);		
			break;
			
			case(29):
			
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
			
			case (30):			
			SetupSingleAxisTrace(b);
			break;
			
			case (31):			
			DisplayTraceResults(b,4);
			break;
			case (32):			
			Reset_Even_Status_Register();
			break;
			default:
			sofstop();
			fprintf_P(stdout,PSTR("Se ha realizado una parada suave en todos los Ejes \n"));
			break;
		}
		
		if ((a>=0)&&(a<=4))
		{
			
			/*do
			{
				error_posicion=readError(a);
				status=PMDCheckEventStatus(a,no_imprimir);
				printf("El movimiento no ha sido considerado completo \n");
				printf("Error Posicion=	%08ld\n",error_posicion);			
			}
			while ((status & PMDEventMotionCompleteMask)==0);
			printf("El movimiento se completo \n");*/
			
			motioncomplete(a);
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
			mov_axis(AxisShoulder,none,300000,100,update);
			break;
			case (3):
			mov_axis(AxisShoulder,none,-300000,100,update);
			break;
			case (4):
			mov_axis(AxisElbow,none,200000,100,update);
			break;
			case (5):
			mov_axis(AxisElbow,none,-200000,100,update);
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

