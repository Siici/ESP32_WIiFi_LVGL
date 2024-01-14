#ifndef _ui_task_h_
#define _ui_task_h_

#include "allhead.h"


// 创建UI进程
void lvgl_PinnedToCore(void);
// 任务回调函数 ---> ui进程 
void Guitask(void *param);  // 显示ui界面

/***********************************************************************/
// 屏幕1添加的事件
void Screen1_lv_obj_add_event_cb(void);

// 编辑模式
void ui_event_Screen1_Edi(lv_event_t *e);
// 编辑模式1 隐藏键盘 显示按钮
void ui_event_Screen1_Edi1(lv_event_t *e);

// 把选中的WiFi名 赋值给 头部标签
void popupPWMsgBox();

// 获取消息框文本的回调函数
void mbox_event_handler(lv_event_t *e);

/***********************************************************************/



#endif