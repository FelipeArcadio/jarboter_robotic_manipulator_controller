

    #include <math.h>
    #include "listas.h"
    #include "soporte.h"
    #include <iostream.h>
    #include <fstream.h>
    #include "sockets.h"
    #include <trigo.h>

    using namespace std;

    char * pointer1, * pointer2;
    fstream filedatalist;
    int i,j;
    FILE * archivotrayectoria;
    char cadena [1024];
    
    char Resultado [ RCVBUFSIZE ];
    char Comando [ RCVBUFSIZE ];
    int trayectorias = 0;
    int IDRobot;
    int IDWorld;

    #define cTARGET 0
    #define cJOINT 1


typedef 
struct {
float X
;
float Y
;
float Z
;
} POSITION
 ;
 
typedef 
struct {
float Q1
;
float Q2
;
float Q3
;
float Q4
;
} ORIENTATION
 ;
 
typedef 
struct {
POSITION POS
;
ORIENTATION ORI
;
} POSE
 ;
 
typedef 
struct {
float LOWER
;
float UPPER
;
} BOUNDS
 ;
 
typedef 
struct {
long A1
;
long A2
;
long A3
;
long A4
;
long A5
;
} MAIN_JOINT
 ;
 
typedef 
struct {
long AA1
;
} ADD_JOINT
 ;
 
typedef 
struct {
MAIN_JOINT M_JOINT
;
ADD_JOINT A_JOINT
;
} JOINT
 ;
 
typedef 
struct {
POSE PSE
;
long STATUS
;
long TURNS
 [ 2 ];
ADD_JOINT A_JOINT
;
} ROBTARGET
 ;
 
typedef 
struct {
long D_NO
;
long D_AXES_TYPE
;
long D_PRED_DEVICE
;
long D_NJ
;
} D_SPEC_TYPE
 ;
 
typedef 
struct {
float R_ACC
;
double R_ACC_PTP
 [ 5 ];
float R_C_PTP
;
float R_C_CP
;
float R_C_SPEED
;
long R_DEVICES
 [ 4 ];
JOINT R_JOINT_ACT
;
ROBTARGET R_ROBTARGET_ACT
;
ROBTARGET R_ROBTARGET_START
;
ROBTARGET R_ROBTARGET_END
;
ROBTARGET R_ROBTARGET_INTER
;
float R_SPEED_ACT
;
float R_SPEED
;
double R_SPEED_PTP
 [ 5 ];
float R_SPEED_ORI
;
POSE R_BASE
;
POSE R_TOOL
;
} R_SPEC_TYPE
 ;
 
typedef 
struct {
long FICHAS
;
POSITION COORDENADA
;
} TORRE
 ;
 
double ser_581
 [ 5 ];
double ser_598
 [ 5 ];
long ser_582
 [ 4 ];
MAIN_JOINT ser_583
;
MAIN_JOINT ser_603
;
ADD_JOINT ser_584
;
ADD_JOINT ser_610
;
ADD_JOINT ser_617
;
ADD_JOINT ser_625
;
ADD_JOINT ser_633
;
ADD_JOINT ser_641
;
JOINT ser_585
;
POSITION ser_586
;
POSITION ser_684
;
ORIENTATION ser_587
;
POSE ser_588
;
long ser_589
 [ 2 ];
long ser_615
 [ 2 ];
long ser_623
 [ 2 ];
long ser_631
 [ 2 ];
long ser_639
 [ 2 ];
ROBTARGET ser_590
;
ROBTARGET ser_593
;
ROBTARGET ser_595
;
ROBTARGET ser_597
;
POSE ser_591
;
POSE ser_592
;
POSE ser_594
;
POSE ser_596
;
POSE ser_599
;
POSE ser_600
;
R_SPEC_TYPE ser_601
;
JOINT ser_602
;
long ser_604
;
long ser_605
;
long ser_606
;
long ser_607
;
long ser_608
;
long ser_609
;
long ser_611
;
long ser_614
;
long ser_616
;
long ser_618
;
long ser_619
;
long ser_622
;
long ser_624
;
long ser_626
;
long ser_627
;
long ser_630
;
long ser_632
;
long ser_634
;
long ser_635
;
long ser_638
;
long ser_640
;
long ser_642
;
long ser_643
;
long ser_683
;
ROBTARGET ser_612
;
POSE ser_613
;
ROBTARGET ser_620
;
POSE ser_621
;
ROBTARGET ser_628
;
POSE ser_629
;
ROBTARGET ser_636
;
POSE ser_637
;
POSE ser_644
;
POSE ser_645
;
D_SPEC_TYPE ser_646
;
D_SPEC_TYPE ser_647
;
D_SPEC_TYPE ser_648
;
D_SPEC_TYPE ser_649
;
D_SPEC_TYPE ser_650
 [ 4 ];
TORRE ser_651
;
TORRE ser_652
;
TORRE ser_653
;
TORRE ser_682
;
TORRE ser_654
 [ 3 ];

    R_SPEC_TYPE * robots[16];
    int robot;
    typedef struct
    {
        int tipo;
        char valor [ 1024 ];
        ROBTARGET * r;
        JOINT * j;
    }punto;    
    punto * p;
    List <punto *> puntos;
    void Transformar ( ROBTARGET r, char texto [ 1024 ] );

float R_C_CP
;
float R_SPEED
;
float R_SPEED_ORI
;
float R_C_SPEED
;
float R_ACC
;
float R_MOVE_TIME
;
float R_SCALE_FORW
;
float R_SCALE_BACK
;
float R_DISTANCE_FORW
;
float R_DISTANCE_BACK
;
POSITION
 & CROSS 
(
POSITION X

,
POSITION Y

)
{
float ser_501
;
float ser_502
;
float ser_503
;
float ser_504
;
POSITION RESP
;
ser_501
 = RESP.X;
ser_502
 = X.Y;
ser_503
 = Y.Z;
ser_502
 = ser_502 * ser_503;
ser_503
 = X.Z;
ser_504
 = Y.Y;
ser_503
 = ser_503 * ser_504;
ser_504
  = ser_502 - ser_503 ;
ser_501
  = ser_504 ;
RESP
.X = ser_501;
ser_502
 = RESP.Y;
ser_503
 = X.Z;
ser_504
 = Y.X;
ser_501
 = ser_503 * ser_504;
ser_503
 = X.X;
ser_504
 = Y.Z;
ser_503
 = ser_503 * ser_504;
ser_504
  = ser_501 - ser_503 ;
ser_502
  = ser_504 ;
RESP
.Y = ser_502;
ser_501
 = RESP.Z;
ser_503
 = X.X;
ser_504
 = Y.Y;
ser_502
 = ser_503 * ser_504;
ser_503
 = X.Y;
ser_504
 = Y.X;
ser_503
 = ser_503 * ser_504;
ser_504
  = ser_502 - ser_503 ;
ser_501
  = ser_504 ;
RESP
.Z = ser_501;
return RESP;
}
double DOT 
(
POSITION X

,
POSITION Y

)
{
float ser_505
;
float ser_506
;
float ser_507
;
float RESP
;
ser_505
 = X.X;
ser_506
 = Y.X;
ser_505
 = ser_505 * ser_506;
ser_506
 = X.Y;
ser_507
 = Y.Y;
ser_506
 = ser_506 * ser_507;
ser_507
 = ser_505 + ser_506 ;
ser_505
 = X.Z;
ser_506
 = Y.Z;
ser_505
 = ser_505 * ser_506;
ser_506
 = ser_507 + ser_505 ;
RESP
  = ser_506 ;
return RESP;
}
char CHR 
(
long ASCIIVALUE

)
{
char RESP
;
RESP
  = ASCIIVALUE ;
return RESP;
}
double FLOAT 
(
long X

)
{
float RESP
;
RESP
  = X ;
return RESP;
}
ORIENTATION
 & ORIVANG 
(
POSITION V

,
double A

)
{
float ser_508
;
float ser_509
;
float ser_510
;
ORIENTATION RESP
;
ser_508
 = RESP.Q1;
ser_509
 = A / 2.000000 ;
ser_509
 = COS ( ser_509 ); 
ser_508
  = ser_509 ;
RESP
.Q1 = ser_508;
ser_509
 = RESP.Q2;
ser_508
 = A / 2.000000 ;
ser_508
 = SIN ( ser_508 ); 
ser_510
 = V.X;
ser_508
 = ser_508 * ser_510;
ser_509
  = ser_508 ;
RESP
.Q2 = ser_509;
ser_510
 = RESP.Q3;
ser_508
 = A / 2.000000 ;
ser_509
 = SIN ( ser_508 ); 
ser_508
 = V.Y;
ser_509
 = ser_509 * ser_508;
ser_510
  = ser_509 ;
RESP
.Q3 = ser_510;
ser_508
 = RESP.Q4;
ser_509
 = A / 2.000000 ;
ser_510
 = SIN ( ser_509 ); 
ser_509
 = V.Z;
ser_510
 = ser_510 * ser_509;
ser_508
  = ser_510 ;
RESP
.Q4 = ser_508;
return RESP;
}
ORIENTATION
 & ORIMULT 
(
ORIENTATION A

,
ORIENTATION B

)
{
float ser_511
;
float ser_512
;
float ser_513
;
float ser_514
;
ORIENTATION RESP
;
ser_511
 = RESP.Q1;
ser_512
 = A.Q2;
ser_512
 = ser_512 * -1;
ser_513
 = B.Q2;
ser_512
 = ser_512 * ser_513;
ser_513
 = A.Q3;
ser_514
 = B.Q3;
ser_513
 = ser_513 * ser_514;
ser_514
  = ser_512 - ser_513 ;
ser_512
 = A.Q4;
ser_513
 = B.Q4;
ser_512
 = ser_512 * ser_513;
ser_513
  = ser_514 - ser_512 ;
ser_514
 = A.Q1;
ser_512
 = B.Q1;
ser_514
 = ser_514 * ser_512;
ser_512
 = ser_513 + ser_514 ;
ser_511
  = ser_512 ;
RESP
.Q1 = ser_511;
ser_513
 = RESP.Q2;
ser_514
 = A.Q2;
ser_512
 = B.Q1;
ser_511
 = ser_514 * ser_512;
ser_514
 = A.Q3;
ser_512
 = B.Q4;
ser_514
 = ser_514 * ser_512;
ser_512
 = ser_511 + ser_514 ;
ser_511
 = A.Q4;
ser_514
 = B.Q3;
ser_511
 = ser_511 * ser_514;
ser_514
  = ser_512 - ser_511 ;
ser_512
 = A.Q1;
ser_511
 = B.Q2;
ser_512
 = ser_512 * ser_511;
ser_511
 = ser_514 + ser_512 ;
ser_513
  = ser_511 ;
RESP
.Q2 = ser_513;
ser_514
 = RESP.Q3;
ser_512
 = A.Q2;
ser_511
 = ser_512 * -1;
ser_513
 = B.Q4;
ser_512
 = ser_511 * ser_513;
ser_511
 = A.Q3;
ser_513
 = B.Q1;
ser_511
 = ser_511 * ser_513;
ser_513
 = ser_512 + ser_511 ;
ser_512
 = A.Q4;
ser_511
 = B.Q2;
ser_512
 = ser_512 * ser_511;
ser_511
 = ser_513 + ser_512 ;
ser_513
 = A.Q1;
ser_512
 = B.Q3;
ser_513
 = ser_513 * ser_512;
ser_512
 = ser_511 + ser_513 ;
ser_514
  = ser_512 ;
RESP
.Q3 = ser_514;
ser_511
 = RESP.Q4;
ser_513
 = A.Q2;
ser_512
 = B.Q3;
ser_514
 = ser_513 * ser_512;
ser_513
 = A.Q3;
ser_512
 = B.Q2;
ser_513
 = ser_513 * ser_512;
ser_512
  = ser_514 - ser_513 ;
ser_514
 = A.Q4;
ser_513
 = B.Q1;
ser_514
 = ser_514 * ser_513;
ser_513
 = ser_512 + ser_514 ;
ser_512
 = A.Q1;
ser_514
 = B.Q4;
ser_512
 = ser_512 * ser_514;
ser_514
 = ser_513 + ser_512 ;
ser_511
  = ser_514 ;
RESP
.Q4 = ser_511;
return RESP;
}
ORIENTATION
 & ORIMAT 
(
POSITION N

,
POSITION O

,
POSITION A

)
{
float ser_515
;
float ser_516
;
float ser_517
;
ORIENTATION RESP
;
float TRACE
;
float S
;
ser_515
 = N.X;
ser_516
 = O.Y;
ser_515
 = ser_515 + ser_516 ;
ser_516
 = A.Z;
ser_515
 = ser_515 + ser_516 ;
ser_516
 = ser_515 + 1.000000 ;
TRACE
  = ser_516 ;
if( TRACE > 0 ) goto par_514
;
goto par_515
 ;
par_514 : ;
ser_515
 = sqrt ( TRACE );
ser_516
 = 0.500000 / ser_515 ;
S
  = ser_516 ;
ser_515
 = RESP.Q1;
ser_516
 = 0.250000 / S ;
ser_515
  = ser_516 ;
RESP
.Q1 = ser_515;
ser_516
 = RESP.Q2;
ser_515
 = O.Z;
ser_517
 = A.Y;
ser_515
  = ser_515 - ser_517 ;
ser_517
 = ser_515 * S;
ser_516
  = ser_517 ;
RESP
.Q2 = ser_516;
ser_515
 = RESP.Q2;
ser_517
 = A.X;
ser_516
 = N.Z;
ser_517
  = ser_517 - ser_516 ;
ser_516
 = ser_517 * S;
ser_515
  = ser_516 ;
RESP
.Q2 = ser_515;
ser_517
 = RESP.Q4;
ser_516
 = N.Y;
ser_515
 = O.X;
ser_516
  = ser_516 - ser_515 ;
ser_515
 = ser_516 * S;
ser_517
  = ser_515 ;
RESP
.Q4 = ser_517;
par_516 : ;
goto par_517
 ;
par_515 : ;
ser_516
 = N.X;
ser_515
 = O.Y;
if( ser_516 > ser_515 ) goto par_507
;
goto par_512
 ;
par_507 : ;
ser_517
 = N.X;
ser_516
 = A.Z;
if( ser_517 > ser_516 ) goto par_511
;
goto par_512
 ;
par_511 : ;
ser_515
 = N.X;
ser_517
 = 1.000000 + ser_515 ;
ser_516
 = O.Y;
ser_515
  = ser_517 - ser_516 ;
ser_517
 = A.Z;
ser_516
  = ser_515 - ser_517 ;
ser_515
 = sqrt ( ser_516 );
ser_517
 = 2.000000 * ser_515;
S
  = ser_517 ;
ser_516
 = RESP.Q1;
ser_515
 = A.Y;
ser_517
 = O.Z;
ser_515
  = ser_515 - ser_517 ;
ser_517
 = ser_515 / S ;
ser_516
  = ser_517 ;
RESP
.Q1 = ser_516;
ser_515
 = RESP.Q2;
ser_517
 = 0.250000 * S;
ser_515
  = ser_517 ;
RESP
.Q2 = ser_515;
ser_516
 = RESP.Q3;
ser_517
 = O.X;
ser_515
 = N.Y;
ser_517
 = ser_517 + ser_515 ;
ser_515
 = ser_517 / S ;
ser_516
  = ser_515 ;
RESP
.Q3 = ser_516;
ser_517
 = RESP.Q4;
ser_515
 = A.X;
ser_516
 = O.Z;
ser_515
  = ser_515 - ser_516 ;
ser_516
 = ser_515 / S ;
ser_517
  = ser_516 ;
RESP
.Q4 = ser_517;
par_513 : ;
goto par_517
 ;
par_512 : ;
ser_515
 = O.Y;
ser_516
 = A.Z;
if( ser_515 > ser_516 ) goto par_508
;
goto par_509
 ;
par_508 : ;
ser_517
 = O.Y;
ser_515
 = 1.000000 + ser_517 ;
ser_516
 = N.X;
ser_517
  = ser_515 - ser_516 ;
ser_515
 = A.Z;
ser_516
  = ser_517 - ser_515 ;
ser_517
 = sqrt ( ser_516 );
ser_515
 = 2.000000 * ser_517;
S
  = ser_515 ;
ser_516
 = RESP.Q1;
ser_517
 = A.X;
ser_515
 = N.Z;
ser_517
  = ser_517 - ser_515 ;
ser_515
 = ser_517 / S ;
ser_516
  = ser_515 ;
RESP
.Q1 = ser_516;
ser_517
 = RESP.Q2;
ser_515
 = O.X;
ser_516
 = N.Y;
ser_515
 = ser_515 + ser_516 ;
ser_516
 = ser_515 / S ;
ser_517
  = ser_516 ;
RESP
.Q2 = ser_517;
ser_515
 = RESP.Q3;
ser_516
 = 0.250000 * S;
ser_515
  = ser_516 ;
RESP
.Q3 = ser_515;
ser_517
 = RESP.Q4;
ser_516
 = A.Y;
ser_515
 = O.Z;
ser_516
 = ser_516 + ser_515 ;
ser_515
 = ser_516 / S ;
ser_517
  = ser_515 ;
RESP
.Q4 = ser_517;
par_510 : ;
goto par_517
 ;
par_509 : ;
ser_516
 = A.Z;
ser_515
 = 1.000000 + ser_516 ;
ser_517
 = N.X;
ser_516
  = ser_515 - ser_517 ;
ser_515
 = O.Y;
ser_517
  = ser_516 - ser_515 ;
ser_516
 = sqrt ( ser_517 );
ser_515
 = 2.000000 * ser_516;
S
  = ser_515 ;
ser_517
 = RESP.Q1;
ser_516
 = O.X;
ser_515
 = N.Y;
ser_516
  = ser_516 - ser_515 ;
ser_515
 = ser_516 / S ;
ser_517
  = ser_515 ;
RESP
.Q1 = ser_517;
ser_516
 = RESP.Q2;
ser_515
 = A.X;
ser_517
 = N.Z;
ser_515
 = ser_515 + ser_517 ;
ser_517
 = ser_515 / S ;
ser_516
  = ser_517 ;
RESP
.Q2 = ser_516;
ser_515
 = RESP.Q3;
ser_517
 = A.Y;
ser_516
 = O.Z;
ser_517
 = ser_517 + ser_516 ;
ser_516
 = ser_517 / S ;
ser_515
  = ser_516 ;
RESP
.Q3 = ser_515;
ser_517
 = RESP.Q4;
ser_516
 = 0.250000 * S;
ser_517
  = ser_516 ;
RESP
.Q4 = ser_517;
par_517 : ;
return RESP;
}
ORIENTATION
 & ORIQUAT 
(
double A

,
double B

,
double C

,
double D

)
{
float ser_518
;
ORIENTATION RESP
;
ser_518
 = RESP.Q1;
ser_518
  = A ;
RESP
.Q1 = ser_518;
ser_518
 = RESP.Q2;
ser_518
  = B ;
RESP
.Q2 = ser_518;
ser_518
 = RESP.Q3;
ser_518
  = C ;
RESP
.Q3 = ser_518;
ser_518
 = RESP.Q4;
ser_518
  = D ;
RESP
.Q4 = ser_518;
return RESP;
}
ORIENTATION
 & ORIRS 
(
double A

,
double B

,
double C

)
{
POSITION ser_519
;
float ser_520
;
ORIENTATION ser_521
;
ORIENTATION ser_522
;
ORIENTATION ser_523
;
POSITION AXISX
;
POSITION AXISY
;
POSITION AXISZ
;
ORIENTATION RESP
;
ser_520
  = 1 ;
ser_519
.X = ser_520;
ser_520
  = 0 ;
ser_519
.Y = ser_520;
ser_520
  = 0 ;
ser_519
.Z = ser_520;
ser_520
  = 1 ;
ser_519
.X = ser_520;
ser_520
  = 0 ;
ser_519
.Y = ser_520;
ser_520
  = 0 ;
ser_519
.Z = ser_520;

    pointer1 = (char *)&ser_519;
    pointer2 = (char *)&AXISX
;
    for ( i = 0; i < sizeof ( AXISX
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_520
  = 0 ;
ser_519
.X = ser_520;
ser_520
  = 1 ;
ser_519
.Y = ser_520;
ser_520
  = 0 ;
ser_519
.Z = ser_520;
ser_520
  = 0 ;
ser_519
.X = ser_520;
ser_520
  = 1 ;
ser_519
.Y = ser_520;
ser_520
  = 0 ;
ser_519
.Z = ser_520;

    pointer1 = (char *)&ser_519;
    pointer2 = (char *)&AXISY
;
    for ( i = 0; i < sizeof ( AXISY
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_520
  = 0 ;
ser_519
.X = ser_520;
ser_520
  = 0 ;
ser_519
.Y = ser_520;
ser_520
  = 1 ;
ser_519
.Z = ser_520;
ser_520
  = 0 ;
ser_519
.X = ser_520;
ser_520
  = 0 ;
ser_519
.Y = ser_520;
ser_520
  = 1 ;
ser_519
.Z = ser_520;

    pointer1 = (char *)&ser_519;
    pointer2 = (char *)&AXISZ
;
    for ( i = 0; i < sizeof ( AXISZ
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_521
 = ORIVANG ( 
 AXISZ 
,
 C 
 ) ; 
ser_522
 = ORIVANG ( 
 AXISY 
,
 B 
 ) ; 
ser_523
 = ORIMULT ( 
 ser_521 
,
 ser_522 
 ) ; 
ser_521
 = ORIVANG ( 
 AXISX 
,
 A 
 ) ; 
ser_522
 = ORIMULT ( 
 ser_523 
,
 ser_521 
 ) ; 

    pointer1 = (char *)&ser_522;
    pointer2 = (char *)&RESP
;
    for ( i = 0; i < sizeof ( RESP
 ) ; i ++ )
        pointer2[i] = pointer1[i];

return RESP;
}
ORIENTATION
 & ORIXYZ 
(
double A

,
double B

,
double C

)
{
POSITION ser_524
;
float ser_525
;
ORIENTATION ser_526
;
ORIENTATION ser_527
;
ORIENTATION ser_528
;
POSITION AXISX
;
POSITION AXISY
;
POSITION AXISZ
;
ORIENTATION RESP
;
ser_525
  = 1 ;
ser_524
.X = ser_525;
ser_525
  = 0 ;
ser_524
.Y = ser_525;
ser_525
  = 0 ;
ser_524
.Z = ser_525;
ser_525
  = 1 ;
ser_524
.X = ser_525;
ser_525
  = 0 ;
ser_524
.Y = ser_525;
ser_525
  = 0 ;
ser_524
.Z = ser_525;

    pointer1 = (char *)&ser_524;
    pointer2 = (char *)&AXISX
;
    for ( i = 0; i < sizeof ( AXISX
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_525
  = 0 ;
ser_524
.X = ser_525;
ser_525
  = 1 ;
ser_524
.Y = ser_525;
ser_525
  = 0 ;
ser_524
.Z = ser_525;
ser_525
  = 0 ;
ser_524
.X = ser_525;
ser_525
  = 1 ;
ser_524
.Y = ser_525;
ser_525
  = 0 ;
ser_524
.Z = ser_525;

    pointer1 = (char *)&ser_524;
    pointer2 = (char *)&AXISY
;
    for ( i = 0; i < sizeof ( AXISY
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_525
  = 0 ;
ser_524
.X = ser_525;
ser_525
  = 0 ;
ser_524
.Y = ser_525;
ser_525
  = 1 ;
ser_524
.Z = ser_525;
ser_525
  = 0 ;
ser_524
.X = ser_525;
ser_525
  = 0 ;
ser_524
.Y = ser_525;
ser_525
  = 1 ;
ser_524
.Z = ser_525;

    pointer1 = (char *)&ser_524;
    pointer2 = (char *)&AXISZ
;
    for ( i = 0; i < sizeof ( AXISZ
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_526
 = ORIVANG ( 
 AXISX 
,
 A 
 ) ; 
ser_527
 = ORIVANG ( 
 AXISY 
,
 B 
 ) ; 
ser_528
 = ORIMULT ( 
 ser_526 
,
 ser_527 
 ) ; 
ser_526
 = ORIVANG ( 
 AXISZ 
,
 C 
 ) ; 
ser_527
 = ORIMULT ( 
 ser_528 
,
 ser_526 
 ) ; 

    pointer1 = (char *)&ser_527;
    pointer2 = (char *)&RESP
;
    for ( i = 0; i < sizeof ( RESP
 ) ; i ++ )
        pointer2[i] = pointer1[i];

return RESP;
}
ORIENTATION
 & ORIYXZ 
(
double A

,
double B

,
double C

)
{
POSITION ser_529
;
float ser_530
;
ORIENTATION ser_531
;
ORIENTATION ser_532
;
ORIENTATION ser_533
;
POSITION AXISX
;
POSITION AXISY
;
POSITION AXISZ
;
ORIENTATION RESP
;
ser_530
  = 1 ;
ser_529
.X = ser_530;
ser_530
  = 0 ;
ser_529
.Y = ser_530;
ser_530
  = 0 ;
ser_529
.Z = ser_530;
ser_530
  = 1 ;
ser_529
.X = ser_530;
ser_530
  = 0 ;
ser_529
.Y = ser_530;
ser_530
  = 0 ;
ser_529
.Z = ser_530;

    pointer1 = (char *)&ser_529;
    pointer2 = (char *)&AXISX
;
    for ( i = 0; i < sizeof ( AXISX
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_530
  = 0 ;
ser_529
.X = ser_530;
ser_530
  = 1 ;
ser_529
.Y = ser_530;
ser_530
  = 0 ;
ser_529
.Z = ser_530;
ser_530
  = 0 ;
ser_529
.X = ser_530;
ser_530
  = 1 ;
ser_529
.Y = ser_530;
ser_530
  = 0 ;
ser_529
.Z = ser_530;

    pointer1 = (char *)&ser_529;
    pointer2 = (char *)&AXISY
;
    for ( i = 0; i < sizeof ( AXISY
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_530
  = 0 ;
ser_529
.X = ser_530;
ser_530
  = 0 ;
ser_529
.Y = ser_530;
ser_530
  = 1 ;
ser_529
.Z = ser_530;
ser_530
  = 0 ;
ser_529
.X = ser_530;
ser_530
  = 0 ;
ser_529
.Y = ser_530;
ser_530
  = 1 ;
ser_529
.Z = ser_530;

    pointer1 = (char *)&ser_529;
    pointer2 = (char *)&AXISZ
;
    for ( i = 0; i < sizeof ( AXISZ
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_531
 = ORIVANG ( 
 AXISY 
,
 A 
 ) ; 
ser_532
 = ORIVANG ( 
 AXISX 
,
 B 
 ) ; 
ser_533
 = ORIMULT ( 
 ser_531 
,
 ser_532 
 ) ; 
ser_531
 = ORIVANG ( 
 AXISZ 
,
 C 
 ) ; 
ser_532
 = ORIMULT ( 
 ser_533 
,
 ser_531 
 ) ; 

    pointer1 = (char *)&ser_532;
    pointer2 = (char *)&RESP
;
    for ( i = 0; i < sizeof ( RESP
 ) ; i ++ )
        pointer2[i] = pointer1[i];

return RESP;
}
ORIENTATION
 & ORIZYX 
(
double A

,
double B

,
double C

)
{
POSITION ser_534
;
float ser_535
;
ORIENTATION ser_536
;
ORIENTATION ser_537
;
ORIENTATION ser_538
;
POSITION AXISX
;
POSITION AXISY
;
POSITION AXISZ
;
ORIENTATION RESP
;
ser_535
  = 1 ;
ser_534
.X = ser_535;
ser_535
  = 0 ;
ser_534
.Y = ser_535;
ser_535
  = 0 ;
ser_534
.Z = ser_535;
ser_535
  = 1 ;
ser_534
.X = ser_535;
ser_535
  = 0 ;
ser_534
.Y = ser_535;
ser_535
  = 0 ;
ser_534
.Z = ser_535;

    pointer1 = (char *)&ser_534;
    pointer2 = (char *)&AXISX
;
    for ( i = 0; i < sizeof ( AXISX
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_535
  = 0 ;
ser_534
.X = ser_535;
ser_535
  = 1 ;
ser_534
.Y = ser_535;
ser_535
  = 0 ;
ser_534
.Z = ser_535;
ser_535
  = 0 ;
ser_534
.X = ser_535;
ser_535
  = 1 ;
ser_534
.Y = ser_535;
ser_535
  = 0 ;
ser_534
.Z = ser_535;

    pointer1 = (char *)&ser_534;
    pointer2 = (char *)&AXISY
;
    for ( i = 0; i < sizeof ( AXISY
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_535
  = 0 ;
ser_534
.X = ser_535;
ser_535
  = 0 ;
ser_534
.Y = ser_535;
ser_535
  = 1 ;
ser_534
.Z = ser_535;
ser_535
  = 0 ;
ser_534
.X = ser_535;
ser_535
  = 0 ;
ser_534
.Y = ser_535;
ser_535
  = 1 ;
ser_534
.Z = ser_535;

    pointer1 = (char *)&ser_534;
    pointer2 = (char *)&AXISZ
;
    for ( i = 0; i < sizeof ( AXISZ
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_536
 = ORIVANG ( 
 AXISZ 
,
 A 
 ) ; 
ser_537
 = ORIVANG ( 
 AXISY 
,
 B 
 ) ; 
ser_538
 = ORIMULT ( 
 ser_536 
,
 ser_537 
 ) ; 
ser_536
 = ORIVANG ( 
 AXISX 
,
 C 
 ) ; 
ser_537
 = ORIMULT ( 
 ser_538 
,
 ser_536 
 ) ; 

    pointer1 = (char *)&ser_537;
    pointer2 = (char *)&RESP
;
    for ( i = 0; i < sizeof ( RESP
 ) ; i ++ )
        pointer2[i] = pointer1[i];

return RESP;
}
ORIENTATION
 & ORIZYZ2 
(
double A

,
double B

,
double C

)
{
POSITION ser_539
;
float ser_540
;
ORIENTATION ser_541
;
ORIENTATION ser_542
;
ORIENTATION ser_543
;
POSITION AXISY
;
POSITION AXISZ
;
ORIENTATION RESP
;
ser_540
  = 0 ;
ser_539
.X = ser_540;
ser_540
  = 1 ;
ser_539
.Y = ser_540;
ser_540
  = 0 ;
ser_539
.Z = ser_540;
ser_540
  = 0 ;
ser_539
.X = ser_540;
ser_540
  = 1 ;
ser_539
.Y = ser_540;
ser_540
  = 0 ;
ser_539
.Z = ser_540;

    pointer1 = (char *)&ser_539;
    pointer2 = (char *)&AXISY
;
    for ( i = 0; i < sizeof ( AXISY
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_540
  = 0 ;
ser_539
.X = ser_540;
ser_540
  = 0 ;
ser_539
.Y = ser_540;
ser_540
  = 1 ;
ser_539
.Z = ser_540;
ser_540
  = 0 ;
ser_539
.X = ser_540;
ser_540
  = 0 ;
ser_539
.Y = ser_540;
ser_540
  = 1 ;
ser_539
.Z = ser_540;

    pointer1 = (char *)&ser_539;
    pointer2 = (char *)&AXISZ
;
    for ( i = 0; i < sizeof ( AXISZ
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_541
 = ORIVANG ( 
 AXISZ 
,
 A 
 ) ; 
ser_542
 = ORIVANG ( 
 AXISY 
,
 B 
 ) ; 
ser_543
 = ORIMULT ( 
 ser_541 
,
 ser_542 
 ) ; 
ser_541
 = ORIVANG ( 
 AXISZ 
,
 C 
 ) ; 
ser_542
 = ORIMULT ( 
 ser_543 
,
 ser_541 
 ) ; 

    pointer1 = (char *)&ser_542;
    pointer2 = (char *)&RESP
;
    for ( i = 0; i < sizeof ( RESP
 ) ; i ++ )
        pointer2[i] = pointer1[i];

return RESP;
}
double QUATA 
(
ORIENTATION ORI

)
{
float ser_544
;
ser_544
 = ORI.Q1;
return ser_544;
}
double QUATB 
(
ORIENTATION ORI

)
{
float ser_545
;
ser_545
 = ORI.Q2;
return ser_545;
}
double QUATC 
(
ORIENTATION ORI

)
{
float ser_546
;
ser_546
 = ORI.Q3;
return ser_546;
}
double QUATD 
(
ORIENTATION ORI

)
{
float ser_547
;
ser_547
 = ORI.Q4;
return ser_547;
}
double ROTANGLE 
(
ORIENTATION ORI

)
{
float ser_548
;
float RESP
;
ser_548
 = ORI.Q1;
ser_548
 = ACOS ( ser_548 );
ser_548
 = 2 * ser_548;
RESP
  = ser_548 ;
return RESP;
}
POSITION
 & ROTAXIS 
(
ORIENTATION ORI

)
{
float ser_549
;
float ser_550
;
float ser_551
;
float MITADTETA
;
POSITION RESP
;
ser_549
 = ORI.Q1;
ser_549
 = ACOS ( ser_549 );
MITADTETA
  = ser_549 ;
ser_549
 = RESP.X;
ser_550
 = ORI.Q2;
ser_551
 = SIN ( MITADTETA ); 
ser_550
 = ser_550 / ser_551 ;
ser_549
  = ser_550 ;
RESP
.X = ser_549;
ser_551
 = RESP.Y;
ser_550
 = ORI.Q3;
ser_549
 = SIN ( MITADTETA ); 
ser_550
 = ser_550 / ser_549 ;
ser_551
  = ser_550 ;
RESP
.Y = ser_551;
ser_549
 = RESP.Z;
ser_550
 = ORI.Q4;
ser_551
 = SIN ( MITADTETA ); 
ser_550
 = ser_550 / ser_551 ;
ser_549
  = ser_550 ;
RESP
.Z = ser_549;
return RESP;
}
POSITION
 & VECTORA 
(
ORIENTATION ORI

)
{
float ser_552
;
float ser_553
;
float ser_554
;
float ser_555
;
POSITION RESP
;
ser_552
 = RESP.X;
ser_553
 = ORI.Q2;
ser_554
 = ORI.Q4;
ser_553
 = ser_553 * ser_554;
ser_554
 = ORI.Q1;
ser_555
 = ORI.Q3;
ser_554
 = ser_554 * ser_555;
ser_555
 = ser_553 + ser_554 ;
ser_553
 = 2 * ser_555;
ser_552
  = ser_553 ;
RESP
.X = ser_552;
ser_554
 = RESP.Y;
ser_555
 = ORI.Q3;
ser_553
 = ORI.Q4;
ser_552
 = ser_555 * ser_553;
ser_555
 = ORI.Q1;
ser_553
 = ORI.Q2;
ser_555
 = ser_555 * ser_553;
ser_553
  = ser_552 - ser_555 ;
ser_552
 = 2 * ser_553;
ser_554
  = ser_552 ;
RESP
.Y = ser_554;
ser_555
 = RESP.Z;
ser_553
 = ORI.Q2;
ser_552
 = ORI.Q2;
ser_554
 = ser_553 * ser_552;
ser_553
 = ORI.Q3;
ser_552
 = ORI.Q3;
ser_553
 = ser_553 * ser_552;
ser_552
 = ser_554 + ser_553 ;
ser_554
 = 2 * ser_552;
ser_553
  = 1 - ser_554 ;
ser_555
  = ser_553 ;
RESP
.Z = ser_555;
return RESP;
}
POSITION
 & VECTORN 
(
ORIENTATION ORI

)
{
float ser_556
;
float ser_557
;
float ser_558
;
float ser_559
;
POSITION RESP
;
ser_556
 = RESP.X;
ser_557
 = ORI.Q3;
ser_558
 = ORI.Q3;
ser_557
 = ser_557 * ser_558;
ser_558
 = ORI.Q4;
ser_559
 = ORI.Q4;
ser_558
 = ser_558 * ser_559;
ser_559
 = ser_557 + ser_558 ;
ser_557
 = 2 * ser_559;
ser_558
  = 1 - ser_557 ;
ser_556
  = ser_558 ;
RESP
.X = ser_556;
ser_559
 = RESP.Y;
ser_557
 = ORI.Q2;
ser_558
 = ORI.Q3;
ser_556
 = ser_557 * ser_558;
ser_557
 = ORI.Q1;
ser_558
 = ORI.Q4;
ser_557
 = ser_557 * ser_558;
ser_558
 = ser_556 + ser_557 ;
ser_556
 = 2 * ser_558;
ser_559
  = ser_556 ;
RESP
.Y = ser_559;
ser_557
 = RESP.Z;
ser_558
 = ORI.Q2;
ser_556
 = ORI.Q4;
ser_559
 = ser_558 * ser_556;
ser_558
 = ORI.Q1;
ser_556
 = ORI.Q3;
ser_558
 = ser_558 * ser_556;
ser_556
  = ser_559 - ser_558 ;
ser_559
 = 2 * ser_556;
ser_557
  = ser_559 ;
RESP
.Z = ser_557;
return RESP;
}
POSITION
 & VECTORO 
(
ORIENTATION ORI

)
{
float ser_560
;
float ser_561
;
float ser_562
;
float ser_563
;
POSITION RESP
;
ser_560
 = RESP.X;
ser_561
 = ORI.Q2;
ser_562
 = ORI.Q3;
ser_561
 = ser_561 * ser_562;
ser_562
 = ORI.Q1;
ser_563
 = ORI.Q4;
ser_562
 = ser_562 * ser_563;
ser_563
  = ser_561 - ser_562 ;
ser_561
 = 2 * ser_563;
ser_560
  = ser_561 ;
RESP
.X = ser_560;
ser_562
 = RESP.Y;
ser_563
 = ORI.Q2;
ser_561
 = ORI.Q2;
ser_560
 = ser_563 * ser_561;
ser_563
 = ORI.Q4;
ser_561
 = ORI.Q4;
ser_563
 = ser_563 * ser_561;
ser_561
 = ser_560 + ser_563 ;
ser_560
 = 2 * ser_561;
ser_563
  = 1 - ser_560 ;
ser_562
  = ser_563 ;
RESP
.Y = ser_562;
ser_561
 = RESP.Z;
ser_560
 = ORI.Q3;
ser_563
 = ORI.Q4;
ser_562
 = ser_560 * ser_563;
ser_560
 = ORI.Q1;
ser_563
 = ORI.Q2;
ser_560
 = ser_560 * ser_563;
ser_563
 = ser_562 + ser_560 ;
ser_562
 = 2 * ser_563;
ser_561
  = ser_562 ;
RESP
.Z = ser_561;
return RESP;
}
long ORD 
(
char CHARACTER

)
{
long RESP
;
RESP
  = CHARACTER ;
return RESP;
}
double ANGLEX 
(
char MODE
 [256]
,
ORIENTATION ORI

)
{
POSITION ser_564
;
long ser_565
;
float ser_566
;
POSITION N
;
POSITION O
;
POSITION A
;
float C
;
float TRX
;
float TRY
;
float RESP
;
ser_564
 = VECTORN ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_564;
    pointer2 = (char *)&N
;
    for ( i = 0; i < sizeof ( N
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_564
 = VECTORA ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_564;
    pointer2 = (char *)&A
;
    for ( i = 0; i < sizeof ( A
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_564
 = VECTORO ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_564;
    pointer2 = (char *)&O
;
    for ( i = 0; i < sizeof ( O
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_565
 = strcmp ( MODE, "RS" );
if( ser_565 == 0 ) goto par_542
;
goto par_535
 ;
par_535 : ;
ser_565
 = strcmp ( MODE, "ZYX" );
if( ser_565 == 0 ) goto par_542
;
goto par_549
 ;
par_542 : ;
ser_566
 = N.Z;
ser_566
 = ser_566 * -1;
ser_566
 = ASIN ( ser_566 );
ser_566
 = COS ( ser_566 ); 
C
  = ser_566 ;
ser_566
 = ABS ( C );
if( ser_566 > 0.005000 ) goto par_538
;
goto par_539
 ;
par_538 : ;
ser_566
 = A.Z;
ser_566
 = ser_566 / C ;
TRX
  = ser_566 ;
ser_566
 = O.Z;
ser_566
 = ser_566 / C ;
TRY
  = ser_566 ;
ser_566
 = ATAN2 ( TRY, TRX );
RESP
  = ser_566 ;
if( RESP < 0 ) goto par_536
;
goto par_540
 ;
par_536 : ;
ser_566
 = RESP + 360 ;
RESP
  = ser_566 ;
par_540 : ;
goto par_541
 ;
par_539 : ;
ser_566
 = O.X;
TRX
  = ser_566 ;
ser_566
 = A.X;
TRY
  = ser_566 ;
ser_566
 = ATAN2 ( TRY, TRX );
RESP
  = ser_566 ;
if( RESP < 0 ) goto par_537
;
goto par_541
 ;
par_537 : ;
ser_566
 = RESP + 360 ;
RESP
  = ser_566 ;
par_541 : ;
return RESP;
par_549 : ;
ser_565
 = strcmp ( MODE, "XYZ" );
if( ser_565 == 0 ) goto par_548
;
goto par_553
 ;
par_548 : ;
ser_566
 = A.X;
ser_566
 = ASIN ( ser_566 );
ser_566
 = COS ( ser_566 ); 
C
  = ser_566 ;
ser_566
 = ABS ( C );
if( ser_566 > 0.005000 ) goto par_544
;
goto par_545
 ;
par_544 : ;
ser_566
 = A.Z;
ser_566
 = ser_566 / C ;
TRX
  = ser_566 ;
ser_566
 = A.Y;
ser_566
 = ser_566 * -1;
ser_566
 = ser_566 / C ;
TRY
  = ser_566 ;
ser_566
 = ATAN2 ( TRY, TRX );
RESP
  = ser_566 ;
if( RESP < 0 ) goto par_543
;
goto par_546
 ;
par_543 : ;
ser_566
 = RESP + 360 ;
RESP
  = ser_566 ;
par_546 : ;
goto par_547
 ;
par_545 : ;
RESP
  = 0.000000 ;
par_547 : ;
return RESP;
par_553 : ;
ser_565
 = strcmp ( MODE, "YXZ" );
if( ser_565 == 0 ) goto par_552
;
goto par_554
 ;
par_552 : ;
ser_566
 = A.Y;
ser_566
 = ser_566 * -1;
ser_566
 = ASIN ( ser_566 );
RESP
  = ser_566 ;
if( RESP < 0 ) goto par_550
;
goto par_551
 ;
par_550 : ;
ser_566
 = RESP + 360 ;
RESP
  = ser_566 ;
par_551 : ;
return RESP;
par_554 : ;
return 0.000000;
}
double ANGLEY 
(
char MODE
 [256]
,
ORIENTATION ORI

)
{
POSITION ser_567
;
long ser_568
;
float ser_569
;
float ser_570
;
float ser_571
;
POSITION N
;
POSITION A
;
POSITION O
;
float C
;
float TRX
;
float TRY
;
float RESP
;
ser_567
 = VECTORN ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_567;
    pointer2 = (char *)&N
;
    for ( i = 0; i < sizeof ( N
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_567
 = VECTORA ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_567;
    pointer2 = (char *)&A
;
    for ( i = 0; i < sizeof ( A
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_567
 = VECTORO ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_567;
    pointer2 = (char *)&O
;
    for ( i = 0; i < sizeof ( O
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_568
 = strcmp ( MODE, "RS" );
if( ser_568 == 0 ) goto par_559
;
goto par_556
 ;
par_556 : ;
ser_568
 = strcmp ( MODE, "ZYX" );
if( ser_568 == 0 ) goto par_559
;
goto par_560
 ;
par_559 : ;
ser_569
 = N.Z;
ser_569
 = ser_569 * -1;
ser_569
 = ASIN ( ser_569 );
RESP
  = ser_569 ;
if( RESP < 0 ) goto par_557
;
goto par_558
 ;
par_557 : ;
ser_569
 = RESP + 360 ;
RESP
  = ser_569 ;
par_558 : ;
return RESP;
par_560 : ;
ser_567
 = VECTORA ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_567;
    pointer2 = (char *)&A
;
    for ( i = 0; i < sizeof ( A
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_568
 = strcmp ( MODE, "XYZ" );
if( ser_568 == 0 ) goto par_563
;
goto par_570
 ;
par_563 : ;
ser_569
 = A.X;
ser_569
 = ASIN ( ser_569 );
RESP
  = ser_569 ;
if( RESP < 0 ) goto par_561
;
goto par_562
 ;
par_561 : ;
ser_569
 = RESP + 360 ;
RESP
  = ser_569 ;
par_562 : ;
return RESP;
par_570 : ;
ser_568
 = strcmp ( MODE, "YXZ" );
if( ser_568 == 0 ) goto par_569
;
goto par_574
 ;
par_569 : ;
ser_569
 = A.Y;
ser_569
 = ser_569 * -1;
ser_569
 = ASIN ( ser_569 );
ser_569
 = COS ( ser_569 ); 
C
  = ser_569 ;
ser_569
 = ABS ( C );
if( ser_569 > 0.005000 ) goto par_565
;
goto par_566
 ;
par_565 : ;
ser_569
 = A.Z;
ser_569
 = ser_569 / C ;
TRX
  = ser_569 ;
ser_569
 = A.X;
ser_569
 = ser_569 / C ;
TRY
  = ser_569 ;
ser_569
 = ATAN2 ( TRY, TRX );
RESP
  = ser_569 ;
if( RESP < 0 ) goto par_564
;
goto par_567
 ;
par_564 : ;
ser_569
 = RESP + 360 ;
RESP
  = ser_569 ;
par_567 : ;
goto par_568
 ;
par_566 : ;
ser_569
 = N.X;
ser_570
 = O.X;
ser_569
  = ser_569 - ser_570 ;
ser_570
 = N.X;
ser_571
 = N.Z;
ser_570
 = ser_570 + ser_571 ;
ser_571
 = ser_569 / ser_570 ;
C
  = ser_571 ;
ser_569
 = ACOS ( C );
RESP
  = ser_569 ;
par_568 : ;
return RESP;
par_574 : ;
ser_568
 = strcmp ( MODE, "ZYZ2" );
if( ser_568 == 0 ) goto par_573
;
goto par_575
 ;
par_573 : ;
ser_570
 = A.X;
ser_571
 = A.X;
ser_569
 = ABS ( ser_571 );
ser_571
 = ser_570 / ser_569 ;
ser_570
 = A.Z;
ser_569
 = ACOS ( ser_570 );
ser_570
 = ser_571 * ser_569;
RESP
  = ser_570 ;
if( RESP < 0 ) goto par_571
;
goto par_572
 ;
par_571 : ;
ser_571
 = RESP + 360 ;
RESP
  = ser_571 ;
par_572 : ;
return RESP;
par_575 : ;
return 0.000000;
}
double ANGLEZ 
(
char MODE
 [256]
,
ORIENTATION ORI

)
{
POSITION ser_572
;
long ser_573
;
float ser_574
;
float ser_575
;
float ser_576
;
POSITION N
;
POSITION A
;
POSITION O
;
float C
;
float TRX
;
float TRY
;
float RESP
;
ser_572
 = VECTORN ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_572;
    pointer2 = (char *)&N
;
    for ( i = 0; i < sizeof ( N
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_572
 = VECTORA ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_572;
    pointer2 = (char *)&A
;
    for ( i = 0; i < sizeof ( A
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_572
 = VECTORO ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_572;
    pointer2 = (char *)&O
;
    for ( i = 0; i < sizeof ( O
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_573
 = strcmp ( MODE, "RS" );
if( ser_573 == 0 ) goto par_584
;
goto par_577
 ;
par_577 : ;
ser_573
 = strcmp ( MODE, "ZYX" );
if( ser_573 == 0 ) goto par_584
;
goto par_592
 ;
par_584 : ;
ser_574
 = N.Z;
ser_574
 = ser_574 * -1;
ser_574
 = ASIN ( ser_574 );
ser_574
 = COS ( ser_574 ); 
C
  = ser_574 ;
ser_574
 = ABS ( C );
if( ser_574 > 0.005000 ) goto par_580
;
goto par_581
 ;
par_580 : ;
ser_574
 = N.X;
ser_574
 = ser_574 / C ;
TRX
  = ser_574 ;
ser_574
 = N.Y;
ser_574
 = ser_574 / C ;
TRY
  = ser_574 ;
ser_574
 = ATAN2 ( TRY, TRX );
RESP
  = ser_574 ;
if( RESP < 0 ) goto par_578
;
goto par_582
 ;
par_578 : ;
ser_574
 = RESP + 360 ;
RESP
  = ser_574 ;
par_582 : ;
goto par_583
 ;
par_581 : ;
ser_574
 = O.X;
TRX
  = ser_574 ;
ser_574
 = N.X;
TRY
  = ser_574 ;
ser_574
 = ATAN2 ( TRY, TRX );
RESP
  = ser_574 ;
if( RESP < 0 ) goto par_579
;
goto par_583
 ;
par_579 : ;
ser_574
 = RESP + 360 ;
RESP
  = ser_574 ;
par_583 : ;
return RESP;
par_592 : ;
ser_573
 = strcmp ( MODE, "XYZ" );
if( ser_573 == 0 ) goto par_591
;
goto par_600
 ;
par_591 : ;
ser_574
 = A.X;
ser_574
 = ASIN ( ser_574 );
ser_574
 = COS ( ser_574 ); 
C
  = ser_574 ;
ser_574
 = ABS ( C );
if( ser_574 > 0.005000 ) goto par_585
;
goto par_586
 ;
par_585 : ;
ser_574
 = N.X;
ser_574
 = ser_574 / C ;
TRX
  = ser_574 ;
ser_574
 = O.X;
ser_574
 = ser_574 * -1;
ser_574
 = ser_574 / C ;
TRY
  = ser_574 ;
ser_574
 = ATAN2 ( TRY, TRX );
RESP
  = ser_574 ;
par_587 : ;
goto par_589
 ;
par_586 : ;
ser_574
 = N.Y;
TRX
  = ser_574 ;
ser_574
 = O.Y;
TRY
  = ser_574 ;
ser_574
 = ATAN2 ( TRY, TRX );
RESP
  = ser_574 ;
par_589 : ;
if( RESP < 0 ) goto par_588
;
goto par_590
 ;
par_588 : ;
ser_574
 = RESP + 360 ;
RESP
  = ser_574 ;
par_590 : ;
return RESP;
par_600 : ;
ser_573
 = strcmp ( MODE, "YXZ" );
if( ser_573 == 0 ) goto par_599
;
goto par_608
 ;
par_599 : ;
ser_574
 = A.Y;
ser_574
 = ser_574 * -1;
ser_574
 = ASIN ( ser_574 );
ser_574
 = COS ( ser_574 ); 
C
  = ser_574 ;
ser_574
 = ABS ( C );
if( ser_574 > 0.005000 ) goto par_593
;
goto par_594
 ;
par_593 : ;
ser_574
 = O.Y;
ser_574
 = ser_574 / C ;
TRX
  = ser_574 ;
ser_574
 = N.Y;
ser_574
 = ser_574 / C ;
TRY
  = ser_574 ;
par_595 : ;
goto par_596
 ;
par_594 : ;
ser_574
 = N.X;
ser_575
 = N.Z;
ser_574
 = ser_574 + ser_575 ;
ser_575
 = N.X;
ser_576
 = O.X;
ser_575
  = ser_575 - ser_576 ;
ser_576
 = N.X;
ser_575
 = ser_575 / ser_576 ;
ser_576
 = N.Z;
ser_575
 = ser_575 + ser_576 ;
ser_576
 = ACOS ( ser_575 );
ser_575
 = TAN ( ser_576 ); 
ser_576
  = 1 - ser_575 ;
ser_575
 = ser_574 * ser_576;
ser_574
 = O.X;
ser_576
 = O.Z;
ser_574
 = ser_574 + ser_576 ;
ser_576
 = ser_575 + ser_574 ;
ser_575
 = ser_576 / 2.000000 ;
C
  = ser_575 ;
ser_574
 = ACOS ( C );
RESP
  = ser_574 ;
par_596 : ;
ser_576
 = ATAN2 ( TRY, TRX );
RESP
  = ser_576 ;
if( RESP < 0 ) goto par_597
;
goto par_598
 ;
par_597 : ;
ser_575
 = RESP + 360 ;
RESP
  = ser_575 ;
par_598 : ;
return RESP;
par_608 : ;
ser_573
 = strcmp ( MODE, "ZYZ2" );
if( ser_573 == 0 ) goto par_607
;
goto par_609
 ;
par_607 : ;
ser_574
 = A.X;
ser_576
 = A.X;
ser_575
 = ABS ( ser_576 );
ser_576
 = ser_574 / ser_575 ;
ser_574
 = A.Z;
ser_575
 = ACOS ( ser_574 );
ser_574
 = ser_576 * ser_575;
ser_576
 = SIN ( ser_574 ); 
C
  = ser_576 ;
ser_575
 = ABS ( C );
if( ser_575 > 0.005000 ) goto par_604
;
goto par_605
 ;
par_604 : ;
ser_574
 = A.X;
ser_576
 = ser_574 / C ;
TRX
  = ser_576 ;
ser_575
 = A.Y;
ser_574
 = ser_575 / C ;
TRY
  = ser_574 ;
ser_576
 = ATAN2 ( TRY, TRX );
RESP
  = ser_576 ;
if( RESP < 0 ) goto par_601
;
goto par_606
 ;
par_601 : ;
ser_575
 = RESP + 360 ;
RESP
  = ser_575 ;
par_606 : ;
goto par_609
 ;
par_605 : ;
ser_574
 = N.X;
TRX
  = ser_574 ;
ser_576
 = O.X;
ser_575
 = ser_576 * -1;
TRY
  = ser_575 ;
ser_574
 = ATAN2 ( TRY, TRX );
RESP
  = ser_574 ;
if( RESP < 0 ) goto par_602
;
goto par_603
 ;
par_602 : ;
ser_576
 = RESP + 360 ;
RESP
  = ser_576 ;
par_603 : ;
return RESP;
par_609 : ;
return 0.000000;
}
double ANGLEZ2 
(
char MODE
 [256]
,
ORIENTATION ORI

)
{
POSITION ser_577
;
long ser_578
;
float ser_579
;
float ser_580
;
POSITION N
;
POSITION O
;
POSITION A
;
float C
;
float TRX
;
float TRY
;
float RESP
;
ser_577
 = VECTORN ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_577;
    pointer2 = (char *)&N
;
    for ( i = 0; i < sizeof ( N
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_577
 = VECTORO ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_577;
    pointer2 = (char *)&O
;
    for ( i = 0; i < sizeof ( O
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_577
 = VECTORA ( 
 ORI 
 ) ; 

    pointer1 = (char *)&ser_577;
    pointer2 = (char *)&A
;
    for ( i = 0; i < sizeof ( A
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_578
 = strcmp ( MODE, "ZYZ2" );
if( ser_578 == 0 ) goto par_616
;
goto par_617
 ;
par_616 : ;
ser_579
 = A.X;
ser_580
 = A.X;
ser_580
 = ABS ( ser_580 );
ser_579
 = ser_579 / ser_580 ;
ser_580
 = A.Z;
ser_580
 = ACOS ( ser_580 );
ser_579
 = ser_579 * ser_580;
ser_580
 = SIN ( ser_579 ); 
C
  = ser_580 ;
ser_579
 = ABS ( C );
if( ser_579 > 0.005000 ) goto par_612
;
goto par_613
 ;
par_612 : ;
ser_580
 = N.Z;
ser_579
 = ser_580 * -1;
ser_580
 = ser_579 / C ;
TRX
  = ser_580 ;
ser_579
 = O.Z;
ser_580
 = ser_579 / C ;
TRY
  = ser_580 ;
ser_579
 = ATAN2 ( TRY, TRX );
RESP
  = ser_579 ;
if( RESP < 0 ) goto par_611
;
goto par_614
 ;
par_611 : ;
ser_580
 = RESP + 360 ;
RESP
  = ser_580 ;
par_614 : ;
goto par_615
 ;
par_613 : ;
RESP
  = 0.000000 ;
par_615 : ;
return RESP;
par_617 : ;
return 0.000000;
}
R_SPEC_TYPE * R_ACT_ROB
;
char R_JT
 [ 5 ];
BOUNDS R_JBD
 [ 5 ];
D_SPEC_TYPE D_DEVICES
 [ 4 ];
TORRE TORRES
 [ 3 ];
ORIENTATION ORI
;
POSE BASE
;
long NUM
;
long ORIGEN
;
long DESTINO
;
long TEMPORAL
;
ROBTARGET DEST
;
void POSICIONINICIAL 
(
)
{
POSITION ser_655
;
POSE ser_656
;
POSE ser_657
;
ser_655
.X = 0.000000;
ser_655
.Y = 450.000000;
ser_655
.Z = 80.000000;

    pointer1 = (char *)&ser_655;
    pointer2 = (char *)&(ser_656
.POS);
    for ( i = 0; i < sizeof ( ser_655 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ORI;
    pointer2 = (char *)&(ser_656
.ORI);
    for ( i = 0; i < sizeof ( ORI ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_656;
    pointer2 = (char *)&BASE
;
    for ( i = 0; i < sizeof ( BASE
 ) ; i ++ )
        pointer2[i] = pointer1[i];

cout << "Origen";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_657
;
    for ( i = 0; i < sizeof ( ser_657
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&BASE;
    pointer2 = (char *)&ser_657
;
    for ( i = 0; i < sizeof ( ser_657
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_657;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_657 ) ; i ++ )
        pointer2[i] = pointer1[i];

par_625 : ;
}
struct {
R_SPEC_TYPE * ACT_ROB
;
long ADAX_CONTROL
;
long DEVICE
;
float C_CP
;
float C_SPEED
;
float SPEED_ORI
;
bool C_PASS
 ;
bool WOBBLE
 ;
float SPEED
;
float ACC
;
float TIME
;
} PATH
 ;
void MOVERDISCO 
(
long ORIGEN

,
long DESTINO

)
{
POSE ser_658
;
ORIENTATION ser_659
;
POSITION ser_660
;
POSITION ser_668
;
float ser_661
;
float ser_664
;
float ser_669
;
long ser_662
;
long ser_666
;
long ser_667
;
long ser_670
;
long ser_671
;
long ser_672
;
long ser_673
;
long ser_674
;
long ser_675
;
long ser_676
;
long ser_677
;
long ser_678
;
long ser_679
;
long ser_680
;
TORRE ser_663
;
ADD_JOINT ser_665
;
long NUMFICHAS
;
float POSXTEMPORAL
;
float POSYTEMPORAL
;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.ORI);
    pointer2 = (char *)&ser_659
;
    for ( i = 0; i < sizeof ( ser_659
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ORI;
    pointer2 = (char *)&ser_659
;
    for ( i = 0; i < sizeof ( ser_659
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_659;
    pointer2 = (char *)&(ser_658
.ORI);
    for ( i = 0; i < sizeof ( ser_659 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];

cout << "Movimiento para ubicarse al lado de la ficha anterior";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.X;
ser_662
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_662]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_663.COORDENADA);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.X;
if( ser_661 < ser_664 ) goto par_626
;
goto par_627
 ;
par_626 : ;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.X;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.X;
ser_664
 = ser_664 + 50.000000 ;
ser_661
  = ser_664 ;
ser_660
.X = ser_661;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];

par_628 : ;
goto par_629
 ;
par_627 : ;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.X;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.X;
ser_661
  = ser_661 - 50.000000 ;
ser_664
  = ser_661 ;
ser_660
.X = ser_664;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];

par_629 : ;

    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_666
 = ser_665.AA1;
ser_666
  = 0 ;
ser_665
.AA1 = ser_666;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento para ubicarse a la altura de la ficha a tomar";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.X;
POSXTEMPORAL
  = ser_661 ;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.Y;
POSYTEMPORAL
  = ser_664 ;
ser_666
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_666]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_667
 = ser_663.FICHAS;
NUMFICHAS
  = ser_667 ;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_667
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_667]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_663.COORDENADA);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_668;
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_668.Z;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.Z;
ser_669
 = 80.000000 * NUMFICHAS;
ser_664
 = ser_664 + ser_669 ;
ser_661
  = ser_664 ;
ser_668
.Z = ser_661;

    pointer1 = (char *)&ser_668;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_668 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_668.X;
ser_669
  = POSXTEMPORAL ;
ser_668
.X = ser_669;

    pointer1 = (char *)&ser_668;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_668 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.Y;
ser_664
  = POSYTEMPORAL ;
ser_660
.Y = ser_664;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_670
 = ser_665.AA1;
ser_670
  = 0 ;
ser_665
.AA1 = ser_670;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento para ubicarse al frente de la ficha a tomar";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_670
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_670]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_663.COORDENADA);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_668;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_668 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.Y;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_668.Y;
ser_664
  = ser_669 - 50.000000 ;
ser_661
  = ser_664 ;
ser_660
.Y = ser_661;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_671
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_671]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_672
 = ser_663.FICHAS;
NUMFICHAS
  = ser_672 ;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_660.Z;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_668.Z;
ser_661
 = 80.000000 * NUMFICHAS;
ser_664
 = ser_664 + ser_661 ;
ser_669
  = ser_664 ;
ser_660
.Z = ser_669;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_672
 = ser_665.AA1;
ser_672
  = 0 ;
ser_665
.AA1 = ser_672;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento para recojer la ficha";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.Y;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_668.Y;
ser_669
 = ser_664 + 50.000000 ;
ser_661
  = ser_669 ;
ser_660
.Y = ser_661;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_672
 = ser_665.AA1;
ser_672
  = 0 ;
ser_665
.AA1 = ser_672;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento sacar la ficha";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.Z;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_668.Z;
ser_661
 = ser_669 + 0.000000 ;
ser_664
  = ser_661 ;
ser_660
.Z = ser_664;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_672
 = ser_665.AA1;
ser_672
  = 1 ;
ser_665
.AA1 = ser_672;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento sobre la torre";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_660.Z;
ser_669
  = 450.000000 ;
ser_660
.Z = ser_669;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_672
 = ser_665.AA1;
ser_672
  = 1 ;
ser_665
.AA1 = ser_672;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento sobre torre destino";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_672
  = DESTINO - 1 ;

    pointer1 = (char *)&(TORRES [ser_672]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_663.COORDENADA);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_668;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_668 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.Z;
ser_661
  = 450.000000 ;
ser_660
.Z = ser_661;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_673
 = ser_665.AA1;
ser_673
  = 1 ;
ser_665
.AA1 = ser_673;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento ubicar la ficha sobre el destino";
//cout.setf (ios::left);
cout << endl;
ser_673
  = DESTINO - 1 ;

    pointer1 = (char *)&(TORRES [ser_673]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_674
 = ser_663.FICHAS;
NUMFICHAS
  = ser_674 ;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_674
  = DESTINO - 1 ;

    pointer1 = (char *)&(TORRES [ser_674]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_663.COORDENADA);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_668;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_668 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_660.Z;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_668.Z;
ser_675
 = NUMFICHAS + 1 ;
ser_661
 = 80.000000 * ser_675;
ser_669
 = ser_669 + ser_661 ;
ser_661
 = ser_669 + 0.000000 ;
ser_664
  = ser_661 ;
ser_660
.Z = ser_664;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_675
 = ser_665.AA1;
ser_675
  = 1 ;
ser_665
.AA1 = ser_675;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento para dejar la ficha";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_669
 = ser_660.Z;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_668.Z;
ser_664
  = ser_661 - 0.000000 ;
ser_669
  = ser_664 ;
ser_660
.Z = ser_669;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_675
 = ser_665.AA1;
ser_675
  = 1 ;
ser_665
.AA1 = ser_675;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout << "Movimiento de regreso";
//cout.setf (ios::left);
cout << endl;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_660
;
    for ( i = 0; i < sizeof ( ser_660
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_661
 = ser_660.Y;

    pointer1 = (char *)&(DEST.PSE);
    pointer2 = (char *)&ser_658
;
    for ( i = 0; i < sizeof ( ser_658
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_658.POS);
    pointer2 = (char *)&ser_668
;
    for ( i = 0; i < sizeof ( ser_668
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_664
 = ser_668.Y;
ser_669
  = ser_664 - 50.000000 ;
ser_661
  = ser_669 ;
ser_660
.Y = ser_661;

    pointer1 = (char *)&ser_660;
    pointer2 = (char *)&(ser_658
.POS);
    for ( i = 0; i < sizeof ( ser_660 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_658;
    pointer2 = (char *)&(DEST
.PSE);
    for ( i = 0; i < sizeof ( ser_658 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(DEST.A_JOINT);
    pointer2 = (char *)&ser_665
;
    for ( i = 0; i < sizeof ( ser_665
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_675
 = ser_665.AA1;
ser_675
  = 0 ;
ser_665
.AA1 = ser_675;

    pointer1 = (char *)&ser_665;
    pointer2 = (char *)&(DEST
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_665 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    p = new punto;
    p->tipo = cTARGET;
    Transformar ( DEST, p->valor );
    p->r = &DEST;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

cout.width ( 10
 );
cout.precision( 4
 );
//cout.setf (ios::rigth);
cout << ORIGEN;
//cout.setf (ios::left);
cout << "->";
//cout.setf (ios::left);
cout.width ( 10
 );
cout.precision( 4
 );
//cout.setf (ios::rigth);
cout << DESTINO;
//cout.setf (ios::left);
cout << endl;
ser_675
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_675]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_676
 = ser_663.FICHAS;
ser_677
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_677]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_678
 = ser_663.FICHAS;
ser_678
  = ser_678 - 1 ;
ser_676
  = ser_678 ;
ser_663
.FICHAS = ser_676;

    pointer1 = (char *)&ser_663;
    pointer2 = (char *)&(TORRES
[ ser_675 ]);
    for ( i = 0; i < sizeof ( ser_663 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_678
  = DESTINO - 1 ;

    pointer1 = (char *)&(TORRES [ser_678]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_676
 = ser_663.FICHAS;
ser_679
  = DESTINO - 1 ;

    pointer1 = (char *)&(TORRES [ser_679]);
    pointer2 = (char *)&ser_663
;
    for ( i = 0; i < sizeof ( ser_663
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_680
 = ser_663.FICHAS;
ser_680
 = ser_680 + 1 ;
ser_676
  = ser_680 ;
ser_663
.FICHAS = ser_676;

    pointer1 = (char *)&ser_663;
    pointer2 = (char *)&(TORRES
[ ser_678 ]);
    for ( i = 0; i < sizeof ( ser_663 ) ; i ++ )
        pointer2[i] = pointer1[i];

par_630 : ;
}
void ALGORITMOHANOI 
(
long DISCOS

,
long ORIGEN

,
long DESTINO

,
long TEMPORAL

)
{
long ser_681
;
if( DISCOS == 1 ) goto par_631
;
goto par_632
 ;
par_631 : ;
MOVERDISCO (  
 ORIGEN 
,
 DESTINO 
 ) ; 
return;
par_632 : ;
ser_681
  = DISCOS - 1 ;
ALGORITMOHANOI (  
 ser_681 
,
 ORIGEN 
,
 TEMPORAL 
,
 DESTINO 
 ) ; 
MOVERDISCO (  
 ORIGEN 
,
 DESTINO 
 ) ; 
ser_681
  = DISCOS - 1 ;
ALGORITMOHANOI (  
 ser_681 
,
 TEMPORAL 
,
 DESTINO 
,
 ORIGEN 
 ) ; 
par_633 : ;
}

#ifndef DATALIST
char datalist [250] = "datalist.dls";
#define DATALIST
#endif

int main ( )
{ 
    filedatalist.open(datalist,ios::in|ios::binary);
    


    char nombreTrayectoria [ 256 ];
    int tamano = strlen ( "HanoiRVM1.irl" );
    strcpy (nombreTrayectoria, "HanoiRVM1.irl" );
    nombreTrayectoria [ tamano - 3 ] = 't';
    nombreTrayectoria [ tamano - 2 ] = 'j';
    nombreTrayectoria [ tamano - 1 ] = 'c';

    if((archivotrayectoria=fopen(nombreTrayectoria,"w"))==NULL)
    {
        cout<<"Error en la construcción de la trayectoria."<<endl;
        exit(1);
    }
    EnviarComando ( "IOIC::Worlds GetWorldId", Resultado );
    IDWorld = atoi ( Resultado );
    fprintf (archivotrayectoria, "\"IRLC\"\"Generado por IRLC\" 0 0 0.0 \n 0 0 \n ");
    fprintf (archivotrayectoria, "( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ) 0 3 0\n" );

par_620 : ;
ser_601
.R_ACC = 50.000000;
ser_581
 [ 0 ] = 50.000000 ;
ser_581
 [ 1 ] = 50.000000 ;
ser_581
 [ 2 ] = 50.000000 ;
ser_581
 [ 3 ] = 50.000000 ;
ser_581
 [ 4 ] = 50.000000 ;

    pointer1 = (char *)&ser_581;
    pointer2 = (char *)&(ser_601
.R_ACC_PTP);
    for ( i = 0; i < sizeof ( ser_581 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_601
.R_C_PTP = 50.000000;
ser_601
.R_C_CP = 200.000000;
ser_601
.R_C_SPEED = 50.000000;
ser_582
 [ 0 ] = 1 ;
ser_582
 [ 1 ] = 3 ;
ser_582
 [ 2 ] = -1 ;
ser_582
 [ 3 ] = -1 ;

    pointer1 = (char *)&ser_582;
    pointer2 = (char *)&(ser_601
.R_DEVICES);
    for ( i = 0; i < sizeof ( ser_582 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_583
.A1 = 40;
ser_583
.A2 = 35;
ser_583
.A3 = 25;
ser_583
.A4 = 15;
ser_583
.A5 = 0;

    pointer1 = (char *)&ser_583;
    pointer2 = (char *)&(ser_585
.M_JOINT);
    for ( i = 0; i < sizeof ( ser_583 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_584
.AA1 = 0;

    pointer1 = (char *)&ser_584;
    pointer2 = (char *)&(ser_585
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_584 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_585.M_JOINT);
    pointer2 = (char *)&ser_583
;
    for ( i = 0; i < sizeof ( ser_583
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_583.A1);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_605
  = ser_604 ;
ser_603
.A1 = ser_605;

    pointer1 = (char *)&(ser_583.A2);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_606
  = ser_604 ;
ser_603
.A2 = ser_606;

    pointer1 = (char *)&(ser_583.A3);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_607
  = ser_604 ;
ser_603
.A3 = ser_607;

    pointer1 = (char *)&(ser_583.A4);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_608
  = ser_604 ;
ser_603
.A4 = ser_608;

    pointer1 = (char *)&(ser_583.A5);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_609
  = ser_604 ;
ser_603
.A5 = ser_609;

    pointer1 = (char *)&ser_603;
    pointer2 = (char *)&(ser_602
.M_JOINT);
    for ( i = 0; i < sizeof ( ser_603 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_585.A_JOINT);
    pointer2 = (char *)&ser_584
;
    for ( i = 0; i < sizeof ( ser_584
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_584.AA1);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_611
  = ser_604 ;
ser_610
.AA1 = ser_611;

    pointer1 = (char *)&ser_610;
    pointer2 = (char *)&(ser_602
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_610 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_602;
    pointer2 = (char *)&(ser_601
.R_JOINT_ACT);
    for ( i = 0; i < sizeof ( ser_602 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_586
.X = 100.000000;
ser_586
.Y = 1000.000000;
ser_586
.Z = 0.000000;
ser_587
 = ORIRS ( 
 90.000000 
,
 0.000000 
,
 90.000000 
 ) ; 

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_588
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&(ser_588
.ORI);
    for ( i = 0; i < sizeof ( ser_587 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_588;
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&(ser_590
.PSE);
    for ( i = 0; i < sizeof ( ser_591 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_590
.STATUS = 0;
ser_589
 [ 0 ] = 0 ;
ser_589
 [ 1 ] = 0 ;

    pointer1 = (char *)&ser_589;
    pointer2 = (char *)&(ser_590
.TURNS);
    for ( i = 0; i < sizeof ( ser_589 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_584
.AA1 = 0;

    pointer1 = (char *)&ser_584;
    pointer2 = (char *)&(ser_590
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_584 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_590.PSE);
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&ser_613
;
    for ( i = 0; i < sizeof ( ser_613
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_613;
    pointer2 = (char *)&(ser_612
.PSE);
    for ( i = 0; i < sizeof ( ser_613 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_590.STATUS);
    pointer2 = (char *)&ser_604
;
    for ( i = 0; i < sizeof ( ser_604
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_614
  = ser_604 ;
ser_612
.STATUS = ser_614;

    pointer1 = (char *)&(ser_590.TURNS);
    pointer2 = (char *)&ser_589
;
    for ( i = 0; i < sizeof ( ser_589
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_615
 [ 0 ] = ser_589 [ 0 ];
ser_615
 [ 1 ] = ser_589 [ 1 ];

    pointer1 = (char *)&ser_615;
    pointer2 = (char *)&(ser_612
.TURNS);
    for ( i = 0; i < sizeof ( ser_615 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_590.A_JOINT);
    pointer2 = (char *)&ser_584
;
    for ( i = 0; i < sizeof ( ser_584
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_584.AA1);
    pointer2 = (char *)&ser_618
;
    for ( i = 0; i < sizeof ( ser_618
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_619
  = ser_618 ;
ser_617
.AA1 = ser_619;

    pointer1 = (char *)&ser_617;
    pointer2 = (char *)&(ser_612
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_617 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_612;
    pointer2 = (char *)&(ser_601
.R_ROBTARGET_ACT);
    for ( i = 0; i < sizeof ( ser_612 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_586
.X = 100.000000;
ser_586
.Y = 1000.000000;
ser_586
.Z = 0.000000;
ser_587
 = ORIRS ( 
 90.000000 
,
 0.000000 
,
 90.000000 
 ) ; 

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_592
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&(ser_592
.ORI);
    for ( i = 0; i < sizeof ( ser_587 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_592;
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&(ser_593
.PSE);
    for ( i = 0; i < sizeof ( ser_591 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_593
.STATUS = 0;
ser_589
 [ 0 ] = 0 ;
ser_589
 [ 1 ] = 0 ;

    pointer1 = (char *)&ser_589;
    pointer2 = (char *)&(ser_593
.TURNS);
    for ( i = 0; i < sizeof ( ser_589 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_584
.AA1 = 0;

    pointer1 = (char *)&ser_584;
    pointer2 = (char *)&(ser_593
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_584 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_593.PSE);
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&ser_621
;
    for ( i = 0; i < sizeof ( ser_621
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_621;
    pointer2 = (char *)&(ser_620
.PSE);
    for ( i = 0; i < sizeof ( ser_621 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_593.STATUS);
    pointer2 = (char *)&ser_618
;
    for ( i = 0; i < sizeof ( ser_618
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_622
  = ser_618 ;
ser_620
.STATUS = ser_622;

    pointer1 = (char *)&(ser_593.TURNS);
    pointer2 = (char *)&ser_589
;
    for ( i = 0; i < sizeof ( ser_589
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_623
 [ 0 ] = ser_589 [ 0 ];
ser_623
 [ 1 ] = ser_589 [ 1 ];

    pointer1 = (char *)&ser_623;
    pointer2 = (char *)&(ser_620
.TURNS);
    for ( i = 0; i < sizeof ( ser_623 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_593.A_JOINT);
    pointer2 = (char *)&ser_584
;
    for ( i = 0; i < sizeof ( ser_584
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_584.AA1);
    pointer2 = (char *)&ser_626
;
    for ( i = 0; i < sizeof ( ser_626
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_627
  = ser_626 ;
ser_625
.AA1 = ser_627;

    pointer1 = (char *)&ser_625;
    pointer2 = (char *)&(ser_620
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_625 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_620;
    pointer2 = (char *)&(ser_601
.R_ROBTARGET_START);
    for ( i = 0; i < sizeof ( ser_620 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_586
.X = 100.000000;
ser_586
.Y = 1000.000000;
ser_586
.Z = 0.000000;
ser_587
 = ORIRS ( 
 90.000000 
,
 0.000000 
,
 90.000000 
 ) ; 

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_594
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&(ser_594
.ORI);
    for ( i = 0; i < sizeof ( ser_587 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_594;
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&(ser_595
.PSE);
    for ( i = 0; i < sizeof ( ser_591 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_595
.STATUS = 0;
ser_589
 [ 0 ] = 0 ;
ser_589
 [ 1 ] = 0 ;

    pointer1 = (char *)&ser_589;
    pointer2 = (char *)&(ser_595
.TURNS);
    for ( i = 0; i < sizeof ( ser_589 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_584
.AA1 = 0;

    pointer1 = (char *)&ser_584;
    pointer2 = (char *)&(ser_595
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_584 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_595.PSE);
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&ser_629
;
    for ( i = 0; i < sizeof ( ser_629
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_629;
    pointer2 = (char *)&(ser_628
.PSE);
    for ( i = 0; i < sizeof ( ser_629 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_595.STATUS);
    pointer2 = (char *)&ser_626
;
    for ( i = 0; i < sizeof ( ser_626
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_630
  = ser_626 ;
ser_628
.STATUS = ser_630;

    pointer1 = (char *)&(ser_595.TURNS);
    pointer2 = (char *)&ser_589
;
    for ( i = 0; i < sizeof ( ser_589
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_631
 [ 0 ] = ser_589 [ 0 ];
ser_631
 [ 1 ] = ser_589 [ 1 ];

    pointer1 = (char *)&ser_631;
    pointer2 = (char *)&(ser_628
.TURNS);
    for ( i = 0; i < sizeof ( ser_631 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_595.A_JOINT);
    pointer2 = (char *)&ser_584
;
    for ( i = 0; i < sizeof ( ser_584
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_584.AA1);
    pointer2 = (char *)&ser_634
;
    for ( i = 0; i < sizeof ( ser_634
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_635
  = ser_634 ;
ser_633
.AA1 = ser_635;

    pointer1 = (char *)&ser_633;
    pointer2 = (char *)&(ser_628
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_633 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_628;
    pointer2 = (char *)&(ser_601
.R_ROBTARGET_END);
    for ( i = 0; i < sizeof ( ser_628 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_586
.X = 100.000000;
ser_586
.Y = 1000.000000;
ser_586
.Z = 0.000000;
ser_587
 = ORIRS ( 
 90.000000 
,
 0.000000 
,
 90.000000 
 ) ; 

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_596
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&(ser_596
.ORI);
    for ( i = 0; i < sizeof ( ser_587 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_596;
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&(ser_597
.PSE);
    for ( i = 0; i < sizeof ( ser_591 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_597
.STATUS = 0;
ser_589
 [ 0 ] = 0 ;
ser_589
 [ 1 ] = 0 ;

    pointer1 = (char *)&ser_589;
    pointer2 = (char *)&(ser_597
.TURNS);
    for ( i = 0; i < sizeof ( ser_589 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_584
.AA1 = 0;

    pointer1 = (char *)&ser_584;
    pointer2 = (char *)&(ser_597
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_584 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_597.PSE);
    pointer2 = (char *)&ser_591
;
    for ( i = 0; i < sizeof ( ser_591
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_591;
    pointer2 = (char *)&ser_637
;
    for ( i = 0; i < sizeof ( ser_637
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_637;
    pointer2 = (char *)&(ser_636
.PSE);
    for ( i = 0; i < sizeof ( ser_637 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_597.STATUS);
    pointer2 = (char *)&ser_634
;
    for ( i = 0; i < sizeof ( ser_634
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_638
  = ser_634 ;
ser_636
.STATUS = ser_638;

    pointer1 = (char *)&(ser_597.TURNS);
    pointer2 = (char *)&ser_589
;
    for ( i = 0; i < sizeof ( ser_589
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_639
 [ 0 ] = ser_589 [ 0 ];
ser_639
 [ 1 ] = ser_589 [ 1 ];

    pointer1 = (char *)&ser_639;
    pointer2 = (char *)&(ser_636
.TURNS);
    for ( i = 0; i < sizeof ( ser_639 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_597.A_JOINT);
    pointer2 = (char *)&ser_584
;
    for ( i = 0; i < sizeof ( ser_584
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(ser_584.AA1);
    pointer2 = (char *)&ser_642
;
    for ( i = 0; i < sizeof ( ser_642
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_643
  = ser_642 ;
ser_641
.AA1 = ser_643;

    pointer1 = (char *)&ser_641;
    pointer2 = (char *)&(ser_636
.A_JOINT);
    for ( i = 0; i < sizeof ( ser_641 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_636;
    pointer2 = (char *)&(ser_601
.R_ROBTARGET_INTER);
    for ( i = 0; i < sizeof ( ser_636 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_601
.R_SPEED_ACT = 0.000000;
ser_601
.R_SPEED = 4.000000;
ser_598
 [ 0 ] = 100.000000 ;
ser_598
 [ 1 ] = 100.000000 ;
ser_598
 [ 2 ] = 100.000000 ;
ser_598
 [ 3 ] = 50.000000 ;
ser_598
 [ 4 ] = 50.000000 ;

    pointer1 = (char *)&ser_598;
    pointer2 = (char *)&(ser_601
.R_SPEED_PTP);
    for ( i = 0; i < sizeof ( ser_598 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_601
.R_SPEED_ORI = 50.000000;
ser_586
.X = 0.000000;
ser_586
.Y = 0.000000;
ser_586
.Z = 0.000000;
ser_587
 = ORIRS ( 
 0.000000 
,
 0.000000 
,
 0.000000 
 ) ; 

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_599
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&(ser_599
.ORI);
    for ( i = 0; i < sizeof ( ser_587 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_599;
    pointer2 = (char *)&ser_644
;
    for ( i = 0; i < sizeof ( ser_644
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_644;
    pointer2 = (char *)&(ser_601
.R_BASE);
    for ( i = 0; i < sizeof ( ser_644 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_586
.X = 0.000000;
ser_586
.Y = 0.000000;
ser_586
.Z = -100.000000;
ser_587
 = ORIRS ( 
 90.000000 
,
 0.000000 
,
 0.000000 
 ) ; 

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_600
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&(ser_600
.ORI);
    for ( i = 0; i < sizeof ( ser_587 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_600;
    pointer2 = (char *)&ser_645
;
    for ( i = 0; i < sizeof ( ser_645
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_645;
    pointer2 = (char *)&(ser_601
.R_TOOL);
    for ( i = 0; i < sizeof ( ser_645 ) ; i ++ )
        pointer2[i] = pointer1[i];


    R_SPEC_TYPE * RVM1
;
    RVM1
 = & ser_601;
    robots[robot] = RVM1
;
    robot ++;

R_ACT_ROB
  = RVM1 ;
ser_646
.D_NO = 1;
ser_646
.D_AXES_TYPE = 0;
ser_646
.D_PRED_DEVICE = -1;
ser_646
.D_NJ = 6;

    pointer1 = (char *)&ser_646;
    pointer2 = (char *)&(ser_650
[ 0 ]);
    for ( i = 0; i < sizeof ( ser_646 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_647
.D_NO = 2;
ser_647
.D_AXES_TYPE = 0;
ser_647
.D_PRED_DEVICE = -1;
ser_647
.D_NJ = 6;

    pointer1 = (char *)&ser_647;
    pointer2 = (char *)&(ser_650
[ 1 ]);
    for ( i = 0; i < sizeof ( ser_647 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_648
.D_NO = 3;
ser_648
.D_AXES_TYPE = 3;
ser_648
.D_PRED_DEVICE = -1;
ser_648
.D_NJ = 2;

    pointer1 = (char *)&ser_648;
    pointer2 = (char *)&(ser_650
[ 2 ]);
    for ( i = 0; i < sizeof ( ser_648 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_649
.D_NO = 4;
ser_649
.D_AXES_TYPE = 3;
ser_649
.D_PRED_DEVICE = -1;
ser_649
.D_NJ = 3;

    pointer1 = (char *)&ser_649;
    pointer2 = (char *)&(ser_650
[ 3 ]);
    for ( i = 0; i < sizeof ( ser_649 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_646
.D_NO = 1;
ser_646
.D_AXES_TYPE = 0;
ser_646
.D_PRED_DEVICE = -1;
ser_646
.D_NJ = 6;

    pointer1 = (char *)&ser_646;
    pointer2 = (char *)&(ser_650
[ 0 ]);
    for ( i = 0; i < sizeof ( ser_646 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_647
.D_NO = 2;
ser_647
.D_AXES_TYPE = 0;
ser_647
.D_PRED_DEVICE = -1;
ser_647
.D_NJ = 6;

    pointer1 = (char *)&ser_647;
    pointer2 = (char *)&(ser_650
[ 1 ]);
    for ( i = 0; i < sizeof ( ser_647 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_648
.D_NO = 3;
ser_648
.D_AXES_TYPE = 3;
ser_648
.D_PRED_DEVICE = -1;
ser_648
.D_NJ = 2;

    pointer1 = (char *)&ser_648;
    pointer2 = (char *)&(ser_650
[ 2 ]);
    for ( i = 0; i < sizeof ( ser_648 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_649
.D_NO = 4;
ser_649
.D_AXES_TYPE = 3;
ser_649
.D_PRED_DEVICE = -1;
ser_649
.D_NJ = 3;

    pointer1 = (char *)&ser_649;
    pointer2 = (char *)&(ser_650
[ 3 ]);
    for ( i = 0; i < sizeof ( ser_649 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(D_DEVICES
 [ 0 ]);
    pointer2 = (char *)&(ser_650 [ 0 ]);
    for ( i = 0; i < sizeof ( ser_650 [ 0 ] ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(D_DEVICES
 [ 1 ]);
    pointer2 = (char *)&(ser_650 [ 1 ]);
    for ( i = 0; i < sizeof ( ser_650 [ 1 ] ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(D_DEVICES
 [ 2 ]);
    pointer2 = (char *)&(ser_650 [ 2 ]);
    for ( i = 0; i < sizeof ( ser_650 [ 2 ] ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(D_DEVICES
 [ 3 ]);
    pointer2 = (char *)&(ser_650 [ 3 ]);
    for ( i = 0; i < sizeof ( ser_650 [ 3 ] ) ; i ++ )
        pointer2[i] = pointer1[i];

par_623 : ;
ser_651
.FICHAS = 0;
ser_586
.X = -200.000000;
ser_586
.Y = 480.000000;
ser_586
.Z = 0.000000;

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_651
.COORDENADA);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_651;
    pointer2 = (char *)&(ser_654
[ 0 ]);
    for ( i = 0; i < sizeof ( ser_651 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_652
.FICHAS = 0;
ser_586
.X = 50.000000;
ser_586
.Y = 480.000000;
ser_586
.Z = 0.000000;

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_652
.COORDENADA);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_652;
    pointer2 = (char *)&(ser_654
[ 1 ]);
    for ( i = 0; i < sizeof ( ser_652 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_653
.FICHAS = 0;
ser_586
.X = 250.000000;
ser_586
.Y = 450.000000;
ser_586
.Z = 0.000000;

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_653
.COORDENADA);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_653;
    pointer2 = (char *)&(ser_654
[ 2 ]);
    for ( i = 0; i < sizeof ( ser_653 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_651
.FICHAS = 0;
ser_586
.X = -200.000000;
ser_586
.Y = 480.000000;
ser_586
.Z = 0.000000;

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_651
.COORDENADA);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_651;
    pointer2 = (char *)&(ser_654
[ 0 ]);
    for ( i = 0; i < sizeof ( ser_651 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_652
.FICHAS = 0;
ser_586
.X = 50.000000;
ser_586
.Y = 480.000000;
ser_586
.Z = 0.000000;

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_652
.COORDENADA);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_652;
    pointer2 = (char *)&(ser_654
[ 1 ]);
    for ( i = 0; i < sizeof ( ser_652 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_653
.FICHAS = 0;
ser_586
.X = 250.000000;
ser_586
.Y = 450.000000;
ser_586
.Z = 0.000000;

    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(ser_653
.COORDENADA);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_653;
    pointer2 = (char *)&(ser_654
[ 2 ]);
    for ( i = 0; i < sizeof ( ser_653 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(TORRES
 [ 0 ]);
    pointer2 = (char *)&(ser_654 [ 0 ]);
    for ( i = 0; i < sizeof ( ser_654 [ 0 ] ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(TORRES
 [ 1 ]);
    pointer2 = (char *)&(ser_654 [ 1 ]);
    for ( i = 0; i < sizeof ( ser_654 [ 1 ] ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&(TORRES
 [ 2 ]);
    pointer2 = (char *)&(ser_654 [ 2 ]);
    for ( i = 0; i < sizeof ( ser_654 [ 2 ] ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_587
 = ORIZYX ( 
 90.000000 
,
 0.000000 
,
 -5.000000 
 ) ; 
ser_587
 = ORIZYX ( 
 90.000000 
,
 0.000000 
,
 -5.000000 
 ) ; 

    pointer1 = (char *)&ser_587;
    pointer2 = (char *)&ORI
;
    for ( i = 0; i < sizeof ( ORI
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ORIGEN
  = 1 ;
DESTINO
  = 3 ;
NUM
  = 4 ;
if( ORIGEN != DESTINO ) goto par_634
;
goto par_646
 ;
par_634 : ;
if( ORIGEN <= 3 ) goto par_635
;
goto par_646
 ;
par_635 : ;
if( ORIGEN >= 1 ) goto par_636
;
goto par_646
 ;
par_636 : ;
if( DESTINO <= 3 ) goto par_637
;
goto par_646
 ;
par_637 : ;
if( DESTINO >= 1 ) goto par_645
;
goto par_646
 ;
par_645 : ;
ser_642
 = ORIGEN + DESTINO ;
if( ser_642 == 4 ) goto par_641
;
goto par_642
 ;
par_641 : ;
TEMPORAL
  = 2 ;
par_643 : ;
goto par_644
 ;
par_642 : ;
ser_642
 = ORIGEN + DESTINO ;
if( ser_642 == 3 ) goto par_638
;
goto par_639
 ;
par_638 : ;
TEMPORAL
  = 3 ;
par_640 : ;
goto par_644
 ;
par_639 : ;
TEMPORAL
  = 1 ;
par_644 : ;
ser_642
  = ORIGEN - 1 ;

    pointer1 = (char *)&(TORRES [ser_642]);
    pointer2 = (char *)&ser_682
;
    for ( i = 0; i < sizeof ( ser_682
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_683
 = ser_682.FICHAS;
ser_683
  = NUM ;
ser_682
.FICHAS = ser_683;

    pointer1 = (char *)&ser_682;
    pointer2 = (char *)&(TORRES
[ ser_642 ]);
    for ( i = 0; i < sizeof ( ser_682 ) ; i ++ )
        pointer2[i] = pointer1[i];

POSICIONINICIAL (  
 ) ; 
ALGORITMOHANOI (  
 NUM 
,
 ORIGEN 
,
 DESTINO 
,
 TEMPORAL 
 ) ; 
par_647 : ;
goto par_648
 ;
par_646 : ;
cout << "Error";
//cout.setf (ios::left);
cout << endl;
par_648 : ;

    pointer1 = (char *)&(BASE.POS);
    pointer2 = (char *)&ser_586
;
    for ( i = 0; i < sizeof ( ser_586
 ) ; i ++ )
        pointer2[i] = pointer1[i];

ser_684
.X = 400.000000;
ser_684
.Y = 200.000000;
ser_684
.Z = 450.000000;

    pointer1 = (char *)&ser_684;
    pointer2 = (char *)&ser_586
;
    for ( i = 0; i < sizeof ( ser_586
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&ser_586;
    pointer2 = (char *)&(BASE
.POS);
    for ( i = 0; i < sizeof ( ser_586 ) ; i ++ )
        pointer2[i] = pointer1[i];

PATH
.C_CP = R_C_CP;
PATH
.SPEED = R_SPEED;
PATH
.SPEED_ORI = R_SPEED_ORI;
PATH
.C_SPEED = R_C_SPEED;
PATH
.ACC = R_ACC;
PATH
.ACT_ROB = R_ACT_ROB;
PATH
.ADAX_CONTROL = 0;
PATH
.DEVICE = 1;
PATH
.C_PASS = false;
PATH
.WOBBLE = false;
PATH
.TIME = -1;

    pointer1 = (char *)&(R_ACT_ROB->R_ROBTARGET_ACT);
    pointer2 = (char *)&ser_590
;
    for ( i = 0; i < sizeof ( ser_590
 ) ; i ++ )
        pointer2[i] = pointer1[i];


    pointer1 = (char *)&BASE;
    pointer2 = (char *)&(ser_590
.PSE);
    for ( i = 0; i < sizeof ( BASE ) ; i ++ )
        pointer2[i] = pointer1[i];


    p = new punto;
    p->tipo = cTARGET;
    Transformar ( ser_590, p->valor );
    p->r = &ser_590;
    puntos.Listadd(p);


    if ( trayectorias > 0 )
        fprintf (archivotrayectoria, ", \n ");
    fprintf (archivotrayectoria, "{\"Sec%d\" 0 0 1 0 1 1 0 0 3 2 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 \n Cartesian \n [ \n ", trayectorias);
    Transformar ( R_ACT_ROB->R_ROBTARGET_ACT, cadena );
    fprintf (archivotrayectoria, " %s, \n ", cadena );
    for ( i = 1; i <= puntos.Listlength ( ); i++ )
    {
        p = puntos.Listdata ( i );
        if(p->tipo == cTARGET)
            fprintf (archivotrayectoria, " %s ", p->valor );
        if ( i < puntos.Listlength ( ) )
            fprintf (archivotrayectoria, ", \n " );
        else
        {
            pointer1 = (char *)p->r;
            pointer2 = (char *)&R_ACT_ROB->R_ROBTARGET_ACT;
            for(i = 0; i < sizeof(R_ACT_ROB->R_ROBTARGET_ACT); i++)
                pointer2[i] = pointer1[i];
        }
    }
    puntos.Listdestroy ( );
    fprintf (archivotrayectoria, "\n ] \n } " );
    trayectorias++;

par_650 : ;

    fclose(archivotrayectoria);
    filedatalist.close ();
    return 1; 
}



void Transformar ( ROBTARGET r, char texto [ 1024 ] )
{    
    sprintf ( texto, "( %f, %f, %f, %f, %f, %f ) 0 3 0 \n ( %f %f %f ) ( %f %f %f ) ( 0.0 0.0 0.0 ) ( 0.0 0.0 0.0 ) ( 0.0 0.0 0.0 ) 0.0", 
        r.PSE.POS.X / 1000,
        r.PSE.POS.Y / 1000,
        r.PSE.POS.Z / 1000,
        ANGLEZ ( "ZYX", r.PSE.ORI ),
        ANGLEY ( "ZYX", r.PSE.ORI ),
        ANGLEX ( "ZYX", r.PSE.ORI ),
        r.PSE.POS.X / 1000,
        r.PSE.POS.Y / 1000,
        r.PSE.POS.Z / 1000,
        ANGLEZ ( "ZYX", r.PSE.ORI ),
        ANGLEY ( "ZYX", r.PSE.ORI ),
        ANGLEX ( "ZYX", r.PSE.ORI )
    );
}


