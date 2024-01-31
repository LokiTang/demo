#ifndef _RGB_DEVICE_H_
#define _RGB_DEVICE_H_


#include "system/includes.h"            //本地库文件路径
#include "typedef.h"
#include <stdlib.h>                     //用于RAND_MAX

#if     0
#define __PWMLED_NEUTRAL_MODE__()           do{ JL_PORTB->DIR |=  BIT(4); JL_PORTB->PD  &= ~BIT(4);JL_PORTB->PU &= ~BIT(4);}while(0)
#define __PWMLED_SUPERIOR_MODE__()          do{ JL_PORTA->DIR &= ~BIT(6); JL_PORTA->OUT |= BIT(6);}while(0);
#define __PWMLED_INFERIOR_MODE__()          do{ JL_PORTA->DIR &= ~BIT(6); JL_PORTA->OUT &= ~BIT(6);}while(0);
#endif

#define LED_NUM 16
#define HSV_THRESHOLD_VALE  0
#define Pixmap_Max  255
#define HUES_MAX    360
#define HUES_CRASH  360 / LED_NUM
#define VALUE_MAX   100

#define __this      (buffer+1)                  
#define __interrpt_clear__    (buffer)          
#define __VALUE_EQUAL_ORNOT__(x , y)      ((x) == (y)) ? ture : false
#define __VALUE_SMALLER__(x)               (x < 0) ? 1: 0
#define __JUDGE_SMALL__(x,y)               (x < y) ? 1: 0

#define color_single_breathing      0

#if HSV_THRESHOLD_VALE
enum{
    HSV_RED = 0;
    HSV_YEELOW = 60;
    HSV_GREEN = 120;
    HSV_GRAY = 180;
    HSV_BLUE = 240;
    HSV_PURPLE = 300;
};
#endif


typedef struct _COLOR_DEVICE_
{
    u8 r;
    u8 g;
    u8 b;
}_COLOR;

typedef struct rgb_device
{
    u16 h;
    u8  s;
    u8  v;
    u16 hues;
    u8  permission;
    s8  step;
    u8  i;
    u32 timer_ret;
}COLOR_;



extern void led_spi_init(void);
extern void led_spi_rgb_to_24byte(u8 r, u8 g, u8 b, u8 *buf, int idx);
extern void led_spi_send_rgbbuf(u8 *rgb_buf, u16 led_num);


void rgb_spi_permit_func_init(void);        
void color_rainbow_init(u8 r,u8 g,u8 b,u8 index);
void color_mode_display(u8 R,u8 G,u8 B);
extern void user_spi_send_rgbbuf(u16 led_num);
extern void user_spi_rgb_to_24byte(u8 r,u8 g,u8 b,u8 index);
// extern void color_mode_chasing(void);


#if color_single_breathing
void hsv_mode_single_light_red_breathing(void);         //单色呼吸-红色
void hsv_mode_single_light_yellow_breathing(void);      //单色呼吸-黄色
void hsv_mode_single_light_green_breathing(void);       //单色呼吸-绿色
void hsv_mode_single_light_gray_breathing(void);        //单色呼吸-青色
void hsv_mode_single_light_blue_breathing(void);        //单色呼吸-蓝色
void hsv_mode_single_light_purple_breathing(void);      //单色呼吸-紫色
#endif


void hsv_crash_maximum_func(void);//七彩初始化
void hsv_mode_color_rotate_infinite(void);              //七彩流光旋转
void hsv_single_color_rotate_infinite(void);            //七彩单色旋转-不灭灯
void color_transform_with_gradient(void);               //渐变灯效-正序
void hsv_single_shutoff_rotate_infinite(void);          //七彩单色流水-灭灯
void color_breathing_with_duration(void);               //彩虹灯圈呼吸
void color_breathing_per_posi_nega_tive(void);          //彩虹循环呼吸
void hsv_wave_pulse_color_mode(void);                   //波浪冲击效果
void hsv_starry_sky_star_flash(void);                   //星空闪耀效果
void hsv_stimulate_color_electronic_func();             //闪电效果




void RGBtoHSV(u8 r, u8 g, u8 b, u16 *h, u8 *s, u8 *v);

void HSVtoRGB(u8 *r, u8 *g, u8 *b, u16 h, u8 s, u8 v);

// void HSVtoRGB(u8 *r, u8 *g, u8 *b, u16 h, u8 s, u8 v);
#endif