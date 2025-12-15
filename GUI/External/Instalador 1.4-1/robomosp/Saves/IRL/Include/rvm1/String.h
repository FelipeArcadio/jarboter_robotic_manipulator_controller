/********************************************************************************/
/*				CLASE	STRING					*/
/********************************************************************************/
/*										*/
/*	Clase:		String							*/
/*	Funcion:	Clase de uso de Cadenas de Caracteres			*/
/*	Atributos:								*/
/*		char 	cadena		cadena de caracteres			*/
/*	File:		string.h						*/
/*	Desarrollador:	Olver Eduardo Ramos Lucumi				*/
/*	email:		oeramos@puj.edu.co					*/
/*	fecha:		11 de Septiembre de 2002				*/
/*										*/
/********************************************************************************/

#define TAM_STRING 2048
#ifndef STRING_H
#define STRING_H
#include <string.h>
#include <iostream.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "listas.h"

class String
{
	friend ostream &operator << (ostream &, String);
	public:
		char S[TAM_STRING];
		//String ( String _s );
		String ( );
		String ( const char * _entrada );
		String ( char _entrada [ ] );
		String ( char _entrada );
		void Copiar ( String _entrada );
		void Copiar ( char _entrada [ ] );
		void Copiar ( const char * _entrada );
		int Longitud ( );
		void operator = ( String _entrada );
		bool operator == ( String _entrada );
		bool operator != ( String _entrada );
		static String ToStringDate (time_t longtime);
		static time_t ToDate ( String _entrada );
		static time_t ToDate (int _dia, int _mes, int _ano);
		static String ToString (long valor);
		static String ToString (int valor);
		static String ToString (double valor);
		static String ToString (bool valor);
		static String Upper ( String s );
		void Upper ( );
		void Lower ( );
		static String Lower ( String s );
		static String Capitalize ( String s );
		static int ToInt (String _entrada);
		static double ToDouble (String _entrada);
		String operator + ( String _entrada );
		String operator + ( const char * _entrada );
		void operator += ( String _entrada );
		void operator += ( const char * _entrada );
		Lista <String> * Split ( const char * separador );
		Lista <String> * Split ( String separador );
		Lista <String> * Split ( char separador );
		Lista <String> * Split ( );
		static String Join ( Lista <String> * source, String enlace );
		static String Replace ( String source, String find, String replace);
};

#endif




