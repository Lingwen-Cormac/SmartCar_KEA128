#ifndef _test_h
#define _test_h

void test_jidianqi(void);
void test_motor(void);
void test_servo(void);
void test_kbi(void);
void test_adc(void);
void test_drdy(void);
void test_quneng(void);
void test_bmkg(void);
void test_chongdian(void);
void test_ganhuangguan(void);
void test_bomakaiguan(void);
void test_bmq(void);
#endif



/* printf("%c",0x01);
  printf("%c",0xFE);
  //协议头
  printf("%c",(char)((data2>>8)&0x00ff));
  printf("%c",(char)(data2&0x00ff));
  //数据 uint16 一个通道
  printf("%c",0xFE);
  printf("%c",0x01);
  
  printf("%c",0x02);
  printf("%c",0xFD);
  //协议头
  printf("%c",(char)((data3>>8)&0x00ff));
  printf("%c",(char)(data3&0x00ff));
  //数据 uint16 一个通道
  printf("%c",0xFD);
  printf("%c",0x02);
  
  printf("%c",0x03);
  printf("%c",0xFC);
  //协议头
  printf("%c",(char)((data4>>8)&0x00ff));
  printf("%c",(char)(data4&0x00ff));
  //数据 uint16 一个通道
  printf("%c",0xFC);
  printf("%c",0x03);
  
  printf("%c",0x04);
  printf("%c",0xFB);
  //协议头
  printf("%c",(char)((data5>>8)&0x00ff));
  printf("%c",(char)(data5&0x00ff));
  //数据 uint16 一个通道
  printf("%c",0xFB);
  printf("%c",0x04);*/