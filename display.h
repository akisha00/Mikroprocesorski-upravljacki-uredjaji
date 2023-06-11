#ifndef DISPLAY
#define DISPLAY

#define LCD_EN P3_2
#define LCD_RS P3_3

#define LCD_D7 P1_0
#define LCD_D6 P1_1
#define LCD_D5 P1_2
#define LCD_D4 P1_3

#define LCD_BL P1_4

unsigned char line = 1;


void delay_milisecond(void){ 
	int i, j;
	for(i = 0; i < 250; i++){	 	
		for(j = 0; j < 200; j++){  		
		}
	}	
}

void delay_microsecond(void){  
	int i;
	for(i = 0; i < 100; i++){  	
	}	   
}


void cmd_2_disp(unsigned char d7, unsigned char d6, unsigned char d5, unsigned char d4)
{
	LCD_RS = 0;
	LCD_EN = 1;
	LCD_D7 = d7;
	LCD_D6 = d6;
	LCD_D5 = d5;
	LCD_D4 = d4;	
	LCD_EN = 0;	
}


void init_disp()
{		
	LCD_BL = 0;   
	delay_milisecond();
	cmd_2_disp(0,0,1,1);
	cmd_2_disp(0,0,1,1);
	delay_milisecond();
	cmd_2_disp(0,0,1,1);
	delay_microsecond();
	cmd_2_disp(0,0,1,0);
	cmd_2_disp(0,0,1,0);
	cmd_2_disp(1,0,0,0);
	delay_microsecond();
	cmd_2_disp(0,0,0,0);
	cmd_2_disp(1,1,1,1);
	delay_microsecond();
	cmd_2_disp(0,0,0,0);
	cmd_2_disp(0,0,0,1);
	delay_microsecond();
	cmd_2_disp(0,0,0,0);
	cmd_2_disp(0,1,1,0);
	delay_milisecond();	 
	LCD_BL = 1;
}

void char_2_disp(char character)
{
	LCD_RS = 1;
	LCD_EN = 1;
	LCD_D7 = character&0x80;
	LCD_D6 = character&0x40;
	LCD_D5 = character&0x20;
	LCD_D4 = character&0x10;	
	LCD_EN = 0;	

	LCD_EN = 1;
	LCD_D7 = character&0x08;
	LCD_D6 = character&0x04;
	LCD_D5 = character&0x02;
	LCD_D4 = character&0x01;	
	LCD_EN = 0;	

	delay_microsecond();
	delay_microsecond();
}

void str_2_disp(char* ptr)
{
 	char *ptr1 = ptr;
	while(*ptr1)
	{
		char_2_disp(*ptr1);
		ptr1++;	
	}
}

void clear_disp()
{
	cmd_2_disp(0, 0, 0, 0);
	cmd_2_disp(0, 0, 0, 1);
	delay_milisecond();
}

void set_DDRAM_addr_disp(unsigned char address)
{
	if(address == 0x00)
	{
		line = 1;
	}
	else
	{
		line = 2;
	}
	cmd_2_disp(1, address&0x40, address&0x20, address&0x10);
	cmd_2_disp(address&0x04, address&0x02, address&0x01, 1);
	delay_milisecond();
}


#endif 