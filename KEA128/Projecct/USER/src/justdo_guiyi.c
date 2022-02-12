/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  电感归一                   00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
extern uint16_t adc_get[8];
extern uint16_t adcmax[8];
extern uint16_t adcmin[8];
extern uint16_t adc_filter[8];
extern float car_error[15];
uint16_t adc_guiyi[8] = {0,0,0,0,0,0,0,0};            //归一化后的值  0~100
float guiyi_left = 0;
float guiyi_right = 0;
float guiyi_auto = 0;
/**************函数**************/
void justdo_guiyi(void)
{
//归一化
  double guiyi[8] = {0,0,0,0,0,0,0};     //电感归一化值0.00~1.00  中间变量
  uint8_t i;
  for(i=0;i<8;i++)
  {
    guiyi[i] = (float)(adc_filter[i]-adcmin[i])/(adcmax[i]-adcmin[i]);
    if(guiyi[i] <= 0.0f)
      guiyi[i] = 0.01f;
    if(guiyi[i] >= 1.0f)
      guiyi[i] = 1.0f;
    adc_guiyi[i] = (uint16)(guiyi[i]*100);      //0~100之间
    if(adc_guiyi[i]<1u)                                //归一化值小于1则为1
      adc_guiyi[i] = 1u;
    guiyi[i] = 0;
  }
  
//动态权重
//  guiyi_auto = (adc_get[0]*2+adc_get[5]*2)/(adcmax[0]+adcmax[5]);
//  if(guiyi_auto > 1)
//    guiyi_auto = 1;
//  else if(guiyi_auto < 0.2)
//    guiyi_auto = 0.2;
//  guiyi_left  = adc_guiyi[0]*guiyi_auto + adc_guiyi[1]*(1-guiyi_auto);
//  guiyi_right = adc_guiyi[5]*guiyi_auto + adc_guiyi[4]*(1-guiyi_auto); 
  
  
////静态权重
    guiyi_left  = adc_guiyi[0]*0.70 + adc_guiyi[1]*0.30;
    guiyi_right = adc_guiyi[5]*0.70 + adc_guiyi[4]*0.30; 
}


////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
  uint8_t guiyi_1[20]="";
  uint8_t guiyi_2[20]="";
  uint8_t guiyi_3[20]="";
  uint8_t guiyi_4[20]="";
  uint8_t guiyi_5[20]="";
  uint8_t guiyi_6[20]="";
  uint8_t guiyi_7[20]="";
void guiyi_print(void)
{
    sprintf((char*)guiyi_1,"FILTER & GUIYI");
    OLED_P6x8Str(0,1,guiyi_1);
    sprintf((char*)guiyi_2,"gy0:%4d  gy5:%4d",(uint16_t)adc_guiyi[0],(uint16_t)adc_guiyi[5]);
    OLED_P6x8Str(0,2,guiyi_2);
    sprintf((char*)guiyi_3,"gy1:%4d  gy4:%4d",(uint16_t)adc_guiyi[1],(uint16_t)adc_guiyi[4]);
    OLED_P6x8Str(0,3,guiyi_3);
    sprintf((char*)guiyi_4,"gy2:%4d  gy3:%4d",(uint16_t)adc_guiyi[2],(uint16_t)adc_guiyi[3]);
    OLED_P6x8Str(0,4,guiyi_4);
    sprintf((char*)guiyi_5,"GYL:%4d  GYR:%4d",(uint16_t)guiyi_left,(uint16_t)guiyi_right);
    OLED_P6x8Str(0,5,guiyi_5);
    sprintf((char*)guiyi_6,"gy - :%4f",guiyi_auto);
    OLED_P6x8Str(0,6,guiyi_6);
  
  printf("%c",0x03);
  printf("%c",0xFc);
  printf("%c",(char)(adc_guiyi[0]&0x00ff));
  printf("%c",(char)((adc_guiyi[0]>>8)&0x00ff));
  printf("%c",(char)(adc_guiyi[1]&0x00ff));
  printf("%c",(char)((adc_guiyi[1]>>8)&0x00ff));
  printf("%c",(char)(adc_guiyi[2]&0x00ff));
  printf("%c",(char)((adc_guiyi[2]>>8)&0x00ff));
  printf("%c",(char)(adc_guiyi[3]&0x00ff));
  printf("%c",(char)((adc_guiyi[3]>>8)&0x00ff));
  printf("%c",(char)(adc_guiyi[4]&0x00ff));
  printf("%c",(char)((adc_guiyi[4]>>8)&0x00ff));
  printf("%c",(char)(adc_guiyi[5]&0x00ff));
  printf("%c",(char)((adc_guiyi[5]>>8)&0x00ff));
  printf("%c",0xFc);
  printf("%c",0x03);
}

