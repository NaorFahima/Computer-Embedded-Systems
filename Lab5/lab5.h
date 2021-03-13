#pragma once

#include "ssd.h"
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include <p32xxxx.h>

#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#define _ON 1
#define OFF 0
#define NONE -1
#define LCD_SCREEN_SIZE 16
#define LCD_FIRST_LINE 0x80
#define LCD_SECOND_LINE 0xce

// global variable counting interrupt cycles (10 cyces = 1 second)
int counter_interrupt = 0;

// global variables holding each digit of the clock
int minute = 0, tens_minute = 0, second = 0, tens_second = 0;

// system configurations (LED, switches, audio)
void sys_config();

// hold program for given iterations
void hold_program();

// when interrupt is on, go to this function
void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void);
