/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：  计算偏差                   00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
extern uint16_t adc_guiyi[8];
extern uint16_t adc_get[8];
extern uint16_t adcmax[8];
extern uint16_t adc_judge_filter[8];
extern float guiyi_left;
extern float guiyi_right;
extern uint16_t bbtime;
extern uint16_t speed_set;
extern uint32_t i_am_groot;
extern uint16_t adc_get_max;
extern uint8_t huandao_dangwei;
///^………界四一锅你从未见过的船新的分gay线………^///
#define HDTIME 85
#define HDAGAIN 1000//5s
#define CHTIME 30
#define SHIZITIME 340
extern uint16_t hd_way[4];
uint8_t hd_i = 0;
uint8_t shizi_huandao_close = 0;
uint16_t huandao_biaozhi = 0;
uint16_t huandao_biaozhi_zuo = 0;
uint16_t huandao_biaozhi_you = 0;
uint16_t huandao_open_l = 0;
uint16_t huandao_open_r = 0;
uint16_t huandao_opentime_l = 0;
uint16_t huandao_opentime_r = 0;
uint16_t huandao_open_again = 0;
uint16_t huandao_out = 0;
uint8_t chuhuan_left = 0;
uint8_t chuhuan_right = 0;
uint8_t chuhuan_again = 0;
uint16_t chuhuan_xiuzheng = 0;
uint8_t stop_flag = 0;
uint16_t ruhuan_biaozhi[4] = {0,0,0,0};
float car_error[15] = {0};
float hd_in_error = 0;
float sqrt0 = 0.0,sqrt1 = 0.0;  

/**************函数**************/
void justdo_error(void)//带有&&&表示改正归一权重，带有***表示直接赋予偏差
{

//避线圈
//  if(i_am_groot>2000&&i_am_groot<2005)
//    huandao_open_again = 600;
//  if(huandao_open_again >2)
//    bbtime = 5;

//___十字判断___
  if(adc_guiyi[2] > 30 && adc_guiyi[3] > 30 && ((adc_guiyi[2]+adc_guiyi[3])>80))
  {
    shizi_huandao_close = 1;
    guiyi_left = adc_guiyi[0];
    guiyi_right = adc_guiyi[5];
    //bbtime = 5;
  }
  if(shizi_huandao_close == 1)
    if((adc_guiyi[2]+adc_guiyi[3])<80)
      shizi_huandao_close = 0;
    
//环岛判断（手动挡）用 mark4为1 控制
  if(huandao_dangwei == 1 && huandao_open_again == 0 && shizi_huandao_close == 0)
  {  
    if( (adc_judge_filter[0]+adc_judge_filter[1])>(adcmax[0]*2) ||  (adc_judge_filter[5]+adc_judge_filter[4])>(adcmax[5]*2) )
    {
      huandao_biaozhi ++;
      if(huandao_biaozhi > 500)
        huandao_biaozhi = 0;
      bbtime = 5;
    }
    else
      huandao_biaozhi = 0;
    
    if(huandao_biaozhi >= 5) //25ms
    {
       //左环岛判断
      if(hd_way[hd_i]==1 && adc_guiyi[2]>27 && adc_guiyi[3]< 10 && adc_guiyi[5]>15)
      {        
        huandao_open_l = 1;
        chuhuan_again = 1;
        huandao_opentime_l = HDTIME;
        huandao_open_again = HDAGAIN;
        huandao_biaozhi = 0;
        hd_i ++;
      }
      //右环岛判断 
      else if(hd_way[hd_i]==0 && adc_guiyi[3]>15 && adc_guiyi[2]< 10 && adc_guiyi[0]>15)
      {
        huandao_open_r = 1;
        chuhuan_again = 1;
        huandao_opentime_r = HDTIME;
        huandao_open_again = HDAGAIN;
        huandao_biaozhi = 0;
        hd_i ++;
      }
      huandao_biaozhi = 0;
    }
  }
  
//环岛判断（自动挡） 用 mark4 为0控制
  if(huandao_dangwei == 0 && huandao_open_again == 0 && shizi_huandao_close == 0)
  {  
    if( (adc_judge_filter[0]+adc_judge_filter[1])>(adcmax[0]*2) ||  (adc_judge_filter[5]+adc_judge_filter[4])>(adcmax[5]*2) )
    {
      huandao_biaozhi ++;
      if(huandao_biaozhi > 500)
        huandao_biaozhi = 0;
      bbtime = 5;
    }
    else
      huandao_biaozhi = 0;
    
    if(huandao_biaozhi >= 3) //15ms
    {
       //左环岛判断
      if(adc_guiyi[2]>27 && adc_guiyi[3]< 10 && adc_guiyi[5]>15)
      {        
        huandao_open_l = 1;
        chuhuan_again = 1;
        huandao_opentime_l = HDTIME;
        huandao_open_again = HDAGAIN;
        huandao_biaozhi = 0;
      }
      //右环岛判断 
      else if(adc_guiyi[3]>15 && adc_guiyi[2]< 10 && adc_guiyi[0]>15)
      {
        huandao_open_r = 1;
        chuhuan_again = 1;
        huandao_opentime_r = HDTIME;
        huandao_open_again = HDAGAIN;
        huandao_biaozhi = 0;
      }
      huandao_biaozhi = 0;
    }
  }
  
  //标志位
  if(huandao_biaozhi_zuo>0 || huandao_biaozhi_you>0 || huandao_biaozhi>0)
  {
    //bbtime = 5;
  }
  
//出环判断
  if(chuhuan_again==1 && (huandao_open_again<HDAGAIN-80 && huandao_open_again>HDAGAIN-800) && ( (adc_judge_filter[0]+adc_judge_filter[1])>(adcmax[0]*1.7) || (adc_judge_filter[5]+adc_judge_filter[4])>(adcmax[5]*1.7) ))
  {
    if(adc_guiyi[2]>95 || adc_guiyi[3]>95)
    {
      huandao_out = CHTIME;
      huandao_open_again = 200;//0.5s
      chuhuan_again = 0;
      bbtime = 5;
    }
  }
//出环标志位超时清零 5s
  if(chuhuan_again==1 &&  huandao_open_again<10)
    chuhuan_again = 0;

  
///////////////环岛执行___归一部分///////////////
  //入环修正
  if(huandao_open_again>HDAGAIN-180)
 {
    //bbtime = 5;
    guiyi_left =  adc_guiyi[0]*0.25 + adc_guiyi[1]*0.75;
    guiyi_right = adc_guiyi[5]*0.25 + adc_guiyi[4]*0.75; 
  }
  //出环修正
  if(chuhuan_xiuzheng > 0)
  {
    chuhuan_xiuzheng --;
    guiyi_left = adc_guiyi[0];
    guiyi_right = adc_guiyi[5];
  }
  //出环标志位归零
  if(huandao_open_again == 1 || chuhuan_xiuzheng == 1)
  {
    chuhuan_left = 0;
    chuhuan_right = 0;
  }

  
  
//////////////////////计算偏差///////////////////////
    sqrt0 = sqrt(guiyi_left);
    sqrt1 = sqrt(guiyi_right);
    car_error[0] =(float)( (sqrt0-sqrt1)/(guiyi_left + guiyi_right) );
    car_error[0] = (float)(car_error[0]*100);
  
//数据异常
    if( car_error[0] - car_error[1] > 100 || car_error[0] - car_error[1] < -100 )  
      car_error[0] = car_error[1];


    
    
///////////////环岛执行___偏差部分///////////////
//入环执行
  if(huandao_open_l == 1)//左入环
  {
    sqrt0 = sqrt(adc_guiyi[2]);
    sqrt1 = sqrt(adc_guiyi[3]);
    hd_in_error =(float)( (float)(sqrt0-sqrt1)/(float)(adc_guiyi[2] + adc_guiyi[3]) );
    hd_in_error = (float)((float)(hd_in_error*100));
    if(car_error[0] > 0 && (car_error[0] > hd_in_error) )
      car_error[0] = car_error[0];
    else 
      car_error[0] = hd_in_error;
    
    if(car_error[0] < 0)
      car_error[0] = 0;
    if(car_error[0] > 10)
      car_error[0] = 10;
    huandao_opentime_l --;
    //bbtime = 5;
  } 
  if(huandao_open_r == 1)//右入环
  {
    sqrt0 = sqrt(adc_guiyi[2]);
    sqrt1 = sqrt(adc_guiyi[3]);
    hd_in_error =(float)( (float)(sqrt0-sqrt1)/(float)(adc_guiyi[2] + adc_guiyi[3]) );
    hd_in_error = (float)((float)(hd_in_error*100));
    if(car_error[0] < 0 && (car_error[0] < hd_in_error))
      car_error[0] = car_error[0];
    else 
      car_error[0] = hd_in_error;
    if(car_error[0] > 0)
      car_error[0] = 0;
    if(car_error[0] < -10)
      car_error[0] = -10;
    huandao_opentime_r --;
    //bbtime = 5;
  }  
  
//手动挡方向标志位归零，可综合时间调整
  if(hd_i >= 3)
    hd_i = 0;

//入环标志位归零
  if(huandao_opentime_l==1)
  {
    huandao_open_l = 0;
  }
  if(huandao_opentime_r == 1)
  {
    huandao_open_r = 0;
  }

//出环执行
  if(huandao_out>0)
  {
    if(car_error[5]+car_error[6]+car_error[7]+car_error[8]+car_error[9]+car_error[10]+car_error[11]+car_error[12]+car_error[13]+car_error[14] > 0)
    {
      car_error[0]=15;
      chuhuan_xiuzheng = 200;
    }
    if(car_error[5]+car_error[6]+car_error[7]+car_error[8]+car_error[9]+car_error[10]+car_error[11]+car_error[12]+car_error[13]+car_error[14] < 0)
    {
      car_error[0]=-15;
      chuhuan_xiuzheng = 200;
    }
    huandao_out --;
  }

//重复入环倒计时
  if(huandao_open_again > 0)
  {
    huandao_open_again --;
  }

  
//___检测丢线___
  if(adc_get[0]<100 && adc_get[5]<100 && adc_get[1]<100 && adc_get[4]<100)
  {
    stop_flag ++;
    if(stop_flag>100)
    {
      stop_flag = 100;
    }
    if(adc_get[1]>adc_get[4])
      car_error[0] = 22;
    if(adc_get[1]<adc_get[4])
      car_error[0] = -22;
    if(adc_get[1]==adc_get[4])
      car_error[0] = car_error[1];
  }
  else
    stop_flag = 0;
  
//偏差队列
  car_error[14] = car_error[13];
  car_error[13] = car_error[12];
  car_error[12] = car_error[11];
  car_error[11] = car_error[10];
  car_error[10] = car_error[9];
  car_error[9] = car_error[8];
  car_error[8] = car_error[7];
  car_error[7] = car_error[6];
  car_error[6] = car_error[5];
  car_error[5] = car_error[4];
  car_error[4] = car_error[3];
  car_error[3] = car_error[2];
  car_error[2] = car_error[1];
  car_error[1] = car_error[0];
}

////////////////////////////////////////////////////////
//******************以下显示当前函数******************//
////////////////////////////////////////////////////////
uint8_t error_test_1[30]="";
uint8_t error_test_2[30]="";
uint8_t error_test_3[30]="";
uint8_t error_test_4[30]="";
uint8_t error_test_5[30]="";
uint8_t error_test_6[30]="";

uint8_t mark_1,mark_2,mark_3,mark_4;
uint8_t panduan_1[20]="";
uint8_t panduan_2[20]="";
uint8_t panduan_3[20]="";
uint8_t panduan_4[20]="";
uint8_t panduan_5[20]="";
uint8_t panduan_6[20]="";
uint8_t panduan_7[20]="";
uint8_t panduan_0[20]="";

void error_print(void)
{
  sprintf((char*)error_test_1,"ERROR:");
  OLED_P6x8Str(0,1,error_test_1);
  sprintf((char*)error_test_2,"AD0:%4d,AD5:%4d",adc_get[0],adc_get[5]);
  OLED_P6x8Str(0,2,error_test_2);
  sprintf((char*)error_test_3,"AD1:%4d,AD4:%4d",adc_get[1],adc_get[4]);
  OLED_P6x8Str(0,3,error_test_3);
  sprintf((char*)error_test_4,"gyL:%4d,gyR:%4d",(uint16_t)guiyi_left,(uint16_t)guiyi_right);
  OLED_P6x8Str(0,4,error_test_4);
    
  if(car_error[0] >= 0)
    sprintf((char*)error_test_5,"ERR[0]: %4f",car_error[0]);
  else
    sprintf((char*)error_test_5,"ERR[0]:-%4f",(0-car_error[0]));
  OLED_P6x8Str(0,5,error_test_5);
}


void panduan_print(void)
{
  mark_1 = gpio_get(MARK_1);
  mark_2 = gpio_get(MARK_2);
  mark_3 = gpio_get(MARK_3);
  sprintf((char*)panduan_0,"%2d,%2d,%2d,time:%1d",mark_1,mark_2,mark_3,hd_i);
  OLED_P6x8Str(0,0,(uint8_t*)panduan_0);

  if(car_error[0] >= 0)
    sprintf((char*)panduan_1,"ERR[0]: %4f",car_error[0]);
  else
    sprintf((char*)panduan_1,"ERR[0]:-%4f",(0-car_error[0]));
  OLED_P6x8Str(0,1,panduan_1);
  sprintf((char*)panduan_2,"ADCmax:%4d",adc_get_max);
  OLED_P6x8Str(0,2,panduan_2); 
  sprintf((char*)panduan_3,"adc0:%4d adc5:%4d",adc_judge_filter[0],adc_judge_filter[5]);
  OLED_P6x8Str(0,3,panduan_3);
  sprintf((char*)panduan_4,"adc1:%4d adc4:%4d",adc_judge_filter[1],adc_judge_filter[4]);
  OLED_P6x8Str(0,4,panduan_4);
  sprintf((char*)panduan_5,"gy2:%4d gy3:%4d",adc_guiyi[2],adc_guiyi[3]);
  OLED_P6x8Str(0,5,panduan_5);
  sprintf((char*)panduan_7,"OPENAGAIN:%4d",huandao_open_again);
  OLED_P6x8Str(0,7,panduan_7); 
}