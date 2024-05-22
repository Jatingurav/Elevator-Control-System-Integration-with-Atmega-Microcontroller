/*
 * Elavator.c
 *
 * Created: 06-05-2024 10:11:35
 * Author : ADMIN
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_DATA PORTA
#define ctrl PORTB
#define rs PB0
#define rw PB1
#define en PB3

void init_LCD(void);
void LCD_cmd(unsigned char cmd);
void LCD_write(unsigned char data);
void LCD_write_string(char *str);
void init_interrupt();

void init_interrupt()
{
	cli();
	GICR = (1<<INT2);
	MCUCSR = (1<<ISC2);
	sei();
}

ISR(INT2_vect)
{
	PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
	init_LCD();
	LCD_cmd(0x01);
	LCD_write_string("FORCE STOP");
	PORTD = 0b01111111;
	
}

int main(void)
{
    
	DDRC = DDRC & (~(1<<0)) & (~(1<<1)) & (~(1<<2)) & (~(1<<3)) & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7))  ; 
	/* Make  PC0 PC1 PC2 PC3 PC4 PC5 PC6 PC7 as input pin */
	
	
	PORTC = PORTC & (~(1<<0)) & (~(1<<1)) & (~(1<<2)) & (~(1<<3)) & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
	/* Enable PC0 PC1 PC2 PC3 PC4 PC5 PC6 PC7  by writing 0 to it */
	
	DDRB = DDRB | (1<<4) | (1<<5) | (1<<6) | (1<<7); 
	/* Make PB4 PB5 (Up MOTOR)  PB6 PB7 (Down MOTOR)  as output pin For Motor Driver */
	
	DDRA = 0xFF;  //LCD DATA 
	DDRB = DDRB | (1<<0) | (1<<1) | (1<<3); //LCD Control PB0 PB1 PB3
	
	DDRB = DDRB & (~(1<<2)); //INT2 PIN PB2
	PORTB = PORTB & (~(1<<2)); 
	
	DDRD = DDRD | (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6)  ; //PORTD Output for 7-Segment display
	unsigned char arr[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; //Common Anode
	int i=0;
	
	init_LCD();
	_delay_ms(2);
	LCD_write_string("WELCOME");
	LCD_cmd(0xC0);
	_delay_ms(2);
	LCD_write_string("GROUND FLOOR");
	PORTD = arr[i];
	
    while (1) 
    {
	
		if (PINC & (1<<3))  /* Ground Floor Motor rest*/
		{
			while (PINC & (1<<3))
			{
				
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				PORTD = arr[0];
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("GROUND FLOOR");
				
			}
		
		}
		else if (PINC & (1<<2)) /* Ground to First Floor Motor Running Forward(Up Direction)*/
		{
			while (PINC & (1<<2))
			{
				init_LCD();
				LCD_cmd(0x01);
				PORTB = PORTB | ((~(1<<4)) & (1<<5) & (~(1<<6)) & (~(1<<7)));
				_delay_ms(4000);
				while (i<=0)
				{
					i++;
					PORTD = arr[i];
					_delay_ms(500);
				}
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("FIRST FLOOR");
			}
	
		}
		
		else if (PINC & (1<<1)) /* Second Floor Motor Running Forward(Up Direction)*/
		{
			while (PINC & (1<<1))
			{
				
				init_LCD();
				LCD_cmd(0x01);
				PORTB = PORTB | ((~(1<<4)) & (1<<5) & (~(1<<6)) & (~(1<<7)));
				_delay_ms(4000);

                while (i<=1)
                {
	                i++;
	                PORTD = arr[i];
	                _delay_ms(500);
	                
                }
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("SECOND FLOOR");
			}
			
		}
		
		else if (PINC & (1<<0)) /* Third Floor Motor Running Forward(Up Direction)*/
		{
			while (PINC & (1<<0))
			{
				init_LCD();
				LCD_cmd(0x01);
				PORTB = PORTB | ((~(1<<4)) & (1<<5) & (~(1<<6)) & (~(1<<7))) ;
				_delay_ms(4000);
				 				
				while (i<=2)
				{
					i++;
					PORTD = arr[i];
					_delay_ms(500);
				}		
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("THIRD FLOOR");
			}
		}
		
		else if (PINC & (1<<4)) /* Third Floor Motor rest*/
		{
			while (PINC & (1<<4))
			{
				
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				PORTD = arr[3];
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("THIRD FLOOR");
			}
		}
		
		else if (PINC & (1<<5)) /* To come to second floor, Motor running reverse(down) direction*/
		{
			while (PINC & (1<<5))
			{
				
				init_LCD();
				LCD_cmd(0x01);
				PORTB = PORTB | ((~(1<<4)) & (~(1<<5)) & (1<<6) & (~(1<<7))) ;
				_delay_ms(4000);
				while (i>=2)
				{
					PORTD = arr[i];
					_delay_ms(500);
					--i;
				}
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("SECOND FLOOR");
			}
		}
		
		else if (PINC & (1<<6)) /* To come to first floor, Motor running reverse(down) direction*/
		{
			while (PINC & (1<<6))
			{
				init_LCD();
				LCD_cmd(0x01);
				PORTB = PORTB | ((~(1<<4)) & (~(1<<5)) & (1<<6) & (~(1<<7))) ;
				_delay_ms(4000);
				while (i>=1)
				{
					PORTD = arr[i];
					_delay_ms(500);
					--i;
				}
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("FIRST FLOOR");
			}
		}
		
		else if (PINC & (1<<7)) /* To come to ground floor, Motor running reverse(down) direction*/
		{
			while (PINC & (1<<7))
			{
				init_LCD();
				LCD_cmd(0x01);
				PORTB = PORTB | ((~(1<<4)) & (~(1<<5)) & (1<<6) & (~(1<<7))) ;
				_delay_ms(4000);
				while (i>=0)
				{
					PORTD = arr[i];
					_delay_ms(500);
					--i;
				}
				PORTB = PORTB & (~(1<<4)) & (~(1<<5)) & (~(1<<6)) & (~(1<<7)) ;
				init_LCD();
				LCD_cmd(0x01);
				_delay_ms(1);
				LCD_write_string("GROUND FLOOR");
			}
		}
			
		init_interrupt(); //To forcefully stop the motor for emergency
    }
}


void init_LCD(void)
{
	LCD_cmd(0x38);
	_delay_ms(1);
	LCD_cmd(0x01);
	_delay_ms(1);
	LCD_cmd(0x0C);
	_delay_ms(1);
	_delay_ms(0x80);
	_delay_ms(1);
	
	return;
}

void LCD_cmd(unsigned char cmd)
{
	LCD_DATA=cmd;
	ctrl=(0<<rs)|(0<<rw)|(1<<en);
	_delay_ms(1);
	ctrl=(0<<rs)|(0<<rw)|(0<<en);
	_delay_ms(1);
	return;
}

void LCD_write(unsigned char data)
{
	LCD_DATA=data;
	ctrl=(1<<rs)|(0<<rw)|(1<<en);
	_delay_ms(1);
	ctrl=(1<<rs)|(0<<rw)|(0<<en);
	_delay_ms(1);
	return;
	
}

void LCD_write_string(char *str)
{
	int i=0;
	while(str[i]!='\0')
	{
		LCD_write(str[i]);
		i++;
	}
	return;
}

