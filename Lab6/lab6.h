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
#define LCD_SECOND_LINE 0xc0
#define KEYBOARD_LENGTH 16

// pair of key code and matching ascii
char keyboard_values[KEYBOARD_LENGTH * 2] = {0x44, '1', 0x34, '2', 0x24, '3', 0x43, '4', 0x33, '5', 0x23, '6', 0x42, '7', 0x32, '8',
    0x22, '9', 0x41, '0', 0x14, 'A', 0x13, 'B', 0x12, 'C', 0x11, 'D', 0x21, 'E', 0x31, 'F'};

// system configurations (LED, switches, audio)
void sys_config();

// hold program for given iterations
void hold_program();

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

// his function returns the digital value corresponding to the analog pin, 
// as the result of analog to digital conversion performed by the ADC module. 
unsigned int ADC_AnalogRead(unsigned char analogPIN);

// convert the key code to ascii
char code_to_ascii(int key_code);

// hold program until key press is released
void wait_to_release_keyboard();

// check for keyboard input and return key code or NONE
int check_keyboard();

// check for a match in the y axis of keyboard
int in_y(int a);