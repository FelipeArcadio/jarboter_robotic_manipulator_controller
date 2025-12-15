#include "mem.h"
#include "types.h"

#ifdef _AVR_IO_H_
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>    		//salida estandar
	#define stepsize 2          // test in byte steps
#else
	#include <memory.h>
#endif


/*****************************************************************************************/
void* declarar(size_t x)
{
  void* p = malloc(x);
  if(p==NULL)
  {
	#ifdef _AVR_IO_H_
		printf("declarar=> ERROR NO HAY MEMORIA\n");
	#endif
  }
  else						    
   memset(p, 0,x);
  
 return p;
}

/*****************************************************************************************/
#ifdef _AVR_IO_H_
uint16 maxavail(void) 
{
	uint8 *ptr;                // pointer to allocated memory
	uint16 testsize;    // what we guess
	
	for(testsize=0; testsize < 65536;testsize++) //64K
	{ 
		ptr = (uint8*)malloc(testsize);
		if(ptr==NULL)  // failed, not enough memory... 
		{
			break;
		}
		else 
		{
		free(ptr);
		}
	}
	return testsize;  // the previous value was ok...
} // maxavail
#endif