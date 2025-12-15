#include <math.h>

#define M_PI 3.1415926535897932384626433832795029L
#define R(X) X*M_PI/180.0
#define G(X) X*180.0/M_PI

double ABS ( double valor )
{
    if ( valor < 0.0 )
        return -valor;
    return valor;
}

double COS ( double angulo )
{
    if ( angulo == 0.0 || ABS ( angulo ) == 360.0 )
        return 1.0;
    if ( ABS ( angulo ) == 180.0 )
        return -1.0;
    if ( ABS ( angulo ) == 90.0 || ABS ( angulo ) == 270.0 )
        return 0.0;
    return cos ( R (angulo) );
}

double SIN ( double angulo )
{
    if ( angulo == 0.0 || ABS ( angulo ) == 180.0 || ABS ( angulo ) == 360.0 )
        return 0.0;
    if ( ABS ( angulo ) == 90.0 )
        return 1.0;
    if ( ABS ( angulo ) == 270.0 )
        return -1.0;
    return sin ( R (angulo) );
}

double TAN ( double angulo )
{
    if ( angulo == 0.0 || ABS ( angulo == 180.0 ) || ABS ( angulo ) == 360.0 )
        return 0.0;
    if ( ABS ( angulo ) == 90.0 )
        return 1.0 / 0.0;
    if ( ABS ( angulo ) == 270.0 )
        return 1.0 / 0.0;
    return tan ( R (angulo) );
}

double ACOS ( double valor )
{
    if ( valor == 1.0 )
        return 0.0;
    if ( valor == -1.0 )
        return 180.0;
    if ( valor == 0.0 )
        return 90.0;
    return G ( acos ( valor ) );
}

double ASIN ( double valor )
{
    if ( valor == 0.0 )
        return 0.0;
    if ( valor == 1.0 )
        return 90.0;
    if ( valor == -1.0 )
        return 270.0;
    return G ( asin ( valor ) );
}

double ATAN ( double valor )
{
    if ( valor == 0.0 )
        return 90.0;
    return G ( atan ( valor ) );
}

double ATAN2 ( double valor1, double valor2 )
{
    return G ( atan2 ( valor1, valor2 ) );
}
