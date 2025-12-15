/****************************************************************************
*      INTERFAZ MODULO: listas.h
*      FECHA DE CREACION: Septiembre 4 de 2002
*----------------------------------------------------------------------------
*      Requerimientos de otros modulos:
*           Se debe definir el template de la Lista
*	    Si se necesita usar las Listas de C++ debe definir CPP
*	    "#define CPP", tomara por defecto ANSI C
*      Autor:
*           Olver Eduardo Ramos Lucumi
*      Modificaciones:
*           Olver Eduardo Ramos Lucumi
*      Fecha:
*	    Diciembre 24 de 2002
*      Ultima Modificacion:
*	    Hacer la translacion de las listas de ANSI C a C++
*
*****************************************************************************/

/*--------------------------------------------------------------------------*
		   Archivos de interfaz de librerias estandar
*--------------------------------------------------------------------------*/

#include <iostream.h>
#ifndef LISTAS
#define LISTAS
/*--------------------------------------------------------------------------*
		  Declaracion de constantes y macros publicos
*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*
	  Declaracion de struct's,union's y typedef's publicos
*--------------------------------------------------------------------------*/


template <class TIPOL>

class Lista
{
	struct nodo_
	{
		TIPOL dato;
		struct nodo_ * sig;
		struct nodo_ * ant;
	};

	typedef struct nodo_ Nodo;

	private:
		Nodo * primero;
		Nodo * ultimo;
		Nodo * ventana;
		int longitud;
		int posicion;
	public:
		Lista ();
		Lista ( TIPOL elem );
		int Insertar (TIPOL elem);
		int Anexar (TIPOL elem);
		int Siguiente ();
		int Anterior ();
		TIPOL & Info ();
		int Longitud ();
		int Fin ();
		int Eliminar ();
		int Eliminar ( TIPOL elem );
		int EliminarTodos ( TIPOL elem );
		int Primero ();
		int Ultimo ();
		bool esUltimo ( );
		bool esPrimero ( );
		int Posicionar (int pos);
		int Destruir ();
		bool estaVacia ();
		int Pegar (Lista<TIPOL> lista);
		int Pegar (Lista<TIPOL> * lista);
		void Imprimir ();
		void operator = (Lista<TIPOL> & lista);
		TIPOL operator [] (int indice);
		int Encontrar ( TIPOL elem );
		int Indice ( );
};




/*================================================================
       LISTAS DOBLEMENTE ENCADENADAS
  ================================================================*/



/*--------------------------------------------------------------------------------
DESCRIPCION:      Inicializador de las Listas

El procedimiento Lista
Supone que la lista no esta inicializada, ni contiene datos insertados

PARAMETROS:       Una variable lista de tipo Lista.
USA:
RETORNA:          - o -
PRE:              La Lista lista no debe haber sido inicializada o si lo fue debe haber
		  sido Destruida.
POST:
EJEMPLO LLAMADO:  Lista lst;
----------------------------------------------------------------------------------*/

template <class TIPOL>

Lista<TIPOL>::Lista ()
{
	primero	= NULL;
	ultimo 	= NULL;
	ventana = NULL;
	longitud= 0;
	posicion = -1;
}


/*--------------------------------------------------------------------------------
DESCRIPCION:      Inicializador de las Listas

El procedimiento Lista
Supone que la lista no esta inicializada, ni contiene datos insertados

PARAMETROS:       Una variable lista de tipo Lista.
USA:
RETORNA:          - o -
PRE:              La Lista lista no debe haber sido inicializada o si lo fue debe haber
		  sido Destruida.
POST:
EJEMPLO LLAMADO:  Lista lst;
----------------------------------------------------------------------------------*/

template <class TIPOL>

Lista<TIPOL>::Lista ( TIPOL elem )
{
	primero	= NULL;
	ultimo 	= NULL;
	ventana = NULL;
	longitud= 0;
	Insertar ( elem );
	posicion = 0;
}


/*--------------------------------------------------------------------------------
DESCRIPCION:      Inserta un dato en la lista, en la posicion donde esta la ventana

El procedimiento Insertar
Supone que la lista esta inicializada y la ventana esta definida a menos que la
lista este vacia.

PARAMETROS:       Una variable lista de tipo Lista y una variable elem de tipo TIPOL
USA:
RETORNA:          Un entero que representa un booleano, Verdadero cuando pudo insertar,
		  Falso cuando no lo logro.
PRE:              La Lista lista debe estar inicializada, la ventana de la Lista lista
		  debe estar definida o la lista debe estar vacia.
POST:             Insertaralizar = 1 si se logro realizar la insercion.
EJEMPLO LLAMADO:  lst.Insertar ( 5 );
----------------------------------------------------------------------------------*/

template <class TIPOL>

int Lista <TIPOL>::Insertar (TIPOL elem)
{
	Nodo * nuevo;
	/*Error, la ventana está indefinida y la lista no es vacia,
	  no se puede realizar la operacion */
	if (Fin() && !estaVacia())
	{
		return 0;
	}

	nuevo = new Nodo;
	nuevo->ant = NULL;
	nuevo->sig = NULL;
	nuevo->dato = elem;

	//Caso 1:  Lista Vacia
	if (estaVacia ())
	{
		primero = nuevo;
		ultimo = nuevo;
		ventana = nuevo;
		longitud ++;
		return 1;
	}
	//Caso 2:  La ventana está en el primer elemento
	if (ventana == primero)
   {
		nuevo->sig = primero;
		ventana->ant = nuevo;
		primero = nuevo;
		ventana = nuevo;
		longitud ++;
		return 1;
	}
	//Caso 3:  Caso General
	nuevo->ant = ventana->ant;
	nuevo->sig = ventana;
	ventana->ant->sig = nuevo;
	ventana->ant = nuevo;
	ventana = nuevo;
	longitud++;
	return 1;
}


/*--------------------------------------------------------------------------------
DESCRIPCION:     Indica el tamaño de la Lista.

El procedimiento Longitud
Supone que la lista esta inicializada.

PARAMETROS:       Una variable lista de tipo Lista.
USA:
RETORNA:          Un entero que representa el tamano de la Lista.
PRE:              La Lista lista debe estar inicializada
POST:             Longitud representa el tamano de la Lista lista.
EJEMPLO LLAMADO:  lst.Longitud (  );
----------------------------------------------------------------------------------*/
template <class TIPOL>

int Lista<TIPOL>::Longitud ()
{
	return longitud;
}

/*--------------------------------------------------------------------------------
DESCRIPCION:      Elimina el dato de la lista senalado por la ventana

El procedimiento Eliminar
Supone que la lista esta inicializada, que tiene al menos un elemento y la ventana 
esta definida.

PARAMETROS:       Una variable lista de tipo Lista
USA:              
RETORNA:          Un entero que representa un booleano, Verdadero cuando pudo eliminar,
		  Falso cuando no lo logro.
PRE:              La Lista lista debe estar inicializada, la lista debe tener al menos un 
		  elemento, la ventana de la Lista lista debe estar definida
POST:             Eliminar = 1 si se logro realizar la eliminacion.
EJEMPLO LLAMADO:  lst.Eliminar ( );
----------------------------------------------------------------------------------*/

template <class TIPOL>

int Lista<TIPOL>:: Eliminar ()
{
	Nodo * elim;
        /*Error, la ventana está indefinida,
	  no se puede realizar la operacion */
	if (Fin())
	{
		return 0;
	}

	elim = ventana;

        //Caso 1: Solo hay un solo elemento
	if (Longitud () == 1)
	{
		primero = NULL;
		ultimo = NULL;
		ventana = NULL;
		longitud --;
		delete (elim);
		return 1;
	}

        //Caso 2:  La ventana esta en la primera posicion
	if (ventana == primero)
	{
		ventana = elim->sig;
		ventana->ant = NULL;
		primero = ventana;
		longitud --;
		delete (elim);
		return 1;
	}
        //Caso 3:  La ventana esta en la ultima posicion
	if (ventana == ultimo)
	{
		ventana = NULL;
		ultimo =  elim->ant;
		ultimo->sig = NULL;
		longitud --;
		delete (elim);
		return 1;
	}
	//Caso 4:  General
	elim->ant->sig = ventana->sig;
	ventana = elim->sig;
	ventana->ant = elim->ant;
	longitud --;
	delete (elim);
	return 1;
}

template <class TIPOL>

int Lista<TIPOL>::Destruir()
{

	/*Error, la lista esta vacia,
	  no se puede realizar la operacion */
	if (estaVacia())
	{
		return 0;
	}

	Primero();
	while (!estaVacia())
	{
		Eliminar();
	}
	return 1;
}

template <class TIPOL>

bool Lista<TIPOL>::estaVacia ()
{
	return (longitud == 0);
}

template <class TIPOL>

bool Lista<TIPOL>::esUltimo ()
{
	if ( ventana != NULL )
		return ( ventana->sig == NULL );
	return false;
}

template <class TIPOL>

bool Lista<TIPOL>::esPrimero ()
{
	if ( ventana != NULL )
		return ( ventana->ant == NULL );
	return false;
}

template <class TIPOL>

int Lista<TIPOL>::Primero ()
{
	/*Error, la lista esta vacia,
	  no se puede realizar la operacion */
	if (estaVacia())
	{
		return 0;
	}

	ventana = primero;
	posicion = 0;
	return 1;
}

template <class TIPOL>

int Lista<TIPOL>::Fin()
{
	return (ventana == NULL);
}


template <class TIPOL>

TIPOL& Lista<TIPOL>::Info ()
{
	return (ventana->dato);
}



template <class TIPOL>

int Lista<TIPOL>::Siguiente ()
{
	/*Error, la ventana está indefinida,
	  no se puede realizar la operacion */
	if (Fin())
	{
		return 0;
	}
	ventana = ventana->sig;
	posicion++;
	if ( posicion == longitud )
	{
		posicion = -1;
	}
	return 1;
}


template <class TIPOL>

int Lista<TIPOL>::Anexar (TIPOL elem)
{
	Nodo * nuevo;
	/*Error, la ventana está indefinida y la lista no es vacia,
	  no se puede realizar la operacion */
	if (Fin() && !estaVacia())
	{
		return 0;
	}

	nuevo = new Nodo;
	nuevo->ant = NULL;
	nuevo->sig = NULL;
	nuevo->dato = elem;

    //Caso 1:  Lista Vacia
	if (estaVacia ())
	{
		primero = nuevo;
		ultimo = nuevo;
		ventana = nuevo;
		longitud ++;
		posicion ++;
		return 1;
	}
	//Caso 2:  La ventana está en el ultimo elemento
	if (ventana == ultimo)
	{
		nuevo->ant = ultimo;
		ventana->sig = nuevo;
		ultimo = nuevo;
		ventana = nuevo;
		longitud ++;
		posicion ++;
		return 1;
	}
	//Caso 3:  Caso General
	nuevo->sig = ventana->sig;
	nuevo->ant = ventana;
	ventana->sig->ant = nuevo;
	ventana->sig = nuevo;
	ventana = nuevo;
	longitud++;
	posicion ++;
	return 1;
}


template <class TIPOL>

int Lista<TIPOL>::Ultimo ()
{
	/*Error, la lista esta vacia,
	  no se puede realizar la operacion */
	if (estaVacia())
	{
		return 0;
	}

	ventana = ultimo;
	posicion = longitud - 1;
	return 1;
}


template <class TIPOL>

int Lista<TIPOL>::Anterior ()
{
	/*Error, la ventana está indefinida,
	  no se puede realizar la operacion */
	if (Fin())
	{
		return 0;
	}
	ventana = ventana->ant;
	posicion --;
	return 1;
}

template <class TIPOL>

int Lista<TIPOL>::Posicionar (int pos)
{
	int i;
	/*Error, la posicion se sale del rango,
	  no se puede realizar la operacion */
	if ( pos >= longitud || pos < -longitud )
	{
		return 0;
	}
	i = 0;
	//Caso 1:  Se quiere ubicar con respecto del comienzo
	if (pos >= 0)
	{
		Primero ();
		while (i < pos)
		{
			Siguiente();
			i++;
		}
		return 1;
	}
	//Caso 2:  Se requiere ubicar desde el final
	i = -longitud;
	Primero ();
	while (i < pos)
	{
		Siguiente();
		i++;
	}
	return 1;
}

template <class TIPOL>

int Lista<TIPOL>::Pegar(Lista<TIPOL> lista)
{
	Ultimo();
	lista.Primero();
	while (!lista.Fin())
	{
		Anexar(lista.Info());
		lista.Siguiente();
	}
	return 1;
}

template <class TIPOL>

int Lista<TIPOL>::Pegar(Lista<TIPOL> * lista)
{
	Ultimo();
	lista->Primero();
	while (!lista->Fin())
	{
		Anexar(lista->Info());
		lista->Siguiente();
	}
	return 1;
}


template <class TIPOL>

void Lista<TIPOL>::Imprimir ()
{
	int i = longitud;
	Nodo * antventana = ventana;
	int pos = posicion;
	for (Primero(); !Fin(); Siguiente(), i--)
	{
		cout << Info();
		if ( i > 1 )
		{
			cout << ',';
		}
	}
	ventana = antventana;
	posicion = pos;
}

template <class TIPOL>

int Lista<TIPOL>::Encontrar (TIPOL elem)
{
	int i;
	Nodo * ventanaant = ventana;
	int pos = posicion;
	for ( i = 0, Primero ( ); !Fin ( ); Siguiente ( ), i++ )
	{
		if ( Info ( ) == elem )
		{
			return i;
		}
	}
	ventana = ventanaant;
	posicion = pos;
	return -1;
}

template <class TIPOL>

void Lista<TIPOL>::operator = (Lista <TIPOL> & lista)
{
	Destruir ();
	Pegar (lista);
}

template <class TIPOL>

int Lista<TIPOL>::Indice ( )
{
	return posicion;
}

template <class TIPOL>

int Lista<TIPOL>::Eliminar ( TIPOL elem )
{
	int indice = Encontrar ( elem );
	if ( indice != -1 )
	{
		Posicionar ( indice );
		Eliminar ( );
		return 1;
	}
	return 0;
}

template <class TIPOL>

int Lista<TIPOL>::EliminarTodos ( TIPOL elem )
{
	for ( Primero ( ); !Fin ( ); Siguiente ( ) )
	{
		if ( Info ( ) == elem )
		{
			Eliminar ( );
			Anterior ( );
		}
	}
	return 1;
}

template <class TIPOL>

TIPOL Lista<TIPOL>::operator [] ( int indice )
{
	Nodo * ventanaant = ventana;
	TIPOL elemento;
	int pos = posicion;
	Posicionar ( indice );
	elemento = Info ( );
	ventana = ventanaant;
	posicion = pos;
	return elemento;
}

#endif
/******************************* EOF ***************************************/

