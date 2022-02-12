#ifndef _justdo_get_h
#define _justdo_get_h

/************宏定义************/ 

#define DG0   ADC0_SE13 
#define DG1   ADC0_SE15
#define DG2   ADC0_SE9 
#define DG3   ADC0_SE8
#define DG4   ADC0_SE14
#define DG5   ADC0_SE12
#define DG6   ADC0_SE10
#define DG7   ADC0_SE11

//#define DG0   ADC0_SE13 
//#define DG1   ADC0_SE15
//#define DG2   ADC0_SE10
//#define DG3   ADC0_SE11
//#define DG4   ADC0_SE14
//#define DG5   ADC0_SE12
//#define DG6   ADC0_SE8
//#define DG7   ADC0_SE9


//#define DG0   ADC0_SE9
//#define DG1   ADC0_SE15
//#define DG2   ADC0_SE11
//#define DG3   ADC0_SE10
//#define DG4   ADC0_SE14
//#define DG5   ADC0_SE8
//#define DG6   ADC0_SE13
//#define DG7   ADC0_SE12
//函数
void get_init(void);
void justdo_get(void);
void get_print(void);
void max_print(void);

#endif