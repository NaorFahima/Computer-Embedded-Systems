#include "func.h"

void sys_config() {
    // LED configurations
    TRISA &= 0xff00;
    PORTA = 0x0; // lights off at start
    // audio configurations
    TRISBbits.TRISB14 = OFF;
    ANSELBbits.ANSB14 = OFF;
    // LCD configurations
    char control[] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1, 0x80};
    TRISE &= 0xff00;
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    PORTDbits.RD5 = 0; //w=0
    PORTBbits.RB15 = 0; //rs=0
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    ANSELEbits.ANSE7 = 0;
    ANSELBbits.ANSB15 = 0;
    lcd_init(control, 7);
    // switch configurations
    TRISFbits.TRISF3 = ON; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = ON; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = ON; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = ON; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = ON; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = ON; // RB11 (SW5) configured as input
    TRISBbits.TRISB10 = ON; // RB10 (SW6) configured as input
    TRISBbits.TRISB9 = ON; // RB9 (SW7) configured as input
    ANSELBbits.ANSB11 = OFF; // RB11 (SW5) disabled analog
    ANSELBbits.ANSB10 = OFF; // RB10 (SW6) disabled analog
    ANSELBbits.ANSB9 = OFF; // RB9 (SW7) disabled analog
}

int current_switch_state() {
    switches[0] = PORTFbits.RF3;
    switches[1] = PORTFbits.RF5;
    switches[2] = PORTFbits.RF4;
    switches[3] = PORTDbits.RD15;
    switches[4] = PORTDbits.RD14;
    switches[5] = PORTBbits.RB11;
    switches[6] = PORTBbits.RB10;
    switches[7] = PORTBbits.RB9;
    return get_next_switch(7);
}

int get_next_switch(int max_switch_on) {
    int j;
    for (j = max_switch_on; j >= 0; j--) {
        if (j != 3 && j != 4 && j != 6) {
            if (switches[j] == ON)
                return j;
        }
    }
    return NONE;
}

void mode_counter(int *current_mode) {
    char str[LCD_SCREEN_SIZE] = "Counter ";
    if (*current_mode != 0) {
        PORTA = 0x01;
    } else {
        if (switches[3]) { // if SW3 is on - change direction
            PORTA--;
            strcat(str, "Dec ");
        } else {
            PORTA++;
            strcat(str, "Inc ");
        }
        strcat(str, get_speed_string());
        if (!switches[6])
            lcd_set_text(MODE_0, str);
    }
    *current_mode = 0;
}

void mode_shift(int *current_mode) {
    char str[LCD_SCREEN_SIZE] = "Shift ";
    if (*current_mode != 1) {
        PORTA = 0x01;
    } else {
        if (switches[3]) { // if SW3 is on - shift right
            if ((PORTA & 0x00ff) <= 0x01) {
                PORTA = 0x80;
            } else {
                PORTA >>= 1;
            }
            strcat(str, "Right ");
        } else { // if SW3 is off - shift left
            if ((PORTA & 0x00ff) >= 0x80) {
                PORTA = 0x01;
            } else {
                PORTA <<= 1;
            }
            strcat(str, "Left  ");
        }
        strcat(str, get_speed_string());
        if (!switches[6])
            lcd_set_text(MODE_1, str);
    }
    *current_mode = 1;
}

void mode_fan(int *current_mode, int *fan_stage) {
    char str[LCD_SCREEN_SIZE] = "  Swing ";
    if (*current_mode != 2) {
        PORTA = 0x18;
    } else {
        switch (modulo(*fan_stage, 4)) {
            case 0:
                PORTA = 0x18;
                break;
            case 1:
                PORTA = 0x24;
                break;
            case 2:
                PORTA = 0x42;
                break;
            case 3:
                PORTA = 0x81;
                break;
            default:
                break;
        }
        if (switches[3]) { // if SW3 is on - change direction
            strcat(str, "In  ");
            (*fan_stage)--;
        } else {
            strcat(str, "Out ");
            (*fan_stage)++;
        }
        strcat(str, get_speed_string());
        if (!switches[6])
            lcd_set_text(MODE_2, str);
    }
    *current_mode = 2;
}

void mode_sound() {
    int i;
    PORTBbits.RB14 = ON;
    for (i = 0; i < 100; i++);
    PORTBbits.RB14 = OFF;
    for (i = 0; i < 1000; i++);
}

void hold_program() {
    int j;
    if (switches[4]) // SW4 is on -> short wait
        for (j = 0; j < FAST; j++);
    else // SW4 is off -> long wait
        for (j = 0; j < SLOW; j++);
}

int modulo(int a, int B) {
    return (a % B + B) % B;
}

char* get_speed_string() {
    if (switches[4])
        return "Fast";
    else
        return "Slow";
}

void lcd_init(char control[], int size) {
    int j;
    for (j = 0; j < size; j++) {
        PORTE = control[j];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
    lcd_clear();
}

void lcd_clear() {
    PORTE = 0x01; // set instruction 0x01
    PORTBbits.RB15 = 0; // rs = 0 -> instruction
    PORTDbits.RD5 = 0; // w = 0
    PORTDbits.RD4 = 1; // enable on
    PORTDbits.RD4 = 0; // enable off
    busy();
}

void lcd_set_text(char first_line[], char second_line[]) {
    busy();
    lcd_clear();
    lcd_set_cursor(LCD_FIRST_LINE);
    lcd_write_line(first_line);
    lcd_set_cursor(LCD_SECOND_LINE);
    lcd_write_line(second_line);
}

void lcd_write_line(char string[]) {
    int j;
    int length = strlen(string);
    PORTBbits.RB15 = 1; //rs = 1 -> data transfer
    PORTDbits.RD5 = 0; //w = 0
    for (j = 0; j < length; j++) {
        PORTE = string[j];
        PORTDbits.RD4 = 1;
        PORTDbits.RD4 = 0;
        busy();
    }
}

void lcd_set_cursor(int line) {
    PORTE = line;
    PORTBbits.RB15 = 0; // rs = 0 -> instruction
    PORTDbits.RD5 = 0; // w = 0
    PORTDbits.RD4 = 1;
    PORTDbits.RD4 = 0;
    busy();
}

void busy(void) {
    char RD, RS;
    int STATUS_TRISE;
    int portMap;
    RD = PORTDbits.RD5;
    RS = PORTBbits.RB15;
    STATUS_TRISE = TRISE;
    PORTDbits.RD5 = 1; //w/r
    PORTBbits.RB15 = 0; //rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do {
        PORTDbits.RD4 = 1; //enable=1
        PORTDbits.RD4 = 0; //enable=0
    } while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}