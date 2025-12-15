#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "PMDpar.h"
#include "uart.h"
#include "messages.h"
#include "debug.h"


// undefine __TIME_WAITFORREADY if it is not desired to have a timeout while waiting for the ready signal
#define __TIME_WAITFORREADY
#ifdef 	__TIME_WAITFORREADY
#include <util/delay.h>
#endif



// only include this if we are running in diagnostics mode
//#include "PMDdiag.h"  


/**	\ingroup PMDpar 
	\brief Función que envía una instrucción a los chips PMDs en el modo 8/16 @see Manual Técnico PMDs
	
	@param PMDChipSet- Corresponde al Juego de chipsets al que se le envía el Comando
	@param dataword- Es el comando o instrucción que se le mandará a los chips PMDs*/
static void OutP8BitCmd(PMDuint16 PMDChipSet, PMDuint16 dataword);

/**	\ingroup PMDpar 
	\brief Función que envía un dato a los chips PMDs en el modo 8/16 @see Manual Técnico PMDs
	
	@param PMDChipSet- Corresponde al Juego de chipsets al que se le envía el Dato
	@param dataword- Es el Dato  que se le mandará a los chips PMDs*/
static void OutP8Bit(PMDuint16 PMDChipSet, PMDuint16 dataword);

/**	\ingroup PMDpar 
	\brief Función que Recibe un dato de los chips PMDs en el modo 8/16 @see Manual Técnico PMDs
	
	@param PMDChipSet- Corresponde al Juego de chipsets del que se recibe el dato
	@return Dato de 16 Bits que se lee de los chips PMDs*/
static PMDuint16 InP8Bit(PMDuint16 PMDChipSet);

/**	\ingroup PMDpar 
	\brief Función que realiza una operación de Lectura de Estado en el chip PMD
	
	@param PMDChipSet- Corresponde al Juego de chipsets al que se le realiza la operación
	@return Dato de 16 Bits que corresponde al EStado actual de los chips PMDs 
	@see Manual Técnico*/
static PMDuint16 InP8BitStatus(PMDuint16 PMDChipSet);

/** \ingroup PMDpar
	\brief Lectura del registro del Estado del chip PMD.
	 Permite al usuario determinar el estado de algunas señales de la interface del Host con 
	 el chipset sin tener que desarrollar una lógica de decodificación especial.
	 
	 Una operación de Status Read se desarrolla seleccionando ya sea PMD1, PMD2 ó PMD3, luego
	 acertar las señales  ~HostRead y ~HostSlct bajo, HostCmd alto, y 
	 leer el bus de datos.*/
static PMDuint16 PMDParallel_GetStatus(void* transport_data);

/** \ingroup PMDpar
	\brief Lectura del Bit 15 (señal HostRdy) del Registro de EStado del Chip PMD*/
static PMDuint16 PMDParallel_IsReady(void* transport_data);

/** \ingroup PMDpar
	\brief Lectura del Bit 14 (señal ~HostIntrp) del Registro de EStado del Chip PMD*/
static PMDuint16 PMDParallel_HasInterrupt(void* transport_data);

/** \ingroup PMDpar
	\brief Lectura del Bit 13 (señal HostIOError) del Registro de EStado del Chip PMD*/
static PMDuint16 PMDParallel_HasError(void* transport_data);

/** \ingroup PMDpar
	\brief Espera por el siguiente Comando hasta que el Chip PMD esté disponible
	(la señal HostRdy debe ser "ALTA")*/
static PMDuint16 PMDParallel_WaitForReady(PMDParallelIOTransportData* transport_data);


/** \ingroup PMDpar
	\brief Lectura de Datos por un puerto de 8 bits de los Chips PMDs
	@param lenght-Números de bytes que se leerán
	@param buffer- Buffer de almacenamiento de datos*/
static PMDuint16 PMDParallel_ReceiveResponse(PMDParallelIOTransportData* transport_data, PMDuint16 length, PMDuint16 buffer[]);

/** \ingroup PMDpar
	\brief Esta función realiza:
	1.Envió de comando/datos a los chips PMDs
	2.Recepción de Datos 
	3. Checksum de datos
	@param XCt- Número de Datos (de 16Bits) que se enviarán a los chips
	@param XDat- Buffer que contiene los datos que serán transmitidos. 
		   Incluye también el Comando
	@param RCt-Número de Datos (de 16Bits) que serán recibidos por el Host de los Chipsets
	@param rDat-Buffer para el almacenamiento de los datos que se reciben*/
static PMDuint16 PMDParallel_Send(PMDParallelIOTransportData* transport_data, 
							  PMDuint8 xCt, PMDuint16* xDat, PMDuint8 rCt, PMDuint16* rDat);
							  
/** \ingroup PMDpar
	\brief Libera espacio que fué asignado a la estructura PMDParallelIOTransportData*/							  
static void PMDParallel_Close(void* transport_data);

////////////funciones agregadas por david
static void OutP8BitHard(void);
static PMDuint16 PMDParallel_HardReset(void* transport_data); 



//**********************************************************************************************
int16 Axis2PMDChipset(tAxis Axis) 
{
	switch(Axis) 
	{                              		// name on connector board:
		//PMDChipSet1 Waist=0
		case AxisWaist 			: 	return PMDChipSet1;      	// cintura
		case AxisShoulder 		: 	return PMDChipSet1;   		// hombro
		case AxisElbow 			: 	return PMDChipSet1;      	// codo
		case AxisPitch 			: 	return PMDChipSet1;      	// pitch
		
		//PMDChipSet2
		case AxisRoll 			: 	return PMDChipSet2;       	// roll esta en otro chipset 
		case AxisAux1 			: 	return PMDChipSet2;       	// aux1
		case AxisAux2 			: 	return PMDChipSet2;       	// aux2
		case AxisAux3 			: 	return PMDChipSet2;       	// aux3
		//PMDChipSet3
		case AxisAux4 			: 	return PMDChipSet3;       	// aux4
		case AxisAux5 			: 	return PMDChipSet3;       	// aux5
		case AxisAux6 			: 	return PMDChipSet3;       	// aux6
		case AxisAux7 			: 	return PMDChipSet3;       	// aux7
		
		default 					: 	return PMDChipSet1;
	}
  return PMDChipSet1;  // we must return something to avoid compiler warning
} // Axis2PMDChipset

//**********************************************************************************************
// mapping which (logical) axis is to find on which chipset axis:

int16 Axis2PMDAxis(tAxis Axis) 
{
	switch(Axis) 
	{
		case AxisWaist 		: 	return PMDAxis1; //0
		case AxisShoulder 	: 	return PMDAxis2; //1
		case AxisElbow 		:	return PMDAxis3; //2
		case AxisPitch 		: 	return PMDAxis4; //3		
		
		case AxisRoll 		: 	return PMDAxis1; //0
		case AxisAux1 		: 	return PMDAxis2; //1		
		case AxisAux2 		: 	return PMDAxis3;    	
		case AxisAux3 		: 	return PMDAxis4;       
		
		case AxisAux4 		: 	return PMDAxis1;       
		case AxisAux5 		: 	return PMDAxis2;       
		case AxisAux6 		: 	return PMDAxis3;       
		case AxisAux7 		: 	return PMDAxis4;      	
		default 			:   return PMDAxis1;
	}
  return PMDAxis1;  // we must return something
} // Axis2PMDAxis


//********************************************************************************************************************
void OutP8BitCmd(PMDuint16 PMDChipSet, PMDuint16 dataword) 
{
  writePMDcommand(PMDChipSet,dataword);   // low-level function, provided by basicIO.h
}

//******************************************************************************************************************** 
void OutP8BitHard(void) 
{
  writePMDreset(); 
}
 
 
 //********************************************************************************************************************
void OutP8Bit(PMDuint16 PMDChipSet, PMDuint16 dataword) 
{
  writePMDdata(PMDChipSet,dataword);
}

//********************************************************************************************************************
PMDuint16 InP8Bit(PMDuint16 PMDChipSet) 
{
  return readPMDdata(PMDChipSet);
}
//********************************************************************************************************************
PMDuint16 InP8BitStatus(PMDuint16 PMDChipSet) 
{
  return readPMDstatus(PMDChipSet);
}

 
//********************************************************************************************************************


PMDuint16 PMDParallel_GetStatus(void* transport_data)
{
	PMDParallelIOTransportData* PIOtransport_data = (PMDParallelIOTransportData*)transport_data;

	return PIOtransport_data->InPStatus(PIOtransport_data->dcs_Port);
}
	
//********************************************************************************************************************
PMDuint16 PMDParallel_IsReady(void* transport_data)
{
	PMDParallelIOTransportData* PIOtransport_data = (PMDParallelIOTransportData*)transport_data;

	return ((PIOtransport_data->InPStatus(PIOtransport_data->dcs_Port)
		& PIOtransport_data->readyMask) == PIOtransport_data->readyValue);
}

//********************************************************************************************************************				 
PMDuint16 PMDParallel_HasInterrupt(void* transport_data)
{
	PMDParallelIOTransportData* PIOtransport_data = (PMDParallelIOTransportData*)transport_data;

	return ((PIOtransport_data->InPStatus(PIOtransport_data->dcs_Port) 
		& PIOtransport_data->hostInterruptMask) == PIOtransport_data->hostInterruptValue);
}
		
//********************************************************************************************************************
PMDuint16 PMDParallel_HasError(void* transport_data)
{
	PMDParallelIOTransportData* PIOtransport_data = (PMDParallelIOTransportData*)transport_data;

	return ((PIOtransport_data->InPStatus(PIOtransport_data->dcs_Port) 
		& PIOtransport_data->commandStatusMask) == PIOtransport_data->commandStatusValue);
}

		
//********************************************************************************************************************		



PMDuint16 PMDParallel_HardReset(void* transport_data)
{

    PMDParallelIOTransportData* PIOtransport_data = (PMDParallelIOTransportData*)transport_data;
	//le debe de entar transport_data por que proviene 
	//de una axis ya inicualizada correctamente
	PIOtransport_data->Hard();
 
    return PMD_NOERROR;
    
}

//********************************************************************************************************************				 	
//Esta Funcion Verifica que el Dato retornado por la Funcion Read_Status_Register contenga en el Bit 15 
//(HostRdy signal) un Valor igual a 1, indicando al HOST que El PMD esta Listo para Operaciones.

PMDuint16 PMDParallel_WaitForReady(PMDParallelIOTransportData* transport_data)
{
	PMDuint16 in_val;
    
#ifdef __TIME_WAITFORREADY
		PMDuint16 timer;
		
  		timer 		= 12000; 																      
		
		for(;;) 
		{
			// poll ready port, if not ready, loop
    		in_val = transport_data->InPStatus(transport_data->dcs_Port);
    		if ((in_val != 0xFFFF) && ((in_val & transport_data->readyMask) == transport_data->readyValue))
				return PMD_ERR_OK;
			else// not ready 
			{ 
				timer--;
				if(timer == 0) 
				{
					/* se enciende el LED Rojo*/
					Led_on(Rojo);
					printf("PMDParallel_WaitForReady=> 40ms Timeout Error\n");
					return PMD_ERR_CommTimeoutError;
				} // if timer == 0
			}
  		} // for EVER   
#else
    // wait indefinitely
    for(;;)
    {
        // poll ready port, if not ready, loop
        in_val = transport_data->InPStatus(transport_data->dcs_Port);
		
        if ((in_val != 0xFFFF) 
			&& ((in_val & transport_data->readyMask) == transport_data->readyValue))
                return PMD_ERR_OK;
    }
#endif
} 
//********************************************************************************************************************				 	
// wait for the PMD´s to be ready for the next command
PMDuint16 PMDParallel_GetCommandStatus(PMDParallelIOTransportData* transport_data)
{
	PMDuint16 result;
	int16     in_val;

	if((result = PMDParallel_WaitForReady(transport_data)) != PMD_ERR_OK)
	{
	    printf("PMDParallel_GetCommandStatus=> Wait Status Register Timeout \n");    
	    return result;
	}	


	//En este punto Verifica que el Dato retornado por la Funcion Read_Status_Register contenga en el Bit 13 
	//(HostIOError signal) un Valor igual a 1, indicando  que El PMD contiene un HOST I/O Error (ver seccion 
	//10.3.3 del Manual de Usuario). 
	
	in_val = transport_data->InPStatus(transport_data->dcs_Port);
	if ((in_val & transport_data->commandStatusMask) == transport_data->commandStatusValue)
	{
		if (transport_data->bDiagnostics)
				printf("PMDParallel_GetCommandStatus=> C-Motion: Command Error bit set.\n");
		return PMD_ERR_CommandError;
	}
	return PMD_ERR_OK;
} 

//********************************************************************************************************************				 	
PMDuint16 PMDParallel_ReceiveResponse(PMDParallelIOTransportData* transport_data, 
															 PMDuint16 length, PMDuint16 buffer[])
{
        PMDuint16 result;
        PMDuint16 index;

       result = PMD_ERR_PMDNotInitialized;

        // get the data from the jarboter Controller
        for(index=0; index<length; index++)
        {
            if((result = PMDParallel_WaitForReady(transport_data)) != PMD_ERR_OK)
			{
			    printf("PMDParallel_ReceiveResponse=> wait for Cheksum timeout \n");		
                break;
			}
            
			buffer[index] = transport_data->InPData(transport_data->dcs_Port);
        }

        return result;
}	


//********************************************************************************************************************				 	
// send the command and data to the chips PMDs

//XCt=Numero de DAtos a Enviar
//XDatPuntero que Contiene los Datos a enviar
//RCt=Numero de Datos a Recibir
//rDat=Puntero de almacena  los Datos a Recibir

//result=MD_ERR_CommTimeoutError o  PMD_ERR_OK

PMDuint16 PMDParallel_Send(PMDParallelIOTransportData* transport_data, 
							  PMDuint8 xCt, PMDuint16* xDat, PMDuint8 rCt, PMDuint16* rDat)
{

    PMDuint16 result;
	PMDuint16 commandstatus = PMD_ERR_OK;
    PMDuint16 index;
    long messageChecksum=0;
    PMDuint16 chipsetChecksum=0;

	result=PMD_ERR_PMDNotInitialized;
///////////////////////////////ESCRIBE UN COMANDO/////////////////////////////////////////  
    if((result = PMDParallel_WaitForReady(transport_data)) == PMD_ERR_OK) //Esta readey
    {  
		// put the command into the PMD1/PMD2/PMD3
		transport_data->OutPCmd(transport_data->dcs_Port, xDat[0]);
		
		// put the data into the PMD1/PMD2/PMD3
		// one word at a time
        for(index=1; index<xCt; index++)
        {
			if((result = PMDParallel_WaitForReady(transport_data)) != PMD_ERR_OK)
				break;
			
			transport_data->OutPData(transport_data->dcs_Port,xDat[index]);
        }
		
		if(result == PMD_ERR_OK)
			commandstatus = PMDParallel_GetCommandStatus(transport_data);
		
		
    }
	else
	   printf("PMDParallel_Send=> Error Envio Datos o Comando \n");    
	//El comando fue un exito
	//El commandstatus por defaut despues de reset tiene un (1) en campo de error
	//Pero si ejecutamos el coammand PMDGetHostIOError este se vuelve (0)
	//Se debe de limpiar esta bandera antes de ejecutar cuanlquier commando, pues
	//no tiene sentido que halla error seguido de un reset.
	
	
	/////////////////////////////LEE DATOS //////////////////////////////
    // get the data from the DK2000
    for(index=0; index<rCt; index++)
    {
        if((result = PMDParallel_WaitForReady(transport_data)) != PMD_ERR_OK)
	    {
	       printf("PMDParallel_Send=> Error datos recibidos\n");    
          break;
		} 
        
		rDat[index] = transport_data->InPData(transport_data->dcs_Port);
    }
///////////////////////////VERIFICACIONES/////////////////////////////////    
    if(transport_data->bVerifyChecksum)
	{
		for(index=0; index<xCt; index++)
			messageChecksum += xDat[index];
		  
		for(index=0; index<rCt; index++)
			messageChecksum += rDat[index];
		 
		messageChecksum = messageChecksum & 0xFFFF;
		
		result = PMDParallel_ReceiveResponse(transport_data, 1, &chipsetChecksum);
		if ( result == PMD_ERR_OK && messageChecksum != chipsetChecksum )
		{			
			if (transport_data->bDiagnostics)
			{	
				printf("PMDParallel_Send=> Checksum failure.  expected: %ld  got: %x\n",messageChecksum,chipsetChecksum);
				
				printf("PMDParallel_Send=> C-Motion: %s ",PMDGetOpcodeText(xDat[0]));
				
				for(index=0; index<xCt; index++)
					printf("%X ",xDat[index]);
				
				printf("\n");
		    }  
			return PMD_ERR_ChecksumError;
		}
	}
	
	if (commandstatus!=PMD_ERR_OK)
	{
		if (transport_data->bDiagnostics)
		{
			printf("PMDParallel_Send=> Host I/O Error \n");
			printf("PMDParallel_Send=> C-Motion: %s ",PMDGetOpcodeText(xDat[0]));
			
			for(index=0; index<xCt; index++)
				printf("%X ",xDat[index]);
			
			printf("\n");
		}
		return commandstatus;
	}
	
	return result;
}



//********************************************************************************************************************				 	 
void PMDParallel_Close(void* transport_data)
{
    free(transport_data);
}	
//********************************************************************************************************************				 	 
void PMDParallel_InitData(PMDParallelIOTransportData* transport_data, int16 IOMode)
{
	// assign default values


	transport_data->readyMask = 0x8000;
	transport_data->readyValue = 0x8000;
	transport_data->hostInterruptMask = 0x4000;
	transport_data->hostInterruptValue = 0x4000;
	transport_data->commandStatusMask = 0x2000;
	transport_data->commandStatusValue = 0x2000;

	// by default always verify the checksum
	transport_data->bVerifyChecksum = 1;
	// by default disable diagnostics
	// Modificado por David
	// Activo los diagnosticos
	transport_data->bDiagnostics = 1;

	// assign default handlers/masks according to IO mode
	switch (IOMode)
	{
	case PMDParallelIOMode_8_16:
		transport_data->OutPData = OutP8Bit ;
		transport_data->OutPCmd = OutP8BitCmd;
		transport_data->InPData = InP8Bit;
		transport_data->InPStatus = InP8BitStatus;
		transport_data->Hard = OutP8BitHard;
		break;	
	}
}


//********************************************************************************************************************				 	 
void PMDParallel_Init(PMDIOTransport* transport)
{
	// setup function pointers
	transport->SendCommand = PMDParallel_Send;
	transport->GetStatus = PMDParallel_GetStatus;
	transport->IsReady = PMDParallel_IsReady;
	transport->HasInterrupt = PMDParallel_HasInterrupt;
	transport->HasError = PMDParallel_HasError;
  	transport->HardReset = PMDParallel_HardReset;

	transport->Close = PMDParallel_Close;
} 

//********************************************************************************************************************	

PMDuint16 PMDSetupAxisInterface_Parallel(PMDAxisHandle* axis_handle, PMDAxis axis_number, PMDuint16 PMDChipSet)
{
	PMDParallelIOTransportData* transport_data;
	
	
	transport_data = (PMDParallelIOTransportData*) malloc( sizeof( PMDParallelIOTransportData ) );
	memset(transport_data, 0, sizeof(PMDParallelIOTransportData));
    
	if(transport_data == 0) return rcNoMemory;  // check if operation succeeded

	// set the axis we are talking to with this handle
	axis_handle->axis = axis_number;
	

	// if required the transport data defaults can be changed here
	// e.g. board IO address (dataPort,commandPort,statusPort)
	//transport_data->dataPort = board_address;
	transport_data->dcs_Port = PMDChipSet;  // actual port is treated in low-level function in basicIO.h
	axis_handle->chipset=PMDChipSet;

	// the transport data is initialized first to setup the defaults
	// make sure the IO mode is set correctly
	PMDParallel_InitData(transport_data, PMDParallelIOMode_8_16);

	axis_handle->transport_data = (void*) transport_data;

	// initialize the transport (inits function pointers)
	PMDParallel_Init(&axis_handle->transport);

 return rcOK;
}

