#include "DialogoRTC.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>



/*****************************************************************************************/
void cargoBotonRTC(HWND hDlg,HINSTANCE hInstance)
{
	/* HANDLE GRAFICO DE  LOS BOTONES*/
	HWND  GET_ALARMA=GetDlgItem (hDlg,IDC_READALARMA);
    HWND  SET_ALARMA=GetDlgItem (hDlg,IDC_SETALARMA);
	HWND  GET_HORA=GetDlgItem (hDlg,IDC_LLEERHORA);
    HWND  SET_HORA=GetDlgItem (hDlg,IDC_SETHORA);


    /*Creo un asiciacion entre al imagen y el boton 
	BM_SETIMAGE no funciona sin activar  BS_ICON or BS_BITMAP en las propiedades de boton */
	SendMessage(GET_ALARMA, BM_SETIMAGE,IMAGE_BITMAP,
     /*Cargo el BMP a el boton*/
    (LPARAM)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GET)));        

	SendMessage(SET_ALARMA, BM_SETIMAGE,IMAGE_BITMAP,
    (LPARAM)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SET)));        

	SendMessage(GET_HORA, BM_SETIMAGE,IMAGE_BITMAP,
    (LPARAM)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GET)));   
	
	SendMessage(SET_HORA, BM_SETIMAGE,IMAGE_BITMAP,
    (LPARAM)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SET)));   
}

/*****************************************************************************************/
void CBoxRTC(HWND hDlg)
{
	HWND hControl = GetDlgItem(hDlg,IDC_TIMEPM);
	char dia[4]="";
	char yeartxt[10]="";
	int i,year=2000;

    /*Tareas programadas*/
	SendDlgItemMessage(hDlg,IDC_COMBO_TAREA_RTC, CB_ADDSTRING, 0, (LPARAM)"Open Gripper");				//0
	SendDlgItemMessage(hDlg,IDC_COMBO_TAREA_RTC, CB_ADDSTRING, 0, (LPARAM)"Close Gripper");				//0
	SendDlgItemMessage(hDlg,IDC_COMBO_TAREA_RTC, CB_ADDSTRING, 0, (LPARAM)"Ejecutar Poses");				//0
	SendDlgItemMessage(hDlg,IDC_COMBO_TAREA_RTC, CB_ADDSTRING, 0, (LPARAM)"Ejecutar Trayectoria");				//0
    /*Mes Alarmar*/														
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Enero");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Febrero");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Marzo");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Abril");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Mayo");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Junio");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Julio");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Agosto");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Septiembre");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Octubre");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Noviembre");				//0
	SendDlgItemMessage(hDlg,IDC_MESALARMA, CB_ADDSTRING, 0, (LPARAM)"Diciembre");				//0
	/*Dia Alarma*/
	memset(dia,0,4);
	for(i=1;i<31;i++)
	{
		itoa(i,dia,10);
		SendDlgItemMessage(hDlg,IDC_DIAALARMA, CB_ADDSTRING, 0, (LPARAM)dia);				//0
	}
    /*Mes time*/
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Enero");				//0
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Febrero");			//1
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Marzo");				//2
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Abril");				//3
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Mayo");				//4
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Junio");				//5
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Julio");				//6
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Agosto");			//7
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Septiembre");		//8
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Octubre");			//9
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Noviembre");			//10
	SendDlgItemMessage(hDlg,IDC_MESTIME, CB_ADDSTRING, 0, (LPARAM)"Diciembre");			//11
    /*dia time 1-31*/
	memset(dia,0,4);
	for(i=1;i<32;i++)
	{
		itoa(i,dia,10);
		SendDlgItemMessage(hDlg,IDC_DIATIME, CB_ADDSTRING, 0, (LPARAM)dia);				//0
	}
    /*dia de la semana*/
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Domingo");				//0
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Lunes");				//0
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Martes");				//0
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Miercoles");				//0
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Jueves");				//0
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Viernes");				//0
	SendDlgItemMessage(hDlg,IDC_DIASEMANA, CB_ADDSTRING, 0, (LPARAM)"Sabado");				//0
	/*AÑO*/
	memset(yeartxt,0,10);
	for(i=1;i<100;i++)
	{
        year++;
		itoa(year,yeartxt,10);
		SendDlgItemMessage(hDlg,IDC_YEARTIME, CB_ADDSTRING, 0, (LPARAM)yeartxt);				//0	SendDlgItemMessage(hDlg,IDC_DIATIME, CB_ADDSTRING, 0, (LPARAM)dia);				//0
	}
}

/*****************************************************************************************/
/*Obtiene la hora desde la interfaz de usuario*/
void SetFechaVisual(HWND hDlg,tFecha* Fecha)
{

	char* Horatxt;
	char YearChar[5]="";//inicializacion de la cadana
	int linetextlen,hh,mm,ss,year,dweek;
	int i=0,Opcion;

	/*Retorna la cantidad de letras escritas*/
	linetextlen = SendDlgItemMessage(hDlg,IDC_CLOCKTIME,EM_LINELENGTH,50, 0L);         
	Horatxt=(char *)declarar(linetextlen);
	/*Lee la cantidad de linetextlen caracteres */
	GetDlgItemText(hDlg,IDC_CLOCKTIME,Horatxt,linetextlen+1);
	GetHORATXT(Horatxt,&hh,&mm,&ss);   
	/*---------------------*/
	if(SendDlgItemMessage(hDlg,IDC_TIMEPM,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		Fecha->hr=hh;
		Fecha->hr= Fecha->hr | 0x20;
	}
	else
		Fecha->hr=hh;
	Fecha->mn=mm;
	Fecha->sc=ss;
	/*FECHA*/
	/*MES*/
	Opcion= SendDlgItemMessage(hDlg,IDC_MESTIME,CB_GETCURSEL,0,0);
	//Modificado david 2013, revisar
	if(Opcion<10)
	Fecha->mo=Opcion+1;
	else
	Fecha->mo=Opcion+7;
	/*DIA SEMANA*/
	dweek=SendDlgItemMessage(hDlg,IDC_DIASEMANA,CB_GETCURSEL,0,0);
	Fecha->dw=dweek;
	/*Dia 1-31*/
	Opcion= SendDlgItemMessage(hDlg,IDC_DIATIME,CB_GETCURSEL,0,0);
	Opcion++;	
	Fecha->dt=int2bcd((uint8)Opcion); 
	/*YEAR 0-99*/
	year=GetDlgItemInt(hDlg,IDC_YEARTIME,NULL,FALSE); 
	year=year-2000;		
	Fecha->yr=int2bcd((uint8)year);
	Fecha->y2k=0x20;			
}


/*****************************************************************************************/
/*Coloca la Hora leida desde USB*/
void getFechaVisual(HWND hDlg)
{

	HWND   hControlAM = GetDlgItem(hDlg,IDC_TIMEAM);
	HWND   hControlPM= GetDlgItem(hDlg,IDC_TIMEPM);
	extern tFecha   rxFecha;
	char Horacompleta[18]="";			  //inicializacion de la cadana
	int		integer;

	/*Mes-DW-Dia-Año*/
	/*Escribo el mes*/
	//Modificado david 2013, revisar
	if(rxFecha.mo<10)
		rxFecha.mo=rxFecha.mo - 1;
	else
		rxFecha.mo=rxFecha.mo - 7;
	SendDlgItemMessage(hDlg,IDC_MESTIME,CB_SETCURSEL,rxFecha.mo, 0);
	SendDlgItemMessage(hDlg,IDC_DIASEMANA,CB_SETCURSEL,rxFecha.dw, 0);
	/*Convierto de BCD a Decimal*/
	
	integer=bcd2int(rxFecha.dt);
	integer--;
	/*Escribo el dia*/
	SendDlgItemMessage(hDlg,IDC_DIATIME,CB_SETCURSEL,integer, 0);
	/*Convierto de BCD a Decimal*/
	integer=bcd2int(rxFecha.yr);
	integer=integer+2000;
	
	/*Escribo la fecha*/
	SetDlgItemInt(hDlg,IDC_YEARTIME,integer,FALSE);
	/*Coloco AM-PM*/
	if((rxFecha.hr & 0x20) != 0)			
	{
		SendMessage(hControlPM,BM_SETCHECK,BST_CHECKED,0);
		rxFecha.hr=rxFecha.hr & 0x1F;
	}
	else //AM
		SendMessage(hControlAM,BM_SETCHECK,BST_CHECKED,0);
	memset(Horacompleta,0,18);
	SetFechaTXT(Horacompleta,rxFecha);
	SetDlgItemText(hDlg,IDC_CLOCKTIME,Horacompleta);
}

/*****************************************************************************************/
/*Obtiene la Hora desde rxFecha y guarda en Horacompleta*/
void SetFechaTXT(char* Horacompleta, tFecha rxFecha)
{
	char Time[5]="";
	char Minutos[5]="";
	char Seg[5]="";
	char dos[2]=":";
	char temChar[5]="";
	/*Inicializo las cadenas */
	memset(Time,0,5);
	memset(Minutos,0,5);
	memset(Seg,0,5);
	/*Convierto desde decimal a BCD txt*/
	BCDtoCHAR(rxFecha.hr,Time);
	BCDtoCHAR(rxFecha.mn,Minutos);
    BCDtoCHAR(rxFecha.sc,Seg);
    strcat(Horacompleta,Time);    
	strcat(Horacompleta,dos);    
	strcat(Horacompleta,Minutos);    
	strcat(Horacompleta,dos);    
	strcat(Horacompleta,Seg);    
	/*Escribo la Hora en formato char */
}

/*****************************************************************************************/
/*Obtiene la Hora desde el texto y la tranforma en formato BCD guardandolo en int*/
void GetHORATXT(char* Horatxt,int* hh,int* mm,int* ss)
{
	char tempChar[3]="";
	int temp;
		
	/*TIEMPO*/		
	/*Hora*/
	memset(tempChar,0,2);
	tempChar[0]=Horatxt[0];
	temp=atoi(tempChar);
    temp=temp<<4;    
	memset(tempChar,0,2);
	tempChar[0]=Horatxt[1];
	*hh=atoi(tempChar);
	*hh=*hh+temp;
    /*Minutos*/
	memset(tempChar,0,2);
	tempChar[0]=Horatxt[3];
	temp=atoi(tempChar);
    temp=temp<<4;    
	memset(tempChar,0,2);
	tempChar[0]=Horatxt[4];
	*mm=atoi(tempChar);
	*mm=*mm+temp;
	/*Segundos*/
    memset(tempChar,0,2);
	tempChar[0]=Horatxt[6];
	temp=atoi(tempChar);
    temp=temp<<4;    
	memset(tempChar,0,2);
	tempChar[0]=Horatxt[7];
	*ss=atoi(tempChar);
	*ss=*ss+temp;

}

/*****************************************************************************************/
void BCDtoCHAR(int BCD,char* Cadena)
{
	int temp;
	char digito1[5]="";			  //inicializacion de la cadana
	char digito2[5]="";


	memset(digito1,0,2);
	memset(digito2,0,2);

	temp=BCD&0x0F;
	itoa(temp,digito1,10);
	temp=BCD&0xF0;
	temp=temp>>4;
	itoa(temp,digito2,10);
	strcat(digito2,digito1);    
	strcpy (Cadena,digito2); 
}

/*****************************************************************************************/
void SetAlarma(HWND hDlg,tAlarm * txAlarma)
{
    int linetextlen,hh,mm,ss;
	int Opcion;
	char* Horatxt;

	/*MES 1-12*/
	Opcion= SendDlgItemMessage(hDlg,IDC_MESALARMA,CB_GETCURSEL,0,0);
	/*Convierto a el formato BCD*/
	Opcion++;
	txAlarma->MOA0=int2bcd((uint8)Opcion);
	if(txAlarma->MOA0!=00)
		txAlarma->MOA0=txAlarma->MOA0 | 0x80;						//0x80 habilita el mes
	/*DIA 1-31*/
	Opcion= SendDlgItemMessage(hDlg,IDC_DIAALARMA,CB_GETCURSEL,0,0);
	/*Convierto a el formato BCD*/
	Opcion++;	
	txAlarma->DTA0=int2bcd((uint8)Opcion);
	if(txAlarma->DTA0!=00)
		txAlarma->DTA0=txAlarma->DTA0 | 0x80;						//0x80 habilita el dia
	/*HORA*/
	/*Retorna la cantidad de letras escritas*/
	linetextlen = SendDlgItemMessage(hDlg,IDC_RTCTIME,EM_LINELENGTH,50, 0L);         
	Horatxt=(char *)declarar(linetextlen);
	/*Lee la cantidad de linetextlen caracteres */
	GetDlgItemText(hDlg,IDC_RTCTIME,Horatxt,linetextlen+1);
	/*Leo la hora desde el texto en formato BCD*/
	GetHORATXT(Horatxt,&hh,&mm,&ss);
	/*Configurp las variables*/
	txAlarma->HRA0=(int8)hh;
	if(txAlarma->HRA0!=00)
		txAlarma->HRA0=txAlarma->HRA0 | 0x80;					//0x80 habilita la Hora
	/*Selecciona AM-PM*/
	if(SendDlgItemMessage(hDlg,IDC_PMALARMA,BM_GETCHECK,0,0) == BST_CHECKED)
		txAlarma->HRA0= txAlarma->HRA0 | 0x20;
	/*MINUTOS 0-59*/
	txAlarma->MNA0=(uint8)mm;
	if(txAlarma->MNA0!=00)
		txAlarma->MNA0=txAlarma->MNA0 | 0x80;						//0x80 habilita Los minutos
	/*Segundos*/
	txAlarma->SCA0=(uint8)ss;
	if(txAlarma->SCA0!=00)
	txAlarma->SCA0=txAlarma->SCA0 | 0x80;						//0x80 habilita Los segundos
	/*Siempre sera no PERIODICA*/
	txAlarma->PER=0;
	/*TAREA*/
	txAlarma->TASK=(uint8)SendDlgItemMessage(hDlg,IDC_COMBO_TAREA_RTC,CB_GETCURSEL,0,0);
}