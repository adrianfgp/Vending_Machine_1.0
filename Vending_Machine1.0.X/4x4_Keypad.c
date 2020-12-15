/*
 * Keypad Interfacing with PIC18F4550
 * http://www.electronicwings.com
 * 
 */

#include "Keypad_File.h"


//void main() 
//{
//    char key;
//    OSCCON = 0x72;
//
//    RDPU=0;                             /* activate pull-up resistor */
//    LCD_Init();                         /* initialize LCD16x2 in 4-bit mode */
//    LCD_String_xy(0,0,"Press a Key");
//    LCD_Command(0xC0);                  /* display pressed key on 2nd line of LCD */
//    while(1)
//    {
//        key = keyfind();                /* find a pressed key */
//        LCD_Char(key);                  /* display pressed key on LCD16x2 */
//    }
//    
//}

unsigned char keyfind()
{      
    
    Direction_Port = 0xf0;              /*PORTD.0-PORTD.3 as a Output Port and PORTD.4-PORTD.7 as a Input Port*/
    //unsigned char temp1;
   
        write_port = 0xf0;              /*Make lower nibble as low(Gnd) and Higher nibble as High(Vcc)*/
      do
      {
        do
        {
          
            col_loc = read_port & 0xf0; /*mask port with f0 and copy it to col_loc variable*/   
        
        }while(col_loc!=0xf0);          /*Check initially at the start there is any key pressed*/ 
         col_loc = read_port & 0xf0;    /*mask port with f0 and copy it to col_loc variable*/  
      }while(col_loc!=0xf0);
      
      MSdelay(50);
        write_port = 0xf0;              /*Make lower nibble as low(Gnd) and Higher nibble as High(Vcc)*/
        do
        { do
            
            {
               
                col_loc = read_port & 0xf0;
            }while(col_loc==0xf0);      /*Wait for key press*/
         col_loc = read_port & 0xf0;
        }while(col_loc==0xf0);          /*Wait for key press*/
               
        MSdelay(20);
        
        col_loc = read_port & 0xf0;
               
   
    
    while(1)
    {
        write_port  = 0xfe;                 /*make Row0(D0) Gnd and keep other row(D1-D3) high*/
        col_loc = read_port & 0xf0;         /*Read Status of PORT for finding Row*/
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=0;                       /*If condition satisfied get Row no. of key pressed*/
            while(temp_col!=0xf0)           /*Monitor the status of Port and Wait for key to release*/
            {
                temp_col = read_port & 0xf0;/*Read Status of PORT for checking key release or not*/    
            }
            break;
        }
        
        write_port = 0xfd;                  /*make Row1(D1) Gnd and keep other row(D0-D2-D3) high*/
        col_loc = read_port & 0xf0;         /*Read Status of PORT for finding Row*/
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=1;                       /*If condition satisfied get Row no. of key pressed*/
            while(temp_col!=0xf0)           /*Monitor the status of Port and Wait for key to release*/
            {
                temp_col = read_port & 0xf0;/*Read Status of PORT for checking key release or not*/
            }
            break;
        }
        
        write_port = 0xfb;                  /*make Row0(D2) Gnd and keep other row(D0-D1-D3) high*/
        col_loc = read_port & 0xf0;         /*Read Status of PORT for finding Row*/
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=2;                       /*If condition satisfied get Row no. of key pressed*/
            while(temp_col!=0xf0)           /*Wait for key to release*/
            {
                temp_col = read_port & 0xf0;/*Read Status of PORT for checking key release or not*/
            }
            break;
        }
        
        write_port = 0xf7;                  /*make Row0(D3) Gnd and keep other row(D0-D2) high*/
        col_loc = read_port & 0xf0;         /*Read Status of PORT for finding Row*/
        temp_col=col_loc;
        if(col_loc!=0xf0)
        {
            rowloc=3;                       /*If condition satisfied get Row no. of key pressed*/
            while(temp_col!=0xf0)           /*Wait for key to release*/
            {
                temp_col = read_port & 0xf0;/*Read Status of PORT for checking key release or not*/
            }
            break;
        }
        
    }

    
    
    while(1)
    {
        
        if(col_loc==0xe0)
        {
             return keypad[rowloc][0];      /*Return key pressed value to calling function*/            
        }
        else 
            if(col_loc==0xd0)
        {
            return keypad[rowloc][1];       /*Return key pressed value to calling function*/   
        }
        else
            if(col_loc==0xb0)
        {
            return keypad[rowloc][2];       /*Return key pressed value to calling function*/
        }
        else
            
        {
            return keypad[rowloc][3];       /*Return key pressed value to calling function*/           
        }    
    }
    
   MSdelay(300);     
 
}