#pragma once

#include <xc.h>
#include <string.h>
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
#define NONE -1
#define FAST 64000
#define SLOW 164000

#define LCD_SCREEN_SIZE 16
#define LCD_FIRST_LINE 0x80
#define LCD_SECOND_LINE 0xc0
#define MODE_0 "Mode 0:"
#define MODE_1 "Mode 1:"
#define MODE_2 "Mode 2:"
#define MODE_5 "Mode 5:"
#define MODE_6 "Mode 6:"
#define MODE_7 "Mode 7:"

int switches[8]; // switches state (0 - off, 1 - on)

// system configurations (LED, switches, audio)
void sys_config();

// get current switches state into switches array
int current_switch_state();

// check if lower then max_switch_on is on, return switch number
int get_next_switch(int max_switch_on);

// light in counter mode
void mode_counter(int *current_mode);

// lights in shift mode
void mode_shift(int *current_mode);

// lights in fan mode
void mode_fan(int *current_mode, int *fan_stage);

// perform sound noise
void mode_sound();

// hold program for given iterations
void hold_program();

// mod operator. B is the positive part of the a mod B.
int modulo(int a, int B);

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