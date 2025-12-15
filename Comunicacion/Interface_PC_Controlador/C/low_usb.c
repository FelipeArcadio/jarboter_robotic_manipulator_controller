#include <avr/io.h>
/**Standard Integer Types*/
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "low_usb.h"
//#define		DEBUGUSBLevel0
//#define		DEBUGUSBLevel1
//#define		DEBUGUSBLevel2



/*---------------------------Funciones Interface PC-uControlador---------------------------*/

int16 usb_read(uint8 *dato_h,uint8 *dato_l)
{
	uint32 timeout=0xFFFF;
	
	CONTROL_USB_HIGH		=		0x00;					 	//Coloco el puerto L como lectura
	CONTROL_USB_LOW		=		0x00;					  	//Coloco el puerto K como lectura
	
	PORTH		&= ~(_BV(USB_EN));						  	//Habilito el Controlador USB
	
	/** Espero a que la señal USB_READY esté en Bajo.*/	 
//	loop_until_bit_is_clear(PINH,USB_READY);									
	do
	{
		timeout--;
		if (timeout==0)
			return rcTimeoutReadingData;
	}
	while(bit_is_set(PINH,USB_READY));	
	
	PORTH			&= ~(_BV(USB_READ));				  	//Realiza Lectura
	_delay_us(2);
	
	if(dato_h!=NULL)
	*dato_h	=	DATA_USB_READ_HIGH;         		//leo dato más significativo en el Puerto
	*dato_l	=	DATA_USB_READ_LOW;             	//leo dato menos significativo	
	PORTH 	 		|= _BV(USB_READ);				 	 	//coloco la señal de read en alto	
	PORTH 	 		|= _BV(USB_EN);					 	//coloco la señal de chip select en alto
	
	return rcOK;
} 
/*----------------------------------------------------------------------------------------------------*/
int16 usb_write(uint8 datoh,uint8 datol)
{
	DDRL	=		0xFF;											//Coloco el puerto L como Salida
	DDRK	=		0xFF;											//Coloco el puerto K como Salida
	//uint32 timeout=0xFFFF;
	
	PORTH		&= ~(_BV(USB_EN));							//Habilito el Controlador USB
	
	/* Espero a que la señal USB_READY esté en Bajo.*/
	
  loop_until_bit_is_clear(PINH,USB_READY);
	
/*	do{
	   timeout--;
	   if (timeout==0)
	       return rcTimeoutSendingData;
	}while(bit_is_set(PINH,USB_READY));
*/	
	
	DATA_USB_WRITE_HIGH	=	datoh;						//Coloco el Dato mas significativo en el Puerto
	DATA_USB_WRITE_LOW	=	datol;						//Coloco el Dato menos significativo en el Puerto
	PORTH		&= ~(_BV(USB_WRITE));						//Realizo una operacion de Escritura
	_delay_us(2);
	PORTH 		|= _BV(USB_WRITE);						//coloco la señal de read en alto	
	PORTH 		|= _BV(USB_EN);							//coloco la señal de chip select en alto
	return rcOK;
	
} 
/*----------------------------------------------------------------------------------------------------*/  
//Esta funcion envia por los puertos del uC los datos
//Maneja tanto tamaños pares como inpares
 int16 Envio_USB(uint8* Buffer,int16 size)
{
	#ifdef DEBUGUSBLevel2
	int16 j;
	#endif

	int16 i=0,rc=0,actual=0;
	uint8 templ,temph;

	#ifdef DEBUGUSBLevel1
		printf("Envio_USB => Cantida de datos a mandar %d \n",size);
	#endif
		
	#ifdef DEBUGUSBLevel2		
	for(j=0;j<size;j++)
		printf("Envio_USB => Buffer[%d]=%X \n",j,Buffer[j]);		   
	#endif
   do
	{
		templ=Buffer[i] ;
		actual=i+1;                    					//actual tiene los impares
		if (actual==size)
			rc=usb_write(0x00,templ);     				//he llegado a el final del 
														//arreglo donde el tamaño es impar
		else
		{  	 
			temph=Buffer[i+1];			
			rc=usb_write(temph,templ);
		}
		i=i+2;
		#ifdef DEBUGUSBLevel1
		printf("Envio_USB => i= %d \n",i);
		#endif
	}
	while (i<size); 
	#ifdef DEBUGUSBLevel0
		printf("Envio_USB => Saliendo\n");
	#endif
	return rc;
	
}
/*----------------------------------------------------------------------------------------------------*/
int16 Leo_USB(uint8* Buffer,int16 size)
{
	int16 i=0,rc=0,actual=0;
	
	#ifdef DEBUGUSBLevel1
	printf("Leo_USB => Cantida de datos por leer %d \n",size);
	#endif
	do
	{	
		actual=i+1;                    					//actual tiene los impares
		if(actual==size)
			rc=usb_read(NULL,&Buffer[i]);     			//he llegado a el final del 
														//arreglo donde el tamaño es impar
		else
		rc=usb_read(&Buffer[i+1],&Buffer[i]);				
		i=i+2;
	}
	while (i<size); 
	
	#ifdef DEBUGUSBLevel0
	printf("Leo_USB => Cantida de datos leidos %d \n",size);
	#endif	
	
	#ifdef DEBUGUSBLevel2
	for(i=0;i<size;i++)
	printf("Leo_USB => Buffer[%d]=%X \n",i,Buffer[i]);		   
	#endif	
	return rc;
}


