#include <avr/io.h>
#include <avr/eeprom.h>
/**Standard Integer Types*/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> 
#include <util/delay.h>
#include "types.h"
#include "macros.h"
#include "tw_teach_pendant.h"
#include "COMMONtypes.h"
#include "tw_rtc.h"
#include "messages.h"


PMDint32 EEMEM ATmegaEEPROM_var=0;

/****************************************************************************************/  
void delay_1s(void)
{
  uint8 i;

  for (i = 0; i < 100; i++)   
    _delay_ms(10);
}

/****************************************************************************************/ 
void buzzer(uint16 valor_s)
{
	uint8 i;	
	
	PORTD 	|= 	_BV(Alarma);
	
	for(i = 0; i < valor_s; i++)
	{
		delay_1s();
	}
	
	PORTD 	&= 	~(_BV(Alarma));
}
/****************************************************************************************/ 
void buzzer_ms(uint32 valor_ms)
{
	
	
	PORTD 	|= 	_BV(Alarma);
	
	_delay_ms((double)valor_ms);
	
	PORTD 	&= 	~(_BV(Alarma));
}
/****************************************************************************************/  
void Led_on (tLeds color)
{
	
	/* Se Coloca el Bit LED0 del Puerto B en Modo de Salida y se le da un valor de 1*/	
	DDRB		|=	_BV(LED0);
	PORTB		|=	_BV(LED0);							

	switch (color)
	{
		case (Amarillo):
			PORTE		&= 	~(_BV(AMARILLOe)); 
		break;
			
		case (Verde):
			PORTE		&= 	~(_BV(VERDEe)); 
		break;
			
		case (Rojo):
			PORTB		&= 	~(_BV(ROJOb));
		break;
			
		default:
			PORTE		|=  _BV(AMARILLOe);
			PORTE		|=  _BV(VERDEe);
			PORTB		|=  _BV(ROJOb);
		break;
	}


}

/****************************************************************************************/  
void Led_off (int16 color)
{
	/* Se Coloca el Bit LED0 del Puerto B en Modo de Salida y se le da un valor de 1*/	
	DDRB		|=	_BV(LED0);
	PORTB		|=	_BV(LED0);							
 
	switch (color)
	{
		case (Amarillo):
			PORTE		|= 	_BV(AMARILLOe); 
		break;
			
		case (Verde):
			PORTE		|= 	_BV(VERDEe); 
		break;
			
		case (Rojo):
			PORTB		|= 	_BV(ROJOb);
		break;
			
		default:
			PORTE		|=  _BV(AMARILLOe);
			PORTE		|=  _BV(VERDEe);
			PORTB		|=  _BV(ROJOb);
		break;
	}
}


void Test_perifericos(void)
{


	PMDint32 RAM_var;
	PoseRobot Pose_test;
	int16 Num_Poses;
	int32 addr;
	int rv;
	char pause;
	tFecha Test_Fecha;
	
	
	printf("Iniciando secuencia de pruebas perifericos...\n");
	//Prueba LCD y LED...
	printf("Mensaje prueba LCD y LED ...\n");
	fprintf(stderr, "T E S T   L C D.\n");
	Led_on(Amarillo);
	delay_1s();	
	Led_on(Verde);	
	delay_1s();	
	Led_on(Rojo);	
	delay_1s();	
	Led_off(Rojo);
	Led_off(Verde);
	Led_off(Amarillo);
	printf("Resultado corecto Y/N\n");
	scanf("%s",&pause);

	
	//Prueba EEPROM Y RAM ATME1280	
	printf("Prueba de EEPROM interna del Atmega1280...\n");
	eeprom_read_block((void*)&RAM_var,(const void*)&ATmegaEEPROM_var,sizeof(PMDint32));
	printf("Leyando ATmegaEEPROM_var = %ld\n",RAM_var);
	printf("Actualizando a 3032 ...\n");
	RAM_var=3032;
	eeprom_write_block((const void*)&RAM_var,(void*)&ATmegaEEPROM_var,sizeof(PMDint32));
	eeprom_read_block((void*)&RAM_var,(const void*)&ATmegaEEPROM_var,sizeof(PMDint32));
	printf("Comprobando, ATmegaEEPROM_var = %ld\n",RAM_var);
	//Reiniciando..
	RAM_var=0;
	eeprom_write_block((const void*)&RAM_var,(void*)&ATmegaEEPROM_var,sizeof(PMDint32));
	printf("Resultado corecto Y/N\n");
	scanf("%s",&pause);

	//Prueba EEPROM Externa y Pose
	/*Obtengo las Poses de la eeprom, no se desea sobreescribir alguna poses existente*/
	Num_Poses=Get_Num_Poses();
	addr=(int32)(Num_Poses)*sizeof(PoseRobot)+sizeof(int16); 
	/*Pasar al Segundo Banco de Memoria */
	if(Num_Poses > (5696/2))
		addr=addr+7;
	
	printf("Prueba de EEPROM externa, Leyendo Pose en la direccion %ld...\n",addr);
	rv=ee24xx_read_bytes(addr,sizeof(PoseRobot),(uint8*)&Pose_test);
    printf("Pose_test->Pos_num = %d   \n",Pose_test.id);	
	printf("Pose_test->Pos_Waist = %ld  \n ",Pose_test.Pos_Waist);	
	printf("Pose_test->Pos_Shoulder = %ld  \n ",Pose_test.Pos_Shoulder);	
	printf("Pose_test->Pos_Elbow = %ld  \n ",Pose_test.Pos_Elbow);	
	printf("Pose_test->Pos_Pitch = %ld  \n ",Pose_test.Pos_Pitch);	
	printf("Pose_test->Pos_Roll= %ld  \n ",Pose_test.Pos_Roll);	
	
	printf("Actualizando Pose_test a \n");
	printf("Pose_test->Pos_num = 3   \n");	
	printf("Pose_test->Pos_Waist = 5670 \n ");	
	printf("Pose_test->Pos_Shoulder = 6000 \n ");	
	printf("Pose_test->Pos_Elbow = 2400 \n ");	
	printf("Pose_test->Pos_Pitch = 1000 \n ");	
	printf("Pose_test->Pos_Roll= 10 \n ");	
	Pose_test.id= 3;	
	Pose_test.Pos_Waist=5670;	
	Pose_test.Pos_Shoulder=6000;	
	Pose_test.Pos_Elbow=2400;	
	Pose_test.Pos_Pitch=1000;	
	Pose_test.Pos_Roll=10;
	/* grabando pose*/
	rv=ee24xx_write_bytes(addr,sizeof(PoseRobot),(uint8 *)&Pose_test);			
	if(rv)
	{
		printf("Numero de datos grabados corectamente al grabar Num_Poses => %d\n",rv);
		
	}		
	printf("Comprobando, Pose_test...\n");
	printf("Pose_test->Pos_num = %d   \n",Pose_test.id);	
	printf("Pose_test->Pos_Waist = %ld  \n ",Pose_test.Pos_Waist);	
	printf("Pose_test->Pos_Shoulder = %ld  \n ",Pose_test.Pos_Shoulder);	
	printf("Pose_test->Pos_Elbow = %ld  \n ",Pose_test.Pos_Elbow);	
	printf("Pose_test->Pos_Pitch = %ld  \n ",Pose_test.Pos_Pitch);	
	printf("Pose_test->Pos_Roll= %ld  \n ",Pose_test.Pos_Roll);	
	//Reiniciando
	Pose_test.id= 0;	
	Pose_test.Pos_Waist=0;	
	Pose_test.Pos_Shoulder=0;	
	Pose_test.Pos_Elbow=0;	
	Pose_test.Pos_Pitch=0;	
	Pose_test.Pos_Roll=0;
	/* grabando pose*/
	rv=ee24xx_write_bytes(addr,sizeof(PoseRobot),(uint8 *)&Pose_test);	
	printf("Resultado corecto Y/N\n");
	scanf("%s",&pause);

	//Prueba RTC
	printf("Prueba RTC...\n");
	Read_Fecha(&Test_Fecha);
	printf("Fecha = Hoy es el dia %s %x de %s de 200%x. \n",Dayname(Test_Fecha.dw),Test_Fecha.dt,Monthname(Test_Fecha.mo),Test_Fecha.yr);						
	if((Test_Fecha.hr & 0x20) != 0)			
		printf("Son las %x:%x:%x PM \n",(Test_Fecha.hr & 0x1F),Test_Fecha.mn,Test_Fecha.sc);
	else 
		printf("Son las %x:%x:%x AM \n",(Test_Fecha.hr & 0x1F),Test_Fecha.mn,Test_Fecha.sc);
	printf("Resultado corecto Y/N\n");
	scanf("%s",&pause);





}
