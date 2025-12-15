#include "macros.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"
#include "lcd.h"
#include "types.h"

/*
 * Setup the LCD controller.  First, call the hardware initialization
 * function, then adjust the display attributes we want.
 */
void
lcd_init(void)
{

  hd44780_init();

  /*
   * Clear the display.
   */
  hd44780_outcmd(HD44780_CLR);
  hd44780_wait_ready();

  /*
   * Entry mode: auto-increment address counter, no display shift in
   * effect.
   */
  hd44780_outcmd(HD44780_ENTMODE(1, 0));
  hd44780_wait_ready();

  /*
   * Enable display, activate non-blinking cursor.
   */
  hd44780_outcmd(HD44780_DISPCTL(1, 1, 0));
  hd44780_wait_ready();
}

/*
 * Send character c to the LCD display.  After a '\n' has been seen,
 * the next character will first clear the display.
 */


int
lcd_putchar(char c, FILE *unused)
{
	static bool nl_seen;
	
	uint8 addr;

	if (nl_seen && c != '\n')
	{
		
		// First character after newline, clear display and home cursor.
		
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_CLR);
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_HOME);
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_DDADDR(0));
		
		nl_seen = false;
	}
	if (c == '\n')
    {
		nl_seen = true;
    }
	else
    {
		
		hd44780_wait_ready();
		/* Leo el Address Counter */
		addr = hd44780_incmd();
		/* Obtengo solo el Address counter */
		addr = addr & (0x7F);
		
		/* Comparo si el address counter llego a la ultima columna de la primera linea del display*/
		if (addr == 0x0F)
		{
			hd44780_wait_ready();
			/* Envio el address counter a la primera columna de la segunda linea del display*/
			hd44780_outcmd(HD44780_DDADDR(0x40));
			
		}
		
		/* Comparo si el address counter llego a la ultima columna de la segunda linea del display*/
		if (addr == 0x4F)
		{
			hd44780_wait_ready();
			/* Limpio el display y posiciono el cursor en la primera columna de la primera linea. */
			hd44780_outcmd(HD44780_CLR);
			
		}
		
		
		hd44780_wait_ready();
		hd44780_outdata(c);
		_delay_ms(100);
	}

	return 0;
}

