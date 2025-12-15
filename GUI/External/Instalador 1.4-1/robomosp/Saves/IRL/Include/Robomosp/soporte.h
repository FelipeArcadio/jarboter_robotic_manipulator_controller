#include <string.h>

int LocateString ( char cadena [ 256 ], char subcadena [ 256 ] )
{
	int i, j;
	int bien = -1;
	i = 0;
	j = 0;
	while ( cadena [ i ] )
	{
		if ( ! subcadena [ j ] )
			return bien;
		if ( cadena [ i ] == subcadena [ j ] )
		{
			if ( bien == -1 )
				bien = i;
			j++;
		}
		else
		{
			j = 0;
			bien = -1;
		}
		i++;
	}
	if ( ! subcadena [ j ] )
		return bien;
	else
		return -1;
}

void DeleteCharacteres ( char cadena [ 256 ], int inicio, int final )
{
	int i = 0;
	
	while ( i + final <= strlen ( cadena ) )
	{
		cadena [ i + inicio ] = cadena [ i + final + 1 ];
		i++;
	}
}

void ExtractString ( char cadena [ 256 ], int inicio, int final )
{
	int i = inicio;
	
	while ( i <= final )
	{
		cadena [ i - inicio ] = cadena [ i ];
		i++;
	}
	cadena [ i - inicio ] = 0;
}

