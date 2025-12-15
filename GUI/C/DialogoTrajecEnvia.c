#include "DialogoTrajecEnvia.h"
#include "Procesamiento_archivos.h" 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************************/
/*Simplemente aumenta el valor de la progressbar (osea que avanza graficamente) 
y duerme unos milisegundos

HWND hWndPB Manejador de la ventana que contiene el progressbar
int nValor: Cantidad posiones que avanza
int nDormir: Tiempo de espera entre una o otra posicion 
*/
void AumentarValor(HWND hWndPB,int nValor, int nDormir)
{
    SendMessage(hWndPB, PBM_STEPIT, (WPARAM) nValor, 0);
    Sleep(nDormir);
}

/*Reinicia el progressbar borrando lo que ha avanzado*/
void Reiniciar(HWND hWndPB)
{
	SendMessage(hWndPB, PBM_SETPOS, (WPARAM) 0 , 0);
}

/*****************************************************************************************/
/*Imprime de nuevo el progressbar*/
void Actualizar(HWND hWndPB)
{
	UpdateWindow(hWndPB);
}

/*****************************************************************************************/
void DisplayTrayectInfo(HWND hDlg,tTrajectory * Pptrajectory,int nIDDlgItem)
{
	HWND hOutputBox;
	char tempbuff[1000];
	int Npoint,Naxis;
   
	hOutputBox = GetDlgItem (hDlg,nIDDlgItem);
	SendMessage (hOutputBox, 
	            LB_ADDSTRING, 
				0, 
				(LPARAM)"Punto	Axis	pos		Vel		Ace");  
   
	for (Npoint=0;Npoint<Pptrajectory->size;Npoint++)  //Recorro todos los puntos
	{
        for (Naxis=0;Naxis<5;Naxis++)  //Recorro todos los puntos
		{
            sprintf(tempbuff,"%d	%d	%ld		%ld		%ld",
            Npoint,
			Naxis,
			Pptrajectory->point[Npoint].axis[Naxis].position,
			Pptrajectory->point[Npoint].axis[Naxis].velocity,
			Pptrajectory->point[Npoint].axis[Naxis].acceleration);
            SendMessage (hOutputBox, LB_ADDSTRING, 0, (LPARAM)tempbuff); 
			//Sleep(10);
		}
   }
}


/*****************************************************************************************/
/*Lena la estructura tTrajectory con el strem file representado por datos */
int Llenar_trayectoria(tTrajectory *ptrajectory,tAxisSettings *axisSettings,pDIMEN dimem,double* datos,int SET_ROBOT)
{
	int Naxis,Npoint;
	pPROFILE DatosAxis=NULL;
	double test;
	double rad;
	double velocity;

	if(SET_ROBOT==0)
		setupAxisSettings(&axisSettings[0]);  //Se inicializa las caracteristicas de los ejes en caso de que el sistema no se halla resetead
	ptrajectory->size=dimem->puntos;
	// DatosAxis es un apuntador una estructura intermedia que tiene la finalidad de estructurar los datos 
	// apuntados por por el parametro (double* datos) de forma para extraer de este la posicion velocidad y aceleracion
	// Para todos los puntos de la trayectoria
	DatosAxis=(pPROFILE)declarar(dimem->puntos*sizeof(PROFILE)); 
	if(dimem->Maxjoint>DOF)											
		return rcFileOpenError;	   
	for(Naxis=0; Naxis<dimem->Maxjoint; Naxis++) 
	{   
		//obtiene la posicion,velocidad y aceleracion para un eje determinado para todos los punto del archivo 
		ObtenerProfile(DatosAxis,Naxis,dimem,datos);  
		/*Actualizo un eje de la estructura ptrajectory para todos los puntos*/
		for(Npoint=0; Npoint<dimem->puntos; Npoint++) 
		{
			switch (Naxis)                  /* manipulador del mensaje */
			{
				case AxisWaist:     //0
					ptrajectory->point[Npoint].axis[Naxis].position=  positionPCtoUC( rad2usteps( &(axisSettings[Naxis]) ,DatosAxis[Npoint].pos) );
				break;   
				case AxisShoulder:  // posicion = posicion-(grados2rad(-140));
					rad = DatosAxis[Npoint].pos - grados2rad((double)axisSettings[Naxis].minAxisAngle);//axisSettings[Naxis].add_num;
					test=rad2usteps( &(axisSettings[Naxis]) ,rad );
					ptrajectory->point[Npoint].axis[Naxis].position=positionPCtoUC(test);
				break;
				case AxisElbow: //posicion = posicion-grados2rad(120); 
					rad = DatosAxis[Npoint].pos - grados2rad((double) axisSettings[Naxis].maxAxisAngle);//(long)axisSettings[Naxis].add_num;
					test=rad2usteps( &(axisSettings[Naxis]) ,rad );
					ptrajectory->point[Npoint].axis[Naxis].position=positionPCtoUC(test);
				break;
				case AxisPitch:
				case AxisRoll : 
					ptrajectory->point[Npoint].axis[Naxis].position=  positionPCtoUC( rad2usteps( &(axisSettings[Naxis]) ,DatosAxis[Npoint].pos) );
				break;
			}
			velocity=abs(velocityPCtoUC(rad2usteps(&(axisSettings[Naxis]),DatosAxis[Npoint].velo)));
			ptrajectory->point[Npoint].axis[Naxis].velocity=(long)velocity;
			if(velocity<axisSettings[Naxis].minVelocity)
				ptrajectory->point[Npoint].axis[Naxis].velocity= axisSettings[Naxis].minVelocity;
			if(velocity>axisSettings[Naxis].maxVelocity)
				ptrajectory->point[Npoint].axis[Naxis].velocity= axisSettings[Naxis].maxVelocity;
		    ptrajectory->point[Npoint].axis[Naxis].acceleration= 
			abs(accelerationPCtoUC(rad2usteps(&(axisSettings[Naxis]),DatosAxis[Npoint].ace))); //ojo el abs mata la parte decimal
		} //for point
	} // for axis
	free(DatosAxis);
	return rcOK;
}		




