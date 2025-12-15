#ifndef __DIALOGOTECHPENDAL_H__
#define __DIALOGOTECHPENDAL_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File:      DialogoTechPendant.h
//  Ubicación fuente: C:\Jarboter_V_2_1\GUI\C\DialogoTechPendant.c
//
//  Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
//  Propósito General:	
// 	Conjunto de funciones que define el comportamiento del cuadro de dialogo "TERMINAL_ENSEÑANZA",	
//	mediante esta interfaz es posible: 
//	-Enviar al robot los ángulos articulares que luego son internamente convertidos en micropasos para mover el robot.
//	-Leer las poses del controlador desde la EEPROM ó guardar poses en ésta.
//	-Ejecutar poses grabadas anteriormente.
//	-Identificar la pose actual. 
//	-Ejecutar movimiento entre poses, es decir ir de la poses 1 a la poses 2.
//	-Crear una trayectoria de poses para mover el robot.
//  -Calcular la cinemática directa e inversa.
//  -Manejo del efector final
//  -Ir a la posición de origen y HOME.
//  Compilado en:
//  Microsoft Visual Studio 6.0 API 
//
//  Author: Felipe Hurtado
//          David Osorio
//
//  Version: 1.0
//  
// 
//  Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "types.h"  
#include "USBfunc.h"    
#include "mem.h"        
#include "resource.h"   
#include "packetIO.h"   
#include "messages.h"   
#include "conversion.h" 
#include <commctrl.h>   


#if defined(__cplusplus)
extern "C" {
#endif

/** \defgroup Cuadro_Dialogo_Tech_Pendant CUADRO DE DIÁLOGO DE LA TERMINAL DE ENSEÑANZA
	\ingroup GUI
	\brief Conjunto de funciones que define el comportamiento del cuadro de dialogo "TERMINAL_ENSEÑANZA",	
	mediante esta interfaz es posible: 
	- Enviar al robot los ángulos articulares que luego son internamente convertidos en micropasos para mover el robot.
	- Leer las poses del controlador desde la EEPROM ó guardar poses en ésta.
	- Ejecutar poses grabadas anteriormente.
	- Identificar la pose actual. 
	- Ejecutar movimiento entre poses, es decir ir de la poses 1 a la poses 2.
	- Crear una trayectoria de poses para mover el robot.
	- Calcular la cinemática directa e inversa.
	- Manejo del efector final
	- Ir a la posición de origen y HOME.
	
	\code #include <DialogoTechPendant.h> \endcode 

*/
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Habilita o deshabilita  los campos de  posición, velocidad y aceleración para un determinado eje
		en el cuadro de diálogo TERMINAL_ENSEÑANZA.
		@param hDlg  Manejador de la ventana principal.
		@param op ID del eje seleccionado.
		@param habilitar 1 en este parámetro habilita el campo, 0 los deshabilita.
	*/
	void Habilitar_campostech(HWND hDlg,int op,int habilitar);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Detecta si se ha seleccionado un eje desde el cuadro de diálog para actualizar el vector de ejes 
		seleccionados (axisOP[DOF]) y habilitar o deshabilitar los respectivos campos gráficos mediante la 
		función Habilitar_campostech.
		@param hDlg  Manejador de la ventana principal.
		@param nIDDlgItem ID de la casilla de verificación.
		@param op Es un número que corresponde al eje seleccionado, este número está definido por la enumeración
			   tEnumtAxis. 
		@param axisOP apuntador al vector de ejes seleccionados por el usuario.
	*/
	void manejador_Campostech(HWND hDlg,int nIDDlgItem,int op,int *axisOP);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Ejecuta las conversiones necesarias para que los datos del usuario estén en el formato numérico de los 
		PMDs (Controladores de Motores). Estos datos son almacenados en la estructura llamada ptrajectoryTECH.
		@param ptrajectoryTECH  Estructura que será actualizada con los datos de usuario.
		@param axisSettings Estructura que contiene los parámetros del motor.
		@param posicion  Valor en grados de la posición deseada.
		@param velocidad Valor en grados/s de la velocidad deseada.
		@param aceleracion Valor en grados/s^2 de la aceleración deseada.
		@param Axis El eje seleccionado.
	*/
	void Configurar_techTraj(tTrajectory * ptrajectoryTECH,tAxisSettings *axisSettings,double posicion,
						double velocidad,double aceleracion,tAxis Axis);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Obtiene los datos del cuadro de diálogo e indica si son válidos.
		@param hDlg Manejador de la ventana principal.
		@param Axis Indica el eje seleccionado.  
		@param nIDDlgItempos ID del campo posición para ese eje.
		@param nIDDlgItemace ID del campo aceleración para ese eje.
		@param nIDDlgItemvel ID del campo velocidad para ese eje.
		@param posicion Apuntador a la variable que almacenará la posición del usuario.
		@param velocidad Apuntador a la variable que almacenará la velocidad del usuario.
		@param aceleracion Apuntador de la variable que almacenerá la aceleración de usuario.
		@return 1 si los datos son válidos, 0 si no lo son.
	*/	
	int GetDatosTech(HWND hDlg,int Axis, int nIDDlgItempos,int nIDDlgItemace,int nIDDlgItemvel,
				 double *posicion,double *velocidad,double *aceleracion);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Llena una estructura tipo trayectoria según la información del usuario en el cuadro de diálogo 
		TERMINAL_ENSEÑANZA.	Para esto hace uso de las funciones:
			- GetDatosTech (Permite obtener los datos del cuadro de diálogo TERMINAL_ENSEÑANZA) 
			- Configurar_techTraj (Realiza las conversiones a los datos para que pueden ser procesados por los PMD).
		@param hDlg Manejador de la ventana principal.
		@param pTECH  Estructura que será actualizada con los datos de usuario.
		@param axisSettings vector que contiene la configuración de cada eje del robot. 
		@param axisOP Apuntador del vector de los ejes seleccionados (axisOP[DOF]), esto con el fin de solo almacenar 
		los eje activos. 
		@return 1 si los datos son válidos o 0 si no lo son.
	*/
	int Llenar_campos_tech(HWND hDlg,tTrajectory * pTECH,tAxisSettings *axisSettings,int *axisOP);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Ejecuta movimientos predefinidos o con la información de usuario.
		@param hDlg Manejador de la ventana principal.
		@param axisSettings Estructura que contiene los parámetros del motor. 
		@param axisOP Apuntador al vector de ejes seleccionados por el usuario.
		@param Caso Indica el tipo de movimiento 0=> Movimiento con datos del Usuario.
												 1=> Movimiento hacia ORIGEN.
												 2=> Movimiento hacia HOME.
		@return 1 Si la información del movimiento es enviada correctamente y cero en caso contrario.
	*/
	int RobotGoto(HWND hDlg,tAxisSettings *axisSettings,int *axisOP,int Caso);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Coloca los botones gráficos en el cuadro de dialogo "TERMINAL_ENSEÑANZA".
		@param hDlg Manejador de la ventana principal.
		@param hInstance Instancia del programa, es necesaria para cargar los mapas de bits.
	*/
	void cargoBotonTechPendal(HWND hDlg,HINSTANCE hInstance);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Obtiene un número punto flotante de un cuadro de edición.
		@param hDlg Manejador de la ventana principal.
		@param IDItem ID del cuadro de edición que se quiere leer.
		@return El número en formato long.
	*/
	double getlong(HWND hDlg,int IDItem);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Coloca un número tipo long en un cuadro de edición.
		@param hDlg Manejador de la ventana principal.
		@param IDC_EDIT Identificador del campo a modificar.
		@param Numero Es el número que se quiere colocar en el cuadro de edición.
	*/
	void setlong(HWND hDlg,int IDC_EDIT,double Numero);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Busca dentro  en el vector de poses GrabadoIDPose[],la etiqueta de pose PoseID. 
		@param GrabadoIDPose Vector de poses.
		@param PoseID ID de pose a buscar.
		@param Maxpose Longitud del vector de poses.
		@return 1 Cuando encuentra a la pose y 0 en caso contrario.
	*/
	BOOLEAN ExisteIDPose(int16 * GrabadoIDPose,int PoseID,int Maxpose);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Retorna el índice en el vector GrabadoIDPose[] donde está grabada la etiqueta de la pose, Pose ID.
		@param GrabadoIDPose Vector de Poses
		@param PoseID ID de pose a buscar.
		@param Maxpose Longitud del vector de poses.
		@return Índice donde se encuentra almacenada la pose ó -1 Si la pose no existe.
	*/
	int BuscarIndexPose(int16 *GrabadoIDPose,int PoseID,int Maxpose);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Retorna el ID de la pose que se encuentra almacenada en la posicion PoseIndex del vector de poses,
		GrabadoIDPose[].
		@param GrabadoIDPose Vector de poses.
		@param PoseIndex Índice del vector Posición.
		@return ID de la pose.
	*/
	int return_id(int16 *GrabadoIDPose,int PoseIndex);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Modifica la dirección a la  que apunta el vector de IDs, pvectorIDs. Esto como el fin 
		de modificarlo en tiempo de ejecución.
		@param P_pvectorIDs Apuntador  al apuntador pvectorIDs.  
		@param mode Indica el modo de actualización:
				1=> Inicialización del Apuntador pvectorIDs con los datos de rxvector que proviene de la memoria EEPROM.
				2=> Se agrega una nueva pose al vector de IDs
				3=> Se borra la última pose en el vector de IDs.
		@param PoseID Pose a almacenar.
	*/
	void Actualizar_Vector(int16** P_pvectorIDs,int mode,int PoseID);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Obtiene los ángulos de los ejes seleccionados por el usuario en el cuadro de dialogo 
		"TERMINAL_ENSEÑANZA", si un eje no es seleccionado o ha sido seleccionado sin llenar sus campos, la función coloca 
		los ángulos a cero para dicho eje.
		@param hDlg Manejador de la ventana principal.
		@return Retorna un Apuntador a un vector de 5 posiciones donde cada posición contiene el ángulo para cada eje
		en el siguiente orden: Cintura, Hombro, Codo, Pitch, Roll.
	*/
	double* GetGradosAxis(HWND hDlg);    
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Configura las barras de desplazamiento del cuadro de dialogo  "TERMINAL_ENSEÑANZA" colocando 
		los máximos y mínimos para cada articulación. 
		@param hDlg Manejador de la ventana principal.
		@param axisSettings Apuntador a la estructura que contiene los ángulos  máximos y mínimos para cada 
				articulación. 
	*/
	void SetScrollBottons(HWND hDlg,tAxisSettings *axisSettings);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Atiende la petición del usuario para la barra de desplazamiento identificada por ControlScrollbar. 
		Los  códigos de usuario	posibles son:
			- SB_LINERIGHT     ( Presionado FLECHA DERECHA).
			- SB_LINELEFT      ( Presionado FLECHA IZQUIERDA).
			- SB_PAGERIGHT     ( Presionado ZONA DESPUES DEL BOTON).
			- SB_PAGELEFT      ( Presionado ZONA ANTES DEL BOTON ).
			- SB_THUMBPOSITION ( Se arrastra el botón).
	
		@param hDlg Manejador de la ventana principal.
		@param ControlScrollbar ID de la bara de desplazamiento a actualizar captada por WINDOWS.
		@param ControlEdit ID del campo edit donde se refleja el valor de la barra de desplazamiento.
		@param Codigo Código captado por WINDOWS cuando se utiliza el ControlScrollbar.
		@param Posicion Posición captada por WINDOWS cuando se utiliza el ControlScrollbar.
		@param max Valor Máximo permitido en la posición del botón.
		@param min Valor Min permitido en la posición del botón.
	*/
	void ProcesarScrollH(HWND hDlg, HWND ControlScrollbar, int ControlEdit,int Codigo, int Posicion,int max, int min);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Detecta si se ha seleccionado la articulación para habilitar su respectiva barra de desplazamiento
		y atiende las peticiones del usuario mediante la función ProcesarScrollH.
		@param hDlg Manejador de la ventana principal.
		@param axisSettings Estructura que contiene los máximos y míninos de la barra de desplazamiento.
		@param ControlScrollbar ID de la barra de desplazamiento a actualizar captada por WINDOWS.
		@param Codigo Código captado por WINDOWS cuando se utiliza el ControlScrollbar.
		@param Posicion Posición captada por WINDOWS cuando se utiliza el ControlScrollbar.
		@param AxisOP Apuntador al Vector que contiene las articulaciones seleccionadas en el momento.
	*/
	void ActualizarScrollBar(HWND hDlg,tAxisSettings *axisSettings, HWND ControlScrollbar,int Codigo, 
								int Posicion,int* AxisOP);

	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Coloca los ángulos articulares para cada eje en el cuadro de diálogo "TERMINAL_ENSEÑANZA".
		@param hDlg Manejador de la ventana principal.
		@param axisOP apuntador al vector de ejes seleccionados por el usuario.
		@param q Vector de ángulos articulares que se colocarán en cada eje.
	*/
	void SetAngle(HWND hDlg,int * axisOP,double * q);
	
	/** \ingroup Cuadro_Dialogo_Tech_Pendant
		\brief Coloca secuencialmente los identificadores de poses que se encuentran en el vector pvectorIDs a la barra
               desplegable identificada por ComboxID.
		@param hDlg Manejador de la ventana principal. 
		@param pvectorIDs Apuntador al vector de poses. 
		@param ComboxID Identificador del cuadro de desplazamiento.
	*/
	void Put_ID(HWND hDlg,int16* pvectorIDs,int ComboxID);

#if defined(__cplusplus)
}
#endif

#endif

