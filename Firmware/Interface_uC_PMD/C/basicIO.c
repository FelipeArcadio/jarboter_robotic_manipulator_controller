 
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "basicIO.h"

/*-----------------------Funciones Interface uControlador-PMD´s--------------------------*/

void writePMDcommand(PMDuint16 PMDChipSet, PMDuint16 dataword)
{

	if(PMDChipSet==PMDChipSet1)
	{
		PORTD			|=	_BV(HostSlct1);
		HostControl 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		
	
		HostControl 	&= 	~(_BV(HostCmd));						//HostCmd Bajo, Condiciones iniciales
		Control_Port	=	0xFF;	  								//Coloco como salida el puerto 
		PORTD		 	&= 	~(_BV(HostSlct1));						//Selecciono el Chipset 
		HostControl 	|= 	_BV(HostCmd);							//Coloco la Señal HostCmd a Nivel Alto
	
		HostData		= 	(PMDuint8)((dataword>>8) & 0xFF);		//Envío por el puerto de datos el byte mas 
																	//significativo  
		HostControl		&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado.
																	//Nota: trabajando a una velocidad de 8MHz se tiene 
		_delay_us(2);
		HostControl 	|= 	_BV(HostWrite);							//que el ciclo de cada instruccion del uC es de 125ns
																	// que es suficiente para relaizar el ciclo de escritura. 
		HostData		=	(PMDuint8)(dataword&0xFF);				//Envío por el Puerto De datos el Byte menos signiicativo 
		HostControl		&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. 
																	//Nota: trabajando a una velocidad de 8MHz se tiene
		_delay_us(2);
		HostControl 	|= 	_BV(HostWrite);							// que el ciclo de cada instruccion del uC es de 125ns
																	// que es suficiente para relaizar el ciclo de escritura.  
		
		HostControl 	&= 	~(_BV(HostCmd));						//Coloca la señal de HostCmd a Nivel bajo
		PORTD		 	|= 	_BV(HostSlct1);							//Deselecciono el Chipset. NOta Para la version 
																	//con varios chipset se debe utilizarel puerto con una
																	// mascara para selecion y deseleccion respectivamente
	}
	
	if(PMDChipSet==PMDChipSet2)
	{
		PORTD			|=	_BV(HostSlct2);
		HostControl 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		
	
		HostControl 	&= 	~(_BV(HostCmd));						//HostCmd Bajo, Condiciones iniciales
		Control_Port	=	0xFF;	  								//Coloco como salida el puerto 
		PORTD		 	&= 	~(_BV(HostSlct2));						//Selecciono el Chipset 
		HostControl 	|= 	_BV(HostCmd);							//Coloco la Señal HostCmd a Nivel Alto
	
		HostData		= 	(PMDuint8)((dataword>>8) & 0xFF);		//Envío por el puerto de datos el byte mas 
																	//significativo  
		HostControl		&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado.
																	//Nota: trabajando a una velocidad de 8MHz se tiene 
		_delay_us(2);
		HostControl 	|= 	_BV(HostWrite);							//que el ciclo de cada instruccion del uC es de 125ns
																	// que es suficiente para relaizar el ciclo de escritura. 
		HostData		=	(PMDuint8)(dataword&0xFF);				//Envío por el Puerto De datos el Byte menos signiicativo 
		HostControl		&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. 
																	//Nota: trabajando a una velocidad de 8MHz se tiene
		_delay_us(2);
		HostControl 	|= 	_BV(HostWrite);							// que el ciclo de cada instruccion del uC es de 125ns
																	// que es suficiente para relaizar el ciclo de escritura.  
		
		HostControl 	&= 	~(_BV(HostCmd));						//Coloca la señal de HostCmd a Nivel bajo
		PORTD		 	|= 	_BV(HostSlct2);							//Deselecciono el Chipset. NOta Para la version 
																	//con varios chipset se debe utilizarel puerto con una
																	// mascara para selecion y deseleccion respectivamente
	}	
	
	if(PMDChipSet==PMDChipSet3)
	{
		PORTD			|=	_BV(HostSlct3);
		HostControl 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		
	
		HostControl 	&= 	~(_BV(HostCmd));						//HostCmd Bajo, Condiciones iniciales
		Control_Port	=	0xFF;	  								//Coloco como salida el puerto 
		PORTD		 	&= 	~(_BV(HostSlct3));						//Selecciono el Chipset 
		HostControl 	|= 	_BV(HostCmd);							//Coloco la Señal HostCmd a Nivel Alto
	
		HostData		= 	(PMDuint8)((dataword>>8) & 0xFF);		//Envío por el puerto de datos el byte mas 
																	//significativo  
		HostControl		&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado.
																	//Nota: trabajando a una velocidad de 8MHz se tiene 
		_delay_us(2);
		HostControl 	|= 	_BV(HostWrite);							//que el ciclo de cada instruccion del uC es de 125ns
																	// que es suficiente para relaizar el ciclo de escritura. 
		HostData		=	(PMDuint8)(dataword&0xFF);				//Envío por el Puerto De datos el Byte menos signiicativo 
		HostControl		&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. 
																	//Nota: trabajando a una velocidad de 8MHz se tiene
		_delay_us(2);
		HostControl 	|= 	_BV(HostWrite);							// que el ciclo de cada instruccion del uC es de 125ns
																	// que es suficiente para relaizar el ciclo de escritura.  
		
		HostControl 	&= 	~(_BV(HostCmd));						//Coloca la señal de HostCmd a Nivel bajo
		PORTD		 	|= 	_BV(HostSlct3);							//Deselecciono el Chipset. NOta Para la version 
																	//con varios chipset se debe utilizarel puerto con una
																	// mascara para selecion y deseleccion respectivamente
	}	

}
/*----------------------------------------------------------------------------------------------------*/
void writePMDdata(PMDuint16 PMDChipSet, PMDuint16 dataword)
{

	if(PMDChipSet==PMDChipSet1)
	{	
		PORTD				|=	_BV(HostSlct1);
		HostControl		 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		

		Control_Port		=	0xFF;			  						//Coloco como salida el puerto 

		HostControl 		|= 	_BV(HostCmd);							//HostCmd Alto Condiciones iniciales
		PORTD		 		&= 	~(_BV(HostSlct1));						//Selecciono el Chipset 
		HostControl 		&= 	~(_BV(HostCmd));			 			//Coloco la Señal HostCmd a Nivel bajo
		HostData			= 	(PMDuint8)((dataword>>8)&0xFF);		//Envío por el puerto de datos el byte mas significativo  
		HostControl			&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. 
																		//Nota: trabajando a una velocidad de 8MHz se tiene
		_delay_us(2);
		HostControl 		|= 	_BV(HostWrite);							// que el ciclo de cada instruccion del uC es de 125ns
																		// que es suficiente para relaizar el ciclo de escritura. 
		HostData			=	(PMDuint8)(dataword&0xFF);				//Envío por el Puerto e datos el Byte menos signiicativo 
		HostControl			&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. Nota: trabajando 
																		//a una velocidad de 8MHz se tiene que el ciclo de cada 
		_delay_us(2);
		HostControl 		|= 	_BV(HostWrite);							//instruccion del uC es de 125ns que es suficiente para
																		// relaizar el ciclo de escritura.  
	
		HostControl 		|= 	_BV(HostCmd);							//Coloca la señal de HostCmd a Nivel Alto
		PORTD		 		|= 	_BV(HostSlct1); 						//Deselecciono el Chipset. NOta Para la version con
																		//varios chipset se debe utilizarel puerto con una mascara
																		// para sellecion y deseleccion respectivamente
	}

	if(PMDChipSet==PMDChipSet2)
	{	
		PORTD				|=	_BV(HostSlct2);
		HostControl		 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		

		Control_Port		=	0xFF;			  						//Coloco como salida el puerto 

		HostControl 		|= 	_BV(HostCmd);							//HostCmd Alto Condiciones iniciales
		PORTD		 		&= 	~(_BV(HostSlct2));						//Selecciono el Chipset 
		HostControl 		&= 	~(_BV(HostCmd));			 			//Coloco la Señal HostCmd a Nivel bajo
		HostData			= 	(PMDuint8)((dataword>>8)&0xFF);		//Envío por el puerto de datos el byte mas significativo  
		HostControl			&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. 
																		//Nota: trabajando a una velocidad de 8MHz se tiene
		_delay_us(2);
		HostControl 		|= 	_BV(HostWrite);							// que el ciclo de cada instruccion del uC es de 125ns
																		// que es suficiente para relaizar el ciclo de escritura. 
		HostData			=	(PMDuint8)(dataword&0xFF);				//Envío por el Puerto e datos el Byte menos signiicativo 
		HostControl			&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. Nota: trabajando 
																		//a una velocidad de 8MHz se tiene que el ciclo de cada 
		_delay_us(2);
		HostControl 		|= 	_BV(HostWrite);							//instruccion del uC es de 125ns que es suficiente para
																		// relaizar el ciclo de escritura.  
	
		HostControl 		|= 	_BV(HostCmd);							//Coloca la señal de HostCmd a Nivel Alto
		PORTD		 		|= 	_BV(HostSlct2); 						//Deselecciono el Chipset. NOta Para la version con
																		//varios chipset se debe utilizarel puerto con una mascara
																		// para sellecion y deseleccion respectivamente
	}	

	if(PMDChipSet==PMDChipSet3)
	{	
		PORTD				|=	_BV(HostSlct3);
		HostControl		 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		

		Control_Port		=	0xFF;			  						//Coloco como salida el puerto 

		HostControl 		|= 	_BV(HostCmd);							//HostCmd Alto Condiciones iniciales
		PORTD		 		&= 	~(_BV(HostSlct3));						//Selecciono el Chipset 
		HostControl 		&= 	~(_BV(HostCmd));			 			//Coloco la Señal HostCmd a Nivel bajo
		HostData			= 	(PMDuint8)((dataword>>8)&0xFF);		//Envío por el puerto de datos el byte mas significativo  
		HostControl			&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. 
																		//Nota: trabajando a una velocidad de 8MHz se tiene
		_delay_us(2);
		HostControl 		|= 	_BV(HostWrite);							// que el ciclo de cada instruccion del uC es de 125ns
																		// que es suficiente para relaizar el ciclo de escritura. 
		HostData			=	(PMDuint8)(dataword&0xFF);				//Envío por el Puerto e datos el Byte menos signiicativo 
		HostControl			&= 	~(_BV(HostWrite));						//Realizo la escritura del byte enviado. Nota: trabajando 
																		//a una velocidad de 8MHz se tiene que el ciclo de cada 
		_delay_us(2);
		HostControl 		|= 	_BV(HostWrite);							//instruccion del uC es de 125ns que es suficiente para
																		// relaizar el ciclo de escritura.  
	
		HostControl 		|= 	_BV(HostCmd);							//Coloca la señal de HostCmd a Nivel Alto
		PORTD		 		|= 	_BV(HostSlct3); 						//Deselecciono el Chipset. NOta Para la version con
																		//varios chipset se debe utilizarel puerto con una mascara
																		// para sellecion y deseleccion respectivamente
	}	
}

/*----------------------------------------------------------------------------------------------------*/
PMDuint16 readPMDdata(PMDuint16 PMDChipSet)
{
	PMDuint16 i			=	0;
	PMDuint16 j			=	0;

	if(PMDChipSet==PMDChipSet1)
	{
		PORTD			|=	_BV(HostSlct1);
		HostControl 	|=	_BV(HostWrite) | _BV(HostRead);					// Condiciones Iniciales		

	


		Control_Port		=	0x00;			  						//Coloco como entrada el puerto 
		HostData			=	0xFF;									//Resistencias de Pullups ( averiguar mas esto)

		HostControl 		|=	_BV(HostCmd);							//HostCmd Alto. Condiciones Iniciales
		PORTD		 		&= 	~(_BV(HostSlct1));						//Selecciono el Chipset 
		HostControl 		&= 	~(_BV(HostCmd));						//Coloco la Señal HostCmd a Nivel bajo 
	
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		i					=	(ReadData & 0xFF)<<8;					//Hago corriminto hacia la izq en i de los
																		//datos leídos en el puerto 
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead
		_delay_us(2);

		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		j					=	(ReadData & 0xFF);						//Asigno a j el valor leído en el Puerto de Datos
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead

	
		HostControl 		|= _BV(HostCmd);							//Coloca la señal de HostCmd a Nivel Alto
		PORTD		 		|= _BV(HostSlct1); 							//Deselecciono el Chipset.
	
	
		
	}

	if(PMDChipSet==PMDChipSet2)
	{
		PORTD			|=	_BV(HostSlct2);
		HostControl 	|=	_BV(HostWrite) | _BV(HostRead);					// Condiciones Iniciales		

	


		Control_Port		=	0x00;			  						//Coloco como entrada el puerto 
		HostData			=	0xFF;									//Resistencias de Pullups ( averiguar mas esto)

		HostControl 		|=	_BV(HostCmd);							//HostCmd Alto. Condiciones Iniciales
		PORTD		 		&= 	~(_BV(HostSlct2));						//Selecciono el Chipset 
		HostControl 		&= 	~(_BV(HostCmd));						//Coloco la Señal HostCmd a Nivel bajo 
	
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		i					=	(ReadData & 0xFF)<<8;					//Hago corriminto hacia la izq en i de los
																		//datos leídos en el puerto 
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead
		_delay_us(2);

		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		j					=	(ReadData & 0xFF);						//Asigno a j el valor leído en el Puerto de Datos
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead

	
		HostControl 		|= _BV(HostCmd);							//Coloca la señal de HostCmd a Nivel Alto
		PORTD		 		|= _BV(HostSlct2); 							//Deselecciono el Chipset.
	
	
		
	}
	
	if(PMDChipSet==PMDChipSet3)
	{
		PORTD			|=	_BV(HostSlct3);
		HostControl 	|=	_BV(HostWrite) | _BV(HostRead);					// Condiciones Iniciales		

	


		Control_Port		=	0x00;			  						//Coloco como entrada el puerto 
		HostData			=	0xFF;									//Resistencias de Pullups ( averiguar mas esto)

		HostControl 		|=	_BV(HostCmd);							//HostCmd Alto. Condiciones Iniciales
		PORTD		 		&= 	~(_BV(HostSlct3));						//Selecciono el Chipset 
		HostControl 		&= 	~(_BV(HostCmd));						//Coloco la Señal HostCmd a Nivel bajo 
	
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		i					=	(ReadData & 0xFF)<<8;					//Hago corriminto hacia la izq en i de los
																		//datos leídos en el puerto 
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead
		_delay_us(2);

		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		j					=	(ReadData & 0xFF);						//Asigno a j el valor leído en el Puerto de Datos
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead

	
		HostControl 		|= _BV(HostCmd);							//Coloca la señal de HostCmd a Nivel Alto
		PORTD		 		|= _BV(HostSlct3); 							//Deselecciono el Chipset.
	
	
		
	}
	
	
	return i | j;
}  

/*----------------------------------------------------------------------------------------------------*/
PMDuint16 readPMDstatus(PMDuint16 PMDChipSet)
{
	PMDuint16 i			=	0;
	PMDuint16 j			=	0;
	
	if(PMDChipSet==PMDChipSet1)
	{
		PORTD				|=	_BV(HostSlct1);
		HostControl		 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		
	
		


		Control_Port		=	0x00;									//Coloco como entrada el puerto A
		HostData			=	0xFF;   								//Resistencias de Pullups ( averiguar mas esto)
	
		HostControl 		&= 	~(_BV(HostCmd));						//HostCmd Bajo.Condiciones Iniciales
		PORTD		 		&= 	~(_BV(HostSlct1));						//Selecciono el Chipset 
		HostControl 		|= 	_BV(HostCmd);							//Coloco la Señal HostCmd a Nivel Alto 
	
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		i					=	(ReadData & 0xFF)<<8;					//Hago corriminto hacia la izq en i de los datos
																		//leídos en el puerto 
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead
		_delay_us(2);
		
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(1);													//retardo de un 1uS para asegurar la lecltura del dato
		j					=	(ReadData & 0xFF);						//Asigno a j el valor leído en el Puerto de Datos
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead  
	
	
		HostControl 		&= 	~(_BV(HostCmd));						//Coloca la señal de HostCmd a Nivel Bajo  
		PORTD		 		|= 	_BV(HostSlct1); 						//Deselecciono el Chipset

		
	}
	
		if(PMDChipSet==PMDChipSet2)
	{
		PORTD				|=	_BV(HostSlct2);
		HostControl		 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		
	
		


		Control_Port		=	0x00;									//Coloco como entrada el puerto A
		HostData			=	0xFF;   								//Resistencias de Pullups ( averiguar mas esto)
	
		HostControl 		&= 	~(_BV(HostCmd));						//HostCmd Bajo.Condiciones Iniciales
		PORTD		 		&= 	~(_BV(HostSlct2));						//Selecciono el Chipset 
		HostControl 		|= 	_BV(HostCmd);							//Coloco la Señal HostCmd a Nivel Alto 
	
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		i					=	(ReadData & 0xFF)<<8;					//Hago corriminto hacia la izq en i de los datos
																		//leídos en el puerto 
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead
		_delay_us(2);
		
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(1);													//retardo de un 1uS para asegurar la lecltura del dato
		j					=	(ReadData & 0xFF);						//Asigno a j el valor leído en el Puerto de Datos
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead  
	
	
		HostControl 		&= 	~(_BV(HostCmd));						//Coloca la señal de HostCmd a Nivel Bajo  
		PORTD		 		|= 	_BV(HostSlct2); 						//Deselecciono el Chipset

		
	}

	if(PMDChipSet==PMDChipSet3)
	{
		PORTD				|=	_BV(HostSlct3);
		HostControl		 	|=	_BV(HostWrite) | _BV(HostRead);				// Condiciones Iniciales		
	
		


		Control_Port		=	0x00;									//Coloco como entrada el puerto A
		HostData			=	0xFF;   								//Resistencias de Pullups ( averiguar mas esto)
	
		HostControl 		&= 	~(_BV(HostCmd));						//HostCmd Bajo.Condiciones Iniciales
		PORTD		 		&= 	~(_BV(HostSlct3));						//Selecciono el Chipset 
		HostControl 		|= 	_BV(HostCmd);							//Coloco la Señal HostCmd a Nivel Alto 
	
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(2);													//retardo de un 1uS para asegurar la lecltura del dato
		i					=	(ReadData & 0xFF)<<8;					//Hago corriminto hacia la izq en i de los datos
																		//leídos en el puerto 
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead
		_delay_us(2);
		
		HostControl			&= 	~(_BV(HostRead));						//Aserto la señal de ~HostRead
		_delay_us(1);													//retardo de un 1uS para asegurar la lecltura del dato
		j					=	(ReadData & 0xFF);						//Asigno a j el valor leído en el Puerto de Datos
		HostControl 		|= 	_BV(HostRead);							//des_Aserto la señal de ~HostRead  
	
	
		HostControl 		&= 	~(_BV(HostCmd));						//Coloca la señal de HostCmd a Nivel Bajo  
		PORTD		 		|= 	_BV(HostSlct3); 						//Deselecciono el Chipset

		
	}
	

		return i | j;
}
/*----------------------------------------------------------------------------------------------------*/
void writePMDreset(void)  
{
	PORTE 	|= _BV(Reset);												//reset alto
	PORTE 	&= ~(_BV(Reset));											//bajo reset
	_delay_us(200);
	PORTE 	|= _BV(Reset);												//reset alto
	_delay_ms(40);                     									//TIEMPO TOMADO DE C_MOTION ORIGINAL	 
	
}

