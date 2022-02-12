/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00               函数作用：  测试                    00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

//宏定义 
//    ADC0_SE0,    //A0
//    ADC0_SE1,    //A1
//    ADC0_SE2,    //A6
//    ADC0_SE3,    //A7
//    ADC0_SE4,    //B0
//    ADC0_SE5,    //B1
//    ADC0_SE6,    //B2
//    ADC0_SE7,    //B3
//    ADC0_SE8,    //C0
//    ADC0_SE9,    //C1
//    ADC0_SE10,   //C2
//    ADC0_SE11,   //C3
//    ADC0_SE12,   //F4
//    ADC0_SE13,   //F5
//    ADC0_SE14,   //F6
//    ADC0_SE15    //F7

extern uint16_t bbtime;

//电机测试
void test_motor(void)//端口D0 D1
{
  ftm_pwm_init(ftm2,ftm_ch2,11000,0);
  ftm_pwm_init(ftm2,ftm_ch3,11000,0);
  OLED_Init();
  uint8_t txt[20]="";
  uint16_t speed = 250;
  
  while(1)
  {
      //duty
      ftm_pwm_duty(ftm2,ftm_ch2,0);
      ftm_pwm_duty(ftm2,ftm_ch3,speed);
      //printf
      sprintf((char*)txt,"PINLV:11000 ZKB:%4d",speed);
      OLED_P6x8Str(0,1,txt);
  }
}

//舵机测试  端口 H0
void test_servo(void)
{  
  uint16_t mid = 660;
  int8_t step = 1; 
  uint16_t pinlv = 50;
  uint16_t mid_max = mid + 140;    
  uint16_t mid_min = mid - 140;
  
  uint8_t txt[20]="";
  OLED_Init(); 
  ftm_pwm_init(ftm0,ftm_ch0,pinlv,mid);
  ftm_pwm_init(ftm2,ftm_ch2,2000,0);
  ftm_pwm_init(ftm2,ftm_ch3,2000,0);
  
  while(1)
  {
      ftm_pwm_duty(ftm0,ftm_ch0,mid);
      mid += step;
      if(mid == mid_max)
        step = -1;
      if(mid == mid_min)
        step =  1;
      sprintf((char*)txt,"PINLV:%4d,ZKB:%4d",pinlv,mid);
      OLED_P6x8Str(0,1,txt);
      systick_delay_ms(20);
//      //motor
       ftm_pwm_duty(ftm2,ftm_ch2,0);
       ftm_pwm_duty(ftm2,ftm_ch3,240);
  }
}

//adc测试
void test_adc(void)
{
  adc_init(DG0);
  adc_init(DG1);
  adc_init(DG2);
  adc_init(DG3);
  adc_init(DG4);
  adc_init(DG5);
  adc_init(DG6);
  adc_init(DG7);
  
  OLED_Init(); 
  uart_init(uart2,115200);
  
  uint8_t txt_1[20]="";
  uint8_t txt_2[20]=""; 
  uint8_t txt_3[20]="";
  uint8_t txt_4[20]=""; 
  uint8_t txt_5[20]="";
  uint8_t txt_6[20]="";
  uint8_t txt_7[20]="";
  uint8_t txt_8[20]="";
  
  uint16_t adcmax[8] = {0,0,0,0,0,0,0,0};
  uint16_t test_adc[8] = {0,0,0,0,0,0,0,0}; 
  uint8_t i = 0;
 
  while(1)
  {
  test_adc[0]=adc_once(DG0,ADC_12bit);
  test_adc[1]=adc_once(DG1,ADC_12bit);  
  test_adc[2]=adc_once(DG2,ADC_12bit);
  test_adc[3]=adc_once(DG3,ADC_12bit); 
  test_adc[4]=adc_once(DG4,ADC_12bit);
  test_adc[5]=adc_once(DG5,ADC_12bit);
  test_adc[6]=adc_once(DG6,ADC_12bit);
  test_adc[7]=adc_once(DG7,ADC_12bit);
  
  for(i=0;i<8;i++)
  {
    if(test_adc[i] > adcmax[i])
      adcmax[i] = test_adc[i];
  }
  
    
  sprintf((char*)txt_1,"A%1d:%4d   M:%4d",0,test_adc[0],adcmax[0]);
  sprintf((char*)txt_2,"A%1d:%4d   M:%4d",1,test_adc[1],adcmax[1]);
  sprintf((char*)txt_3,"A%1d:%4d   M:%4d",2,test_adc[2],adcmax[2]);
  sprintf((char*)txt_4,"A%1d:%4d   M:%4d",3,test_adc[3],adcmax[3]);
  sprintf((char*)txt_5,"A%1d:%4d   M:%4d",4,test_adc[4],adcmax[4]);
  sprintf((char*)txt_6,"A%1d:%4d   M:%4d",5,test_adc[5],adcmax[5]);
  sprintf((char*)txt_7,"A%1d:%4d   M:%4d",6,test_adc[6],adcmax[6]);
  sprintf((char*)txt_8,"A%1d:%4d   M:%4d",7,test_adc[7],adcmax[7]);
  
  OLED_P6x8Str(0,0,(uint8_t*)txt_1);
  OLED_P6x8Str(0,1,(uint8_t*)txt_2);
  OLED_P6x8Str(0,2,(uint8_t*)txt_3);
  OLED_P6x8Str(0,3,(uint8_t*)txt_4);
  OLED_P6x8Str(0,4,(uint8_t*)txt_5);
  OLED_P6x8Str(0,5,(uint8_t*)txt_6);
  OLED_P6x8Str(0,6,(uint8_t*)txt_7);
  OLED_P6x8Str(0,7,(uint8_t*)txt_8);
  
  printf("%c",0x03);
  printf("%c",0xFc);
  printf("%c",(char)(test_adc[0]&0x00ff));
  printf("%c",(char)((test_adc[0]>>8)&0x00ff));
  printf("%c",(char)(test_adc[1]&0x00ff));
  printf("%c",(char)((test_adc[1]>>8)&0x00ff));
  printf("%c",(char)(test_adc[2]&0x00ff));
  printf("%c",(char)((test_adc[2]>>8)&0x00ff));
  printf("%c",(char)(test_adc[3]&0x00ff));
  printf("%c",(char)((test_adc[3]>>8)&0x00ff));
  printf("%c",(char)(test_adc[4]&0x00ff));
  printf("%c",(char)((test_adc[4]>>8)&0x00ff));
  printf("%c",(char)(test_adc[5]&0x00ff));
  printf("%c",(char)((test_adc[5]>>8)&0x00ff));
  printf("%c",(char)(test_adc[6]&0x00ff));
  printf("%c",(char)((test_adc[6]>>8)&0x00ff));
  printf("%c",(char)(adcmax[6]&0x00ff));
  printf("%c",(char)((adcmax[6]>>8)&0x00ff));
  printf("%c",0xFc);
  printf("%c",0x03);
  }
}      

//继电器测试
void test_jidianqi(void)
{
  gpio_init(H7,GPO,0);
  uint16_t dy,dy_all;
  OLED_Init();
  uint8_t txt_1[20]="";
  adc_init(ADC0_SE2);
  adc_init(ADC0_SE3);
  while(1)
  {
    dy_all = adc_once(ADC0_SE3,ADC_12bit)*10/167;
    dy = adc_once(ADC0_SE2,ADC_12bit)*10/167;
    
    sprintf((char*)txt_1,"dy:%4d,dy_all:%4d",dy,dy_all);
    OLED_P6x8Str(0,1,txt_1);
    
    gpio_turn(H7);
    systick_delay_ms(1000);
  }
}

//KBI中断测试
void test_kbi(void)
{
    //通道对应的引脚，可以根据通道编号在KEA128_kbi.h文件中查找
    kbi_init(KBI0_P0,IRQ_FALLING);		//通道选择为KBI1_P0，上升沿触发
    set_irq_priority(KBI0_IRQn,1);		//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
    enable_irq(KBI0_IRQn);			//打开KBI1_IRQn的中断开关
    EnableInterrupts;				//打开总的中断开关
    OLED_Init(); 
    //中断函数在isr.c文件中
    while(1)
    {
      //郑重地告诉你这里啥都没有
    }
}  

//电容电压检测测试
void test_drdy(void)
{
  adc_init(ADC0_SE2);//单组电压检测
  adc_init(ADC0_SE3);//猜猜这是什么电压?
  OLED_Init(); 

  uint8_t txt_1[20]="";
  uint8_t txt_2[20]="";
  float DY=0; 
  float DY_all=0; 
  while(1)
  {  
  DY=adc_once(ADC0_SE2,ADC_12bit)*10/167;
  sprintf((char*)txt_1,"DIANYA:%4f",DY);
  OLED_P6x8Str(0,0,(uint8_t*)txt_1);
  DY_all=adc_once(ADC0_SE3,ADC_12bit)*10/167;
  sprintf((char*)txt_2,"DIANYA:%4f",DY_all);
  OLED_P6x8Str(1,1,(uint8_t*)txt_2);
  }
}  

//关pwm测试，慎用
void test_quneng(void)//卓然写的
{
  uint8_t i;
  while(1)
  {
    ftm_pwm_init(ftm1,ftm_ch1,50,400);
    for(i=0;i<10;i++)
    systick_delay_ms(1000);
//    SIM->SCGC &= ~ SIM_SCGC_FTM1_MASK; 
    SIM->PINSEL &= ~SIM_PINSEL_FTM1PS1_MASK;
    for(i=0;i<10;i++)
      systick_delay_ms(1000);
  }
}  

//拨码开关测试
void test_bmkg(void)//这也是卓然写的
{
  uart_init(uart2,115200);
  OLED_Init();
  bmkg_init();
  uint8_t kg[8] = {0};
  uint8_t txt_bmkg[20]="";
  while(1)
  {
    kg[0]= gpio_get(MARK_1);
    kg[1]= gpio_get(MARK_2);
    kg[2]= gpio_get(MARK_3);
    kg[3]= gpio_get(MARK_4);
    kg[4]= gpio_get(MARK_5);
    kg[5]= gpio_get(MARK_6);
    kg[6]= gpio_get(MARK_7);
    kg[7]= gpio_get(MARK_8);
    
    sprintf((char*)txt_bmkg,"%2d%2d%2d%2d%2d%2d%2d%2d",kg[0],kg[1],kg[2],kg[3],kg[4],kg[5],kg[6],kg[7]);
    OLED_P6x8Str(0,0,(uint8_t*)txt_bmkg);
    
    printf("%c",0x03);
    printf("%c",0xFc);
      printf("%c",(char)(kg[0]&0x00ff));
      printf("%c",(char)((kg[0]>>8)&0x00ff));
      printf("%c",(char)(kg[1]&0x00ff));
      printf("%c",(char)((kg[1]>>8)&0x00ff));
      printf("%c",(char)(kg[2]&0x00ff));
      printf("%c",(char)((kg[2]>>8)&0x00ff));
      printf("%c",(char)(kg[3]&0x00ff));
      printf("%c",(char)((kg[3]>>8)&0x00ff));
      printf("%c",(char)(kg[4]&0x00ff));
      printf("%c",(char)((kg[4]>>8)&0x00ff));   
      printf("%c",(char)(kg[5]&0x00ff));
      printf("%c",(char)((kg[5]>>8)&0x00ff));
      printf("%c",(char)(kg[6]&0x00ff));
      printf("%c",(char)((kg[6]>>8)&0x00ff));   
      printf("%c",(char)(kg[7]&0x00ff));
      printf("%c",(char)((kg[7]>>8)&0x00ff));
    printf("%c",0xFc);
    printf("%c",0x03);
  }
}

//干簧管测试
void test_ganhuangguan(void)//这还是卓然写的
{
  uint8_t txt_1[20]="";
  uint8_t DYY = 0;
  uint8_t ghgflag = 0;
  OLED_Init(); 
  gpio_init(H3,GPI,0);
  port_pull(H3); //上拉
  while(1)
  {
      if(gpio_get(H3) == 0)
   {
      ghgflag++;
      if(ghgflag >= 2)
      {
        DYY ++;
        ghgflag = 0;
      }
   }
    sprintf((char*)txt_1,"%4d",DYY);
    OLED_P6x8Str(0,0,(uint8_t*)txt_1);
  }
}

//拨码开关测试
void test_bomakaiguan(void)//其实这个是我写的，不知道为啥当时没找到这个函数，于是卓然重新写了一个，在上面
{
  uint8_t txt_1[20]="";
  uint8_t txt_2[20]="";
  uint8_t BMKG[8] = {0};
  OLED_Init(); 
  bmkg_init();
  
  while(1)
  {
   BMKG[0] = gpio_get(MARK_1);
   BMKG[1] = gpio_get(MARK_2);
   BMKG[2] = gpio_get(MARK_3);
   BMKG[3] = gpio_get(MARK_4);
   BMKG[4] = gpio_get(MARK_5);
   BMKG[5] = gpio_get(MARK_6);
   BMKG[6] = gpio_get(MARK_7);
   BMKG[7] = gpio_get(MARK_8);
   
    sprintf((char*)txt_1,"%4d,%4d,%4d,%4d",BMKG[0],BMKG[1],BMKG[2],BMKG[3]);
    OLED_P6x8Str(0,0,(uint8_t*)txt_1);
    sprintf((char*)txt_1,"%4d,%4d,%4d,%4d",BMKG[4],BMKG[5],BMKG[6],BMKG[7]);
    OLED_P6x8Str(0,1,(uint8_t*)txt_2);
  }
}

//无线充电测试，慎用
void test_chongdian()
{
  uint8_t txt_1[20]="";
  uint8_t txt_2[20]="";
  uint16_t BBB = 6000;
  uint16_t dy_all;
  ftm_pwm_init(ftm1,ftm_ch1,11000,BBB);//PWM
 // justdo_power();
  OLED_Init();
  ftm_pwm_duty(ftm1,ftm_ch1,BBB);
  adc_init(ADC0_SE3);
    while(1)
    {
      dy_all = adc_once(ADC0_SE3,ADC_12bit)*10/167;
      sprintf((char*)txt_1,"dy_all:%3d",dy_all);
      OLED_P6x8Str(0,0,txt_1);
      sprintf((char*)txt_2,"PWM:%4d",BBB);
      OLED_P6x8Str(0,1,txt_2);
    }
}

extern uint16_t speed_now;
//编码器测试
void test_bmq()//没错，这也是卓然写的
{
  ftm_pwm_init(ftm2,ftm_ch2,11000,0);
  ftm_pwm_init(ftm2,ftm_ch3,11000,0);
  
  uint8_t txt_2[20]="";
  OLED_Init();
  
  pit_init_ms(pit1,5);
  set_irq_priority(PIT_CH1_IRQn,1);
  enable_irq(PIT_CH1_IRQn);
  
  kbi_init(KBI1_P24,IRQ_RISING);
  set_irq_priority(KBI1_IRQn,1);
  enable_irq(KBI1_IRQn);
  
  EnableInterrupts;
  while(1)
  {
    sprintf((char*)txt_2,"SUDU:%4d",speed_now);
    OLED_P6x8Str(0,2,txt_2);

    ftm_pwm_duty(ftm2,ftm_ch2,0);
    ftm_pwm_duty(ftm2,ftm_ch3,400);//300-150
  }
}