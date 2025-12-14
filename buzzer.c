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
#include "Timer1.h"

#define _XTAL_FREQ 1000000

#define STOP 0
#define DOH 1
#define REH 2
#define MIH 3
#define FAH 4
#define SOH 5
#define LAH 6
#define SIH 7
#define DOM 8
#define REM 9
#define MIM 10
#define FAM 11
#define SOM 12
#define LAM 13
#define SIM 14
#define DOL 15
#define REL 16
#define MIL 17
#define FAL 18
#define SOL 19
#define LAL 20
#define SIL 21

const unsigned char music_scale[] = {
    0,
    60,     // DoH
    53,     // ReH
    47,     // MiH
    45,     // FaH
    40,     // SoH
    36,     // LaH
    32,     // SiH
    118,    // DoM
    105,    // ReM
    94,     // MiM
    88,     // FaM
    79,     // SoM
    70,     // LaM
    62,     // SiM
    239,    // DoL
    213,    // ReL
    189,    // MiL
    178,    // FaL
    158,    // SoL
    141,    // LaL
    126     // SiL
};

#define CLOSE 0         // no sound
#define START 1
#define BACKGROUND 2    // always with me
#define EAT 3           // mario coin
#define WARNING 4       
#define WIN 5
#define LOSE 6

volatile unsigned int soundOp = 0;
const int bgm[];
volatile int bgmIndex;

void startMusic(){
    unsigned char notes[] = {15, 18, 23, 18, 11 }; 
    unsigned char k, i;
    for(i=0; i<5; i++) {
        k = notes[i];
        
        PR2 = k; CCPR1L = k >> 1;
        T2CONbits.TMR2ON = 1;
        if (i < 4) __delay_ms(150);
        else __delay_ms(250);
        
        T2CONbits.TMR2ON = 0;
        __delay_ms(50);
    }
    soundOp = 2;
}

// mario coin sound
void eatMusic(){
    OSCCONbits.IRCF = 0b110;
    
    // High pitch Si
    unsigned char sound;
    T2CONbits.TMR2ON = 1;
    for (sound = 120; sound <= 122; sound++){
        PR2 = sound; 
        CCPR1L = sound >> 1;
    }
    __delay_ms(90*4);

    // More higer pitch Do
    for (sound = 94; sound >= 92; sound--){
        PR2 = sound; // =~ 660Hz
        CCPR1L = sound >> 1;
    }
    __delay_ms(250*4);

    T2CONbits.TMR2ON = 0;
    OSCCONbits.IRCF = 0b100;
    soundOp = 2;
}

void warningMusic(){
    unsigned char sound, i, j;
    for (j = 0; j < 2; j++){
        for (i = 0; i < 4; i++){
            sound = 10;
            PR2 = sound;
            CCPR1L = sound >> 1;
            T2CONbits.TMR2ON = 1;
            __delay_ms(50);
            T2CONbits.TMR2ON = 0;
            __delay_ms(50);
        }
        if (j == 0) __delay_ms(500);
    }
    
    T2CONbits.TMR2ON = 0;
    soundOp = 2;
}

void winMusic(){
    unsigned char k, sound;
    sound = 29;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1;
    __delay_ms(500);       

    sound = 21;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1;  __delay_ms(200);
    T2CONbits.TMR2ON = 0;  __delay_ms(100);
    
    sound = 21;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1;  __delay_ms(80);
    T2CONbits.TMR2ON = 0;  __delay_ms(50);
    
    sound = 21;
    PR2 = sound; CCPR1L = sound >> 1;  
    T2CONbits.TMR2ON = 1; __delay_ms(450);
    T2CONbits.TMR2ON = 0; __delay_ms(300);
    //------------------------//
    __delay_ms(200);
    sound = 29;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1;  __delay_ms(500);       

    sound = 19;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1; __delay_ms(200);
    T2CONbits.TMR2ON = 0; __delay_ms(100);
    
    sound = 23;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1; __delay_ms(80);
    T2CONbits.TMR2ON = 0; __delay_ms(70);
    
    sound = 21;
    PR2 = sound;  CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1;  __delay_ms(450);
    T2CONbits.TMR2ON = 0;  __delay_ms(300);
    
    T2CONbits.TMR2ON = 0;
    soundOp = 0;
}

void loseMusic(){
    unsigned char sound = 200;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1; __delay_ms(100);
    
    T2CONbits.TMR2ON = 0;
    __delay_ms(150);
    
    sound = 180;
    PR2 = sound; CCPR1L = sound >> 1;
    T2CONbits.TMR2ON = 1; __delay_ms(1000);
    
    T2CONbits.TMR2ON = 0;
    soundOp = 0;
}

void __interrupt(high_priority) Master_ISR(void){
    if (INTCONbits.INT0IF){
        soundOp = 1;
        INTCONbits.INT0IF = 0;
    }else if (INTCON3bits.INT1IF){
        if (LATBbits.LATB4 == 0){
            soundOp = 3;
        }else{
            soundOp = 4;
        }
        INTCON3bits.INT1IF = 0;
    }else if (INTCON3bits.INT2IF){
        if (LATBbits.LATB5 == 0){
            soundOp = 5;
        }else{
            soundOp = 6;
        }
        INTCON3bits.INT2IF = 0;
    }
}

typedef struct {
    unsigned char pr2_val; // pitch (0: stop)
    unsigned char duration; // 20: 200ms
} Note;

// Under 1MHz, PR2: Do(118), Re(105), Mi(94), Fa(88), So(79)
const Note bgm_song[] = {
    {118, 200}, {105, 200}, {94, 200}, {118, 200}, // Do Re Mi Do
    {118, 1000}, {105, 1000}, {94, 200}, {118, 200}, // Do Re Mi Do
    {94, 200},  {88, 200},  {79, 200},            // Mi Fa So~
    {94, 200},  {88, 200},  {79, 200},            // Mi Fa So~
    {0, 0}                                     // stop symbol
};

volatile unsigned char noteIndex = 0;
volatile unsigned char noteTime = 0;

void __interrupt(low_priority) Timer1_ISR(void){
    if (PIR1bits.TMR1IF) {
        TMR1H = (63036 >> 8);
        TMR1L = (63036 & 0xFF);
        
        if (soundOp == 2) {
            if (noteTime == 0) {
                unsigned char p = bgm_song[noteIndex].pr2_val;
                unsigned char d = bgm_song[noteIndex].duration;
                
                if(d == 0) {
                    noteIndex = 0;
                    p = bgm_song[0].pr2_val;
                    d = bgm_song[0].duration;
                }
                
                if (p == 0) {
                    T2CONbits.TMR2ON = 0;
                }else {
                    PR2 = p;
                    CCPR1L = p / 8;
                    T2CONbits.TMR2ON = 1;
                }
                
                noteTime = d;
                noteIndex = noteIndex + 1;
            } else {
                noteTime = noteTime - 1;
            }
        }else {
            T1CONbits.TMR1ON = 1;
        }
        PIR1bits.TMR1IF = 0;
    }
}


void main(void){
    // Timer2 -> off, prescaler -> 16
    T2CONbits.TMR2ON = 0b0;
    T2CONbits.T2CKPS = 0b11;
    
    TIMER1_Initialize();
    
    // Internal Oscillator Frequency, Fosc = 1 MHz, Tosc = 1 us
    OSCCONbits.IRCF = 0b100;
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
    ADCON1bits.PCFG = 0b1111;      // all AN pin are digital input
    TRISBbits.RB0 = 0b1;
    
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    INTCON2bits.INTEDG1 = 1;
    TRISBbits.RB1 = 0b1;
    
    INTCON3bits.INT2IE = 1;
    INTCON3bits.INT2IF = 0;
    INTCON2bits.INTEDG2 = 1; 
    TRISBbits.RB2 = 0b1;
    
    /*
    TRISBbits.RB3 = 0b1;
    TRISBbits.RB4 = 0b1;
    TRISBbits.RB5 = 0b1;
    */
    
    /*// test
    startMusic();
    __delay_ms(2000);
    eatMusic();
    __delay_ms(2000);
    warningMusic();
    __delay_ms(2000);
    winMusic();
    __delay_ms(2000);
    loseMusic();
    __delay_ms(2000);
    */
    
    
    // test
    TRISBbits.RB3 = 0b0;
    TRISBbits.RB4 = 0b0;
    TRISBbits.RB5 = 0b0;
    LATBbits.LATB3 = 0b0;
    LATBbits.LATB4 = 0b0;
    LATBbits.LATB5 = 0b0;
    
    while(1){
        if (soundOp == 1){
            startMusic();
        }else if (soundOp == 3){
            eatMusic();
        }else if (soundOp == 4){
            warningMusic();
        }else if (soundOp == 5){
            winMusic();
        }else if (soundOp == 6){
            loseMusic();
        }
    }
    return;
}




/**
 const unsigned char notes[] = {19, 20, 24, 35, 37, 23, 18, 14 }; 
    
    T2CONbits.TMR2ON = 1;
    unsigned char k;
    for(k=0; k<8; k++) {
        PR2 = notes[k];
        CCPR1L = notes[k] >> 1;
        
        __delay_ms(100); 
    }
    
    T2CONbits.TMR2ON = 0;
 * 
 * */