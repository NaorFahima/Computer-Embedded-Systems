#include "lab6.h"

void main() {
    char key_ascii; // current asccii value for pressed key
    int prev_key = NONE;
    int key_code = NONE; // current key code for pressed key

    double voltage_div_value = 1023 / 3.3;
    int voltage = 0;
    char message[LCD_SCREEN_SIZE];
    sys_config();

    while (1) {
        key_code = check_keyboard();
        if (key_code == prev_key)
            continue;

        key_ascii = code_to_ascii(key_code);
        switch (key_ascii) {
            case '1':
                voltage = ADC_AnalogRead(2); //IN analog RB2
                sprintf(message, "Analog 1 - %.2fV", voltage / voltage_div_value);
                lcd_set_text("Lab6:", message);
                break;
            case '2':
                voltage = ADC_AnalogRead(4); //IN analog RB4
                sprintf(message, "Analog 2 - %.2fV", voltage / voltage_div_value);
                lcd_set_text("Lab6:", message);
                break;
        }
        hold_program();
    }
}

unsigned int ADC_AnalogRead(unsigned char analogPIN) {
    int adc_val = 0;

    IEC0bits.T2IE = 0;
    AD1CHS = analogPIN << 16; // AD1CHS<16:19> controls which analog pin goes to the ADC

    AD1CON1bits.SAMP = 1; // Begin sampling
    while (AD1CON1bits.SAMP); // wait until acquisition is done
    while (!AD1CON1bits.DONE); // wait until conversion is done

    adc_val = ADC1BUF0;
    IEC0bits.T2IE = 1;
    return adc_val;
}

void sys_config() {
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
    lcd_set_text("Lab6:", " ");
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
    // ADC configurations
    AD1CON1 = OFF;
    AD1CON1bits.SSRC = 7; // Internal counter ends sampling and starts conversion (auto convert)
    AD1CON1bits.FORM = 0; // Integer 16-bit
    // Setup for manual sampling
    AD1CSSL = 0;
    AD1CON3 = 0x0002; // ADC Conversion Clock Select bits: TAD = 6 TPB
    AD1CON2 = 0;
    AD1CON2bits.VCFG = OFF; // Voltage Reference Configuration bits: VREFH = AVDD and VREFL = AVSS
    // Turn on ADC
    AD1CON1bits.ON = _ON;

    // potentiometer configurations
    TRISBbits.TRISB2 = _ON;
    ANSELBbits.ANSB2 = _ON;

    // microphone configurations
    TRISBbits.TRISB4 = _ON;
    ANSELBbits.ANSB4 = _ON;
}

void hold_program() {
    int j;
    for (j = 0; j < 200000; j++);
}

void lcd_init(char control[], int size) {
    int j;
    for (j = 0; j < size; j++) {
        PORTE = control[j];
        PORTDbits.RD4 = _ON;
        PORTDbits.RD4 = OFF;
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