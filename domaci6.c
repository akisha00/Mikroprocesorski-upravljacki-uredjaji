#include <at89c51rc2.h>


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

void interrupt_init_T1_T0()
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
	
	EA = 1;	
}



void main()
{
	unsigned char mode = 0;
	unsigned char state = 0;
	unsigned char rev;
	unsigned char mask_1;
	unsigned char mask_2;
	unsigned char mask_2_1;
	unsigned char counter = 0;

	interrupt_init_T1_T0();

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
		
		if(!state )
		{
			P2 = 0;
			mode = 0;
			continue;	
		}

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