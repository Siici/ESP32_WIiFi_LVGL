#include "event.h"
#include "wifitask.h"
#include "WString.h"
/*****************************下面为 屏幕1 事件回调函数 ************************************/

// 组的指针
extern lv_group_t *group1;
// wifi 名称 密码
String ssidName, password;

// 判断组1 是否为编辑模式
void ui_event_Screen1_Edi(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_LONG_PRESSED)
    {
        // 判断当前是否是编辑模式若是则退出若不是则进入
        uint8_t en_val = lv_group_get_editing(group1) ? 0 : 1;
        lv_group_set_editing(group1, en_val);
    }
}

// 编辑模式1 隐藏键盘 显示按钮
void ui_event_Screen1_Edi1(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_LONG_PRESSED)
    {
        // 判断当前是否是编辑模式若是则退出若不是则进入
        uint8_t en_val = lv_group_get_editing(group1) ? 0 : 1;
        lv_group_set_editing(group1, en_val); // 切换模式

        if (0 == en_val) // 1 为编辑模式  0 为导航模式 只要不是编辑模式就隐藏键盘
        {
            _ui_flag_modify(ui_Keyboard1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_TOGGLE); // 隐藏键盘
            _ui_flag_modify(ui_Panel2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);    // 显示按钮
            lv_obj_clear_state(ui_Button2, LV_STATE_ANY);                              // 清除按钮 所有状态
            lv_label_set_text(ui_Label2, "connect");                                   // 设置本值为 connect
        }
    }
}

// 回调函数 ---> 获取下拉框的文本值
void Get_Dropdown1_Text_event_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];                                                 // 定义一个字符数组buf
        lv_dropdown_get_selected_str(ui_Dropdown1, buf, sizeof(buf)); // 从名为ui_Dropdown1的下拉框中获取当前选中的字符串，并存储在buf中。
        ssidName = String(buf);                                       // 将字符数组buf转换为字符串对象，并赋值给变量ssidName。

        // 提取WiFi名
        for (int i = 0; i < ssidName.length() - 1; i++) // 遍历字符串ssidName的每一个字符，直到倒数第二个字符。
        {
            if (ssidName.substring(i, i + 2) == " (") // 遍历字符串ssidName的每一个字符，直到倒数第二个字符。
            {
                ssidName = ssidName.substring(1, i); // 如果子字符串为"("，则从索引1到索引i的子字符串赋值给ssidName
                break;
            }
        }
        popupPWMsgBox();
    }
}

// 清空文本输入框 清空头部标签 把选中的WiFi名 赋值给 头部标签
void popupPWMsgBox()
{
    if (ssidName == NULL || ssidName.length() == 0)
    {
        return;
    }

    lv_label_set_text(ui_Label1, "");               // 清除头标签
    lv_textarea_set_text(ui_TextArea1, "");         // 清空文本输入框
    lv_label_set_text(ui_Label1, ssidName.c_str()); // 设置头部标签 内容为ssidName的值。
}

// 回调函数 ---> 获取消息框文本值
void Get_TextArea1_Text_event_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        password = lv_textarea_get_text(ui_TextArea1);
        password.trim(); //  函数来删除password字符串两端的空白字符,为了确保密码不包含任何前导或尾随的空格。
    }
}

// 按钮连接WiFi 回调函数事件
void connectWIFI_event(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        if (strcmp(lv_label_get_text(ui_Label2), "cancel") == 0)  // cancel
        {
            connectWIFI();
        }
        else if (strcmp(lv_label_get_text(ui_Label2), "connect") == 0)  // connect
        {
            Cancel_WiFi(); // 断开连接WiFi
        }
        
    }
}

/*****************************上面为 屏幕1 事件回调函数 ************************************/

// 屏幕1添加的事件
void Screen1_lv_obj_add_event_cb(void)
{
    // 编辑模式回调函数
    lv_obj_add_event_cb(ui_Dropdown1, ui_event_Screen1_Edi, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_add_event_cb(ui_Keyboard1, ui_event_Screen1_Edi1, LV_EVENT_LONG_PRESSED, NULL);

    // 当下拉框的值改变时 获取下拉框的文本值
    lv_obj_add_event_cb(ui_Dropdown1, Get_Dropdown1_Text_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // 当文本输入区域的值改变时 获取文本输入区域的文本值
    lv_obj_add_event_cb(ui_TextArea1, Get_TextArea1_Text_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // 按钮连接WiFi 回调函数事件
    lv_obj_add_event_cb(ui_Button2, connectWIFI_event, LV_EVENT_CLICKED, NULL);
}
