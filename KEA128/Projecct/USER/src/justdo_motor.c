/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  电机                       00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
#define STOPTIME 6000 // 6000*5/1000，30秒以后允许触发停车
#define SPEED_1 300
#define SPEED_2 280

#define VMAX 126
extern uint16_t VOLTAGE;
extern float car_error[15];
extern uint16_t adc_judge_filter[7];
extern uint8_t stop_flag; 
extern uint16_t bbtime;
extern uint8_t huandao_biaozhi_zuo;
extern uint8_t huandao_biaozhi_you;
uint16_t SPEED = 0;
uint16_t speed_set = 0;
uint16_t speed = 0;      
uint16_t deadtime = 0;
uint16_t dy = 0;
uint16_t dy_all = 0;
uint8_t kbistop = 0;
uint8_t stop = 0;
uint8_t ghgflag = 0;
uint32_t i_am_groot = 0;
float speed_error = 0;
float speed_error_last = 0;
float speed_error_last_last = 0;
float speed_out = 0;
uint16_t speed_now = 0;
uint16_t speed_now_filter = 0;
uint8_t S_i = 0;
uint16_t speed_temp[10] = {0};
uint16_t speed_sum = 0;
uint16_t speed_flag = 0;
/***PID***/ 
float speed_p = 4;//2.5
float speed_i = 0.07;//0.07
float speed_d = 0.0;

/**************函数**************/
void motor_init(void)//定义端口
{
  ftm_pwm_init(ftm2,ftm_ch2,11000,0);//电机 端口 D0
  ftm_pwm_init(ftm2,ftm_ch3,11000,0);//电机 端口 D1
  adc_init(ADC0_SE3);//总电压检测
  //干簧管相关
  gpio_init(H3,GPI,0);
  port_pull(H3); //上拉

  //改变if顺序调整优先级
  if(gpio_get(MARK_8) == 1)
    SPEED = SPEED_1;
  else if(gpio_get(MARK_7) == 1)
    SPEED = SPEED_2;
  else
    SPEED = SPEED_2;
} 

void justdo_motor(void)//函数开始
{
  //计时 这个既有用又没用，人生亦如此
  if(i_am_groot<12000)
    i_am_groot++;//WAAAAAA I AM GROOT!!!! （ * - * ）
  else
    i_am_groot = 12000;
  //关翻盖舵机
  if(i_am_groot>400 && i_am_groot<410 )
    ftm_pwm_init(ftm0,ftm_ch1,50,0);
  //第一圈蜂鸣
  if(gpio_get(H3) == 0)
   {
      ghgflag++;
      if(ghgflag >= 2)
      {
        bbtime = 10;
      }
   }
  //第二圈停车
  if(gpio_get(H3) == 0 && i_am_groot >= STOPTIME)
   {
      ghgflag++;//消抖
      if(ghgflag >= 2)
      {
        kbistop = 1;
        bbtime = 1000;
      }
   }
  if(ghgflag >= 2)
    ghgflag = 0;
  
  if(gpio_get(MARK_6) == 1)
  {
    //根据偏差赋值
    if(car_error[0] > 4 || car_error[0] < -4)
    {
      speed_set = (uint16_t)(SPEED/1.1);
    }
    else if(car_error[0] > 7 || car_error[0] < -7)
    {
      speed_set = (uint16_t)(SPEED/1.3);
    }
    else
    {
      speed_set = SPEED;
    }
  }
  else if(gpio_get(MARK_6) == 0)
  {
    speed_set = SPEED;
  }
    
    //入环减速
    //if(huandao_biaozhi_zuo>0)

   
  //停车
  if(kbistop == 0)
  {
    if(stop_flag >= 100)
      stop = 1;
    else
      stop = 0;
  }
  else
    stop = 1;
//电容总电压控制速度
  dy_all = adc_once(ADC0_SE3,ADC_12bit)*10/167;
  dy = adc_once(ADC0_SE2,ADC_12bit)*10/167;
  speed = (speed_set*VMAX)/dy_all;
  
  
//滤波
  speed_temp[S_i] = speed_now;
  //队列
  S_i++;
  if(S_i > 9)
    S_i = 0;
  //累和归零
  speed_sum = 0;
  //累和
  for(uint8_t j=0;j<10;j++)
  {
    speed_sum += speed_temp[j];
  }
  //平均
  speed_now_filter = speed_sum/10;
  
  
  
  //速度偏差
  speed_error = (float)(speed_set - speed_now_filter);
  //pid
  speed_out += speed_p * (speed_error - speed_error_last) + speed_i * speed_error + 
               speed_d*(speed_error - 2*speed_error_last + speed_error_last_last);
  
  speed_error_last_last = speed_error_last;
  speed_error_last = speed_error; 
  
  //限幅
  if(speed_out >= 900)
    speed_out = 900;
  else if(speed_out <= 0)
    speed_out = 1;
  //赋值输出
  speed = (uint16_t)speed_out;  

  if(stop == 0)
  {
    ftm_pwm_duty(ftm2,ftm_ch2,0);
    ftm_pwm_duty(ftm2,ftm_ch3,speed);
  }
  else
  {
    ftm_pwm_duty(ftm2,ftm_ch2,0);
    ftm_pwm_duty(ftm2,ftm_ch3,0);
  }
    
//  //满占空比标志
//  if(speed > 800)
//    speed_flag ++;
//  else
//    speed_flag = 0;
//  if(speed_flag > 200)
//    bbtime = 200;
  
  
  //过冲报警
  if(dy_all > VOLTAGE && speed_now_filter == 0)
    gpio_set(H5,1);
  else
    gpio_set(H5,0);
}


////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
  uint8_t motor_1[20]="";
  uint8_t motor_2[20]="";
  uint8_t motor_3[20]="";
  uint8_t motor_4[20]="";
  uint8_t motor_5[20]="";
  uint8_t motor_6[20]="";
  uint8_t motor_7[20]="";
  extern uint8_t jdq_biaozhi;
void motor_print(void)
{
  sprintf((char*)motor_1,"dy_all:%4d,dy%4d",dy_all,dy);
    OLED_P6x8Str(0,1,motor_1);
  sprintf((char*)motor_2,"speed_set:%4d",speed_set);
    OLED_P6x8Str(0,2,motor_2);
  sprintf((char*)motor_3,"speed_now_f:%5d",speed_now_filter);
    OLED_P6x8Str(0,3,motor_3);
  sprintf((char*)motor_4,"speed:%5d",speed);
    OLED_P6x8Str(0,4,motor_4);
  sprintf((char*)motor_5,"P:%5f,I:%5f",speed_p,speed_i);
    OLED_P6x8Str(0,5,motor_5);
    
  sprintf((char*)motor_7,"jdq:%2d",jdq_biaozhi);
    OLED_P6x8Str(0,7,motor_7);
    
    
   //蓝牙传输
  printf("%c",0x03);
  printf("%c",0xFc);
  printf("%c",(char)(speed_set&0x00ff));
  printf("%c",(char)((speed_set>>8)&0x00ff));
  printf("%c",(char)(speed_now_filter&0x00ff));
  printf("%c",(char)((speed_now_filter>>8)&0x00ff));
  printf("%c",(char)(speed&0x00ff));
  printf("%c",(char)((speed>>8)&0x00ff));

  printf("%c",(char)(dy&0x00ff));
  printf("%c",(char)((dy>>8)&0x00ff));
  printf("%c",0xFc);
  printf("%c",0x03);
}