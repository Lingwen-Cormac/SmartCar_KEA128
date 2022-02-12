/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：   PID                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
extern float car_error[15];
extern float val_out;
float pid_out = 0;
float Servo_Kp = 0;
float Servo_Kd = 0;
float sum_error = 0;
/**************函数**************/
void pd_auto()  //位置式动态PD
{
  //偏差绝对值
  float pid_temp = car_error[0];
  if(car_error[0] < 0)
    pid_temp = 0 - car_error[0];
  //PD赋值
  Servo_Kp = 5 + pow(pid_temp/4.2,1.5);//国赛那信号烂得一匹，老子调了那么久，根本没用
  Servo_Kd = 30;
  pid_out = Servo_Kp*car_error[0] + Servo_Kd*(car_error[0]-car_error[1]);
}


void pd_static()//静态PD，参数未调
{
  //PD赋值
  Servo_Kp = 7; 
  Servo_Kd = 40;
  pid_out = Servo_Kp*car_error[0] + Servo_Kd*(car_error[0]-car_error[1]);
}



////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
  uint8_t pid_1[20]="";
  uint8_t pid_2[20]="";
  uint8_t pid_3[20]="";
  uint8_t pid_4[20]="";
  uint8_t pid_5[20]="";
void pid_print(void)
{
    sprintf((char*)pid_1,"PD:");
    OLED_P6x8Str(0,2,pid_1);
    sprintf((char*)pid_2,"p:%3d,d:%3d",(uint8_t)Servo_Kp,(uint8_t)Servo_Kd);
    OLED_P6x8Str(0,3,pid_2);

  if(car_error[0] >= 0)
    sprintf((char*)pid_3,"ERR[0]: %5d",(uint16_t)car_error[0]);
  else
    sprintf((char*)pid_3,"ERR[0]:-%5d",(uint16_t)(0-car_error[0]));
  OLED_P6x8Str(0,5,pid_3);

  if(car_error[1] >= 0)
    sprintf((char*)pid_4,"ERR[1]: %4d",(uint16_t)car_error[1]);
  else
    sprintf((char*)pid_4,"ERR[1]:-%4d",(uint16_t)(0-car_error[1]));
  OLED_P6x8Str(0,6,pid_4);
  
  if(pid_out >= 0)
    sprintf((char*)pid_5,"OUT:%4d",(uint16_t)pid_out);
  else
    sprintf((char*)pid_5,"OUT:-%4d",(uint16_t)(0-pid_out));
  OLED_P6x8Str(0,7,pid_5);
}