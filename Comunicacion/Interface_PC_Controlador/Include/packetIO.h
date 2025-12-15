#ifndef __PACKETIO_H__
#define __PACKETIO_H__

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      packetIO.h
//  Ubicación fuente: C:\Jarboter_V_2_1\Comunicacion\Interface_PC_Contorlador\C\packetIO.c
//
//  Proyecto: Controlador Robótico de 5 grados de libertad
//  
//  Propósito General:	
//  Sistema de comunicación simétrico, el cual debe verse igual desde el lado del PC como del microcontrolador. 
//
//  El protocolo de comunicación implementado es el siguiente:
//
//	- Para  enviar una trama, primero se manda la Cabecera (header)  el cual contiene toda la información 
//	relacionada con ésta; como el tipo de dato, la cantidad de datos, el  Checksum de los datos y de la cabecera. 
//	Con esta información el receptor puede determinar el tipo de variable a recibir con el fin  reservar 
//	memoria para  almacenar  los datos.  
//
//	- Una vez el receptor ha leído correctamente la cabecera, el emisor debe enviar los datos.
//
//	- El receptor calcula  el CheckSum mediante al sumatoria de todos los datos recibidos, que se compara con el campo 
//	tDataChecksum de la cabecera, para detectar un posible error de trasmisión.
//
//	|--------------------------------------------------------------|
//	|                                  CABECERA                    |
//	|--------------------------------------------------------------|
//	|   TIPO DE    | lONGITUD EN | CHECKSUM DE   | CHECKSUM DE     |     |-----------------|
//	|     DATO     | BYTES       | LOS DATOS     |    HEADER       |     |     DATOS       |
//	|--------------|-------------|---------------|-----------------|     |-----------------|
//	| tDataType:   | tDataLength:| tDataChecksum:| tHeaderChecksum:|
//	|   int16      |   int16     |   int16       |    int16        |
//	|--------------|-------------|---------------|-----------------|
//
//  Compilado en:
//    avr-gcc 4.3.3 ó Microsoft Visual Studio 6.0 API 
//
//  Author: Felipe Hurtado
//          David Osorio
//
//  Version: 1.0
//  
// 
//  Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#if !defined _WIN32
#include <avr/io.h>
#include "low_usb.h"
#endif
#ifdef _WIN32
	#include "USBfunc.h"     
	#include <windows.h>			//funciones básicas de ventanas graficas
	#include <windowsx.h>			//funciones extendidas de ventanas graficas
	#include <string.h>
	#include <stdio.h>
#endif

#include "types.h" 

#if defined(__cplusplus)
extern "C" 
{
#endif
	
	/** \defgroup Sistema_de_Comunicación SISTEMA DE COMUNICACIÓN ENTRE EL PC Y EL CONTROLADOR ROBÓTICO
	\brief Sistema de comunicación simétrico, el cual debe verse igual desde el lado del PC como del microcontrolador. 

	El protocolo de comunicación implementado es el siguiente:

	- Para  enviar una trama, primero se manda la Cabecera (header)  el cual contiene toda la información 
	relacionada con ésta; como el tipo de dato, la cantidad de datos, el  Checksum de los datos y de la cabecera. 
	Con esta información el receptor puede determinar el tipo de variable a recibir con el fin  reservar 
	memoria para  almacenar  los datos.  

	- Una vez el receptor ha leído correctamente la cabecera, el emisor debe enviar los datos.

	- El receptor calcula  el CheckSum mediante al sumatoria de todos los datos recibidos, que se compara con el campo 
	tDataChecksum de la cabecera, para detectar un posible error de trasmisión.
	
	\verbatim
	|--------------------------------------------------------------|
	|                                  CABECERA                    |
	|--------------------------------------------------------------|
	|   TIPO DE    | lONGITUD EN | CHECKSUM DE   | CHECKSUM DE     |     |-----------------|
	|     DATO     | BYTES       | LOS DATOS     |    HEADER       |     |     DATOS       |
	|--------------|-------------|---------------|-----------------|     |-----------------|
	| tDataType:   | tDataLength:| tDataChecksum:| tHeaderChecksum:|
	|   int16      |   int16     |   int16       |    int16        |
	|--------------|-------------|---------------|-----------------|
	\endverbatim		
		
	\code #include <packetIo.h> \endcode 

	*/	

			
/** \defgroup Protocolo_comunicacion_PC PROTOCOLO DE COMUNICACIÓN MEDIANTE PAQUETES
	\ingroup Sistema_de_Comunicación
	\brief Contiene todas las funciones que intervienen en el proceso tanto de recepción como de transmisión  datos 
	mediante paquetes. 
	
	
*/

	/** \ingroup Protocolo_comunicacion_PC
	\brief Vector que almacena la configuración leída del robot*/
	tAxisSettings rxAxisSettings[DOF]; //    
	
	/** \ingroup Protocolo_comunicacion_PC
	\brief Vector dinámico que guarda los datos del trazado obtenidos por el sistema de control de movimiento*/
	int32        *rxTraceBuffer; 
    
	/** \ingroup Protocolo_comunicacion_PC
	\brief Guarda el ID de la pose cuando el usuario lo solicita*/
	uint16        rxPosesActual;
	
	
	/** \defgroup Recepcion RECEPCIÓN
	\ingroup Protocolo_comunicacion_PC
	\brief 	Funciones relacionadas con la recepción de paquetes, las cuales consisten en almacenar en la variable 
	global RXBUFFER, los datos que provienen directamente de la interfaz USB, para luego mediante la información de 
	la cabecera actualizar el buffer de recepción respectivo, si el cheksum de los datos recibidos es correcto.
	*/	
		
		#ifdef _AVR_IO_H_
		/** \ingroup Recepcion
		\brief (Compilada sólo en el microcontrolador). Función principal para la recepción de datos, esta función llama 
		readPacketHeaderRX para obtener el tipo de dato recibido con el fin de colocar la información recibida 
		en su respectivo buffer de recepción. 
		@return rcTimeoutReadingData    Se cumplió el tiempo de espera y no se Recibió ningún dato.         
		@return rcInvalidPacket    	 No es uno de los paquetes de datos conocidos. 
		@return rcTooFewData            La longitud de datos (dataLength) es menor a la esperada.
		@return rcInvalidHeaderChecksum Checksum de la cabecera incorrecto.
		@return rcInvalidChecksum  	 Cuando el Checksum de los datos es cero o no es correcto.          
		@return rcInvalidCommand   	 El comando recibido no es conocido.         
		@return rcInvalidError     	 El código de error no es conocido.         
		@return rcOK 					 No hay errores.
		*/
		int16 receiptPacket(void);	
		#endif
		
		#ifdef _WIN32
		/** \ingroup Recepcion
		\brief (Compilada sólo en el PC). Misma funcionalidad que receiptPacket(void),  utiliza el driver y funciones 
		de la API de WINDOWS para comunicarse con el controlador de periférico USB CY6801.
		@param Devicename[] Contiene el nombre que el driver asigna al dispositivo USB una vez conectado y 
							detectado por el sistema operativo.
		@param hDlg Contiene el manejador de la Ventana con que se está trabajando, esto con el fin de 
		poder desplegar mensajes de error para el usuario.
		*/
		int16 receiptPacket(char Devicename[],HANDLE hDlg);
		#endif
		
		/** \ingroup Recepcion
		\brief Esta función se encarga de leer la cabecera del buffer de recepción y es llamada cada vez que se recibe  
		información proveniente del bus USB para poder identificar el tipo de dato y toda la información relacionada 
			con el paquete.
		@param *ppacketHeader 			 Es un apuntador en donde se almacena la cabecera (Header) de los datos.
		@return rcInvalidPacket	    	 Tipo de packete desconocido.
		@return rcTooFewData             La longitud de Datos (dataLength) es menor a lo esperado.
		@return rcInvalidHeaderChecksum  Error de Checksum en la cabecera de los datos (Header).
		@return rcInvalidChecksum	  	 Cuando el Checksum de los datos es cero o no es correcto .         
		@return rcOK 					 No hay errores.
		*/
		int16 readPacketHeaderRX(tPacketHeader *ppacketHeader);
		
		/** \ingroup Recepcion
		\brief Su principal función es leer los datos almacenados en RXBUFFER que provienen directamente del bus
		USB una vez es leída la cabecera correctamente. 
		@param *ppacketHeader 			Es un apuntador a la cabecera (Header) de los datos.
		@param *pdata 					Es un apuntador a la posición de memoria en donde se almacenan los datos.
		@return rcOK,				    Los Datos han sido leídos y almacenados satisfactoriamente.
		@return rcInvalidChecksum	    Error de Checksum en los datos.
		@return rcInvalidData	        Longitud de los datos es cero*/
		int16 readPacketDataRX(tPacketHeader *ppacketHeader, uint8 *pdata); 
		
		/** \ingroup Recepcion
		\brief Actualiza la estructura de tipo tTrajectory leyendo la información directamente desde 
		RXBUFFER.  Antes de ejecutar esta función es necesario que la cabecera  se haya recibido con 
		éxito. 
		@param *pTrajectory 			El es apuntador a la variable de tipo tTrajectory donde se quieren almacenar los datos.
		@param packetHeader 			La cabecera de los datos a leer.
		@return rcInvalidChecksum     	Error de Checksum en los datos.
		@return rcOK 					No hay errores.
		*/
		int16 readTrajectoryRX(tTrajectory *pTrajectory,tPacketHeader packetHeader); 
		
	
	/** \defgroup Transmision TRANSMISIÓN
	\ingroup Protocolo_comunicacion_PC
	\brief 	Funciones relacionadas con la transmisión de paquetes, las cuales consisten en almacenar en la variable 
	global de transmisión TXBUFFER, la cabecera y los datos que serán enviados a través de la interfaz USB.
	*/	
		
		/** \ingroup Transmision
		\brief Inicializa la variable minPacketData[], la cual contiene los tamaños mínimos  para cada paquete 
		de datos, con el fin de poder determinar algún error cuando se está 	accediendo a la información de 
		la cabecera recibida.
		*/
		void initPacketIO(void);
		
		/** \ingroup Transmision
		\brief Esta función llena la cabecera con la siguiente información:
		\verbatim
		-El tipo de paquete a enviar.
		-La cantidad de datos (bytes) que contiene el paquete a ser enviado.
		-El checksum de los datos, calculado por la sumatoria de todos los datos a enviar.
		-El checksum de la cabecera, calculado cuando la información de la cabecera está actualizada. 
		\endverbatim
		@param *ppacketHeader 				Es un apuntador a la cabecera (Header) de los datos.
		@param dataType 					Tipo de Dato.
		@param dataLength 					Tamaño de los datos.
		@param pdata 						Apuntador hacia los datos.
		@return rcOK                		Cabecera que ha sido correctamente llenada.
		@return rcInvalidPacket     		Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        		La longitud de datos (dataLength) es menor a la esperada. */
		int16 fillPacketHeader(tPacketHeader *ppacketHeader, tDataType dataType, tDataLength dataLength,
					uint8 *pdata); 
		
		
		/** \ingroup Transmision
		\brief Actualiza la variable global de tranmisión TXBUFFER, almacenando la cabecera y los datos. Antes de 
		llamar a esta función la cabecera debe de contener toda la información relaciona con el paquete a enviar.
		*/
		void BUFFERPacketDataTX(tPacketHeader *ppacketHeader, uint8 *pdata,tDataType tipo);
		
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo comando 
		(tcomand).  Esta función llama a fillPacketHeader para construir la cabecera del paquete y a BUFFERPacketDataTX 
		para colocar tanto la cabecera como los datos del paquete en TXBUFFER.
		@param  command 			Es un apuntador a la estructura de tipo tCommand el cual contiene el comando a enviar.
		@return rcOK	            Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERCommandTX(tCommand* command); 
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo char (uint8*),
		la cadena es ingresada como un vector de bytes. Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.
		@param string 			    Apuntador a la cadena de caracteres a enviar.
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERStringTX(uint8 *string);
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo trayectoria
		(tTrajectory). 
		Esta función llama a fillPacketHeader para construir la cabecera del paquete y a BUFFERPacketDataTX para 
		colocar tanto la cabecera como los datos del paquete en TXBUFFER.
		@param pTrajectory 			Apuntador a la estructura trayectoria a enviar.
		@param puntos      			Cantidad de puntos de la trayectoria
		@param tipo        			Puede tomar dos valores: PacketTrajectory ó PacketTeachPendant.
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERTrajectoryTX(tTrajectory *pTrajectory,int16 puntos,tPacket tipo);
		
		/** \ingroup Transmision 
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo tAxisSettings. 
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.
		@param paxisSettings 		 Apuntador a la estructura tipo tAxisSettings a enviar.
		@return rcOK                 Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket      Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData         La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERAxisSettingsTX(tAxisSettings *paxisSettings);
		
		/** \ingroup Transmision
		\brief Crea un buffer de Transmision almacenado en TXBUFFER, que contiene el header seguido de una 
		trama de tipo tAxisLimits. 
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.
		@param paxisLimits Apuntado a la estructua tipo tAxisLimits a enviar
		@return rcOK                 Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket      Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData         La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERAxisLimitsTX(tAxisLimits *paxisLimits);
		
		/** \ingroup Transmision
		\brief  Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo tFecha 
		(uint8 *), la estructura tFecha es ingresada mediante un cast a la función como un vector de bytes. 
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.
		@param Fecha 				Apuntador a la estructura tipo tFecha que se desea enviar
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERFechaTX(uint8 * Fecha); 
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo tAlarm (uint8 *), 
		la estructura tAlarm es ingresada mediente un cast a la funcion como un vector de bytes. 
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.
		@param AlarmaTx  			Apuntador a la estructura tipo tAlarm que se desea enviar
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERAlarmaTX(uint8 * AlarmaTx);
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo TraceTX (uint8 *), 
		los datos de int32 TraceTX que coresponde a la informacion del trazador son	ingresados como un vector de bytes.
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.
		@param  TraceTX  			 Apuntador el primer byte de la informacion del trazador.
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */	
		int BUFFERTraceTX(uint8 * TraceTX); 
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo Pose, 
		la estructura PoseRobot es ingresada mediante un cast a la función. 
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.		
		@param  pVectorpose  		 Apuntador a el vector de poses a enviar
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */
		int BUFFERVectorPoseTX(uint8 * pVectorpose);		
		
		
		/** \ingroup Transmision
		\brief Crea un buffer de transmisión que se almacena en TXBUFFER el cual contiene un paquete tipo (uint8 *), 
		la variable tipo unit16 es ingresada como un vector de bytes. 
		Esta función llama a fillPacketHeader para construir 
		la cabecera del paquete y a BUFFERPacketDataTX 	para colocar tanto la cabecera como los datos del paquete 
		en TXBUFFER.	
		@param  unit16TX  			 Apuntador a el dato tipo 
		@return rcOK                Cabecera que ha sido llenada correctamente.
		@return rcInvalidPacket     Se está tratando de llenar la cabecera con un paquete inválido.
		@return rcTooFewData        La longitud de datos (dataLength) es menor a la esperada. */	
		int BUFFERunit16TX(uint8 * unit16TX); 		
		
		
		
		#ifdef _AVR_IO_H_
		/** \ingroup Transmision
		\brief (Compilada sólo en el microcontrolador). Función principal para el envío de datos, esta función 
		llama fillPacketHeader para construir la cabecera para luego reservar memoria para los datos. 
		Finalmente coloca tanto la cabecera como los datos en el buffer de Transmision, TXBUFFER. 
		@param tipodato 				 Tipo de dato a enviar.
		@param datos    				 Apuntador a los datos a enviar.
		@return rcTimeoutReadingData     Se cumplió el tiempo de espera y no se envió ningún dato.          
		@return rcInvalidPacket     	 No es uno de los paquetes de datos conocidos. 
		@return rcTooFewData             La longitud de datos (dataLength) es menor a la esperada.
		@return rcInvalidHeaderChecksum  Checksum de la caberera incorrecto.
		@return rcInvalidChecksum   	 Cuando el Checksum de los datos es cero ó no es correcto.          
		@return rcInvalidCommand    	 El comando recibido no es conocido.         
		@return rcInvalidError      	 El código de error no es conocido.         
		@return rcOK  					 No hay errores.
		*/
			int16 dispatchPacket(tPacket tipodato,uint8* datos);
		#endif
		
		
		#ifdef _WIN32
		/** \ingroup Transmision
		\brief (Compilada sólo en el PC).  Misma funcionalidad que receiptPacket(void),  utiliza el driver y funciones 
		de la API de WINDOWS para comunicarse con el controlador de periférico USB CY6801.
		@param Devicename[] Contiene el nombre que el driver asigna al dispositivo USB una vez conectado y 
							detectado por el sistema operativo.
		@param hDlg Contiene el manejador de la Ventana con que se está trabajando, esto con el fin de 
		poder desplegar mensajes de error para el usuario.
		*/
			int16 dispatchPacket(tPacket tipodato,uint8* datos,char Devicename[], HANDLE hDlg ); 
		#endif

		
	/** \defgroup Checksum CHECKSUM
		\ingroup Protocolo_comunicacion_PC
		\brief Contiene todas las funciones que intervienen el cálculo del Checksum de los datos recibidos o que se van
		a enviar.
	*/		
		/** \ingroup Checksum
		\brief Calcula el checksum de los datos según el tipo. El dato es dado como un puntero y tratado como un 
		arreglo de tipo	unsigned char.
		@param length Número de bytes que contienen los datos.
		@param pdata Apuntador a los datos que se les calcula el checksum.
		@param dataType Tipo de Paquete.
		@return Checksum de los datos (la suma de todos los datos).	
		*/
		tDataChecksum calcChecksum(tDataLength length, uint8* pdata,tDataType dataType);
		
		/** \ingroup Checksum
		\brief Esta función compara el checksum de los datos recibidos con el campo de la cabecera  (tDataChecksum).
		@param length Longitud de los datos.
		@param *data Apuntador hacia los datos.
		@param dataType Tipo de Paquete.
		@param checksum (de los datos).
		@return TRUE si el checksum es igual FALSE si no.
		*/
		int16 checksumOk (tDataLength length, uint8* data, tDataChecksum checksum,tDataType dataType);				

		
	/** \defgroup Tools UTILIDADES
		\ingroup Protocolo_comunicacion_PC
		\brief Contiene todas las funciones relacionadas con el manejo de paquetes de datos especiales tanto en el
		microcontrolador como en el PC.
	*/			
		#ifdef _AVR_IO_H_
			/** \ingroup Tools
			\brief (Compilada sólo en el microcontrolador). Devuelve la máscara asignada a Axis.
			@param Axis Eje al cual se desea obtener la máscara.
			@return AxisMasK La máscara asignada.
			*/
			AxisMask Axis2AxisMask(tAxis Axis);
			
			
			/** \ingroup Tools
			\brief (Compilada sólo en el microcontrolador). Copia la información que se encuentra en rxtrayectoria 
			a una estructura PoseRobot.
			@param Apuntador a la estructura PoseRobot que se quiere actualizar.
			*/
			void rxTrayectoriatoPose(PoseRobot* EEpromPose);
			
			
			/** \ingroup Tools
			\brief (Compilada sólo en el microcontrolador). Imprime todo los puntos de la estructura rxTrajectory 
			en consola.
			*/
			void Print_rxTrajectory(void);
		#endif
			/** \ingroup Tools
			\brief Determina si un número  es impar o no.
			@param i Número a evaluar.
			@return TRUE Si es par.
			@return FALSE Si no es par.
					
			*/
			int Esimpar(int i);
		#ifdef _WIN32
			/** \ingroup Tools
			\brief (Compilada sólo en el PC). Envía un reset por hardware a través del bus USB para reiniciar 
			el controlador robótico.
			@param DeviceName[] Nombre del dispositivo identificado por el archivo ezusbw2k.INF.
			@param hDlg Manejador la ventana principal.
			*/
			void Mandar_uCreset(char DeviceName[],HWND hDlg);
			
			
			/** \ingroup Tools
			\brief (Compilada sólo en el PC). Envía una parada de emergencia por hardware a través del bus USB.
			@param DeviceName[] Nombre del dispositivo identificado por el archivo ezusbw2k.INF.
			@param hDlg Manejador la ventana principal
			*/
			int Mandar_Parada(char DeviceName[],HWND hDlg);
			
			
		#endif
#if defined(__cplusplus)
}
#endif

#endif
