#include "lab3.h"

void main() {
    char key_ascii; // current asccii value for pressed key
    int current_mode = NONE; // save current mode of operation
    int fan_stage = 0; // counter for fan mode stage (0-3)

    sys_config();

    while (1) {

        if (key_code != NONE) {
            key_ascii = code_to_ascii(key_code);
            set_flags(key_ascii);
        }
        if (flags[3]) { // halt flag
            lcd_set_text("      Halt      ", MODE_5);
            continue;
        }
        if (flags[2]) { // sound flag
            mode_sound();
            lcd_set_text("   Beep Sound   ", MODE_6);
        }

        switch (flag_mode) {
            case '0':
                mode_counter(&current_mode);
                break;
            case '1':
                mode_shift(&current_mode);
                break;
            case '2':
                mode_fan(&current_mode, &fan_stage);
                break;
            case '7':
                lcd_set_text("      Exit      ", MODE_7);
                PORTA = 0x0;
                return;
            default:
                if (flag_mode != NONE) {
                    mode_string[strlen(mode_string) - 1] = flag_mode;
                    lcd_set_text("Lab3:", mode_string);
                }
                break;
        }
        hold_program();
    }
}

void set_flags(char key_ascii) {
    if (key_ascii == '3') {
        if (flags[0] == OFF)
            flags[0] = _ON;
        else
            flags[0] = OFF;
    } else if (key_ascii == '4') {
        if (flags[1] == OFF)
            flags[1] = _ON;
        else
            flags[1] = OFF;
    } else if (key_ascii == '5') {
        if (flags[3] == OFF)
            flags[3] = _ON;
        else
            flags[3] = OFF;
    } else if (key_ascii == '6') {
        if (flags[2] == OFF)
            flags[2] = _ON;
        else
            flags[2] = OFF;
    } else {
        flag_mode = key_ascii;
    }
}

char code_to_ascii(int key_code) {
    int i;
    for (i = 0; i < KEYBOARD_LENGTH * 2; i += 2) {
        if (keyboard_values[i] == key_code)
            return keyboard_values[i + 1];
    }
}

void wait_to_release_keyboard() {
    PORTCbits.RC2 = OFF;
    PORTCbits.RC1 = OFF;
    PORTCbits.RC4 = OFF;
    PORTGbits.RG6 = OFF;
    while (!PORTCbits.RC3);
    while (!PORTGbits.RG7);
    while (!PORTGbits.RG8);
    while (!PORTGbits.RG9);
}

int check_keyboard() {
    int xy;
    PORTCbits.RC2 = _ON;
    PORTCbits.RC1 = _ON;
    PORTCbits.RC4 = _ON;
    PORTGbits.RG6 = _ON;
    PORTCbits.RC2 = OFF;
    xy = in_y(1);
    if (xy != NONE)
        return xy;
    PORTCbits.RC2 = _ON;
    PORTCbits.RC1 = OFF;
    xy = in_y(2);
    if (xy != NONE)
        return xy;
    PORTCbits.RC1 = _ON;
    PORTCbits.RC4 = OFF;
    xy = in_y(3);
    if (xy != NONE)
        return xy;
    PORTCbits.RC4 = _ON;
    PORTGbits.RG6 = OFF;
    xy = in_y(4);
    PORTGbits.RG6 = _ON;
    return xy;
}

int in_y(int a) {
    int j = 1, flag = 0;
    if (!PORTCbits.RC3) {
        flag = 1;
        j = 1;
    } else if (!PORTGbits.RG7) {
        flag = 1;
        j = 2;
    } else if (!PORTGbits.RG8) {
        flag = 1;
        j = 3;
    } else if (!PORTGbits.RG9) {
        flag = 1;
        j = 4;
    }
    if (flag == 0)
        return NONE;
    else
        return (j | (a << 4));
}

void sys_config() {
    // LED configurations
    TRISA &= 0xff00;
    PORTA = 0x0; // lights off at start

    // audio configurations
    TRISBbits.TRISB14 = OFF;
    ANSELBbits.ANSB14 = OFF;

    // keyboard configurations
    TRISCbits.TRISC2 = OFF; //RC2            
    TRISCbits.TRISC1 = OFF; //RC1           
    TRISCbits.TRISC4 = OFF; //RC4            
    TRISGbits.TRISG6 = OFF; //RG6
    TRISCbits.TRISC3 = _ON; //RC3
    TRISGbits.TRISG7 = _ON; //RG7
    TRISGbits.TRISG8 = _ON; //RG8
    TRISGbits.TRISG9 = _ON; //RG9
    ANSELGbits.ANSG6 = OFF;
    ANSELGbits.ANSG7 = OFF;
    ANSELGbits.ANSG8 = OFF;
    ANSELGbits.ANSG9 = OFF;
    CNPUCbits.CNPUC3;
    CNPUGbits.CNPUG7;
    CNPUGbits.CNPUG8;
    CNPUGbits.CNPUG9;

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
    lcd_init(control, 6);
    lcd_set_text("Lab3:", "*Gal Naor Natan*");

    // switch configurations
    TRISFbits.TRISF3 = _ON; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = _ON; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = _ON; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = _ON; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = _ON; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = _ON; // RB11 (SW5) configured as input
    TRISBbits.TRISB10 = _ON; // RB10 (SW6) configured as input
    TRISBbits.TRISB9 = _ON; // RB9 (SW7) configured as input
    ANSELBbits.ANSB11 = OFF; // RB11 (SW5) disabled analog
    ANSELBbits.ANSB10 = OFF; // RB10 (SW6) disabled analog
    ANSELBbits.ANSB9 = OFF; // RB9 (SW7) disabled analog

    // interrupt configuration
    PR4 = 0xffff; //             set period register, generates one interrupt every 1 ms
    TMR4 = 0; //             initialize count to 0
    T4CONbits.TCKPS0 = 1; //            1:256 prescale value
    T4CONbits.TCKPS1 = 1;
    T4CONbits.TCKPS2 = 1;
    T4CONbits.TGATE = 0; //             not gated input (the default)
    T4CONbits.TCS = 0; //             PCBLK input (the default)
    T4CONbits.ON = 1; //             turn on Timer1
    IPC4bits.T4IP = 2; //             priority
    IPC4bits.T4IS = 0; //             subpriority
    IFS0bits.T4IF = 0; //             clear interrupt flag
    IEC0bits.T4IE = 1;
    INTCONbits.MVEC = 1; //vector interrupt
    IPTMR = 0; //INTERRUPT PROXIMITY TIMER REGISTER
    asm("ei"); //on interrupt


}

void mode_counter(int *current_mode) {
    char str[LCD_SCREEN_SIZE] = "Counter ";
    if (*current_mode != 0) {
        PORTA = 0x01;
    } else {
        if (flags[0]) { // if SW3 is on - change direction
            PORTA--;
            strcat(str, "Dec ");
        } else {
            PORTA++;
            strcat(str, "Inc ");
        }
        strcat(str, get_speed_string());
        if (!flags[2])
            lcd_set_text(str, MODE_0);
    }
    *current_mode = 0;
}

void mode_shift(int *current_mode) {
    char str[LCD_SCREEN_SIZE] = "Shift ";
    if (*current_mode != 1) {
        PORTA = 0x01;
    } else {
        if (flags[0]) { // if SW3 is on - shift right
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
        if (!flags[2])
            lcd_set_text(str, MODE_1);
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
        if (flags[0]) { // if SW3 is on - change direction
            strcat(str, "In  ");
            (*fan_stage)--;
        } else {
            strcat(str, "Out ");
            (*fan_stage)++;
        }
        strcat(str, get_speed_string());
        if (!flags[2])
            lcd_set_text(str, MODE_2);
    }
    *current_mode = 2;
}

void mode_sound() {
    int j, i;
    for (i = 0; i < 100; i++) {
        PORTBbits.RB14 = _ON;
        for (j = 0; j < 700; j++);
        PORTBbits.RB14 = OFF;
    }
}

char* get_speed_string() {
    if (flags[1])
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
}

void lcd_clear() {
    PORTE = 0x01; // set instruction 0x01
    PORTBbits.RB15 = OFF; // rs = 0 -> instruction
    PORTDbits.RD5 = OFF; // w = 0
    PORTDbits.RD4 = _ON; // enable on
    PORTDbits.RD4 = OFF; // enable off
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
    PORTBbits.RB15 = _ON; //rs = 1 -> data transfer
    PORTDbits.RD5 = OFF; //w = 0
    for (j = 0; j < length; j++) {
        PORTE = string[j];
        PORTDbits.RD4 = _ON;
        PORTDbits.RD4 = OFF;
        busy();
    }
}

void lcd_set_cursor(int line) {
    PORTE = line;
    PORTBbits.RB15 = OFF; // rs = 0 -> instruction
    PORTDbits.RD5 = OFF; // w = 0
    PORTDbits.RD4 = _ON;
    PORTDbits.RD4 = OFF;
    busy();
}

void busy(void) {
    char RD, RS;
    int STATUS_TRISE;
    int portMap;
    RD = PORTDbits.RD5;
    RS = PORTBbits.RB15;
    STATUS_TRISE = TRISE;
    PORTDbits.RD5 = _ON; //w/r
    PORTBbits.RB15 = OFF; //rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do {
        PORTDbits.RD4 = _ON; //enable=1
        PORTDbits.RD4 = OFF; //enable=0
    } while (PORTEbits.RE7); // BF ?????
    PORTDbits.RD5 = RD;
    PORTBbits.RB15 = RS;
    TRISE = STATUS_TRISE;
}

void hold_program() {
    int j;
    if (flags[1]) // flag is on -> short wait
        for (j = 0; j < FAST; j++);
    else // SW4 is off -> long wait
        for (j = 0; j < SLOW; j++);
}

int modulo(int a, int B) {
    return (a % B + B) % B;
}

void __ISR(_TIMER_4_VECTOR, ipl2) Timer4SR(void) {
    key_code = check_keyboard();
    wait_to_release_keyboard();
    IFS0bits.T4IF = 0; // clean interrupt flag
}
