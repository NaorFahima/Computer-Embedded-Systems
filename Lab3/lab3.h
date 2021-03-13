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
#define FAST 64000
#define SLOW 164000
#define MODE_0 "Mode 0:"
#define MODE_1 "Mode 1:"
#define MODE_2 "Mode 2:"
#define MODE_5 "Mode 5:"
#define MODE_6 "Mode 6:"
#define MODE_7 "Mode 7:"
#define LCD_SCREEN_SIZE 16
#define LCD_FIRST_LINE 0x80
#define LCD_SECOND_LINE 0xc0
#define KEYBOARD_LENGTH 16

// pair of key code and matching ascii
char keyboard_values[KEYBOARD_LENGTH * 2] = {0x44, '1', 0x34, '2', 0x24, '3', 0x43, '4', 0x33, '5', 0x23, '6', 0x42, '7', 0x32, '8',
    0x22, '9', 0x41, '0', 0x14, 'A', 0x13, 'B', 0x12, 'C', 0x11, 'D', 0x21, 'E', 0x31, 'F'};

// current key code for pressed key
int key_code = NONE; 

int flags[4] = {OFF}; // flags array: 0 - direction, 1 - speed, 2 - halt, 3 - sound

char flag_mode = NONE; // flag for current operation mode 0-2

char mode_string[] = "Mode  "; // template for printing mode

// set the flags according to the current key pressed
void set_flags(char key_ascii);

// convert the key code to ascii
char code_to_ascii(int key_code);

// hold program until key press is released
void wait_to_release_keyboard();

// check for keyboard input and return key code or NONE
int check_keyboard();

// check for a match in the y axis of keyboard
int in_y(int a);

// system configurations (LED, switches, audio)
void sys_config();

// light in counter mode
void mode_counter(int *current_mode);

// lights in shift mode
void mode_shift(int *current_mode);

// lights in fan mode
void mode_fan(int *current_mode, int *fan_stage);

// perform sound noise
void mode_sound();

// return string representing speed based on switch 4
char* get_speed_string();

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

// mod operator. B is the positive part of the a mod B.
int modulo(int a, int B);

// when interrupt is on, go to this function
void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void); 
