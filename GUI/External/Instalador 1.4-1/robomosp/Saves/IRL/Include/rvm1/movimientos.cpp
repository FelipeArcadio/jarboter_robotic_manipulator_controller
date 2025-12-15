#include <movimientos.h>

void MovPointToPoint(AT parametros)
 {
     int tam = posiciones->Listlength();
     static char* mensaje[];
         
     for(i=1; i<=tam; i++){         
         sprintf(mensaje,"PD %d, %.0f, %.0f, %.0f, %.0f, %.0f\r\n",point->posNumber, point->valor[0],point->valor[1],point->valor[2],point->valor[3],point->valor[4]);
         if(!TransmisionSerial(mensaje))
             exit -1;    
     }         
     if(posiciones->Listlength() > 1){                
         punto * point1 = (*posiciones)[1];
         punto * point2 = (*posiciones)[tam];         
         sprintf(mensaje,"MS %.0f, %.0f, %c\r\n", point1->posNumber,point2->posNumber,point1->grip);          
     }
     else{ 
        point = (*posiciones)[1];
        if(point->tipo == CTARGET)	
            sprintf(mensaje,"MO %d %c\r\n",point->posNumber,point->grip);
        else if(point->tipo == CJOINT)
            sprintf(mensaje,"MJ %.0f,%.0f,%.0f,%.0f,%.0f\r\n",point->valor[0],point->valor[1],point->valor[2],point->valor[3],point->valor[4]);            
        else
            sprintf(mensaje,"G%c\r\n",point->grip);                
    }    
     posiciones->Listdestroy();
     if(!TransmisionSerial(mensaje))
         exit -1;
 }

int TransmisionSerial(static char* mensaje[])
 {
     CSerial serial; /*Puerto de comunicacion serial*/ 
     int size; 
     
     if (!serial.Open(port, 9600)){
            cout<<"Falla abriendo el puerto de comunicacion #"<<port<<endl;
            return 0;
     }
     size = strlen(mensaje);
     if(!serial.SendData(mensaje, strlen(mensaje)) == size){  
             cout<<"Error enviando los datos por el puerto #"<<port<<endl; 
             return 0;
    }
    return size;                
 }