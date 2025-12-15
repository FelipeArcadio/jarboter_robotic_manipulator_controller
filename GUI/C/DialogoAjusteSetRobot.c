#include "DialogoAjusteSetRobot.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************************/
/*Llena con las posibles opciones los campos de la interfaz que configura el robot*/
void CBoxAJUSTE(HWND hDlg,int uPasos,int MotorPower,int Switch_Int)
{

	HWND   hControl = GetDlgItem(hDlg,Switch_Int);
	int i,IntuPasos=0;
	char Texto[10]="0";
			
	/*uPasos*/
	for(i=0;i<9;i++)
	{
		IntuPasos=(int)pow(2,i);
		itoa(IntuPasos,Texto,10);
		SendDlgItemMessage(hDlg,uPasos, CB_ADDSTRING, 0, (LPARAM)Texto);				
	}
	SendDlgItemMessage(hDlg,uPasos,CB_SETCURSEL, 5,0);				
	/*Motor Power*/
	for(i=1;i<101;i++)
	{
		itoa(i,Texto,10);
		SendDlgItemMessage(hDlg,MotorPower, CB_ADDSTRING, 0, (LPARAM)Texto);				
	}
	SendDlgItemMessage(hDlg,MotorPower,CB_SETCURSEL,99,0);				
	/*Switch_Int*/
	SendMessage(hControl,BM_SETCHECK,BST_CHECKED,0);
}

/*****************************************************************************************/
/*Llena con tAxisSettings rxAxisSettings[DOF] la interfaz que configura el robot, rxAxisSettings[DOF] es 
modifica por el USB*/
void SetCBoxAJUSTE_USB(HWND hDlg,tAxisSettings * tAxisSettingUSB,int uPasos,int MotorPower,int Switch_Int,int Encoder,int ErrorLimit,int TrackWindow, int SettleWindow,int SettleTime)
{

	HWND   hControl;
	int IndexuPasos,uPasosPMD,temp;
	int uPasosTable[9]={1,2,4,8,16,32,64,128,256};
	char Texto[10]="0";
	
			

	/*uPasos*/
	uPasosPMD=tAxisSettingUSB->ustepsPerStep;
	for(IndexuPasos=0;IndexuPasos<9;IndexuPasos++)
	{
		if(uPasosTable[IndexuPasos]==uPasosPMD)
			{	
				temp=IndexuPasos;
				break;
			}
	}
	SendDlgItemMessage(hDlg,uPasos,CB_SETCURSEL,temp,0);				
	/*Motor Power*/
	MotorPower=tAxisSettingUSB->motorPower;
	itoa(MotorPower,Texto,10);
	SendDlgItemMessage(hDlg,MotorPower, CB_ADDSTRING, 0, (LPARAM)Texto);				
	/*Switch_Int*/
	hControl = GetDlgItem(hDlg,Switch_Int);
	if(tAxisSettingUSB->interruptEnable==TRUE)
	SendMessage(hControl,BM_SETCHECK,BST_CHECKED,0);
	else
	SendMessage(hControl,BM_SETCHECK,BST_UNCHECKED,0);
	/*Encoder*/
	hControl = GetDlgItem(hDlg,Encoder);
	if(tAxisSettingUSB->encoderEnable==TRUE)
	{
	
		SendMessage(hControl,BM_SETCHECK,BST_CHECKED,0);
		manejador_camposSetRobot(hDlg,Encoder);
		SetDlgItemInt(hDlg,ErrorLimit,tAxisSettingUSB->ErrorLimit,FALSE);
		SetDlgItemInt(hDlg,TrackWindow,tAxisSettingUSB->TrackingWindow,FALSE);
		SetDlgItemInt(hDlg,SettleWindow,tAxisSettingUSB->SettleWindow,FALSE);
		SetDlgItemInt(hDlg,SettleTime,tAxisSettingUSB->SettleTime,FALSE);

	}
	else
	SendMessage(hControl,BM_SETCHECK,BST_UNCHECKED,0);
	

	
	
}

/*****************************************************************************************/
/*Obtiene las opciones ingresadas por el usuario y la copia en una estructura tAxisSettings*/
void GetCBoxAJUSTE(HWND hDlg,tAxisSettings * tAxisSetting,int uPasos,int MotorPower,int Switch_Int,int Encoder,int ErrorLimit,int TrackWindow, int SettleWindow,int SettleTime)
{

	HWND   hControl = GetDlgItem(hDlg,Switch_Int);
	HWND   hControl1 = GetDlgItem(hDlg,Encoder);
	int PointIndex=0;
	char Texto[10]="0";
			
	/*uPasos*/
	PointIndex= SendDlgItemMessage(hDlg,uPasos,CB_GETCURSEL,0,0);
	tAxisSetting->ustepsPerStep=(int16)pow(2,PointIndex);
	/*Motor Power*/
	PointIndex= SendDlgItemMessage(hDlg,MotorPower,CB_GETCURSEL,0,0);
	PointIndex++;
	tAxisSetting->motorPower=PointIndex;
	/*Switch_Int*/
	if(SendMessage(hControl,BM_GETCHECK,0,0) == BST_CHECKED)
		tAxisSetting->interruptEnable=TRUE;
	else
	    tAxisSetting->interruptEnable=FALSE;
	/*Encoder*/
	if(SendMessage(hControl1,BM_GETCHECK,0,0) == BST_CHECKED)
		tAxisSetting->encoderEnable=TRUE;
	else
	    tAxisSetting->encoderEnable=FALSE;
}

/*****************************************************************************************/
/*Op debe ser der igual a el checkbox*/
void Habilitar_camposSetRobot(HWND hDlg,int op,int habilitar)
{

	HWND  CINERROR= GetDlgItem (hDlg,IDC_CBOXERROR);
	HWND  CINTRACKWIN= GetDlgItem (hDlg,IDC_CBOXTRACKWIN);
	HWND  CINSETTLEW= GetDlgItem (hDlg,IDC_CBOXSETTLEW);
	HWND  CINSETTLET= GetDlgItem (hDlg,IDC_CBOXSETTLET);

	HWND  HOMERROR= GetDlgItem (hDlg,IDC_CBOXERROR2);
	HWND  HOMTRACKWIN= GetDlgItem (hDlg,IDC_CBOXTRACKWIN2);
	HWND  HOMSETTLEW= GetDlgItem (hDlg,IDC_CBOXSETTLEW2);
	HWND  HOMSETTLET= GetDlgItem (hDlg,IDC_CBOXSETTLET2);

	HWND  CODERROR= GetDlgItem (hDlg,IDC_CBOXERROR3);
	HWND  CODTRACKWIN= GetDlgItem (hDlg,IDC_CBOXTRACKWIN3);
	HWND  CODSETTLEW= GetDlgItem (hDlg,IDC_CBOXSETTLEW3);
	HWND  CODSETTLET= GetDlgItem (hDlg,IDC_CBOXSETTLET3);

	HWND  PITCHERROR= GetDlgItem (hDlg,IDC_CBOXERROR4);
	HWND  PITCHTRACKWIN= GetDlgItem (hDlg,IDC_CBOXTRACKWIN4);
	HWND  PITCHSETTLEW= GetDlgItem (hDlg,IDC_CBOXSETTLEW4);
	HWND  PITCHSETTLET= GetDlgItem (hDlg,IDC_CBOXSETTLET4);

	HWND  ROLLERROR= GetDlgItem (hDlg,IDC_CBOXERROR5);
	HWND  ROLLTRACKWIN= GetDlgItem (hDlg,IDC_CBOXTRACKWIN5);
	HWND  ROLLSETTLEW= GetDlgItem (hDlg,IDC_CBOXSETTLEW5);
	HWND  ROLLSETTLET= GetDlgItem (hDlg,IDC_CBOXSETTLET5);

	switch(op)
	{
	    case IDC_ENCODER_EN1:
			if (habilitar==1)
			{
			    EnableWindow (CINERROR, TRUE);
		        EnableWindow (CINTRACKWIN, TRUE);
			    EnableWindow (CINSETTLEW, TRUE);
				EnableWindow (CINSETTLET, TRUE);
			}
			else
            {
                EnableWindow (CINERROR, FALSE);
		        EnableWindow (CINTRACKWIN, FALSE);
			    EnableWindow (CINSETTLEW, FALSE);
				EnableWindow (CINSETTLET, FALSE);
			}
		break;
        case  IDC_ENCODER_EN2:
			if (habilitar==1)
			{
			    EnableWindow (HOMERROR,TRUE);
		        EnableWindow (HOMTRACKWIN,TRUE);
			    EnableWindow (HOMSETTLEW,TRUE);
				EnableWindow (HOMSETTLET,TRUE);
			}
            else
			{
                EnableWindow (HOMERROR,FALSE);
		        EnableWindow (HOMTRACKWIN,FALSE);
			    EnableWindow (HOMSETTLEW,FALSE);
				EnableWindow (HOMSETTLET,FALSE);
			}
		break;
        case  IDC_ENCODER_EN3:
			if (habilitar==1)
			{
			    EnableWindow (CODERROR,TRUE);
		        EnableWindow (CODTRACKWIN,TRUE);
			    EnableWindow (CODSETTLEW,TRUE);
				EnableWindow (CODSETTLET,TRUE);
			}
            else
			{
                EnableWindow (CODERROR,FALSE);
		        EnableWindow (CODTRACKWIN,FALSE );
			    EnableWindow (CODSETTLEW,FALSE);
				EnableWindow (CODSETTLET,FALSE);
			}
        break;
        case  IDC_ENCODER_EN4:	
			if (habilitar==1)
			{
			    EnableWindow (PITCHERROR, TRUE);
		        EnableWindow (PITCHTRACKWIN, TRUE);
			    EnableWindow (PITCHSETTLEW, TRUE);
				EnableWindow (PITCHSETTLET, TRUE);
			}
            else
			{
                EnableWindow (PITCHERROR, FALSE);
		        EnableWindow (PITCHTRACKWIN, FALSE);
			    EnableWindow (PITCHSETTLEW, FALSE);
				EnableWindow (PITCHSETTLET, FALSE);
			}
        break;
        case  IDC_ENCODER_EN5:	
			if (habilitar==1)
			{
			    EnableWindow (ROLLERROR, TRUE);
		        EnableWindow (ROLLTRACKWIN, TRUE );
			    EnableWindow (ROLLSETTLEW, TRUE);
				EnableWindow (ROLLSETTLET, TRUE);
			}
			else
			{
                EnableWindow (ROLLERROR, FALSE);
		        EnableWindow (ROLLTRACKWIN, FALSE );
			    EnableWindow (ROLLSETTLEW, FALSE);
				EnableWindow (ROLLSETTLET, FALSE);
			}
        break;
	}
}

/*****************************************************************************************/
void manejador_camposSetRobot(HWND hDlg,int op)
{
	if(SendDlgItemMessage(hDlg,op,BM_GETCHECK,0,0) == BST_CHECKED)
		Habilitar_camposSetRobot(hDlg,op,1);
	else
		Habilitar_camposSetRobot(hDlg,op,0);
}

