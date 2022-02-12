/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  获取电感                   00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
uint16_t adc_get[8] = {0}; 
uint16_t adcmax[8]={0};
uint16_t adc_judge[8]={0};
uint16_t adcmin[8] = {2,2,2,2,2,2,2};
uint16_t hd_way[4]={0};

#define NUM 8                   //队列深度
uint8_t N_i = 0;                //循环
uint16_t adc_filter[8] = {0};   //滤波值
uint16_t adc_judge_filter[8] = {0};
uint16 temp[NUM][8] = {0};      //队列数组
uint16 sum[8] = {0};            //累和
extern float car_error[15];
extern uint32_t i_am_groot;
extern uint16_t adc_get_max;

/**************函数**************/
//初始化
void get_init(void)
{
  adc_init(DG0);                   
  adc_init(DG1);                 
  adc_init(DG2);                   
  adc_init(DG3);                   
  adc_init(DG4);                   
  adc_init(DG5);  
  adc_init(DG6);
  adc_init(DG7);
    
  if(gpio_get(MARK_1) == 1)
    hd_way[0]=1;
  if(gpio_get(MARK_2) == 1)
    hd_way[1]=1;
  if(gpio_get(MARK_3) == 1)
    hd_way[2]=1;
}

//运行
void justdo_get(void)
{
//获取电感值
  adc_get[0]=adc_once(DG0,ADC_12bit);
  adc_get[1]=adc_once(DG1,ADC_12bit);
  adc_get[2]=adc_once(DG2,ADC_12bit);
  adc_get[3]=adc_once(DG3,ADC_12bit);
  adc_get[4]=adc_once(DG4,ADC_12bit);
  adc_get[5]=adc_once(DG5,ADC_12bit);
  adc_get[6]=0;
  adc_get[7]=0;
  
  adc_judge[0] = adc_get[0];
  adc_judge[1] = adc_get[1];
  adc_judge[2] = adc_get[2];
  adc_judge[3] = adc_get[3];
  adc_judge[4] = adc_get[4];
  adc_judge[5] = adc_get[5];
  adc_judge[6] = adc_get[6];
  adc_judge[7] = adc_get[7];

  uint8_t i;  
  for(i=0;i<8;i++)
  {
    if(adc_get[i] >= adcmax[i])
      adc_get[i] = adcmax[i];
    if(adc_get[i] <= adcmin[i])
      adc_get[i] = adcmin[i];
  }
  
//电感滤波
  uint8 j = 0;
  for(j=0;j<8;j++)
  {
    temp[N_i][j] = adc_judge[j];
  }
  //队列
  N_i++;
  if(N_i == NUM)
    N_i = 0;
  //累和归零
  for(j=0;j<8;j++)
    sum[j] = 0;
  //累和
  for(j=0;j<NUM;j++)
  {
    sum[0] += temp[j][0];
    sum[1] += temp[j][1];
    sum[2] += temp[j][2];
    sum[3] += temp[j][3];
    sum[4] += temp[j][4];
    sum[5] += temp[j][5];
    sum[6] += temp[j][6];
    sum[7] += temp[j][7];
  }
  //平均
  for(j=0;j<8;j++)
  {
    adc_judge_filter[j] = sum[j]/NUM;
  } 
  
//限幅滤波
  for(j=0;j<8;j++)
  {
    temp[N_i][j] = adc_get[j];
  }
  //队列
  N_i++;
  if(N_i == NUM)
    N_i = 0;
  //累和归零
  for(j=0;j<8;j++)
    sum[j] = 0;
  //累和
  for(j=0;j<8;j++)
  {
    sum[0] += temp[j][0];
    sum[1] += temp[j][1];
    sum[2] += temp[j][2];
    sum[3] += temp[j][3];
    sum[4] += temp[j][4];
    sum[5] += temp[j][5];
    sum[6] += temp[j][6];
    sum[7] += temp[j][7];
  }
  //平均
  for(j=0;j<8;j++)
  {
    adc_filter[j] = sum[j]/NUM;
  } 
}


////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
uint8_t get_1[30]="";
uint8_t get_2[30]="";
uint8_t get_3[30]="";
uint8_t get_4[30]="";
uint8_t get_5[30]="";
uint8_t get_6[30]="";
uint8_t get_7[30]="";
extern float guiyi_left;
extern float guiyi_right;
void get_print(void)
{
  printf("%c",0x03);
  printf("%c",0xFc);
  printf("%c",(char)(adc_judge_filter[0]&0x00ff));
  printf("%c",(char)((adc_judge_filter[0]>>8)&0x00ff));
  printf("%c",(char)(adc_judge_filter[1]&0x00ff));
  printf("%c",(char)((adc_judge_filter[1]>>8)&0x00ff));
  printf("%c",(char)(adc_judge_filter[2]&0x00ff));
  printf("%c",(char)((adc_judge_filter[2]>>8)&0x00ff));
  printf("%c",(char)(adc_judge_filter[3]&0x00ff));
  printf("%c",(char)((adc_judge_filter[3]>>8)&0x00ff));
  printf("%c",(char)(adc_judge_filter[4]&0x00ff));
  printf("%c",(char)((adc_judge_filter[4]>>8)&0x00ff));
  printf("%c",(char)(adc_judge_filter[5]&0x00ff));
  printf("%c",(char)((adc_judge_filter[5]>>8)&0x00ff));
  
    printf("%c",(char)(adc_judge_filter[6]&0x00ff));
  printf("%c",(char)((adc_judge_filter[6]>>8)&0x00ff));
    printf("%c",(char)(adcmax[5]&0x00ff));
  printf("%c",(char)((adcmax[5]>>8)&0x00ff));
  printf("%c",0xFc); 
  printf("%c",0x03);
//    sprintf((char*)get_1,"AD0:%4d  AD5:%4d",adc_get[0],adc_get[5]);
//    OLED_P6x8Str(0,2,get_1);
//    sprintf((char*)get_2,"AD1:%4d  AD4:%4d",adc_get[1],adc_get[4]);
//    OLED_P6x8Str(0,3,get_2);
//    sprintf((char*)get_3,"AD2:%4d  AD3:%4d",adc_get[2],adc_get[3]);
//    OLED_P6x8Str(0,4,get_3);
//    sprintf((char*)get_4,"L:%4d   R:%4d",(uint8_t)guiyi_left,(uint8_t)guiyi_right);
//    OLED_P6x8Str(0,6,get_4);
//    if(car_error[0]>0) sprintf((char*)get_5,"error: %3d",(uint8_t)car_error[0]);
//    else sprintf((char*)get_5,"error:-%3d",(0-(int8_t)car_error[0]));
//    OLED_P6x8Str(0,1,get_5);
}

void max_print(void)
{
  sprintf((char*)get_1,"G%1d:%4d   M:%4d",0,adc_get[0],adcmax[0]);
  sprintf((char*)get_2,"G%1d:%4d   M:%4d",1,adc_get[1],adcmax[1]);
  sprintf((char*)get_3,"G%1d:%4d   M:%4d",2,adc_get[2],adcmax[2]);
  sprintf((char*)get_4,"G%1d:%4d   M:%4d",3,adc_get[3],adcmax[3]);
  sprintf((char*)get_5,"G%1d:%4d   M:%4d",4,adc_get[4],adcmax[4]);
  sprintf((char*)get_6,"G%1d:%4d   M:%4d",5,adc_get[5],adcmax[5]);
  sprintf((char*)get_7,"max:%6d",(uint16_t)adc_get_max);

  OLED_P6x8Str(0,0,(uint8_t*)get_1);
  OLED_P6x8Str(0,1,(uint8_t*)get_2);
  OLED_P6x8Str(0,2,(uint8_t*)get_3);
  OLED_P6x8Str(0,3,(uint8_t*)get_4);
  OLED_P6x8Str(0,4,(uint8_t*)get_5);
  OLED_P6x8Str(0,5,(uint8_t*)get_6);
  OLED_P6x8Str(0,6,(uint8_t*)get_7);
}