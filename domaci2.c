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

void delay_1s()
{
	int i,j = 0;
	for(i = 0; i < 200; i++)
		for(j = 0; j < 340; j++);
}

void delay_2s()
{
	int i, j = 0;
	for(i = 0; i < 400; i++)
		for(j = 0; j < 340; j++);	
}

void main()
{
	unsigned char mode = 0;
	unsigned char speed = 0;
	unsigned char state = 0;
	unsigned char rev;
	unsigned char mask_1;
	unsigned char mask_2;
	unsigned char mask_2_1;

	while(1)
	{
		if(!P0_0)
		{
			state = 1;
		}

		if(!P0_1)
		{
			state = 0;
		}

		if(!state)
		{
			continue;
		}

		if(P0_6 == 0)
		{
			speed = 0;
		}
		else if(P0_7 == 0)
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

		if(!speed)
			{
				delay_1s();
			}
			else
			{
				delay_2s();
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