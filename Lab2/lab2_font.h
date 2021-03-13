#pragma once

#include <xc.h>
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

#define ON 1
#define OFF 0
#define SPEED 262144
#define LCD_FIRST_LINE_START 0x80
#define LCD_FIRST_LINE_END 0x85
#define LCD_SECOND_LINE_START 0xc0
#define LCD_SECOND_LINE_END 0xc5
#define LCD_LINES_DIFFERENCE 0x40

int CG_gal[16] = {0x1f, 0x1, 0x1, 0x1, 0x3, 0x5, 0x9, 0x11,
    0x30, 0x30, 0x30, 0x3f, 0x21, 0x21, 0x22, 0x22}; // gal

int str_gal[2] = {1, 0};

int CG_naor[32] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x5f,
    0x09, 0x0a, 0x0c, 0x0c, 0x0c, 0x0a, 0x09, 0x09,
    0x86, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82,
    0xbf, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1}; // naor

int str_naor[4] = {5, 4, 3, 2};

int CG_natan[16] = {0xcf, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xc9, 0xd9,
    0xee, 0xee, 0xe6, 0xe6, 0xe6, 0xe6, 0xe6, 0xe6}; // natan

int str_natan[3] = {7, 6, 2};

// switches state (0 - off, 1 - on)
int switches[8];

// system configurations (LED, switches, audio)
void sys_config();

// get current switches state into switches array
void current_switch_state();

// perform sound noise
void mode_sound();

// hold program for SPEED iterations
void hold_program();

// configure LCD module with given controls
void lcd_init(char control[], int size);

// program new fonts to CGRAM
void lcd_add_cgram();

// add given font code array to CGRAM
void lcd_add_font(int font_code[], int arr_size);

// send instruction to clear LCD screen
void lcd_clear();

// print names to screen
void lcd_print_names();

// write given string to the LCD
void lcd_print_word(int string[], int size);

// print a " " character to screen
void lcd_print_space();

// set LCD cursor to given line
void lcd_set_cursor(int line);

// wait until LCD finished loading
void busy(void);