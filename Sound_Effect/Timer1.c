#include <xc.h>
#define pre1 0b00
#define pre2 0b01
#define pre4 0b10
#define pre16 0b11
#define TMR1INITIAL 63036   // 10 ms

void TIMER1_Initialize(){
    T1CONbits.RD16 = 1;
    T1CONbits.T1CKPS = pre1;
    TMR1H = (TMR1INITIAL >> 8) & 0xFF;
    TMR1L = TMR1INITIAL & 0xFF;
    PIR1bits.TMR1IF = 0;
    IPR1bits.TMR1IP = 0;
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1;
}
