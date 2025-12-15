#include "USBfunc.h"
#include <windows.h>			//funciones basicas de ventanas graficas
#include <windowsx.h>			//funciones extendidas de ventanas graficas
#include "types.h"			
#include <string.h>				//manejo de caracteres
#include <stdio.h>				//Entrada y salida estandar
#include <stdlib.h>     
#include <winioctl.h>           //manejo del drivers (Manderle comandos al drivers)
#include <process.h>			//procesos multitareas
#include "COMMONtypes.h"
#include "mem.h"

/*****************************************************************************************/
BOOLEAN bOpenDriver (HWND hDlg,HANDLE * phDeviceHandle, PCHAR devname)
{
	char completeDeviceName[64] = "";
	char pcMsg[64] = "";
   
	strcat (completeDeviceName,"\\\\.\\");
	strcat (completeDeviceName,devname);
   
	*phDeviceHandle = CreateFile(completeDeviceName,
      GENERIC_WRITE,
      FILE_SHARE_WRITE,
      NULL,
      OPEN_EXISTING,
      0,
      NULL);

	if (*phDeviceHandle == INVALID_HANDLE_VALUE) 
	{
		MessageBox(hDlg, "Fallo de comunicación - revise el cable USB", "Advertencia", MB_OK);
		return (FALSE);      
	} 
	else 
	{
		return (TRUE);
	} /*else*/
}

/*****************************************************************************************/
void Ini_estruc_USB(BULK_TRANSFER_CONTROL* BulkControl ,THREAD_CONTROL* ThreadControl, ULONG PipeNum, HANDLE hDevice,
					PVOID inBuffer, ULONG TransferSize, HANDLE CompleteEvent,int tipo )
{
	
	BulkControl->pipeNum = PipeNum;  
	ThreadControl->hDevice = hDevice; 
	ThreadControl->InBuffer = (PVOID)BulkControl; //(Coloco el Numero del Endpoint)
	ThreadControl->InBufferSize = sizeof(BULK_TRANSFER_CONTROL); 
	ThreadControl->OutBuffer = inBuffer;       //OutBuffer no tiene que ver con IN o OUT USB, solo es el resultado de la operacion
	ThreadControl->OutBufferSize = TransferSize;
	ThreadControl->completionEvent = CompleteEvent;
	ThreadControl->status = FALSE;
    ThreadControl->BytesReturned = 0;
	if(tipo==1)
	    ThreadControl->Ioctl = IOCTL_EZUSB_BULK_WRITE;
	else
		ThreadControl->Ioctl = IOCTL_EZUSB_BULK_READ;		
}

/*****************************************************************************************/
void TransferThread(PTHREAD_CONTROL threadControl)  
{
 
	threadControl->status = DeviceIoControl (threadControl->hDevice,
                                             threadControl->Ioctl,
                                             threadControl->InBuffer,
                                             threadControl->InBufferSize,
                                             threadControl->OutBuffer,
                                             threadControl->OutBufferSize,
                                             &threadControl->BytesReturned,
                                             NULL);
	// if an event exists, set it
	if (threadControl->completionEvent) //SI FUE CREADA
		SetEvent(threadControl->completionEvent);  
	//pone el evento a estado señalado on, SIGNIFICA QUE HA TERMINADO LA TRANSFERENCIA
	return;
}

/*****************************************************************************************/
int Mandar_verdorR(HWND hDlg, UCHAR request,char * Buffer,char * DeviceName)
{
  
	VENDOR_OR_CLASS_REQUEST_CONTROL myRequest;
	HANDLE hOutDevice;
	char *buffer;
    ULONG bufferSize = 0;
	int rc,nBytes=1;
	buffer=malloc(sizeof(char)*10);
		   
    if(bOpenDriver(hDlg,&hOutDevice,DeviceName)==TRUE)
	{
	    myRequest.request = request;
        myRequest.value = 0x0000;
        myRequest.index = 0xbeef;
        myRequest.direction = 0x01;
	    myRequest.requestType=2; // vendor specific request type (2)
        myRequest.recepient=0; // recepient is device (0)
        bufferSize = 0x02;   
	   	rc = DeviceIoControl (hOutDevice,
			IOCTL_EZUSB_VENDOR_OR_CLASS_REQUEST,
			(PVOID)&myRequest,
			sizeof(VENDOR_OR_CLASS_REQUEST_CONTROL),
			(PVOID)buffer,
			bufferSize,
			(unsigned long *)&nBytes,
			NULL);
        *Buffer=*buffer;   //Esto es para poder recivir el buffer del request
        CloseHandle (hOutDevice);
		return rcOK;
	}
return rcCommunicationError;                
}

/*****************************************************************************************/
int Mandar_datos_Por_USB(HWND hDlg,ULONG OUTPipeNun, PVOID datos,int Nundata,char* DeviceName )
{
  
	HANDLE WriteCompleteEvent;
	BULK_TRANSFER_CONTROL   outBulkControl;
	THREAD_CONTROL outThreadControl;
	HANDLE  hOutDevice;
	int tiempo=1;
	PUCHAR buffertemp=00; 


	if(bOpenDriver(hDlg,&hOutDevice,DeviceName)==TRUE) //creo el puente de comunicacion entre el sistema operativo y el dispocitivo
	{	
		WriteCompleteEvent = CreateEvent(0,FALSE,FALSE,NULL);                //Se crea un Evento que representa la una transferencia de escritura
		Ini_estruc_USB(&outBulkControl,&outThreadControl,OUTPipeNun,hOutDevice,        //Se confira la estructura outThreadControl nesecaria para la transferencia
					datos,Nundata,WriteCompleteEvent,1);
	
		_beginthread(TransferThread,0,&outThreadControl); 
                             //Se crea el Proceso HILO donde se estara ejecutando  la funcion TransferThread																				   //con le parametros outThreadControl
		WaitForSingleObject(WriteCompleteEvent,10000);	
  								//Se espera un tiempo de timeout en esta caso 10000 = 10s
	    if(outThreadControl.status==FALSE)
			return rcTimeoutSendingData;											   //Si la operacion del codigo IOCTL (IOCTL_EZUSB_BULK_WRITE) no es exitosa retorno rcTimeoutSendingData;
		CloseHandle (hOutDevice);
		return rcOK;
	}
return rcCommunicationError;

}

/*****************************************************************************************/
int Leer_datos_Por_USB(HWND hDlg,ULONG InPipeNun, PVOID datos,int Nundata,char* DeviceName )
{
	////OJO NO SE PUEDE CERRAR EL HANDLE CUANDO GPIF ESTA OCUPADO
	HANDLE ReadCompleteEvent;
	BULK_TRANSFER_CONTROL   inBulkControl;
	THREAD_CONTROL inThreadControl;
	HANDLE hInDevice;
	int tiempo=10000;
	PUCHAR buffertemp=00; 

	if(bOpenDriver(hDlg,&hInDevice,DeviceName)==TRUE)
	{
		// Los mismo pasos  pero con otro codigo IOCTL (IOCTL_EZUSB_BULK_READ)
		ReadCompleteEvent = CreateEvent(0,FALSE,FALSE,NULL);
		Ini_estruc_USB(&inBulkControl,&inThreadControl,InPipeNun,hInDevice,
					datos, Nundata,ReadCompleteEvent,0);
		_beginthread(TransferThread,0,&inThreadControl);
		WaitForSingleObject(ReadCompleteEvent,INFINITE);  
		if(inThreadControl.status==FALSE)
		{  
			return rcTimeoutReadingData; //ojo el handle queda abierto
		}
		CloseHandle (hInDevice);
		return rcOK;
	}
	return rcCommunicationError;
}

/*****************************************************************************************/
//Siempre que algo sale mal hay que usar abortar porque si no el driver que ABIERTO!!!
//pipeNum = 0 endpoint 2 OUT
//          1 endpoint 8 INT 
int Abortar(HWND hDlg,ULONG pipeNun,char* DeviceName )
{
	int nBytes=0,request;	
	BOOL result;
	char buffertemp=00;  
	HANDLE hOutDevice;

	if(bOpenDriver(hDlg,&hOutDevice,DeviceName)==TRUE)
	{
        request=0xC6;  // low in_token_event y INT_FLAGin = 1
	    Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,DeviceName);
		do
		{
            request=0xC9; // Cancela el gpif
	        Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,DeviceName);
		}
		while(!UsbGpifIDEL(hDlg,DeviceName));				
	    result=DeviceIoControl (hOutDevice,
			IOCTL_Ezusb_ABORTPIPE,          
			&pipeNun,
			sizeof(ULONG),
			NULL,
			0,
			(unsigned long *)&nBytes,
			NULL);  //Cancela la pedicion de utilizar el USB 
        request=0xC4;  //Limpia datos en fifo de hardware
	    Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,DeviceName);
            //Si resul == 0 Es que hubo un error
		CloseHandle (hOutDevice);
		return result;                                     
	}
	return rcCommunicationError;
  //Si es exitoso  la operacion retorna 1
}

/*****************************************************************************************/
//Usa el driver y lo cierra
int UsbGpifIDEL(HWND hDlg,char* DeviceName)
{
    int request,result;
	char buffertemp=00;  

    request=0xE0;
	Mandar_verdorR(hDlg,(UCHAR)request,&buffertemp,DeviceName);
	result=(int)buffertemp;
	result=result&0x80;
	if (result!=0x80)
	{
		return 0;
	}	

	return 1;
}

/*****************************************************************************************/
BOOLEAN Esta_Conectado_USB(HWND hDlg,char* DeviceName)
{
    HANDLE hdevice;        //guarda el acceso derecto hacia el dispositivo

    if (bOpenDriver (hDlg,&hdevice, DeviceName) == TRUE)
	{
        CloseHandle (hdevice);
	    return TRUE;
	}
	return FALSE;
}