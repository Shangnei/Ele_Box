#ifndef U8G2_D_H
#define U8G2_D_H


// Include necessary standard libraries
#include "main.h"

#define OLED_RST_Clr() PINS_GPIO_WritePin(GPIOB,0,0)//RES
#define OLED_RST_Set() PINS_GPIO_WritePin(GPIOB,0,1)

#define OLED_DC_Clr()  PINS_GPIO_WritePin(GPIOA,6,0)//DC
#define OLED_DC_Set()  PINS_GPIO_WritePin(GPIOA,6,1)
 		     
#define OLED_CS_Clr()  PINS_GPIO_WritePin(GPIOC,14,0)//CS
#define OLED_CS_Set()  PINS_GPIO_WritePin(GPIOC,14,1)

#define OLED_CMD  0
#define OLED_DATA 1

typedef struct 
{
    signed int min;
    signed int max;
    signed int time;
}Ele;


extern unsigned char Lin_buff[3][10];

void OLED_Init(void);
void u8g2_init(void);
void  Menu_Show(void);


#endif 