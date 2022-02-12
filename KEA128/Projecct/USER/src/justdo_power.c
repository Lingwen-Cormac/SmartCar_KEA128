/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用： 恒功率                      00**
**0000000000000000000000000000000000000000000000000000000**
***********************************************************/

#include "headfile.h"

/**************变量**************/
#define TARGET 1000//目标值
#define POWER_PWM 4500//中值PWM
#define CUR_NUM 20//滤波队列深度
//PID系数
float power_p = 0.5;//1 0.3
float power_i = 0.3;
float power_d = 0.8;
//变量
extern uint16_t dy;
extern uint16_t dy_all;
uint16_t vol_get = 0;
uint16_t cur_get = 0;
uint16_t cur[CUR_NUM] = {0};
uint16_t cur_filter[CUR_NUM] = {0};
uint32_t cur_sum = 0;
uint16_t cur_sign =0;
double cur_right = 0;
float pow_target = TARGET;
float pow_error = 0;
float pow_error_last = 0;
float pow_error_last_last = 0;
float power_out = POWER_PWM;
uint16_t v_l[20] = {0};
uint16_t c_l[20] = {0};
uint32_t v_l_sum = 0;
uint32_t c_l_sum = 0;
uint16_t v_p = 0;
uint16_t c_p = 0;
uint16_t pow_over = 0;
uint16_t power_pwm = POWER_PWM;
//循环变量
uint8_t c_i = 0;
uint8_t p_i = 0;


/*************函数**************/
void power_init(void)
{
  ftm_pwm_init(ftm1,ftm_ch1,11000,POWER_PWM);//PWM
  adc_init(ADC0_SE5);
  adc_init(ADC0_SE4);
  adc_init(ADC0_SE3);
  gpio_init(H5,GPO,0);
}

/************************************
              闭环充电
************************************/
void justdo_power(void)
{
//以下是杨氏超级无用滤波法，经与中国民航大学三位DaLao交流后，更没用了。
//PS：傻冲比恒功率速度更快

  //读取电压电流
  vol_get = adc_once(ADC0_SE5,ADC_12bit);
  cur_get = adc_once(ADC0_SE4,ADC_12bit);
  if(cur_get<=0)
    cur_get = 1;
  if(cur_get > cur_sign)
  {
    cur[c_i] = cur_get;
    c_i++;
  }
  if(c_i == CUR_NUM)
    c_i = 0;
  //排序数组
  uint8_t i,j;
  for(i=0;i<CUR_NUM;i++)
    cur_filter[i] = cur[i];
  //冒泡排序法
  uint16_t cur_temp = 0;
  for(i=0;i<CUR_NUM-1;i++)
    {
      for(j=0;j<CUR_NUM-i-1;j++)
      {
        if(cur_filter[j]>cur_filter[j+1])
        {
          cur_temp=cur_filter[j+1];
          cur_filter[j+1]=cur_filter[j];
          cur_filter[j]=cur_temp;
        }
      }
    }
  //sign
  cur_sign = (uint16_t)(cur_get*2)/3;
  if(cur_sign >= cur_filter[CUR_NUM-2])
    cur_sign = cur_filter[0];
  //取平均
  cur_right = (float)(cur_filter[CUR_NUM-6]+cur_filter[CUR_NUM-7]+
                       cur_filter[CUR_NUM-8]+cur_filter[CUR_NUM-9]+
                       cur_filter[CUR_NUM-10]+cur_filter[CUR_NUM-11]+
                       cur_filter[CUR_NUM-12]+cur_filter[CUR_NUM-13]+
                       cur_filter[CUR_NUM-14]+cur_filter[CUR_NUM-15])/10;
  //计算功率
  v_l[p_i] = vol_get;
  c_l[p_i] = (uint16_t)cur_right;
  p_i ++;
  if(p_i == 20)
  {
    p_i = 0;
  }
  //累和归零
  v_l_sum = 0;
  c_l_sum = 0;
  for(i=0;i<20;i++)
  {
    v_l_sum += v_l[i];
    c_l_sum += c_l[i];
  }
  v_p = (uint32_t)(v_l_sum/200);
  c_p = (uint32_t)(c_l_sum/200);
  pow_over = (uint16_t)((c_p*175)/46.4);//25.8
  
  //高电压校正功率
  if(dy_all > 70)
    pow_target = TARGET-20;
  if(dy_all > 80)
    pow_target = TARGET-40;
  if(dy_all > 90)
    pow_target = TARGET-60;
  if(dy_all > 100)
    pow_target = TARGET-80;
  if(dy_all > 105)
    pow_target = TARGET-90;
   
  //计算偏差
  pow_error = (float)(pow_target - pow_over);
  //计算反馈 增量式PID
  power_out += power_p * (pow_error - pow_error_last) + power_i * pow_error + 
              power_d*(pow_error - 2*pow_error_last + pow_error_last_last);
  
  pow_error_last_last = pow_error_last;
  pow_error_last = pow_error; 

  //限幅
  if(power_out >= 9920)
    power_out = 9920;
  else if(power_out <= 0)
    power_out = 1;
  //赋值输出
  power_pwm = (uint16_t)power_out;
  ftm_pwm_duty(ftm1,ftm_ch1,power_pwm);  
 
//蓝牙传输
  printf("%c",0x03);
  printf("%c",0xFc);
  printf("%c",(char)(c_p&0x00ff));
  printf("%c",(char)((c_p>>8)&0x00ff));
  printf("%c",(char)(v_p&0x00ff));
  printf("%c",(char)((v_p>>8)&0x00ff));
  printf("%c",(char)(pow_over&0x00ff));
  printf("%c",(char)((pow_over>>8)&0x00ff));
  printf("%c",(char)(cur_get&0x00ff));
  printf("%c",(char)((cur_get>>8)&0x00ff));
  printf("%c",(char)((uint16_t)cur_right&0x00ff));
  printf("%c",(char)(((uint16_t)cur_right>>8)&0x00ff));  
  printf("%c",(char)(power_pwm&0x00ff));
  printf("%c",(char)((power_pwm>>8)&0x00ff));
  printf("%c",(char)(dy_all&0x00ff));
  printf("%c",(char)((dy_all>>8)&0x00ff));
  printf("%c",0xFc);
  printf("%c",0x03);

}
