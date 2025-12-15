#include "Procesamiento_archivos.h"


/*****************************************************************************************/
void Get_dimension_trayec(char NArchivo [], pDIMEN tdimen)
{
    
	int i=0,Nunfilas,Ncolum=0,Maxj=0,NunData=0;
	char datos[18]="",tempChar[18]="";					  //inicializacion de la cadana
	FILE* pArchivo=NULL;
	DWORD dwSize;
	HANDLE hFile;
	DWORD dwNumRead;
	BOOL bTest;
	LPSTR pszFileText;

   
	hFile = CreateFile(NArchivo, GENERIC_READ, FILE_SHARE_READ,
                   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	dwSize = GetFileSize (hFile, NULL); 
	pszFileText = GlobalAlloc(GPTR,dwSize + 1);
	bTest=ReadFile(hFile, pszFileText, dwSize, &dwNumRead, NULL);
	if(bTest == TRUE)            //Si abre bien
	{ 
		do
		{
			memset(datos,0,18);
			while (!isspace(pszFileText[i]))                     //Si datos ==0 significa que he llegado hasta el tiempo 1
			{
				tempChar[0]=pszFileText[i];
				strcat(datos,tempChar);    
				i++;
			}
			if(!isspace(pszFileText[i-1]))
				Ncolum++; //Ncolum guarda el numero de columnas desde 1          
			i++;
		}
		while (datos[0] !='0');  //Si datos ==0 significa que he llegado hasta el tiempo 1                   
			    			   
		do                                       
		{   
			memset(datos,0,18);
			while (!isspace(pszFileText[i]))                     
			{
				tempChar[0]=pszFileText[i];
				strcat(datos,tempChar);    
				i++;
			}
			if(!isspace(pszFileText[i-1]))          // La pimera vez	que sale esta en el primer dato
				NunData++;					 
			i++;
		}
		while (i<(int)dwSize);                  
		NunData++;  //uno mas por que no conta el primer valor
		
		Nunfilas=NunData/(Ncolum-2);                   // 1 del nº5 y 1 del 0.02 pero se incluye del 0.01
        tdimen->puntos=Nunfilas;
        tdimen->col=Ncolum-2;
	    tdimen->Numdata=NunData;
		tdimen->Maxjoint=(Ncolum-3)/3;          // 1 del nº5  1 del 0.01 1 del 0.02 luego div entre las tres variables
	}
	CloseHandle(hFile);
}

/*****************************************************************************************/
//pdatos es para un archivo simple 
//numeros es para una tipo trayectoria
//modicado 1
int Obtener_datosFILE(HWND hDlg,char NArchivo [],double* numeros,pDIMEN Dimen)
{
	int i=0,DataIndex=0;
	char datos[18]="",tempChar[18]="";					  //inicializacion de la cadana
	FILE *dumpfile=NULL;
	DWORD dwSize;
	HANDLE hFile;
	DWORD dwNumRead;
	BOOL bTest;
	LPSTR pszFileText,Numeros;
  

	hFile = CreateFile(NArchivo, GENERIC_READ, FILE_SHARE_READ,
                   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   
   
	if (hFile == INVALID_HANDLE_VALUE)
		MessageBox(hDlg,"Error: El archivo no se pudo abrir.","Información",MB_RIGHT);
	dwSize = GetFileSize (hFile, NULL); 
	pszFileText = GlobalAlloc(GPTR,dwSize + 1);
	Numeros = GlobalAlloc(GPTR,dwSize + 1);

	bTest=ReadFile(hFile, pszFileText, dwSize, &dwNumRead, NULL);
    if(bTest == TRUE)            //Si abre bien
	{ 
		do
		{
			memset(datos,0,18);
			while (!isspace(pszFileText[i]))                     //Si datos ==0 significa que he llegado hasta el tiempo 1
			{
				tempChar[0]=pszFileText[i];
				strcat(datos,tempChar);    
				i++;
			}
			i++;
		}
		while (datos[0] !='0');  //Si datos ==0 significa que he llegado hasta el tiempo 1   
		i=i-9;              //Me regreso para poder grabar el primer tiempo 
		do                                       
		{   
			memset(datos,0,18);
			while (!isspace(pszFileText[i]))                     
			{
				tempChar[0]=pszFileText[i];
				strcat(datos,tempChar);    
				i++;
			}
			if(!isspace(pszFileText[i-1]))          // La pimera vez	que sale esta en el primer dato
			{
				numeros[DataIndex]=atof(datos);	
				DataIndex++;
			}					 
			i++;
		}
		while (i<(int)dwSize);                  
		CloseHandle(hFile);
        return rcFileReadOK;
    }	    

	MessageBox(hDlg,"Error: El archivo no se pudo leer.","Información",MB_RIGHT);
	return  rcFileReadError;
}

/*****************************************************************************************/
//Join representa a que axis
//datos Joint Contenera la informacion de posicion, velocidad y aceleracion para todos 
//puntos de una axis determinada.
void ObtenerProfile(pPROFILE datosJoint,int Joint,const pDIMEN dimem,double* numeros)
{
	int i=0,J=0,a,v,p,offset;

	if(Joint<dimem->Maxjoint)
	{
		J=0;												//J representa los puntos 
		for(i=0;i<dimem->Numdata;i++)                  //recorro todos los datos
		{
			datosJoint[J].Tiempo=numeros[i];		//grabo el primer dato (TIEMPO)
			for(p=i;p<Joint+i;p++)
				numeros[p];                         //me muevo hacia al axis correcta
			datosJoint[J].pos=numeros[p+1];	 //lo grabo la posicion para esa Joint
			for(v=p;v<(dimem->Maxjoint+p);v++)
				numeros[v];                         //me muevo hacia el valor de velocidad
			datosJoint[J].velo=numeros[v+1];    //grabo la velocidad para esa Joint
			for(a=v;a<(dimem->Maxjoint+v);a++)
				numeros[a];                         //me muevo hacia el valor de aceleracion 
			datosJoint[J].ace=numeros[a+1];     //graba la aceleracion para esa Joint
			if(J==0)
				offset=abs(dimem->col-a)-1;        //
			i=a+offset;                          //me  nuevo hacie el borde para pasar a la 
							                                     //siguiente fila para acapturar el
																 //el proximo tiempo (el siquiente punto)
			if(J<dimem->puntos)
				J++;                             //aumento del punto                
			if(J==dimem->puntos)                //si he llegado al final fuerzo la 
				i=dimem->Numdata;               //terminacion del for principal
		}
	}					
}


/*****************************************************************************************/
//Comprueba si el archivo que es esta abriendo  exite en disco, basandose en su tipo 
int Existe_Arhivo(HWND hDlg,char NArchivo [])
{
	FILE *dumpfile=NULL;
	HANDLE hFile;
  	 

	hFile = CreateFile(NArchivo, GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{ 
		CloseHandle(hFile);
		return  rcFileOpenOK;
	}
	
	MessageBox(hDlg, "El Archivo no existe", "Advertencia", MB_OK);
	return rcFileOpenError;
}

