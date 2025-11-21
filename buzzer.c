// PIC18F4520 Configuration Bit Settings
// 'C' source line config statements

#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled and controlled by software (SBOREN is enabled))
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

#include <xc.h>
#include <pic18f4520.h>
#define _XTAL_FREQ 4000000

int state = 0;
const unsigned char music_scale[] = {
    237, // Do
    211, // Re
    188, // Mi
    178, // Fa
    158, // So
    141, // La
    125,  // Si
    10
};

const int song[] = {4, 2, 2, 7, 3, 1, 1, 7, 0, 1, 2, 3, 4, 4, 4};

void __interrupt(high_priority) RB0_ISR(void){
    for (int i = 0; i < sizeof(song)/sizeof(song[0]); i++){
        // Set pitch
        PR2 = music_scale[song[i]];
        // The value of Duty cycle is half of PR2
        CCPR1L = music_scale[state] >> 1;
        /*
        CCPR1L = (music_scale[state] >> 1) >> 2; 
        CCP1CONbits.DC1B = (music_scale[state] >> 1) & 0b11
        */
        // Turn on Timer2, start to make sound
        T2CONbits.TMR2ON = 1; 
        __delay_ms(500);
        // Turn off Timer2, stop to make sound
        T2CONbits.TMR2ON = 0;
        __delay_ms(100);
    }
    INTCONbits.INT0IF = 0;
}

void main(void){
    // Timer2 -> off, prescaler -> 16
    T2CONbits.TMR2ON = 0b0;
    T2CONbits.T2CKPS = 0b11;
    
    // Internal Oscillator Frequency, Fosc = 4 MHz, Tosc = 0.25 µs
    OSCCONbits.IRCF = 0b110;
    // PWM mode, P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.CCP1M = 0b1100;
    // CCP1/RC2 -> Output
    TRISC = 0;
    LATC = 0;
    
    // RD2 interrupt
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    RCONbits.IPEN = 1;
    // Rising-edge triggered
    INTCON2bits.INTEDG0 = 1;
    
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    ADCON1bits.PCFG = 0x0F;      // all AN pin are digital input
    TRISBbits.RB0 = 0b1;
    
    while(1);
    return;
}

