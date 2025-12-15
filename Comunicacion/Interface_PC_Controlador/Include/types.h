#ifndef __TYPES_H__
#define __TYPES_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:      types.h
// Ubicación fuente: No tiene
//
// Proyecto: JARBOTER V2.1. Controlador Robótico de 5 grados de libertad
//  
// Propósito General:	
// Incluye todas los tipos de datos necesarias tanto para el microcontrolador como para el PC.
//
// Compilado en:
//    avr-gcc 4.3.3 
//
// Author: Felipe Hurtado
//         David Osorio
//
// Version: 1.0
//  
// 
// Copyright (c) 2010
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined _WIN32
#include <avr/io.h>
#endif

#include "COMMONtypes.h"

#if !defined _AVR_IO_H_			//si no es complilado por el uControlador
	
	#include "PCtypes.h"
	#include "USBtypes.h"	
#endif

#if defined _AVR_IO_H_
#include "UCtypes.h"
#endif

#endif
