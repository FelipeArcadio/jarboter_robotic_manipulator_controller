/**	\ingroup firmware_usb
	\brief En este archivo se encuentran implemantadas las dos máquinas de estado (Lectura y Escritura)
	del módulo GPIF. Este módulo interáctua con el microcontrolador ATMega1280 para realizar transferencia
	de datos.
*/


// This program configures the General Programmable Interface (GPIF) for FX2.              
// Parts of this program are automatically generated using the GPIFTool.                  
// Please do not modify sections of text which are marked as "DO NOT EDIT ...".            
// You can modify the comments section of this GPIF program file using the dropdown menus  
// and pop-up dialogs. These controls are available as hot spots in the text. Modifying the
// comments section will generate program code which will implement your GPIF program.     
//                                                                                         
// DO NOT EDIT ...                                                                         
// GPIF Initialization                                                                     
// Interface Timing      Async                                                             
// Internal Ready Init   IntRdy=0                                                          
// CTL Out Tristate-able Binary                                                            
// SingleWrite WF Select     0                                                             
// SingleRead WF Select      1                                                             
// FifoWrite WF Select       2                                                             
// FifoRead WF Select        3                                                             
// Data Bus Idle Drive   Tristate                                                          
// END DO NOT EDIT                                                                         
                                                                                           
// DO NOT EDIT ...                                                                         
// GPIF Wave Names                                                                         
// Wave 0   = Single_W                                                                     
// Wave 1   = Single_R                                                                     
// Wave 2   = Burst_W                                                                      
// Wave 3   = Burst_R                                                                      
                                                                                           
// GPIF Ctrl Outputs   Level                                                               
// CTL 0    = USB_RDY  CMOS                                                                
// CTL 1    = UNUSED1  CMOS                                                                
// CTL 2    = UNUSED2  CMOS                                                                
// CTL 3    = UNUSED3  CMOS                                                                
// CTL 4    = UNUSED4  CMOS                                                                
// CTL 5    = UNUSED5  CMOS                                                                
                                                                                           
// GPIF Rdy Inputs                                                                         
// RDY0     = USB_READ                                                                     
// RDY1     = USB_WRTE                                                                     
// RDY2     = USB_EN                                                                       
// RDY3     = Unuded3                                                                      
// RDY4     = Unused4                                                                      
// RDY5     = Unused5                                                                      
// FIFOFlag = FIFOFlag                                                                     
// IntReady = IntReady                                                                     
// END DO NOT EDIT                                                                         
// DO NOT EDIT ...                                                                         
//                                                                                         
// GPIF Waveform 0: Single_W                                                               
//                                                                                         
// Interval     0         1         2         3         4         5         6     Idle (7) 
//          _________ _________ _________ _________ _________ _________ _________ _________
//                                                                                         
// AddrMode Same Val  Same Val  Same Val  Inc Val   Same Val  Same Val  Same Val           
// DataMode NO Data   Activate  Activate  Activate  NO Data   NO Data   NO Data            
// NextData SameData  SameData  SameData  SameData  SameData  SameData  SameData           
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    Trig Int           
// IF/Wait  Wait 1    Wait 2    Wait 3    Wait 1    Wait 4    Wait 4    Wait 4             
//   Term A                                                                                
//   LFunc                                                                                 
//   Term B                                                                                
// Branch1                                                                                 
// Branch0                                                                                 
// Re-Exec                                                                                 
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default            
// USB_RDY      1         1         1         1         1         1         1         1    
// UNUSED1      1         0         0         1         1         1         1         1    
// UNUSED2      1         1         1         1         1         1         1         0    
// UNUSED3      1         1         1         1         1         1         1         0    
// UNUSED4      1         1         1         1         1         1         1         0    
// UNUSED5      1         1         1         1         1         1         1         0    
//                                                                                         
// END DO NOT EDIT                                                                         
// DO NOT EDIT ...                                                                         
//                                                                                         
// GPIF Waveform 1: Single_R                                                               
//                                                                                         
// Interval     0         1         2         3         4         5         6     Idle (7) 
//          _________ _________ _________ _________ _________ _________ _________ _________
//                                                                                         
// AddrMode Inc Val   Same Val  Same Val  Same Val  Same Val  Same Val  Same Val           
// DataMode NO Data   Activate  NO Data   NO Data   NO Data   NO Data   NO Data            
// NextData SameData  SameData  SameData  SameData  SameData  SameData  SameData           
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    Trig Int           
// IF/Wait  Wait 1    Wait 2    Wait 3    Wait 1    Wait 4    Wait 4    Wait 4             
//   Term A                                                                                
//   LFunc                                                                                 
//   Term B                                                                                
// Branch1                                                                                 
// Branch0                                                                                 
// Re-Exec                                                                                 
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default            
// USB_RDY      1         0         0         1         1         1         1         1    
// UNUSED1      1         1         1         1         1         1         1         1    
// UNUSED2      1         1         1         1         1         1         1         0    
// UNUSED3      1         1         1         1         1         1         1         0    
// UNUSED4      1         1         1         1         1         1         1         0    
// UNUSED5      1         1         1         1         1         1         1         0    
//                                                                                         
// END DO NOT EDIT                                                                         
// DO NOT EDIT ...                                                                         
//                                                                                         
// GPIF Waveform 2: Burst_W                                                                
//                                                                                         
// Interval     0         1         2         3         4         5         6     Idle (7) 
//          _________ _________ _________ _________ _________ _________ _________ _________
//                                                                                         
// AddrMode Same Val  Same Val  Same Val  Inc Val   Same Val  Same Val  Same Val           
// DataMode NO Data   Activate  NO Data   NO Data   NO Data   NO Data   NO Data            
// NextData SameData  SameData  SameData  NextData  SameData  SameData  NextData           
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    No Int             
// IF/Wait  Wait 255  IF        Wait 255  Wait 255  Wait 1    Wait 1    Wait 255           
//   Term A           USB_READ                                                             
//   LFunc            AND                                                                  
//   Term B           USB_READ                                                             
// Branch1            Then 2                                                               
// Branch0            Else 2                                                               
// Re-Exec            No                                                                   
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default            
// USB_RDY      1         0         1         1         1         1         1         1    
// UNUSED1      1         1         1         1         1         1         1         1    
// UNUSED2      1         1         1         1         1         1         1         0    
// UNUSED3      1         1         1         1         1         1         1         0    
// UNUSED4      1         1         1         1         1         1         1         0    
// UNUSED5      1         1         1         1         1         1         1         0    
//                                                                                         
// END DO NOT EDIT                                                                         
// DO NOT EDIT ...                                                                         
//                                                                                         
// GPIF Waveform 3: Burst_R                                                                
//                                                                                         
// Interval     0         1         2         3         4         5         6     Idle (7) 
//          _________ _________ _________ _________ _________ _________ _________ _________
//                                                                                         
// AddrMode Same Val  Same Val  Same Val  Inc Val   Same Val  Same Val  Same Val           
// DataMode NO Data   NO Data   Activate  NO Data   NO Data   NO Data   NO Data            
// NextData SameData  SameData  SameData  NextData  SameData  SameData  NextData           
// Int Trig No Int    No Int    No Int    No Int    No Int    No Int    No Int             
// IF/Wait  Wait 255  IF        Wait 255  Wait 255  Wait 1    Wait 1    Wait 255           
//   Term A           USB_WRTE                                                             
//   LFunc            AND                                                                  
//   Term B           USB_WRTE                                                             
// Branch1            Then 2                                                               
// Branch0            Else 2                                                               
// Re-Exec            No                                                                   
// Sngl/CRC Default   Default   Default   Default   Default   Default   Default            
// USB_RDY      1         0         1         1         1         1         1         1    
// UNUSED1      1         1         1         1         1         1         1         1    
// UNUSED2      1         1         1         1         1         1         1         0    
// UNUSED3      1         1         1         1         1         1         1         0    
// UNUSED4      1         1         1         1         1         1         1         0    
// UNUSED5      1         1         1         1         1         1         1         0    
//                                                                                         
// END DO NOT EDIT                                                                         
                                                                                           
// GPIF Program Code                                                                       
                                                                                           
// DO NOT EDIT ...                                                                         
#include "fx2.h"                                                                           
#include "fx2regs.h"                                                                       
// END DO NOT EDIT                                                                         
                                                                                           
// DO NOT EDIT ...                                                                         
const char xdata WaveData[128] =                                                           
{                                                                                          
// Wave 0                                                                                  
/* LenBr */ 0x01,     0x02,     0x03,     0x01,     0x04,     0x04,     0x04,     0x07,    
/* Opcode*/ 0x00,     0x02,     0x02,     0x0A,     0x00,     0x00,     0x10,     0x00,    
/* Output*/ 0xFF,     0xFD,     0xFD,     0xFF,     0xFF,     0xFF,     0xFF,     0xC3,    
/* LFun  */ 0x00,     0x09,     0x12,     0x1B,     0x1B,     0x2D,     0x3F,     0x3F,    
// Wave 1                                                                                  
/* LenBr */ 0x01,     0x02,     0x03,     0x01,     0x04,     0x04,     0x04,     0x07,    
/* Opcode*/ 0x08,     0x02,     0x00,     0x00,     0x00,     0x00,     0x10,     0x00,    
/* Output*/ 0xFF,     0xFE,     0xFE,     0xFF,     0xFF,     0xFF,     0xFF,     0xC3,    
/* LFun  */ 0x00,     0x12,     0x1B,     0x1B,     0x1B,     0x2D,     0x3F,     0x3F,    
// Wave 2                                                                                  
/* LenBr */ 0xFF,     0x12,     0xFF,     0xFF,     0x01,     0x01,     0xFF,     0x07,    
/* Opcode*/ 0x00,     0x03,     0x00,     0x0C,     0x00,     0x00,     0x04,     0x00,    
/* Output*/ 0xFF,     0xFE,     0xFF,     0xFF,     0xFF,     0xFF,     0xFF,     0xC3,    
/* LFun  */ 0x12,     0x00,     0x00,     0x12,     0x1B,     0x2D,     0x36,     0x3F,    
// Wave 3                                                                                  
/* LenBr */ 0xFF,     0x12,     0xFF,     0xFF,     0x01,     0x01,     0xFF,     0x07,    
/* Opcode*/ 0x00,     0x01,     0x02,     0x0C,     0x00,     0x00,     0x04,     0x00,    
/* Output*/ 0xFF,     0xFE,     0xFF,     0xFF,     0xFF,     0xFF,     0xFF,     0xC3,    
/* LFun  */ 0x12,     0x09,     0x09,     0x12,     0x00,     0x2D,     0x36,     0x3F     
};                                                                                         
// END DO NOT EDIT                                                                         
                                                                                           
// DO NOT EDIT ...                                                                         
const char xdata InitData[7] =                                                             
{                                                                                          
/* Regs  */ 0x00,0x00,0x00,0xC3,0x06,0x1B,0x11                            
};                                                                                         
// END DO NOT EDIT                                                                         

// TO DO: You may add additional code below.


void GpifInit( void )
{
   BYTE i;

  // 8051 doesn't have access to waveform memories 'til
  // the part is in GPIF mode.
  IFCONFIG = 0xEE;	// IFCLKSRC=1   , GPIF executes on internal clk source
  				    // xMHz=1       , 48MHz internal clk rate
  				    // IFCLKOE=0    , Don't drive IFCLK pin signal at 48MHz
  				    // IFCLKPOL=0   , Don't invert IFCLK pin signal from internal clk
  				    // ASYNC=1      , GPIF samples asynchronous to IFCLK
  				    // GSTATE=1     , Drive GPIF states out on PORTE[2:0], debug WF
  				    // IFCFG[1:0]=10, FX2 in GPIF mode
  
   GPIFABORT = 0xFF;  // abort any waveforms pending

  // To configure EPx FIFOs, please refer to Slave FIFO TRM Chapter
  //FIFORESET = 0xFF;  // reset slave FIFOs

  // TODO: Configure Slave FIFOs here as per your need.

  GPIFREADYCFG = InitData[ 0 ];
  GPIFCTLCFG = InitData[ 1 ];
  GPIFIDLECS = InitData[ 2 ];
  GPIFIDLECTL = InitData[ 3 ];
  GPIFWFSELECT = InitData[ 5 ];
  GPIFREADYSTAT = InitData[ 6 ];


  // TC Expires Not RDY5 Pin feature "bit" polarity swaps from REVB to REVD
  // override GPIFTool generated data. (default = enabled)
  GPIFREADYCFG |= 0x20;				// Don't use TC Expires Not RDY5 Pin feature
  // Uncomment the following to use the TC Expires Not RDY5 Pin feature
  // GPIFREADYCFG &= ~0x20;			// don't use TC Expires Not RDY5 Pin feature
  
   AUTOPTRSETUP = 0x06;          // increment both pointers, 
                                // ....on-chip access via SFR versions
  // Source
  APTR1H = MSB( &WaveData );
  APTR1L = LSB( &WaveData );
  // Destination
  AUTOPTRH2 = 0xE4;
  AUTOPTRL2 = 0x00;
  // Transfer from source to destination
  for ( i = 0x00; i < 128; i++ )
  {
    XAUTODAT2 = XAUTODAT1;
  }

  // Now, dual autopointer to scratch ram...
  // Source
  APTR1H = 0xE4;
  APTR1L = 0x00;
  // Destination
  AUTOPTRH2 = 0xE0;
  AUTOPTRL2 = 0x00;

  // Transfer from source to Destination
  for ( i = 0x00; i < 128; i++ )
  {
    XAUTODAT2 = XAUTODAT1;
  }

  

  // Configure GPIF Address pins, output initial value,
  PORTCCFG = 0xFF;          // [7:0] as alt. func. GPIFADR[7:0]
  OEC = 0x00;               // and as outputs
  PORTECFG |= 0x80;         // [8] as alt. func. GPIFADR[8]
  OEC |= 0x80;              // and as output

  // ...OR... tri-state GPIFADR[8:0] pins
  //  PORTCCFG = 0x00;      // [7:0] as port I/O
  //  OEC = 0x00;           // and as inputs
  //  PORTECFG &= 0x7F;     // [8] as port I/O
  //  OEC &= 0x7F;          // and as input

  // GPIF address pins update when GPIFADRH/L written
  GPIFADRH = 0x00;          // bits[7:1] always 0
  GPIFADRL = 0x00;          // point to PERIPHERAL address 0x0000
  // TODO: change GPIF Address pins to meet your needs

  // Initialize UDMA related registers (default values)
  FLOWSTATE=0x00;           //Defines GPIF flow state
  FLOWHOLDOFF=0x00;
  FLOWLOGIC=0x00;           //Defines flow/hold decision criteria
  FLOWEQ0CTL=00;            //CTL states during active flow state
  FLOWEQ1CTL=0x00;          //CTL states during hold flow state
  FLOWSTB=0x20;             //CTL/RDY Signal to use as master data strobe 
  FLOWSTBEDGE=0x01;         //Defines active master strobe edge
  FLOWSTBHPERIOD=0x02;      //Half Period of output master strobe
  GPIFHOLDAMOUNT=0x00;      //Data delay shift 
  UDMACRCQUAL=0x00;         //UDMA In only, host terminated use only
}