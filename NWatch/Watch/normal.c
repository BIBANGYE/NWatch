#include "common.h"

#define TIME_POS_X	1
#define TIME_POS_Y	20
#define TICKER_GAP	4

typedef struct
{
    byte x;
    byte y;
    const byte* bitmap;
    byte w;
    byte h;
    byte offsetY;
    byte val;
    byte maxVal;
    bool moving;
} tickerData_t;


static bool down(void);
static bool up(void);
static display_t draw(void);
static void drawDate(void);
#if COMPILE_ANIMATIONS
    static bool animateIcon(bool, byte*);
#endif
static display_t ticker(void);
static void drawTickerNum(tickerData_t*);

void watchface_normal()
{
    display_setDrawFunc(draw);
//    buttons_setFuncs(up, menu_select, down);
//    animation_start(NULL, ANIM_MOVE_ON);
}

static bool down()
{
    // Set watchface
    display_set(watchfaceOpen);
    display_load();

    return true;
}

static bool up()
{
    // Set watchface
    display_set(watchfaceOpen);
    display_load();

    return true;
}


static display_t draw()
{
    #if COMPILE_ANIMATIONS
    static byte usbImagePos = FRAME_HEIGHT;
    static byte chargeImagePos = FRAME_HEIGHT;
    #endif


    drawDate(); // 日期

    display_t busy ; // Draw time animated

    busy = ticker();

    drawBattery(); // Draw battery icon

    byte x = 20;

    #if COMPILE_ANIMATIONS

    if(animateIcon(UDADDR != 0, &usbImagePos))
    {
        draw_bitmap(0, 0, usbIcon, 16, 8, NOINVERT, 0);
        x += 20;
    }

    #else

    if(UDADDR != 0)
    {
        draw_bitmap(x, FRAME_HEIGHT - 9, usbIcon, 16, 8, NOINVERT, 0);
        x += 20;
    }

    #endif

    // Draw charging icon
    #if COMPILE_ANIMATIONS

    if(animateIcon(1, &chargeImagePos))
    {
        draw_bitmap(20, 0, chargeIcon, 8, 8, NOINVERT, 0);
        x += 12;
    }

    #else

    if(CHARGING())
    {
        draw_bitmap(x, FRAME_HEIGHT - 9, chargeIcon, 8, 8, NOINVERT, 0);
        x += 12;
    }

    #endif

    #if COMPILE_STOPWATCH
//    if(stopwatch_active())     // 秒表
    {
        draw_bitmap(40, 0, stopwatch, 8, 8, NOINVERT, 0);
        x += 12;
    }
    #endif
    return busy;
}

static void drawDate()
{
    // Get day string
    char day[BUFFSIZE_STR_DAYS];
    strcpy(day, days[timeDate.date.day]); // 星期

    // Get month string
    char month[BUFFSIZE_STR_MONTHS];
    strcpy(month, months[timeDate.date.month]); // 月份


    // Draw date
    char buff[BUFFSIZE_DATE_FORMAT];
    //                                 星期    日期    月份  年份

    timeDate.date.date = 3;
//    timeDate.date.year = 22;
    sprintf_P(buff, PSTR(DATE_FORMAT), day, timeDate.date.date, month, timeDate.date.year);
    draw_string(buff, false, 20, 0);
}

#if COMPILE_ANIMATIONS
static bool animateIcon(bool active, byte* pos)
{
    byte y = *pos;

    if(active || (!active && y < FRAME_HEIGHT))
    {
        if(active && y > FRAME_HEIGHT - 8)
            y -= 1;
        else if(!active && y < FRAME_HEIGHT)
            y += 1;

        *pos = y;
        return true;
    }

    return false;
}
#endif

static display_t ticker()
{
    static byte yPos;
    static byte yPos_secs;
    static bool moving = false;
    static bool moving2[5];

    #if COMPILE_ANIMATIONS
    static byte hour2;
    static byte mins;
    static byte secs;

    if(appConfig.animations)
    {
        if(timeDate.time.secs != secs)
        {
            yPos = 0;
            yPos_secs = 0;
            moving = true;

            // 时间变化移动标志
            moving2[0] = div10(timeDate.time.hour) != div10(hour2);
            moving2[1] = mod10(timeDate.time.hour) != mod10(hour2);
            moving2[2] = div10(timeDate.time.mins) != div10(mins);
            moving2[3] = mod10(timeDate.time.mins) != mod10(mins);
            moving2[4] = div10(timeDate.time.secs) != div10(secs);

            hour2 = timeDate.time.hour;
            mins = timeDate.time.mins;
            secs = timeDate.time.secs;
        }

        //时钟显示动画
        if(moving)
        {
            if(yPos <= 3)
                yPos++;
            else if(yPos <= 6)
                yPos += 3;
            else if(yPos <= 16)
                yPos += 5;
            else if(yPos <= 22)
                yPos += 3;
            else if(yPos <= 24 + TICKER_GAP)
                yPos++;

            if(yPos >= MIDFONT_HEIGHT + TICKER_GAP)
                yPos = 255;

            if(yPos_secs <= 1)
                yPos_secs++;
            else if(yPos_secs <= 13)
                yPos_secs += 3;
            else if(yPos_secs <= 16 + TICKER_GAP)
                yPos_secs++;

            if(yPos_secs >= FONT_SMALL2_HEIGHT + TICKER_GAP)
                yPos_secs = 255;

            if(yPos_secs > FONT_SMALL2_HEIGHT + TICKER_GAP && yPos > MIDFONT_HEIGHT + TICKER_GAP)
            {
                yPos = 0;
                yPos_secs = 0;
                moving = false;
                memset(moving2, false, sizeof(moving2));
            }
        }
    }
    else
    #endif
    {
        yPos = 0;
        yPos_secs = 0;
        moving = false;
        memset(moving2, false, sizeof(moving2));
    }

    tickerData_t data;

    // Seconds
    data.x = 104;
    data.y = 28;
    data.bitmap = (const byte*)&small2Font;
    data.w = FONT_SMALL2_WIDTH;
    data.h = FONT_SMALL2_HEIGHT;
    data.offsetY = yPos_secs;
    data.val = div10(timeDate.time.secs);
    data.maxVal = 5;
    data.moving = moving2[4];
    drawTickerNum(&data);

    data.x = 116;
    data.val = mod10(timeDate.time.secs);
    data.maxVal = 9;
    data.moving = moving;
    drawTickerNum(&data);

    // Set new font data for hours and minutes
    data.y = TIME_POS_Y;
    data.w = MIDFONT_WIDTH;
    data.h = MIDFONT_HEIGHT;
    data.bitmap = (const byte*)&midFont;
    data.offsetY = yPos;

    // Minutes
    data.x = 60;
    data.val = div10(timeDate.time.mins);
    data.maxVal = 5;
    data.moving = moving2[2];
    drawTickerNum(&data);

    data.x = 83;
    data.val = mod10(timeDate.time.mins);
    data.maxVal = 9;
    data.moving = moving2[3];
    drawTickerNum(&data);

    // Hours
    data.x = 1;
    data.val = div10(timeDate.time.hour);
    data.maxVal = 5;
    data.moving = moving2[0];
    drawTickerNum(&data);

    data.x = 24;
    data.val = mod10(timeDate.time.hour);
    data.maxVal = 9;
    data.moving = moving2[1];
    drawTickerNum(&data);

    // Draw colon for half a second   画半秒的冒号
    if((milliseconds % 1000)  > 500 ) // 假装是半秒钟  500ms
    {
        draw_bitmap(TIME_POS_X + 46 + 2, TIME_POS_Y, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT, NOINVERT, 0);
    }


    // Draw AM/PM character
    char tmp[2];
//    tmp[0] = timeDate.time.ampm;
    tmp[0] = CHAR_PM ;
    tmp[1] = 0x00;
    draw_string(tmp, false, 104, 20);

    return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
}

static void drawTickerNum(tickerData_t* data)
{
    byte arraySize = (data->w * data->h) / 8;
    byte yPos = data->offsetY;
    const byte* bitmap = &data->bitmap[data->val * arraySize];
    byte x = data->x;
    byte y = data->y;

    if(!data->moving || yPos == 0 || yPos == 255)
        draw_bitmap(x, y, bitmap, data->w, data->h, NOINVERT, 0);
    else
    {
        byte prev = data->val - 1;

        if(prev == 255)
            prev = data->maxVal;

        draw_bitmap(x, y, bitmap, data->w, data->h, NOINVERT, yPos - data->h - TICKER_GAP);
        draw_bitmap(x, y, &data->bitmap[prev * arraySize], data->w, data->h, NOINVERT, yPos);
    }
}
