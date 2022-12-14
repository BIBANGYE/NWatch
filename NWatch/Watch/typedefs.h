/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdbool.h>
#include <stdint.h>
#include "common.h"

#if defined ( __CC_ARM   )
    #pragma anon_unions
#endif

typedef uint8_t  byte;
typedef uint16_t uint;
typedef uint32_t ulong;

typedef uint8_t  u8;
typedef uint16_t u16;


extern byte oledBuffer[];

extern uint32_t  BKPDataRR[];



typedef unsigned char millis8_t;
typedef unsigned int  millis_t;

// bool from stdbool does extra stuff to make the value
// always either 0 or 1, which isn't needed most of the time.
// So here's BOOL which doesn't do that.
typedef	uint8_t BOOL;

typedef uint32_t timestamp_t;

typedef enum
{
    DISPLAY_DONE,
    DISPLAY_BUSY,
} display_t; //	DISPLAY_TOOFAST

typedef enum
{
    MENU_TYPE_STR,
    MENU_TYPE_ICON
} menu_type_t;

typedef enum
{
    MONTH_JAN = 0,
    MONTH_FEB = 1,
    MONTH_MAR = 2,
    MONTH_APR = 3,
    MONTH_MAY = 4,
    MONTH_JUN = 5,
    MONTH_JUL = 6,
    MONTH_AUG = 7,
    MONTH_SEP = 8,
    MONTH_OCT = 9,
    MONTH_NOV = 10,
    MONTH_DEC = 11
} month_t;

typedef enum
{
    DAY_MON = 0,
    DAY_TUE = 1,
    DAY_WED = 2,
    DAY_THU = 3,
    DAY_FRI = 4,
    DAY_SAT = 5,
    DAY_SUN = 6,
} day_t;

typedef struct
{
    byte secs;
    byte mins;
    byte hour;
    char ampm;
} time_s;

typedef struct
{
    day_t day; //星期
    byte date; //日期
    month_t month; //月份
    byte year; //年份
} date_s;

typedef struct
{
    time_s time;
    date_s date;
} timeDate_s;

typedef enum
{
    TIMEMODE_24HR = 0,
    TIMEMODE_12HR = 1
} timemode_t;

typedef struct
{
    byte hour;
    byte min;
    union
    {
        byte days;
        struct  // get rid of these bitfields?
        {
            bool mon: 1;
            bool tues: 1;
            bool wed: 1;
            bool thurs: 1;
            bool fri: 1;
            bool sat: 1;
            bool sun: 1;
            bool enabled: 1;
        };
    };
} alarm_s;



typedef display_t (*draw_f)(void);
typedef void (*display_f)(void);

// Function for buttons to call
// Return true to only call function once per press
// Return false to continually call function while the button is pressed
typedef bool (*button_f)(void);

typedef void (*menu_f)(void);
typedef void (*itemLoader_f)(byte);

typedef struct
{
    menu_f btn1; // make array
    menu_f btn2;
    menu_f btn3;
    draw_f draw;
    itemLoader_f loader;
} menuFuncs_t;

typedef struct
{
    byte selected;
    byte scroll;
    byte optionCount;
    bool isOpen;
    const char* title;
    menu_type_t menuType;
    menuFuncs_t func;
    menu_f prevMenu;
} menu_s;

typedef struct
{
    byte lastSelected;
    menu_f last;
} prev_menu_s;

typedef struct
{
    bool active;  // 使能动画
    byte offsetY; // 偏移
    void (*animOnComplete)(void); //动画完成，执行函数
    bool goingOffScreen; //向上移出屏幕
} anim_s;

typedef struct
{
    byte x;
    byte y;
    const byte* bitmap;
    byte width;
    byte height;
    bool invert;
    byte offsetY;
} image_s;

#endif /* TYPEDEFS_H_ */
