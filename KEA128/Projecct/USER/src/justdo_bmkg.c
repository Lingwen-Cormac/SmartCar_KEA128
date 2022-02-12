/**********************************************************
**0000000000000000000000000000000000000000000000000000000**
**00                                                   00**
**00               东秦节能二队 编写                   00**
**00             队员 ：丰辰 卓然 令闻                 00**
**00                                                   00**
**0000000000000000000000000000000000000000000000000000000**
**00            函数作用：拨码开关                     00**
**0000000000000000000000000000000000000000000000000000000**
**********************************************************/

#include "headfile.h"

/**************变量**************/
void bmkg_init(void)
{
  //拨码开关
  gpio_init(MARK_1,GPI,0);
  gpio_init(MARK_2,GPI,0);
  gpio_init(MARK_3,GPI,0);
  gpio_init(MARK_4,GPI,0);
  gpio_init(MARK_5,GPI,0);
  gpio_init(MARK_6,GPI,0);
  gpio_init(MARK_7,GPI,0);
  gpio_init(MARK_8,GPI,0);
  //上拉
  port_pull(MARK_1);
  port_pull(MARK_2);
  port_pull(MARK_3);
  port_pull(MARK_4);
  port_pull(MARK_5);
  port_pull(MARK_6);
  port_pull(MARK_7);
  port_pull(MARK_8);
}