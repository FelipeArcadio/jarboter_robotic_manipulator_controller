#include <avr/io.h>
#include <stdint.h>
#include <util/twi.h>		
#include <stdio.h>
#include "types.h"
#include "tw_eeprom.h"

/*****************************************************************************************************/
int16 ee24xx_read_bytes(uint32 eeaddr, int16 len, uint8 *buf)
{
	uint8 sla;
	int16 rv;
	
	/* patch high bits of EEPROM address into SLA 
		 Acomodo a B0*/
	sla = TWI_SLA_24LC1025 | (((eeaddr >> 16) & 0x01) << 3); 
	
	rv=	read_bytes_I2C(sla,eeaddr,len,buf);	
	
	if (rv == -1)
	{
		error();
		return -1;
	}
 
	return rv;
}
/*****************************************************************************************************/
int16 ee24xx_write_bytes(uint32 eeaddr, int16 len, uint8 *buf)	
{
	uint8 sla, PAGE_SIZE;
	int16 rv;
	
	/* patch high bits of EEPROM address into SLA 
		 Acomodo a B0*/
	sla = TWI_SLA_24LC1025 | (((eeaddr >> 16) & 0x01) << 3); 
	
	/* the 24LC1025 EEPROM permits a 128-byte writes in a page  */
	PAGE_SIZE=128;
	
	rv= write_bytes_I2C(PAGE_SIZE,sla, eeaddr, len, buf);
	
	if (rv == -1)
	{
		error();
		return -1;
	}
 
	return rv;	
}