#include "Joystick.h"

unsigned char PotenmeterFlag;

uint32_t AINX, AINY;
void Potenmeter(void)         //4096/2==2048 用来扫描按键值
{
   
    PotenmeterFlag =  AINX > 2500 ? (PotenmeterFlag|0x01) :PotenmeterFlag;
    PotenmeterFlag =  AINX < 1500 ? (PotenmeterFlag|0x02) :PotenmeterFlag;
    PotenmeterFlag =  AINY > 2500 ? (PotenmeterFlag|0x08) :PotenmeterFlag;
    PotenmeterFlag =  AINY < 1500 ? (PotenmeterFlag|0x04) :PotenmeterFlag;

}

void Key_Scan(Button* button)
{
    static unsigned int count;         //长按状态检测
    uint8_t Now;                 //当前状态获取
    Now = !(GetKey);    //获取到当前是否按下
    switch (button->state)               
    {
        case IDLE :   
    
            if (Now)                   //休眠转到按下
            {
                button->state = PRESSED;
            }
        break;
        case PRESSED: 
      
            if (Now)
            {
                if(count++>100)      //长按计时检测
                {
                  
                    button->state = HELD;   //去到长按状态
                    count = 0;
                }
            } 
            else           
            {
                count = 0;
                button->state = RELEASED;  //短按去到松开状态
            }           
        break;
        case HELD : 
            if(!Now)//松开状态
            {
                PotenmeterFlag|=0x20; //置位一次去到空闲状态
                button->state = IDLE;
            } 
            // if(Now)//长按状态
            //     PotenmeterFlag|=0x20; //一直对当前状态置一    注：这是外部要调用的，so我并没有封装起来
            // else 
            //     button->state = IDLE;              //否则就去到空闲状态
        break;
        case RELEASED :
                                
            if(!Now)//松开状态
            {
                PotenmeterFlag|=0x10; //置位一次去到空闲状态
                button->state = IDLE;
            }
        break;
        default:   //未知状态
        break;
    }
}

void Botton_Scan(void)
{
    static Button Keys={0,0};  //定义了两个按键
    unsigned char i;
    Key_Scan(&Keys);
}

