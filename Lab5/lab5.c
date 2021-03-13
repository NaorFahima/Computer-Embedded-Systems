#include "lab5.h"

void main() {
    int current_digit = 0;

    sys_config();

    while (1) {
        if (PORTFbits.RF0) { // BTNC -> turn on 1 second timer and go infinite loop
            T4CONbits.ON = _ON;
            while (1);
        }
        if (PORTBbits.RB0) { // BTNL -> shift current_digit one to the left
            current_digit++;
        }
        if (PORTBbits.RB8) { // BTNR -> shift current_digit one to the right
            current_digit--;
        }
        if (PORTBbits.RB1) { // BTNU -> increase current_digit by 1
            switch (current_digit % 4) {
                case 0:
                    second++;
                    if (second == 10)
                        second = 0;
                    break;
                case 1:
                    tens_second++;
                    if (tens_second == 6)
                        tens_second = 0;
                    break;
                case 2:
                    minute++;
                    if (minute == 10)
                        minute = 0;
                    break;
                case 3:
                    tens_minute++;
                    if (tens_minute == 6)
                        tens_minute = 0;
                    break;
            }
        }
        if (PORTAbits.RA15) { // BTND -> decrease current_digit by 1
            switch (current_digit % 4) {
                case 0:
                    if (second == 0)
                        second = 9;
                    else
                        second--;
                    break;
                case 1:
                    if (tens_second == 0)
                        tens_second = 5;
                    else
                        tens_second--;
                    break;
                case 2:
                    if (minute == 0)
                        minute = 9;
                    else
                        minute--;
                    break;
                case 3:
                    if (tens_minute == 0)
                        tens_minute = 5;
                    else
                        tens_minute--;
                    break;
            }
        }
        hold_program();
        SSD_WriteDigits(second, tens_second, minute, tens_minute, 0, 0, 0, 0);
    }
}

void sys_config() {
    // 7Segment configuration
    SSD_Init();
    SSD_WriteDigits(0, 0, 0, 0, 0, 0, 0, 0);

    // Button configuration
    TRISBbits.TRISB1 = _ON; // RB1 (BTNU) configured as input
    TRISBbits.TRISB0 = _ON; // RB0 (BTNL) configured as input
    TRISFbits.TRISF0 = _ON; // RF0 (BTNC) configured as input
    TRISBbits.TRISB8 = _ON; // RB8 (BTNR) configured as input
    TRISAbits.TRISA15 = _ON; // RA15 (BTND) configured as input
    ANSELBbits.ANSB1 = OFF; // RB1 (BTNU) disabled analog
    ANSELBbits.ANSB0 = OFF; // RB0 (BTNL) disabled analog
    ANSELBbits.ANSB8 = OFF; // RB8 (BTNR) disabled analog

    // Timer 4 configuration
    PR4 = 0x7a12; // set period register interrupt each 100 ms
    TMR4 = 0; // initialize count to 0
    T4CONbits.TGATE = OFF; // not gated input (the default)
    T4CONbits.TCS = OFF; // PCBLK input (the default)
    T4CONbits.TCKPS0 = _ON; // 1:256 prescale value
    T4CONbits.TCKPS1 = _ON;
    T4CONbits.TCKPS2 = _ON;
    IPC4bits.T4IP = 2; // interrupt vector priority
    IPC4bits.T4IS = 0; // interrupt vector subpriority
    IEC0bits.T4IE = _ON; // timer enable
    IFS0bits.T4IF = OFF; // clear interrupt flag
    
    INTCONbits.MVEC = _ON; //vector interrupt
    IPTMR = OFF; //INTERRUPT PROXIMITY TIMER REGISTER
    asm("ei"); //on interrupt
}

void hold_program() {
    int j;
    for (j = 0; j < 200000; j++);
}

void __ISR(_TIMER_4_VECTOR, IPL2) Timer4SR(void) {
    if (counter_interrupt == 10) {
        second++;
        //update hour, minute and second
        if (second == 10) {
            tens_second++;
            second = 0;
        }
        if (tens_second == 6) {
            minute++;
            tens_second = 0;
        }
        if (minute == 10) {
            tens_minute++;
            minute = 0;
        }
        if (tens_minute == 6) {
            second = tens_second = minute = tens_minute = 0;
        }
        counter_interrupt = 0;
        SSD_WriteDigits(second, tens_second, minute, tens_minute, 0, 0, 0, 0);
    } else {
        counter_interrupt++;
    }
    IFS0bits.T4IF = OFF; // clean interrupt flag
}