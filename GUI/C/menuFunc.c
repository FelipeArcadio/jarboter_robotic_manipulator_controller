#include "menuFunc.h"


/*****************************************************************************************/

void InsertarBotonBMP(HMENU hmenu,HINSTANCE hInstance, HWND hwnd,WORD wInteger,UINT uID,UINT Item)
{
    HBITMAP hBitmap;
	HBITMAP hBMOpcion;
    HBITMAP hBMv1, hBMv2;
    HBRUSH pluma, hpant;
	HGDIOBJ hGDI;
    HFONT hfont;
    HDC memDC;  //Disposito en memoria virtual para poder acceder a un mapa de bits
    HDC memDCdest;
    HDC hDC;
	
	

    MENUITEMINFO infoMenu;

    char plantilla[64];
    
            
	//A device context is a structure that defines a set of graphic objects and their associated 
	//attributes, as well as the graphic modes that affect output. The graphic objects include a
	//pen for line drawing, a brush for painting and filling, a bitmap for copying or scrolling 
	//parts of the screen, a palette for defining the set of available colors, a region for 
	//clipping and other operations, and a path for painting and drawing operations. 
    //El contexto del Dispositiovo DC, significa  el tipo de salida para el motor grafico
	// DC impresora, DC memoris, DC pantalla.
    hDC = GetDC(hwnd);  // Obtiene el contexto  generico del dispositivo a partir el handle de la ventana
    memDC = CreateCompatibleDC(hDC);  //Inicializa el DC comparatible con la resolucion de la pantala actual 
    memDCdest = CreateCompatibleDC(hDC);
    pluma = CreateSolidBrush(GetSysColor(COLOR_MENU)); //Crea un pincel para rellenar superficies
    hpant = SelectObject(memDC, pluma); //Solo puede seleccionar un contexto de dispositivo
										// a la vez, y se selecciona para poder cambiar sus 
										//atributos o trabajar con el. 
										// EN ESTE CASO SE RELLENA EL RECUADRO

    
    // Seleccionar una fuente de ancho proporcional:

    hfont = CreateFont(-8 * GetDeviceCaps(memDC, LOGPIXELSY) / 72, // 28
	0, 0, 0, FW_BOLD,FALSE, FALSE, FALSE,
	ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
	PROOF_QUALITY, VARIABLE_PITCH, "Arial");

	hGDI=SelectObject(memDC, hfont);
    // Y el modo transparente:
    SetBkMode(memDC, TRANSPARENT);

    // Crear mapas de bits: un especio para un mapa de bit 100 x 55 dentro 
	//del hDC y devuelve el apuntador a dicho espacio
    hBMOpcion = CreateCompatibleBitmap(hDC,90,105);
    // Cargar mapas de bits de opciones de 48x48 px:
    hBitmap = LoadBitmap(hInstance,MAKEINTRESOURCE(wInteger)); //IDB_BITMAP
    // Seleccionamos la seccion del mapa de bit dentro del hdc
    hBMv1 = SelectObject(memDC,hBMOpcion);
	// Fondo de color de menú:
    ExtFloodFill(memDC, 0, 0, GetSysColor(COLOR_MENU), FLOODFILLBORDER);   
    // Seleccionar el mapa de bits de recurso en otro DC:
    hBMv2 = SelectObject(memDCdest, hBitmap); // hbitmap tiene el bmp   
    // Copia el rectángulo de origen directamente en el rectángulo de destino.
	if(wInteger==IDB_RESET)
		BitBlt(memDC,10, 10, 80, 70, memDCdest, 0, 0, SRCCOPY);
	else
		BitBlt(memDC,5, 10, 80, 70, memDCdest, 0, 0, SRCCOPY);
	//carga el texto desde el recurso hasta la cadena plantilla que tiene 64 bytes
    LoadString(hInstance, uID, plantilla, 64);
	switch(uID)
	{
		case ID_ITEMOPCION00:  //TERMINAL
			TextOut(memDC,20, 80, plantilla, lstrlen(plantilla));  //x,y
			LoadString(hInstance,ID_ITEMOPCION07, plantilla, 64);
			TextOut(memDC,12,90, plantilla, lstrlen(plantilla));  //x,y
		break;
		case ID_ITEMOPCION01: //EMERGENCIA
		break;
		case ID_ITEMOPCION02: //RESET
			TextOut(memDC,25, 80, plantilla, lstrlen(plantilla));  //x,y
		break;

		case ID_ITEMOPCION03: //TRAYECTORIA
			TextOut(memDC,10, 80, plantilla, lstrlen(plantilla));  //x,y
		break;

		case ID_ITEMOPCION04: //CONFIGURACION
			TextOut(memDC,2, 80, plantilla, lstrlen(plantilla));  //x,y
		break;
		case ID_ITEMOPCION05: //TAREAS
			TextOut(memDC,30, 80, plantilla, lstrlen(plantilla));  //x,y
			LoadString(hInstance,ID_ITEMOPCION08, plantilla, 64);
			TextOut(memDC,7,90, plantilla, lstrlen(plantilla));  //x,y
		break;		
		case ID_ITEMOPCION06: //TRAZADO
			TextOut(memDC,20, 80, plantilla, lstrlen(plantilla));  //x,y
		break;		

	}

	infoMenu.cbSize = sizeof(MENUITEMINFO);
    infoMenu.fMask = MIIM_TYPE | MIIM_ID;
    infoMenu.fType = MFT_BITMAP; 
    infoMenu.wID = Item;//IDC_TECHBOX;
    infoMenu.dwTypeData = (LPSTR)hBMOpcion; 
    
	InsertMenuItem(hmenu, 0, TRUE, &infoMenu);
	// Ya no lo necesitamos más el apuntador a le BMP
    DeleteObject(hBitmap);
	// Ni tampoco el DC de destino que contenia a el BMP:
    SelectObject(memDCdest, hBMv2);       
    // Seleccionar pluma original:
    SelectObject(memDC, hpant);
    // Seleccionar fuente previa:
    SelectObject(memDC, hfont);
    // Borrar los DC de memoria:
    DeleteObject(memDC);
    DeleteDC(memDCdest);
    // Liberar el DC compatible:
	ReleaseDC(hwnd, hDC);
   
}

/*****************************************************************************************/
void InsertarMenu(HWND hwnd,HINSTANCE hInstance)
{

	HMENU hMenu1;
   
	hMenu1 = CreateMenu();

	/*Ultimo cuador*/
	InsertarBotonBMP(hMenu1,hInstance,hwnd,IDB_RESET,ID_ITEMOPCION02,ID_RESET);
	InsertarBotonBMP(hMenu1,hInstance,hwnd,IDB_AJUSTE,ID_ITEMOPCION04,ID_SETROBOT);
	InsertarBotonBMP(hMenu1,hInstance,hwnd,IDB_ALARMA,ID_ITEMOPCION05,IDC_SETRTC);
	InsertarBotonBMP(hMenu1,hInstance,hwnd,IDB_GRAFICA,ID_ITEMOPCION06,IDC_SETTRACE);
	InsertarBotonBMP(hMenu1,hInstance,hwnd,IDB_TECHBOX,ID_ITEMOPCION00,ID_TECHBOX);
	/*Primer cuadro*/
	InsertarBotonBMP(hMenu1,hInstance,hwnd,IDB_TRAJ,ID_ITEMOPCION03,ID_TIPOTRAYECTORIA); 

	SetMenu (hwnd, hMenu1);  

}



