#pragma once


//instructiones used
#define BCF    0x9100  //1001 bbba ffff ffff
#define BSF    0x8100  //1000 bbba ffff ffff
#define CLRF   0x6B00  //0110 101a ffff ffff
#define BTFSC  0xB100  //1011 bbba ffff ffff 1011 0001 0000 0000
#define BTFSS  0xA100  //1010 bbba ffff ffff 1010 0001 0000 0000
#define BTG    0x7100  //0111 bbba ffff ffff 0111 0001 0000 0000
#define CALL   0xEC00  //1110 110s kkkk kkkk  1s w   1110 1100 0000 0000
             //0xF000  //1111 kkkk kkkk kkkk  2s w
#define GOTO   0xEF00  //1110 1111 kkkk kkkk  1s w
             //0xF000  //1111 kkkk kkkk kkkk  2s w
#define MOVFF  0xC000  //1100 ffff ffff ffff  1s w source 
             //0xF000  //1111 ffff ffff ffff  2s w distination
#define MOVLW  0x0E00  //0000 1110 kkkk kkkk
#define MOVWF  0x6E00  //0110 111a ffff ffff
#define RETURN 0x0012  //0000 0000 0001 001s
#define SETF   0x6800  //0110 100a ffff ffff

//the R1 register for processing the signal data   bits2 =( biti & bits1) | bits2 //
#define R1     0x1FD
#define biti   0
#define bits1  1
#define bits2  2 
#define bitand 3
//-----------------//

//----
#define valofst  0x17D
#define Rinpt    0x1FE   
#define Routp    0x1FF
//#define subpr    0x00009 //the offset adress of the subprogram
//#define prog     0x00012 //the offset adress of the main program
#define PORTA    0xF80   
#define PORTB    0xF81  
#define TRISA    0xF92  
#define TRISB    0XF93
#define ADCON1   0xFC1
#define BSR      0xE0
//----

//inputs 
#define E0  0
#define E1  1
#define E2  2
#define E3  3
#define E4  4
#define E5  5
#define E6  6
#define E7  7
//------//
//outputs 
#define A0  0
#define A1  1
#define A2  2
#define A3  3
#define A4  4
#define A5  5
#define A6  6
#define A7  7
//------//


//------banks-----// 
#define BANK0   0
#define BANK1   1
#define BANK2   2
#define BANK3   3
#define BANK4   4
#define BANK5   5
#define BANK6   6
#define BANK7   7
#define BANK8   8 
#define BANK9   9
#define BANK10  10 
#define BANK11  11
#define BANK12  12 
#define BANK13  13
#define BANK14  14
#define BANK15  15
//-----------------//

