#pragma once

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

// global variables counting interrupt cycles and displaying number
int counter_number = 0, counter_interrupt = 0;

// system configurations (LED, switches, audio)
void sys_config();

// configure LCD module
void lcd_init(char *control, int size);

// send instruction to clear LCD screen
void lcd_clear();

// write first_line and second_line in their correct position
void lcd_set_text(char first_line[], char second_line[]);

// set LCD cursor to given line
void lcd_set_cursor(int line);

// write given string to the LCD
void lcd_write_line(char string[]);

// wait until LCD finished loading
void busy(void);

// hold program for given iterations
void hold_program();

// when interrupt is on, go to this function
void __ISR(_TIMER_1_VECTOR, ipl2auto) Timer1SR(void);

// when interrupt is on, go to this function
void __ISR(_TIMER_3_VECTOR, ipl2auto) Timer3SR(void);

// when interrupt is on, go to this function
void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void);
