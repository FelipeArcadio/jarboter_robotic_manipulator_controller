#include <avr/io.h>
#include <avr/interrupt.h>

#include "uC_setup.h"


/****************************************************************************************/ 
void initCard(void)
{
 /////////////LED
 DDRE		=	_BV(LED1) | _BV(LED2) | _BV(Reset);				//Salidas
 PORTE		=	_BV(LED1) | _BV(LED2);				                //Condiciones Iniciales							
 

 DDRB		=	_BV(LED0);
 PORTB		=	_BV(LED0);											//Condiciones Iniciales
 
 //////////////PMDS
 PORTE      &= 	~(_BV(Reset));						                //Mantenido reset
 DDRD		=	_BV(HostSlct1) | _BV(HostSlct2) | _BV(HostSlct3) | _BV(Alarma); // Salidas.
 PORTD		=	_BV(HostSlct1) | _BV(HostSlct2) | _BV(HostSlct3);	//Condiciones Iniciales

 
 
 DDRG 		= 	_BV(HostCmd) | _BV(HostWrite) | _BV(HostRead); 			// HostCmd,HostWrite,HostRead,Reset Salidas
 PORTG		= 	_BV(HostWrite) | _BV(HostRead);							//Condiciones Iniciales


 DDRH		=	_BV(USB_EN) | _BV(USB_WRITE) | _BV(USB_READ);		//Salidas
 PORTH		=	_BV(USB_EN) | _BV(USB_WRITE) | _BV(USB_READ);   	//Condiciones Iniciales
 
 /****************************** GRIPEER***************************************/
 
  //coloco como entrada el puerto de gripper para no colocar esta tarjeta. El puerto esta en
  //Hi-Z
   //DDRF		=	0x00;
  // PORTF    =	0x00;
 
 
  
  
  //Mantener el motor apadado y coloca resistencias de Pull up en FINDER E FINDIZQ
  PORTF     =	_BV(DER) | _BV(IZQ) | _BV(FINDER) | _BV(FINIZQ);
  
   //Solo Salidas DER y IQZ 
  DDRF		=	_BV(DER) | _BV(IZQ);
 
  
 //Necesito que las entradas tengan resistencia de pull up
 // MCUCR		 &= 	~(_BV(PUD));	
  

 
/****************************** oTROS***************************************/

/** La interrupcion 3 (INT3) se activa en flanco de SUBIDA y la Interrupcion 2 (INT2)
	 se activa en flanco de Subida.*/

	EICRA		=	_BV(ISC31) | _BV(ISC30) | _BV(ISC21) | _BV(ISC20);

/** Las interrupciones INT4 E INT5 ( PMD1 y PMD2) se activan en flanco de Bajada.*/	
	
	EICRB		=	_BV(ISC51) | _BV(ISC41);

/**Habilito la Interrupcion Externa 2 (interrupción Botón Parada Emergencia)
			la Interrupcion Externa 3 (interrupción USB)
			la Interrupcion Externa 4 (interrupción PMD1)
			la Interrupcion Externa 5 (interrupción PMD2)*/
		
	EIMSK		=	_BV(INT5) | _BV(INT4) | _BV(INT3) | _BV(INT2);

/** Habilito la Memoria RAM Externa con las siguientes características:
	-Toda la Memoria Ocupa un solo sector: SRL2, SRL1, y SRL0 están en Cero.
	-con dos ciclos de espera: SRW11 y SRW10 bits están en ALTO.  */
	
	XMCRA		=	_BV(SRE) | _BV(SRW11) | _BV(SRW10);


/** Configuracion Timer 1 (Timer de 16 bits) */

/** Timer/ Counter 1 Interrrupt Mask Register. Activo el Bit Timer/Counter1, Overflow
    interrupt Enable*/


	TIMSK1		=	_BV(TOIE1);	
	
/** Modo de Operacion Normal, con un prescaler de CLK/1024 osea que trabaja con un reloj 
de 7.8125Khz por lo tanto cada 0,000128 seg aumenta se desborda cada 8,38848 seg*/	

	TCCR1B 		=	_BV(CS12) | _BV(CS10);
	
	
/** Configuracion Timer 3 (Timer de 16 bits) */

/** Timer/ Counter 3 Interrrupt Mask Register. Activo el Bit Timer/Counter3, Overflow
    interrupt Enable*/


	TIMSK3		=	_BV(TOIE3);	
	
/** Modo de Operacion Normal, con un prescaler de CLK/1024*/	

	TCCR3B 		=	_BV(CS32) | _BV(CS30);
	
	

 }
 
 
 
 //agregado por david el 19/05/2014 para hacer la prueba del boton de emergencia.
 ////////////////////////////////////////////////////////////////////////////
//Returna el registro del TIMER1 y lo borra
//Si esta funcion es utilizada periodicamente antes que se
//cumpla la INT del timer1 nunca este se va adesvordar y NUNCA se 
//genera la INT por desvorde...
/////////////////////////////////////////////////////////////////////////////
unsigned int TIM16_ReadTCNTn(void)
{
	unsigned char sreg;
	unsigned int i;
	
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Read TCNTn into i */
	i = TCNT1;
	TCNT1=0;
	/* Restore global interrupt flag */
	SREG = sreg;
	sei();
	return i;
}

 