/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  舵机                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
extern float pid_out;
extern float car_error[15];
#define servo_middle 660  //servo_middle舵机中位值 660
uint16_t servo_max = 140; //定义偏转最大值为12
int32_t angle = 0;
uint16_t servo_now = servo_middle;//660

/**************函数**************/
void servo_print(void);

//舵机定义
void servo_init(void)
{
  ftm_pwm_init(ftm0,ftm_ch0,50,servo_middle);
}

//舵机主程序
void justdo_servo(void)
{
  //PD系数
  pd_auto();     //May the force be with you
  //pd_static(); //翻译：别太放肆，没什么用
  //偏转赋值
  angle = (int32_t)pid_out;
  //限幅 servo_max为偏转最大值
  if(angle > servo_max)
    angle = servo_max;
  if(angle < -servo_max)
    angle = -servo_max;
  //舵机中位值和转向值加和
  servo_now = servo_middle + angle;
  //赋值给舵机  
  ftm_pwm_duty(ftm0,ftm_ch0,servo_now);
  //更新偏转最大值
  servo_max = servo_max;     //servo_max偏转最大值
  //更新偏差
  uint8_t i;
  for(i=0;i<7;i++)
  car_error[i+1] = car_error[i];
}


////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
  uint8_t servo_1[20]="";
  uint8_t servo_2[20]="";
void servo_print(void)
{
    sprintf((char*)servo_1,"angle:%4d",angle);
    OLED_P6x8Str(0,1,servo_1);
    sprintf((char*)servo_2,"servo_now:%4d",servo_now);
    OLED_P6x8Str(0,2,servo_2);
}
