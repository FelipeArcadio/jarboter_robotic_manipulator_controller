#ifndef __PCTYPES_H__
#define __PCTYPES_H__


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      PCtypes.h
// Ubicación fuente: No tiene
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Declaración de las estructuras que son utilizadas sólo en  el programa de alto nivel que se ejecuta en el PC.
//
// Compilado en:
//   Microsoft Visual Studio 6.0 API 
//
// Author: Felipe Hurtado
//         David Osorio
//
// Version: 1.0
//  
// 
// Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <windows.h>			//funciones básicas de ventanas graficas
#include <windowsx.h>			//funciones extendidas de ventanas graficas


#define		uint8			byte		

/** \defgroup tipo_datos_pc  TIPOS DE DATOS DE LA INTERFAZ GRÁFICA DE USUARIO
    \ingroup GUI
	\brief Declaración de las estructuras que son utilizadas en el programa de la interfaz gráfica de usuario.
    \code #include <PCtypes.h> \endcode */





/** \ingroup tipo_datos_pc 
*	\brief Almacena el perfil de movimiento de una articulación, esta información es extraída del archivo de 
			trayectoria generado por ROBOMOSP. 
*	\struct PROFILE
*/
typedef struct
{ 
 double Tiempo; 	/**< Almacena la línea de tiempo del movimiento*/
 double pos; 		/**< Almacena la posición de la articulación para ese instante de tiempo*/
 double velo;		/**< Graba la velocidad de la articulación para ese instante de tiempo */
 double ace;		/**< Guarda la aceleración de la articulación para ese instante de tiempo*/
}PROFILE,*pPROFILE; 


//----------------------------------------------------------------------------

/** \ingroup tipo_datos_pc 
*	\brief Estructura utilizada para almacenar las características del archivo  de trayectoria generado por ROBOMOSP.
*	\struct DIMEN *pDIMEN
*/
typedef  struct 
{ 
 int puntos;			/**< Cantidad de puntos en el archivo trayectoria */
 int Numdata;			/**< Cantidad de datos en el archivo trayectoria */
 int col;				/**< Número de columnas del archivo */
 int Maxjoint;	 		/**< Número de ejes con información de movimiento*/
}DIMEN,*pDIMEN;



//----------------------------------------------------------------------------

/** \ingroup tipo_datos_pc 
*	\brief Conjunto de parámetros que se necesitan ser enviados o entregados a los cuadros de diálogo emergentes 
			utilizando la función DialogBoxParam. 
*	\struct Listapara
*/
typedef struct 
{ 
 int PoseID;					     /**< Código que indica que no hay ejes activados */
 HWND  Handle; 					     /**<  Manejador de la ventana principal */
 ULONG Tsize;					     /**< Código que indica que no hay ejes activados */
 HINSTANCE Instancia;			     /**< Instancia del programa */
 int *NpuntosAlmacenados;		     /**< Apuntador a la cantidad de poses almacenadas */
 signed short int **P_pVectorposes;  /**< Apuntador a al vector de poses */
}Listapara; 



#endif
