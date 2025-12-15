#if !defined _WIN32
#include <avr/io.h>
#endif

#ifdef _AVR_IO_H_
	#include <stdio.h>
	#include <string.h>
	#include "messages.h"
	#include "tw_teach_pendant.h"
	#include "c-motion.h"
	#include "PMDsetup.h"
	#include "PMDtrace.h"
  //#define	  DEBUGUSBLevel0
  //#define	  DEBUGUSBLevel1
  //#define	  DEBUGUSBLevel2
	
	
#endif
#include "mem.h"
#include "packetIO.h"

/*Variables globales de recepcion*/
tPacketHeader rxHeader;            
tCommand      rxCommand;  	
tTrajectory   rxTrajectory;       
tString       rxString;           //Es una cadena de 255 posiciones que ya esta definida                                            
tAxisLimits   rxAxisLimits;
tPacket       rxPacket;                      
uint8 		  *RXBUFFER;           //varible que recibe los datos del USB 
int16         *rxvector;  
tFecha        rxFecha;
tAlarm		  rxAlarma;



int16 FlagTrayectoryok=0;
int16 FlagrxVectorok=0;
int16 FlagTraceok=0;



/*Variables globales de Transmicion*/
// Solo debo de utilizar TXBUFFER,  ya que los otros bufferes son incesarios por que 
// corresponden a las mismas variables que ya esta correctamente declarados 
// en la main o donde sea que se quiera mandar la informacion 
// la informacion 
uint8 		  *TXBUFFER;		
tPacket       txPacket;// es importante que el main sapa que paquete recibi
  	


// used to check validity of packet header content (not really necessary, but done in 
// fillPacketHeader and readPacketHeader)
// Attention: the UC compiler can't initialize this (even as const), as it seems. Thus we fill it in initPacketIO()
static int16 minPacketData[19]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
/* = {
  0,
  0,
  sizeof(tCommand),
  sizeof(tError),
  sizeof(uint8),
  sizeof(int16),
  sizeof(int32),
  0,
  sizeof(tTrajectory),
  1	              // minimum string must have a '\0' character
  sizeof(PacketAxisSettings),
  sizeof(PacketAxisLimits)
  sizeof(PacketAny)
  
}; // minPacketData */

/*****************************************************************************************/
void initPacketIO(void) 
{
// initialize min and max fields for data tests (due to compiler problem with initialized structures)
	minPacketData[PacketNOP] = 0;
	minPacketData[PacketCommand] = sizeof(tCommand);
	minPacketData[PacketVector] = 0;
	minPacketData[PacketFecha] = 0;
	minPacketData[PacketTrajectory] = sizeof(tTrajectory);
	minPacketData[PacketString] = 1;
	minPacketData[PacketAxisSettings] = sizeof(PacketAxisSettings);
	minPacketData[PacketAxisLimits] =sizeof(PacketAxisLimits);
	minPacketData[PacketHeader] =sizeof(tPacketHeader);
	minPacketData[Packetuint16] =sizeof(uint16);
} // initPacketIO

/*****************************************************************************************/
tDataChecksum calcChecksum(tDataLength length, uint8* pdata,tDataType dataType) 
{
   tDataLength i;
   tDataChecksum sum = 0;
	tTrajectory *pTrajectory;
	int16 fin;

   if(dataType==PacketTrajectory || dataType==PacketTeachPendant)   
   {
        pTrajectory=(tTrajectory *)pdata;        
		//sumo los 4 primeros bytes osea los dos campos int16 (campos size e invalidMask).
		for(i=0; i<4; i++) 
			sum= sum+(tDataChecksum)pdata[i];      
		//apunto al lo que apunta el apuntador point  
		pdata = (uint8*)pTrajectory->point;       
		//sumo todos los bytes pero a la longitud length le resto 
		//los dos campos int16 osea cuatro bytes que ya sume
		for(i=0; i<(length-4); i++) 
			sum= sum+(tDataChecksum)pdata[i];     
		return sum;
	} 
	if(dataType==PacketHeader)
	{		
		/*ojo para calcular el chksum DEL HEADER no se puede sumar el campo cksum del mismo
		ya que la primera vez que se calcula éste es cero para la trama que se va enviar.
		Luego cuando los datos son recibidos el checksum no es 
		cero. Por lo tanto cuando se recalcula este para la trama recibida 
		da como resultado un numero mayor y DIFERENTE!
		sizeof(tHeaderChecksum) = 8 byte		
		por lo tanto se suma todo menos un byte (que es el que corresponde con el campo Checksum)*/
		fin=sizeof(tPacketHeader)-sizeof(tHeaderChecksum);
		for(i=0; i<fin; i++)   
			sum += (tHeaderChecksum)pdata[i];
		return sum; 
    }	
	//calquier otro paquete de datos	
      for(i=0; i<length; i++) 
          sum = sum + (tDataChecksum)pdata[i];  //calquier otro paquete de datos	
	#ifdef DEBUGUSBLevel2
	for(i=0; i<length; i++) 
          printf("calcChecksum => (tDataChecksum)pdata[%d]=%X \n",i,(tDataChecksum)pdata[i]);
	#endif
	#ifdef DEBUGUSBLevel1
	printf("calcChecksum => cksum %X del paquete %s \n",sum,PacketMsg(dataType));							    
	#endif
	if(dataType==Packetuint16) //Este paquete puede contener como dato un cero, el significa pose no valida.
	{		
		if(sum==0)
			sum=1;
    }

	return sum;
} // calcChecksum

/*****************************************************************************************/
int16 checksumOk (tDataLength length, uint8* data, tDataChecksum checksum,tDataType dataType) 
{
	tHeaderChecksum testh;
  
	if(dataType==PacketHeader)
	{
		testh=(tHeaderChecksum)calcChecksum(length,data,dataType);
		if(testh==checksum)
		{
			#ifdef DEBUGUSBLevel1
			printf("checksumOk=> %x OK  \n",checksum);							    
			#endif  
			return TRUE;
		}
	}
	else
    if(calcChecksum(length,data,dataType) == checksum)
	{
		#ifdef DEBUGUSBLevel1
		printf("checksumOk=> %x OK  \n",checksum);							    
		#endif  
        return TRUE;
	}
	#ifdef DEBUGUSBLevel1
	printf("checksumOk=> Error CheckSum en PacketHeader = %X  \n",checksum);							    
	#endif  
	return FALSE;
} // checksumOk

/*****************************************************************************************/
// Nota: Se llama initPacketIO() antes de usar readPacketHeaderRX para inicializar 
// la longitud de minima para cada tipo de paquete
int16 readPacketHeaderRX(tPacketHeader *ppacketHeader) 
{
	int16 i;
	uint8 *pdata;   
	
	pdata = (uint8*)ppacketHeader;					 
	//se lee 8 bytes del buffer de datos para obtener el header
	for(i=0; i<sizeof(tPacketHeader); i++) 	
		pdata[i]=RXBUFFER[i];						 
	//deteccion de errores
	// A el tipo de paquete le corresponde un numero  
	// este tiene que estar dentro el rango PacketNOP = 0 y PacketHeader = 13
	if( ( (*ppacketHeader).dataType < 0 || (*ppacketHeader).dataType > lastPacket ) ) 	
		return rcInvalidPacket;
	if((*ppacketHeader).dataLength < minPacketData[(*ppacketHeader).dataType]) 
		return rcTooFewData;
	//Compara el valor de  (*ppacketHeader).headerChecksum
	//con el calculado con el header recivido por pdata que proviene RXBUFFER
	if (!checksumOk(sizeof(tPacketHeader),pdata,(*ppacketHeader).headerChecksum,PacketHeader))
		return rcInvalidHeaderChecksum;
	//compruevo posible inconsistencia de paquete sin datos.
	/*Si el paquete es PacketTrace el *ppacketHeader).dataChecksum debe ser igual a 1 */
	if ((*ppacketHeader).dataChecksum==0)      //compruebo posible inconsistencia   
			return rcInvalidChecksum;    

	return rcOK; 
} // readPacketHeader

/*****************************************************************************************/
//pdata es un apuntador a RXbuffer donde pdata[0] coresponde a el primer
//byte de la variable que quiero leer sin el header
//Es utilizada por todos los tipos de paquetes menos trajectory o PacketTeachPendant
int16 readPacketDataRX(tPacketHeader *ppacketHeader, uint8 *pdata) 
{

	int16 i;
	
	tDataLength longitud;
	
	if((*ppacketHeader).dataLength != 0)
	{ 
		/*Si el paquete es PacketTrace su dataChecksum es igual a 1*/
		if((*ppacketHeader).dataChecksum != 0)
		{
			longitud=(*ppacketHeader).dataLength;
			#ifdef DEBUGUSBLevel1
			printf("readPacketDataRX=> direccion RXBUFFER en readPacketDataRX=>  %X\n",(int16)RXBUFFER);
			printf("readPacketDataRX=> direccion pdata en readPacketDataRX=>  %X\n",(int16)pdata);							       							       
			#endif
			#ifdef DEBUGUSBLevel2
			for(i=0; i<longitud; i++) 
				printf("readPacketDataRX=> RXBUFFER[%d]=%X\n",i,RXBUFFER[i]);							       
			#endif
			
			for(i=0; i<longitud; i++) 
				pdata[i]=RXBUFFER[i];   //pdata[0] tiene el primer dato
			#ifdef DEBUGUSBLevel2
			for(i=0; i<longitud; i++) 
				printf("readPacketDataRX=> Actualizado en readPacketDataRX=> pdata[%d] =%X\n",i,pdata[i]);							       
			for(i=0; i<longitud; i++) 
				printf("readPacketDataRX=> Comprobado en readPacketDataRX=> RXBUFFER[%d]=%X\n",i,RXBUFFER[i]);							       		   
			#endif
			if((*ppacketHeader).dataType!=PacketTrace)
			{
				if( checksumOk(longitud,pdata,(*ppacketHeader).dataChecksum,(*ppacketHeader).dataType)== TRUE ) 
				return rcOK;	
			}
			else
				/*No de puede calcular el dataChecksum, por la forma  con que uC manda los datos*/
			   return rcOK; 

		}
		return  rcInvalidChecksum;
	}   	
	return rcInvalidData;
} // readPacketData


/*****************************************************************************************/
//pTrajectory es la trayectoria destino
//RXBUFFER[0] tiene la estructura trayectoria no el header	
int16 readTrajectoryRX(tTrajectory *pTrajectory,tPacketHeader packetHeader) 
{
	#ifdef DEBUGUSBLevel2
	int16 k;
	#endif
	int16 rc = rcOK;
	int16 i,j;
    uint8* pdata=NULL;
  
	
	pdata = (uint8*)pTrajectory;				
	//Leemos  4 bytes  del RXBUFFER los 3 campos: puntos,dummy,*tpont 
	for(i=0; i<4; i++) 
		pdata[i]=RXBUFFER[i];
	#ifdef DEBUGUSBLevel1    
	printf("readTrajectoryRX=> pTrajectory->size =%d  \n",pTrajectory->size);							      
	#endif
	#ifdef DEBUGUSBLevel2
	printf("readTrajectoryRX => Addr de pTrajectory = %X\n",(int16)pTrajectory);							    
	for(k=0; k<4; k++) 
	    printf("readTrajectoryRX => RXBUFFER[%d]=%X pdata[%d]=%X\n",k,RXBUFFER[k],k,pdata[k]);							       
	#endif
	//pTrajectory->size es la cantidad de puntos de la trayectoria, 
	//cada punto tiene un tamaño tpoint 
    j=(pTrajectory->size)*sizeof(tpoint);       
	//Reservo espacio para guardar toda esa informacion que viene desde el USB
	pTrajectory->point=(tpoint *)declarar(j); 
	pdata = (uint8*)pTrajectory->point;        
	//En el siguiente  for pdata[0] corresponde a los datos a los que apunta pTrajectory->point
	//RXBUFFER[j] = tiene la informacion de los puntos que llegaron desde USB
	//i = 3, 3 campos puntos,dummy,y apuntador a tpont que ya incluido
	//j tiene que llegar hasta el final de los datos
	for(j=i; j<packetHeader.dataLength;j++) 
		pdata[j-i]=RXBUFFER[j]; 
	 
	#ifdef DEBUGUSBLevel2
		//K tiene que llegar hasta 120
    for(k=0; k<packetHeader.dataLength-i; k++) 
	{
	    printf("readTrajectoryRX => RXBUFFER[%d]=%X pdata[%d]=%X \n",k+i,RXBUFFER[k+i],k,pdata[k]);							       
	}
	printf("readTrajectoryRX => Addr pTrajectory->point = %X \n",(int16)pTrajectory->point);							    
    #endif
	//checksumOk calcula el cheksum con los datos recibidos
	if(!checksumOk (packetHeader.dataLength,(uint8*)pTrajectory,packetHeader.dataChecksum,PacketTrajectory))
		return rcInvalidChecksum;
return rc;
} 

/*****************************************************************************************/
//**************************************
//functions to make communication easier
//**************************************

// receiptPacket and dispatchPacket should make the code more readable.
// main receive function with error handling
// get packet type and data and store it in the global rxXXX buffers.
// check return code in case of error

// Comentarios nuevos 
// Como  es el uC el unico que conoce la cantidad de los datos 
// que esta enviando hacia el computador 
// 1. Ver si el Gpif esta ocupado con alguna transaccion anterior
// 2. Se limpian los Bufferes 
// 3. Se inicia el proceso para poder leer el header
//		- Se ejecuta un request con 0xC8 para poder modicar el TC a 4bytes 
//        (como se utiliza 16 bis  tiene que tener la mitad de los bytes 8/2) 
//      - Actualizo el TC a 4
//		- Se ejecuta un request con 0xC0 para interumpir el uC y pedirle lo datos.
//		- Lleer el header con la funcion readPacketHeaderRX(&rxHeader)
// 3. Mediante el header, detectar la cantidad de datos  que necesitan ser recividos por el PC
// 4. Si es impartar convertirlo en par esto es  por que USB esta configurado a 16BITS 
// 4. Asignar memoria a RxBuffer con la longitud de los datos 
// 7. de Rx buffer se lee toda la informacion 

#ifdef _WIN32
int16 receiptPacket(char Devicename[],HANDLE hDlg)  
{
	int16 rc=0,i=0,delay=6500;
	char buffertemp=00;  
	uint8 * TC;
	int request;

	//Esta ocupado debido a una tranferencia de lectura o escritura, uC fallo...
	if (UsbGpifIDEL(hDlg,Devicename))                              
	{
		//Activa INT_FLAGin y low in_toquen_event
		Abortar(hDlg,1,Devicename);
		//Esta variable guardara la cantidad de byte el uC envia
		TC=(uint8*)declarar(3);             
		//habilito modificacion del TC
		request=0xC8;
		Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename); 
		//Siempre tengo que pedir la mitad de los bytes que necesito (16bits)	                                                       
		//El header tiene 8 bytes por lo tanto TC=4	
		i=4;                                                          
		TC = (uint8*)&i;											   
		//Actulizo TC dentro del uC 8051 para leer la cabecera
		rc=Mandar_datos_Por_USB(hDlg,0,TC,2,Devicename);		   
		//HAbilito las transferencias IN
		request=0xC0;                                              
		Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename);
		//Creo un Buffer para almacenar el header
		RXBUFFER=(uint8 *)declarar(8);                              
		rc = Leer_datos_Por_USB(hDlg,1,RXBUFFER,8,Devicename);	   
		if(rc != rcOK) 
		{
			free(RXBUFFER);
			return rc;                                  
		}
		rc = readPacketHeaderRX(&rxHeader);				           
		free(RXBUFFER);
		if(rc != rcOK) return rc;                                  
		//Miro la cantidad de datos enviados despues de la cabecera    
		i=rxHeader.dataLength;													
		/*rxHeader.dataLength puede ser impar o par ya que lo que solo  es es TXBUFFER, 
		mas no los datos que contiene, ahora bien i equivale a la cantidad de bytes que 
		leo esta tiene que ser igual a TXBUFFER, rxHeader.dataLength no poser cambiado*/

		if(!Esimpar(i))
			i= i/2;
		else 
		{
			i++;                                                 
			i=i/2;                                                
		}
		TC = (uint8*)&i;                         
		//habilito modificacion del TC
		request=0xC8;
		Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename); 
		rc=Mandar_datos_Por_USB(hDlg,0,TC,2,Devicename);            
		i=i*2;
		RXBUFFER=(uint8 *)declarar(i); 
		while(!UsbGpifIDEL(hDlg,Devicename))
		{
			request=0xC9; // Cancela el gpif
			Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename);
		}
		//Abilito transferencias IN
		request=0xC0;
		Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename);
		/*Siempre Leo una cantidad par de datos*/
		rc=Leer_datos_Por_USB(hDlg,1,RXBUFFER,i,Devicename);
		if(rc != rcOK) return rc; 
		rxPacket = rxHeader.dataType;					
		switch(rxPacket)
		{
			case PacketTeachPendant:
			case PacketTrajectory:	
				if(FlagTrayectoryok)				
					/*Libero la rxtrayectory anterior*/
					free(rxTrajectory.point); 
				/*Creo una nueva*/
				rc=readTrajectoryRX(&rxTrajectory,rxHeader);
				FlagTrayectoryok=1;
			break;
			case PacketCommand:
				rc = readPacketDataRX(&rxHeader, (uint8*)&rxCommand);
				if(rxCommand.CommandOP > lastCommand) 
				{
					rxCommand.CommandOP = CommandNOP;     // make access to CommandMsg[rxCommand] safe
					return rcInvalidCommand;
				}
            break;    
			case PacketString:			 
				rc = readPacketDataRX(&rxHeader,(uint8*)rxString); //Max 255 char
			break;
			case PacketAxisSettings:
				rc = readPacketDataRX(&rxHeader, (uint8*)&rxAxisSettings); 
			break;
			case PacketAxisLimits:
				rc = readPacketDataRX(&rxHeader, (uint8*)&rxAxisLimits); 
			break;
			case PacketPosesvector:
				if(FlagrxVectorok)					
					free(rxvector); /*Liberamos a rxVector*/
				/*Creamos uno nuevo*/
				rxvector=(int16*)declarar(rxHeader.dataLength);
				rc = readPacketDataRX(&rxHeader, (uint8*)rxvector); 
				FlagrxVectorok=1;
			break;
			case PacketFecha:
				rc = readPacketDataRX(&rxHeader, (uint8*)&rxFecha); 
			break;
			case PacketAlarma:
				rc = readPacketDataRX(&rxHeader, (uint8*)&rxAlarma); 
			break;
			case Packetuint16:
				rc = readPacketDataRX(&rxHeader, (uint8*)&rxPosesActual); 
			break;
			case PacketTrace:
				if(FlagTraceok)					
					free(rxTraceBuffer); /*Liberamos a rxTraceBuffer*/
				rxTraceBuffer=(int32*)declarar(rxHeader.dataLength);
				rc = readPacketDataRX(&rxHeader, (uint8*)rxTraceBuffer); 
				FlagTraceok=1;
			break;
		}//end case
		if(rc==rcOK)
		{
			while(!UsbGpifIDEL(hDlg,Devicename))
			{
				request=0xC9; // Cancela el gpif
				Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename);
			}
		}
		free(RXBUFFER);//libero el RXbuffer para poder recibir la siguiente	
		return rc;
	}//end if busy
	
	return rcUnexpectedState;
} //  end receiptPacket
#endif

/*****************************************************************************************/
// Comentarios nuevos 
// Proceso de recepcion de datos
// 1. Se limpian los Bufferes
// 2. Se lee header para poder saber las caracteristicas del paquete:
//    Esto implica declarar un buffer  8 byte que es la longitud del header y  luego 
//    llamar a  la funcion Leo_USB(RXBUFFER,8) para colocar 8 bytes en èl. Finalmente 
//    mediante la funcion readPacketHeaderRX se copia la informacion de RXBUFFER 
//    a la varible rxHeader, si hay error se sale de la funcion.
// 3. Se lee el campo rxHeader.dataLength se declara a RXBUFFER con esa longitud
//    y luego se utiliza de nuevo la funcion Leo_USB(RXBUFFER,8)
// 4. Se lee el campo rxHeader.dataType del header recibido y se copia a rxPacket
// 5. Dependiendo del tipo de paquete se guarda en su respectiva variable

#ifdef _AVR_IO_H_
int16 receiptPacket(void)  
{
	
	#ifdef DEBUGUSBLevel2
	int16 i;
	#endif
   	#ifdef DEBUGUSBLevel1
	int16 i1;
	uint16 spacefree;
	#endif
   	int16 rc;
	tDataLength longitud;    
   //Creo un Buffer para almacenar el header
	RXBUFFER=(uint8 *)declarar(8);                    
	
	#ifdef DEBUGUSBLevel1
	printf("receiptPacket=> Addr RXBUFFER para almacenar los datos Header = %X \n",(int16)RXBUFFER);							    
	#endif
	
	//Siempre se manda primero el header y luego los dato
	//En esta función el uC recibe los datos desde la fifo, la cual es controlada por el GPIF
	rc = Leo_USB(RXBUFFER,8);
	if(rc!=rcOK)
		return rc;				
	//Se lee el Header de RXBUFFER
	rc = readPacketHeaderRX(&rxHeader);				    
	free(RXBUFFER); 
	if(rc != rcOK) 
	{
		#ifdef DEBUGUSBLevel0
		printf("receipPacket=> Error al leer el Header\n");							    
		#endif
		return rc;                       
	}
	// Reservo memoria a los datos
	longitud=rxHeader.dataLength;		
	/*tengo que crear un buffer siempre de datos pares pues USB siempre manda par*/
	if(Esimpar(longitud))
		longitud++;
	#ifdef DEBUGUSBLevel1
	spacefree=maxavail();
	printf("receiptPacket=> La cantidad de memoria ram disponible SIN rxbuffer declarado %d \n",spacefree);
	#endif	
		
	RXBUFFER=(uint8 *)declarar(longitud);    
	
	#ifdef DEBUGUSBLevel1
	spacefree=maxavail();
	printf("receiptPacket=> La cantidad de memoria ram disponible CON rxbuffer declarado %d , con una log de %d\n",spacefree,longitud);
	#endif
	//En esta función el uC recibe los datos desde la fifo, la cual es controlada por el GPIF
	rc=Leo_USB(RXBUFFER,longitud); 
	if(rc!=rcOK)
	{
		#ifdef DEBUGUSBLevel0
		printf("receipPacket=> Error al leer los datos\n");							    
		#endif
		return rc;				
	}
	
	rxPacket = rxHeader.dataType;	
	
	#ifdef DEBUGUSBLevel0
	printf("receiptPacket=> Addr RXBUFFER de los datos = %X \n",(int16)RXBUFFER);							    
	printf("receiptPacket=> Actualizado rxHeader.dataType  = %X \n",rxHeader.dataType);							    
	printf("receiptPacket=> Actualizado rxHeader.dataLength  = %X \n",rxHeader.dataLength);							    
	printf("receiptPacket=> Actualizado rxHeader.dataChecksum  = %X \n",rxHeader.dataChecksum);	
	printf("receiptPacket=> Actualizado rxHeader.headerChecksum  = %X \n",rxHeader.headerChecksum);							    						    
	#endif
	#ifdef DEBUGUSBLevel2
	if(rxPacket!=PacketTrajectory ||rxPacket!=PacketTeachPendant)   
		for(i=0; i<longitud; i++) 
		{
			printf("receipPacket=> RXBUFFER[%d] = %X \n",i,RXBUFFER[i]);							    
		}
	#endif	
	
	switch(rxPacket)
	{  	
		case PacketTeachPendant:
		case PacketTrajectory:	
			#ifdef DEBUGUSBLevel0
			printf("receipPacket=> FlagTrayectoryok = %d\n",FlagTrayectoryok);							    
			#endif
			if(FlagTrayectoryok)				
			{
				/*Libero la rxtrayectory anterior*/
				#ifdef DEBUGUSBLevel0
				spacefree=maxavail();
				printf("receipPacket=> La cantidad de memoria ram disponible %d \n",spacefree);
				printf("receipPacket=> rxTrajectory.point = %X\n",(int16)rxTrajectory.point);
				#endif
				free(rxTrajectory.point); 
				#ifdef DEBUGUSBLevel0
				spacefree=maxavail();
				printf("receipPacket=> rxTrajectory.point  free...\n");	
				printf("receipPacket=> La cantidad de memoria ram disponible %d \n",spacefree);						    
				#endif		
			}
			rc=readTrajectoryRX(&rxTrajectory,rxHeader); 
			#ifdef DEBUGUSBLevel0
			printf("receipPacket=> He ejecutado readTrajectoryRX ...\n");							    
			#endif 
			FlagTrayectoryok=1;
		break;		
		case PacketCommand:
			rc = readPacketDataRX(&rxHeader, (uint8*)&rxCommand);
			if(rxCommand.CommandOP > lastCommand) 
			{
				rxCommand.CommandOP = CommandNOP;     // make access to CommandMsg[rxCommand] safe
				return rcInvalidCommand;
			}
        break;   
		case PacketString:			 
			rc = readPacketDataRX(&rxHeader,(uint8*)rxString); //Max 255 char
		break;	 
		case PacketAxisSettings:
			#ifdef DEBUGUSBLevel1
			for(i1=0; i1<DOF; i1++) 
			{
				
				printf("receipPacket=>  direccion de &(rxAxisSettings[%d]) %x\n",i1,(int16)&(rxAxisSettings[i1]));
				printf("receipPacket=>  direccion rxAxisSettings %x\n",(int16)rxAxisSettings);
				
			}
			printf("receipPacket=>  rxHeader.dataLength = %d\n",rxHeader.dataLength);
			printf("receipPacket=>  sizeof(tAxisSettings) = %d\n",sizeof(tAxisSettings));
			#endif
			readPacketDataRX(&rxHeader, (uint8*)&(rxAxisSettings[0]) ); 
			
			#ifdef DEBUGUSBLevel2
			for(i=0; i<DOF; i++) 
			printf("receiptPacket => rxAxisSettings[%d].ustepsPerStep= %d \n",i,rxAxisSettings[i].ustepsPerStep);
			#endif
			
		break;	 
		case PacketAxisLimits:
			rc = readPacketDataRX(&rxHeader, (uint8*)&rxAxisLimits); 
		break;	 
		case PacketPosesvector:
			#ifdef DEBUGUSBLevel0
			printf("receipPacket=>  FlagrxVectorok = %d\n",FlagrxVectorok);							    
			#endif
			if(FlagrxVectorok)					
				free(rxvector); /*Liberamos a rxVector*/
			/*Creamos uno nuevo*/
		    rxvector=(int16*)declarar(rxHeader.dataLength);
		    rc = readPacketDataRX(&rxHeader, (uint8*)rxvector); 
			FlagrxVectorok=1;
		break;
		case PacketFecha:
			rc = readPacketDataRX(&rxHeader, (uint8*)&rxFecha); 
		break;
		case PacketAlarma:
			rc = readPacketDataRX(&rxHeader, (uint8*)&rxAlarma); 
		break;
		case Packetuint16:
			rc = readPacketDataRX(&rxHeader, (uint8*)&rxPosesActual); 
		break;
		case PacketTrace:
			#ifdef DEBUGUSBLevel0
			printf("receipPacket=> FlagTraceOK = %d\n",FlagTraceok);							    
			#endif
			if(FlagTraceok)					
				free(rxTraceBuffer); 
			/*Creamos un buffer dinamico de la cantidad de datos de 8bits 
			presentes en la cantidad de datos de 32 bits enviados*/
			rxTraceBuffer=(int32*)declarar(rxHeader.dataLength);
			rc = readPacketDataRX(&rxHeader, (uint8*)rxTraceBuffer); 
			FlagTraceok=1;
		break;
	}//end case
	free(RXBUFFER);//libero el RXbuffer para poder recibir la siguiente	
	#ifdef DEBUGUSBLevel1
	spacefree=maxavail();
	printf("receipPacket=> La cantidad de memoria ram disponible CON rxbuffer LIBERADO.. %d \n",spacefree);
	#endif
	return rc;
} // receiptPacket
#endif

/*****************************************************************************************/
/*-------------------------------------WRITE---------------------------------------------*/
// fill the header structure and calculate the data and header checksum. Return rc

int16 fillPacketHeader(tPacketHeader *ppacketHeader, tDataType dataType, tDataLength dataLength, 
					 uint8 *pdata) 
{
	(*ppacketHeader).dataChecksum = 0;    // will stay so if the packet has no data attached
	(*ppacketHeader).headerChecksum = 0;  // necessary initializazion for the following calculation!
	(*ppacketHeader).dataType = dataType;
	(*ppacketHeader).dataLength = dataLength;
	if(dataType > lastPacket) 
		return rcInvalidPacket;  // ensure that we don't try to send invalid data:  
	if(dataLength > 0)                                 // calculate data checksum, if data is present
	{
	    if((*ppacketHeader).dataType==PacketTrace)
			/*No es posible calcular el dataChecksum debido a la forma de como de manda este tipo
			de dato, se coloca a 1 para no tener problemas a la hora de leer*/
			(*ppacketHeader).dataChecksum = 1;
		else
			(*ppacketHeader).dataChecksum = calcChecksum(dataLength,pdata,dataType); 
	}
	else
		return rcTooFewData;       
	(*ppacketHeader).headerChecksum=(tHeaderChecksum)calcChecksum(sizeof(tPacketHeader),(uint8*)ppacketHeader,PacketHeader);
    return rcOK;
}

/*****************************************************************************************/
// if data is ready, Se prepara el buffer para ser mandado almacenando la informacion necesaria en este
void BUFFERPacketDataTX(tPacketHeader *ppacketHeader, uint8 *pdata, tDataType tipo) 
{ 
  
	int16 i,j;
	tTrajectory *temp;
  
	switch (tipo)
	{
		case PacketHeader:  
			// Se guarda el Header al principio de TXBUFFER
			for(i=0; i<sizeof(tPacketHeader); i++)   
				TXBUFFER[i]=pdata[i]; 
		break;
		case PacketTrajectory:	
		case PacketTeachPendant:
			//me desplazo 8 bytes en TXBUFFER porque deben de contener le header																
			//copio los 4 bytes de la trayectoria, 2 campos  int16 (dummy y size)
			for(i=sizeof(tPacketHeader); i<4+sizeof(tPacketHeader); i++)   
				TXBUFFER[i]=pdata[i-sizeof(tPacketHeader)]; 
			//copio los datos en una variable temporal  de tipo tTrajectory para poder 	
			//acceder a el campo point y asi extraer el apuntador a los  puntos
			temp=(tTrajectory *)pdata;		
			pdata = (uint8*)temp->point;		
			for(j=i; j<ppacketHeader->dataLength+i; j++) 
				TXBUFFER[j]=pdata[j-i]; 	        
		break;
		default:								//para todos los demas paquetes
			j=(int16)ppacketHeader->dataLength;
			j=j+sizeof(tPacketHeader);
			for(i=sizeof(tPacketHeader);i<j;i++) 
			{
				TXBUFFER[i]=pdata[i-sizeof(tPacketHeader)]; //Coloca toda la informacion a enviar  
				if(tipo==Packetuint16)
				{
					printf("BUFFERPacketDataTX=> pdata[%d-sizeof(tPacketHeader)] = %d \n",i,pdata[i-sizeof(tPacketHeader)]);
				}
			}
	}  
} // sendPacketData

/*****************************************************************************************/
// a short way to send a command byte
int BUFFERCommandTX(tCommand* command) 
{
	tPacketHeader packetHeader;
	int16 buffersize;	
	
	buffersize=sizeof(tCommand)+sizeof(tPacketHeader);
	if(Esimpar(buffersize))
		buffersize++;		
	TXBUFFER=(uint8*)declarar(buffersize);
	//sizeof(tCommand)
	fillPacketHeader(&packetHeader,PacketCommand,sizeof(tCommand),(uint8*)command);
	// meto el header en TXBUFFER	
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader); 
	#ifdef DEBUGUSBLevel1
	printf("BUFFERCommandTX=> ComandoOP en BUFFERCommandTX=> %d  \n",command->CommandOP);
	printf("BUFFERCommandTX=> Axis en BUFFERCommandTX=> %d\n",command->Axis);
	printf("BUFFERCommandTX=> Dato en BUFFERCommandTX=> %ld\n",command->Dato);	    
    printf("BUFFERCommandTX=> packetHeader.dataType en BUFFERCommandTX=> %s\n",PacketMsg(packetHeader.dataType));
	printf("BUFFERCommandTX=> header.datalength en BUFFERCommandTX=> %x\n", packetHeader.dataLength);
	printf("BUFFERCommandTX=> packetHeader.dataChecksum en BUFFERCommandTX=> %x\n",packetHeader.dataChecksum);
	printf("BUFFERCommandTX=> packetHeader.headerChecksum en BUFFERCommandTX=> %d\n",packetHeader.headerChecksum);	
	#endif
	//meto el command en TXBUFFER despues del header 
	BUFFERPacketDataTX(&packetHeader,(uint8*)command,PacketCommand);     
	return buffersize;
} // sendCommand

/*****************************************************************************************/
int BUFFERStringTX(uint8 *string) 
{  
	uint8 *info;
	int16 buffersize= 0;
	int16 Datasize= 0;
	tPacketHeader packetHeader;
    
	info=(uint8*)string;
	buffersize=strlen((char*)info)+sizeof(tPacketHeader);
	if(Esimpar(buffersize))
		buffersize++;		
	TXBUFFER=(uint8*)declarar(buffersize);
	//Datasize tiene la logitud de la informacion en bytes
	Datasize=strlen((char*)info);
	fillPacketHeader(&packetHeader,PacketString,(tDataLength)Datasize,string);  
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);     
	// meto el string en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,string,PacketString);                   
	return buffersize;
} // sendString
	
/*****************************************************************************************/
//Coloca la trayectoria + el header en TXBUFFER para luego ser enviada
//tipo puede ser PacketTeachPendant o PacketTrajectory 
int BUFFERTrajectoryTX(tTrajectory *pTrajectory,int16 puntos,tPacket tipo) 
{
	int16 buffersize,Datasize;
	tPacketHeader packetHeader;
	//asigno memoria para guardar la trayectoria, 
	//3 de  campos: una estructura tpoint * numero de puntos 
	//+ 2 campos  int16 (dummy y size) 
	//mas el tamaño del header
	buffersize=sizeof(tpoint)*puntos+2*sizeof(int16)+sizeof(tPacketHeader); 
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-9;						  
	}
	else
	{
		Datasize=buffersize-8;						  	
	}
	
	TXBUFFER=(uint8*)declarar(buffersize);    
	//Resto el numero de bytes del header pues este es la cantidad de datos 
	//que se quieren enviar	   	    
	//
	
	fillPacketHeader(&packetHeader,tipo,(tDataLength)Datasize,(uint8*)pTrajectory);
	#ifdef DEBUGUSBLevel1
	printf("BUFFERTrajectoryTX=> Addr de pTrajectory en BUFFERTrajectoryTX=> %X\n",(int16)pTrajectory);							    
	printf("BUFFERTrajectoryTX=> Addr de pTrajectory->point en BUFFERTrajectoryTX=> %X \n",(int16)pTrajectory->point);							    
    printf("BUFFERTrajectoryTX=> packetHeader.dataType en BUFFERCommandTX=> %s\n",PacketMsg(packetHeader.dataType));
	printf("BUFFERTrajectoryTX=> header.datalength en BUFFERCommandTX=> %x\n", packetHeader.dataLength);
	printf("BUFFERTrajectoryTX=> packetHeader.dataChecksum en BUFFERCommandTX=> %x\n",packetHeader.dataChecksum);
	printf("BUFFERTrajectoryTX=> packetHeader.headerChecksum en BUFFERCommandTX=> %d\n",packetHeader.headerChecksum);	
	#endif
	//coloco el packet Header en el TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader); 
	 //coloco los datos en el TXBUFFER independiente si es de tipo PacketTeachPendant 
	//o PacketTrajectory uso PacketTrajectory  
	BUFFERPacketDataTX(&packetHeader,(uint8*)pTrajectory,PacketTrajectory); 
	return buffersize;
} // sendTrajectory

/*****************************************************************************************/
// send axis limits information PC-uC uC-Pc
int BUFFERAxisSettingsTX(tAxisSettings *paxisSettings) 
{
	tPacketHeader packetHeader;
	int16 buffersize,Datasize;
	
	buffersize=sizeof(tAxisSettings)*DOF+sizeof(tPacketHeader);		   
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-9;						  
	}
	else
	{
		Datasize=buffersize-8;						  	
	}		
	TXBUFFER=(uint8*)declarar(buffersize);
	//sizeof(tAxisSettings)
	fillPacketHeader(&packetHeader,PacketAxisSettings,Datasize,(uint8*)paxisSettings);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);        
	// meto el tAxisSttings en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)paxisSettings,PacketAxisSettings);  
	return buffersize;
} // sendAxisSettings

/*****************************************************************************************/
// send axis limits information
int BUFFERAxisLimitsTX(tAxisLimits *paxisLimits) 
{

	tPacketHeader packetHeader;
	int16 buffersize,Datasize;
	
	buffersize=sizeof(tAxisLimits)+sizeof(tPacketHeader);	
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-9;						  
	}
	else
	{
		Datasize=buffersize-8;						  	
	}		
	TXBUFFER=(uint8*)declarar(buffersize);
	//sizeof(tAxisLimits)
	fillPacketHeader(&packetHeader,PacketAxisLimits,Datasize,(uint8*)paxisLimits);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);      
	// meto el tAxisLimits en TXBUFFER  
	BUFFERPacketDataTX(&packetHeader,(uint8*)paxisLimits,PacketAxisLimits);    
	return buffersize;
} // sendAxisLimits

/*****************************************************************************************/
int BUFFERVectorPoseTX(uint8 * pVectorpose) 
{

	tPacketHeader packetHeader;
	int16 buffersize,Num_data,Datasize;
	int16* pvectortemp;

    
	pvectortemp=(int16*)pVectorpose;
    Num_data=pvectortemp[0];
	#ifdef DEBUGUSBLevel1
	printf("BUFFERVectorPoseTX => Numero de datos en pvectortemp[0] %X\n",Num_data);			
	#endif		
	buffersize=sizeof(int16)*Num_data+sizeof(tPacketHeader);		   
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-(sizeof(tPacketHeader)+1);
	}
	else
	{
		Datasize=buffersize-sizeof(tPacketHeader);
	}
	TXBUFFER=(uint8*)declarar(buffersize);
	#ifdef DEBUGUSBLevel1
	printf("BUFFERVectorPoseTX => cantidad a mandar al PC %X\n",buffersize);			
	printf("BUFFERVectorPoseTX => Datasize = %X\n",Datasize);			
	#endif		
	fillPacketHeader(&packetHeader,PacketPosesvector,Datasize,(uint8*)pVectorpose);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);      
	// meto el tAxisLimits en TXBUFFER  
	BUFFERPacketDataTX(&packetHeader,(uint8*)pVectorpose,PacketPosesvector);    
	return buffersize;
} 

/*****************************************************************************************/

int BUFFERFechaTX(uint8 * Fecha) 
{

	tPacketHeader packetHeader;
	int16 buffersize,Datasize;
	#ifdef DEBUGUSBLevel2
	int16 j;
	#endif		
    
	buffersize=sizeof(tFecha)+sizeof(tPacketHeader);		   
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-(sizeof(tPacketHeader)+1);	
	}
	else
	{
		Datasize=buffersize-sizeof(tPacketHeader);	
	}
	TXBUFFER=(uint8*)declarar(buffersize);
	
	#ifdef DEBUGUSBLevel2
	for(j=0;j<Datasize;j++) 
          printf("BUFFERFechaTX => Fecha[%d]=%X \n",j,Fecha[j]);
	#endif		
	#ifdef DEBUGUSBLevel1
	printf("BUFFERFechaTX => cantidad a mandar al PC %X\n",buffersize);			
	printf("BUFFERFechaTX => Datasize = %X\n",Datasize);			
	#endif		
	fillPacketHeader(&packetHeader,PacketFecha,Datasize,(uint8*)Fecha);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);      
	// meto a Fecha en TXBUFFER  
	BUFFERPacketDataTX(&packetHeader,(uint8*)Fecha,PacketFecha);    
	return buffersize;
} 

/*****************************************************************************************/
int BUFFERAlarmaTX(uint8 * AlarmaTX) 
{

	tPacketHeader packetHeader;
	int16 buffersize,Datasize;
	#ifdef DEBUGUSBLevel2
	int16 j;
	#endif		
    
	buffersize=sizeof(tAlarm)+sizeof(tPacketHeader);		   
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-(sizeof(tPacketHeader)+1);	
	}
	else
	{
		Datasize=buffersize-8;	
	}
		
	TXBUFFER=(uint8*)declarar(buffersize);
	#ifdef DEBUGUSBLevel2
	for(j=0;j<Datasize;j++) 
          printf("BUFFERFechaTX => AlarmaTX[%d]=%X \n",j,AlarmaTX[j]);
	#endif		
	#ifdef DEBUGUSBLevel1
	printf("BUFFERFechaTX => cantidad a mandar al PC %X\n",buffersize);			
	printf("BUFFERFechaTX => Datasize = %X\n",Datasize);			
	#endif		
	fillPacketHeader(&packetHeader,PacketAlarma,Datasize,(uint8*)AlarmaTX);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);      
	// meto a Fecha en TXBUFFER  
	BUFFERPacketDataTX(&packetHeader,(uint8*)AlarmaTX,PacketAlarma);    
	return buffersize;
} 

/*****************************************************************************************/
/*TraceTX la direccion del apuntador del primer dato*/
/*Esta funcion mete en TXBUFFER los datos del trazador*/
int BUFFERTraceTX(uint8 * TraceTX) 
{
	int32     *pTraceBuffer;
	tPacketHeader packetHeader;
	int16 buffersize,Datasize;
	/*Estructuro el buffer a 32 bits*/
    pTraceBuffer=(int32*)TraceTX;
	buffersize=(  pTraceBuffer[0]*sizeof(int32)) +sizeof(tPacketHeader)+(sizeof(int32)*3);		   
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-(sizeof(tPacketHeader)+1);	
	}
	else
	{
		Datasize=buffersize-8;	
	}
		
	TXBUFFER=(uint8*)declarar(buffersize);
	#ifdef DEBUGUSBLevel1
	printf("BUFFERTraceTX => cantidad a mandar al PC %X\n",buffersize);			
	printf("BUFFERTraceTX => Datasize de 8bit = %X\n",Datasize);			
	printf("BUFFERTraceTX => Cantidad de datos de 32bit = %lX\n",pTraceBuffer[0]);			
	#endif		
	fillPacketHeader(&packetHeader,PacketTrace,Datasize,(uint8*)TraceTX);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);      
	// meto a Fecha en TXBUFFER  
	BUFFERPacketDataTX(&packetHeader,TraceTX,PacketTrace);    
	return buffersize;
}



int BUFFERunit16TX(uint8 * unit16TX) 
{
	
	tPacketHeader packetHeader;
	int16 buffersize,Datasize;


	buffersize=sizeof(uint16) +sizeof(tPacketHeader);		   
	if(Esimpar(buffersize))
	{
		buffersize++;		
		Datasize=buffersize-(sizeof(tPacketHeader)+1);	
	}
	else
	{
		Datasize=buffersize-8;	
	}
		
	TXBUFFER=(uint8*)declarar(buffersize);
	#ifdef DEBUGUSBLevel1
	printf("BUFFERunit16TX => cantidad a mandar al PC %X\n",buffersize);			
	printf("BUFFERunit16TX => Datasize de 8bit = %X\n",Datasize);	
	printf("BUFFERunit16TX => *unit16TX => %d\n",*unit16TX);	
				
	#endif		
	fillPacketHeader(&packetHeader,Packetuint16,Datasize,(uint8*)unit16TX);
	// meto el header en TXBUFFER
	BUFFERPacketDataTX(&packetHeader,(uint8*)&packetHeader,PacketHeader);      
	// meto a Fecha en TXBUFFER  
	BUFFERPacketDataTX(&packetHeader,unit16TX,Packetuint16);    
	return buffersize;
}
/*****************************************************************************************/
// dispatch Packet 
// 1. mediante txPacket (variable de tipo global), tiene el tipo de dato a enviar 
// 2. Dependiendo de eso tratamos los datos 
// 3. Se le reserva espacio a TXBUFFER
// 3. llenamos de TXBUFFER con el header + datos
// 4. enviamos la informacion por el USB
#ifdef _WIN32
int16 dispatchPacket(tPacket tipodato,uint8* datos,char Devicename[],HANDLE hDlg) 
{
	int16 rc,puntos,buffersize;       //puntos:cantidad de puntos de la trayectoria que es esta enviado,
							//i: cantidad en bytes que se envia por el USB
	int request;
	char buffertemp=00;
	tTrajectory * Traytemp;//Trayectoria temporal para poder acceder a el campo size, que 
							//la variable datos esta en formato tipo byte.



	txPacket=tipodato;
	if(txPacket == PacketNOP || txPacket > lastPacket) 
		return rcInvalidPacket; 
																			   //we won't send 'nothing'

	if(Esta_Conectado_USB(hDlg,Devicename)) 
	{
		if (UsbGpifIDEL(hDlg,Devicename))                              //Esta ocupado debido a una tranferencia de lectura o escritura se cancela
		{
			switch(txPacket) 
			{   
				case PacketTeachPendant:
				case PacketTrajectory:
					//Como dispatchPacket es una funcion generica los datos vienen en bytes
					Traytemp=(tTrajectory *)datos;  
					//Con puntos puedo calcular el numero de bytes que necesito								  
					puntos=Traytemp->size;          
					buffersize=BUFFERTrajectoryTX(Traytemp,puntos,txPacket);
				break;
				case PacketCommand:
					buffersize=BUFFERCommandTX((tCommand*)datos);
				break;
				case PacketString:       // send \0-terminated string, such as copyright or debug info
					buffersize=BUFFERStringTX(datos);
				break;
				case PacketAxisSettings:
					buffersize=BUFFERAxisSettingsTX((tAxisSettings*)datos);		   
				break;
				case PacketAxisLimits:
					buffersize=BUFFERAxisLimitsTX((tAxisLimits*)datos);
				break;
				case PacketPosesvector:
					buffersize=BUFFERVectorPoseTX(datos); 
				break;
				case PacketFecha:   // 14
					buffersize=BUFFERFechaTX(datos); 
				break;
				case PacketAlarma:
					buffersize=BUFFERAlarmaTX(datos); 
				break;
				case PacketTrace:
					buffersize=BUFFERTraceTX(datos); 
				break;
				case Packetuint16:
					buffersize=BUFFERunit16TX(datos);
				break;
				default:
				return rcInvalidPacket;
			} // switch txPacket
			//Una vez acomodado el buffer mando la informacion
			//Mando un request para interupir a el uC
			//buffersize solo es par, es decir siempre el buffer es par. Aunque el dato a enviar 
			//tenga una longitud de bytes impar. Internamente tPacketHeader.dataLength siempre
			//tiene la cantidad de bytes correcta del dato.
			request=0xCA;
			Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,Devicename);  
			rc=Mandar_datos_Por_USB(hDlg,0,TXBUFFER,buffersize,Devicename);//Finalmente se manda el buffer por el USB
			free(TXBUFFER);
			return rc;
		}
		return rcUnexpectedState;
	}
	return rcCommunicationError;
} // dispatchPacket
#endif

/*****************************************************************************************/
#ifdef _AVR_IO_H_
int16 dispatchPacket(tPacket tipodato,uint8* datos)
{
	#ifdef DEBUGUSBLevel1						
	//Variables para string
	int16 j;               
	int8 * info;   
	#endif		          
	#ifdef DEBUGUSBLevel2						
	//Variables para command
	int16 k;
	#endif		   
    //variables para enviar la trajectoria
	tTrajectory * Traytemp;//Trayectoria temporal para poder acceder a el campo size,
    int16 puntos;
    //Variables Transmicion 
	int16 rc,buffersize=0;       

	txPacket=tipodato;
	if(txPacket == PacketNOP || txPacket > lastPacket) 
		return rcInvalidPacket;   

	switch(txPacket)  
	{  
		case PacketTeachPendant:
		case PacketTrajectory :
			// Como dispatchPacket es una funcion generica los datos vienen en bytes   
			Traytemp=(tTrajectory *)datos;  
			//Con puntos puedo calcular el numero de bytes que necesito								  
			puntos=Traytemp->size;          
			//4 bytes por los dos campos int16 de la estructura trayectoria			
			buffersize=BUFFERTrajectoryTX(Traytemp,puntos,txPacket); 
        break;
		case PacketCommand:
			buffersize=BUFFERCommandTX((tCommand*)datos);
			#ifdef DEBUGUSBLevel1						
			printf("dispatchPacket=> Addr de txCommand en dispatchPacket => %X\n",(int16)&datos);							    
			#endif		   
			#ifdef DEBUGUSBLevel2						
			for(k=0; k<buffersize;k++) 
            printf("dispatchPacket => TXBUFFER[%d]=%X\n",k,TXBUFFER[k]);							    
			#endif		   
		break;
		case PacketString:    // send \0-terminated string, such as copyright or debug info   
			buffersize=BUFFERStringTX(datos);
			#ifdef DEBUGUSBLevel1
			info=(int8*)datos;
			j=strlen((char*)info);
			printf("dispatchPacket=> strlen(info) en dispatchPacket=>%d \n",j);		   
			printf("dispatchPacket=> Tamaño de los datos en dispatchPacket=>%d \n",buffersize);
			#endif
		break;
		case PacketAxisSettings:
			buffersize=BUFFERAxisSettingsTX((tAxisSettings*)datos);		   
		break;
		case PacketAxisLimits:
			buffersize=BUFFERAxisLimitsTX((tAxisLimits*)datos);
		break;
		case PacketPosesvector:
		    buffersize=BUFFERVectorPoseTX(datos); 
		break;
		case PacketFecha:   // 14
			buffersize=BUFFERFechaTX(datos); 
		break;
		case PacketAlarma:
			buffersize=BUFFERAlarmaTX(datos); 
		break;
		case PacketTrace:
			buffersize=BUFFERTraceTX(datos);
		break;
		case Packetuint16:
			buffersize=BUFFERunit16TX(datos);
		break;
		default:
			return rcInvalidPacket;
    } // switch txPacket
	//buffersize solo es par, es decir siempre el buffer es par. Aunque el dato a enviar 
	//tenga una longitud de bytes impar. Internamente tPacketHeader.dataLength siempre
	//tiene la cantidad de bytes correcta del dato.
	rc=Envio_USB(TXBUFFER,buffersize);  
	free(TXBUFFER);
return rc;
} // dispatchPacket

#endif
/*****************************************************************************************/

int Esimpar(int i)
{
	int residuo=0;
    
	residuo= i % 2;

	if(residuo==0)
		return FALSE;
		
	return TRUE;
}
#ifdef _WIN32

/*****************************************************************************************/
void Mandar_uCreset(char DeviceName[],HWND hDlg)
{
	int request,rc;  
	char buffertemp=00;  
	char rest[6]="Reset";


	request=0xC5;
	rc=Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,DeviceName); //Mando reset al atmega 
	if(rc==rcOK)
	MessageBox(hDlg,"El Controlador se ha reiniciado correctamente", "Informacion", MB_OK);
	
}

int Mandar_Parada(char DeviceName[],HWND hDlg)
{
	int request,rc;  
	char buffertemp=00;  

	request=0xCC;
	rc=Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,DeviceName); //Mando reset al atmega
	if(rc==rcOK)
	MessageBox(hDlg,"Se ha efectuado una parada de emergencia en el Robot", "Informacion", MB_OK);
	
	return 0;
}

#endif

/*****************************************************************************************/
#ifdef _AVR_IO_H_
// mapping which (logical) axis is to find on which MaskAxis corresponds to Axis:
AxisMask Axis2AxisMask(tAxis Axis)
{
	switch(Axis) 
	{
		case AxisWaist 		: 	return WaistMask; 
		case AxisShoulder 	: 	return ShoulderMask; 
		case AxisElbow 		:	return ElbowMask; 
		case AxisPitch 		: 	return PitchMask; 
		
		case AxisRoll 		: 	return RollMask;
		case AxisAux1 		: 	return Aux1Mask; 
		case AxisAux2 		: 	return Aux2Mask;    
		case AxisAux3 		: 	return Aux3Mask; 			
		
		default 			: 	return NOAxisMask;
	}
  return PMDAxis1;  // we must return something
} // Axis2AxisMask


/*****************************************************************************************/
void Print_rxTrajectory(void)
{
	int16 pointIndex,axis,stop;
	
	printf("Print_rxTrajectory=> El Numero de puntos de la trayectoria es en packetIO.c => %d\n",rxTrajectory.size);
	printf("Print_rxTrajectory=> invalid mask en packetIO.c => %X\n",rxTrajectory.invalidmask);
	
	
	for(pointIndex=0;pointIndex < rxTrajectory.size;pointIndex++)
	{
		printf("Print_rxTrajectory=> En el punto %d de la trayectoria....(packetIO.c)\n",pointIndex+1);
			
		for(axis=0; axis < DOF; axis++) 
		{
			printf("Print_rxTrajectory=> La posicion de %s es en packetIO.c => %ld\n",AxisName(axis),
				   rxTrajectory.point[pointIndex].axis[axis].position);
			printf("Print_rxTrajectory=> La velocidad de %s es en packetIO.c => %ld\n",AxisName(axis),
				   rxTrajectory.point[pointIndex].axis[axis].velocity);
			printf("Print_rxTrajectory=> La aceleracion de %s es en packetIO.c => %ld\n",AxisName(axis),
				   rxTrajectory.point[pointIndex].axis[axis].acceleration);
			scanf("%d",&stop);
			
		} 
	}	
}
/*****************************************************************************************/


#endif

