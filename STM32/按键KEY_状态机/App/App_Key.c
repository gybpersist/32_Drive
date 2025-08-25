#include "App_Key.h"

void App_Key_Handle(void)
{
    Key_Event key_event = Int_Key_Scan();

    // 1 处理按键事件
    switch (key_event)
    {
    case KEY_EVENT_KEY1:
        DEBUG_PRINTLN("KEY1 被按下...");
        
        break;
    case KEY_EVENT_KEY2:
        DEBUG_PRINTLN("KEY2 被按下...");
        
        break;
    case KEY_EVENT_KEY3:
        DEBUG_PRINTLN("KEY3 被按下...");
        
        break;
    case KEY_EVENT_KEY4:
        
    }

}
