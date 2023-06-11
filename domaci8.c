#include <at89c51rc2.h>

#include "display.h"


char prg1[10] = {'P','r', 'o', 'g', 'r', 'a', 'm', ' ', '1', '\0'};
char prg2[10] = {'P','r', 'o', 'g', 'r', 'a', 'm', ' ', '2', '\0'};
char prg3[10] = {'P','r', 'o', 'g', 'r', 'a', 'm', ' ', '3', '\0'};
char prg4[10] = {'P','r', 'o', 'g', 'r', 'a', 'm', ' ', '4', '\0'};

char stopped[8] = {'S','t','o','p','p','e','d','\0'};
char running[8] = {'R','u','n','n','i','n','g','\0'};

unsigned char status = 0;
unsigned char counter1_speed = 0;
unsigned char counter2_speed = 0;
unsigned char speed = 0;
unsigned char indication = 0;

unsigned char counter0 = 0;
unsigned char p0_real_last = 0;
unsigned char p0_soft = 0;
unsigned char p0_soft_last = 0;

unsigned char counter1 = 0;
unsigned char p1_real_last = 0;
unsigned char p1_soft = 0;
unsigned char p1_soft_last = 0;

unsigned char counter2 = 0;
unsigned char p2_real_last = 0;
unsigned char p2_soft = 0;
unsigned char p2_soft_last = 0;

char input_buffer[10];
char in;
char* error = ">error<";
char* ok = ">OK<";
unsigned char iterator_input = 0;
unsigned char* ptr_send_data;

unsigned char state = 0;
unsigned char mode = 0;
unsigned char rev;
unsigned char mask_1;
unsigned char mask_2;
unsigned char mask_2_1;

void interrupt_handler_serial() interrupt 4
{
	if(RI)
	{
		RI = 0;
		in = SBUF;
		if(in == '>')
		{
			iterator_input = 0;
		}
		else if(in == '<')
		{
			if(input_buffer[0] == 'S' && input_buffer[1] == 'T' && input_buffer[2] == 'A' && input_buffer[3] == 'R' && input_buffer[4] == 'T')
			{	
				state = 1;
				ptr_send_data = ok;
				SBUF = *ptr_send_data;
				
			}
			else if(input_buffer[0] == 'S' && input_buffer[1] == 'T' && input_buffer[2] == 'O' && input_buffer[3] == 'P')
			{
				state = 0;
				ptr_send_data = ok;
				SBUF = *ptr_send_data;
			}
			else if(input_buffer[0] == 'P' && input_buffer[1] == ':')
			{
				if(input_buffer[2] == '1')
				{
					mode = 1;
					ptr_send_data = ok;
					SBUF = *ptr_send_data;
				}
				else if(input_buffer[2] == '2')
				{
					mode = 2;
					ptr_send_data = ok;
					SBUF = *ptr_send_data;
				}
				else if(input_buffer[2] == '3')
				{
					mode = 3;
					mask_1 = 0x01;
					mask_2 = 0x80;

					ptr_send_data = ok;
					SBUF = *ptr_send_data;
				}
				else if(input_buffer[2] == '4')
				{
					mode = 4;
					rev = 0;
					mask_2_1 = 64;
					mask_1 = 0x01;
					mask_2 = 0x80;

					ptr_send_data = ok;
					SBUF = *ptr_send_data;
				}
				else
				{
					ptr_send_data = error;
					SBUF = *ptr_send_data;
				}
			}
			else if(input_buffer[0] == 'B' && input_buffer[1] == ':')
			{
				if(input_buffer[2] == '1')
				{
					speed = 0;
					ptr_send_data = ok;
					SBUF = *ptr_send_data;
					counter2_speed = 0;
				 	counter1_speed = 0;
	
				}
				else if(input_buffer[2] == '2')
				{
					speed = 1;
					ptr_send_data = ok;
					SBUF = *ptr_send_data;
					counter2_speed = 0;
					counter1_speed = 0;	
				}
				else
				{
					ptr_send_data = error;
					SBUF = *ptr_send_data;	
				}	
			}
			else
			{
				ptr_send_data = error;
				SBUF = *ptr_send_data;
			}

		}

		else
		{
			input_buffer[iterator_input++] = in;
			if(iterator_input > 19)
			{
				iterator_input = 0;
			}
		}
	 }
	if(TI)
	{
		TI = 0;
		ptr_send_data++;
		if(*ptr_send_data)
			SBUF = *ptr_send_data;
	}
}





void interrupt_handler1() interrupt 3 
{
	
	if(++counter1_speed == 250)
	{
		counter1_speed = 0;
		if(!speed)
		{
			if(++counter2_speed == 16)
			{
				counter2_speed = 0;
				status = 1;	
			}
	    }
		else
		{
			if(++counter2_speed == 8)
			{
				counter2_speed = 0;
				status = 1;
			}
		}
		
	}
		
}

void interrupt_handler_t0(void) interrupt 1
{
	TH0 = 0xEE;
	TL0 = 0x00;
	indication = 1;
}

void interrupt_init()
{
	P2 = 0;
	TMOD = 0x21;
		 
	TH1 = 0x18;
	TL1 = 0x18;
	ET1 = 1;
	TR1 = 1;
	
	TH0 = 0xEE;
	TL0 = 0x00;
	TR0 = 1;
	ET0 = 1;

	CKCON0 &= 0xFE;
	SCON = 0x50;
	PCON &= 0x3F;
	PCON |= 0x80;
	BDRCON = 0x1C;
	BRL = 253;
	ES = 1;
	
	EA = 1;	
}


void lcd_display(unsigned char mode)
{

	if(state == 0)
	{
		cleardisp();
		setDDRAMaddrDisp(0x00);
		str2disp(stopped);
	}
	else
	{
		cleardisp();
		setDDRAMaddrDisp(0x00);
		str2disp(running);
	}

	if(mode == 1)
	{
		setDDRAMaddrDisp(0x40);
		str2disp(prg1);
	}
	else if(mode == 2)
	{
		setDDRAMaddrDisp(0x40);
		str2disp(prg2);	
	}
	else if(mode == 3)
	{
		setDDRAMaddrDisp(0x40);
		str2disp(prg3);		
	}
	else if(mode == 4)
	{
		setDDRAMaddrDisp(0x40);
		str2disp(prg4);	
	}	
}



void main()
{

	interrupt_init();
	initDsp();
	cleardisp();

	while(1)
	{
		if(indication)
		{
			indication = 0;
			if(p0_real_last != P0_0)
			{
				counter0 = 0;
			}
			else
			{
				counter0++;
				if(counter0	> 4)
				{
					p0_soft = P0_0;
					counter0 = 0;
				}
			}

			if(p0_soft < p0_soft_last)
			{
				if(state)
					state = 0;  //ne radi
				else
					state = 1;	//radi
			}

			p0_soft_last = p0_soft;
			p0_real_last = P0_0;

			////
							
			if(p1_real_last != P0_1)
			{
				counter1 = 0;
			}
			else
			{
				counter1++;
				if(counter1	> 4)
				{
					p1_soft = P0_1;
					counter1 = 0;
				}
			}


			if(p1_soft < p1_soft_last)
			{
				if(speed)
					speed = 0;	 
				else
					speed = 1;

				counter2_speed = 0;
				counter1_speed = 0;	 
			
			}
			p1_real_last = P0_1;
			p1_soft_last = p1_soft;

			////

			if(p2_real_last != P0_2)
			{
				counter2 = 0;
			}
			else
			{
				counter2++;
				if(counter2	> 4)
				{
					p2_soft = P0_2;
					counter2 = 0;
				}
			}


			if(p2_soft < p2_soft_last)
			{
				if(mode >= 4)
				{
					P2 = 0;
					mode = 1;
				}
				else
				{
					P2 = 0;
					mode++;	
				}
				if(mode == 3)
				{
					mask_1 = 0x01;
					mask_2 = 0x80;					
				}
				else if(mode == 4)
				{
					rev = 0;
					mask_2_1 = 64;
					mask_1 = 0x01;
					mask_2 = 0x80;
				} 
			
			}
			p2_real_last = P0_2;
			p2_soft_last = p2_soft;
				
		}

		if(!status)
			continue;

		status = 0;
		
		lcd_display(mode);

		switch(mode)
		{
			case 1:

				P2 = ~P2;
				break;

			case 2:

				if(P2 == 0x00 || P2_7 == 1)
				{
					P2 = 0x01;
					break;	
				}
				
				P2 = P2 << 1;
				break;

			case 3:

				if(mask_1 == 0x10)
				{
					mask_1 = mask_1 << 1;
					mask_2 = mask_2 >> 1;
				}
				 
				P2 = mask_1 | mask_2;
				mask_1 = mask_1 << 1;
				mask_2 = mask_2 >> 1;

				if(mask_1 == 0x00)
				{
					mask_1 = 0x02;
					mask_2 = 0x40;
				}
				break;

			case 4:
				
				if(mask_1 == 0x1F)
				{
					rev = 1;
					mask_2 = mask_2 << 2;
					mask_1 = mask_1 >> 2;

				}
				else if(mask_1 == 0x00)
				{
					mask_2 = 0x80;
					mask_2_1 = mask_2 >> 1;
					mask_1 = 0x01;
					P2 = 0x00;
					rev = 0;
					break;
				}

				if(!rev)
				{
					P2 = mask_1 | mask_2;
					mask_1 = (mask_1 << 1) + 1;
					mask_2 = mask_2 | mask_2_1;
					mask_2_1 = mask_2_1 >> 1;
				}
				else
				{
					P2 = mask_1 | mask_2;
					mask_1 = mask_1 >> 1;
					mask_2 = mask_2 << 1;
				}
				break;

			default:
				break;
		}

   }

}