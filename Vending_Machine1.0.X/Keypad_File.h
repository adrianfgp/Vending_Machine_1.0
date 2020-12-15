/*!
\file   keypad.c
\date   2020-12-14
\author Adrian Felipe Gironza<adrianf@unicauca.edu.co>
        Jose Ricardo Galindez<jrgalindez@unicauca.edu.co>
        Kevin Daryany Morales<kevindm@unicauca.edu.co>
\brief  Example keypad.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2020. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/
#ifndef KEYPAD_H
#define	KEYPAD_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include <pic18f4550.h>
#include "Configuration_Header_File.h"
#include "20x4_LCD_File.h"

#define write_port LATD             /* latch register to write data on port */
#define read_port PORTD             /* PORT register to read data of port */
#define Direction_Port TRISD

unsigned char keyfind();            /* function to find pressed key*/
//void MSdelayr(unsigned int );        /*Generate delay in ms*/


unsigned char col_loc,rowloc,temp_col;

unsigned char keypad[4][4]= {'7','8','9','/',
                             '4','5','6','*',
                             '1','2','3','-',
                             ' ','0','=','+'};


#endif	/* KEYPAD_H */

