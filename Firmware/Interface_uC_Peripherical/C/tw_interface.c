
#include <avr/io.h>
#include <stdint.h>
#include <util/twi.h>		
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "tw_interface.h"

	
	/** \ingroup Protocolo_I2C
	\brief Permite mandar el ADDH además del ADDL*/
	#define TWI_ADDRH

	/** \ingroup Protocolo_I2C
	\brief Maximal number of iterations to wait for a device to respond for a
	selection.  Should be large enough to allow for a pending write to
	complete, but low enough to properly abort an infinite loop in case
	a slave is broken or not present at all.  With 100 kHz TWI clock,
	transfering the start condition and SLA+R/W packet takes about 10
	µs.  The longest write period is supposed to not exceed ~ 10 ms.
	Thus, normal operation should not require more than 100 iterations
	to get the device to respond to a selection.*/
	#define MAX_ITER	400

	/** \ingroup Protocolo_I2C
	\brief Saved TWI status register, for error messages only.  We need to
	save it in a variable, since the datasheet only guarantees the TWSR
	register to have valid contents while the TWINT bit in TWCR is set.*/
	uint8 twst;

/*****************************************************************************************************/
void TWI_init(void)
{

  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
#if defined(TWPS0)
  /* has prescaler (mega128 & newer) */
  TWSR = 0;
#endif

#if F_CPU < 3600000UL
  TWBR = 10;			/* smallest TWBR value*/
#else
  TWBR = (F_CPU / 400000UL - 16) / 2;
#endif
}

/*****************************************************************************************************/
int16 read_bytes_I2C(uint8 sla, uint32 eeaddr, int16 len, uint8 *buf)
{
	uint8 twcr, n = 0;
	int16 rv = 0; 
 
	/* 1. First cycle: master transmitter mode*/
 
	restart:
		if (n++ >= MAX_ITER)
			return -1;
	begin:
		
		/* send start condition TWCR = Registro que controla el TWI*/
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); 
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		switch ((twst = TW_STATUS))
		{
			case TW_REP_START:							/* OK, but should not happen */
			case TW_START:
			break;
			
			case TW_MT_ARB_LOST:						
				goto begin;
					
			default:
				return -1;								/* error: not in start condition */
														/* NB: do /not/ send stop condition */
		}	
		
		
		/**< send SLA+W */
		TWDR = sla | TW_WRITE; 							/* LO coloca para lectura*/
		TWCR = _BV(TWINT) | _BV(TWEN); 				/* clear interrupt to start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		switch ((twst = TW_STATUS))
		{
			case TW_MT_SLA_ACK:
			break;
			
			case TW_MT_SLA_NACK:						/* nack during select: device busy writing ACK POLLING*/				
				goto restart;	
				
			case TW_MT_ARB_LOST:						/* re-arbitrate */
				goto begin;
				
			default:
				goto error;								/* must send stop condition */
		}
		
		
		#ifdef TWI_ADDRH
			TWDR =(uint8)((eeaddr>>8)&0xFF);			/* high 8 bits of addr if define TWI_ADDRH*/
			TWCR = _BV(TWINT) | _BV(TWEN); 			/* clear interrupt to start transmission */
			while ((TWCR & _BV(TWINT)) == 0) ; 		/* wait for transmission */
			switch ((twst = TW_STATUS))
			{
				case TW_MT_DATA_ACK:
				break;
					
				case TW_MT_DATA_NACK:
					goto quit;         					/* En espera */
					
				case TW_MT_ARB_LOST:
					goto begin;
					
				default:
					goto error;							/* must send stop condition */
			}			
		#endif
		
		
     
		TWDR = (uint8)(eeaddr&0xFF);      				/* low 8 bits of addr */
		TWCR = _BV(TWINT) | _BV(TWEN);     			/* clear interrupt to start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		switch ((twst = TW_STATUS))
		{
			case TW_MT_DATA_ACK:
			break;
				
			case TW_MT_DATA_NACK:
				goto quit;
				
			case TW_MT_ARB_LOST:
				goto begin;
				
			default:
				goto error;								/* must send stop condition */
		}
		
		
		/* 2. Next cycle(s): master receiver mode*/
		
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); 	/* send (rep.) start condition */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		switch ((twst = TW_STATUS))
		{
			case TW_START:								/* OK, but should not happen */
			case TW_REP_START:
			break;
				
			case TW_MT_ARB_LOST:
				goto begin;
				
			default:
				goto error;
		}
		
		/* send SLA+R */
		TWDR = sla | TW_READ;
		TWCR = _BV(TWINT) | _BV(TWEN); 				/* clear interrupt to start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		switch ((twst = TW_STATUS))
		{
			case TW_MR_SLA_ACK:
			break;
				
			case TW_MR_SLA_NACK:
				goto quit;
				
			case TW_MR_ARB_LOST:
				goto begin;
				
			default:
				goto error;
		}		
		
		for (twcr = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);len > 0;len--)
		{
			if (len == 1)
				twcr = _BV(TWINT) | _BV(TWEN); 		/* send NAK this time */
			
			TWCR = twcr;								/* clear int to start transmission */
			while ((TWCR & _BV(TWINT)) == 0) ; 		/* wait for transmission */
			switch ((twst = TW_STATUS))
			{
				case TW_MR_DATA_NACK:
					len = 0;							/* force end of loop */
				/* FALLTHROUGH */
				case TW_MR_DATA_ACK:
					*buf++ = TWDR;
					rv++;
				break;
					
				default:
					goto error;
			}
		}
		
		
	quit:
		
		TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
		
		return rv;
		
	error:
		rv = -1;
		goto quit;
}

/*****************************************************************************************************/
int16 write_page_I2C(uint8 PAGE_SIZE,uint8 sla, uint32 eeaddr, int16 len, uint8 *buf)
{
	uint8  n = 0;
	int16 rv = 0;
	uint16 endaddr;

	/* si la direccion mas la cantidad de datos en menor a  el tamaño de pagina*/
	if (eeaddr + len <  (eeaddr | (PAGE_SIZE - 1)) )  
		endaddr = eeaddr + len;
	else
		/* Entonces colocar el final hasta el fin de pagina*/
		endaddr = (eeaddr | (PAGE_SIZE - 1)) + 1;  
	
	len = endaddr - eeaddr;


	restart:
		if (n++ >= MAX_ITER)
			return -1;
	begin:
		
		
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); 	/* send start condition */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		twst = TW_STATUS;
		
		switch (twst)
		{
			case TW_REP_START:							/* OK, but should not happen */
			case TW_START:
			break;
				
			case TW_MT_ARB_LOST:
				goto begin;
				
			default:
				return -1;								/* error: not in start condition */
														/* NB: do /not/ send stop condition */
		}
		
		/* send SLA+W */
		TWDR = sla | TW_WRITE;
		TWCR = _BV(TWINT) | _BV(TWEN); 				/* clear interrupt to start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		twst = TW_STATUS;
		
		switch (twst)
		{
			case TW_MT_SLA_ACK:
			break;
				
			case TW_MT_SLA_NACK:						/* nack during select: device busy writing */
				goto restart;
				
			case TW_MT_ARB_LOST:						/* re-arbitrate */
			goto begin;
				
			default:
				goto error;								/* must send stop condition */
		}	
			
		#ifdef TWI_ADDRH
			TWDR =(uint8)((eeaddr>>8)&0xFF);			/* high 8 bits of addr if define TWI_ADDRH*/
			TWCR = _BV(TWINT) | _BV(TWEN); 			/* clear interrupt to start transmission */
			while ((TWCR & _BV(TWINT)) == 0) ; 		/* wait for transmission */
			twst = TW_STATUS;
			
			switch (twst)
			{
				case TW_MT_DATA_ACK:
				break;
					
				case TW_MT_DATA_NACK:
					goto quit;
					
				case TW_MT_ARB_LOST:
					goto begin;
					
				default:
					goto error;							/* must send stop condition */
			}
		#endif
		
		TWDR = (uint8)(eeaddr&0xFF);					/* low 8 bits of addr */
		TWCR = _BV(TWINT) | _BV(TWEN); 				/* clear interrupt to start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */
		twst = TW_STATUS;
		
		switch (twst)
		{
			case TW_MT_DATA_ACK:
			break;
				
			case TW_MT_DATA_NACK:
				goto quit;
				
			case TW_MT_ARB_LOST:
				goto begin;
				
			default:
				goto error;								/* must send stop condition */
		}
		
		for (; len > 0; len--)
		{
			TWDR = *buf++;
			TWCR = _BV(TWINT) | _BV(TWEN); 			/* start transmission */
			while ((TWCR & _BV(TWINT)) == 0) ; 		/* wait for transmission */
			switch ((twst = TW_STATUS))
			{
				case TW_MT_DATA_NACK:
					goto error;							/* device write protected -- Note [16] */
					
				case TW_MT_DATA_ACK:
					rv++;
				break;
					
				default:
					goto error;
			}
		}
	quit:
		TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); 	/* send stop condition */
			return rv;
		
	error:
		rv = -1;
		goto quit;
}
/*****************************************************************************************************/
int16 write_bytes_I2C(uint8 PAGE_SIZE,uint8 sla, uint32 eeaddr, int16 len, uint8 *buf)
{
	int16 rv, total;

	total = 0;
  
	do
    {
		
		rv = write_page_I2C(PAGE_SIZE,sla, eeaddr, len, buf);
		if (rv == -1)
		{
			error();
			return -1;
		}
	  
		eeaddr += rv;
		len -= rv;
		buf += rv;
		total += rv;
    }
  while (len > 0);

  return total;
}
/*****************************************************************************************************/
void error(void)
{

  printf("error=> TWI status %#x\n", twst);
  exit(0);
}
