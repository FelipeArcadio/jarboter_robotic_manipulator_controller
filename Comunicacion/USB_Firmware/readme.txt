This directory contains the 8051 firmware source code
for the Cypress Semiconductor EZ-USB FX2 chip.

The example uses the GPIFTool utility to illustrate 
single/burst data read(s)/write(s).


Make sure you have the latest development tools (version 2.0) 
installed on your machine.  As such, this example is to be run on a 
Rev E silicon.

It is best to have "Device Manager" open to check that the system
enumerates (as FX2) and then renumerates as a sample device.



To run the example.
-------------------

(1) Use the DK schematic to map FX2 pins to logic analyzer pods.
    CTL0
    CTL1
    PORTB (FD[7..0])=> Data bus
    PORTE (0..2) => GSTATE
(2) Start the control panel application.Select "View|Pending Ops" 
    from within EZUSB Control Panel. This provides you with immediate 
    system feedback when transferring data over the usb.
(3) Download the  firmware  gpifburst8a.hex.
(4) Following are the available vendor requests
	VX_A2 0xA2    // testing, GPIF single byte read waveform
	VX_A3 0xA3    // testing, GPIF single byte write waveform
	VX_A4 0xA4    // testing, GPIFABORT 
	VX_A6 0xA6    // turn debug LED[3:0] off...
	VX_A7 0xA7    // setup peripheral for high speed FIFO xfr(s), TC=8
	VX_A8 0xA8    // do a FIFO Rd transaction w/TC=8 into EP8
	VX_A9 0xA9    // do a FIFO Wr transaction w/TC=BC from EP2 
	VX_AA 0xAA    // manually commit IN data to host... EP8BC=FIFOBC
	VX_AB 0xAB    // manually commit OUT data to master... 
	VX_AC 0xAC    // manually commit IN data to host... INPKTEND
	VX_AD 0xAD    // setup GPIF FIFO Reads w/TC=8
	VX_AE 0xAE    // return status of GPIF
	VX_AF 0xAF    // comitt one zerolen IN pkt

	VX_B1 0xB1    // examine flag status, XDATA - EP8FIFOFLGS
	VX_B2 0xB2    // examine flag status, SFR - EP2468STAT
	VX_B3 0xB3    // examine flag status, SFR - EP68FIFOFLGS
	VX_B4 0xB4    // examine ep8bch
	VX_B5 0xB5    // examine ep8bcl
	VX_B6 0xB6    // examine ep8fifobcl
	VX_B7 0xB7    // examine ep8fifobcl

	VX_C1 0xC1    // examine flag status, XDATA - EP2FIFOFLGS
	VX_C2 0xC2    // examine flag status, SFR - EP2468STAT
	VX_C3 0xC3    // examine flag status, SFR - EP24FIFOFLGS
	VX_C4 0xC4    // examine ep2bch
	VX_C5 0xC5    // examine ep2bcl
	VX_C6 0xC6    // examine ep2fifobcl
	VX_C7 0xC7    // examine ep2fifobcl

	VX_D1 0xD1    // let TD_Poll( ); firmware handle data xfr's...
	VX_D2 0xD2    // handle data xfr's via vendor specific cmds...

	VX_E0 0xE0    // set GPIF FIFO Read event flag

     The GPIF burst read and write transfers can be triggered  by issuing 
     vendor commands or  can be handled by the firmware (inTD-Poll routine).  
     
     In order ot use the vendor requests to do the same please follow the 
     sequence below.  On sending each vendor request LED2 will be turned ON (blink)
     for one second.  Make sure the LED2 has blinked to indicate that the vendor 
     request was completed, before issuing another request.
   
     LED1 will be turned ON if the GPIF is already busy.  Make sure that  this 
     is not the case  before triggerring another request.  This is for debugging 
     purposes. 

     If Buffer is not avilable on an IN transfer or data is not available in the 
     buffer on an OUT transfer,  LED3 will be turn On.  This is again for 
     debugging purposes.

     (a) Testing OUT data transfers: (GPIF Burst Write)
	 ----------------------------------------------
         send vend cmnd 0xA6 to turn off ALL LEDs.	
	 trigger LA on CTL1, 
         Using the control panel to a Bulk OUT transfer through Endpoint 2
         of 64 bytes bytes.  (  NOTE: you may also do a file transfer, 
         selecting the "File  Trans" through EP2 OUT and select 64_Count.hex
	 file, which is a file with incrementing numbers from 0 - 3F).
         Issue the following vendoir request in the following sequence. 
		send vend cmnd 0xC2,
		send vend cmnd 0xC4,
		send vend cmnd 0xC5,
	 examine CP for proper endpflags and endpbc,
		send vend cmnd 0xAB, 
		send vend cmnd 0xC1,
		send vend cmnd 0xC6,
		send vend cmnd 0xC7,
	 examine CP for proper fifoflags and fifobc,
		send vend cmnd 0xA9, 
	 check logic analyzer (LA)

     (b) Testing OUT data transfers: (GPIF Single Write)
	 -----------------------------------------------
	 send vend cmnd 0xA6 to turn off ALL LEDs.	
		trigger LA on CTL1, 
		send vend cmnd 0xA3,
	 check logic analyzer (LA)

     (c) Testing IN data transfers: (GPIF Burst Read)
         --------------------------------------------
	 send vend cmnd 0xA6 to turn off ALL LEDs.	
	 trigger LA on CTL0, 
		send vend cmnd 0xA7,
		send vend cmnd 0xA8,
	 check logic analyzer (LA)
		send vend cmnd 0xB1,
		send vend cmnd 0xB6,
		send vend cmnd 0xB7,
	 examine CP for proper fifoflags and fifobc,
		send vend cmnd 0xAC,   
	 Do an IN token for EP8 IN endpoint of 8 bytes,

     (d) Testing IN data transfers: (GPIF Single Read)
	 ----------------------------------------------
	 send vend cmnd 0xA6 to turn off ALL LEDs.	
		trigger LA on CTL0, 
		send vend cmnd 0xA2,
	 check logic analyzer (LA)



(5) GPIF triggered in TD_Poll


In order to let the data transfers be handled  by the firmware Issue a vendor 
command of 0xD1 which sets the xfr_via_TD_Poll flag True.  You should also see 
LED0 turned  on, once in this mode. 


    (a) GPIF Burst Write
        ----------------
        Using the control panel perform  a Bulk OUT  transfer through Endpoint 
        2 of 64 bytes bytes.  (  NOTE: you may also do a file  transfer, 
        selecting the "File  Trans" through EP2 OUT and select 64_Count.hex
        file, which is a file with incrementing numbers from 0 - 3F).
        You will see the data displayed on the logic analyzer (LA).


   (b) GPIF Burst Read (TC=8)
       ----------------------
       Using the control panel perform  a vendor request of 0xE0. 
       You will see the data displayed on the logic analyzer (LA) along 
       with the waveforms.








Developer's note(s):
====================

Note0
=====
This example was developed and tested with teh REV E silicon. 



Note1:
======
GSTATES[2:0] pins - show the current state of the GPIF state machine
-alt. func. PORTE[2:0], set via IFCONFIG.2=1
-typically used during debugging
GPIFADR[8] pin - updates when register written
-alt. func. PORTE.7
GPIFADR[7:0] pins - updates when register written
-alt. func PORTC[7:0]
-can be tri-stated


Note2:
======
Taking a look at dscr.a51 will show that the start of the device
descriptor tables is fixed at CSEG 90H for this example.  This 
was done to minimize the word-alignment issue.  Basically, the 
tables all need to be word-aligned and even.  The first 
indication that the tables may not be properly aligned is that 
the device fails to renumerate and typically comes up as 
"banged out" unknown USB device.  If this happens, "uninstall"
the device (right click on it) and examine the *.m51 file 
for possible table alignment issues.


Note3:
======
When the system is enumerating/renumerating the device under 
a HS host controller (which must be used to do development 
with rev B silicon) it is best to wait until W2K OS has finished
before attempting to do usb transfers to talk to the device.  One
method for detecting this event is to just listen and wait until 
the hard disk spins.


