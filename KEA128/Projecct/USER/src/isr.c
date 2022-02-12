/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		中断文件
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/



#include "isr.h"

 uint16_t SD=0;
 uint8_t return_flag=0;
 uint32 v_us=2000;
 uint32 v_pr=0; 
 uint8 JY=0; 
 extern int stopp;
 uint16 pwm_dianji,pwm_duoji,pwm_pr;
 char ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch_beg; 
 unsigned char c1,c2,c3,c4,c5,summ;
 
void speed_filiter(void);
void motor_pid(uint16_t speed_set,uint16_t speed_get);
#define MOTOR_KP  1.1  //1.1   //0.45 
#define MOTOR_KI  0.08 //0.08  //0.04
#define MOTOR_KD  0.02 //0.02  //0.12
#define MOTOR_MAX 1700
#define MOTOR_MIN 1500
#define MOTOR_STOP 1500
#define DEEP 10

//#define MOTOR_MIN 1200
float speed_error = 0;
float speed_error_last = 0;
float speed_error_last_last = 0;
float result = 0;
uint8_t  S_i = 0;

uint32_t speed_temp[DEEP] = {0};
uint16_t speed_sum = 0;
uint16_t speed_flag = 0;
uint16_t pwm_get_filter = 0;
uint16_t motor_out = 1500;
uint16_t pwm_get_filter_last = 0;
uint16_t pwm_return=0;
 
void PIT_CH0_IRQHandler(void)
{
    PIT_FLAG_CLR(pit0);
}


void PIT_CH1_IRQHandler(void)
{
    PIT_FLAG_CLR(pit1);
    pwm_pr=(uint16_t)((18691.6/v_us+54.1109)/0.0348);//测定值
    //测定值限幅
    if(pwm_pr >= 1800)
      pwm_pr = 1800;
    else if(pwm_pr <= 1500)
      pwm_pr = 1500;
    
    if(pwm_dianji > 2200)
        pwm_dianji = 2200;
    //手动开关PID
    if(pwm_dianji>1800)
    {
      ftm_pwm_duty(ftm2,ftm_ch2,(pwm_dianji-300));
    }
    else if(pwm_dianji > 1500)
    {
      //滤波
      //speed_filiter();
      //pid控制
      if(pwm_dianji < MOTOR_MIN)
        pwm_dianji = MOTOR_MIN;
      motor_pid(pwm_dianji,pwm_pr);
      ftm_pwm_duty(ftm2,ftm_ch2, motor_out);
    }
    else
      ftm_pwm_duty(ftm2,ftm_ch2,pwm_dianji);

    //开环
    //ftm_pwm_duty(ftm2,ftm_ch2,pwm_dianji);
////限幅
//  printf("%c",0x03);
//  printf("%c",0xFc);
//  printf("%c",(char)(pwm_dianji&0x00ff));
//  printf("%c",(char)((pwm_dianji>>8)&0x00ff));
//  printf("%c",(char)(pwm_pr&0x00ff));
//  printf("%c",(char)((pwm_pr>>8)&0x00ff));
//  printf("%c",(char)(motor_out&0x00ff));
//  printf("%c",(char)((motor_out>>8)&0x00ff));
//  if(speed_error>=0)
//  {
//    printf("%c",(char)((uint16_t)(speed_error)&0x00ff));
//    printf("%c",(char)(((uint16_t)(speed_error)>>8)&0x00ff));
//  }
//  else
//  {
//    printf("%c",(char)((uint16_t)(-speed_error)&0x00ff));
//    printf("%c",(char)(((uint16_t)(-speed_error)>>8)&0x00ff));
//  }//
//  printf("%c",(char)((uint16_t)(pwm_duoji)&0x00ff));
//  printf("%c",(char)(((uint16_t)(pwm_duoji)>>8)&0x00ff));  
//  
//  
//  printf("%c",0xFc);
//  printf("%c",0x03);
}

void IRQ_IRQHandler(void)
{
    CLEAR_IRQ_FLAG;
}


void KBI0_IRQHandler(void)
{

//  
  CLEAN_KBI0_FLAG;
    
}


void KBI1_IRQHandler(void)
{
 //return_flag=1;
  SD++;
  if(SD==1)  pit_time_start(pit0);
  v_us=pit_time_get(pit0)*1000/bus_clk_khz;
  if(v_us!=0)  v_pr=953271/v_us;
 // if(v_us!=0)v_pr=460000/v_us;
  JY=v_pr%7+v_pr/7;
  if(v_pr<20)  v_pr=0;
  
  
  uart_putchar(uart0,0x00);
  uart_putchar(uart0,v_pr);
  uart_putchar(uart0,JY);
  pit_time_start(pit0);
  CLEAN_KBI1_FLAG;
    
    
}


void UART2_IRQHandler (void)
{
 
  stopp++;
}




void speed_filiter()
{
  //滤波
  speed_temp[S_i] = pwm_pr;
  //队列
  S_i++;
  if(S_i > (DEEP-1))
    S_i = 0;
  //累和归零
  speed_sum = 0;
  //累和
  for(uint8_t j=0;j<DEEP;j++)
  {
    speed_sum += speed_temp[j];
  }
  //pingjun
  pwm_get_filter = speed_sum/DEEP;
  if(pwm_get_filter < 1500)
    pwm_get_filter = 1500;
  //更新
  pwm_get_filter_last = pwm_get_filter;
}




void motor_pid(uint16_t speed_set,uint16_t speed_get)
{
  //偏差
  speed_error = speed_set - speed_get;
  //增量式pid
  result +=  MOTOR_KP * (speed_error - speed_error_last)
             + MOTOR_KI * speed_error
             + MOTOR_KD * (speed_error - 2*speed_error_last + speed_error_last_last);
  //更新偏差
  speed_error_last_last = speed_error_last;
  speed_error_last = speed_error; 
        
  //限幅
  if(result >= MOTOR_MAX)
    result = MOTOR_MAX;
  else if(result <= MOTOR_STOP)
    result = MOTOR_STOP;
  motor_out = (uint16_t)result;  
}



/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了周期定时器 通道0得中断
void PIT_CH0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位

FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/



