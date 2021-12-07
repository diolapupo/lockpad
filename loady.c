#include <xc.h>
#define _XTAL_FREQ 20000000
#include "eeprom.h"
#include "libcd.h"
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF   // Flash Program Memory Code Protection bit (Code protection off)
 unsigned char key, pass[6], dob[6];
 unsigned char i = 0;
  unsigned char safe = 0;
 unsigned char col = 10;   
unsigned char row = 1;

unsigned char keypadscan(void){
    unsigned char keypressed = 70;
    while(keypressed == 70){
        PORTA = 0b00000001;
        if(RC4){
            __delay_ms(100);
            while(RC4);
            __delay_ms(100);
            keypressed = 7;
            return keypressed;
        }
        if(RC5){
            __delay_ms(100);
            while(RC5);
            __delay_ms(100);
            keypressed = 8;
            return keypressed;
        }
        if(RC6){
            __delay_ms(100);
            while(RC6);
            __delay_ms(100);
            keypressed = 9;
            return keypressed;
        }
        if(RC7){
            __delay_ms(100);
            while(RC7);
            __delay_ms(100);
            keypressed =20;
            return keypressed;
        }
        
        
        PORTA = 0b00000010;
        if(RC4){
            __delay_ms(100);
            while(RC4);
            __delay_ms(100);
            keypressed = 4;
            return keypressed;
        }
        if(RC5){
            __delay_ms(100);
            while(RC5);
            __delay_ms(100);
            keypressed = 5;
            return keypressed;
        }
        if(RC6){
            __delay_ms(100);
            while(RC6);
            __delay_ms(100);
            keypressed = 6;
            return keypressed;
        }
        if(RC7){
            __delay_ms(100);
            while(RC7);
            __delay_ms(100);
            keypressed =18;
            return keypressed;
        }
        
        PORTA = 0b00000100;
        if(RC4){
            __delay_ms(100);
            while(RC4);
            __delay_ms(100);
            keypressed = 1;
            return keypressed;
        }
        if(RC5){
            __delay_ms(100);
            while(RC5);
            __delay_ms(100);
            keypressed = 2;
            return keypressed;
        }
        if(RC6){
            __delay_ms(100);
            while(RC6);
            __delay_ms(100);
            keypressed = 3;
            return keypressed;
        }
        if(RC7){
            __delay_ms(100);
            while(RC7);
            __delay_ms(100);
            keypressed =17;
            return keypressed;
        }
        
        PORTA = 0b00001000;
        if(RC4){
            __delay_ms(100);
            while(RC4);
            __delay_ms(100);
            keypressed = 15;
            return keypressed;
        }
        if(RC5){
            __delay_ms(100);
            while(RC5);
            __delay_ms(100);
            keypressed = 0;
            return keypressed;
        }
        if(RC6){
            __delay_ms(100);
            while(RC6);
            __delay_ms(100);
            keypressed = 19;
            return keypressed;
        }
        if(RC7){
            __delay_ms(100);
            while(RC7);
            __delay_ms(100);
            keypressed =16;
            return keypressed;
        }
    }
}

void disp(unsigned char keyp){    
    if(keyp < 10){
//        LCD_CURSOR(row,col-1);
        LCD_WRITE_NUM(key);
        LCD_CURSOR(row,col);
        __delay_ms(300);
        LCD_WRITE_STRING("*");
        col++;
    }
    else if(key==19){
         pass[i]=0;
         pass[i--]=0;
         i--;
        col--;
        LCD_CURSOR(row,col);
        LCD_WRITE_STRING(" ");
        LCD_CURSOR(row,col);
    }

}
unsigned char checkPass(unsigned char checker[6],unsigned char start){
    unsigned char val=0;
    for(unsigned char k = start; k<6;k++){
        if((unsigned char)read_eeprom(k) == checker[val]){
            val++;
        }
    }
    if(val == 6){
        return 1;
    }else{
        return 0;
    }
}
void changePass(unsigned char newPass[6],unsigned char start){
    unsigned char j=start;
    unsigned char k=0;
    while(k<6){
     write_eeprom(j,newPass[k]);   
     j++;
     k++;
    }
    write_eeprom(6,1);
    
}

void reset(void){
    RB0=0;
    i=0;
    col=10;
    row=1;
    LCD_CLR();
    LCD_CURSOR(1,1);
    LCD_WRITE_STRING("Password:");
    LCD_CURSOR(1,10);
    if(safe==1){
    safe=0;
    }
}
void main(void) {
    PORTA=PORTB=PORTC=PORTD=PORTE=0;
    TRISD = 0;
    TRISA = 0;
    TRISB=0;
    TRISC = 0b11110000;
    LCD_INIT();
    __delay_ms(500);
    LCD_CLR();
    LCD_WRITE_STRING("Welcome");
    __delay_ms(750);
    LCD_CURSOR(1,1);
    LCD_WRITE_STRING("Password:");
    LCD_CURSOR(1,10);
    
    if(!(read_eeprom(6)==1)){
        for(signed char j =0; j<6;j++){
            pass[j]=9;
        }
        changePass(pass,0);
    }
    
    while(1){     
    key = keypadscan();
    
    if(i<6 && safe!=1){
        pass[i]=key;
        disp(key);
        i++;
    }
    if(key==16 && safe==1){
    LCD_CLR();
    LCD_CURSOR(2,1);
    LCD_WRITE_STRING("DOB[dd/mm/yy]:");
    LCD_CURSOR(1,1);
    LCD_WRITE_STRING("New Password:");
    col = 14;
    row=1;
    while(i<6){
        if(col>16){
        LCD_SCROLL(1,10,0);
        }
        key = keypadscan();
        pass[i]=key;
        disp(key);
        i++;
    }
    i=0;
    LCD_CURSOR(2,15);
    col = 15;
    row=2;
    LCD_SCROLL(0,9,1);
    while(i<6){
        if(col>16){
        LCD_SCROLL(1,10,0);
        }
        key = keypadscan();
        dob[i]=key;
        disp(key);
        i++;
    }
   
    changePass(pass,0);
    changePass(dob,7);
    LCD_CURSOR(1,1);
    LCD_CLR();
    LCD_WRITE_STRING("Password changed.");
    __delay_ms(100);
    reset();
    }
    
    if(i==6){
        if(checkPass(pass,0)){
            i=0;
            RB0=1;
            safe=1;
            LCD_CURSOR(1,1);
            LCD_WRITE_STRING("You can enter...");
            LCD_CURSOR(2,1);
            LCD_WRITE_STRING("Press + to change your password");
            LCD_SCROLL(1,9,16);
            }
        else{
            if(safe==4){
                LCD_CURSOR(1,1);
            LCD_WRITE_STRING("Forgot Pass?");
            LCD_CURSOR(2,1);
            LCD_WRITE_STRING("DOB[dd/mm/yy]:");
            i=0;
            LCD_CURSOR(2,15);
            col = 15;
            row=2;
        while(i<6){
            if(col>16){
                LCD_SCROLL(1,10,0);
            }
            key = keypadscan();
            dob[i]=key;
            disp(key);
            i++;
            }
            if(checkPass(dob,7)){
                LCD_CLR();
    LCD_CURSOR(2,1);
    LCD_WRITE_STRING("DOB[dd/mm/yy]:");
    LCD_CURSOR(1,1);
    LCD_WRITE_STRING("New Password:");
    col = 14;
    row=1;
    while(i<6){
        if(col>16){
        LCD_SCROLL(1,10,0);
        }
        key = keypadscan();
        pass[i]=key;
        disp(key);
        i++;
    }
    i=0;
    LCD_CURSOR(2,15);
    col = 15;
    row=2;
    LCD_SCROLL(0,9,1);
    while(i<6){
        if(col>16){
        LCD_SCROLL(1,10,0);
        }
        key = keypadscan();
        dob[i]=key;
        disp(key);
        i++;
    }
   
    changePass(pass,0);
    changePass(dob,7);
    LCD_CURSOR(1,1);
    LCD_CLR();
    LCD_WRITE_STRING("Password changed.");
    __delay_ms(100);
    reset();
            }    
            
            }
        LCD_CLR();
        LCD_CURSOR(1,1);
        LCD_WRITE_STRING("Wrong Password");
//        __delay_ms(1000);
        reset();
        safe+=2;
        }
    }   
}   
    return;
}
