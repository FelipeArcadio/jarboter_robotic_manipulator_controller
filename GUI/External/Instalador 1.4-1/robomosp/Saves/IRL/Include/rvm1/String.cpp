/********************************************************************************/
/*				CLASE	STRING					*/
/********************************************************************************/
/*										*/
/*	Clase:		String							*/
/*	Funcion:	Clase de uso de Cadenas de Caracteres			*/
/*	Atributos:								*/
/*		char 	cadena		cadena de caracteres			*/
/*	File:		string.cpp						*/
/*	Desarrollador:	Olver Eduardo Ramos Lucumi				*/
/*	email:		oeramos@puj.edu.co					*/
/*	fecha:		11 de Septiembre de 2002				*/
/*										*/
/********************************************************************************/

#ifndef STRING_CPP
#define STRING_CPP

#include "String.h"

/********************************************************************************/
/*		Método:			String					*/
/*		Descripcion:		Inicializadora de la Clase		*/
/*		Parámetros:		- o -					*/
/*		Retorna:		- o -					*/
/********************************************************************************/

String :: String ( )
{
	S[0] = '\0';
}

/*String :: String ( String _s )
{
        strcpy ( S, _s.S );
	}*/

/********************************************************************************/
/*		Método:			String					*/
/*		Descripcion:		Copia la cadena de entrada y lo asigna	*/
/*					al objeto.				*/
/*		Parámetros:		Array char _entrada			*/
/*		Retorna:		- o -					*/
/********************************************************************************/

String :: String ( char _entrada [ ] )
{
	strcpy ( S, _entrada );
}

String :: String ( char _entrada )
{
        S[0] = _entrada;
        S[1] = '\0';
}

/********************************************************************************/
/*		Método:			String					*/
/*		Descripcion:		Copia la cadena de entrada y lo asigna	*/
/*					al objeto.				*/
/*		Parámetros:		Pointer char _entrada			*/
/*		Retorna:		- o -					*/
/********************************************************************************/

String :: String ( const char * _entrada )
{
	strcpy ( S, _entrada );
}

/********************************************************************************/
/*		Método:			Copiar					*/
/*		Descripcion:		Copia la cadena de entrada y lo asigna	*/
/*					al objeto.				*/
/*		Parámetros:		String _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

void String :: Copiar ( String _entrada )
{
	strcpy ( S, _entrada.S );
}

/********************************************************************************/
/*		Método:			Copiar					*/
/*		Descripcion:		Copia la cadena de entrada y lo asigna	*/
/*					al objeto.				*/
/*		Parámetros:		Array char _entrada			*/
/*		Retorna:		- o -					*/
/********************************************************************************/

void String :: Copiar ( char _entrada [ ] )
{
	strcpy ( S, _entrada );
}

/********************************************************************************/
/*		Método:			Copiar					*/
/*		Descripcion:		Copia la cadena de entrada y lo asigna	*/
/*					al objeto.				*/
/*		Parámetros:		Pointer char _entrada			*/
/*		Retorna:		- o -					*/
/********************************************************************************/

void String :: Copiar ( const char * _entrada )
{
	strcpy ( S, _entrada );
}


/********************************************************************************/
/*		Método:			Longitud				*/
/*		Descripcion:		Calcula la longitud de la cadena	*/
/*		Parámetros:		- o -					*/
/*		Retorna:		int -> Longitud				*/
/********************************************************************************/

int String :: Longitud (  )
{
	return strlen ( S );
}

/********************************************************************************/
/*		Método:			Asignar ( = )				*/
/*		Descripcion:		Copia la cadena de entrada y lo asigna	*/
/*					al objeto.				*/
/*		Parámetros:		String _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

void String :: operator = ( String _entrada )
{
	strcpy ( S, _entrada.S );
}

/********************************************************************************/
/*		Método:			Concatenar ( + )			*/
/*		Descripcion:		Pega la cadena con la cadena de entrada */
/*		Parámetros:		String _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

String String :: operator + ( String _entrada )
{
	char texto[256];
	strcpy (texto, S);
	String nuevo (strcat ( texto, _entrada.S ));
	return nuevo;
}

/********************************************************************************/
/*		Método:			Concatenar ( + )			*/
/*		Descripcion:		Pega la cadena con la cadena de entrada */
/*		Parámetros:		char * _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

String String :: operator + ( const char * _entrada )
{
	char texto[256];
	strcpy (texto, S);
	String nuevo (strcat ( texto, _entrada ));
	return nuevo;
}

/********************************************************************************/
/*		Método:			Concatenar ( += )			*/
/*		Descripcion:		Pega la cadena con la cadena de entrada */
/*		Parámetros:		String _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

void String :: operator += ( String _entrada )
{
	strcat ( S, _entrada.S );
}

/********************************************************************************/
/*		Método:			Concatenar ( += )			*/
/*		Descripcion:		Pega la cadena con la cadena de entrada */
/*		Parámetros:		char * _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

void String :: operator += ( const char * _entrada )
{
	strcat ( S, _entrada );
}


/********************************************************************************/
/*		Método:			Comparar Igualdad ( == )		*/
/*		Descripcion:		Compara dos String e indica si son 	*/
/*					iguales o no.				*/
/*		Parámetros:		String _entrada				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

bool String :: operator == ( String _entrada )
{
	if (strcmp ( S, _entrada.S ) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/********************************************************************************/
/*		Método:			ToStringDate				*/
/*		Descripcion:		Genera un String a partir de un time_t	*/
/*		Parámetros:		time_t _longtime			*/
/*		Retorna:		String					*/
/********************************************************************************/

String String :: ToStringDate (time_t longtime)
{
	char Temp[50];

	struct tm * DT = localtime (&longtime);

	sprintf (Temp, "%d-%d-%d-%d-%d-%d", DT->tm_mday,DT->tm_mon,DT->tm_year+1900,
					   DT->tm_hour,DT->tm_min,DT->tm_sec);
	return String(Temp);
}

/********************************************************************************/
/*		Método:			ToString				*/
/*		Descripcion:		Genera un String a partir de un int	*/
/*		Parámetros:		int val					*/
/*		Retorna:		String					*/
/********************************************************************************/

String String :: ToString (int val)
{
	char  cadenareal[50];
	sprintf(cadenareal,"%d",val);
	String s ( cadenareal );
	return s;
}


/********************************************************************************/
/*		Método:			ToString				*/
/*		Descripcion:		Genera un String a partir de un int	*/
/*		Parámetros:		long val					*/
/*		Retorna:		String					*/
/********************************************************************************/

String String :: ToString (long val)
{

	char  cadenareal[50];
	sprintf(cadenareal,"%ld",val);
	String s ( cadenareal );
	return s;
}

/********************************************************************************/
/*		Método:			ToString				*/
/*		Descripcion:		Genera un String a partir de un double	*/
/*		Parámetros:		double val				*/
/*		Retorna:		String					*/
/********************************************************************************/

String String :: ToString (double val)
{
	char * Temp = new char[50];
	sprintf(Temp,"%f",val);
	return String(Temp);
}

/********************************************************************************/
/*		Método:			ToString				*/
/*		Descripcion:		Genera un String a partir de un bool	*/
/*		Parámetros:		bool val				*/
/*		Retorna:		String					*/
/********************************************************************************/

String String :: ToString (bool val)
{
	if ( !val )
		return String ("false");
	return String("true");
}


/********************************************************************************/
/*		Método:			ToInt					*/
/*		Descripcion:		Genera un int a partir de un String	*/
/*		Parámetros:		String _entrada				*/
/*		Retorna:		int					*/
/********************************************************************************/

int String :: ToInt (String _entrada)
{
	return atoi ( _entrada.S );
}

/********************************************************************************/
/*		Método:			ToDouble				*/
/*		Descripcion:		Genera un double a partir de un String	*/
/*		Parámetros:		String _entrada				*/
/*		Retorna:		double					*/
/********************************************************************************/

double String :: ToDouble (String _entrada)
{
	return atof ( _entrada.S );
}

/********************************************************************************/
/*		Método:			ToDate					*/
/*		Descripcion:		Genera una fecha a partir de un String	*/
/*		Parámetros:		int _dia				*/
/*					int _mes				*/
/*					int _ano				*/
/*		Retorna:		time_t					*/
/********************************************************************************/

time_t String :: ToDate (int _dia, int _mes, int _ano)
{
	tm * tiempo = new tm;
	tiempo->tm_sec = 0;
	tiempo->tm_min = 0;
	tiempo->tm_hour = 0;
	tiempo->tm_mday = _dia;
	tiempo->tm_year = _ano - 1900;
	tiempo->tm_mon = _mes;
	return mktime ( tiempo );
}


/********************************************************************************/
/*		Método:			ToDate					*/
/*		Descripcion:		Genera una fecha a partir de un String	*/
/*		Parámetros:		String _entrada				*/
/*		Retorna:		time_t					*/
/********************************************************************************/

time_t String :: ToDate (String _entrada)
{
	Lista <String> * cadenas = _entrada.Split ( '-' );

	cadenas->Primero ();
	
	tm * tiempo = new tm;
	tiempo->tm_mday = String::ToInt(cadenas->Info());
	cadenas->Siguiente();

	tiempo->tm_mon = String::ToInt(cadenas->Info());
	cadenas->Siguiente();

	tiempo->tm_year = String::ToInt(cadenas->Info());
	cadenas->Siguiente();
	
	tiempo->tm_hour = String::ToInt(cadenas->Info());
	cadenas->Siguiente();
	tiempo->tm_min = String::ToInt(cadenas->Info());
	cadenas->Siguiente();
	tiempo->tm_sec = String::ToInt(cadenas->Info());
	cadenas->Siguiente();

	tiempo->tm_year -= 1900;

	return mktime ( tiempo );
}

/********************************************************************************/
/*		Método:			Split					*/
/*		Descripcion:		Separa una cadena de caracteres por el 	*/
/*					caracter separador en sub cadenas	*/
/*		Parámetros:		char separador				*/
/*		Retorna:		Lista de String				*/
/********************************************************************************/

Lista <String> * String :: Split ( const char * separador )
{
	Lista <String> * resp = new Lista <String>;
	char * pCharS, * cadenaresp;

	pCharS = &S[0];

	cadenaresp = strstr ( pCharS, separador );

	while ( cadenaresp != NULL )
	{
		int i;
		String temp;
		for ( i = 0; i < strlen ( pCharS ) - strlen ( cadenaresp ); i ++ )
		{
			temp.S [i] = pCharS [i];
		}
		temp.S [i] = '\0';
		resp->Anexar ( temp );
		pCharS = cadenaresp + strlen ( separador );
		cadenaresp = strstr ( pCharS, separador );
	}
	String temp (pCharS);
	resp->Anexar ( temp );

	return resp;
}

Lista <String> * String :: Split ( String separador )
{
	Lista <String> * resp = new Lista <String>;
	char * pCharS, * cadenaresp;

	pCharS = &S[0];

	cadenaresp = strstr ( pCharS, separador.S );

	while ( cadenaresp != NULL )
	{
		int i;
		String temp;
		for ( i = 0; i < strlen ( pCharS ) - strlen ( cadenaresp ); i ++ )
		{
			temp.S [i] = pCharS [i];
		}
		temp.S [i] = '\0';
		resp->Anexar ( temp );
		pCharS = cadenaresp + strlen ( separador.S );
		cadenaresp = strstr ( pCharS, separador.S );
	}
	String temp (pCharS);
	resp->Anexar ( temp );

	return resp;
}


/********************************************************************************/
/*		Método:			Split					*/
/*		Descripcion:		Separa una cadena de caracteres por el 	*/
/*					caracter separador en sub cadenas	*/
/*		Parámetros:		char separador				*/
/*		Retorna:		Lista de String				*/
/********************************************************************************/

Lista <String> * String :: Split ( char separador )
{
	int i = 0;
	char t = S[i];
	Lista <String> * resp = new Lista<String>;
	while (t != '\0')
	{
		char cadena [256];
		int j = 0;
		while (t != separador && t != '\0' )
		{
			cadena[j] = t;
			j++;
			i++;
			t = S[i];
			if ( i > 256 )
				break;
		}
		if ( i > 256 )
			break;
		cadena[j] = '\0';
		String _cad ( cadena );
		resp -> Ultimo ( );
		resp -> Anexar ( _cad );
		if ( t == separador )
		{
			i++;
			t = S[i];
		}
	}
	return resp;
}



Lista <String> * String :: Split ( )
{
	int i = 0;
	char t = S[i];
	Lista <String> * resp = new Lista<String>;
	while (t != '\0')
	{
		char cadena [256];
		int j = 0;
		while (t != ' ' && '\t' && t != '\0' )
		{
			cadena[j] = t;
			j++;
			i++;
			t = S[i];
			if ( i > 256 )
				break;
		}
		if ( i > 256 )
			break;
		cadena[j] = '\0';
		String _cad ( cadena );
		resp -> Ultimo ( );
		resp -> Anexar ( _cad );
		if ( t == ' ' || t == '\t' )
		{
			i++;
			t = S[i];
		}
	}
	return resp;
}

/********************************************************************************/
/*		Método:			Join					*/
/*		Descripcion:		Une una lista de Cadenas en una sola	*/
/*					usando un enlace, si es necesario	*/
/*		Parámetros:		lista String cadenas			*/
/*					String enlace				*/
/*		Retorna:		String 					*/
/********************************************************************************/

String String :: Join ( Lista <String> * source, String enlace )
{
	String resp;
	source->Primero ( );
	while ( !source->Fin ( ) )
	{
		resp += source->Info ( );
		if ( !source->esUltimo ( ) )
		{
			resp += enlace;
		}
		source->Siguiente ( );
	}
	return resp;
}

/********************************************************************************/
/*		Método:			Replace					*/
/*		Descripcion:		Reemplaza en la cadena todas las 	*/
/*					instancias de la cadena find en la 	*/
/*					cadena sorcepor la cadena replace	*/
/*		Parámetros:		String sorce				*/
/*					String find				*/
/*					String replace				*/
/*		Retorna:		String 					*/
/********************************************************************************/

String String :: Replace ( String source, String find, String replace)
{
	if ( find == "" )
		return source;
	Lista <String> * lista = source.Split ( find );
	return Join ( lista, replace );
}

/********************************************************************************/
/*		Método:			Impresion ( << )			*/
/*		Descripcion:		Escribe el String en pantalla		*/
/*		Parámetros:		String a				*/
/*		Retorna:		- o -					*/
/********************************************************************************/

ostream &operator << (ostream &output, String a)
{
	output 	<< a.S;
	return output;
}

String String :: Upper ( String s )
{
	String cadena = s;
	unsigned int i;
	for (i = 0; i < strlen (cadena.S); i++)
		if (cadena.S[i] >= 'a' && cadena.S[i] <= 'z')
			cadena.S[i] = cadena.S[i] & 0xDF;
	return cadena;
}

String String :: Lower ( String s )
{
	String cadena = s;
	unsigned int i;
	for (i = 0; i < strlen (cadena.S); i++)
		if (cadena.S[i] >= 'A' && cadena.S[i] <= 'Z')
			cadena.S[i] = cadena.S[i] | 0x20;
	return cadena;
}

void String :: Upper ( )
{
	unsigned int i;
	for (i = 0; i < strlen (S); i++)
		if (S[i] >= 'a' && S[i] <= 'z')
			S[i] = S[i] & 0xDF;
}

void String :: Lower ( )
{
	unsigned int i;
	for (i = 0; i < strlen (S); i++)
		if (S[i] >= 'A' && S[i] <= 'Z')
			S[i] = S[i] | 0x20;
}

bool String :: operator != ( String s )
{
	return !((*this) == s);
}

#endif



