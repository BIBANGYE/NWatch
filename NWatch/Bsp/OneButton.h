#ifndef __ONEBUTTON_H
#define __ONEBUTTON_H

#include "include.h"

extern uint8_t button_click_state;

#define BUTTON_PRESS 1 /* 按键按下 */
#define BUTTON_FREE  0 /* 按键释放 */

typedef void (*callbackFunction)(void);
typedef void (*parameterizedCallbackFunction)(void *);

typedef struct
{
    uint8_t _state;
    uint8_t _lastState;
} BUTTON_STATE_T;

// 有限状态机
typedef enum
{
    OCS_INIT = 0,
    OCS_DOWN = 1,
    OCS_UP = 2,
    OCS_COUNT = 3,
    OCS_PRESS = 6,
    OCS_PRESSEND = 7,
    UNKNOWN = 99
} stateMachine_e;

typedef enum
{
    CLICK = 1,
    DOUBLE_CLICK = 2,
    MULTI_CLICK = 3,
    LONG_PRESS_START =4,
    DURING_LONG_PRESS =5,
    LONG_PRESS_STOP =6,
    NONE = 99
}BUTTON_STATE_e;

void button_init(void);
void Button_Tick_IRQHandler(void);
void set_debounce_ticks(const int ticks);
void set_click_ticks(const int ticks);
void set_press_ticks(const int ticks);
int get_number_clicks(void);

void write_fifo(BUTTON_STATE_e state);
BUTTON_STATE_e read_fifo(void);

void attachClick(callbackFunction newFunction);
void attachClickParam(parameterizedCallbackFunction newFunction, void *parameter);
void attachDoubleClick(callbackFunction newFunction);
void attachDoubleClickParam(parameterizedCallbackFunction newFunction, void *parameter);
void attachMultiClick(callbackFunction newFunction);
void attachMultiClickParam(parameterizedCallbackFunction newFunction, void *parameter);
void attachLongPressStart(callbackFunction newFunction);
void attachLongPressStartParam(parameterizedCallbackFunction newFunction, void *parameter);
void attachLongPressStop(callbackFunction newFunction);
void attachLongPressStopParam(parameterizedCallbackFunction newFunction, void *parameter); 
void attachDuringLongPress(callbackFunction newFunction);
void attachDuringLongPressParam(parameterizedCallbackFunction newFunction, void *parameter);
#endif

