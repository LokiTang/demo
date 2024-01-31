#include"rgb_device.h"


COLOR_ rgb_array[LED_NUM]; 
_COLOR color_pixmap = {0,0,0};
COLOR_ rgb_dev;
static u8 hsv_array[] ={95,100};
static u8 buffer[] = {0,1};     

u8 current_led_breath = 0; 
u8 breath_direction = 1;  
void color_breathing_with_duration(void)            
{
    static u8 breath_value = 0; 

    for (u8 i = 0; i < LED_NUM; i++) {
        rgb_array[i].h = (i * (HUES_MAX / LED_NUM)) % HUES_MAX;
        memcpy(&rgb_array[i].s, hsv_array + 1, sizeof(rgb_array[i].s)); 
        rgb_array[i].v = breath_value; 
    }

    #if 1
    for (u8 i = 0; i < LED_NUM; i++) {
        HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[i].h, rgb_array[i].s, rgb_array[i].v);
        color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
    }
    #else       //单个灯流水呼吸方式
    // 设置当前呼吸灯的颜色
    HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[current_led_breath].h, rgb_array[current_led_breath].s, rgb_array[current_led_breath].v);
    color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, current_led_breath);
    #endif

    breath_value += breath_direction;

    if (breath_value >= 100) {
        breath_direction = -1;
    }

    if (breath_value <= 0) {
        breath_direction = 1;
        current_led_breath = (current_led_breath + 1) % LED_NUM;
    }
}

u8 current_color_index = 0;  
u8 breath_value = 0;         
s8 single_breath_direction = 1;     
void color_breathing_per_posi_nega_tive(void)           
{
    for (u8 i = 0; i < LED_NUM; i++) {
        rgb_array[i].h = (current_color_index * (HUES_MAX / LED_NUM)) % HUES_MAX;
        memcpy(&rgb_array[i].s, hsv_array + 1, sizeof(rgb_array[i].s)); 
        rgb_array[i].v = breath_value; 
    }

    for (u8 i = 0; i < LED_NUM; i++) {
        HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[i].h, rgb_array[i].s, rgb_array[i].v);
        color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
    }

    breath_value += single_breath_direction;

    if (breath_value >= 100) {
        single_breath_direction = -1;
    }

    if (breath_value <= 0) {
        single_breath_direction = 1;

        current_color_index++;

        if (current_color_index >= HUES_MAX / (HUES_MAX / LED_NUM)) {
            current_color_index = 0;
        }
    }
}
void color_transform_with_gradient(void)        //渐变灯效-正序
{
        static u16 hue = 0; 
        static u8 i;

        rgb_dev.h = hue;
        memcpy(&rgb_dev.s,hsv_array+1,sizeof(rgb_dev.s));
        memcpy(&rgb_dev.v,hsv_array,sizeof(rgb_dev.v));
        
        hue = (hue + 1) % HUES_MAX; 
    
        for (i = 0; i < LED_NUM; i++) 
        {
            HSVtoRGB( &color_pixmap.r, &color_pixmap.g, &color_pixmap.b,rgb_dev.h, rgb_dev.s, rgb_dev.v);
            color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
        }
}



void hsv_mode_color_rotate_infinite(void)        //七彩旋转
{
    static u16 hue = 0; 
    u8 i;

    for (i = 0; i < LED_NUM; i++) {
        rgb_array[i].h = (hue + i * (HUES_MAX / LED_NUM)) % HUES_MAX;
        memcpy(&rgb_array[i].s,hsv_array+1,sizeof(rgb_array[i].s));
        memcpy(&rgb_array[i].v,hsv_array,sizeof(rgb_array[i].v));
    }
    hue = (hue + 1) % HUES_MAX; 
    
    for (i = 0; i < LED_NUM; i++) 
    {
        HSVtoRGB( &color_pixmap.r, &color_pixmap.g, &color_pixmap.b,rgb_array[i].h, rgb_array[i].s, rgb_array[i].v);
        color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
    }
}

static u8 current_led = 0;        
void hsv_single_color_rotate_infinite(void)        
{
    static u16 hue = 0; 

    // // 熄灭上一个点亮的LED
    // HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, 0, 0, 0);
    // color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, current_led);

    for (u8 i = 0; i < LED_NUM; i++) {
        rgb_array[i].h = hue;
        memcpy(&rgb_array[i].s, hsv_array + 1, sizeof(rgb_array[i].s)); 
        memcpy(&rgb_array[i].v, hsv_array, sizeof(rgb_array[i].v));     
    }

    HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[current_led].h, rgb_array[current_led].s, rgb_array[current_led].v);
    color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, current_led);

    hue = (hue + 1) % HUES_MAX;

    current_led = (current_led + 1) % LED_NUM;
}

void hsv_single_shutoff_rotate_infinite(void)       //七彩单色旋转-灭灯
{
    static u16 hue = 0; 

    //为了避免初始化异常
    if(current_led != (LED_NUM-1))
    color_rainbow_init(0, 0, 0, LED_NUM-1);

    for (u8 i = 0; i < LED_NUM; i++) {
        rgb_array[i].h = hue;
        memcpy(&rgb_array[i].s, hsv_array + 1, sizeof(rgb_array[i].s)); 
        memcpy(&rgb_array[i].v, hsv_array, sizeof(rgb_array[i].v));     
    }

    HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[current_led].h, rgb_array[current_led].s, rgb_array[current_led].v);
    color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, current_led);
    
    color_rainbow_init(0, 0, 0, current_led-1);

    hue = (hue + 1) % HUES_MAX;

    current_led = (current_led + 1) % LED_NUM;
}

#define MAX_BRIGHTNESS 100

static float wave_speed = 0.1; // 波浪的速度
static float wave_frequency = 0.1; // 波浪的频率
static float wave_amplitude = 20.0; // 波浪的振幅
void hsv_wave_pulse_color_mode(void)            //波浪冲击效果
{
    static float time = 0;

    // 更新波浪效果的时间
    time += wave_speed;

    for (u8 i = 0; i < LED_NUM; i++) {
        // 计算每个LED的高度，使用正弦函数模拟波浪
        float height = wave_amplitude * sin((wave_frequency * i) + time);

        // 将高度映射到亮度范围
        rgb_array[i].v = (int)(MAX_BRIGHTNESS * (height + 1) / 2);

        // 设置其他颜色属性
        rgb_array[i].h = hsv_array[0];
        memcpy(&rgb_array[i].s, hsv_array + 1, sizeof(rgb_array[i].s));
    }

    // 设置每个LED的颜色
    for (u8 i = 0; i < LED_NUM; i++) {
        HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[i].h, rgb_array[i].s, rgb_array[i].v);
        color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
    }
}


static float star_density = 0.1; // 星星的密度
void hsv_starry_sky_star_flash(void)            //星空闪耀效果
{
    // 清空 LED 数组
    rgb_spi_permit_func_init();
    static u16 hue = 0; 

    for (u8 i = 0; i < LED_NUM; i++) {
        if ((float)rand() / RAND_MAX < star_density) {
            rgb_array[i].h = (hue + i * (HUES_MAX / LED_NUM)) % HUES_MAX;
            memcpy(&rgb_array[i].s, hsv_array + 1, sizeof(rgb_array[i].s));
            rgb_array[i].v = rand() % (MAX_BRIGHTNESS + 1);
        }
    }
    // hue = (hue + 1) % HUES_MAX;          //若需选择多种颜色可以打开
    // 设置每个 LED 的颜色
    for (u8 i = 0; i < LED_NUM; i++) {
        HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[i].h, rgb_array[i].s, rgb_array[i].v);
        color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
    }
}


void hsv_stimulate_color_electronic_func()              //闪电效果
{
    // 清空 LED 数组
    rgb_spi_permit_func_init();

    u8 start_led = rand() % LED_NUM;
    u8 end_led = rand() % LED_NUM;

    // 在起始点和结束点之间生成颜色渐变效果
    u16 hue_step = (end_led > start_led) ? (HUES_MAX / (end_led - start_led)) : (HUES_MAX / (LED_NUM - start_led + end_led));

    for (u8 i = start_led; i != end_led; i = (i + 1) % LED_NUM) {
        rgb_array[i].h = (i - start_led) * hue_step;
        rgb_array[i].s = 100;               
        rgb_array[i].v = rand() % (MAX_BRIGHTNESS + 1); 
    }

    for (u8 i = 0; i < LED_NUM; i++) {
        HSVtoRGB(&color_pixmap.r, &color_pixmap.g, &color_pixmap.b, rgb_array[i].h, rgb_array[i].s, rgb_array[i].v);
        color_rainbow_init(color_pixmap.r, color_pixmap.g, color_pixmap.b, i);
    }
}

void rgb_spi_permit_func_init(void)             //七彩灯内部初始化
{
    memcpy(&rgb_dev.permission,buffer+1,sizeof(rgb_dev.permission));
    for (u8 i = 0; i < LED_NUM; i++)
    {
        color_rainbow_init(0,0,0,i);
        //memset(buffer,0,sizeof(buffer));      
    }
}

void ui_mode_rgb_superior_show(u8 mode_flag)        //SPIRGB_主函数
{
    led_spi_init();                                 //SPI初始化
    switch (mode_flag)
    {
        rgb_spi_permit_func_init();                //清空buf中的数据
        // if(rgb_dev.timer_ret)
        // {
            sys_hi_timer_del(rgb_dev.timer_ret);      //调用前删除之前的定时器
            memcpy(&rgb_dev.timer_ret,__interrpt_clear__,sizeof(rgb_dev.timer_ret));
        // }
    case 0:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,hsv_mode_color_rotate_infinite,15);          //七彩灯旋转
        break;
    case 1:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,color_transform_with_gradient,45);           //七彩渐变
        break;
    case 2:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,hsv_single_shutoff_rotate_infinite,100);     //单色旋转-不灭灯
        break;
    case 3:
        #if color_single_breathing      //单色呼吸
        rgb_dev.timer_ret = sys_timer_add(NULL,hsv_mode_single_light_red_breathing,15);
        rgb_dev.timer_ret = sys_timer_add(NULL,hsv_mode_single_light_yellow_breathing,15);
        rgb_dev.timer_ret = sys_timer_add(NULL,hsv_mode_single_light_green_breathing,15);
        rgb_dev.timer_ret = sys_timer_add(NULL,hsv_mode_single_light_gray_breathing,15);
        rgb_dev.timer_ret = sys_timer_add(NULL,hsv_mode_single_light_blue_breathing,15);
        rgb_dev.timer_ret = sys_timer_add(NULL,hsv_mode_single_light_purple_breathing,15);
        #else
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,color_breathing_with_duration,50);           //彩虹灯圈呼吸
        #endif
        break;
    case 4:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,color_breathing_per_posi_nega_tive,30);       //单色循环呼吸
        break;
    case 5:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,hsv_single_color_rotate_infinite,100);        //单色旋转-不灭灯       
        break;
    case 6:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,hsv_wave_pulse_color_mode,100);               //波浪效果
        break;
    case 7:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,hsv_starry_sky_star_flash,150);               //星空闪耀效果
        break;
    case 8:
        rgb_dev.timer_ret = sys_hi_timer_add(NULL,hsv_stimulate_color_electronic_func,200);      //闪电效果
        break;
    default:
        break;
    }
}



#if 0
void hsv_crash_maximum_func(void)       //七彩初始化
{
    memcpy(&rainbow,&buffer,sizeof(buffer));
    for (rgb_dev.i = 0; rgb_dev.i < LED_NUM; rgb_dev.i++)
    {
        rgb_dev.h += HUES_MAX / LED_NUM;
        rgb_dev.s = 95; // 饱和度保持为最大值
        rgb_dev.v = 100; // 亮度保持为最大值

        //HSVtoRGB( &color_pixmap.r, &color_pixmap.g, &color_pixmap.b,rgb_dev.h, rgb_dev.s, rgb_dev.v);
        //color_rainbow_init(color_pixmap.r,color_pixmap.g,color_pixmap.b,rgb_dev.i);
        hsv_crash_cycle_with_motivate(rgb_dev.h,rgb_dev.s,rgb_dev.v,rgb_dev.i);          //点亮一个发送一个buf
    }
}

//extern void user_spi_send_rgbbuf(void);
void hsv_crash_cycle_with_motivate(u16 h,u8 s,u8 v,u8 index)    //七彩旋转
{
    memcpy(&rainbow,&buffer,sizeof(buffer));

    rgb_dev.hues = h;
    HSVtoRGB( &color_pixmap.r, &color_pixmap.g, &color_pixmap.b,h, s, v);

    rgb_dev.hues++;
    rgb_dev.hues %= HUES_MAX;
    color_rainbow_init(color_pixmap.r,color_pixmap.g,color_pixmap.b,index);
}
#endif

#if     color_single_breathing          //倘若无需求可直接注释
void hsv_mode_single_light_red_breathing(void)          //单色呼吸-红色
{
        if(count_flag)
        {   cnt++;if(cnt >= Pixmap_Max)memcpy(&count_flag,__interrpt_clear__,sizeof(rainbow));}
        else
        {   cnt--;if(!cnt)memcpy(&count_flag,__this,sizeof(buffer));}
        color_mode_display(cnt,0,0);
}
void hsv_mode_single_light_yellow_breathing(void)       //单色呼吸-黄色
{
        if(count_flag)
        {   cnt++;if(cnt >= Pixmap_Max)memcpy(&count_flag,__interrpt_clear__,sizeof(rainbow));}
        else
        {   cnt--;if(!cnt)memcpy(&count_flag,__this,sizeof(buffer));}
        color_mode_display(cnt,cnt,0);
}
void hsv_mode_single_light_green_breathing(void)         //单色呼吸-绿色
{
        if(count_flag)
        {   cnt++;if(cnt >= Pixmap_Max)memcpy(&count_flag,__interrpt_clear__,sizeof(rainbow));}
        else
        {   cnt--;if(!cnt)memcpy(&count_flag,__this,sizeof(buffer));}
        color_mode_display(0,cnt,0);
}
void hsv_mode_single_light_gray_breathing(void)         //单色呼吸-青色
{
        if(count_flag)
        {   cnt++;if(cnt >= Pixmap_Max)memcpy(&count_flag,__interrpt_clear__,sizeof(rainbow));}
        else
        {   cnt--;if(!cnt)memcpy(&count_flag,__this,sizeof(buffer));}
        color_mode_display(cnt,cnt,0);
}
void hsv_mode_single_light_blue_breathing(void)         //单色呼吸-蓝色
{
        if(count_flag)
        {   cnt++;if(cnt >= Pixmap_Max)memcpy(&count_flag,__interrpt_clear__,sizeof(rainbow));}
        else
        {   cnt--;if(!cnt)memcpy(&count_flag,__this,sizeof(buffer));}
        color_mode_display(0,0,cnt);
}
void hsv_mode_single_light_purple_breathing(void)      //单色呼吸-紫色
{
        if(count_flag)
        {   cnt++;if(cnt >= Pixmap_Max)memcpy(&count_flag,__interrpt_clear__,sizeof(rainbow));}
        else
        {   cnt--;if(!cnt)memcpy(&count_flag,__this,sizeof(buffer));}
        color_mode_display(cnt,0,cnt);
}
#endif
