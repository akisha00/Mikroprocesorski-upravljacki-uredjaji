#include <at89c51rc2.h>

//P0 - ulaz
//P2 - izlaz

/* 

P0_0 - start
P0_1 - stop
P0_2 - program 1
P0_3 - program 2
P0_4 - program 3
P0_5 - program 4
P0_6 - brzina 1
P0_7 - brzina 2

Normalno zatvoreni tasteri
   
*/

unsigned char status = 0;
unsigned char counter1 = 0;
unsigned char counter2 = 0;
unsigned char speed = 0;

void interrupt_handler1() interrupt 3 
{
	
	if(++counter1 == 250)
	{
		counter1 = 0;
		if(!speed)
		{
			if(++counter2 == 16)
			{
				counter2 = 0;
				status = 1;	
			}
	    }
		else
		{
			if(++counter2 == 8)
			{
				counter2 = 0;
				status = 1;
			}
		}
		
	}
		
}

void interrupt_init_T1()
{
	TMOD = 0x20;	 
	TH1 = 0x18;
	TL1 = 0x18;
	EA = 1;
	ET1 = 1;
	TR1 = 1;	
}





void main()
{
	unsigned char mode = 0;
	unsigned char state = 0;
	unsigned char rev;
	unsigned char mask_1;
	unsigned char mask_2;
	unsigned char mask_2_1;

	interrupt_init_T1();

	while(1)
	{
		if(!P0_0)
		{
			state = 1;
		}

		if(!P0_1)
		{
			state = 0;
			P2 = 0x00;
			mode = 0;
			counter2 = 0;
		}

		if(!state)
		{
			continue;
			P2 = 0x00;
			mode = 0;
		}

		if(!P0_6)
		{
			speed = 0;
		}
		else if(!P0_7)
		{
			speed = 1;
		}

		if(!P0_2)
		{
			mode = 1;
			P2 = 0x00;
		}

		if(!P0_3)
		{
			mode = 2;
			P2 = 0x00;	
		}


		if(!P0_4)
		{
			mode = 3;
			mask_1 = 0x01;
			mask_2 = 0x80;
			P2 = 0x00;	
		}

		if(!P0_5)
		{
			mode = 4;
			P2 = 0x00;
			rev = 0;
			mask_2_1 = 64;
			mask_1 = 0x01;
			mask_2 = 0x80;
		}

		if(!status)
		{
			continue;
		}
		status = 0;
				
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