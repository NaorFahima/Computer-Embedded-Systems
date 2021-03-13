#include "lab4.h"

void main() {

    sys_config();
    while (1){};
}

void sys_config() {
    // RGB configurations
    TRISDbits.TRISD2 = OFF; // R LED8_R AN25/RPD2/RD2 output RED
    TRISDbits.TRISD12 = OFF; //G LED8_G RPD12/PMD12/RD12 output

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
    lcd_set_text("Lab4:", "00");

    // Timer 1 configuration
    PR1 = 0xc35; // set period register interrupt each 10 ms
    TMR1 = 0; // initialize count to 0
    T1CONbits.TGATE = OFF; // not gated input (the default)
    T1CONbits.TCS = OFF; // PCBLK input (the default
    T1CONbits.TCKPS0 = _ON; // 1:256 prescale value
    T1CONbits.TCKPS1 = _ON;
    T1CONbits.TSYNC = _ON;
    T1CONbits.ON = _ON; // turn on Timer1
    IPC1bits.T1IP = 2; // interrupt vector priority
    IPC1bits.T1IS = 2; // interrupt vector subpriority
    IEC0bits.T1IE = _ON;
    IFS0bits.T1IF = OFF; // clear interrupt flag

    // Timer 2-3 configuration
    PR2 = 0x8968; //PR2x interrupt each 2 second
    PR3 = 0x9; //PR2y
    TMR2 = 0; //TMRx
    TMR3 = 0; //TMRy
    T2CONbits.TGATE = OFF; // not gated input (the default)
    T2CONbits.TCS = OFF; // PCBLK input (the default)
    T2CONbits.TCKPS0 = _ON; // 1:256 prescale value
    T2CONbits.TCKPS1 = _ON;
    T2CONbits.TCKPS2 = _ON;
    T2CONbits.T32 = _ON; // mode 32bit
    T2CONbits.ON = _ON; // turn on Timer2/3
    IPC2bits.T2IP = 2; // interrupt vector priority
    IPC2bits.T2IS = 1; // interrupt vector subpriority
    IPC3bits.T3IP = 2; // interrupt vector priority
    IPC3bits.T3IS = 1; // interrupt vector subpriority
    IEC0bits.T2IE = _ON;
    IEC0bits.T3IE = _ON;
    IFS0bits.T3IF = OFF; // clear interrupt flag

    // Timer 4 configuration
    PR4 = 0x7a12; // set period register interrupt each 100 ms
    TMR4 = 0; // initialize count to 0
    T4CONbits.TGATE = OFF; // not gated input (the default)
    T4CONbits.TCS = OFF; // PCBLK input (the default)
    T4CONbits.TCKPS0 = _ON; // 1:256 prescale value
    T4CONbits.TCKPS1 = _ON;
    T4CONbits.TCKPS2 = _ON;
    T4CONbits.ON = _ON; // turn on Timer4
    IPC4bits.T4IP = 2; // interrupt vector priority
    IPC4bits.T4IS = 0; // interrupt vector subpriority
    IEC0bits.T4IE = _ON;
    IFS0bits.T4IF = OFF; // clear interrupt flag

    INTCONbits.MVEC = _ON; //vector interrupt
    IPTMR = OFF; //INTERRUPT PROXIMITY TIMER REGISTER
    asm("ei"); //on interrupt
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
    for (j = 0; j < 64000; j++);
}

void __ISR(_TIMER_1_VECTOR, ipl2) Timer1SR(void) {
    PORTDbits.RD2 = !PORTDbits.RD2;
    IFS0bits.T1IF = OFF; // clean interrupt flag
}

void __ISR(_TIMER_3_VECTOR, ipl2) Timer3SR(void) {
    PORTDbits.RD12 = !PORTDbits.RD12;
    IFS0bits.T3IF = OFF; // clean interrupt flag
}

void __ISR(_TIMER_4_VECTOR, ipl2) Timer4SR(void) {
    if (counter_interrupt == 10) {
        char str_int[3];
        itoa(str_int, counter_number, 10);
        if (strlen(str_int) == 1) {
            str_int[1] = str_int[0];
            str_int[0] = '0';
        }
        lcd_set_text("Lab4:", str_int);
        counter_number++;
        if (counter_number == 100)
            counter_number = 0;
        counter_interrupt = 0;
    } else {
        counter_interrupt++;
    }
    IFS0bits.T4IF = OFF; // clean interrupt flag
}