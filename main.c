/*
 * CS145Lab2.c
 *
 * Created: 4/21/2016 3:54:15 PM
 * Author : toran
 */ 

#include "avr.h"
unsigned int speed = 1;
unsigned int convertKeypad(unsigned int input)
{
	unsigned int output = input;
	if(input == 5 || input == 6 || input == 7)
	{
		output = input - 1;
	}
	else if(input == 9 ||input == 10 || input == 11)
	{
		output = input - 2;
	}
	else if(input == 14)
	{
		output = 0;
	}
	
	
	return output;
	
}

unsigned char pressed(unsigned char r, unsigned char c)
{
	DDRC = 0;
	PORTC = 0;
	SET_BIT(DDRC, c + 4);
	SET_BIT(PORTC, r);
	CLR_BIT(PORTC, c + 4);
	//CLR_BIT(PORTC, r << 4);
	
	wait_avr(1);
	
	if(!(GET_BIT(PINC, r)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

unsigned char get_key()
{
	unsigned char r;
	unsigned char c;
	
	for(r = 0; r < 4; ++r)
	{
		for(c = 0; c < 4; ++c)
		{
			
			if (pressed(r, c))
			{
				return (r * 4) + (c + 1);
			}
		}
	}
	return 0;
}


						
 
 int main (void)
 {
	 DDRA = 0;
	 ADMUX |=(1<<REFS0);
	 ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
	 double results[1048];
	 int resultCount = 0;
	 
	 
	 float voltage = 0;
	 float total = 0;
	 float min = 100;
	 float max = 0;
	 char voltInst[4];
	 char voltMin[4];
	 char voltMax[4];
	 char voltAvg[4];
	 
	
	ini_lcd();
	unsigned int running = 1;
	unsigned int keypressed = 0;
	char line1[32];
	char line2[32];
	pos_lcd(0,0);
	clr_lcd();
	sprintf(line1, "%s" , "Welcome to Voltmeter");
	puts_lcd2(line1);
	pos_lcd(1,0);
	sprintf(line2, "%s" , "Press 1 to start reading");
	puts_lcd2(line2);
	
	 ADCSRA |=(1<<ADSC);
	
	while(running)
	{
		if(!(keypressed = get_key()))
		{
				
		}
		else
		{
			//
			if(convertKeypad(keypressed) == 1)
			{
				
				while(convertKeypad(keypressed) != 2)
				{
					int keypressed2 = -1;
					voltage = ADC/204.8;
					total += voltage;
					
					if (voltage > max)
					{
						max = voltage;
					}
					
					if(voltage < min)
					{
						min = voltage;
					}
					resultCount++;
					//calculate
					dtostrf(voltage, 4, 2, voltInst);
					dtostrf(min, 4, 2, voltMin);
					dtostrf(max, 4, 2, voltMax);
					
					dtostrf((total / resultCount), 4, 2, voltAvg);
					
					
					
					//display
					pos_lcd(0,0);
					clr_lcd();
					pos_lcd(1,0);
					clr_lcd();
					pos_lcd(0,0);
					sprintf(line1, "%.4s, %.4s", voltInst, voltAvg);
					puts_lcd2(line1);
					pos_lcd(1,0);
					sprintf(line2, "%.4s, %.4s", voltMin, voltMax);
					puts_lcd2(line2);
					//wait
					wait_avr(500);
					keypressed2 = get_key();
					if(keypressed2 != 0)
					{
						keypressed = keypressed2;
					}
				}
				
				pos_lcd(0,0);
				clr_lcd();
				pos_lcd(1,0);
				clr_lcd();
				pos_lcd(0,0);
				sprintf(line1, "%.4s, %.4s", "----", "----");
				puts_lcd2(line1);
				pos_lcd(1,0);
				sprintf(line2, "%.4s, %.4s", "----", "----");
				puts_lcd2(line2);
				max = 0;
				min = 100;
				total = 0;
				resultCount = 0;
				//reset
			}

		}
	}
				
	 
	 return 0;
 }
