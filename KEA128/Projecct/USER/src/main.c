#include "headfile.h"

extern    char ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,ch9,ch10,ch11,ch12,ch_beg; 
extern    unsigned char c1,c2,c3,c4,c5,summ;  
extern  uint16 pwm_dianji,pwm_duoji,pwm_pr;
    int stopp;
   uint8 dat;
   
   
   int main(void)
{
//获取时钟频率 必须执行
   get_clk();
   uart_init(uart0,38400); 
   uart_init(uart2,115200); 
   ftm_pwm_init(ftm2,ftm_ch2,50, 1500); 
   ftm_pwm_init(ftm2,ftm_ch3,50, 1500); 
       
       
   kbi_init(KBI1_P9,IRQ_RISING);//通道选择为KBI1_P9，上 升沿触发
   set_irq_priority(KBI1_IRQn,0);
   enable_irq(KBI1_IRQn);
       
   pit_init_ms(pit1,5);	        //定时2ms后中断，用户请使用h文件里面的函数，比如pit_init_ms(pit0,1000);这样时间就是1S
   set_irq_priority(PIT_CH1_IRQn,1);
   //设置优先级,根据自己的需求设置 可设置范围为 0 - 3  越小优先级越高
   enable_irq(PIT_CH1_IRQn);		//打开pit1的中断开关
   //设置优先级,根据自己的需求设置 可设置范围为 0 - 3
   while(1)
	{
//          ftm_pwm_duty(ftm2,ftm_ch2, 1570);
//          ftm_pwm_duty(ftm2,ftm_ch3, 1570);
          
          uart_getchar(uart0,&ch_beg);
          if(ch_beg==-86)
          {

		uart_getchar(uart0,&ch1);
                uart_getchar(uart0,&ch2);
                uart_getchar(uart0,&ch3);
                uart_getchar(uart0,&ch4);
                uart_getchar(uart0,&ch5);

                c1=(unsigned char)ch1;
                c2=(unsigned char)ch2;
                c3=(unsigned char)ch3;
                c4=(unsigned char)ch4;
                c5=(unsigned char)ch5;
                summ=c1+c2+c3+c4;
             if(summ==c5 && stopp==0)
              {
                pwm_dianji = c2*256+c1;
                pwm_duoji  = c4*256+c3;
  //              ftm_pwm_duty(ftm2,ftm_ch2, pwm_dianji);
                ftm_pwm_duty(ftm2,ftm_ch3, pwm_duoji);	
             }
           }
          }

}