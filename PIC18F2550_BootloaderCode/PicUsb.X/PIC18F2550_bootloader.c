/***********************************************************
   PIC18F2550 bootloader
   by : Assaous oussama
   description : this code is a part of the PIC18F2550 PLC
 ***********************************************************/

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 2       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes from the 96 MHz PLL divided by 2)

// CONFIG1H
#pragma config FOSC = XT_XT     // Oscillator Selection bits (XT oscillator (XT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = ON      // USB Voltage Regulator Enable bit (USB voltage regulator enabled)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

            // 08     09     0A      0B     0C    0D     0E     0F 
__EEPROM_DATA(0x12 , 0x01 , 0x00 , 0x02 , 0x02 , 0x00 , 0x00 , 0x08); 
            // 10     11     12      13     14    15     16     17
__EEPROM_DATA(0xD8 , 0x04 , 0x0A , 0x00 , 0x00 , 0x00 , 0x01 , 0x02);  
            // 18     19     1A      1B     1C    1D     1E     1F
__EEPROM_DATA(0x00 , 0x01 , 0x09 , 0x02 , 0x43 , 0x00 , 0x02 , 0x01);  
            // 20     21     22      23     24    25     26     27
__EEPROM_DATA(0x00 , 0x80 , 0x32 , 0x09 , 0x04 , 0x00 , 0x00 , 0x01);  
            // 28     29     2A      2B     2C    2D     2E     2F
__EEPROM_DATA(0x02 , 0x02 , 0x01 , 0x00 , 0x05 , 0x24 , 0x00 , 0x10);  
            // 30     31     32      33     34    35     36     37
__EEPROM_DATA(0x01 , 0x04 , 0x24 , 0x02 , 0x02 , 0x05 , 0x24 , 0x06);  
            // 38     39     3A      3B     3C    3D     3E     3F 
__EEPROM_DATA(0x00 , 0x01 , 0x05 , 0x24 , 0x01 , 0x00 , 0x01 , 0x07);  
            // 40     41     42      43     44    45     46     47
__EEPROM_DATA(0x05 , 0x82 , 0x03 , 0x08 , 0x00 , 0x02 , 0x09 , 0x04);  
            // 48     49     4A      4B     4C    4D     4E     4F 
__EEPROM_DATA(0x01 , 0x00 , 0x02 , 0x0A , 0x00 , 0x00 , 0x00 , 0x07);  
            // 50     51     52      53     54    55     56     57
__EEPROM_DATA(0x05 , 0x00 , 0x02 , 0x40 , 0x00 , 0x00 , 0x07 , 0x05);//__EEPROM_DATA(0x05 , 0x03 , 0x02 , 0x40 , 0x00 , 0x00 , 0x07 , 0x05);  
            // 58     59     5A      5B     5C    5D     5E     5F
__EEPROM_DATA(0x80 , 0x02 , 0x40 , 0x00 , 0x00 , 0x04 , 0x03 , 0x09);//__EEPROM_DATA(0x83 , 0x02 , 0x40 , 0x00 , 0x00 , 0x04 , 0x03 , 0x09);
            // 60     61     62      63     64    65     66     67
__EEPROM_DATA(0x04 , 0x34 , 0x03 , 0x41 , 0x00 , 0x73 , 0x00 , 0x73);//__EEPROM_DATA(0x04 , 0x34 , 0x03 , 0x4D , 0x00 , 0x69 , 0x00 , 0x63);
            // 68     69     6A      6B     6C    6D     6E     6F
__EEPROM_DATA(0x00 , 0x61 , 0x00 , 0x6f , 0x00 , 0x75 , 0x00 , 0x73); //__EEPROM_DATA(0x00 , 0x72 , 0x00 , 0x0F , 0x00 , 0x63 , 0x00 , 0x68);  
            // 70     71     72      73     74    75     76     77
__EEPROM_DATA(0x00 , 0x20 , 0x00 , 0x6f , 0x00 , 0x75 , 0x00 , 0x73); //__EEPROM_DATA(0x00 , 0x69 , 0x00 , 0x70 , 0x00 , 0x20 , 0x00 , 0x54);  
            // 78     79     7A      7B     7C    7D     7E     7F
__EEPROM_DATA(0x00 , 0x73 , 0x00 , 0x61 , 0x00 , 0x6d , 0x00 , 0x61);//__EEPROM_DATA(0x00 , 0x65 , 0x00 , 0x63 , 0x00 , 0x68 , 0x00 , 0x6E);  
            // 80     81     82      83     84    85     86     87
__EEPROM_DATA(0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00); //__EEPROM_DATA(0x00 , 0x6F , 0x00 , 0x6C , 0x00 , 0x6F , 0x00 , 0x67);  
            // 88     89     8A      8B     8C    8D     8E     8F
__EEPROM_DATA(0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00); //__EEPROM_DATA(0x00 , 0x79 , 0x00 , 0x20 , 0x00 , 0x49 , 0x00 , 0x6E); 
            // 90     91     92      93     94    95     96     97
__EEPROM_DATA(0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x34 , 0x03 , 0x50); //__EEPROM_DATA(0x00 , 0x63 , 0x00 , 0x2E , 0x00 , 0x34 , 0x03 , 0x43); 
            // 98     99     9A      9B     9C    9D     9E     9F
__EEPROM_DATA(0x00 , 0x4c , 0x00 , 0x43 , 0x00 , 0x20 , 0x00 , 0x62); //__EEPROM_DATA(0x00 , 0x44 , 0x00 , 0x43 , 0x00 , 0x20 , 0x00 , 0x52);
            // A0     A1     A2      A3     A4    A5     A6     A7
__EEPROM_DATA(0x00 , 0x79 , 0x00 , 0x20 , 0x00 , 0x41 , 0x00 , 0x73); //__EEPROM_DATA(0x00 , 0x53 , 0x00 , 0x2D , 0x00 , 0x32 , 0x00 , 0x33);
            // A8     A9     AA      AB     AC    AD     AE     AF
__EEPROM_DATA(0x00 , 0x73 , 0x00 , 0x61 , 0x00 , 0x6f , 0x00 , 0x75); //__EEPROM_DATA(0x00 , 0x32 , 0x00 , 0x20 , 0x00 , 0x45 , 0x00 , 0x6D);
            // B0     B1     B2      B3     B4    B5     B6     B7
__EEPROM_DATA(0x00 , 0x73 , 0x00 , 0x20 , 0x00 , 0x4f , 0x00 , 0x75); //__EEPROM_DATA(0x00 , 0x75 , 0x00 , 0x6C , 0x00 , 0x61 , 0x00 , 0x74);
            // B8     B9     BA      BB     BC    BD     BE     BF
__EEPROM_DATA(0x00 , 0x73 , 0x00 , 0x73 , 0x00 , 0x61 , 0x00 , 0x6d); //__EEPROM_DATA(0x00 , 0x69 , 0x00 , 0x6F , 0x00 , 0x6E , 0x00 , 0x20);
            // C0     C1     C2      C3     C4    C5     C6     C7
__EEPROM_DATA(0x00 , 0x61 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00); //__EEPROM_DATA(0x00 , 0x44 , 0x00 , 0x65 , 0x00 , 0x6D , 0x00 , 0x6F);
            // C8     C9     CA      CB     CC    CD     CE     CF
__EEPROM_DATA(0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00); 

//bRequest types
#define GET_STATUS        0x00
#define CLEAR_FEATURE     0x01
//#define Reserved  0x02 //for future use
#define SET_FEATURE       0x03
//#define Reserved 0x04 //for future use
#define SET_ADDRESS       0x05
#define GET_DESCRIPTOR    0x06
#define SET_DESCRIPTOR    0x07
#define GET_CONFIGURATION 0x08
#define SET_CONFIGURATION 0x09
#define GET_INTERFACE     0x0A
#define SET_INTERFACE     0x0B
#define SYNCH_FRAME       0x0C


//Descriptor Types
#define DEVICE                    0x01
#define CONFIGURATION             0x02
#define STRING                    0x03
#define INTERFACE                 0x04
#define ENDPOINT                  0x05
#define DEVICE_QUALIFIER          0x06
#define OTHER_SPEED_CONFIGURATION 0x07
#define INTERFACE_POWER           0x08



char BDOSTATOUT    __at(0x0400);
char BDOCNTOUT     __at(0x0401);
char BDOADRLOUT    __at(0x0402);
char BDOADRHOUT    __at(0x0403);
 
char bmRequestType __at(0x0500);
char bRequest      __at(0x0501);
char wValue[2]     __at(0x0502);
char wIndex[2]     __at(0x0504);
//char wLength[2]    __at(0x0506);
int wLength    __at(0x0506);
  

char BDOSTATIN     __at(0x0404);
char BDOCNTIN      __at(0x0405);
char BDOADRLIN     __at(0x0406);
char BDOADRHIN     __at(0x0407);

char IN0DATAOF [8] __at(0x0508);
char switsh [100]    __at(0x0500);
char bwitsh [100]    __at(0x0540);
 

int cp = 0;
char i;
int progcount=0;
char Request;
char wLengthDev = 0x00;
int DeviceCun ;
char DeviceCun1 = 0x08;

int ConfigCun;
char ConfigCun1 = 0x1A;


char stringcun ;

char stringval;
int INnum ;
char TokenId;



int DevMax = 18;
int ConfMax = 67;
int stringMax1 = 4;
int stringMax2 = 52;
int stringMax3 = 52;


char send[100]     __at(0x0508);
char send1    __at(0x0509);
char send2    __at(0x050A);
char send3    __at(0x050B);
char send4    __at(0x050C);
char send5    __at(0x050D);
char send6    __at(0x050E);
char send7    __at(0x050F);
char DevBiCun;

int DevByte;

int f = 0;



char EpromRead(char);
int resetdiv(void);
int reqhand();

void program();

void outset(int);
void inset(int,int);

int main()

{
    
    
 //UCON = 0x00;
    for(i =0 ;i<192;i++)
    {
      IN0DATAOF [i]  = EpromRead(i);  
    }
    TRISB =0x00;
    TRISC =0xff;
    PORTB = 0x00;
  
    UIE = 0x00;
    UIR = 0x00;
    
    UEP0 = 0x16;
    
    UCFG = 0x14;//UCFG = 0x10;
 //   if(RC0)
//    {
     UCON = 0x08;
     
   // }
    // RB0=1;
  //  while(SE0) {}
    
    while(1)//while(RC0)
    {
       
        if(UERRIF)   UEIR = 0x00;
        else if(SOFIF) SOFIF = 0;
             else if(IDLEIF) { IDLEIF = 0; SUSPND = 1;}
                  else if(ACTVIF)  {ACTVIF =0; SUSPND =0;}
                       else if(STALLIF) STALLIF = 0;
                            else if(URSTIF) { 
                                             resetdiv();                                          
                                            }
                                        else if(TRNIF) 
                                        {
                                            if(DIR)
                                            {//The last transaction was an IN token
                                                TRNIF = 0 ;
                                                switch(Request)
                                                {
                                                    case DEVICE:
                                                                {      
                                                                     reqhand();                                 
                                                                }
                                                    break;
                                                    case CONFIGURATION:
                                                                 {
                                                                     reqhand();                                                                    
                                                                 }
                                                    break;
                                                    case STRING:
                                                                {
                                                                     switch(stringval)
                                                                     {
                                                                         case 0x00:
                                                                         {
                                                                             outset(0x08);
                                                                       /*  BDOCNTOUT = 0x08;
                                                                         BDOADRLOUT = 0x00;
                                                                         BDOADRHOUT =0x05;
                                             
                                                                         BDOSTATOUT =0x80;  */
                                                                         }
                                                                         break;
                                                                         case 0x01:
                                                                         {
                                                                             reqhand();                                                                    
                                                                         }
                                                                         break;
                                                                         case 0x02:
                                                                         {                                                                             
                                                                             reqhand();                                                                                                                                                                                                                                   
                                                                         }
                                                                         break;
                                                                             
                                                                     }
                                                                }
                                                    break;
                                                    //case INTERFACE:
                                                    //break;
                                                    //case ENDPOINT:
                                                    //break;
                                                    //case DEVICE_QUALIFIER:
                                                    //break;
                                                    //case OTHER_SPEED_CONFIGURATION:
                                                    //break;
                                                    //case INTERFACE_POWER:
                                                    //break;
                                                }
                                             
                                            }
                                            else
                                            {//The last transaction was an OUT or SETUP token
                                                RB0 = 1 ;
                                                TRNIF = 0 ;
                                                PKTDIS = 0;
                                                
                                                TokenId = BDOSTATOUT & 0x3c;
                                                
                                                if(TokenId == 0x34)
                                                {   
                                                  switch(bRequest)
                                                  {
                                                        case GET_STATUS:
                                                        {
                                                            outset(0x08);
                                                       /* BDOCNTOUT = 0x08;
                                                        BDOADRLOUT = 0x00;
                                                        BDOADRHOUT =0x05;
                                             
                                                        BDOSTATOUT =0x80;*/
                                                        }
                                                        break;
                                                        //case CLEAR_FEATURE:
                                                        //break;
                                                        //case SET_FEATURE:
                                                        //break;
                                                        case SET_ADDRESS:
                                                        {
                                                            inset(0x00,0x08);
                                                           /* BDOCNTIN  = 0x00 ;
                                                            BDOADRLIN = 0x08 ;
                                                            BDOADRHIN = 0x05 ;
                                                                      
                                                            BDOSTATIN = 0x80 ;*/
                                                        }
                                                        break;
                                                        case GET_DESCRIPTOR:
                                                         {
                                                            switch(wValue[1])
                                                             {
                                                                 case DEVICE:
                                                                 {     
                                                                     
                                                                      Request = DEVICE ;
                                                                      
                                                                     DeviceCun = wLength;
                                                                     DeviceCun1 = 0x08;
                                                                     if(DeviceCun >=  DevMax )
                                                                     {
                                                                       DevByte = DevMax;  
                                                                     }
                                                                     else { DevByte = DeviceCun ;}
                                                                                                                                        
                                                                     inset(0x08,0x08);
                                                                    /*  BDOCNTIN  = 0x08 ;
                                                                      BDOADRLIN = 0x08 ;
                                                                      BDOADRHIN = 0x05 ;
                                                                      
                                                                      BDOSTATIN = 0x80 ;*/
                                                                                                                                                                                                             
                                                                 }
                                                                 break;
                                                                 case CONFIGURATION:
                                                                 {
                                                                     Request = CONFIGURATION ;
                                                                     DeviceCun1 = 0x1A;
                                                                    DeviceCun = wLength;
                                                                     
                                                                     if(DeviceCun >=  ConfMax )
                                                                     {
                                                                       DevByte = ConfMax;  
                                                                     }
                                                                     else { DevByte = DeviceCun ;}
                                                                     inset(0x08,0x1A);
                                                                     /* BDOCNTIN  = 0x08 ;
                                                                      BDOADRLIN = 0x1A ;
                                                                      BDOADRHIN = 0x05 ;
                                                                      
                                                                      BDOSTATIN = 0x80 ;*/
                                                                 }
                                                                 break;
                                                                 case STRING:
                                                                 {
                                                                    Request = STRING ;
                                                                     
                                                                     //ConfigCun = wLength;
                                                                     stringval = wValue[0];
                                                                     switch(wValue[0])
                                                                     {
                                                                         case 0x00:
                                                                         {  
                                                                             DeviceCun1 = 0x5D;
                                                                             DeviceCun = wLength;
                                                                     
                                                                            if(DeviceCun >=  stringMax1 )
                                                                              {
                                                                               DevByte = stringMax1;  
                                                                              }
                                                                              else { DevByte = DeviceCun ;}
                                                                             
                                                                           //  stringcun = 0x5D ;
                                                                             inset(0x04,0x5D);
                                                                         /*  BDOCNTIN  = 0x04 ;
                                                                           BDOADRLIN = 0x5D ;
                                                                           BDOADRHIN = 0x05 ;
                                                                        
                                                                           BDOSTATIN = 0x80 ;  */
                                                                         }
                                                                         break;
                                                                         case 0x01:
                                                                         {
                                                                              DeviceCun1 = 0x61;
                                                                             DeviceCun = wLength;
                                                                     
                                                                            if(DeviceCun >=  stringMax2 )
                                                                              {
                                                                               DevByte = stringMax2;  
                                                                              }
                                                                              else { DevByte = DeviceCun ;}
                                                                            // stringcun = 0x61 ;
                                                                             inset(0x08,0x61);
                                                                           /*  BDOCNTIN  = 0x08 ;
                                                                             BDOADRLIN = 0x61 ;
                                                                             BDOADRHIN = 0x05 ;
                                                                      
                                                                             BDOSTATIN = 0x80 ;  */
                                                                         }
                                                                         break;
                                                                         case 0x02:
                                                                         {
                                                                              DeviceCun1 = 0x95;
                                                                             DeviceCun = wLength;
                                                                     
                                                                            if(DeviceCun >=  stringMax3 )
                                                                              {
                                                                               DevByte = stringMax3;  
                                                                              }
                                                                              else { DevByte = DeviceCun ;}
                                                                            // stringcun = 0x95 ;
                                                                             inset(0x08,0x95);
                                                                           /*  BDOCNTIN  = 0x08 ;
                                                                             BDOADRLIN = 0x95 ;
                                                                             BDOADRHIN = 0x05 ;
                                                                      
                                                                             BDOSTATIN = 0x80 ; */
                                                                         }
                                                                         break;
                                                                         
                                                                     }
                                                                      
                                                                 }
                                                                 break;
                                                                 //case INTERFACE:
                                                                 //break;
                                                                 //case ENDPOINT:
                                                                 //break;
                                                                 //case DEVICE_QUALIFIER:
                                                                 //break;
                                                                 //case OTHER_SPEED_CONFIGURATION:
                                                                 //break;
                                                                 //case INTERFACE_POWER:
                                                                 //break;
                                                             }
                                                         }
                                                        break;
                                                       /* case SET_DESCRIPTOR:
                                                         { 
                                                             switch(wValue[0])
                                                             {
                                                                 //case DEVICE:
                                                                 //break;
                                                                 //case CONFIGURATION:
                                                                 //break;
                                                                 //case STRING:
                                                                 //break;
                                                                 //case INTERFACE:
                                                                 //break;
                                                                 //case ENDPOINT:
                                                                 //break;
                                                                 //case DEVICE_QUALIFIER:
                                                                 //break;
                                                                 //case OTHER_SPEED_CONFIGURATION:
                                                                 //break;
                                                                 //case INTERFACE_POWER:
                                                                 //break;
                                                             }
                                                            
                                                         }
                                                        break; */
                                                        //case GET_CONFIGURATION:
                                                        //break;
                                                        case SET_CONFIGURATION:
                                                        {
                                                            inset(0x00,0x08);
                                                           /* BDOCNTIN  = 0x00 ;
                                                            BDOADRLIN = 0x08 ;
                                                            BDOADRHIN = 0x05 ;
                                                                      
                                                            BDOSTATIN = 0x80 ;*/
                                                        }
                                                        break;
                                                        //case GET_INTERFACE:
                                                        //break;
                                                        case SET_INTERFACE:
                                                        {
                                                            inset(0x00,0x08);
                                                           /* BDOCNTIN  = 0x00 ;
                                                            BDOADRLIN = 0x08 ;
                                                            BDOADRHIN = 0x05 ;
                                                                      
                                                            BDOSTATIN = 0x80 ;*/
                                                        }
                                                        break;
                                                        //case SYNCH_FRAME:
                                                        //break;
                                                        
                                                 }
                                            }else if(TokenId == 0x04)
                                            {  
                                               
                                                
                                                if(switsh [0] == 'O' && switsh [1] == 'N') //(bmRequestType == 0x61) 
                                                  {
                                                                                                                                                    
                                                   /*  TBLPTR = 0x7FE0;//0x000A;
                                                    for(int rt=0 ;rt < 32 ; rt++)
                                                     {
                                                      __asm("TBLRD*+");
                                                      send[rt] =  TABLAT;
                                                     }
                                                                                                                                                         
                                                    
                                                     inset(0x20,0x08); */
                                                   /* BDOCNTIN  = 0x20;//0x10; //0x08 ;
                                                    BDOADRLIN = 0x08 ;
                                                    BDOADRHIN = 0x05 ;
                                                                      
                                                    BDOSTATIN = 0x80;*/
                                                                                                 
                                                  }
                                                    else if(switsh [0] == 'O' && switsh [1] == 'F')//bmRequestType == 0x62 
                                                    {RB0= 0;
                                                    
                                                    
                                                    switsh [0] = 0xFF;  
                                                    switsh [1] = 0xFF;  
                                                    switsh [2] = 0xFF;  
                                                    switsh [3] = 0xFF;
                                                              
                                                    outset(0x08);
                                                      /* BDOCNTOUT = 0x08;//0x08;
                                                       BDOADRLOUT = 0x00;
                                                       BDOADRHOUT =0x05;
                                             
                                                       BDOSTATOUT =0x80; */ 
                                                       UCON = 0x00;
                                                
                                                    __asm("GOTO 0x7FFC");
                                                    }else if(switsh [0] == 'P' && switsh [1] == 'R' && switsh [2] == 'O' && switsh [3] == 'G')
                                                           {                                                                                                                                                                                                     
                                                             program();                                                                                                                                                                                                                     
                                                           }
                                                    else if(switsh [0] == 'E' && switsh [1] == 'R' && switsh [2] == 'E' && switsh [3] == 'S')
                                                    {
                                                        TBLPTR = 0x800;
                                                        
                                                       // TBLPTR = 0x7fc0;
                                                        while (TBLPTR < 0x7FFF)
                                                        {
                                                        EEPGD = 1;
                                                        CFGS = 0;
                                                        WREN = 1;
                                                        FREE = 1 ;
                                                        GIE = 0;
                                                        EECON2 = 0x55;
                                                        EECON2 = 0x0AA;
                                                        WR = 1;
                                                        GIE = 1; 
                                                        FREE = 0 ;
                                                        TBLPTR = TBLPTR + 0x40 ;
                                                        }
                                                        
                                                    }
                                                
                                                outset(0xc8);
                                                      /* BDOCNTOUT = 0xC8;//0x64;//0x08;
                                                       BDOADRLOUT = 0x00;
                                                       BDOADRHOUT =0x05;
                                             
                                                       BDOSTATOUT =0x80; */
                                                    
                                            }
                                                                                             
                                            }   
                                        }
                                                                                                                                                      
        
    }
    
   // __asm("GOTO 0x4");
     UCON = 0x00;
    __asm("GOTO 0x7FFC");
     
}

///********************************************///
///     FUNCTION : char EpromRead(char addres) ///
///     OUTPUT : read from the EEPROM          ///
///********************************************///
char EpromRead(char addres)
{
    EEADR = addres;
    EEPGD = 0;
    CFGS = 0;
    RD = 1;
   return EEDATA ;
    
}

///******************************************///
///        FUNCTION : void resetdiv()        ///
///        OUTPUT : reset the USB interface  ///
///******************************************///
int resetdiv()
{    
     URSTIF = 0; 
     TRNIF = 0; 
     TRNIF = 0;
     TRNIF = 0; 
     TRNIF = 0;
     UEP0 = 0x00;
     UEP1 = 0x00;
     UEP2 = 0x00;
     UEP3 = 0x00;
     UEP4 = 0x00;
     UEP5 = 0x00;
     UEP6 = 0x00;
     UEP7 = 0x00;
     UEP8 = 0x00;
     UEP9 = 0x00;
     UEP10 = 0x00;
     UEP11 = 0x00;
     UEP12 = 0x00; 
     UEP13 = 0x00;
     UEP14 = 0x00;
     UEP15 = 0x00;
                                             
     UEP0 = 0x16;
     DeviceCun1 = 0x08;
     ConfigCun1 = 0x1A;
     BDOCNTOUT = 0x08;
     BDOADRLOUT = 0x00;
     BDOADRHOUT =0x05;
                                             
     BDOSTATOUT =0x80;
     return 0;
}
///******************************************///

///*********************************************///
///          FUNCTION : int reqhand()           ///
///         OUTPUT : handl the request          ///
///*********************************************///
int reqhand()
{
    DevByte -= 8;
    if(DevByte > 0)
    {   
        DeviceCun1 += 0x08;
        if(DevByte >= 8)  {DevBiCun = 0x08;}
        else {  DevBiCun = DevByte ; }
        
        inset(DevBiCun,DeviceCun1);
       /* BDOCNTIN  = DevBiCun ;
        BDOADRLIN = DeviceCun1 ;
        BDOADRHIN = 0x05 ;
        BDOSTATIN = 0x80 ; */
        
    }
    else
    {
       outset(0x08);
       /* BDOCNTOUT = 0x08;
        BDOADRLOUT = 0x00;
        BDOADRHOUT =0x05;
        
        BDOSTATOUT =0x80;*/
        
    }
    return 0;
    
}
///*********************************************///


///************************************************///
///        FUNCTION : void program()               ///
///       OUTPUT : read data from the USB ram then ///
///  program it to the flash memory program        ///
///                                                ///
///************************************************///
void  program()
{
    int op=0;                                                                                                     
                  
    cp = 6;
    //TBLPTR = 0x00;
    
    TBLPTRL = switsh [5];
    TBLPTRH = switsh [4];
    TBLPTRU = 0x00;
     //------------------------1------------------------------
    for(op=0 ; op<31; op++)
    {
                                                      
     TABLAT = switsh [cp];
     __asm("TBLWT*+");
       cp++;
    }         
    TABLAT = switsh [cp];
     __asm("TBLWT");                                                                                                                                                       
    cp++;
    EEPGD = 1;
    CFGS = 0;
    WREN = 1 ;
    GIE = 0 ;
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    WR = 1;
    GIE = 1 ;
    WREN = 0 ;  
                                                      
    TBLPTR++;
                                        
}
///************************************************///

///*************************************************///
///*************************************************///
void outset(int outbitcon)
{
     BDOCNTOUT = outbitcon;//0x08;
     BDOADRLOUT = 0x00;
     BDOADRHOUT =0x05;
                                             
     BDOSTATOUT =0x80;  
}

///*************************************************///

///*************************************************///
///*************************************************///
void inset(int inbitcon,int inlowadr)
{
     BDOCNTIN  = inbitcon;//0x10; //0x08 ;
     BDOADRLIN = inlowadr ;
     BDOADRHIN = 0x05 ;
                                                                      
     BDOSTATIN = 0x80;
}
///*************************************************///