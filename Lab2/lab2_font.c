#include "lab2_font.h"

void main() {

    sys_config(); // configure the module
    lcd_add_cgram(); // program cgram with new fonts

    int line_end, line_start; // locations for line start/end
    int cursor_pos = LCD_FIRST_LINE_START; // cursor location

    while (1) {
        current_switch_state();

        if (switches[1]) {
            line_end = LCD_SECOND_LINE_END;
            line_start = LCD_SECOND_LINE_START;
            if (cursor_pos < LCD_SECOND_LINE_START)
                cursor_pos += LCD_LINES_DIFFERENCE;
        } else {
            line_end = LCD_FIRST_LINE_END;
            line_start = LCD_FIRST_LINE_START;
            if (cursor_pos >= LCD_SECOND_LINE_START)
                cursor_pos -= LCD_LINES_DIFFERENCE;
        }

        if (switches[0]) {
            if (cursor_pos < line_end)
                cursor_pos++;
        } else {
            if (cursor_pos > line_start)
                cursor_pos--;
        }

        if (switches[7])
            mode_sound();

        lcd_set_cursor(cursor_pos);
        lcd_print_names();
        hold_program();
    }
}

void sys_config() {
    // audio configurations
    TRISBbits.TRISB14 = OFF;
    ANSELBbits.ANSB14 = OFF;
    // LCD configurations
    char control[] = {0x38, 0x38, 0x38, 0xe, 0x6, 0x1};
    TRISE &= 0xff00;
    TRISDbits.TRISD4 = OFF; // RD4 (DISP_EN) set as an output
    TRISDbits.TRISD5 = OFF; // RD5 (DISP_RW) set as an output
    TRISBbits.TRISB15 = OFF; // RB15 (DISP_RS) set as an output
    PORTDbits.RD5 = OFF; //w=0
    PORTBbits.RB15 = OFF; //rs=0
    ANSELEbits.ANSE2 = OFF;
    ANSELEbits.ANSE4 = OFF;
    ANSELEbits.ANSE5 = OFF;
    ANSELEbits.ANSE6 = OFF;
    ANSELEbits.ANSE7 = OFF;
    ANSELBbits.ANSB15 = OFF;
    lcd_init(control, sizeof (control) / sizeof (control[0]));
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

void current_switch_state() {
    switches[0] = PORTFbits.RF3;
    switches[1] = PORTFbits.RF5;
    switches[2] = PORTFbits.RF4;
    switches[3] = PORTDbits.RD15;
    switches[4] = PORTDbits.RD14;
    switches[5] = PORTBbits.RB11;
    switches[6] = PORTBbits.RB10;
    switches[7] = PORTBbits.RB9;
}

void mode_sound() {
    int j, i;
    for (i = 0; i < 100; i++) {
        PORTBbits.RB14 = ON;
        for (j = 0; j < 500; j++);
        PORTBbits.RB14 = OFF;
    }
}

void hold_program() {
    int j;
    for (j = 0; j < SPEED; j++);
}

void lcd_init(char control[], int size) {
    int j;
    for (j = 0; j < size; j++) {
        PORTE = control[j];
        PORTDbits.RD4 = ON;
        PORTDbits.RD4 = OFF;
        busy();
    }
}

void lcd_add_cgram() {
    PORTE = 0x40; // set CGRAM=0x40
    PORTDbits.RD4 = ON;
    PORTDbits.RD4 = OFF;
    busy();

    PORTBbits.RB15 = ON; //rs
    lcd_add_font(CG_gal, sizeof (CG_gal) / sizeof (int));
    lcd_add_font(CG_naor, sizeof (CG_naor) / sizeof (int));
    lcd_add_font(CG_natan, sizeof (CG_natan) / sizeof (int));
}

void lcd_add_font(int font_code[], int arr_size) {
    int i;
    for (i = 0; i < arr_size; i++) {
        PORTE = font_code[i];
        PORTDbits.RD4 = ON; //enable=1
        PORTDbits.RD4 = OFF; //enable=0
        busy();
    }
}

void lcd_clear() {
    PORTE = 0x01; // set instruction 0x01
    PORTBbits.RB15 = OFF; // rs = 0 -> instruction
    PORTDbits.RD5 = OFF; // w = 0
    PORTDbits.RD4 = ON; // enable on
    PORTDbits.RD4 = OFF; // enable off
    busy();
}

void lcd_print_names() {
    lcd_print_word(str_gal, sizeof (str_gal) / sizeof (int));
    lcd_print_space();
    lcd_print_word(str_naor, sizeof (str_naor) / sizeof (int));
    lcd_print_space();
    lcd_print_word(str_natan, sizeof (str_natan) / sizeof (int));
}

void lcd_print_word(int string[], int size) {
    int j;
    PORTBbits.RB15 = ON; //rs = 1 -> data transfer
    PORTDbits.RD5 = OFF; //w = 0
    for (j = 0; j < size; j++) {
        PORTE = string[j];
        PORTDbits.RD4 = ON;
        PORTDbits.RD4 = OFF;
        busy();
    }
}

void lcd_print_space() {
    PORTBbits.RB15 = ON; //rs = 1 -> data transfer
    PORTDbits.RD5 = OFF; //w = 0
    PORTE = ' ';
    PORTDbits.RD4 = ON;
    PORTDbits.RD4 = OFF;
    busy();
}

void lcd_set_cursor(int line) {
    lcd_clear();
    PORTE = line;
    PORTBbits.RB15 = OFF; // rs = 0 -> instruction
    PORTDbits.RD5 = OFF; // w = 0
    PORTDbits.RD4 = ON;
    PORTDbits.RD4 = OFF;
    busy();
}

void busy(void) {
    char RD, RS;
    int STATUS_TRISE;
    RD = PORTDbits.RD5;
    RS = PORTBbits.RB15;
    STATUS_TRISE = TRISE;
    PORTDbits.RD5 = ON; //w/r
    PORTBbits.RB15 = OFF; //rs 
    TRISE |= 0x80;
    do {
        PORTDbits.RD4 = ON; //enable=1
        PORTDbits.RD4 = OFF; //enable=0
    } while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}