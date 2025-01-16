#ifndef JOYSTICK_H
#define JOYSTICK_H

#include"main.h"

#define ADC_INST    (0) /* ADC instance */

#define GetKey   ((PINS_GPIO_ReadPins(GPIOB))&0x08)   // 获取按键状态，返回值为0或1

#define GetUp    (PotenmeterFlag&0x01)             //位的形式获取上下左右 ，长按就通过多次进入处理
#define GetDown  (PotenmeterFlag&0x02)             //每次获取要记得清除。我逻辑就是这莫写的
#define GetLeft  (PotenmeterFlag&0x04)
#define GetRight (PotenmeterFlag&0x08)
#define GetOk    (PotenmeterFlag&0x10)
#define GetLOk   (PotenmeterFlag&0x20)

#define ClrUp    (PotenmeterFlag&=(0xFE))
#define ClrDown  (PotenmeterFlag&=(0xFD))
#define ClrLeft  (PotenmeterFlag&=(0xFB))
#define ClrRight (PotenmeterFlag&=(0xF7))
#define ClrOk    (PotenmeterFlag&=(0xEF))
#define ClrLOk   (PotenmeterFlag&=(0xDF))

typedef enum {
    IDLE,                   // 空闲状态
    PRESSED,                // 按下状态
    HELD,                   // 持续按下状态
    RELEASED                // 释放状态
} Key_State;

typedef struct {
    Key_State state;        // 按键状态
    unsigned char BottonNum; // 按键编号
} Button;

extern unsigned char PotenmeterFlag;     //你猜猜这里为什么要加extern
extern uint32_t AINX, AINY;
void Potenmeter(void);
void Botton_Scan(void);

#endif
