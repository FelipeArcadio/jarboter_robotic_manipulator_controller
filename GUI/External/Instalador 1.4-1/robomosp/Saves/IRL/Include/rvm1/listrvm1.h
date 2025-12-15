/****************************************************************************************/
/*					TAD LISTA					*/
/****************************************************************************************/
/*	El Siguiente TAD será añadido al código generado por el compilador para PA-10		*/
/****************************************************************************************/

#include <stdio.h>

template <class TIPOL>

class List
{
	struct nodo_
	{
		TIPOL dato;
		struct nodo_ * sig;
	};
	typedef struct nodo_ Nodo;
	
	private:
		Nodo * primero;
		Nodo * ultimo;
		int longitud;
	public:
	/*--------------------------------------------------------------------------------
	DESCRIPCION: Inicializador de las Lista. El procedimiento Lista
				Supone que la Lista no esta inicializada, ni contiene datos insertados
	PARAMETROS:  Una variable Lista de tipo Lista.
	USA:
	RETORNA:          - o -
	PRE: La Lista Lista no debe haber sido inicializada o si lo fue debe haber sido 
		Destruida.
	POST:
	----------------------------------------------------------------------------------*/
	
	List( )
	{
		primero	= new Nodo;
		ultimo 	= new Nodo;
		longitud = 0;
	}

	/*--------------------------------------------------------------------------------
	DESCRIPCION: Inserta un dato en la Lista, en la posicion donde esta la ventana
	             El procedimiento Insertar Supone que la Lista esta inicializada y la 
				 ventana esta definida a menos que la Lista este vacia.
	PARAMETROS:  Una variable Lista de tipo Lista y una variable elem de tipo TIPOL
	USA:
	RETORNA:  Un entero que representa un booleano, Verdadero cuando pudo insertar,
			  Falso cuando no lo logro.
	PRE: La Lista Lista debe estar inicializada, la ventana de la Lista Lista
		 debe estar definida o la Lista debe estar vacia.
	POST: Insertaralizar = 1 si se logro realizar la insercion.
	----------------------------------------------------------------------------------*/
	int Listins (TIPOL elem, int indice)
	{
		Nodo * nuevo;
		Nodo * iterator;
		Nodo * atras;
		int i;
		/*Error, la ventana está indefinida y la Lista no es vacia,
		no se puede realizar la operacion */
		if ( indice > longitud )
		{
			indice = longitud;
		}
	
		nuevo = new Nodo;
		nuevo->sig = NULL;
		nuevo->dato = elem;
	
		//Caso 1:  Lista Vacia
		if (longitud == 0)
		{
			primero = nuevo;
			ultimo = nuevo;
			longitud ++;
			return 1;
		}
		//Caso 2:  Se va insertar en el primer elemento
		if ( indice == 0 )
		{
			nuevo->sig = primero;
			primero = nuevo;
			longitud ++;
			return 1;
		}
		//Caso 3:  Caso General
		for ( i = 0, iterator = primero; i < indice; i++ )
		{
			atras = iterator;
			iterator = iterator -> sig;
		}
		nuevo->sig = iterator;
		atras->sig = nuevo;
		longitud++;
		return 1;
	}


	/*--------------------------------------------------------------------------------
	DESCRIPCION: Indica el tamaño de la Lista. El procedimiento Longitud
	             Supone que la Lista esta inicializada.
	PARAMETROS:  Una variable Lista de tipo Lista.
	USA:
	RETORNA: Un entero que representa el tamano de la Lista.
	PRE: La Lista Lista debe estar inicializada
	POST: Longitud representa el tamano de la Lista Lista.
	----------------------------------------------------------------------------------*/
	int Listlength ( )
	{
		return longitud;
	}
	
	/*--------------------------------------------------------------------------------
	DESCRIPCION: retorna el elememto situado en la posicion indicada por indice
	PARAMETROS: Un entero indice
	USA:
	RETORNA: Un elemento de la lista
	PRE: La Lista Lista debe estar inicializada
	POST: 
	----------------------------------------------------------------------------------*/
	TIPOL Listdata ( int indice )
	{
		Nodo * iterador;
		int i;
		for ( i = 0, iterador = primero; i < indice; i++ )
		{
			iterador = iterador -> sig;
		}
		return ( iterador->dato );
	}

	/*--------------------------------------------------------------------------------
	DESCRIPCION: retorna el elememto situado en la posicion indicada por indice
	PARAMETROS: Un entero indice
	USA:
	RETORNA: Un elemento de la lista
	PRE: La Lista Lista debe estar inicializada
	POST: 
	----------------------------------------------------------------------------------*/
	TIPOL operator [ ] ( int indice )
	{
		Nodo * iterador;
		int i;
		for ( i = 1, iterador = primero; i < indice; i++ )
		{
			iterador = iterador -> sig;
		}
		return ( iterador->dato );
	}

			
	/*--------------------------------------------------------------------------------
	DESCRIPCION: Adiciona un elemento a la lista
	PARAMETROS:  El elemento a adicionar
	USA:
	RETORNA: 1 si lo hizo, 0 si no
	PRE:  La lista creada, vacia o llena
	POST: Lista con un elemento adicional
	----------------------------------------------------------------------------------*/
	int Listadd (TIPOL elem) 
	{
		Nodo * nuevo;
		nuevo = new Nodo;
		nuevo->sig = NULL;
		nuevo->dato = elem;
	
		//Caso 1:  Lista Vacia
		if (longitud == 0)
		{
			primero = nuevo;
			ultimo = nuevo;
			longitud ++;
			return 1;
		}
		//Caso 2:  Caso General
		ultimo->sig = nuevo;
		ultimo = nuevo;
		longitud ++;
		return 1;
	}

	/*--------------------------------------------------------------------------------
	DESCRIPCION: Elimina un elemento de la Lista
	PARAMETROS:  Un entero que indica el indice del elementoa eliminar
	USA:
	RETORNA: 1 si lo logro, 0 si no.
	PRE: La Lista Lista debe tener elementos
	POST: Lla lista con un elemento menos
	----------------------------------------------------------------------------------*/
	int Listdel (int indice )
	{
		Nodo * iterador;
		Nodo * anterior;
		int i;
		if ( indice >= longitud )
		{
			return 0;
		}
		
		if ( indice == 1 )
		{
			Nodo * eliminar = primero;
			primero = primero->sig;
			if ( ultimo == eliminar )
			{
				ultimo = NULL;
			}
			delete eliminar;
			longitud --;
			return 1;
		}
		
		for ( i = 0, iterador = primero; i < indice; i++ )
		{
			anterior = iterador;
			iterador = iterador -> sig;
		}
		if ( ultimo == iterador )
		{
			ultimo = anterior;
			anterior->sig = NULL;
			delete ( iterador );
			longitud--;
			return 1;
		}
		anterior->sig = iterador->sig;
		delete ( iterador );
		longitud--;
		return 1;
	}

	/*--------------------------------------------------------------------------------
	DESCRIPCION: devuelve el indice de un elemento
	PARAMETROS:  elemento a buscar
	USA:
	RETORNA: -1 si no lo encuentra o el indice del elemento si lo encuentra
	PRE: La Lista Lista debe tener elementos
	POST: 
	----------------------------------------------------------------------------------*/
	int Listindex(TIPOL elem)
	{
		Nodo * iterator = primero;
		int equal;
		char * pointer1 = (char *)&elem;
		char * pointer2 = (char *)&iterator->dato;
		
		for(int i= 0; i<longitud; i++)
		{
			for(int j= 0; j< sizeof(TIPOL); j++)
			{
				equal = 1;
				pointer2 = (char *)&iterator->dato;
				if(pointer1[j]!= pointer2[j])
				{	
					equal = 0;
					j = sizeof(TIPOL);
				}	
			}
			if(equal == 1)
				return i + 1;
				
			iterator = iterator->sig;
		}
		return -1;
	}
	/*--------------------------------------------------------------------------------
	DESCRIPCION: Destruye la Lista
	PARAMETROS:  
	USA:
	RETORNA: -1 si no lo encuentra o el indice del elemento si lo encuentra
	PRE: La Lista Lista debe tener elementos
	POST: 
	----------------------------------------------------------------------------------*/
	void Listdestroy ( )
	{
		Nodo * iterador = primero;
		
		while ( iterador != NULL )
		{
			Nodo * eliminar = iterador;
			iterador = iterador->sig;
			delete eliminar;
		}
		ultimo = NULL;
		longitud = 0;
	
	}
};

// int main ( )
// {
// 	List <int> *l = new List <int>;
// 	int  a;
// 	int  b;
// 	int  c;
// 	int  d;
// 	int i;
// 	a = 10;
// 	b = 23;
// 	c = 50;
// 	d = 17;
// 	l->Listadd ( a );
// 	l->Listadd ( b );
// 	l->Listins ( c, 1 );
// 	l->Listins ( d, 0 );
// 	l->Listdel ( 2 );
// 	printf ("Datos:\n");
// 	for ( i = 0; i < l->Listlength(); i ++ )
// 	{
// 		printf ("d: %d\n", l->Listdata(i));
// 	}
// 	
// 	printf("La posicion de 23 es: %d\n", l->Listindex(23) );
// 	printf("La posicion de 50 es: %d\n", l->Listindex(50) );
// 	printf("La posicion de 10 es: %d\n", l->Listindex(10) );
// 	return 1;
// }

