/*
   Project: N|Watch
   Author: Zak Kemble, contact@zakkemble.co.uk
   Copyright: (C) 2013 by Zak Kemble
   License: GNU GPL v3 (see License.txt)
   Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
*/

#include "common.h"

#define BTN_IS_PRESSED	4
#define BTN_NOT_PRESSED	4

typedef struct
{
    millis_t pressedTime;	// Time of press
    bool processed;			// Time of press has been stored (don't store again until next press)
    byte counter;			// 消抖计数
    bool funcDone;			// Function has been ran (don't run again until next press)
    button_f onPress;		// Function to run when pressed
    const uint* tune;		// 蜂鸣器
} s_button;

static s_button buttons[BTN_COUNT];

static void processButtons(void);
static void processButton(s_button*, BOOL);
static byte bitCount(byte);

void buttons_init()
{
    buttons_startup();
}

// 按键初始化函数
void buttons_startup(void)
{
    KEY_Init();
}

void buttons_update()
{
    static millis8_t lastUpdate;

    // Update every 10ms
    millis8_t now = millis();

    if ((millis8_t)(now - lastUpdate) >= 10)
    {
        lastUpdate = now;
        processButtons();
    }
}




// Sets button pins to OUTPUT LOW
// This stops them wasting current through the pull-up resistor when pressed
void buttons_shutdown()
{

}

static void processButtons(void)
{
    // Get button pressed states
    BOOL isPressed[BTN_COUNT];

    isPressed[BTN_1] = KEY1;
    isPressed[BTN_2] = KEY2;
    isPressed[BTN_3] = KEY0;
    isPressed[BTN_4] = KEY3;
    
    

    // Process each button
    LOOPR(BTN_COUNT, i)
    processButton(&buttons[i], !isPressed[i]);
}

static void processButton(s_button* button, BOOL isPressed)
{
    button->counter <<= 1;
    if (isPressed)
    {
        led_flash(LED_GREEN, LED_FLASH_FAST, LED_BRIGHTNESS_MAX);
        // Set debounce counter bit
        button->counter |= 1;

        // Are enough bits set to count as pressed?
        if (bitCount(button->counter) >= BTN_IS_PRESSED) // 按键消抖
        {
            // Store time of press
            if (!button->processed)
            {
                button->pressedTime = millis();
                button->processed = true;
            }

            // Run function
            if (!button->funcDone && button->onPress != NULL && button->onPress())
            {
                button->funcDone = true;

                led_flash(LED_GREEN, LED_FLASH_FAST, LED_BRIGHTNESS_MAX);
            }
        }
    }
    else // Not pressed
    {
        // Has button been not pressed for long enough?
        if (bitCount(button->counter) <= BTN_NOT_PRESSED)
        {
            button->processed = false;
            button->funcDone = false;
        }
    }
}

// Count set bits in value
static byte bitCount(byte val)
{
    byte count = 0;

    for (; val; val >>= 1)
        count += val & 1;

    return count;
}

// Set new function to run when button is pressed and return the old function
button_f buttons_setFunc(btn_t btn, button_f func)
{
    button_f old = buttons[btn].onPress;
    buttons[btn].onPress = func;
    return old;
}

// Set functions to run for each button
void buttons_setFuncs(button_f btn1, button_f btn2, button_f btn3)
{
    buttons[BTN_1].onPress = btn1; 
    buttons[BTN_2].onPress = btn2; 
    buttons[BTN_3].onPress = btn3; 
}


// See if a button has been pressed in the past x milliseconds
bool buttons_isActive()
{

    if (!appConfig.sleepTimeout)
        return true;

    uint timeout = (appConfig.sleepTimeout * 5) * 1000 * 3;
//  uint timeout =  1000;

    // See if a button has been pressed within that timeout
    LOOPR(BTN_COUNT, i)
    {
        if (millis() - buttons[i].pressedTime < timeout)
            return true;
    }

    return false;
}

// Set button status to pressed, processed etc but don't run their functions
void buttons_wake()
{
    LOOPR(BTN_COUNT, i)
    {
        buttons[i].funcDone		= true;
        buttons[i].processed	= true;
        buttons[i].counter		= BTN_IS_PRESSED;
        buttons[i].pressedTime	= millis();
    }
}
