#include <math.h>
#include <windows.h>			//funciones basicas de ventanas graficas
#include <windowsx.h>			//funciones extendidas de ventanas graficas
#include <mem.h>
#include <stdio.h>				//Entrada y salida estandar
#include <stdlib.h>     
#include <string.h>				//manejo de caracteres
#include <commctrl.h>           // namejo de controles graficos 

#include "Interfaz_grafica.h"   
#include "resource.h"
#include "types.h"
#include "robotData.h"   
#include "packetIO.h"    
#include "DialogoAjusteSetRobot.h"
#include "DialogoRTC.h"
#include "DialogoTechPendant.h"
#include "DialogoTRACE.h"
#include "DialogoTrajecEnvia.h"
#include "menuFunc.h"   
#include "USBfunc.h"   
#include "Procesamiento_archivos.h"
#include "gnugraph.h"
#include "quaternion.h"
#include "robot.h"
#include "utils.h"


/********************************************************************************************************************/
/*************************************
*VARIABLES PARA LA  LIBRERIA ROBOOP  *
************************************ */
/*Matriz que configura la estuctura Robot*/
Matrix Robot_Jarboter_INI(5,23);


/*Vector que contiene las caracteristicas  DH, Utiliza la notacion que explican 
en el manual de ROBOOP */
const Real Jarboter_data[] =
  /*type,teta,d,a,alfa*/
 //1            2     3     4        5            6           7  8 9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
  {0,           0, 0.30,    0, -M_PI/2, -2.37364778, 2.12930168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, -2.44346095,    0, 0.22,       0,	-2.44346095, 0.17453292, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0,  2.09439510,    0, 0.22,       0, -2.19911485, 2.09439510, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0,  2.09439510,    0,    0,  M_PI/2, -2.09439510, 2.09439510, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0,           0, 0.10,    0,       0, -2.96705972, 2.96705972, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/********************************************************************************************************************/
/********************************************
*VARIABLES PARA LA BARRA DE DESPLAZAMIENTO  *
*********************************************/
const CP_SMOOTH          = WS_CHILD|WS_VISIBLE|PBS_SMOOTH;///< Constante utiliza en barra de progreso para la intensidad del color 
const COLOR_AZUL         = RGB(0,128,192);                ///< Constante utiliza en barra de progreso para el color 

/********************************************************************************************************************/
/********************************************
*VARIABLES PARA LA COMUNICACION CON uC-USB  *
*********************************************/
/*Del PacketOI*/
extern tPacketHeader rxHeader;  
/*Para enviar comandos hacia el controlador*/
tCommand txCommand;  
     
/********************************************************************************************************************/
/************************************
*BANDERAS DE ESTADOS DEL PROGRAMA   *
************************************/
/*Indica que la memoria EEprom ha sido leida correctamente*/
static int EepromOK=0;
/*Bandera que indica que robot se ha movido por primera vez*/
int Robot_MOVE=0;
/*Bandera que indica que se ha reconfigurado el PMD*/
int SET_ROBOT=0;

/********************************************************************************************************************/
/************************************
*VARAIBLES GLOBALES DEL TRAZADO     *
************************************/
/*Este es una estructura tipo comando la cual solo se actualiza cuando se configura el trazado, 
por lo tanto contiene tanto le numero de variales configuras actualmente y cuales se estam muestreando  */
static tCommand ComandTrace; 
/*Guarda el Tiempo de muestreo*/
double GraficoTraceSampletime;
/*Conteo de varaiables configuradas em PMD1*/
int PMD1NVariables;
/*Conteo de varaiables configuradas em PMD2*/
int PMD2NVariables;


/********************************************************************************************************************/
/************************************
*VARAIBLES GLOBALES DE ROBOOP       *
************************************/
static Robot Jarboter;
Matrix Homogeniakine(4,4);
Matrix Homogeniainvkine(4,4);
Real* Tetha;
ColumnVector RPY(3);	
ColumnVector Q(5);
Real q[5];
bool Converge=0;

/********************************************************************************************************************/
/************************************
*VARAIBLE GLOBAL GENERAL            *
************************************/
/*Contiene la configuracion por defecto usada en muchas funciones*/ 
tAxisSettings axisSettings[DOF];

/********************************************************************************************************************/
/************************************
*VARAIBLE GLOBAL TECH PENDALT       *
************************************/
/*Se inicializa a la posicion de Home*/ 
static double AnguloCintura=0;
static double AnguloHombro=-140;
static double AnguloCodo=124;
static double AnguloPich=170;
static double AnguloRoll=0;
static int ResetFlag=0;



/********************************************************************************************************************/
/**********************************************************************/
//							WINMAIN                                   //  
/**********************************************************************/

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
    HWND hwnd;               /* Manipulador de ventana */
    MSG mensaje;             /* Mensajes recibidos por la aplicación */
    WNDCLASSEX wincl;        /* Estructura de datos para la clase de ventana */
    /* Estructura de la ventana */
    wincl.hInstance = hInstance;
    wincl.lpszClassName = "NUESTRA_CLASE";
    wincl.lpfnWndProc = WindowProcedure;      /* Esta función es invocada por Windows */
    wincl.style = CS_DBLCLKS;                 /* Captura los doble-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);
    /* Usar icono y puntero por defector */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* Sin menú */
    wincl.cbClsExtra = 0;                      /* Sin información adicional para la */
    wincl.cbWndExtra = 0;                      /* clase o la ventana */
    /* Usar el color de fondo por defecto para la ventana */
    wincl.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
    /* Registrar la clase de ventana, si falla, salir del programa */
    if(!RegisterClassEx(&wincl)) return 0;
    /* La clase está registrada, crear la ventana */
    hwnd = CreateWindowEx(
           0,                   /* Posibilidades de variación */
           "NUESTRA_CLASE",     /* Nombre de la clase */
           "INTERFAZ DE CONTROL",       /* Texto del título */
           WS_OVERLAPPEDWINDOW, /* Tipo por defecto */
           200,       /* Windows decide la posición */
           10,       /* donde se coloca la ventana */
           554,                 /* Ancho */
           130,                 /* Alto en pixels */
           HWND_DESKTOP,        /* La ventana es hija del escritorio */
           LoadMenu(hInstance, "Menu"),
           hInstance,           /* Manipulador de instancia */
           NULL                 /* No hay datos de creación de ventana */
	);
    /* Mostrar la ventana */
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    /* Bucle de mensajes, se ejecuta hasta que haya error o 
	GetMessage devuelva FALSE */
    while(TRUE == GetMessage(&mensaje, NULL, 0, 0))
    {
        /* Traducir mensajes de teclas virtuales a mensajes de caracteres */
        TranslateMessage(&mensaje);
        /* Enviar mensaje al procedimiento de ventana */
        DispatchMessage(&mensaje);
    }
    /* Salir con valor de retorno */
    return mensaje.wParam;
}



/********************************************************************************************************************/
/********************************************
*WindowProcedure:DispatchMessage() invoque  *
*********************************************/
/*Las variables que estan aqui como static es porque se quieren manter en memoria 
en el transcurso del programa principal y se deberan de mandar como parametro a las
ventanas hijos que lo necesiten */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
	/*variables para el sistema windows ventanas y bitmap*/
	static HINSTANCE hInstance;
	static HWND hButton = 0;
    /*Variables de programa para manejo del usb*/
	char    DeviceName[MAX_DRIVER_NAME]   = "EZUSB-0"; 
    /*Variables para generar trayectorias desde techpendant*/ 
	static 	int16 *pPosesID; 
	static  int NpuntosAlmacenados;	

	
	Listapara *Parametros=(Listapara*)declarar(sizeof(Listapara));
     /* manipulador del mensaje */
	switch (msg)                 
    {
		case WM_CREATE:  
			/*obtengo la istancia de la ventana, osea la primera llamada al programa*/
			hInstance = ((LPCREATESTRUCT)lParam)->hInstance; 
			/*Inserto el menu*/
			InsertarMenu(hwnd,hInstance); 
			/*Configura los parametos principales del robot*/
			if(SET_ROBOT==0)
				setupAxisSettings(&axisSettings[0]);	                  
			NpuntosAlmacenados=0;
        return 0;
        case WM_COMMAND: 
            switch(LOWORD(wParam)) 
		    {
			    /*configura los parametros del PMD*/
		        case ID_SETROBOT:                                 
					Parametros->Instancia=hInstance;      
					DialogBoxParam(hInstance,"CONFIGURACION",hwnd,SETRobot, (LPARAM)Parametros);
				break;	
				/*Configuraro la hora y alarmas*/
				case IDC_SETRTC:
					Parametros->Instancia=hInstance;      
					DialogBoxParam(hInstance,"RTC",hwnd,SETRTC, (LPARAM)Parametros);
				break;
				/*Configura el trazado*/
				case IDC_SETTRACE:
					Parametros->Instancia=hInstance;      
					DialogBoxParam(hInstance,"TRAZADO",hwnd,SETTRACE, (LPARAM)Parametros);
				break;
				/*Ejecuta reset al sistema*/
		        case  ID_RESET:                                 
                    Mandar_uCreset(DeviceName,hwnd);
					AnguloCintura=0;
					AnguloHombro=-140;
					AnguloCodo=120;
					AnguloPich=174;
					AnguloRoll=0;			
			    break;
				/*Solo manda la trayectoria y lo imprime en pantalla*/
			    case ID_TIPOTRAYECTORIA: 
					Parametros->Instancia=hInstance;      
					DialogBoxParam(hInstance,"TRAYECTORIA",hwnd, Envia, (LPARAM)Parametros);
				break;
				/*Ejecuta que techpendant la cual puede mover el robot, grabar poses y generar trayectorias 
				de poses grabadas*/
			    case ID_TECHBOX: 
					/* Un apuntador permite cambiar el valor de lo que apunta
					en este caso se esta mandando la direccion de la variable apuntador pPosesID */
					Parametros->P_pVectorposes=&(pPosesID);
					Parametros->NpuntosAlmacenados=&NpuntosAlmacenados;
					Parametros->Instancia=hInstance;
					DialogBoxParam(hInstance,"TERMINAL_ENSEÑANZA",hwnd,TechBox,(LPARAM)Parametros);
				break;	   
            }
        break;   
        case WM_DESTROY:  // Esto es cuando se le da la X de la ventana
            PostQuitMessage(0);    /* envía un mensaje WM_QUIT a la cola de mensajes */
        break;
        default:                  /* para los mensajes de los que no nos ocupamos */
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
	return 0;
}


/********************************************************************************************************************/
/****************************************************
*Envia:Funcion del dialogo que envia una trayectoria*
*****************************************************/
BOOL CALLBACK Envia(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
 
    /*Barra de progreso y ventanas*/
	static HWND       hWndPB;      
    HINSTANCE hInstance;           
//	int i;
	/*variables para procesar los archivos*/
	char Path[254]="C:\\Jarboter_V_2_1\\External\\Instalador 1.4-1\\robomosp\\Saves\\GNUPlot\\InverseKinematics\\";
    char *NArchivo = NULL;       
    int linetextlen;
    double * datos;                
    pDIMEN Dimen=NULL;   
	tTrajectory trajectory;     
    /*variables para el USB*/
    char    DeviceName[MAX_DRIVER_NAME]   = "EZUSB-0";
	int rc;
    
	switch (msg)              
    {
        case  WM_INITDIALOG:
			/*obtengo la istancia de la aplicacion*/
            hInstance = ((Listapara *)lParam)->Instancia;                      
			/*Limito el monbre maximo a 50 caracteres*/
            SendDlgItemMessage(hDlg,IDC_EDIT1, EM_LIMITTEXT,50, 0L);           
            SetFocus(GetDlgItem(hDlg,IDC_EDIT1));   
			
			/* Creo un recuadro para el progressbar*/
		    //hWndPB = CreateWindowEx(0,PROGRESS_CLASS, (LPSTR) NULL,CP_SMOOTH , 
			 //       20,80 ,370, 20, hDlg, 0, hInstance, NULL);  
			/*El progressber esta desde 1-100*/
		    //SendMessage(hWndPB, PBM_SETRANGE, 0, MAKELPARAM(1, 100));		   	
            /*Indica la cantidad de unidades a usar en el  incremento de la progressbar*/
		    //SendMessage(hWndPB, PBM_SETSTEP, (WPARAM) 1, 0);					    
			/* Establece el color de la progressbar*/
		    //SendMessage(hWndPB, PBM_SETBARCOLOR, 0, (LPARAM) COLOR_AZUL);          
        return FALSE;														
        case WM_COMMAND:
            switch(LOWORD(wParam)) 
		    {
                case IDOK: 			
					/*Retorna la cantidad de letras escritas*/
					linetextlen = SendDlgItemMessage(hDlg,IDC_EDIT1,EM_LINELENGTH,50, 0L);         
					NArchivo=(char *)declarar(linetextlen);
					/*Lee la cantidad de linetextlen caracteres */
				    GetDlgItemText(hDlg,IDC_EDIT1,NArchivo,linetextlen+1);
					/*El coloco el .plo y el Path*/
					strcat (NArchivo,".plot");         
					strcat (Path,NArchivo);    
					if(Existe_Arhivo(hDlg,Path)==rcFileOpenOK)    
					{
				        Dimen= (pDIMEN)declarar(sizeof(DIMEN));		 
						/*Obtengo dimeciones filas, columnas, Nº datos*/
					    Get_dimension_trayec(Path,Dimen);			           
					    datos= (double*)declarar(Dimen->Numdata*sizeof(double));   
	  			        Obtener_datosFILE(hDlg,Path,&datos[0],Dimen);      						
					    trajectory.point=(tpoint*)declarar(Dimen->puntos*sizeof(tpoint)); 
						/*Llena la trayectoria ya declara usando el vector datos*/													  
					    rc=Llenar_trayectoria(&trajectory,&(axisSettings[0]),Dimen,datos,SET_ROBOT);
						if(rc!=rcOK)
						{
							MessageBox(hDlg, "Error: El archivo leído no corresponde al Robot Jarboter", "Información", MB_OK);
							break;
						}
						/*limpio los buferres de la tranferencia para no leer falsos datos*/
						initPacketIO();										   
						trajectory.invalidmask=31;                    
					//	trajectory.invalidmask=1;                    
					    rc=dispatchPacket(PacketTrajectory,(byte*)&trajectory,DeviceName,hDlg);//esta funcion se encarga de llenar el header y finalmente enviarlo por el USB						
					    if(rc!=rcOK )
					    {	
							/*No fue causado por el cable*/
							if(rc!=rcCommunicationError)
							{
								/*El proceso de abortar se divide en 3 pasos C6 C9 C4*/
								//1. Quito la interupciones VX_C6
								//2. Abortar la tranferencia del Gpig mediante el vendor VX_C9	
							    //2. Cancelar la peticion del software usando IOCTL_Ezusb_ABORTPIPE
								//3. Limpiar los buffer C4
								Abortar(hDlg,0,DeviceName);        
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
							    EndDialog(hDlg, FALSE);
							}
					    }
						else  //Si todo esta bien
						{
						//	Reiniciar(hWndPB);                        //  Reiniciamos la progressbar
						//	for(i=0;i<=97;i++)
						//	{ 
						//		AumentarValor(hWndPB,i, 5);             // Cada iteración aumentamos el valor de la progressbar en X y dormimos 5 milisegundos 
						//		Actualizar(hWndPB); 
						//	}
							DisplayTrayectInfo(hDlg,&trajectory,IDC_TRAJPRINT);
							MessageBox(hDlg, "Archivo enviado corectamente", "Información", MB_OK);
							/*Bandera que indica que el robot se  esta moviendo*/
							Robot_MOVE=1;
						}
	                }
			    break;
				case IDCANCEL:
                EndDialog(hDlg, FALSE);
                break;
            }
        return TRUE;
    } //end case command
 return FALSE;
}


/********************************************************************************************************************/
/****************************************************
*TechBox:Comanda poses al robot					    *
*****************************************************/
/*Necesitamos
1. El vector dimanico que contiene los ids en memoria EEPROM
2. Un vector dimanico que contiene los ids del usuario + los de eeprom
   este vector es especial pues aumenta en caliente, osea que cada vez que el 
   usuario ingresa un nuevo id este vector se destruye y se crea una igual mas
   grande para almacenar esta nueva pocion 
3. Un vector temporal para almacenar los ids hasta el monento con el fin de crear el 
   vector mas grande.
Nota el apuntador al vector dinamico # 2 es que se retorna a el WindowProcedure 
*/

BOOL CALLBACK TechBox(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{

	/*VARIABLES DE WINDOWS*/
	static  HINSTANCE hInstance;
	//HANDLE GRAFICO DE LOS BOTONES POSE
	HWND  EJEPOSE = GetDlgItem (hDlg,IDC_EJEPOSE);
	HWND  GUARDARPOSE = GetDlgItem (hDlg,IDC_GPOSE);
	HWND  GENTRAJ = GetDlgItem (hDlg,IDC_TRAJPENDANT);
	HWND  LEEREEPROM = GetDlgItem (hDlg,IDC_LEERPOSES);
	HWND  POSEACTUAL = GetDlgItem (hDlg,IDC_PoseActual);
	HWND  BorrarPoses = GetDlgItem (hDlg,IDC_BORRAR);
	//HANDLE GRAFICO DE LOS BOTONES MATRIZ HOMOGENIA
	HWND  MATRIXKINE = GetDlgItem (hDlg,IDC_MATRIXKINE);
	HWND  MATRIXINVKINE = GetDlgItem (hDlg,IDC_MATRIXINVKINE);
	//HANDEL GRAFICOS DE LOS COMBOS
	HWND ComboxID  = GetDlgItem(hDlg,IDC_COMBOID);

	/*VARIABLES PARA EL MANEJO DE POSES*/
    static 	int	PoseActual;
	char text_pose_almacenada[10];
	/*Usado para mamdar el ID si en cuadro de dialogo sobreescribir*/
	Listapara *Parampose=(Listapara*)declarar(sizeof(Listapara));
	/*Contiene la dirección del apuntador que apunta a el vector de IdS*/
    static	int16** P_pVectorID; 
	int16* tempVector;
	static  int*  NpuntosAlmacenados;
	char    CharposeID[50];
	int UserPoseID,IndexPose,PoseCmd,Nundatos; 
	static int* ResetPoseFlag;
	/*Parametros utilizados cuando se genera una trayectoria por poses*/
    Listapara *TrajParam=(Listapara*)declarar(sizeof(Listapara));
	
	/*Variables manejo de trayectoria un punto*/
	static  int axisOP[DOF]; 
	static  tTrajectory TrayectoriaTECH;     	
	int i,HAYDATOS=0;
	/*USB*/
	char    Devicename[MAX_DRIVER_NAME]   = "EZUSB-0";   //Por defaul
	int rc;
	
	/*Parametros utilizados cuando se imprime la matriz homogenia*/
	Listapara *RoboParam=(Listapara*)declarar(sizeof(Listapara));
	/*Estas variables se utilizan para almacenar la configuracion actual 
        del robot*/
	extern tAxisSettings rxAxisSettings[DOF];
	extern uint16        rxPosesActual;
	
	
	switch (msg)
    {
		case WM_INITDIALOG:
			hInstance = ((Listapara *)lParam)->Instancia;                    
			/*Guardo la direccion del apuntador para poder cambiar su valor y asi cambiar la  direccion a la que apunta*/
		    P_pVectorID=((Listapara *)lParam)->P_pVectorposes;
			/*Guardo la direccion de variable NpuntosAlmacenados del WindowProcedure (Main() principal) */
			NpuntosAlmacenados=((Listapara *)lParam)->NpuntosAlmacenados;
			/*Coloco los BMPs*/
			cargoBotonTechPendal(hDlg,hInstance);
			/*Se inicializa las caracteristicas de los ejes*/
			/*Configuro el commando para mandarlo*/
			txCommand.CommandOP=CommandGetAxisSettings;		       
			/*Mando por usb*/
			/*
			rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
			if(rc!=rcOK)
			{
				if(rc!=rcCommunicationError)
				{
					Abortar(hDlg,0,Devicename);
					MessageBox(hDlg, "El controlador Ocupado. No se puede ejecutar Teach pendal", "Información", MB_OK); //significa que hubo un problema del proceso de 
					EndDialog(hDlg, FALSE);
				}
			
			}
			else 
			{  
				//Envie bien el commando
				if(rc==rcOK)
				{
					Sleep(300);
					rc=receiptPacket(Devicename,hDlg);
					if(rc==rcOK)
					{
						setupAxisSettings(&rxAxisSettings[0]);  
					}
					else
					{
						MessageBox(hDlg,"El Controlador no envió la Configuración actual. Itente de nuevo.", "Información", MB_OK);
						Abortar(hDlg,1,Devicename);
						EndDialog(hDlg, FALSE);
					}									
				}
			}
			//Se inicializa todas las opciones a no seleccionada */
			for(i =0; i <DOF; i++)
				axisOP[i]=0;
			/*Inicializo la estructura trayectoriaTECH */
			TrayectoriaTECH.point=(tpoint*)declarar(sizeof(tpoint));
			TrayectoriaTECH.size=1;
			/*Inicializo los combo edit para los angulos */
			setlong(hDlg,IDC_EPOS,AnguloCintura);
			setlong(hDlg,IDC_EPOS2,AnguloHombro);
			setlong(hDlg,IDC_EPOS3,AnguloCodo);
			setlong(hDlg,IDC_EPOS4,AnguloPich);
			setlong(hDlg,IDC_EPOS5,AnguloRoll);			
			/*Inicializo la estructura de la libreria de robotica*/
			Robot_Jarboter_INI<<Jarboter_data;			
			Jarboter = Robot(Robot_Jarboter_INI);
			/*Inicializo todo los ScrollBar*/
			SetScrollBottons(hDlg,&axisSettings[0]);	
			ResetPoseFlag=(int*)declarar(sizeof(int));
	    return FALSE;
		case WM_HSCROLL:
			/*lParam en este momento ya no tiene los parametros 
			de la llamada de dialogo sino el Handle de Control scrollbar espichado*/
			/*axisOP[DOF]*/
			 ActualizarScrollBar(hDlg,&axisSettings[0], (HWND)lParam,(int)LOWORD(wParam),(int)HIWORD(wParam),axisOP);
			//ProcesarScrollH(hDlg,(HWND)lParam, (int)LOWORD(wParam),(int)HIWORD(wParam) );
        return FALSE;
		case WM_COMMAND:
			switch(LOWORD(wParam)) 
			{
				/*Hubo un click sobre checkbox del eje ya sea para activar o desactivar*/
				case  IDC_RCINTURA:
                /*Habilita o desabilita los campos y axisOP[AxisWaist]=1 */
					manejador_Campostech(hDlg,IDC_RCINTURA,AxisWaist,axisOP);
				break;
				case  IDC_RHOMBRO:
					manejador_Campostech(hDlg,IDC_RHOMBRO,AxisShoulder,axisOP);
                break;
				case  IDC_RCODO:
			        manejador_Campostech(hDlg,IDC_RCODO,AxisElbow,axisOP);			
                break;
				case  IDC_RPITCH:
					manejador_Campostech(hDlg,IDC_RPITCH,AxisPitch,axisOP);			
                break;
				case  IDC_RROLL:	
					manejador_Campostech(hDlg,IDC_RROLL,AxisRoll,axisOP);			
                break;
				/*Mando el commando CommandOpenGripper*/
				case  IDC_GRIOPEN:
				    txCommand.CommandOP=CommandOpenGripper;
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
				    if(rc!=rcOK) 
					{
						/*No fue causado por el cable*/
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else
					{	
							EnableWindow (GUARDARPOSE, TRUE);
					}

				
                break;
				case  IDC_GRICLOSE:	
				    txCommand.CommandOP=CommandCloseGripper;
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
					if(rc!=rcOK)
					{
					    if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else
					{
						EnableWindow (GUARDARPOSE, TRUE);
					}
				break;
				/*Este comando le pide al uC un vector con los id que tiene la eeprom
				Por lo tanto en el siguiente IN el uC los debe de mandar*/
				case IDC_LEERPOSES:
					/*Mando por usb*/
					txCommand.CommandOP=CommandGetPosesID;		       
                    rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else 
					{   
						if(rc==rcOK)
						{	/*Espero un segundo a que uC lee el comando CommandGetPosesID */
							Sleep(200);
							/*Una vez lleido el comando uC esta listo para pedirle los IDs */
							rc=receiptPacket(Devicename,hDlg);
							if(rc==rcOK)
							{
								/*Leeo el vector recivido por rxvector*/
								Actualizar_Vector(P_pVectorID,1,0);
								/*Actualizo la cantidad de puntos grabados segun la Eeprom*/
								tempVector=*P_pVectorID;
								if(tempVector[1]==-1)
									*NpuntosAlmacenados=0;
								else
								{
									/*Actualizo los puntos almacenados*/
									*NpuntosAlmacenados=tempVector[0]-1;
									Put_ID(hDlg,tempVector,IDC_COMBOID);
								}
								/*Habilitos los botones*/;
								if(*NpuntosAlmacenados==0)
								{
									EnableWindow (BorrarPoses, FALSE);								
									EnableWindow (EJEPOSE, FALSE);
									EnableWindow (GUARDARPOSE, FALSE);
									EnableWindow (GENTRAJ, FALSE );
									EnableWindow (POSEACTUAL, FALSE);
								}
								else
								{
									EnableWindow (BorrarPoses, TRUE);								
									EnableWindow (EJEPOSE, TRUE);
									if(Robot_MOVE==1)
									{
										EnableWindow (GUARDARPOSE, TRUE);
									}
									else
									{
										EnableWindow (GUARDARPOSE, FALSE);
									}
									EnableWindow (GENTRAJ, TRUE );
									EnableWindow (POSEACTUAL, TRUE);
								}
								EnableWindow (LEEREEPROM, FALSE);
								EepromOK=1;	
							}
							else
							{
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
								Abortar(hDlg,1,Devicename);
							}
						}
					}
                break;
				case  IDC_GPOSE:  /*Guarda la pose*/
					/*El comando CommandSetPoseEEPROM guarda en EEprom la pose actual 
                    del robot y le coloca una etiqueta UserPoseID 
					(esta opcion debe estas desabilitada si no se ha leido las poses de 
					la memoria eeprom)
					Una vez grabada de bede de desabilitar el boton  y conprobar que 
					la etiqueta utilizada no exsista.
					*/
					/*Configuro el commando para mandarlo*/
				    txCommand.CommandOP=CommandSetPoseEEPROM;
					UserPoseID=GetDlgItemInt(hDlg,IDC_COMBOID,NULL,FALSE);	
                    txCommand.Dato=UserPoseID;
					/*Compruebo que sea mayor a 1*/
					if(UserPoseID<1)
					{
						MessageBox(hDlg, "El Identificador de la Pose (ID) no es válido", "Información", MB_OK); 
						break;
					}
					/*Como *P_pVectorID es un vector dinamico necesitamos Nundatos para
					saber su dimension*/
					Nundatos=*P_pVectorID[0];
					if(ExisteIDPose(*P_pVectorID,UserPoseID,Nundatos))
					{
						DialogBoxParam(hInstance,"ADVERTENCIA",hDlg,RSETPOSE,(LPARAM)ResetPoseFlag);
						/*Es modificada por le dialogo "ADVERTENCIA" */
						if(*ResetPoseFlag)
						{
							txCommand.CommandOP=CommandReSetPoseEEPROM;
							*ResetPoseFlag=0;
						}
						else
							break; //Me salgo porque no quiero hacer nada.
					}
					else //Tengo que actualizar 
					{
						/*Actualizar_Vector guarda la Información de Ids hasta el 
						momento en un vector temporal, luego destruye el vector
						*P_pVectorID y crea un nuevo con los datos de el vector temporal
						mas la nueva etiquieta actualizando de nuevo el puntero *P_pVectorID */
						Actualizar_Vector(P_pVectorID,2,UserPoseID);
						(*NpuntosAlmacenados)++;
					}
					
					/*Mando por el USB*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
					if(rc!=rcOK) 
					{
						/*No fue problema del cable*/
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "La Pose No Pudo ser almacenada", "Información", MB_OK); //significa que hubo un problema del proceso de 			
						}
					}
					else
					{
						sprintf(CharposeID,"Pose %d almacenada correctamente",UserPoseID);
						MessageBox(hDlg,CharposeID, "Información", MB_OK);
						/*Si me he movido a la pose X, necesariamente X es la poses actual*/
						PoseActual=UserPoseID;
						/*Desabilito el boton para no grabar dos veces la misma pose*/
						EnableWindow (GUARDARPOSE, FALSE );	
						EnableWindow (EJEPOSE, TRUE);
						EnableWindow (POSEACTUAL, TRUE);
						EnableWindow (BorrarPoses, TRUE);	
						itoa(UserPoseID,text_pose_almacenada,10);
						SendDlgItemMessage(hDlg,IDC_COMBOID, CB_ADDSTRING, 0, (LPARAM)text_pose_almacenada);
						
					}
				break; 
				case  IDC_EJEPOSE:
					/*Lee una pose de la memoria EEprom utilizando la etiquieta para 
					luego ejecutarla a velocidades maximas*/
					/*Configuro el comando*/
				    txCommand.CommandOP=CommandGetPoseEEPROM;
                    UserPoseID=GetDlgItemInt(hDlg,IDC_COMBOID,NULL,FALSE);	
                    txCommand.Dato=UserPoseID;
					if(UserPoseID<1)
					{
                       MessageBox(hDlg, "El Identificador de la Pose (ID) no es válido", "Información", MB_OK); //significa que hubo un problema del proceso de 
					   break;
					}
					Nundatos=*P_pVectorID[0];
					/*Si encontro la etiqueta ejecutela*/
                    if(ExisteIDPose(*P_pVectorID,UserPoseID,Nundatos))
					{
						/*Mando por USB*/
						rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
						if(rc!=rcOK)
						{
							/*No fue problema del cable*/
							if(rc!=rcCommunicationError)
							{
								Abortar(hDlg,0,Devicename);
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
							}							
						}
						else
						{
							sprintf(CharposeID,"Ejecutando  Identificador de Pose (ID) %d",UserPoseID);
							MessageBox(hDlg,CharposeID, "Información", MB_OK);
							/*Me estoy moviendo a la pose X, por lo tando esa es la pose actual*/
							PoseActual=UserPoseID;
							/*Bandera que indica que el robot se  esta moviendo*/
							Robot_MOVE=1;
						}
                    }
					else 
						MessageBox(hDlg, "Indentificador de Pose (ID) no existe en el sistema", "Información", MB_OK); //significa que hubo un problema del proceso de */
				break;
				/*Genera una secuencia de puntos en forma de vector*/
				case IDC_TRAJPENDANT:
					/*Esto es necesario pues si tiene que mantener la 
					Información de las etiquetas de pose asi se cierren todas
					las ventanas hijas de la ventana principal*/
					TrajParam->P_pVectorposes=P_pVectorID;
					TrajParam->NpuntosAlmacenados=NpuntosAlmacenados;
					TrajParam->Instancia=hInstance;
					if((*NpuntosAlmacenados)>=2)
						DialogBoxParam(hInstance,"TRATJ",hDlg,TechTracj,(LPARAM)TrajParam);
					else
						MessageBox(hDlg, "Para Generar una Trayectoria se necesitan por los menos 2 poses almacenadas en el sistema.", "Información", MB_OK); //significa que hubo un problema del proceso de 
                break;
				case IDC_MATRIXKINE:
					RoboParam->Instancia=hInstance;
					/*Para indicar que se trata de una matriz generada por KINE*/
					RoboParam->Tsize=1;
					DialogBoxParam(hInstance,"DIALOGO_MATRIZ",hDlg,DISPLAYMATIZ,(LPARAM)RoboParam);
                break;
				case IDC_MATRIXINVKINE:
					RoboParam->Instancia=hInstance;
					/*Para indicar que se trata de una matriz generada por KINE*/
					RoboParam->Tsize=0;
					DialogBoxParam(hInstance,"DIALOGO_MATRIZ",hDlg,DISPLAYMATIZ,(LPARAM)RoboParam);
                break;
				case IDC_FORWARD:
					if(EepromOK) //He leido las Poses
					{	
						Nundatos=*P_pVectorID[0];
                        /*Nundatos tiene las cantidad de casillas de P_pVectorID */
						IndexPose=BuscarIndexPose(*P_pVectorID,PoseActual,Nundatos);
						/*Si la pose actual exite*/
						if(IndexPose!=-1)
						{
							/*Pose esta al Final del vector, i=0 no es una pose es 
							la cantidad de datos */
							if(IndexPose==Nundatos-1)
								IndexPose=1;
							else 
								IndexPose++;
							/*Obtiene una Pose de la EEPROM*/
							txCommand.CommandOP=CommandGetPoseEEPROM;
							PoseCmd=return_id(*P_pVectorID,IndexPose);	
							txCommand.Dato=PoseCmd; 
							/*Mando por USB*/
							rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
							if(rc!=rcOK) 
							{
								if(rc!=rcCommunicationError)
								{
									Abortar(hDlg,0,Devicename);
									MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
								}
							}
							else
							{
								sprintf(CharposeID,"Ejecutando  Identificador de Pose (ID) %d",PoseCmd);
								MessageBox(hDlg,CharposeID, "Información", MB_OK);
								PoseActual=PoseCmd;
								Robot_MOVE=1;
							}
						}
						else 
							MessageBox(hDlg, "Debe de mover el robot a una pose conocida.", "Información", MB_OK);
					}
					else 
						MessageBox(hDlg, "Primero debe leer las poses almacenadas en el sistema. ", "Información", MB_OK); //significa q                
				break;
				case IDC_BACK:
					if(EepromOK) //He leido las Poses
					{
						Nundatos=*P_pVectorID[0];
                        /*Nundatos tiene las cantidad de casillas de P_pVectorID */
						IndexPose=BuscarIndexPose(*P_pVectorID,PoseActual,Nundatos);
						/*Si la pose actual exite*/
						if(IndexPose!=-1)
						{
							/*Pose esta al principio del vector, i=0 no es una pose es 
							la cantidad de datos */
							if(IndexPose==1)
								IndexPose=Nundatos-1;
							else 
								IndexPose--;
							/*Obtiene una Pose de la EEPROM*/
							txCommand.CommandOP=CommandGetPoseEEPROM;
							PoseCmd=return_id(*P_pVectorID,IndexPose);	
							txCommand.Dato=PoseCmd; 
							/*Mando por USB*/
							rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
							if(rc!=rcOK)
							{
								if(rc!=rcCommunicationError)
								{
									Abortar(hDlg,0,Devicename);
									MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
								}
							}
							else
							{
								sprintf(CharposeID,"Ejecutando  Identificador de Pose (ID) %d",PoseCmd);
								MessageBox(hDlg,CharposeID, "Información", MB_OK);
								PoseActual=PoseCmd;
								Robot_MOVE=1;
							}
						}
						else
							MessageBox(hDlg, "Debe de mover el robot a una pose conocida.", "Información", MB_OK);
						
					}
					else 
						MessageBox(hDlg, "Primero debe leer las poses almacenadas en el sistema. ", "Información", MB_OK); //significa q                
				break;
				case IDC_PoseActual:
					txCommand.CommandOP=CommandGetPoseActual;		       
					/*Mando por usb*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "El controlador Ocupado. No se puede Consultar la Pose actual", "Información", MB_OK); //significa que hubo un problema del proceso de 
							EndDialog(hDlg, FALSE);
						}
					}
					else 
					{  
						/*Envie bien el commando*/
						if(rc==rcOK)
						{
							Sleep(300);
							rc=receiptPacket(Devicename,hDlg);
							if(rc!=rcOK)
							{
								MessageBox(hDlg,"El Controlador no envió la Pose actual. Itente de nuevo.", "Información", MB_OK);
								Abortar(hDlg,1,Devicename);
							}									
						}
					}
					if(rxPosesActual<1)
						MessageBox(hDlg, "El robot no está en una pose definida", "Información", MB_OK);			
					else
					{
						sprintf(CharposeID,"El robot esta en la Pose %d",rxPosesActual);
						MessageBox(hDlg,CharposeID, "Información", MB_OK);			
					}
				break;
				case IDC_BENVIAR:  
					/*Es la funcion de PLAY*/
					if(RobotGoto(hDlg,&axisSettings[0],axisOP,0))
					{
						Robot_MOVE=1;
						PoseActual=0;
						if(EepromOK)
							EnableWindow (GUARDARPOSE, TRUE );
						/*Obtenemos los angulos tetha */
						Tetha=GetGradosAxis(hDlg);
						/*Actualizando las variables globales*/
						AnguloCintura=rad2grados(Tetha[0]);
						AnguloHombro=rad2grados(Tetha[1]);
						AnguloCodo=rad2grados(Tetha[2]);
						AnguloPich=rad2grados(Tetha[3]);
						AnguloRoll=rad2grados(Tetha[4]);

						/*Actualizamos la estructura especial de la liberia de robotica*/
						Jarboter.set_q(Tetha[0],1);
						Jarboter.set_q(Tetha[1],2);
						Jarboter.set_q(Tetha[2],3);
						Jarboter.set_q(Tetha[3],4);
						Jarboter.set_q(Tetha[4],5);	
						/*Obtenemos la cinematica inversa*/
						Homogeniakine=Jarboter.kine();
						/*Colocamos los resultados en el modo grafico*/
						setlong(hDlg,IDC_POSX,Homogeniakine(1,4));
						setlong(hDlg,IDC_POSY,Homogeniakine(2,4));
						setlong(hDlg,IDC_POSZ,Homogeniakine(3,4));	
						RPY=irpy(Homogeniakine);
						RPY=RPY/(M_PI/180);
						setlong(hDlg,IDC_eulerPICHkine,RPY(2));	
						setlong(hDlg,IDC_eulerROLLkine,RPY(3));	
						/*Habilito el boton de imprimir la matriz homogenia*/
						EnableWindow (MATRIXKINE, TRUE);
						/*Habilito el boton generación de trayectorias por poses */
						if(*NpuntosAlmacenados>=1)
							EnableWindow (GENTRAJ, TRUE );
					}
				break; 
				case IDC_CALDIRECTA:
						/*Obtenemos los angulos tetha */
						Tetha=GetGradosAxis(hDlg);
						/*Actualizamos la estructura especial de la liberia de robotica*/
						Jarboter.set_q(Tetha[0],1);
						Jarboter.set_q(Tetha[1],2);
						Jarboter.set_q(Tetha[2],3);
						Jarboter.set_q(Tetha[3],4);
						Jarboter.set_q(Tetha[4],5);	
						/*Obtenemos la cinematica inversa*/
						Homogeniakine=Jarboter.kine();
						/*Colocamos los resultados en el modo grafico*/
						setlong(hDlg,IDC_POSX,Homogeniakine(1,4));
						setlong(hDlg,IDC_POSY,Homogeniakine(2,4));
						setlong(hDlg,IDC_POSZ,Homogeniakine(3,4));	
						RPY=irpy(Homogeniakine);
						RPY=RPY/(M_PI/180);
						setlong(hDlg,IDC_eulerPICHkine,RPY(2));	
						setlong(hDlg,IDC_eulerROLLkine,RPY(3));	
						/*Habilito el boton de imprimir la matriz homogenia*/
						EnableWindow (MATRIXKINE, TRUE);
				break;
				case IDC_CALINVERSA:
					/*Cargo los Parametros DH*/	  /*d1			     /*A2                /*d5*/
					//PointTOAngleGEOMETRICO(hDlg,Jarboter_data[2],Jarboter_data[26],Jarboter_data[94]);
					//PointTOAngleGEOMETRICO(hDlg,Jarboter_data[2],Jarboter_data[26],Jarboter_data[49],Jarboter_data[94]);
					RPY(1)=0;
					RPY(2)=grados2rad(getlong(hDlg,IDC_eulerPICH));
					RPY(3)=grados2rad(getlong(hDlg,IDC_eulerROLL));					
					Homogeniainvkine=rpy(RPY);
					/*Coloco el punto*/
					Homogeniainvkine(1,4)=getlong(hDlg,IDC_POSXINV);
					Homogeniainvkine(2,4)=getlong(hDlg,IDC_POSYINV);
					Homogeniainvkine(3,4)=getlong(hDlg,IDC_POSZINV);
					/*ColumnVector Q */
					i=Jarboter.get_dof();
					Q=Jarboter.inv_kin(Homogeniainvkine,0,i,Converge);
					if(Converge==1)
					{
						/*Lo paso a grados*/
						Q=Q/(M_PI/180);
						/*Cambio de tipo ColumnVector to float */
						for(i=1;i<6;i++)	
							q[i-1]=Q(i);
						/*Coloco los angulos en el modo grafico*/
						SetAngle(hDlg,axisOP,&q[0]);
						EnableWindow (MATRIXINVKINE, TRUE);
					}
					else
						MessageBox(hDlg, "Se produjo singularidad efectuando la cinemática inversa", "Información", MB_OK); //significa que hubo un problema del proceso de 

				break;
				case  IDC_NEST:
					txCommand.CommandOP=CommandGoHome;		  
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
					/*Esto es para, si el cable USB esta desconectado no aparesca tambien, El controlador esta ocupado */
					if(rc!=rcOK)
					{
					    if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
						if(EepromOK)
							EnableWindow (GUARDARPOSE, TRUE );	

					}
    			break;
				case  IDC_ORIGEN:
					Robot_MOVE=RobotGoto(hDlg,&axisSettings[0],axisOP,2);
						if(Robot_MOVE)
						{
							if(EepromOK)
							{
								EnableWindow (GUARDARPOSE, TRUE );
							}
						}			
				break;
				case IDC_BORRAR:
					txCommand.CommandOP=CommandEraseEEPROM;		  
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,Devicename,hDlg); 
					/*Esto es para, si el cable USB esta desconectado no aparesca tambien, El controlador esta ocupado */
					if(rc!=rcOK)
					{
					    if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,Devicename);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else
					{
						/*Habilitos los botones*/
						EnableWindow (EJEPOSE, FALSE);
						EnableWindow (GUARDARPOSE, FALSE);
						EnableWindow (GENTRAJ, FALSE );
						EnableWindow (LEEREEPROM, FALSE);
						EnableWindow (POSEACTUAL, FALSE);
						EnableWindow (BorrarPoses,FALSE);	
						/*Reinicio a cero la cantidad de puntos almacenados*/
						*NpuntosAlmacenados=0;
						free(*P_pVectorID);
						*P_pVectorID=(short *)declarar(sizeof(int16)*2);
						tempVector=*P_pVectorID;
						tempVector[0]=2;
						tempVector[1]=-1;
						/*Coloca Nada el combo box*/
						Put_ID(hDlg,tempVector,IDC_COMBOID);
						MessageBox(hDlg, "Se ha Borrado Todas Las Poses Almacenadas en el Sistema", "Información", MB_OK); //significa que hubo un problema del proceso de 
					}
				break;
				case IDC_EMER:
					Mandar_Parada(Devicename,hDlg);	
				break;
			    case IDCANCEL:
                    EndDialog(hDlg, FALSE);
				break;
			}
        return TRUE;  //SIGO CON LA VENTANA
		
    }
	return FALSE; //TERMINO LA VENTANA  
}




/********************************************************************************************************************/
BOOL CALLBACK RSETPOSE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
	static HINSTANCE hInstance;
	char DeviceName[MAX_DRIVER_NAME]   = "EZUSB-0";   //Por defaul
	static int *ResetPose;

    switch (msg)                  /* manipulador del mensaje */
    {
		case WM_INITDIALOG:
		ResetPose=(int *)lParam;
	    return FALSE;
		case WM_COMMAND:
            switch(LOWORD(wParam)) 
		    {              
				case IDC_SESCRIBIR:
					*ResetPose=1;
					EndDialog(hDlg, FALSE);
				break;
				case ID_CANCELPOSES:
					*ResetPose=0;
                    EndDialog(hDlg, FALSE);
                break;
				case IDCANCEL: 
                    EndDialog(hDlg, FALSE);
                break;
            }
        return TRUE;
    }
    return FALSE;
}


/********************************************************************************************************************/
/*********************************************************
*TechTracj:Construye una trayectoria con poses grabadas  *
**********************************************************/
BOOL CALLBACK TechTracj(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Listapara *TrajParam=(Listapara*)declarar(sizeof(Listapara));
	UINT	PointIndex,PointID;
	HWND hListTraj  = GetDlgItem(hDlg,IDC_PUNTOSUSER);
	HWND hListPoses = GetDlgItem(hDlg,IDC_PUNTOSDIS);
	int rc;
	char temp[10];
	char ActualID[10];
	char Devicename[MAX_DRIVER_NAME]   = "EZUSB-0";   //Por defaul
	static HINSTANCE hInstance;
	static int i,IDPuntoExistente;
	static int PuntosProgramados,*NpuntosAlmacenados;
	static int16** P_pVectorID;
	static int16*  tempVector;
	static int16** P_pVectorFINALIDs;
	static int16*  pVectorFINALIDs;

	switch (msg)              
    {
		case WM_INITDIALOG:
			TrajParam=(Listapara *)lParam;
			hInstance = TrajParam->Instancia;                      
			/*Obtengo el vector de Ids y los puntos disponible*/
			P_pVectorID=((Listapara *)lParam)->P_pVectorposes;
			NpuntosAlmacenados=TrajParam->NpuntosAlmacenados;
			/*Inicializa los puntos Programados para la nueva trayectoria a generar*/
			PuntosProgramados=0;			
			SetDlgItemInt(hDlg,IDC_PUNTOSMAX,(*NpuntosAlmacenados),FALSE);
			SetDlgItemInt(hDlg,IDC_PUNTOACTUAL,PuntosProgramados,FALSE);
			/*Inicializacion de apuntadores*/
			P_pVectorFINALIDs=&pVectorFINALIDs;
            /* *P_pVectorFINALIDs permite modificar la direccion a la que apunta el apuntador, 
			pero no el contenido de esa direccion*/
			*P_pVectorFINALIDs=(short *)declarar(sizeof(int));
			/*tempVector tiene la direccion a lo que apunta pVectorFINALIDs */
            tempVector=*P_pVectorFINALIDs;
			/*tempVector[] tiene los datos a los que pVectorFINALIDs*/
			tempVector[0]=2;
			tempVector[1]=-1;
            /*Imprime los puntos disponibles en el cuadro de dialogo
			tempVector en esta cado tiene los Ids Disponibles*/
			tempVector=*P_pVectorID;
			for(i=1;i<((*NpuntosAlmacenados)+1);i++)
			{
			   	IDPuntoExistente=tempVector[i];
			    sprintf(temp,"%d",IDPuntoExistente);
                SendDlgItemMessage(hDlg,IDC_PUNTOSDIS, LB_ADDSTRING, 0, (LPARAM)&temp);
			}
			SetFocus(GetDlgItem(hDlg,IDC_PUNTOSDIS));
        return FALSE;
		case WM_COMMAND:
            switch(LOWORD(wParam)) 
			{      //Agrego los Puntos y genero la trayevtoria final
				case IDC_AGREGAR:
					/*Se lee la etiqueta de pose del cuadro trayectoria */
					if(PuntosProgramados!=0)
						SendMessage(hListTraj, LB_GETTEXT,(WPARAM)(PuntosProgramados-1),(LPARAM)temp); 
					/*Detecta la posicion del Ids eleguido en el cuadro de poses*/
					PointIndex= SendDlgItemMessage(hDlg,IDC_PUNTOSDIS,LB_GETCURSEL,0,0);
					if(PointIndex<(UINT)(*NpuntosAlmacenados))
					{
						/*tempVector tiene los Ids Disponibles*/
						tempVector=*P_pVectorID;
						/*Extraemos el ID del vector*/
						PointID=tempVector[PointIndex+1];
						sprintf(ActualID,"%d",PointID);
						/*Comparamos el Id seleccionado graficamente que esta en temp con la 
						etiqueta ActualID con el fin de no repetir las etiquetas*/
						if(strcmp(temp,ActualID))  /*retorna=0 cuando son iguales*/
						{
							/*Coloca la etiqueta en el cuadro de dialogo trayectoria*/
							SendDlgItemMessage(hDlg,IDC_PUNTOSUSER, LB_ADDSTRING, 0, (LPARAM)ActualID);
							/*Se añade la etiqueta a el vector final*/
							Actualizar_Vector(P_pVectorFINALIDs,2,PointID);						
							PuntosProgramados++;
							//Borar
							tempVector=*P_pVectorFINALIDs;
							SetDlgItemInt(hDlg,IDC_PUNTOACTUAL,PuntosProgramados,FALSE);
						}
					}
					else
						MessageBox(hDlg, "Debe seleccionar un Identificador de Pose (ID)", "Información", MB_OK);
				break;
				/*Simplemente se manda la secuencia de puntos deseada por el usuario*/
				case IDC_GENTRACJ:
					if(PuntosProgramados>=2)
					{
						tempVector=*P_pVectorFINALIDs;
						rc=dispatchPacket(PacketPosesvector,(byte*)tempVector,Devicename,hDlg);
						if(rc!=rcOK)
						{
							if(rc!=rcCommunicationError)
							{
								Abortar(hDlg,0,Devicename);
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
								EndDialog(hDlg, FALSE);
							}
						}
						else
							Robot_MOVE=1;
					}
					else 
					MessageBox(hDlg, "Para Generar una Trayectoria se necesitan por los menos 2 poses programadas", "Información", MB_OK); //significa que hubo un problema del proceso de 
				break;
				case IDC_DELETE:
					if(PuntosProgramados!=0)
					{
							tempVector=*P_pVectorFINALIDs;
							/*Borramos un miembro de la cadena*/
							if(tempVector[0]>2)						
							    Actualizar_Vector(P_pVectorFINALIDs,3,0);	
							else 
								tempVector[1]=-1;
							/*Borramos la parte grafica del cuadro de trayectorias*/
							SendDlgItemMessage(hDlg,IDC_PUNTOSUSER,LB_DELETESTRING,(PuntosProgramados-1) , (LPARAM)temp);
							PuntosProgramados--;
							/*Actualizamos lo puntos programados*/
							SetDlgItemInt(hDlg,IDC_PUNTOACTUAL,PuntosProgramados,FALSE);
					}
				break;
				case ID_CANCELPROGRAMA:
				case IDCANCEL: 
                    EndDialog(hDlg, FALSE);
                break;
			}
		return TRUE;  //SIGO CON LA VENTANA
    }
	return FALSE; //TERMINO LA VENTANA*/
}



/********************************************************************************************************************/
BOOL CALLBACK SETRobot(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
	static HINSTANCE hInstance;
	char DeviceName[MAX_DRIVER_NAME]   = "EZUSB-0";   //Por defaul
	Listapara *Param=(Listapara *)declarar(sizeof(Listapara));	
	extern tAxisSettings rxAxisSettings[DOF];
	int rc;

	
	int i;

    switch (msg)                  /* manipulador del mensaje */
    {
		case WM_INITDIALOG:
			Param=(Listapara *)lParam;
			hInstance = Param->Instancia; 
			/*Cintura*/
			CBoxAJUSTE(hDlg,IDC_CBOXuPASO,IDC_CBOXPOT,IDC_SWTENABLE1);
			/*Hombro*/
			CBoxAJUSTE(hDlg,IDC_CBOXuPASO2,IDC_CBOXPOT2,IDC_SWTENABLE2);
			/*Codo*/
			CBoxAJUSTE(hDlg,IDC_CBOXuPASO3,IDC_CBOXPOT3,IDC_SWTENABLE3);
			/*Pich*/
			CBoxAJUSTE(hDlg,IDC_CBOXuPASO4,IDC_CBOXPOT4,IDC_SWTENABLE4);
			/*Roll*/
			CBoxAJUSTE(hDlg,IDC_CBOXuPASO5,IDC_CBOXPOT5,IDC_SWTENABLE5);
			FillAxisTracePMD1(hDlg,IDC_EJEMUESTRA);
			SendDlgItemMessage(hDlg,IDC_EJEMUESTRA, CB_ADDSTRING, 0, (LPARAM)"Roll");
        return FALSE;
		case WM_COMMAND:
            switch(LOWORD(wParam)) 
		    {		
				case IDC_ENCODER_EN1:
					manejador_camposSetRobot(hDlg,IDC_ENCODER_EN1);
				break;
				case IDC_ENCODER_EN2:
					manejador_camposSetRobot(hDlg,IDC_ENCODER_EN2);
				break;
				case IDC_ENCODER_EN3:
					manejador_camposSetRobot(hDlg,IDC_ENCODER_EN3);
				break;
				case IDC_ENCODER_EN4:
					manejador_camposSetRobot(hDlg,IDC_ENCODER_EN4);
				break;
				case IDC_ENCODER_EN5:
					manejador_camposSetRobot(hDlg,IDC_ENCODER_EN5);
				break;
		        case IDC_SETROBODATA: 
					//Inicializo a default
					for(i=0; i<DOF; i++) 
					/*Coloca la Información por defecto*/
					defaultAxisSettings(i,&(axisSettings[i]));
					/*Sobreescribe la informacio por defecto con lo nuevos datos  de Cintura*/
					GetCBoxAJUSTE(hDlg,&(axisSettings[0]),IDC_CBOXuPASO,IDC_CBOXPOT,IDC_SWTENABLE1,IDC_ENCODER_EN1,IDC_CBOXERROR,IDC_CBOXTRACKWIN,IDC_CBOXSETTLEW,IDC_CBOXSETTLET);
					/*Lo mismo para Hombro*/
					GetCBoxAJUSTE(hDlg,&(axisSettings[1]),IDC_CBOXuPASO2,IDC_CBOXPOT2,IDC_SWTENABLE2,IDC_ENCODER_EN2,IDC_CBOXERROR2,IDC_CBOXTRACKWIN2,IDC_CBOXSETTLEW2,IDC_CBOXSETTLET2);
					/*Lo mismo para Codo*/
					GetCBoxAJUSTE(hDlg,&(axisSettings[2]),IDC_CBOXuPASO3,IDC_CBOXPOT3,IDC_SWTENABLE3,IDC_ENCODER_EN3,IDC_CBOXERROR3,IDC_CBOXTRACKWIN3,IDC_CBOXSETTLEW3,IDC_CBOXSETTLET3);
					/*Lo mismo para Pich*/
					GetCBoxAJUSTE(hDlg,&(axisSettings[3]),IDC_CBOXuPASO4,IDC_CBOXPOT4,IDC_SWTENABLE4,IDC_ENCODER_EN4,IDC_CBOXERROR4,IDC_CBOXTRACKWIN4,IDC_CBOXSETTLEW4,IDC_CBOXSETTLET4);
					/*Lo mismo para Roll*/
					GetCBoxAJUSTE(hDlg,&(axisSettings[4]),IDC_CBOXuPASO5,IDC_CBOXPOT5,IDC_SWTENABLE5,IDC_ENCODER_EN5,IDC_CBOXERROR5,IDC_CBOXTRACKWIN5,IDC_CBOXSETTLEW5,IDC_CBOXSETTLET5);
					rc=dispatchPacket(PacketAxisSettings,(byte*)&(axisSettings[0]),DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else
						SET_ROBOT=1;
				break;
				case IDC_READROBODAT:
					/*Configuro el commando para mandarlo*/
					txCommand.CommandOP=CommandGetAxisSettings;		       
					/*Mando por usb*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else 
					{  
						/*Envie bien el commando*/
						if(rc==rcOK)
						{
							Sleep(300);
							rc=receiptPacket(DeviceName,hDlg);
							if(rc==rcOK)
							{
								/*Cintura*/
								SetCBoxAJUSTE_USB(hDlg,&(rxAxisSettings[0]),IDC_CBOXuPASO,IDC_CBOXPOT,IDC_SWTENABLE1,IDC_ENCODER_EN1,IDC_CBOXERROR,IDC_CBOXTRACKWIN,IDC_CBOXSETTLEW,IDC_CBOXSETTLET);
								/*Hombro*/
								SetCBoxAJUSTE_USB(hDlg,&(rxAxisSettings[1]),IDC_CBOXuPASO2,IDC_CBOXPOT2,IDC_SWTENABLE2,IDC_ENCODER_EN2,IDC_CBOXERROR2,IDC_CBOXTRACKWIN2,IDC_CBOXSETTLEW2,IDC_CBOXSETTLET2);
								/*Codo*/
								SetCBoxAJUSTE_USB(hDlg,&(rxAxisSettings[2]),IDC_CBOXuPASO3,IDC_CBOXPOT3,IDC_SWTENABLE3,IDC_ENCODER_EN3,IDC_CBOXERROR3,IDC_CBOXTRACKWIN3,IDC_CBOXSETTLEW3,IDC_CBOXSETTLET3);
								/*Pich*/
								SetCBoxAJUSTE_USB(hDlg,&(rxAxisSettings[3]),IDC_CBOXuPASO4,IDC_CBOXPOT4,IDC_SWTENABLE4,IDC_ENCODER_EN4,IDC_CBOXERROR4,IDC_CBOXTRACKWIN4,IDC_CBOXSETTLEW4,IDC_CBOXSETTLET4);
								/*Roll*/
								SetCBoxAJUSTE_USB(hDlg,&(rxAxisSettings[4]),IDC_CBOXuPASO5,IDC_CBOXPOT5,IDC_SWTENABLE5,IDC_ENCODER_EN5,IDC_CBOXERROR5,IDC_CBOXTRACKWIN5,IDC_CBOXSETTLEW5,IDC_CBOXSETTLET5);
							}
							else
							{
								MessageBox(hDlg,"El Controlador no envió la Configuración actual. Itente de nuevo.", "Información", MB_OK);
								Abortar(hDlg,1,DeviceName);
							}									
						}
					}
				break;
				////---NUEVA ADICION 28 MAY 2010 -----///
				case ID_GetSensor: 
					txCommand.CommandOP=CommandGetSensor;		 
					txCommand.Axis= SendDlgItemMessage(hDlg,IDC_EJEMUESTRA,CB_GETCURSEL,0,0);
					/*Mando por usb*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
				break;
				case ID_EventStatus: 
					txCommand.CommandOP=CommandGetEventStatus;	
					txCommand.Axis= SendDlgItemMessage(hDlg,IDC_EJEMUESTRA,CB_GETCURSEL,0,0);
					/*Mando por usb*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
				break;
				case ID_AvtivityStatus: 
					txCommand.CommandOP=CommandGetAvtivityStatus;
					txCommand.Axis= SendDlgItemMessage(hDlg,IDC_EJEMUESTRA,CB_GETCURSEL,0,0);
					/*Mando por usb*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
				break;
				case ID_CommandGetinfomov: 
					txCommand.CommandOP=CommandGetinfomov;
					txCommand.Axis= SendDlgItemMessage(hDlg,IDC_EJEMUESTRA,CB_GETCURSEL,0,0);
					/*Mando por usb*/
					rc=dispatchPacket(PacketCommand,(byte*)&txCommand,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
				break;
				////-------------/////
				case IDCANCEL: 
					EndDialog(hDlg, FALSE);
				break;
			}
        return TRUE;
    }
    return FALSE;
}


/********************************************************************************************************************/
BOOL CALLBACK SETRTC(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
	static HINSTANCE hInstance;
	char DeviceName[MAX_DRIVER_NAME]   = "EZUSB-0";   //Por defaul
	Listapara *Param=(Listapara *)declarar(sizeof(Listapara));
	static tFecha Fecha;
	static 	tAlarm txAlarma;
	int rc;

    switch (msg)                  /* manipulador del mensaje */
    {
		case WM_INITDIALOG:
			Param=(Listapara *)lParam;
			hInstance = Param->Instancia; 
			cargoBotonRTC(hDlg,hInstance);
			CBoxRTC(hDlg);
        return FALSE;
		case WM_COMMAND:
            switch(LOWORD(wParam)) 
		    {              
				case IDC_SETHORA:
					/*Leeo de la interfaz Grafica y lo guardo en Fecha*/
					SetFechaVisual(hDlg,&Fecha);
					rc=dispatchPacket(PacketFecha,(byte*)&Fecha,DeviceName,hDlg);
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
						
					}
				break;
                case IDC_LLEERHORA:
					/*Configuro el commando para mandarlo*/
				    txCommand.CommandOP=CommandGetFecha;		       
					/*Mando por usb*/
                    rc=dispatchPacket(PacketCommand,(byte*)&txCommand,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}
					}
					else 
					{   
						/*Mande el commando correctamente*/
						if(rc==rcOK)
						{
							/*Espero un segundo a que uC lee el comando CommandGetPosesID */
							Sleep(100);
							/*Una vez lleido el comando uC esta listo para pedirle los IDs */
							rc=receiptPacket(DeviceName,hDlg);
							if(rc==rcOK)
								/*Leeo desde rxFecha y lo escribo*/
								getFechaVisual(hDlg);
							else
							{
							MessageBox(hDlg,"El Controlador no envió la Fecha. Intente más tarde", "Información", MB_OK);
							Abortar(hDlg,1,DeviceName);
							}
							
						}
					}
				break;
				case IDC_SETALARMA:
					SetAlarma(hDlg,&txAlarma);
					/*Mando por usb*/
					rc=dispatchPacket(PacketAlarma,(byte*)&txAlarma,DeviceName,hDlg); 
					if(rc!=rcOK)
					{
						if(rc!=rcCommunicationError)
						{
							Abortar(hDlg,0,DeviceName);
							MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
						}	
					}				
				break;
				case IDCANCEL: 
                    EndDialog(hDlg, FALSE);
                break;
            }
        return TRUE;
    }
    return FALSE;
}



/********************************************************************************************************************/
BOOL CALLBACK SETTRACE(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND  MAKEbufferboton= GetDlgItem (hDlg,IDC_MAKEBUFFER);
	Listapara *Param=(Listapara *)declarar(sizeof(Listapara));
	Matrix t(1,200),y(1,200);
	extern  int32     *rxTraceBuffer;
	static  HINSTANCE hInstance;
	static  int ConfiguracionOK=0;
	char    DeviceName[MAX_DRIVER_NAME]   = "EZUSB-0";   //Por defaul
	int     rc;
	int     N_varialbles_visual=0;
	int     TraceIndex=0;
   

    switch (msg)                  /* manipulador del mensaje */
    {
		case WM_INITDIALOG:
			Param=(Listapara *)lParam;
			hInstance = Param->Instancia;   
			CBoxTRACE(hDlg);
			if(ConfiguracionOK)
				EnableWindow (MAKEbufferboton, TRUE);
        return FALSE;
		case WM_COMMAND:
            switch(LOWORD(wParam)) 
		    {   
				case IDC_PMDTrace1:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace1);
				break;
				case IDC_PMDTrace2:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace2);
				break;
				case IDC_PMDTrace3:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace3);
				break;
				case IDC_PMDTrace4:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace4);
				break;           
				case IDC_PMDTrace5:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace5);
				break;           
				case IDC_PMDTrace6:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace6);
				break;           
				case IDC_PMDTrace7:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace7);
				break;           
				case IDC_PMDTrace8:
					manejador_camposSetTRACE(hDlg,IDC_PMDTrace8);
				break;           
				case IDC_CONFIGURARTRACE:
					GetCBoxTRACE(hDlg,&ComandTrace,&GraficoTraceSampletime);
					/*Obtengo el numero de variables configuradas y actualiza el conteo de variables para PMD1 y PMD2*/
					/*Este conteo servira a la hora procesar los datos muestreados para graficarlos*/
					N_varialbles_visual=GetNvariables(hDlg,ComandTrace,&PMD1NVariables,&PMD2NVariables);
					if(N_varialbles_visual>0)
					{
						ComandTrace.CommandOP=CommandSetPMDTrace;
						ComandTrace.Dato=N_varialbles_visual;
						/*Modifica el Vector para que no tengo espacios vacios, organiza PMD1-PMD2*/
						FixVector(&ComandTrace);					
						/*Mando por usb*/
						rc=dispatchPacket(PacketCommand,(byte*)&ComandTrace,DeviceName,hDlg); 
						if(rc!=rcOK)
						{
							if(rc!=rcCommunicationError)
							{
								Abortar(hDlg,0,DeviceName);
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
							}
						}				
						else
						{
							
							ConfiguracionOK=1;
						}
					}
					else
						MessageBox(hDlg, "Debe Configurar por lo menos un trazado", "Información", MB_OK);
				break;
				case IDC_MAKEBUFFER:
					/*El boton de MAKEBUFFER permanece desabilitado si no se configura una variable*/
					if(Robot_MOVE==1) /*Ya Movi el robot*/
					{
						ComandTrace.CommandOP=CommandGetPMDTrace;
						/*Mando por usb*/
						rc=dispatchPacket(PacketCommand,(byte*)&ComandTrace,DeviceName,hDlg); 
						if(rc!=rcOK)
						{
							if(rc!=rcCommunicationError)
							{	
								Abortar(hDlg,0,DeviceName);
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
							}
						}
						else /*Mande el comando correctamente*/
						{	
							Sleep(500);
							rc=receiptPacket(DeviceName,hDlg);
							/*Si llego bien la Información tenemos que graficar*/
							if(rc==rcOK)
							{
								/*Hay Datos Muestreados en PMD1*/
								if(rxTraceBuffer[1]>0)
								{
									GraficTraceData(rxTraceBuffer[1],PMD1NVariables,&TraceIndex,1);
									EnableWindow (MAKEbufferboton,FALSE);
									Robot_MOVE=1;
								}	
								/*Hay Datos Muestrados en PMD2*/
								if(rxTraceBuffer[2]>0)
								{
									GraficTraceData(rxTraceBuffer[2],PMD2NVariables,&TraceIndex,2);
									EnableWindow (MAKEbufferboton,FALSE);
									Robot_MOVE=1;
								}
							}
							else /*Si no recivi correctamente*/
							{
								MessageBox(hDlg, "El controlador está ejecutando un proceso actualmente. Intente más tarde", "Información", MB_OK); //significa que hubo un problema del proceso de 
								Abortar(hDlg,1,DeviceName);
								break;
							}
						}
					}
					else
						MessageBox(hDlg,"Antes de graficar el robot tiene que ejecutar algún movimiento o trayectoria", "Información", MB_OK);				
				break;
				case IDCANCEL: 
                    EndDialog(hDlg, FALSE);
                break;
            }
        return TRUE;
    }
    return FALSE;
}



/********************************************************************************************************************/
/*
Ejemplo:

  Dentro la estructura Command la variable Tracecomand esta organizada en esta forma

                PMD1-Variable0    PMD2-Variable1                          PMD2-Variable0
  Tracecomand[CommandTraceINDEX] Tracecomand[CommandTraceINDEX] Tracecomand[CommandTraceINDEX]
	                 0                            1                                2
	  
*/
void GraficTraceData(int32 log,int NVariablesGraficar,int * TraceINDEX,int PMDId)
{
	int i,col,j,Axis;
	Matrix t(1,200),y(1,200);
	char    VarTitle[30];
	char    VarTipo[10];	


    /*Cargamos los parametros por defector del robot*/
	setupAxisSettings(&axisSettings[0]);
	/*Cantidad Total de datos*/
	if(PMDId==1)
		log = rxTraceBuffer[1]; 
	else
		log = rxTraceBuffer[2];
	/*Cantidad de datos muestreados para cada variable*/
	log = log/NVariablesGraficar;
    /*Creamos el vector de tiempo*/
	t.resize(1,log);
	t=0.0;
	/*Llenamos el tiempo*/
	for(col=1;col<=log;col++)
		t(1,col) = col*GraficoTraceSampletime;
	/*Creamos el vector*/
	y.resize(1,log);
	y=0.0;			
	/*Extraemos los datos para cada variable del mismo PMD*/
	for(i=(*TraceINDEX);i<(NVariablesGraficar+(*TraceINDEX));i++)
	{
		/*Los datos empienzan el casilla 3*/
		if(PMDId==1)
			col=i+3;
		else
			col=i+3+rxTraceBuffer[1]-PMD1NVariables;
		/*Recorremos todos los datos muestreados para la variable N*/
		for(j=1;j<=log;j++)
		{				
			Axis=ComandTrace.Tracecomand[i].AxisTrace;
			//y(1,j) =rxTraceBuffer[col];
			y(1,j)=uPasosTOgradroGraficos(&axisSettings[Axis],rxTraceBuffer[col],ComandTrace.Tracecomand[i].TraceVar);
			/*Nos desplazamos solos en datos de una misma variable*/
			if(PMDId==1)
				col=col+PMD1NVariables;
			else
				col=col+PMD2NVariables;
		}
		/*i empieza en cero*/
		sprintf(VarTitle,"%s %s",AxisName(Axis),TraceName((PMDTraceVariable)ComandTrace.Tracecomand[i].TraceVar));
		sprintf(VarTipo," %s",Tracetype((PMDTraceVariable)ComandTrace.Tracecomand[i].TraceVar));
		/* LINES,DATAPOINTS,   LINESPOINTS,IMPULSES,    DOTS,STEPS,    BOXES*/
		set_plot2d(VarTitle, "time (sec)",VarTipo, "q", LINES,t,y, 1, 1);
		/*Aumentamos el CommandTraceINDEX para indicar que  */
	}	
	*TraceINDEX=*TraceINDEX+PMD1NVariables;
}

/********************************************************************************************************************/
BOOL CALLBACK DISPLAYMATIZ(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
	static HINSTANCE hInstance;
	HWND hOutputBox;
	int fila;
	Listapara *Param=(Listapara *)declarar(sizeof(Listapara));
	char tempbuff[1000];
	

    switch (msg)                  /* manipulador del mensaje */
    {
		case WM_INITDIALOG:
			Param=(Listapara *)lParam;
			hInstance = Param->Instancia; 
			hOutputBox = GetDlgItem (hDlg,IDC_MATRIX);
			if(Param->Tsize==1)
			{
				for (fila=1;fila<5;fila++)   
				{
          			sprintf(tempbuff,"    %f     %f      %f     %f",
					Homogeniakine(fila,1),
					Homogeniakine(fila,2),
					Homogeniakine(fila,3),
					Homogeniakine(fila,4));
					SendMessage (hOutputBox, LB_ADDSTRING, 0, (LPARAM)tempbuff); 			   
				}
			}
			else
			{
				for (fila=1;fila<5;fila++)   
				{
          			sprintf(tempbuff,"    %f     %f      %f     %f",
					Homogeniainvkine(fila,1),
					Homogeniainvkine(fila,2),
					Homogeniainvkine(fila,3),
					Homogeniainvkine(fila,4));
					SendMessage (hOutputBox, LB_ADDSTRING, 0, (LPARAM)tempbuff); 			   
				}

			}
        return FALSE;
		case WM_COMMAND:
            switch(LOWORD(wParam)) 
		    {              
				case IDCANCELMATRIX: 
                    EndDialog(hDlg, FALSE);
                break;
				case IDCANCEL: 
                    EndDialog(hDlg, FALSE);
                break;
            }
        return TRUE;
    }
    return FALSE;
}

