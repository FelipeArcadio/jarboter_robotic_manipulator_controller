#include <avr/io.h>
#include <stdio.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#include "macros.h"
#include "types.h"
//#define DEBUGGRIPPER



void openGripper(void) 
{
	uint16 timer;
	int16 pin;
	timer 		= 10; 	
 
 
	// Los switch son normalmente Abierto. (Se necesitan de alguna resistencia de pullup para estos)
    
    //abriendo el gripper
	GRIPPERCTL |= _BV(DER);  		//1
    GRIPPERCTL &= ~(_BV(IZQ)); 	//0
	pin=PINF;
	
	#ifdef DEBUGGRIPPER
	printf("openGripper=> Abriendo Gripper= %#X\n",pin);
	#endif	
	for(;;) 
	{
		
		if (!bit_is_set(PINF,FINDER))
		{
			//Parar Motor
			//Mantener el motor apadado
			PORTF     |=	_BV(DER) | _BV(IZQ);
			#ifdef DEBUGGRIPPER
			printf("openGripper=> El motor del Gripper se ha detenido accionando switch \n");
			#endif	
			return;
		}
		
		else// not ready 
		{ 
			timer--;
			_delay_ms(100);
	
			if(timer == 0) 
			{
				//Parar Motor
				//Mantener el motor apadado
				PORTF     |=	_BV(DER) | _BV(IZQ);
				#ifdef DEBUGGRIPPER
				printf("openGripper=> El motor del Gripper se ha detenido por timeotu \n");	
				#endif			
				return;
			} // if timer == 0
		}
  	} // for EVER      
} // openGripper

void openGripper_time(int tiempo) 
{
	uint8 i;
	
	//se abre el gripper por un periodo de tiempo
	
	GRIPPERCTL |= _BV(DER);  		//1
    GRIPPERCTL &= ~(_BV(IZQ)); 	//0
	
	

	for (i = 0; i < tiempo; i++)   
			_delay_ms(100);
	
	//Parar Motor
	//Mantener el motor apadado
	PORTF     |=	_BV(DER) | _BV(IZQ);
	#ifdef DEBUGGRIPPER
	printf("openGripper=> El motor del Gripper se ha detenido \n");
	#endif	

	
	
	
    
} // openGripper_time


void closeGripper(void) 
{

	uint16 timer;
	int16 pin;

		
	timer 		= 10; 	
	
	
	
	
	//cerrando el gripper
	GRIPPERCTL &= ~(_BV(DER));  	//0
    GRIPPERCTL |= _BV(IZQ); 	//1
	pin=PINF;
	
	#ifdef DEBUGGRIPPER
	printf("closeGripper=> Cerrando Gripper= %#X\n",pin);
	#endif	
	
		
	for(;;) 
	{
		
		if (!bit_is_set(PINF,FINIZQ))
		{
			//Parar Motor
			//Mantener el motor apadado
			PORTF     |=	_BV(DER) | _BV(IZQ);
			#ifdef DEBUGGRIPPER
			printf("closeGripper=> El motor del Gripper se ha detenido accionando switch \n");
			#endif	
			return;
		}
		
		else// not ready 
		{ 
			timer--;
			_delay_ms(100);
			
			if(timer == 0) 
			{
				//Parar Motor
				//Mantener el motor apadado
				PORTF     |=	_BV(DER) | _BV(IZQ);
				#ifdef DEBUGGRIPPER
					printf("closeGripper=> El motor del Gripper se ha detenido por timeotu \n");				
				#endif	
				
				return;
			} // if timer == 0
		}
  	} // for EVER  
 
 

} // closeGripper


void closeGripper_time(int tiempo) 
{
	uint8 i;
	
	//se cierra el gripper por un periodo de tiempo
	
	GRIPPERCTL &= ~(_BV(DER));  	//0
    GRIPPERCTL |= _BV(IZQ); 		//1
	
	

	for (i = 0; i < tiempo; i++)   
			_delay_ms(100);
	
	//Parar Motor
	//Mantener el motor apadado
	PORTF     |=	_BV(DER) | _BV(IZQ);
	#ifdef DEBUGGRIPPER
		printf("closeGripper_time=> El motor del Gripper se ha detenido \n");
	#endif	
	
	
	
	
    
} // closeGripper_time


//Si el gripper tiene CARGA una pieza, la funcion retorn abierto asi el gripper este cerrado.......
int8 Estado_gripper(void)
{

	if (!bit_is_set(PINF,FINIZQ))
	{
		//#ifdef DEBUGGRIPPER
		printf("Estado_gripper=> El Gripper esta cerrado. \n");
		//#endif
		return 1;
	}
	if (!bit_is_set(PINF,FINDER))
	{
		//#ifdef DEBUGGRIPPER
		printf("Estado_gripper=> El Gripper esta abierto. \n");
		//#endif		
		return 0;
	}
	//#ifdef DEBUGGRIPPER
	printf("Estado_gripper=> El Gripper no esta definido. \n");
	//#endif
	return -1;


}