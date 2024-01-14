#include "ui_task.h"

// 任务回调函数 ---> ui进程
void Guitask(void *param)
{
    lvgl_Init();  // lvgl相关初始化
    ui_init();    // ui初始化 调用ui
    Group_Init(); // 组管理
    while (1)
    {
        lv_timer_handler(); /* let the GUI do its work */
        delay(5);
    }
}

// 创建UI进程
void lvgl_PinnedToCore(void)
{
    xTaskCreatePinnedToCore(
        Guitask,   /* 任务回调函数的指针 */
        "Guitask", /* 任务名 最多16字节 */
        4 * 4096,  /* 任务栈大小，根据需要自行设置*/
        NULL,      /* 创建任务的参数，会传给任务函数的形参 传递入参时,需传递该变量的地址并强转为void *型，否则无法传递，编译报错 */
        2,         /* 任务优先级 (0~24) 数字越大优先级越高*/
        NULL,      /* 任务句柄 */
        1);        /* CPU1 */
}
