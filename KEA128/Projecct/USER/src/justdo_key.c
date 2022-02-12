/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00            东秦节能二队专用库文件                 00**
**00         队员 ：杨丰辰 武卓然 杨令闻               00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  按键                       00**
**00           修改时间：2018.03.24                    00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

#define init_up G1      //G1
#define init_down G0    //G0
#define init_left G2    //G2
#define init_right E0   //E0
#define init_ok G3       //G3

void key_init(void)
{
  gpio_init(init_up,GPI,0); //UP
  gpio_init(init_down,GPI,0); //DOWN
  gpio_init(init_left,GPI,0); //LEFT
  gpio_init(init_right,GPI,0); //RIGHT
  gpio_init(init_ok,GPI,0); //OK
}

int key_choose(void)
{
   if(gpio_get(init_up) == 1)//说明按键按下
   {
      systick_delay_ms(10);//消抖
      if(gpio_get(init_up) == 1)//确认按键按下
      {
        while(gpio_get(init_up) == 1)//等待按键释放
        return KEY_UP;
      }
   }
   if(gpio_get(init_down) == 1)
   {
      systick_delay_ms(10);
      if(gpio_get(init_down) == 1)
      {
        while(gpio_get(init_down) == 1)
        return KEY_DOWN;
      }  
   }
   if(gpio_get(init_left) == 1)
   {
      systick_delay_ms(10);
      if(gpio_get(init_left) == 1)
      {
        while(gpio_get(init_left) == 1)
        return KEY_LEFT;
      }
   }
   if(gpio_get(init_right) == 1)
   {
      systick_delay_ms(10);
      if(gpio_get(init_right) == 1)
      {
        while(gpio_get(init_right) == 1)
        return KEY_RIGHT;
      }
   }
   if(gpio_get(init_ok) == 1)
   {
      systick_delay_ms(10);
      if(gpio_get(init_ok) == 1)
      {
        while(gpio_get(init_ok) == 1)
        return KEY_CHOOSE;
      } 
   }
   return 0;
}