/**	\defgroup firmware_usb   FIRMWARE CONTROLADOR DE PERIFÉRICOS USB
	\ingroup Sistema_de_Comunicación
	\brief Corresponde al código que corre en el controlador de periféricos USB CY7C68013 FX2.
	
 */


/**	\ingroup firmware_usb
	\brief Estas funciones junto con el módulo GPIF desarrollan el flujo de comunicación entre
	el microcrontrolador ATMega 1280 y el PC.
*/


#pragma NOIV               		// Do not generate interrupt vectors

#include "fx2.h"
#include "fx2regs.h"
#include "fx2sdly.h"			// SYNCDELAY macro


extern BOOL   GotSUD;			// Received setup data flag,	Firmware frameworks
extern BOOL   Sleep;			//	Use in Firmware frameworks
extern BOOL   Rwuen;			//	Use in Firmware frameworks
extern BOOL   Selfpwr;			//	Use inFirmware frameworks


static WORD temp;				// VX_C7 aroja a temp=temporal, temporal EP2TC
static BOOL   INT_FLAGout = 0;  // Event flg for INT One time on OUT (PC->USB->uC), SET VX_CA, SEE VX_CB
static BOOL   INT_FLAGin = 1;   // Event flg for INT One time on IN  (PC<-USB<-uC), SET VX_C6 
static BOOL   enum_high_speed = 0;     // Bandera para indicar si esta en usb 2.0

WORD   xdata IN_BC=0;           // Cantidad de byte enviados hacia el PC
BYTE   Configuration;			// Current configuration
BYTE   AlternateSetting;		// Alternate settings
BOOL   in_token_event = 0;      // Event flg for PC READ
BOOL   Mod_TCIN =0;             // Por defecto no se quiere modificar el TC



//////////////DECLARACION DE LOS PUETOS

sbit RESET   = IOA ^ 0;  
sbit INT_USB = IOA ^ 2;
sbit USB_FLAG_WRITE   = IOA ^ 6;
sbit USB_FLAG_READ   = IOA ^ 7;

/////////  VENDOR REQUEST /////////////////////////////////////////////

#define VX_E0 0xE0    // return status of GPIF, Endpoint 2,4,6,8 GPIF slave FIFO Trigger-> BIT 7 GPIF DONE 
#define VX_C0 0xC0    // set in_token_event
#define VX_C1 0xC1    // flag int_in
#define VX_C3 0xC3    // flaf int_out
#define VX_C4 0xC4    // FLUSH EP2 EP8
#define VX_C5 0xC5    // RESET
#define VX_C6 0xC6    // low in_token_event y set INT_FLAGin
#define VX_C7 0xC7    // Temporal LSB
#define VX_C8 0xC8    // Quiero Modificar el TC
#define VX_C9 0xC9    // Abort Gpif
#define VX_CA 0xCA    // Set INT_FLAGout  
#define VX_CB 0xCB    // Examinar el INT_FLAGout
#define VX_CC 0xCC    // Botton emergency
#define VX_CD 0xCD    // EP8GPIFTCH 
#define VX_CE 0xCE    // EP8GPIFTCL  
#define VX_D0 0xD0    // EP8AUTOINLENL 
#define VX_D1 0xD1    // EP8AUTOINLENH 



void GpifInit( void );

// Some #defines from gpif.c, typically be in a common header file
#define GPIF_FLGSELPF 0
#define GPIF_FLGSELEF 1
#define GPIF_FLGSELFF 2

#define GPIFTRIGWR 0
#define GPIFTRIGRD 4

#define GPIF_EP2 0
#define GPIF_EP4 1
#define GPIF_EP6 2
#define GPIF_EP8 3


//#define USING_REVD				// Select silicon revision (comment for REVB)

void TD_Init( void )
{ 
	/*Called once at startup*/
	CPUCS = 0x10;				// CLKSPD[1:0]=10, for 48MHz operation
	GpifInit( );				// Configure GPIF from GPIFTool generated waveform data
    /*INCIALIZANDO EL GPIF*/
	GPIFADRH = 0x00;			// Setup initial gpif address 
	GPIFADRL = 0x00;			// ...outputs.
	EP8GPIFTCH = 0x00;			// Setup initial transaction count
	EP8GPIFTCL = 0x08;			// EP8GPIFTC = 8 bytes
	SYNCDELAY;
	EP8GPIFFLGSEL=0X02;			// Using Fifi FULL flAG
	/*CONFIGURANDO LAS FIFOS*/
	SYNCDELAY;
	FIFORESET = 0x80;           // activate NAK-ALL to avoid race conditions
	SYNCDELAY;                  // see TRM section 15.14
	FIFORESET = 0x02;           // reset, FIFO 2
	SYNCDELAY;                  // 
	FIFORESET = 0x08;           // reset, FIFO 8
	SYNCDELAY;                  // 
	FIFORESET = 0x00;           // deactivate NAK-ALL
	SYNCDELAY;
	EP2FIFOCFG = 0x05;          //MANUAL OUT 16BIT 
	SYNCDELAY;
	EP8FIFOCFG = 0x0D;  		//AUTO IN  16 BIT
	//Configuro auto IN
	if( EZUSB_HIGHSPEED( ) )
	{ // FX2 enumerated at high speed
    	SYNCDELAY;                  // 
    	EP8AUTOINLENH = 0x02;       // set AUTOIN commit length to 512 bytes
    	SYNCDELAY;                  // 
    	EP8AUTOINLENL = 0x00;
    	SYNCDELAY;                  
    	enum_high_speed = 1;
  	}
  	else
  	{ // FX2 enumerated at full speed
    	SYNCDELAY;                   
    	EP8AUTOINLENH = 0x00;       // set AUTOIN commit length to 64 bytes
    	SYNCDELAY;                   
    	EP8AUTOINLENL = 0x40;
    	SYNCDELAY;                  
  	}      
	/*INCIALIZANDO EP2*/
	SYNCDELAY;
	EP2BCL = 0x80;				// Commit buffer, 1x - w/skip=1
	SYNCDELAY;
	EP2BCL = 0x80;				// Commit buffer, 2x - w/skip=1
	SYNCDELAY;
	/*CONFIGURACION DE LOS PUERTOS*/
	PORTACFG = 0x00;			// PORTA as I/O pins
	OEA = 0xFF;					// Set PORTA as outputs
	IOA |= 0x01;     			// pull PA0 high initially   Reset al controlador  
	IOA |= 0x04;     			// pull PA2 high initially   INT (pide ser atendido por el uC)
	IOA |= 0x40;     			// pull PA6 high initially   Indicado de que uC tiene que escribir  (IN)               
	IOA |= 0x80;     			// pull PA7 high initially   Indicado de que uC tiene que leer      (OUT)  
}


void TD_Poll( void )
{ 

	///////////////////////////////////////////////////////////////////
	////////////////////////////////// OUT ////////////////////////////
	///////////////////////////////////////////////////////////////////
	static WORD xFIFOTC_OUT = 0x0000;
	static WORD xFIFOTC_IN = 0x0000;


    if( !( EP2468STAT & 0x01 ) )					// If the host busy sending data, EP2E=0, !NO esta ENPY! 
    {                            					
		if( GPIFTRIG & 0x80 )      				    // Gpif Desocupado
        {  
            xFIFOTC_OUT = ((EP2BCH<<8) + EP2BCL);   // Copio la cantidad de datos enviados por el PC
			if(!xFIFOTC_OUT)					    // Handle host sending zero length pkt.             
				EP2BCL = 0x80;                		// AUTOOUT=0, so "skip" zerolen pkt.		        
			else //Si hay datos
		    {   
                if(!Mod_TCIN)                       // Si no esta activado "Quiero modificar el tc in"
                {
					USB_FLAG_WRITE=1;                        // Esto no es una tranferencia de escritura hacia el USB (IN)	  
					USB_FLAG_READ=0; 						// bring PA7 low 0111 1111  indica que el ATMEGA tiene que leer datos 
					if(INT_FLAGout)				    // Solo se puede interupir al principio de las transferencias
					{		
						INT_USB=0;     				// bring PA2 low 1111 1011 interunpo indicando inicio de transf.
						EZUSB_Delay (1); 			// ~1ms
						INT_USB=1;     				// pull PA2 high initially 
						INT_FLAGout=0;
			        }
					xFIFOTC_OUT=xFIFOTC_OUT>>1; 	// Corrro uno para divivir por dos
					EP2GPIFTCH = MSB(xFIFOTC_OUT);  // Setup transaction count
					EP2GPIFTCL = LSB(xFIFOTC_OUT);
					EP2BCL = 0x00;                  // AUTOOUT=0, so "pass-on" pkt. to master (GPIF)
					GPIFTRIG = GPIF_EP2;  		    // Trigger FIFO write transaction(s), using SFR
                }
				else //Significa que se quiere modificar TC, solo se utiliza para IN
				{
					SYNCDELAY;
					EP8GPIFTCH =EP2FIFOBUF[1];  	// setup transaction count
					SYNCDELAY;
					EP8GPIFTCL =EP2FIFOBUF[0];  	// set EP8GPIFTC = datos que le mando por el PC
					SYNCDELAY;
					FIFORESET=0x02;                 // RESET FIFO EP2
					SYNCDELAY;
					EP2BCL = 0x80;                  // AUTOOUT=0, Skip = 1 (Desecho el dato)
					SYNCDELAY;
					EP2BCL = 0x80;                  // AUTOOUT=0, Skip = 1 EP2 is double buffer
					Mod_TCIN=0;         
                }
            }
		}//end  if( GPIFTRIG & 0x80 )        
	}//end del  ! EP2468STAT & 0x01 
	//////////////////////////////////////////////////////////////////
	////////////////////////////////// IN ////////////////////////////
	//////////////////////////////////////////////////////////////////
	if( in_token_event )        			         // Esta activada el lectura (El PC quiere leer)
    {
		xFIFOTC_IN =  ((EP8GPIFTCH<<8) + EP8GPIFTCL);// Miro datos transmitidos hasta el momento
		SYNCDELAY;
		IN_BC=((EP8FIFOBCH<<8) + EP8FIFOBCL);        // Miro los bytes que llegaron al PC
		SYNCDELAY;
		if(!xFIFOTC_IN)  			 			     // El Gpif ha terminado la transferencia 
		{
			if(enum_high_speed)
			{
				if(IN_BC<0x200)						 // En la FIfo hay menos de 512 bytes
					INPKTEND=0x08;					 // Manualmente los mando hacia el PC
			}
			else
			{
				if(IN_BC<0x40)					     // En la FIfo hay menos de 64 bytes
					INPKTEND=0x08;					 // Manualmente los mando hacia el PC
			}
			in_token_event=0;
			INT_FLAGin=1;
			
		}
		else						                 // Hay datos por transmitir
		{
			if( GPIFTRIG & 0x80 )                    // GPIF disponible
			{
				if(!(EP68FIFOFLGS & 0x10))           // EP8FF=0, esta el FIFO disponible     
				{
					USB_FLAG_READ=1;                       	 // Esto no es una transferencia de lectura del USB (OUT)
					USB_FLAG_WRITE=0;      			     	 // bring PA6 low 1011 1111  indica que el USB quiere datos
					if(INT_FLAGin)					 // Solo se puede interupir al principio de las transferencias
					{		
						INT_USB=0;     				 // bring PA2 low 1111 1011 interumpo indicando inicio de transf.
						EZUSB_Delay (1); 			 //~1ms
						INT_USB=1;     				 // pull PA2 high initially 
						INT_FLAGin=0;
					}
                    GPIFTRIG = GPIFTRIGRD | GPIF_EP8;// Trigger FIFO read transaction(s), using SFR
				    SYNCDELAY;
				}
			}
		}
    } // end if( in_token_event ) 

}//end td_poll

BOOL TD_Suspend(void)				// Called before the device goes into suspend mode
{
   return(TRUE);
}

BOOL TD_Resume(void)          		// Called after the device resumes
{
   return(TRUE);
}

//-----------------------------------------------------------------------------
// Device Request hooks
//   The following hooks are called by the end point 0 device request parser.
//-----------------------------------------------------------------------------

BOOL DR_GetDescriptor(void)
{
   return(TRUE);
}

BOOL DR_SetConfiguration(void)   	// Called when a Set Configuration command is received
{

  Configuration = SETUPDAT[ 2 ];
  return( TRUE );					// Handled by user code
}

BOOL DR_GetConfiguration(void)		// Called when a Get Configuration command is received
{
   EP0BUF[0] = Configuration;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);					// Handled by user code
}

BOOL DR_SetInterface(void)			// Called when a Set Interface command is received
{
   AlternateSetting = SETUPDAT[2];
   return(TRUE);					// Handled by user code
}

BOOL DR_GetInterface(void)			// Called when a Set Interface command is received
{
   EP0BUF[0] = AlternateSetting;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);					// Handled by user code
}

BOOL DR_GetStatus(void)
{
   return(TRUE);
}

BOOL DR_ClearFeature(void)
{
   return(TRUE);
}

BOOL DR_SetFeature(void)
{
   return(TRUE);
}




BOOL DR_VendorCmnd( void )
{
	switch( SETUPDAT[ 1 ] )
	{ 
		//// GPIF
		case VX_E0:	//get status of GPIF
		{ 									
			*EP0BUF = GPIFTRIG; 	
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;   				// Acknowledge handshake phase of device request
			break;
		}	
		//GENERAL
		case VX_C0: //set in_token_event
		{ 
			in_token_event = 1;
			*EP0BUF = in_token_event;
			EP0BCH = 0;
			EP0BCL = 1;         			
			EP0CS |= bmHSNAK;  				
			break;
		}
		case VX_C4: //FLUSH EP2
		{  
			SYNCDELAY;
			FIFORESET = 0x80;             // Active Nack para todas las tranferencias pendientes
			SYNCDELAY;                    // see TRM section 15.14
			FIFORESET = 0x02;             // reset, FIFO 2
			SYNCDELAY;                    // 
			FIFORESET = 0x08;             // reset, FIFO 8
			SYNCDELAY;                    // 
			FIFORESET = 0x00;             // Desactiva el Nack
			SYNCDELAY;                    // 
			EP2BCL = 0x80;                 
			SYNCDELAY;
			EP2BCL = 0x80;                 
			SYNCDELAY;
			*EP0BUF =0xC4;
			EP0BCH = 0;
			EP0BCL = 1;         			
			EP0CS |= bmHSNAK;  				
			break;
		}
		case VX_C5: //RESET
		{  
			RESET=0;
			*EP0BUF = 0xC5;
			SYNCDELAY;
			SYNCDELAY;
			EZUSB_Delay (100);
			RESET=1; 		 
			EZUSB_Delay (20);
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  				// Acknowledge handshake phase of device request
		break;
		}
		case VX_C6: //LOW in_token_event y set INT_FLAGin
		{ 
			INT_FLAGin = 1; 
			in_token_event = 0;
			*EP0BUF = 0xC6;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  				// Acknowledge handshake phase of device request		 
			break;
		}
		case VX_C8: //set Mod_TCIN
		{          
			Mod_TCIN=1;
			*EP0BUF =Mod_TCIN; 
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  				// Acknowledge handshake phase of device request		 
			break;
		}
		case VX_C9: //Aborta la transferencia
		{
			GPIFABORT = 0xFF;
			*EP0BUF =GPIFABORT;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
		}
		case VX_CA: //Set INT_FLAGout  
		{
			INT_FLAGout=1;
			*EP0BUF =INT_FLAGout;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
        }
		case VX_CC: //Botton emergency
        {
			/*Estado espacial solo utilizado para hacer hardstop en el PMD*/
			USB_FLAG_WRITE=0;
			USB_FLAG_READ=0;        
			INT_USB=0;     				// bring PA2 low 1111 1011 interunpo indicando inicio de transf.
			EZUSB_Delay (1); 			    //~1ms
			INT_USB=1;     				// pull PA2 high initially 
			*EP0BUF =0xCC;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
        }
		case VX_CD: //EP8GPIFTCH  
		{
			INT_FLAGout=1;
			*EP0BUF =EP8GPIFTCH;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
        }
		case VX_CE: //EP8GPIFTCL  
		{
			INT_FLAGout=1;
			*EP0BUF =EP8GPIFTCL;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
        }
		case VX_D0: //EP8AUTOINLENL  
		{
			*EP0BUF =EP8AUTOINLENL;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
        }
		case VX_D1: //EP8AUTOINLENH  
		{
			*EP0BUF =EP8AUTOINLENH;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  
			break;
        }
        default:
			GPIFABORT = 0xFF;
			*EP0BUF =Mod_TCIN;
			EP0BCH = 0;
			EP0BCL = 1;         				// Arm endpoint with # bytes to transfer
			EP0CS |= bmHSNAK;  				// Acknowledge handshake phase of device request
	}  
	return( FALSE );        				// no error; command handled OK
}

//-----------------------------------------------------------------------------
// USB Interrupt Handlers
//   The following functions are called by the USB interrupt jump table.
//-----------------------------------------------------------------------------

// Setup Data Available Interrupt Handler
void ISR_Sudav(void) interrupt 0
{
   GotSUD = TRUE;            // Set flag
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUDAV;         // Clear SUDAV IRQ
}

// Setup Token Interrupt Handler
void ISR_Sutok(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUTOK;         // Clear SUTOK IRQ
}

void ISR_Sof(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSOF;            // Clear SOF IRQ
}

void ISR_Ures(void) interrupt 0
{
   if (EZUSB_HIGHSPEED())
   {
      pConfigDscr = pHighSpeedConfigDscr;
      pOtherConfigDscr = pFullSpeedConfigDscr;
   }
   else
   {
      pConfigDscr = pFullSpeedConfigDscr;
      pOtherConfigDscr = pHighSpeedConfigDscr;
   }
   
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmURES;         // Clear URES IRQ
}

void ISR_Susp(void) interrupt 0
{
   Sleep = TRUE;
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUSP;
}

void ISR_Highspeed(void) interrupt 0
{
   if (EZUSB_HIGHSPEED())
   {
      pConfigDscr = pHighSpeedConfigDscr;
      pOtherConfigDscr = pFullSpeedConfigDscr;
   }
   else
   {
      pConfigDscr = pFullSpeedConfigDscr;
      pOtherConfigDscr = pHighSpeedConfigDscr;
   }

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmHSGRANT;
}
void ISR_Ep0ack(void) interrupt 0
{
}
void ISR_Stub(void) interrupt 0
{
}
void ISR_Ep0in(void) interrupt 0
{
}
void ISR_Ep0out(void) interrupt 0
{
}
void ISR_Ep1in(void) interrupt 0
{
}
void ISR_Ep1out(void) interrupt 0
{
}
void ISR_Ep2inout(void) interrupt 0
{
}
void ISR_Ep4inout(void) interrupt 0
{
}
void ISR_Ep6inout(void) interrupt 0
{
}
void ISR_Ep8inout(void) interrupt 0
{
}
void ISR_Ibn(void) interrupt 0
{
}
void ISR_Ep0pingnak(void) interrupt 0
{
}
void ISR_Ep1pingnak(void) interrupt 0
{
}
void ISR_Ep2pingnak(void) interrupt 0
{
}
void ISR_Ep4pingnak(void) interrupt 0
{
}
void ISR_Ep6pingnak(void) interrupt 0
{
}
void ISR_Ep8pingnak(void) interrupt 0
{
}
void ISR_Errorlimit(void) interrupt 0
{
}
void ISR_Ep2piderror(void) interrupt 0
{
}
void ISR_Ep4piderror(void) interrupt 0
{
}
void ISR_Ep6piderror(void) interrupt 0
{
}
void ISR_Ep8piderror(void) interrupt 0
{
}
void ISR_Ep2pflag(void) interrupt 0
{
}
void ISR_Ep4pflag(void) interrupt 0
{
}
void ISR_Ep6pflag(void) interrupt 0
{
}
void ISR_Ep8pflag(void) interrupt 0
{
}
void ISR_Ep2eflag(void) interrupt 0
{
}
void ISR_Ep4eflag(void) interrupt 0
{
}
void ISR_Ep6eflag(void) interrupt 0
{
}
void ISR_Ep8eflag(void) interrupt 0
{
}
void ISR_Ep2fflag(void) interrupt 0
{
}
void ISR_Ep4fflag(void) interrupt 0
{
}
void ISR_Ep6fflag(void) interrupt 0
{
}
void ISR_Ep8fflag(void) interrupt 0
{
}
void ISR_GpifComplete(void) interrupt 0
{
}
void ISR_GpifWaveform(void) interrupt 0
{
}
