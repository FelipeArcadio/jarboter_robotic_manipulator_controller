#include <string.h>

typedef double REAL64;
typedef double REAL32;
typedef long INT32;
typedef char CHAR;

typedef struct angle
{
	REAL32 s1;
	REAL32 s2;
	REAL32 s3;
	REAL32 e1;
	REAL32 e2;
	REAL32 w1;
	REAL32 w2;
}ANGLE, *ANGLEP;

typedef double MATRIX[3][4];

#define pa_ini_sys() cout <<"iniciando libreria PA-10"<<endl
#define pa_ter_sys() cout <<"terminando libreria PA-10"<<endl
#define FALSE false
#define TRUE true
#define MM_XYZNOA 8
#define WM_WAIT 1
#define S1 1
#define S2 2
#define S3 4
#define E1 8
#define E2 16
#define W1 32
#define W2 64
#define pa_mov_mat(arm,MM_XYZNOA,M,AN,WM_WAIT) cout<<"Moviendo con Matriz"<<endl
#define pa_exe_axs(arm,ejes,angulo,WM_WAIT) cout<<"Moviendo con JOINT"<<endl

#define pa_opn_arm(arm) cout<<"iniciando robot"<<endl
#define pa_sta_arm(arm) cout<<"iniciando control robot"<<endl
#define pa_ext_arm(arm) cout<<"cerrando control robot"<<endl
#define pa_cls_arm(arm) cout<<"cerrando robot"<<endl
#define pa_stp_arm(arm,WM_WAIT) cout<<"Para el robot"<<endl
#define pa_sus_arm(arm,WM_WAIT) cout<<"Para el robot temporalmente"<<endl
#define pa_rsm_arm(arm,WM_WAIT) cout<<"El robot continua"<<endl
