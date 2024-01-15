#include "lvgl_task.h"

/************************************************************************************************/
// 屏幕参数配置
static const uint16_t screenWidth = 128;
static const uint16_t screenHeight = 160;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

/***********************************************************************************************/

/***********************************************************************************************/
// 按键引脚定义 LED引脚定义
#define PinA 19 // 左键引脚
#define PinB 3  // 右键引脚
#define PinC 9  // 确定键引脚

/***********************************************************************************************/

/***********************************************************************************************/

// 按键注册返回值
lv_indev_t *indev_button;

// 组的指针
lv_group_t *group1;

// 控件声明
// 屏幕1
extern lv_obj_t *ui_Dropdown1;
extern lv_obj_t *ui_Keyboard1;
extern lv_obj_t *ui_TextArea1;
extern lv_obj_t * ui_Button2;

/************************************************************************************************/

/************************************************************************************************/
/*                                 函数由此开始                                                  */
/************************************************************************************************/

// 按键初始化函数
void my_key_init()
{
    pinMode(PinC, INPUT_PULLUP);
    pinMode(PinA, INPUT_PULLUP);
    pinMode(PinB, INPUT_PULLUP);
}


// 按键扫描函数，用于区分哪个按键按下了
uint8_t Key_Scan()
{

    if (digitalRead(PinC) == LOW) // 确认键
    {         
        return 1;
    }

    if (digitalRead(PinA) == LOW)
    {
        // 编辑模式状态按键返回值
        if (lv_group_get_editing(group1) == 1)
        {
            return 4;
        }
        return 2;
    }
    if (digitalRead(PinB) == LOW)
    {
        // 编辑模式状态按键返回值
        if (lv_group_get_editing(group1) == 1)
        {
            return 5;
        }
        return 3;
    }

    return 0; // 没按下返回0
}

// 按键状态更新函数
void my_key_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    static uint32_t last_key = 0;
    uint8_t act_enc = Key_Scan();

    if (act_enc != 0)
    {
        switch (act_enc)
        {
        case 1:
            act_enc = LV_KEY_ENTER; // 确认键
            data->state = LV_INDEV_STATE_PRESSED;
            break;
        case 2:
            act_enc = LV_KEY_PREV; // 聚焦到上一个
            data->state = LV_INDEV_STATE_PRESSED;
            break;
        case 3:
            act_enc = LV_KEY_NEXT; // 聚焦到下一个
            data->state = LV_INDEV_STATE_PRESSED;
            break;
        case 4:
            act_enc = LV_KEY_LEFT; // 减少或向左
            data->state = LV_INDEV_STATE_PRESSED;
            break;
        case 5:
            act_enc = LV_KEY_RIGHT; // 增加或向右
            data->state = LV_INDEV_STATE_PRESSED;
            break;
        }
        last_key = (uint32_t)act_enc;
    }
    data->key = last_key;
}


// 刷新显示回调函数
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp_drv);
}

// 输入设备初始化函数
void lv_port_indev_init(void)
{
    // 初始化按键
    my_key_init();
    // 注册输入设备
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv); // 初始化输入设备
    // 注册的按键类型
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = my_key_read; // 按键控制回调函数
    indev_button = lv_indev_drv_register(&indev_drv);
}

// 组管理
void Group_Init(void)
{
    group1 = lv_group_create(); // 创建分组

    // 添加控件到组1中
    lv_group_add_obj(group1, ui_Dropdown1);
    lv_group_add_obj(group1, ui_Keyboard1);
    lv_group_add_obj(group1, ui_TextArea1);
    lv_group_add_obj(group1, ui_Button2);   
    
    // 默认与组1绑定
    lv_indev_set_group(indev_button, group1);
    lv_group_focus_obj(ui_Dropdown1);
}

/// lvgl 初始化
void lvgl_Init(void)
{
    lv_init();
    tft.begin();        /* TFT init */
    tft.setRotation(0); /* Landscape orientation, flipped */

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    lv_port_indev_init(); // 按键注册
}
