#include "DialogoTRACE.h"
#include "DialogoTechPendant.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>




/*****************************************************************************************/
void FillAxisTracePMD1(HWND hDlg,int nIDDlgItem)
{
	 /*Axis*/
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Cintura");	//0
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Hombro");		//1
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Codo");		//2
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Pitch");		//3
	/*Axis Cintura por default*/
	SendDlgItemMessage(hDlg,nIDDlgItem,CB_SETCURSEL, 0,0);				


}

/*****************************************************************************************/
void CBoxTRACE(HWND hDlg)
{ 
	HWND hControl = GetDlgItem(hDlg,IDC_TIMEPM);

	/*Axis*/
	FillAxisTracePMD1(hDlg,IDC_AXISTRACE);
	FillAxisTracePMD1(hDlg,IDC_AXISTRACE2);
	FillAxisTracePMD1(hDlg,IDC_AXISTRACE3);
	FillAxisTracePMD1(hDlg,IDC_AXISTRACE4);
	/*Tracetime*/
	SetDlgItemInt(hDlg,IDC_TRACETIME,200,FALSE);
    /*IDC_PMDT1*/														
	CBoxTraceCommand(hDlg,IDC_PMDT1);
	/*IDC_PMDT2*/														
	CBoxTraceCommand(hDlg,IDC_PMDT2);
	/*IDC_PMDT3*/														
	CBoxTraceCommand(hDlg,IDC_PMDT3);
	/*IDC_PMDT4*/														
	CBoxTraceCommand(hDlg,IDC_PMDT4);
	/*IDC_PMDT5*/														
	CBoxTraceCommand(hDlg,IDC_PMDT5);
	/*IDC_PMDT6*/					
	CBoxTraceCommand(hDlg,IDC_PMDT6);
	/*IDC_PMDT7*/					
	CBoxTraceCommand(hDlg,IDC_PMDT7);
	/*IDC_PMDT8*/					
	CBoxTraceCommand(hDlg,IDC_PMDT8);
}

/*****************************************************************************************/
void CBoxTraceCommand(HWND hDlg,int nIDDlgItem)
{

	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Error de Posición");	     //0 - 1
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Posición Comandada");	     //1 - 2	
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Velocidad Comandada");	 //2 - 3	 
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Aceleración Comandada");   //3 - 3	
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Posicion Censada");		 //4 - 5
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Velocidad Censada");       //5 - 6	
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Registro EventStatus");	 //6 - 12
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Registro ActivityStatus"); //7 - 13
	SendDlgItemMessage(hDlg,nIDDlgItem, CB_ADDSTRING, 0, (LPARAM)"Registro SignalStatus");	 //8 - 14
	
}

/*****************************************************************************************/
/*Obtien los campos del usuario y los guarda en una estructura tipo tCommand del packetIO*/
void GetCBoxTRACE(HWND hDlg,tCommand * ComandTrace,double * GraficSampletime)
{
	int	Opcion;
	double PMDreal;
	double UserSampleTime;

	
	//Max 39321
	UserSampleTime=GetDlgItemInt(hDlg,IDC_TRACETIME,NULL,FALSE);
	if(UserSampleTime>39321)
	{	
		MessageBox(hDlg, "Se produjo un desbordamiento en el tiempo de muestreo. Ingrese Valor Menor", "Advertencia", MB_OK);
	}
	else
	{

		UserSampleTime=UserSampleTime*1e-3;
		UserSampleTime=UserSampleTime/SAMPLETIME;
		ComandTrace->TraceTime=(uint16)ceil(UserSampleTime);
		PMDreal=ComandTrace->TraceTime*SAMPLETIME;
		*GraficSampletime=PMDreal;
		PMDreal=PMDreal*1000;
		setlong(hDlg,IDC_PMDregistro,PMDreal);
	}
	if(SendDlgItemMessage(hDlg,IDC_TRACETRAJECT,BM_GETCHECK,0,0) == BST_CHECKED)
		ComandTrace->TraceTraj=1;
	else
		ComandTrace->TraceTraj=0;
	
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace1,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable1*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT1,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[0].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[0].TraceVar=Opcion+6;
		}
		Opcion= SendDlgItemMessage(hDlg,IDC_AXISTRACE,CB_GETCURSEL,0,0);
		ComandTrace->Tracecomand[0].AxisTrace=Opcion;
	}
	else
	{
		ComandTrace->Tracecomand[0].TraceVar=-1;
		ComandTrace->Tracecomand[0].AxisTrace=-1;
	}
		
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace2,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable2*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT2,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[1].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[1].TraceVar=Opcion+6;
		}
		Opcion= SendDlgItemMessage(hDlg,IDC_AXISTRACE2,CB_GETCURSEL,0,0);
		ComandTrace->Tracecomand[1].AxisTrace=Opcion;

	}
	else
	{
		ComandTrace->Tracecomand[1].TraceVar=-1;
		ComandTrace->Tracecomand[1].AxisTrace=-1;
	}
		
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace3,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable3*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT3,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[2].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[2].TraceVar=Opcion+6;
		}
		Opcion= SendDlgItemMessage(hDlg,IDC_AXISTRACE3,CB_GETCURSEL,0,0);
		ComandTrace->Tracecomand[2].AxisTrace=Opcion;
	}
	else
	{
		ComandTrace->Tracecomand[2].TraceVar=-1;
		ComandTrace->Tracecomand[2].AxisTrace=-1;
	}

	if(SendDlgItemMessage(hDlg,IDC_PMDTrace4,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable4*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT4,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[3].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[3].TraceVar=Opcion+6;
		}
		Opcion= SendDlgItemMessage(hDlg,IDC_AXISTRACE4,CB_GETCURSEL,0,0);
		ComandTrace->Tracecomand[3].AxisTrace=Opcion;

	}
	else
	{
		ComandTrace->Tracecomand[3].TraceVar=-1;
		ComandTrace->Tracecomand[3].AxisTrace=-1;
	}

		if(SendDlgItemMessage(hDlg,IDC_PMDTrace5,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable5*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT5,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[4].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[4].TraceVar=Opcion+6;
		}
		ComandTrace->Tracecomand[4].AxisTrace=AxisRoll;

	}
	else
	{
		ComandTrace->Tracecomand[4].TraceVar=-1;
		ComandTrace->Tracecomand[4].AxisTrace=-1;
	}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace6,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable6*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT6,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[5].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[5].TraceVar=Opcion+6;
		}
		ComandTrace->Tracecomand[5].AxisTrace=AxisRoll;

	}
	else
	{
		ComandTrace->Tracecomand[5].TraceVar=-1;
		ComandTrace->Tracecomand[5].AxisTrace=-1;
	}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace7,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable7*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT7,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[6].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[6].TraceVar=Opcion+6;
		}
		ComandTrace->Tracecomand[6].AxisTrace=AxisRoll;

	}
	else
	{
		ComandTrace->Tracecomand[6].TraceVar=-1;
		ComandTrace->Tracecomand[6].AxisTrace=-1;
	}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace8,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		/*Comando Variable8*/
		Opcion= SendDlgItemMessage(hDlg,IDC_PMDT4,CB_GETCURSEL,0,0);
		/*La variable esta seleccionada pero tiene datos*/
		if(Opcion!=-1) 
		{
			if(Opcion<6) 
				ComandTrace->Tracecomand[7].TraceVar=Opcion+1;
			else
				ComandTrace->Tracecomand[7].TraceVar=Opcion+6;
		}
		ComandTrace->Tracecomand[7].AxisTrace=AxisRoll;

	}
	else
	{
		ComandTrace->Tracecomand[7].TraceVar=-1;
		ComandTrace->Tracecomand[7].AxisTrace=-1;
	}

}
/*****************************************************************************************
Devuelve el numero total de variables configuras, y ademas actualiza la cantidad de 
variables configuradas tanto para PMD1 como para PMD2
/*****************************************************************************************/
int GetNvariables(HWND hDlg,tCommand ComandTrace,int * PMD1NVariables,int* PMD2NVariables)
{
	int NVariables=0;

	/*Se Inicializa el conteo*/
	* PMD1NVariables=0;
	* PMD2NVariables=0;

	/*Para PMD1 -> Esta Selecionada la Variables*/
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace1,BM_GETCHECK,0,0) == BST_CHECKED)
		/*Tiene un Comando Valido*/
		if(ComandTrace.Tracecomand[0].TraceVar>0 && ComandTrace.Tracecomand[0].TraceVar<=14)
		{
			NVariables++;
			* PMD1NVariables=* PMD1NVariables+1;
		}
	 if(SendDlgItemMessage(hDlg,IDC_PMDTrace2,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[1].TraceVar>0 && ComandTrace.Tracecomand[1].TraceVar<=14)
		{
			NVariables++;
			* PMD1NVariables=* PMD1NVariables+1;
		}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace3,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[2].TraceVar>0 && ComandTrace.Tracecomand[2].TraceVar<=14)
		{
			NVariables++;
			* PMD1NVariables=* PMD1NVariables+1;
		}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace4,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[3].TraceVar>0 && ComandTrace.Tracecomand[3].TraceVar<=14)
		{
			NVariables++;
			* PMD1NVariables=* PMD1NVariables+1;
		}

	/* Para PMD2*/
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace5,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[4].TraceVar>0 && ComandTrace.Tracecomand[4].TraceVar<=14)
		{
			NVariables++;
			* PMD2NVariables=* PMD2NVariables+1;
		}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace6,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[5].TraceVar>0 && ComandTrace.Tracecomand[5].TraceVar<=14)
		{
			NVariables++;
			* PMD2NVariables=* PMD2NVariables+1;
		}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace7,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[6].TraceVar>0 && ComandTrace.Tracecomand[6].TraceVar<=14)
		{
			NVariables++;
			* PMD2NVariables=* PMD2NVariables+1;
		}
	if(SendDlgItemMessage(hDlg,IDC_PMDTrace8,BM_GETCHECK,0,0) == BST_CHECKED)
		if(ComandTrace.Tracecomand[7].TraceVar>0 && ComandTrace.Tracecomand[7].TraceVar<=14)
		{
			NVariables++;
			* PMD2NVariables=* PMD2NVariables+1;
		}

	return NVariables;

}

/*****************************************************************************************/
void Habilitar_camposSetTRACE(HWND hDlg,int op,int habilitar)
{

	HWND  TRACEcmd1= GetDlgItem (hDlg,IDC_PMDT1);
	HWND  TRACEcmd2= GetDlgItem (hDlg,IDC_PMDT2);
	HWND  TRACEcmd3= GetDlgItem (hDlg,IDC_PMDT3);
	HWND  TRACEcmd4= GetDlgItem (hDlg,IDC_PMDT4);
	HWND  TRACEcmd5= GetDlgItem (hDlg,IDC_PMDT5);
	HWND  TRACEcmd6= GetDlgItem (hDlg,IDC_PMDT6);
	HWND  TRACEcmd7= GetDlgItem (hDlg,IDC_PMDT7);
	HWND  TRACEcmd8= GetDlgItem (hDlg,IDC_PMDT8);
	HWND  TRACEAxis1= GetDlgItem (hDlg,IDC_AXISTRACE);
	HWND  TRACEAxis2= GetDlgItem (hDlg,IDC_AXISTRACE2);
	HWND  TRACEAxis3= GetDlgItem (hDlg,IDC_AXISTRACE3);
	HWND  TRACEAxis4= GetDlgItem (hDlg,IDC_AXISTRACE4);
	

	switch(op)
	{
	    case IDC_PMDTrace1:
			if (habilitar==1)
			{
			    EnableWindow (TRACEcmd1, TRUE);
				EnableWindow (TRACEAxis1, TRUE);
			}
			else
			{
				EnableWindow (TRACEcmd1,FALSE);
				EnableWindow (TRACEAxis1,FALSE);
			}
		break;
        case  IDC_PMDTrace2:
			if (habilitar==1)
			{
			    EnableWindow (TRACEcmd2, TRUE);
				EnableWindow (TRACEAxis2, TRUE);
			}
			else
			{
				EnableWindow (TRACEcmd2,FALSE);
				EnableWindow (TRACEAxis2,FALSE);
			}

		break;
        case  IDC_PMDTrace3:
			if (habilitar==1)
			{
			    EnableWindow (TRACEcmd3, TRUE);
				EnableWindow (TRACEAxis3, TRUE);
			}
			else
			{
				EnableWindow (TRACEcmd3,FALSE);
				EnableWindow (TRACEAxis3,FALSE);
			}

        break;
        case  IDC_PMDTrace4:	
			if (habilitar==1)
			{
			    EnableWindow (TRACEcmd4, TRUE);
				EnableWindow (TRACEAxis4, TRUE);
			}
			else
			{
				EnableWindow (TRACEcmd4,FALSE);
				EnableWindow (TRACEAxis4,FALSE);
			}

        break;
        case  IDC_PMDTrace5:	
			if (habilitar==1)			
			    EnableWindow (TRACEcmd5, TRUE);
			else
				EnableWindow (TRACEcmd5,FALSE);
        break;
        case  IDC_PMDTrace6:	
			if (habilitar==1)
			    EnableWindow (TRACEcmd6, TRUE);
			else
				EnableWindow (TRACEcmd6,FALSE);
        break;
        case  IDC_PMDTrace7:	
			if (habilitar==1)
			    EnableWindow (TRACEcmd7, TRUE);
			else
				EnableWindow (TRACEcmd7,FALSE);
        break;
        case  IDC_PMDTrace8:	
			if (habilitar==1)
			    EnableWindow (TRACEcmd8, TRUE);
			else
				EnableWindow (TRACEcmd8,FALSE);
        break;
	}
}

/*****************************************************************************************/
void manejador_camposSetTRACE(HWND hDlg,int op)
{
	if(SendDlgItemMessage(hDlg,op,BM_GETCHECK,0,0) == BST_CHECKED)
		Habilitar_camposSetTRACE(hDlg,op,1);
	else
		Habilitar_camposSetTRACE(hDlg,op,0);
}

/*****************************************************************************************/
void FixVector(tCommand * ComandTrace)
{
	int i;
	int NewIndex=0;
	
	for(i=0;i<8;i++)
	{
		if(ComandTrace->Tracecomand[i].TraceVar!=-1)
		{
			ComandTrace->Tracecomand[NewIndex].TraceVar=ComandTrace->Tracecomand[i].TraceVar;
			ComandTrace->Tracecomand[NewIndex].AxisTrace=ComandTrace->Tracecomand[i].AxisTrace;
			if(i!=NewIndex)
			{
				ComandTrace->Tracecomand[i].TraceVar=-1;
				ComandTrace->Tracecomand[i].AxisTrace=-1;
			}
			NewIndex++;
		}

	}
}	
	
