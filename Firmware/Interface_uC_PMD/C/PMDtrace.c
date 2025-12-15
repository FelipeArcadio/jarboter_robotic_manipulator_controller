#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "c-motion.h"
#include "messages.h"
#include "PMDsetup.h"
#include "PMDtrace.h"
#include "mem.h"
#include "low_usb.h"
#include "packetIO.h"


/*Indica que es la primera vez que se toman los datos del buffer*/
uint8 printOK=0;
/*Conteo del Numero de variables configuradas en el PMD1*/
uint8 PMD1NVariables;
/*Conteo del Numero de variables configuradas en el PMD2*/
uint8 PMD2NVariables;

extern PMDAxisHandle hAxis[DOF]; 



/*****************************************************************************************/
void Memoryconfig(void)
{
	uint8 axis;
	
	//Defino el Comienzo y la longitud del Buffer de memoria para ambos chipsets
	
	for(axis=0; axis < DOF; axis++)
	{
		if(hAxis[axis].axis == 0)  
		{
			PMDSetBufferStart(&hAxis[axis],BUFFER_ID,BUFFER_START);
			PMDSetBufferLength(&hAxis[axis],BUFFER_ID,BUFFER_LENGTH);
		}
	}
}

/*****************************************************************************************/
void SetupSingleAxisTrace(tAxis Axis)
{
	
	PMDSetTraceVariable(&hAxis[Axis], PMDTrace1, Axis2PMDAxis(Axis), PMDTraceCommandedPosition );
	PMDSetTraceVariable(&hAxis[Axis], PMDTrace2, Axis2PMDAxis(Axis), PMDTraceActualPosition);
	PMDSetTraceVariable(&hAxis[Axis], PMDTrace3, Axis2PMDAxis(Axis), PMDTracePositionError);
	PMDSetTraceVariable(&hAxis[Axis], PMDTrace4, Axis2PMDAxis(Axis), PMDTraceEventStatus);
	
	PMDSetTracePeriod(&hAxis[Axis],200);				//120ms
	PMDSetTraceMode(&hAxis[Axis],PMDTraceOneTime);	
	PMDSetTraceStop(&hAxis[Axis], Axis2PMDAxis(Axis),  PMDTraceConditionEventStatus, PMDEventMotionCompleteBit, PMDTraceTriggerStateHigh );
	PMDSetTraceStart( &hAxis[Axis], Axis2PMDAxis(Axis), PMDTraceConditionUpdate, 0, 0);
	
}
/*****************************************************************************************/
void SetupAxisTrace(tCommand VisualTrace)
{
	int8 i,Axis,NvaribleVisual,VariableConf;
	
	NvaribleVisual=VisualTrace.Dato;
	printf("SetupAxisTrace=> Numero de variables a configurar %d \n",NvaribleVisual);
	Memoryconfig();
	printf("SetupAxisTrace=> Ejecute Memoryconfig \n");
	DisableVariable();
	printf("SetupAxisTrace=> Ejecute DisableVariable \n");
	if(NvaribleVisual>0 && NvaribleVisual <= 8) 
	{
		/*Reiniciamos el contreo de variables configuradas (son globales)*/
		PMD1NVariables=0;
		PMD2NVariables=0;
		for(i=0;i<NvaribleVisual;i++)
		{
			//Obtenemos el eje a configurar
			Axis=VisualTrace.Tracecomand[i].AxisTrace;
			//Obtenemos a que PMD pertenece, y llevamos la cuenta de la varialies configuradas para cada PMD
			VariableConf=Tracevar2PMDVar(VisualTrace,i);
			printf("SetupAxisTrace=> Configurando la variable %d = al CMD # %d en el EJE %s \n",VariableConf,VisualTrace.Tracecomand[i].TraceVar,AxisName(Axis));
			PMDSetTraceVariable(&hAxis[Axis],VariableConf,Axis2PMDAxis(Axis),VisualTrace.Tracecomand[i].TraceVar); 
			printf("SetupAxisTrace=> Configurando  Sample Trace_time %d \n",VisualTrace.TraceTime);		
			PMDSetTracePeriod(&hAxis[Axis],VisualTrace.TraceTime);
			PMDSetTraceMode(&hAxis[Axis],PMDTraceOneTime);	
			if(VisualTrace.TraceTraj==0)
				PMDSetTraceStop(&hAxis[Axis], Axis2PMDAxis(Axis),  PMDTraceConditionEventStatus, PMDEventMotionCompleteBit, PMDTraceTriggerStateHigh );
			PMDSetTraceStart( &hAxis[Axis], Axis2PMDAxis(Axis), PMDTraceConditionUpdate, BUFFER_ID, PMDTraceTriggerStateHigh );
		}
		
	}
}

/*****************************************************************************************/
void DisplayTraceResults(int Chipset,PMDuint8 nVariables)
{
	
	PMDuint32 i;
	PMDuint32 total_data;
	PMDuint32 readindex;
	PMDlong32 bufferdata ;
    int Axis;

	
	Axis = AxisWaist;
	if(Chipset==2) 
		Axis = AxisRoll;
	PMDGetTraceCount(&hAxis[Axis], &total_data);
	printf("DisplayTraceResults=> Total de Datos= %ld \n", total_data);
	
	do
	{
		PMDGetBufferReadIndex(&hAxis[Axis], BUFFER_ID, &readindex);
		printf("DisplayTraceResults=> readindex= %ld",readindex);
		
		for(i=0 ; i<nVariables; i++)
		{
			PMDReadBuffer(&hAxis[Axis], BUFFER_ID, &bufferdata);	
			if(i==3)			
				printf("DisplayTraceResults=> \t%#lX", bufferdata);
			else 
				printf("DisplayTraceResults=> \t%ld", bufferdata);
		}
		
		printf("\n");
	}
	while (readindex < (total_data - nVariables));	
} 

int8 Tracevar2PMDVar(tCommand VisualTrace,int8 TracevarIndex)
{
	int8 Result,Axis;
	
	
	Axis=VisualTrace.Tracecomand[TracevarIndex].AxisTrace;
	if(Axis==AxisRoll) 
	{
		Result=PMD2NVariables;
		PMD2NVariables++;
	}
	else
	{
		Result=PMD1NVariables;
		PMD1NVariables++;
	}
	
	return Result;

}


/*****************************************************************************************/
void EnvioTrazado2PC(uint8 N_traceVariable)
{
	uint8 Axis=0;
	PMDuint32 total_data_Chipset2=0;
	PMDuint32 total_data_Chipset1=0;
	PMDuint32 total_data=0;
	int32 Tracebufferdata[4];
	uint8 *BufferData;
	tPacketHeader PacketHeader;
	int16 i,buffersize;
	#ifdef DEBUGUSBLevel1
	uint16 spacefree;
	#endif
	total_data=0;
	
	PMDGetTraceCount(&hAxis[AxisWaist], &total_data_Chipset1);		
	printf("EnvioTrazado2PC=> total_data_Chipset1=  %ld\n",total_data_Chipset1);
	PMDGetTraceCount(&hAxis[AxisRoll], &total_data_Chipset2);
	printf("EnvioTrazado2PC=> total_data_Chipset2=  %ld\n",total_data_Chipset2);
	
	PMDSetBufferReadIndex(&hAxis[AxisWaist], BUFFER_ID,0);
	PMDSetBufferReadIndex(&hAxis[AxisRoll], BUFFER_ID,0);
	

	total_data=total_data_Chipset1+total_data_Chipset2;
	/*He configurado alguna variable para trazar*/
	if(N_traceVariable>0 && N_traceVariable<=8)
	{
		/*total_data = Numero de datos de 32 bists del trazado*/			
		Tracebufferdata[0]=total_data;
		Tracebufferdata[1]=total_data_Chipset1;
		Tracebufferdata[2]=total_data_Chipset2;
		printf("EnvioTrazado2PC=> Tracebufferdata[0]=  %ld\n",Tracebufferdata[0]);
		if(total_data_Chipset1>0)
			Axis=AxisWaist;
		/*Solo tamaño de los datos, 4N+12 siempre es par*/
		buffersize=(Tracebufferdata[0]*sizeof(int32))+(sizeof(int32)*3);
		fillPacketHeader(&PacketHeader,PacketTrace,buffersize,NULL); 
		BufferData=(uint8*)&PacketHeader;
		Envio_USB(BufferData,sizeof(tPacketHeader));
		/*Mando el Tracebufferdata[0] */
		BufferData=(uint8*)&Tracebufferdata[0];
		Envio_USB(BufferData,sizeof(int32));
		/*Mando el Tracebufferdata[1] */
		BufferData=(uint8*)&Tracebufferdata[1];
		Envio_USB(BufferData,sizeof(int32));
		/*Mando el Tracebufferdata[2] */
		BufferData=(uint8*)&Tracebufferdata[2];
		Envio_USB(BufferData,sizeof(int32));
		/*Grabo todos los datos*/
		printf("EnvioTrazado2PC=> Iniciando envio de trazado... \n");
		for(i=0;i<total_data;i++)
		{
			/*Estoy en el PMD2*/
			if(i>=(total_data_Chipset1))
				Axis=AxisRoll;
			/*Leo el dato*/
			PMDReadBuffer(&hAxis[Axis], BUFFER_ID,&Tracebufferdata[3]);	
			/*Mando el Tracebufferdata[3], dato lleido de la memoria del PMD */
			BufferData=(uint8*)&Tracebufferdata[3];
			Envio_USB(BufferData,sizeof(int32));
			//printf("Tracebufferdata[3]=  %ld, Index %d \n",(int32)Tracebufferdata[3],i);
		}
		printf("EnvioTrazado2PC=> Envio de trazado finalizado... \n");
		PMDSetBufferReadIndex(&hAxis[AxisWaist], BUFFER_ID,0);
		PMDSetBufferWriteIndex(&hAxis[AxisWaist], BUFFER_ID,0);
		PMDSetBufferReadIndex(&hAxis[AxisRoll], BUFFER_ID,0);
		PMDSetBufferWriteIndex(&hAxis[AxisRoll], BUFFER_ID,0);
	}
	printf("EnvioTrazado2PC=> total_data=  %ld\n ",total_data);
	printf("EnvioTrazado2PC=> Saliendo readbuffer\n ");
} 

void DisableVariable(void)
{

	PMDSetTraceVariable(&hAxis[AxisWaist], PMDTrace1, Axis2PMDAxis(AxisWaist), PMDTraceNoVariable);
	PMDSetTraceVariable(&hAxis[AxisWaist], PMDTrace2, Axis2PMDAxis(AxisWaist), PMDTraceNoVariable);
	PMDSetTraceVariable(&hAxis[AxisWaist], PMDTrace3, Axis2PMDAxis(AxisWaist), PMDTraceNoVariable);
	PMDSetTraceVariable(&hAxis[AxisWaist], PMDTrace4, Axis2PMDAxis(AxisWaist), PMDTraceNoVariable);	
	PMDSetTraceVariable(&hAxis[AxisRoll], PMDTrace1, Axis2PMDAxis(AxisRoll), PMDTraceNoVariable);
	PMDSetTraceVariable(&hAxis[AxisRoll], PMDTrace2, Axis2PMDAxis(AxisRoll), PMDTraceNoVariable);
	PMDSetTraceVariable(&hAxis[AxisRoll], PMDTrace3, Axis2PMDAxis(AxisRoll), PMDTraceNoVariable);
	PMDSetTraceVariable(&hAxis[AxisRoll], PMDTrace4, Axis2PMDAxis(AxisRoll), PMDTraceNoVariable);	
}