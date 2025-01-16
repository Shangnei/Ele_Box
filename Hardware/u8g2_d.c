#include "u8g2_d.h"

unsigned char OLED_GRAM[144][8];
/*
	给到外部电流的数据
*/
Ele Current[3];
char Ele_Buff[100];
unsigned char Lin_buff[3][10]={0};
//XBMP图标数据 ，定义一个大的数组直接扫描所有的图片 通过位移x，来达到运动效果
 unsigned char ICON[][300] = 
 {
    {
        0xF0,0xFF,0xFF,0xFF,0xFF,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x03,0xFE,0xFF,0x00,0x00,0xFE,0x07,0xFE,0x7F,0x00,0x00,0xFE,0x07,0xFF,0x7F,0x00,0x00,0xFF,0x0F,0xFF,0x3F,
        0x00,0x80,0xFF,0x0F,0xFF,0x3F,0x00,0x80,0xFF,0x0F,0xFF,0x3F,0x00,0xC0,0xFF,0x0F,0xFF,0x1F,0x00,0xC0,0xFF,0x0F,0xFF,0x1F,0x00,0xE0,0xFF,0x0F,0xFF,0x1F,0x00,0xE0,
        0xFF,0x0F,0xFF,0x0F,0x00,0xF0,0xFF,0x0F,0xFF,0x0F,0x00,0xF0,0xFF,0x0F,0xFF,0x07,0x00,0xF8,0xFF,0x0F,0xFF,0x07,0x00,0xF8,0xFF,0x0F,0xFF,0x07,0x00,0xFC,0xFF,0x0F,
        0xFF,0x03,0x00,0x00,0xE0,0x0F,0xFF,0x03,0x00,0x00,0xF0,0x0F,0xFF,0x01,0x00,0x00,0xF0,0x0F,0xFF,0x01,0x00,0x00,0xF8,0x0F,0xFF,0x01,0x00,0x00,0xFC,0x0F,0xFF,0x00,
        0x00,0x00,0xFE,0x0F,0xFF,0x00,0x00,0x00,0xFF,0x0F,0xFF,0x00,0x00,0x80,0xFF,0x0F,0x7F,0x00,0x00,0x80,0xFF,0x0F,0xFF,0xFF,0x03,0xC0,0xFF,0x0F,0xFF,0xFF,0x07,0xE0,
        0xFF,0x0F,0xFF,0xFF,0x07,0xF0,0xFF,0x0F,0xFF,0xFF,0x07,0xF8,0xFF,0x0F,0xFF,0xFF,0x03,0xF8,0xFF,0x0F,0xFF,0xFF,0x03,0xFC,0xFF,0x0F,0xFF,0xFF,0x03,0xFE,0xFF,0x0F,
        0xFF,0xFF,0x01,0xFF,0xFF,0x0F,0xFF,0xFF,0x81,0xFF,0xFF,0x0F,0xFF,0xFF,0x81,0xFF,0xFF,0x0F,0xFF,0xFF,0xC0,0xFF,0xFF,0x0F,0xFF,0xFF,0xE0,0xFF,0xFF,0x0F,0xFF,0xFF,
        0xF0,0xFF,0xFF,0x0F,0xFF,0xFF,0xF8,0xFF,0xFF,0x0F,0xFF,0x7F,0xFC,0xFF,0xFF,0x0F,0xFE,0x7F,0xFE,0xFF,0xFF,0x07,0xFE,0x7F,0xFF,0xFF,0xFF,0x07,0xFC,0xFF,0xFF,0xFF,
        0xFF,0x03,0xF0,0xFF,0xFF,0xFF,0xFF,0x00
    },//ele
    {
        0xF0,0xFF,0xFF,0xFF,0xFF,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0x03,0xFE,0xFF,0xFF,0xFF,0xFF,0x07,0xFE,0xFF,0xFF,0xFF,0xFF,0x07,0x1F,0x00,0x00,0x00,0x00,0x0F,0x1F,0x00,
        0x00,0x00,0x00,0x0E,0xC7,0xFF,0xFF,0xFF,0xFF,0x0C,0xE7,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,
        0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,
        0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0x01,0x00,0x00,0xF0,0x0C,0xF3,0x00,0x00,0x00,0xF8,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,
        0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0x01,0x00,0xF8,0xFF,0x0C,0xF3,0x00,0x00,0xFC,
        0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,0xF3,0xFF,0xFF,0xFF,0xFF,0x0C,
        0xE7,0xFF,0xFF,0xFF,0x7F,0x0E,0x0F,0xF0,0x07,0x00,0x00,0x0F,0x1F,0xF0,0x07,0x00,0x80,0x0F,0xFF,0xE7,0xF3,0xFF,0xFF,0x0F,0xFF,0x8F,0xF9,0xFF,0xFF,0x0F,0xFF,0x1F,
        0xFC,0xFF,0xFF,0x0F,0xFF,0x3F,0xFE,0xFF,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0xFE,0xFF,0xFF,0xFF,0xFF,0x07,0xFE,0xFF,0xFF,0xFF,0xFF,0x07,0xFC,0xFF,0xFF,0xFF,
        0xFF,0x03,0xF0,0xFF,0xFF,0xFF,0xFF,0x00
    }//lin
};



u8g2_t u8g2;   //u8g2结构体

//用来做pid的核心函数?   speed控制速度   ，c_speed保障震荡
int run_str(int *now,int *trag,const int speed,const int c_speed)
{
	int temp = 0;
	if (*now > *trag)
	{
		temp = ((*now - *trag) > c_speed) ? speed : c_speed;
		*now -= temp;
	}
	else if (*now < *trag)
	{
		temp = ((*trag - *now) > c_speed) ? speed : c_speed;
		*now += temp;
	}
	else 
	{
		return 1;
	}
	return 0;
}

unsigned char Move(int *Main_Menu_x_taget,int *Main_Menu_y_taget,unsigned char x, unsigned char y)
{
	if(GetLeft)                                 
	{
		ClrLeft;
		*Main_Menu_x_taget+=x;
		PRINTF("Key Main_Menu_x_taget  %d\n",*Main_Menu_x_taget );
		return 2;
	}
	if(GetRight)
	{
		ClrRight;
		*Main_Menu_x_taget-=x;
		PRINTF("Key Main_Menu_x_taget  %d\n",*Main_Menu_x_taget );
		return 3;
	}
	if(GetUp)
	{
		ClrUp;
		*Main_Menu_y_taget-=y;
		PRINTF("Key Main_Menu_y_taget  %d\n",*Main_Menu_y_taget );
	}
	if(GetDown)
	{
		ClrDown;
		*Main_Menu_y_taget+=y;
		PRINTF("Key Main_Menu_y_taget  %d\n",*Main_Menu_y_taget );
	}
	if(GetOk)
	{
		ClrOk;
		return 1;
	}
	return 0;
}
void  Menu_Show(void)//显示菜单  这里显示了所有的图片
{
 	signed char Value;										 //每次的增量值
	unsigned char x,y,ret;									    //用来确定当前位置
	
	static unsigned char MenuFlag=0,CtrlFlag=0;           //两个都菜单选择变量
	static int Main_Menu_x = 42,Main_Menu_x_taget = 42,Main_Menu_y = 0,Main_Menu_y_taget = 0,Main_Menu_x_1 = 0;// PID控制的位置变量
    unsigned char i ;
	switch (MenuFlag)
	{
	case 0:
		for(i=0;i<2;i++)
		{
			u8g2_DrawXBMP(&u8g2,Main_Menu_x+i*128,10,44,44,ICON[i]);
		}   
		if(GetRight)
		{
			ClrRight;
			Main_Menu_x_taget +=128;
			Main_Menu_x_taget = Main_Menu_x_taget>42?42:Main_Menu_x_taget; //限位
			PRINTF("Key one %d\n",Main_Menu_x_taget);
			
		}
		if(GetLeft)
		{
			ClrLeft;
			Main_Menu_x_taget -=128;
			Main_Menu_x_taget = Main_Menu_x_taget<-86?-86:Main_Menu_x_taget;
			PRINTF("Key two  %d\n",Main_Menu_x_taget );
			
		}
		if(GetOk)
		{
			ClrOk;
			
			MenuFlag = (unsigned char)((42-Main_Menu_x_taget)/128)+1;
			Main_Menu_x = 0,Main_Menu_x_taget = 0,Main_Menu_y = 0,Main_Menu_y_taget = 18;
			PRINTF("Key two  %d\n",MenuFlag );
		}
		
		break;
	case 1:
		
        u8g2_DrawUTF8(&u8g2,14,12,"最小");
        u8g2_DrawUTF8(&u8g2,50,12,"最大");
        u8g2_DrawUTF8(&u8g2,86,12,"时间");
		for(i=0;i<3;i++)
		{
			sprintf(Ele_Buff,"%4d  %4d  %4d" ,Current[i].min,Current[i].max,Current[i].time);
			u8g2_DrawUTF8(&u8g2,10,25+(i*12),Ele_Buff);
		}
        
		if(GetLOk)   //这里是返回键
		{
			ClrLOk;
			MenuFlag = 0;
			CtrlFlag = 0;
			Main_Menu_x_taget = 42;
		}
		/*	
			反算出当前所在的位置
			这里是进行数值加减的地方
		*/
		if(CtrlFlag)
		{
			y = (Main_Menu_y_taget-22)/10;    
			x = (Main_Menu_x_taget-6)/36;
			Value = AINX>2500?1:AINX<1500?-1:0;
			u8g2_DrawRFrame(&u8g2,Main_Menu_x,Main_Menu_y,31,12,2);

			if(GetOk)
			{
				ClrOk;
				CtrlFlag= 0;
				
			}
			
			switch (x)//这里是对电流配置的加减
			{
				case 0:
					Current[y].min+=Value;
					Current[y].min = Current[y].min <= 0 ?0:Current[y].min;
					Current[y].min = Current[y].min >= 5000 ?5000:Current[y].min;
					break;
				case 1:
					Current[y].max+=Value;
					Current[y].max = Current[y].max <= 0 ?0:Current[y].max;
					Current[y].max = Current[y].max >= 5000 ?5000:Current[y].max;
					break;
				case 2:
					Current[y].time+=Value;
					Current[y].time = Current[y].time <= 0 ?0:Current[y].time;
					Current[y].time = Current[y].time >= 50 ?50:Current[y].time;
					break;
				default:
					break;
			}
		
			PRINTF(" Value %d  AINX %d\n",Value, AINY );
			break;
		}
		else
		{
			/*
				这里是对菜单的移动
			*/
			if(Move(&Main_Menu_x_taget,&Main_Menu_y_taget,36,12)==1)
				CtrlFlag = 1;
			u8g2_DrawTriangle(&u8g2,Main_Menu_x,Main_Menu_y,Main_Menu_x,Main_Menu_y+8,Main_Menu_x+4,Main_Menu_y+4);
			// u8g2_DrawBox(&u8g2,Main_Menu_x,Main_Menu_y,2,10);
		
		}
		Main_Menu_x_taget = Main_Menu_x_taget<6?6:Main_Menu_x_taget;
		Main_Menu_x_taget = Main_Menu_x_taget>78?78:Main_Menu_x_taget;
		Main_Menu_y_taget = Main_Menu_y_taget<12?12:Main_Menu_y_taget;
		Main_Menu_y_taget = Main_Menu_y_taget>60?60:Main_Menu_y_taget;
		break;
	case 2:  //这里是Lin的菜单
		u8g2_DrawUTF8(&u8g2, 3,12,"ID");
        u8g2_DrawUTF8(&u8g2,21,12,"时间");
		u8g2_DrawUTF8(&u8g2,60,12,"报文");
		if(GetLOk)
		{
			ClrLOk;
			MenuFlag = 3;
		}
		for(i=0;i<3;i++)
		{
			sprintf(Ele_Buff,"%3d%3d%3d%3d%3d%3d%3d%3d%3d%3d" ,Lin_buff[i][0],Lin_buff[i][1],Lin_buff[i][2],Lin_buff[i][3],Lin_buff[i][4],Lin_buff[i][5],Lin_buff[i][6],Lin_buff[i][7],Lin_buff[i][8],Lin_buff[i][9]);	
			u8g2_DrawUTF8(&u8g2,Main_Menu_x_1,34+(i*12),Ele_Buff);
			
		}
		if(CtrlFlag == 0)
		{
		
			
			// u8g2_DrawBox(&u8g2,Main_Menu_x,Main_Menu_y,2,10);
			u8g2_DrawLine(&u8g2,Main_Menu_x,Main_Menu_y,Main_Menu_x+5,Main_Menu_y);
			if(Main_Menu_x_taget<60)
				ret = Move(&Main_Menu_x_taget,&Main_Menu_y_taget,10,12);
			else                         //这里是配合我横线的移动
			{
				ret = Move(&Main_Menu_x_taget,&Main_Menu_y_taget,5,12);
				if(Main_Menu_x_taget<120&&Main_Menu_x_taget>60) //为了做限位不然一直跑
				{
					if(ret == 3)
						Main_Menu_x_1 += 5;
					if(ret == 2)
						Main_Menu_x_1 -= 5;
				}
				
			}
			if(ret==1)
			{
				CtrlFlag = 1;
			}
		
		}
		else
		{
			x = Main_Menu_x_taget/12;
			y = (Main_Menu_y_taget-35)/12;	
			if(GetUp)
			{
				ClrUp;
				Lin_buff[y][x] += 1;
				PRINTF("Key Main_Menu_y_taget  %d\n",Main_Menu_y_taget );
			}
			if(GetDown)
			{
				ClrDown;
				Lin_buff[y][x] -= 1;
				PRINTF("Key Main_Menu_y_taget  %d\n",Main_Menu_y_taget );
			}
			if(GetOk)
			{
				ClrOk;
				CtrlFlag = 0;
			}
		}
		
		/*
			限位
		// */
	
		PRINTF("  AINX %d\n",Main_Menu_x_taget );
		Main_Menu_x_taget = Main_Menu_x_taget<6?6:Main_Menu_x_taget;
		Main_Menu_x_taget = Main_Menu_x_taget>120?120:Main_Menu_x_taget;
		Main_Menu_y_taget = Main_Menu_y_taget<35?35:Main_Menu_y_taget;
		Main_Menu_y_taget = Main_Menu_y_taget>59?59:Main_Menu_y_taget;
		break;
		case 3:
			sprintf(Ele_Buff,"%f ",Current_vlue) ;
			u8g2_DrawUTF8(&u8g2,50,34,Ele_Buff);
	default:
		break;
	}
    
    /*
		这里是位置的移动解算，以及刷新
	*/

    run_str(&Main_Menu_x,&Main_Menu_x_taget,3,1);   //每次进行运算以一定的速度靠近X_taget
	run_str(&Main_Menu_y,&Main_Menu_y_taget,3,1);   //每次进行运算以一定的速度靠近Y_taget
	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
    
    
}





/* USER CODE BEGIN PD */
uint8_t u8g2_gpio_and_delay_YTM(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg){
		//Initialize SPI peripheral
		case U8X8_MSG_GPIO_AND_DELAY_INIT:
			/* HAL initialization contains all what we need so we can skip this part. */

		break;

		//Function which implements a delay, arg_int contains the amount of ms
		case U8X8_MSG_DELAY_MILLI:
		OSIF_TimeDelay(arg_int);

		break;
		//Function which delays 10us
		case U8X8_MSG_DELAY_10MICRO:
		for (uint16_t n = 0; n < 320; n++)
		{
			__NOP();
		}

		break;
		//Function which delays 100ns
		case U8X8_MSG_DELAY_100NANO:
		__NOP();

		break;
		// Function to define the logic level of the CS line
		case U8X8_MSG_GPIO_CS:
			if (arg_int) OLED_CS_Set();
			else OLED_CS_Clr() ;

		break;
		//Function to define the logic level of the Data/ Command line
		case U8X8_MSG_GPIO_DC:
			if (arg_int) OLED_DC_Set() ;
			else OLED_DC_Clr();

		break;
		//Function to define the logic level of the RESET line
		case U8X8_MSG_GPIO_RESET:
			if (arg_int) OLED_RST_Set();
			else OLED_RST_Clr();

		break;
		default:
			return 0; //A message was received which is not implemented, return 0 to indicate an error
	}

	return 1; // command processed successfully.
}

void u8g2_init(void)
{
	
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_4write_hw_spi, u8g2_gpio_and_delay_YTM);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2,0);
   	u8g2_SetFontDirection(&u8g2, 0);
	u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese3);
    u8g2_ClearBuffer(&u8g2);

}

void OLED_WR_Byte(unsigned char dat,unsigned char cmd)
{	
	unsigned char i;			  
	if(cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
	OLED_CS_Clr();
	SPI_DRV_MasterTransfer(SPI_INST, &dat, NULL, 1);			 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
}

void OLED_Refresh(void)
{
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD);
	   OLED_WR_Byte(0x00,OLED_CMD);   
	   OLED_WR_Byte(0x10,OLED_CMD);   
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}

void OLED_Clear(void)
{
	unsigned char i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;
			}
  }
	OLED_Refresh();
}

void OLED_Init(void)
{
	
	
	
	OLED_RST_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction  
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
}