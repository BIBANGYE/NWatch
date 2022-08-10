/******************************************************************************
需要实现的函数：
void button_init(void)
uint8_t scanf_button(void)
uint32_t millis(void)


*/


#include "OneButton.h"

uint8_t _state = OCS_INIT;
uint8_t _lastState = OCS_INIT;

int _maxClicks = 3; // 支持的多连击次数
unsigned int _debounceTicks = 50; // 消抖时间 ms
unsigned int _clickTicks = 400;   // 400ms以上为单击
unsigned int _pressTicks = 800;   // 800ms以上为长按

uint32_t _startTime;        // start of current input change to checking debouncing
int _nClicks;               // count the number of clicks with this variable


// These variables will hold functions acting as event source.
callbackFunction _clickFunc = NULL;
parameterizedCallbackFunction _paramClickFunc = NULL;
void *_clickFuncParam = NULL;

callbackFunction _doubleClickFunc = NULL;
parameterizedCallbackFunction _paramDoubleClickFunc = NULL;
void *_doubleClickFuncParam = NULL;

callbackFunction _multiClickFunc = NULL;
parameterizedCallbackFunction _paramMultiClickFunc = NULL;
void *_multiClickFuncParam = NULL;

callbackFunction _longPressStartFunc = NULL;
parameterizedCallbackFunction _paramLongPressStartFunc = NULL;
void *_longPressStartFuncParam = NULL;

callbackFunction _longPressStopFunc = NULL;
parameterizedCallbackFunction _paramLongPressStopFunc = NULL;
void *_longPressStopFuncParam;

callbackFunction _duringLongPressFunc = NULL;
parameterizedCallbackFunction _paramDuringLongPressFunc = NULL;
void *_duringLongPressFuncParam = NULL;



static uint32_t b_millis(void);
static void button_reset(void);
static uint8_t scanf_button(void);
static void new_state(uint8_t nextstate);
static void button_tick(uint8_t activeLevel);


/*******************************************************************************/

// 按键初始化
void button_init(void)
{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};

//    __HAL_RCC_GPIOA_CLK_ENABLE();

//    GPIO_InitStruct.Pin =  GPIO_PIN_15;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 按键扫描
static uint8_t scanf_button(void)
{
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15))
    {
        return BUTTON_FREE;
    }
    else
    {
        return BUTTON_PRESS;
    }
}

static uint32_t b_millis(void)
{
    uint32_t ticks = HAL_GetTick() ;
    return (ticks);
}
/*******************************************************************************/



// 在定时器中断中调用
void Button_Tick_IRQHandler(void)
{
    button_tick(scanf_button() == BUTTON_PRESS);
}

// 设置按键消抖时间
void set_debounce_ticks(const int ticks)
{
    _debounceTicks = ticks;
}
// 设置单击时间
void set_click_ticks(const int ticks)
{
    _clickTicks = ticks;
}
// 设置长按时间
void set_press_ticks(const int ticks)
{
    _pressTicks = ticks;
}

// save function for click event
void attachClick(callbackFunction newFunction)
{
    _clickFunc = newFunction;
}

// save function for parameterized click event
void attachClickParam(parameterizedCallbackFunction newFunction, void *parameter)
{
    _paramClickFunc = newFunction;
    _clickFuncParam = parameter;
}

// save function for doubleClick event
void attachDoubleClick(callbackFunction newFunction)
{
    _doubleClickFunc = newFunction;
    _maxClicks = 2;
} // attachDoubleClick

// save function for parameterized doubleClick event
void attachDoubleClickParam(parameterizedCallbackFunction newFunction, void *parameter)
{
    _paramDoubleClickFunc = newFunction;
    _doubleClickFuncParam = parameter;
    _maxClicks = 2;
}

// save function for multiClick event
void attachMultiClick(callbackFunction newFunction)
{
    _multiClickFunc = newFunction;
    _maxClicks = _maxClicks;
} // attachMultiClick

// save function for parameterized MultiClick event
void attachMultiClickParam(parameterizedCallbackFunction newFunction, void *parameter)
{
    _paramMultiClickFunc = newFunction;
    _multiClickFuncParam = parameter;
    _maxClicks = _maxClicks;
} // attachMultiClick

// save function for longPressStart event
void attachLongPressStart(callbackFunction newFunction)
{
    _longPressStartFunc = newFunction;
} // attachLongPressStart

// save function for parameterized longPressStart event
void attachLongPressStartParam(parameterizedCallbackFunction newFunction, void *parameter)
{
    _paramLongPressStartFunc = newFunction;
    _longPressStartFuncParam = parameter;
}

// save function for longPressStop event
void attachLongPressStop(callbackFunction newFunction)
{
    _longPressStopFunc = newFunction;
} // attachLongPressStop

// save function for parameterized longPressStop event
void attachLongPressStopParam(parameterizedCallbackFunction newFunction, void *parameter)
{
    _paramLongPressStopFunc = newFunction;
    _longPressStopFuncParam = parameter;
} // attachLongPressStop

// save function for during longPress event
void attachDuringLongPress(callbackFunction newFunction)
{
    _duringLongPressFunc = newFunction;
} // attachDuringLongPress

// save function for parameterized during longPress event
void attachDuringLongPressParam(parameterizedCallbackFunction newFunction, void *parameter)
{
    _paramDuringLongPressFunc = newFunction;
    _duringLongPressFuncParam = parameter;
} // attachDuringLongPress




// return number of clicks in any case: single or multiple clicks
int get_number_clicks(void)
{
    return _nClicks;
}




static void button_tick(uint8_t activeLevel)
{
    uint32_t now = b_millis(); // current (relative) time in msecs.
    uint32_t waitTime = (now - _startTime);

    switch(_state)
    {
        case OCS_INIT:
            if(activeLevel)
            {
                new_state(OCS_DOWN);
                _startTime = now; // remember starting time
                _nClicks = 0;
            }
            break;

        case OCS_DOWN:

            // 消抖时间
            if((!activeLevel) && (waitTime < _debounceTicks))
            {
                new_state(_lastState);
            }
            else if(!activeLevel)
            {
                new_state(OCS_UP);
                _startTime = now;
            }
            else if((activeLevel) && (waitTime > _pressTicks)) // 长按
            {
                // 按键按下
                new_state(LONG_PRESS_START);
            }

            break;

        case OCS_UP:
            if((activeLevel) && (waitTime < _debounceTicks))
            {
                new_state(_lastState);
            }
            else if ((waitTime >= _debounceTicks))
            {
                // count as a short button down
                _nClicks++;
                new_state(OCS_COUNT);
            }

            break;

        case OCS_COUNT:
            if (activeLevel)
            {
                new_state(OCS_DOWN);
                _startTime = now;
            }
            else if ((waitTime > _clickTicks) || (_nClicks == _maxClicks))
            {
                if (_nClicks == 1)
                {

                }
                else if (_nClicks == 2)
                {
                    // this was a 2 click sequence.

                }
                else // 多连击
                {
                    // this was a multi click sequence.

                }

                button_reset();
            }

            break;

        case OCS_PRESS:

            // 等待长按结束
            if (!activeLevel)
            {

                new_state(OCS_PRESSEND);
                _startTime = now;
            }
            else
            {
                // still the button is pressed

            }

            break;

        case OCS_PRESSEND:

            // button was released.
            if ((activeLevel) && (waitTime < _debounceTicks))
            {
                // button was released to quickly so I assume some bouncing.
                new_state(_lastState); // go back
            }
            else if (waitTime >= _debounceTicks)
            {

                button_reset();
            }

            break;

        default:
            // unknown state detected -> reset state machine
            new_state(OCS_INIT);
            break;
    }
}

static void new_state(uint8_t nextstate)
{
    _lastState = _state;
    _state = nextstate;
}

static void button_reset(void)
{
    _state = OCS_INIT;
    _lastState = OCS_INIT;
    _nClicks = 0;
    _startTime = 0;
}






