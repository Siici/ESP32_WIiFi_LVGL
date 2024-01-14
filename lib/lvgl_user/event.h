#ifndef _event_h_
#define _event_h_

#include "ui.h"


// 屏幕1添加的事件
void Screen1_lv_obj_add_event_cb(void);

// 回调函数 ---> 获取下拉框的文本值
void Get_Dropdown1_Text_event_handler(lv_event_t *e);
void popupPWMsgBox();

// 回调函数 ---> 获取消息框文本值
void Get_TextArea1_Text_event_handler(lv_event_t *e);



#endif