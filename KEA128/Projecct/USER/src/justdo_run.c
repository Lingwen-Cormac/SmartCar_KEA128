/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用： 运行函数                    00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
extern uint16_t adc_get[8]; 
extern uint16_t adc_judge_filter[8];
extern float car_error[15];
extern float guiyi_left;
extern float guiyi_right;
extern uint16_t speed;
extern uint16_t speed_set;
extern uint8_t stop;
extern float Servo_Kp;
extern float Servo_Kd;
extern uint16_t servo_now;
extern uint8_t kbistop;
extern uint16_t dy_all;
extern uint16_t dy;
extern uint16_t huandao_biaozhi;
extern uint16_t huandao_open_again;
extern uint8_t jdq_biaozhi;
uint8_t txt_bingo[30]="";
uint8_t txt_bingo_1[30]="";
uint8_t m_1[30]="";
uint8_t m_2[30]="";
uint8_t m_3[30]="";
uint8_t j_1[30]="";
uint8_t j_2[30]="";
uint8_t j_3[30]="";
uint8_t mk1,mk2,mk3,huandao_dangwei;
uint16_t VOLTAGE = 0;
//  uint16_t pwm_begin = 3000;//（恒功率）
  uint16_t adc_get_max = 0;
  uint16_t adc_get_now[10] = {0};
  

/**************函数**************/
void gogogo_run(void)
{
  uint32_t adc_get_sum = 0;
  extern uint16_t adcmax[8]; 
  
////启动能量,闭环使用（恒功率）
//  uint8_t j = 0;
//  for(j=250;j>0;j--)
//  {
//    ftm_pwm_duty(ftm1,ftm_ch1,pwm_begin+=5);
//    systick_delay_ms(10);
//  }  
  
  
//总电压检测
  adc_init(ADC0_SE3);
  dy_all = adc_once(ADC0_SE3,ADC_12bit)*10/167;
//单组电压检测  
  adc_init(ADC0_SE2);
  dy = adc_once(ADC0_SE2,ADC_12bit)*10/167;
  
//启动电压在这里修改
  if(gpio_get(MARK_5) == 1)
    VOLTAGE = 115;
  if(gpio_get(MARK_6) == 1)
    VOLTAGE = 120;
//判环挡位
   if(gpio_get(MARK_4) == 1)
     huandao_dangwei = 1;
   else
     huandao_dangwei = 0;
//充电循环检测
  adc_init(ADC0_SE8);
  uint16_t adc_i = 0;
  uint8_t dy_biaozhi = 0;
  while(dy_all<=VOLTAGE || dy_biaozhi<5)
  {      
    dy_all = adc_once(ADC0_SE3,ADC_12bit)*10/167;
    dy = adc_once(ADC0_SE2,ADC_12bit)*10/167;
    
    if(dy_all >= VOLTAGE)
    {
      dy_biaozhi ++;
    }
    
    sprintf((char*)txt_bingo_1,"dy_all:%3d",dy_all);
    OLED_P8x16Str(0,2,txt_bingo_1);
    sprintf((char*)txt_bingo,"dy:%3d %",(uint8_t)(((float)dy/(float)VOLTAGE)*100));
    OLED_P8x16Str(0,0,txt_bingo);
    
    //环岛方向档位
    mk1 = gpio_get(MARK_1);
    mk2 = gpio_get(MARK_2);
    mk3 = gpio_get(MARK_3);
    sprintf((char*)j_2,"huandao:%3d,%3d,%3d",mk1,mk2,mk3);
    OLED_P6x8Str(0,5,j_2);
    //电量挡位
    sprintf((char*)j_3,"voltage:%4d",VOLTAGE);
    OLED_P6x8Str(0,6,j_3);
    //计时
    sprintf((char*)m_3,"TIME:%4d",adc_i);
    OLED_P6x8Str(0,7,m_3);
    
    //无线充电（恒功率）
    //justdo_power();
  }
  
//pwm归零（恒功率）
//  ftm_pwm_duty(ftm1,ftm_ch1,0);
//清屏
  OLED_Fill(0x00);
//收起支架
  ftm_pwm_init(ftm0,ftm_ch1,50,520);
  servo_init();//舵机定义
//闭合继电器  
  gpio_set(H7,0);
  
//电感自动标定
  systick_delay_ms(150);
  uint8_t i = 0;
  uint8_t j = 0;
  //循环检测，输入队列
  for(j=10;j>0;j--)//检测次数
  {
    adc_get_now[i] = adc_once(DG6,ADC_12bit);
    i ++;
    if(i >= 10)
      i = 0;
    systick_delay_ms(10);
  }
  //队列求和
  for(i=0;i<10;i++)
    adc_get_sum += adc_get_now[i];
  //求平均
  adc_get_max = adc_get_sum/10;
  
  //计算各电感最大值 更换赛道环境应重新标定
  adcmax[0] = (uint16_t)(adc_get_max * 0.861);//2170
  adcmax[1] = (uint16_t)(adc_get_max * 0.861);//2170
  adcmax[2] = (uint16_t)(adc_get_max * 1.024);//2580
  adcmax[3] = (uint16_t)(adc_get_max * 1.191);//3000
  adcmax[4] = (uint16_t)(adc_get_max * 0.905);//2280
  adcmax[5] = (uint16_t)(adc_get_max * 0.877);//2210
  
//开启定时器
  pit_init_ms(pit0,5);             //定时5MS
  set_irq_priority(PIT_CH0_IRQn,0);//优先级,0-3四个级别
  enable_irq(PIT_CH0_IRQn);        //开启0通道定时器 
//打开速度闭环中断  
  kbi_init(KBI1_P24,IRQ_RISING);//通道选择为KBI1_P0，上升沿触发
  set_irq_priority(KBI1_IRQn,1);//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
  enable_irq(KBI1_IRQn);	//打开KBI1_IRQn的中断开关
//显示数据
  while(1)
  {
//OLED显示内容
    all_print();
//    get_print();
//    max_print();
//    guiyi_print();
//    error_print();
//    panduan_print();
//    pid_print();
//    motor_print();
  }
}



////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
uint8_t txt_0[20]="";
uint8_t txt_1[20]="";
uint8_t txt_2[20]="";
uint8_t txt_3[20]="";
uint8_t txt_4[20]="";
uint8_t txt_5[20]="";
uint8_t txt_6[20]="";
uint8_t txt_7[20]="";
uint8_t txt_8[20]="";

void all_print(void)
{
  printf("%c",0x03);
  printf("%c",0xFc);
  printf("%c",(char)(adc_get_max&0x00ff));
  printf("%c",(char)((adc_get_max>>8)&0x00ff));
  printf("%c",(char)(huandao_biaozhi&0x00ff));
  printf("%c",(char)((huandao_biaozhi>>8)&0x00ff));
  printf("%c",(char)(speed&0x00ff));
  printf("%c",(char)((speed>>8)&0x00ff));
  printf("%c",(char)((uint32_t)(3000+car_error[0]*100)&0x00ff));
  printf("%c",(char)(((uint32_t)(3000+car_error[0]*100)>>8)&0x00ff));
  printf("%c",(char)(huandao_open_again&0x00ff));
  printf("%c",(char)((huandao_open_again>>8)&0x00ff));
  printf("%c",(char)(dy&0x00ff));
  printf("%c",(char)((dy>>8)&0x00ff));
  printf("%c",0xFc);
  printf("%c",0x03);
  //电压检测
  sprintf((char*)txt_0,"Voltage:%3d",dy_all);
  OLED_P6x8Str(0,0,txt_0);
  //停车
  sprintf((char*)txt_1,"STOP:%2d",stop);
  OLED_P6x8Str(0,1,txt_1);
  //左右平电感
  sprintf((char*)txt_2,"AD:%4d  AD:%4d",adc_get[0],adc_get[5]);
  OLED_P6x8Str(0,2,txt_2);
  //左右归一值
  sprintf((char*)txt_3,"L:%4d  R:%4d",(uint8_t)guiyi_left,(uint8_t)guiyi_right);
  OLED_P6x8Str(0,3,txt_3);
  //速度档位
  sprintf((char*)txt_4,"speed_set:%4d",speed_set);
  OLED_P6x8Str(0,4,txt_4);
  //偏差计算
  if(car_error[0]>0) sprintf((char*)txt_5,"error: %3d",(uint8_t)car_error[0]);
  else sprintf((char*)txt_5,"error:-%3d",(0-(int8_t)car_error[0]));
  OLED_P6x8Str(0,5,txt_5);
  //PID
  sprintf((char*)txt_6,"P:%4d,D:%4d",(uint8_t)Servo_Kp,(uint8_t)Servo_Kd);
  OLED_P6x8Str(0,6,txt_6);
  //舵机打脚
  sprintf((char*)txt_7,"servo:%4d",servo_now);
  OLED_P6x8Str(0,7,txt_7);
}
