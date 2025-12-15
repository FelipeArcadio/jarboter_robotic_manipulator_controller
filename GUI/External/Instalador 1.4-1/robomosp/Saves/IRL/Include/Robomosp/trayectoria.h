#include <list.h>

typedef struct 
{
	bool target;
	ANGLE * angulo;
	REAL64 matriz[3][4];
}punto;

punto * p;
List <punto *> puntos = new List <punto *>;

void getMat(POSE p, REAL64 m[3][4])
{
	POSITION o,a,n;
	a = VECTORA(p.ORI);
	o = VECTORO(p.ORI);
	n = VECTORN(p.ORI);
	
	m[0][0] =  n.X;
	m[0][1] =  n.Y;
	m[0][2] =  n.Z;

	m[1][0] =  o.X;
	m[1][1] =  o.Y;
	m[1][2] =  o.Z;

	m[2][0] =  a.X;
	m[2][1] =  a.Y;
	m[2][2] =  a.Z;

	m[3][0] =  p.POS.X;
	m[3][1] =  p.POS.Y;
	m[3][2] =  p.POS.Z;
}

