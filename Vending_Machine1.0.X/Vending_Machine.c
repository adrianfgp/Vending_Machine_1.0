/*!
\file   Vending_Machine.c
\date   2020-12-14
\author Adrian Felipe Gironza<adrianf@unicauca.edu.co>
        Jose Ricardo Galindez<jrgalindez@unicauca.edu.co>
        Kevin Daryany Morales<kevindm@unicauca.edu.co>
\brief  Example Vending_Machine.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2020. All rights reserved.

\par
The copyright notices above do not evidence any actual or
intended publication of this material.
******************************************************************************
*/
#include <stdio.h>
#include "Configuration_Header_File.h"
#include "20x4_LCD_File.h"
#include <pic18f4550.h>
#include "I2C_Master_File.h"
#include "Keypad_File.h"
#include <xc.h>
#include<string.h>
#define device_id_write 0xD0
#define device_id_read 0xD1
#define device_id_eeprom_write 0xA0
#define device_id_eeprom_read 0xA1
#define MX2 LATA0
#define MX1 LATA1
#define MXE LATA2

int consultarprecio(char a);
char *itoa__(int value);
void RTC_Read_Clock(char read_clock_address);
void RTC_Read_Calendar(char read_calendar_address);
void Rtc();
int sec, min, hour;
int Day, Date, Month, Year;
char Data_read;
int saldo=0;
int v_proentregar[3]={0,0,0};

void EEPROM_Write(int, char); /* Write byte to EEPROM data memory */
char EEPROM_Read(int); /* Read Byte From EEPROM data memory */
void EEPROM_WriteString(int, char*); /* Write byte to EEPROM data memory */
void memoria();
void escribirmemoria(int posicion,char c);
void leermemoria(int posicion);
void entregarproducto(int a);
void entregarproductos();


void main(void) {
    //ADCON1 |= 0x0F;
    //CMCON  |= 7;  

    TRISA=0X00;
    
    int p,x = 0;
    saldo = 5000;
    char key;
    int v_prodisponibles[] = {2,3,1,1};
    int v_proprecios[] = {1000,3500,500,2000};

    
    
    //memoria();

    unsigned char op = 1;
    //   OSCCON = 0x72;          
    RDPU = 0; /* activate pull-up resistor */
    I2C_Init(); /*initialize I2C protocol*/
    LCD_Init(); /*initialize LCD16x2*/
    MXE=0XFF;
    LCD_Clear();
    MSdelay(10);
    while (1) {
        int i=0;
        for(int z=0;z<3;z++){
            v_proentregar[z]=0;
        }
        int compra = 0;
        //int saldo =0;
        do {
            LCD_Clear();
            Rtc();
            MSdelay(1000);
            LCD_String_xy(3, 7, "-Bienvenido-");
            LCD_String_xy(4, 0, "Presione una tecla..");
            key = keyfind();
        } while (key == '1' && (key == '2'));
       
        MSdelay(20);
        do {
            LCD_Clear();
            LCD_String_xy(1, 0, "1.Cafe $1000");
            LCD_String_xy(2, 0, "2.Gaseosa 3L $3500");
            LCD_String_xy(3, 0, "3.Bombones $500");
            LCD_String_xy(4, 0, "4.Papas $2000");
            key = keyfind(); 
        
            if (key == '1') {
                p=0;
          //  leermemoria(1);
                //   x = Data_read -'0';
                if (v_prodisponibles[p] > 0) {
                    v_prodisponibles[p] =v_prodisponibles[p] - 1;
                     // char y;
                     // y=x+'0';
                    //escribirmemoria(1,y);
                    LCD_Clear();
                    LCD_String("Se agrego Cafe");
                    compra = compra + v_proprecios[p];
                    i = i + 1;
                    v_proentregar[i-1]=p+1;
                    
                } else {
                  
                    LCD_Clear();
                    LCD_String("No disponible");
                }

            }
            if (key == '2') {
                  p=1;
                if (v_prodisponibles[p] > 0) {
                    v_prodisponibles[p] =v_prodisponibles[p] - 1;
                    LCD_Clear();
                            LCD_String("Se agrego Gaseosa");
                    compra = compra + v_proprecios[p];
                    i = i + 1;
                    v_proentregar[i-1]=p+1;
                    
                } else {
                    LCD_Clear();
                    LCD_String("No disponible");
                }
          
            }
            if (key == '3') {
                  p=2;
                if (v_prodisponibles[p] > 0) {
                    v_prodisponibles[p] =v_prodisponibles[p] - 1;
                    LCD_Clear();
                            LCD_String("Se agrego bombones");
                    compra = compra + v_proprecios[p];
                    i = i + 1;
                    v_proentregar[i-1]=p+1;
                } else {
                    LCD_Clear();
                    LCD_String("No disponible");
                }
             }
            if (key == '4') {
                      p=3;
                if (v_prodisponibles[p] > 0) {
                    v_prodisponibles[p] =v_prodisponibles[p] - 1;
                    LCD_Clear();
                            LCD_String("Se agrego Papas");
                    compra = compra + v_proprecios[p];
                    i = i + 1;
                    v_proentregar[i-1]=p+1;
                } else {
                    LCD_Clear();
                    LCD_String("No disponible");
                }
             }
            MSdelay(700);
            LCD_Clear();
            
            LCD_String("Desea aniadir");
            LCD_String_xy(2, 0, "otro producto");
            LCD_String_xy(3, 0, "+.Si");
            LCD_String_xy(4, 0, "=.No");
            //LCD_Clear();
            key = keyfind();
            /* find a pressed key */
            //LCD_Char(key);  
            MSdelay(100);



        } while (key == '+' && i < 3);

     
            MSdelay(1000);
            LCD_Clear();
            char c[6];
            int a = compra;
            sprintf(c, "%d", a);
            LCD_String("TOTAL :");
            LCD_String_xy(2, 0, c);
            MSdelay(1000);
            if (saldo < compra) {
                LCD_Clear();
                LCD_String("Saldo insuficinte ");
                int a1=saldo;
                char c1[6];
                sprintf(c1, "%d", a1);
                LCD_String_xy(3,0,"Su cambio es: ");
                LCD_String_xy(4, 0, c1);
                MSdelay(1000);
            } else {
                entregarproductos();
                LCD_Clear();
                int a1 = saldo - compra;
                char c1[6];
                sprintf(c1, "%d", a1);
                LCD_String("Su cambio es: ");
                LCD_String_xy(2, 0, c1);
                LCD_String_xy(3, 0, "GRACIAS POR COMPRAR");
                MSdelay(1000);
            }
        

    }

}

void RTC_Read_Clock(char read_clock_address)
{
    I2C_Start(device_id_write);
    I2C_Write(read_clock_address);     /* address from where time needs to be read*/
    I2C_Repeated_Start(device_id_read);
    sec = I2C_Read(0);                 /*read data and send ack for continuous reading*/
    min = I2C_Read(0);                 /*read data and send ack for continuous reading*/
    hour= I2C_Read(1);                 /*read data and send nack for indicating stop reading*/
      I2C_Stop();
}

void RTC_Read_Calendar(char read_calendar_address)
{   
    I2C_Start(device_id_write);
    I2C_Write(read_calendar_address); /* address from where time needs to be read*/
    I2C_Repeated_Start(device_id_read);
    Day = I2C_Read(0);                /*read data and send ack for continuous reading*/
    Date = I2C_Read(0);               /*read data and send ack for continuous reading*/
    Month = I2C_Read(0);              /*read data and send ack for continuous reading*/
    Year = I2C_Read(1);               /*read data and send nack for indicating stop reading*/
    I2C_Stop();
}
void Rtc()
{

    char secs[10],mins[10],hours[10];
    char date[10],month[10],year[10];
    char Clock_type = 0x06;
    char AM_PM = 0x05;
    char days[7] = {'S','M','T','W','t','F','s'};
    OSCCON=0x72;                    /*Use internal oscillator and 
                                     *set frequency to 8 MHz*/ 

        RTC_Read_Clock(0);              /*gives second,minute and hour*/
      
        if(hour & (1<<Clock_type)){     /* check clock is 12hr or 24hr */
            
            if(hour & (1<<AM_PM)){      /* check AM or PM */
                LCD_String_xy(1,14,"PM");
            }
            else{
                LCD_String_xy(1,14,"AM");
            }
            
            hour = hour & (0x1f);
            sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
            sprintf(mins,"%x:",min);    
            sprintf(hours,"Hora:%x:",hour);  
            LCD_String_xy(0,0,hours);            
            LCD_String(mins);
            LCD_String(secs);
        }
        else{
            
            hour = hour & (0x3f);
            sprintf(secs,"%x ",sec);   /*%x for reading BCD format from RTC DS1307*/
            sprintf(mins,"%x",min);    
            sprintf(hours,"Time:%x:",hour);  
            LCD_String_xy(0,0,hours);            
            LCD_String(mins);
            //LCD_String(secs); 
        }
        
        RTC_Read_Calendar(3);        /*gives day, date, month, year*/        
        I2C_Stop();
        sprintf(date,"Date: %x-",Date);
        sprintf(month,"%x-",Month);
        sprintf(year,"%x ",Year);
        LCD_String_xy(2,0,date);
        LCD_String(month);
        LCD_String(year);

    /* find day */
        switch(days[Day])
        {
            case 'S':
                        LCD_String("Sun");
                        break;
            case 'M':
                        LCD_String("Mon");
                        break;                
            case 'T':
                        LCD_String("Tue");
                        break;                
            case 'W':   
                        LCD_String("Wed");
                        break;                
            case 't':
                        LCD_String("Thu");
                        break;
            case 'F':
                        LCD_String("Fri");
                        break;                
            case 's':
                        LCD_String("Sat");
                        break;
            default: 
                        break;
                        
        }

}

void entregarproductos(){
    for(int i=0;i<3;i++){
        entregarproducto(v_proentregar[i]);
    }
}
void entregarproducto(int a){
    MXE=0;
    if(a==1){
        MX1=0;
        MX2=0;
    } 
    if(a==2){
        MX1=0;
        MX2=0xff;
    } 
    if(a==3){
        MX1=0xff;
        MX2=0;
    } 
    if(a==4){
        MX1=0xff;
        MX2=0xff;
    }
    MSdelay(1000); 
    MXE=0XFF;
}


void memoria(){
        escribirmemoria(1,"2");
        escribirmemoria(2,"2");
        escribirmemoria(3,"2");
        escribirmemoria(4,"2");
}


void escribirmemoria(int posicion,char c){
    OSCCONbits.IRCF=0b1110;
    OSCCONbits.SCS=0b00;
    //Puertos
    TRISB=255;
    SSPCON1bits.SSPEN=1;
    
    SSPCON1bits.SSPM3=1;
    SSPCON1bits.SSPM2=0;
    SSPCON1bits.SSPM1=0;
    SSPCON1bits.SSPM0=0;
    SSPADD=0X4F;//100KHZ BAUDRATE
    //TRAMA DE ESCRITURA//
    SSPCON2bits.SEN=1; //señal de start
    while(SSPCON2bits.SEN); //Espera que se envie la señal de start
    SSPBUF=0B10100000; //DIRECCION +0
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    SSPBUF=0;
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    SSPBUF=posicion;
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    SSPBUF=c;
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    
    SSPBUF=10;
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    SSPCON2bits.PEN=1;
    while(SSPCON2bits.PEN==1);
    
}

void leermemoria(int posicion){
  char y=(char)posicion;//+'0';
    
    OSCCONbits.IRCF=0b1110;
    OSCCONbits.SCS=0b00;
    //Puertos
    TRISB=255;
    SSPCON1bits.SSPEN=1;
    
    SSPCON1bits.SSPM3=1;
    SSPCON1bits.SSPM2=0;
    SSPCON1bits.SSPM1=0;
    SSPCON1bits.SSPM0=0;
    SSPADD=0X4F;//100KHZ BAUDRATE
    //Trama de lectura
    SSPCON2bits.SEN=1; //señal de start
    while(SSPCON2bits.SEN); //Espera que se envie la señal de start
    SSPBUF=0B10100000; //DIRECCION +0
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    SSPBUF=0;
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    SSPBUF=y;
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    //señal de restart
    SSPCON2bits.RSEN=1;
    while(SSPCON2bits.RSEN); //Espera que se envie la señal de start
    SSPBUF=0B10100001; //DIRECCION + 1 LE
    while(SSPSTATbits.BF); //Si se manda el dato bf =0
    while(SSPSTATbits.R_nW); //9scl
    //MODO RECEPTCION MAESTRO
    SSPCON2bits.RCEN=1;
    while(SSPSTATbits.BF==0); //MIENTRAS ESTE VACIO
    Data_read=SSPBUF;
    SSPCON2bits.ACKDT=1; //No ACK
    SSPCON2bits.ACKEN=1;
    while(SSPCON2bits.ACKEN);
    SSPCON2bits.PEN=1;
    while(SSPCON2bits.PEN);
    
    
}
