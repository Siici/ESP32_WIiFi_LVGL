#include "wifitask.h"

unsigned long timeout = 5000; // 5sec

/* 创建连接WiFi和搜索WiFi的句柄，并初始化 */
TaskHandle_t WIFIConnect;
TaskHandle_t WIFISerch;

// wifi 名称 密码
extern String ssidName;
extern String password;

// 创建搜索WiFi任务
void networkScanner()
{
    vTaskDelay(500);
    xTaskCreatePinnedToCore(
        scanWIFITask, /* 任务输入函数的指针 */
        "WIFISerch",  /* 任务名 */
        8 * 1024,     /* 任务栈大小*/
        NULL,         /* 参数为变量，传递入参时,需传递该变量的地址并强转为void *型，否则无法传递，编译报错 */
        1,            /* 优先级 */
        &WIFISerch,   /* 任务句柄 */
        0);           /* CPU0 */
}

// 搜索WiFi ---> 回调函数 添加到 下拉列表
void scanWIFITask(void *pvParameters)
{
    vTaskDelay(1000);
    while (1)
    {
        int n = WiFi.scanNetworks(); // 搜索WiFi数量
        if (n <= 0)
        {
            // 若搜索到的WiFi 数量为0 则在下拉列表显示 Not Found WIFI
            lv_dropdown_add_option(ui_Dropdown1, "Not Found WIFI", LV_DROPDOWN_POS_LAST);
        }
        else
        {
            // 清除下拉列表
            lv_dropdown_clear_options(ui_Dropdown1);
            vTaskDelay(10);

            for (int i = 0; i < n; ++i)
            {
                // 拼接字符串           WiFi名                 信号强度                       是否加密
                String item = (i + 1) + WiFi.SSID(i) + " (" + WiFi.RSSI(i) + ") " + ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
                // 把搜索到的WiFi追加到下拉列表
                lv_dropdown_add_option(ui_Dropdown1, item.c_str(), LV_DROPDOWN_POS_LAST);
                vTaskDelay(10);
            }
        }
        vTaskDelay(30000); // 30s 后重新开始
    }
}

// 连接WiFi 任务
void connectWIFI()
{
    if (ssidName == NULL || ssidName.length() < 1 || password == NULL || password.length() < 1)
    {
        lv_label_set_text(ui_Label1, "");                                   // 清空文本输入框
        lv_label_set_text(ui_Label1, "Please enter password and WiFiname"); // 输入提示语
        return;
    }

    vTaskDelete(WIFISerch); // 结束搜索WiFi的进程
    vTaskDelay(500);
    xTaskCreate(beginWIFITask,
                "BeginWIFITask",
                2048,
                NULL,
                0,
                &WIFIConnect);
}

// 连接WiFi 回调函数
void beginWIFITask(void *pvParameters)
{
    unsigned long startingTime = millis();
    WiFi.begin(ssidName.c_str(), password.c_str());  // 连接WiFi

    // WiFi 没有连接且 等待时间少于 timeout
    while (WiFi.status() != WL_CONNECTED && (millis() - startingTime) < timeout)
    {
        vTaskDelay(250);
    }
    // 检查是否连接上
    // 如果连接上
    if (WiFi.status() == WL_CONNECTED)
    {
        IPAddress localIP = WiFi.localIP();
        const char *charIp = localIP.toString().c_str(); // 将 IPAddress 转换为 std::string，再转换为 const char*

        lv_textarea_set_text(ui_TextArea1, "");       // 清空文本输入框
        lv_textarea_set_text(ui_TextArea1, charIp);   // 设置文本框 内容为IP地址
        lv_label_set_text(ui_Label1, "");             // 清空文本输入框
        lv_label_set_text(ui_Label1, "Successfully"); // 设置头部标签 内容为成功连接到WiFi
    }
    else
    {
        String please = "Please try again";
        String restart = "Restart";

        lv_label_set_text(ui_Label1, "");              // 清空文本输入框
        lv_label_set_text(ui_Label1, restart.c_str()); // 设置头部标签 内容为重启

        lv_textarea_set_text(ui_TextArea1, "");             // 清空文本输入框
        lv_textarea_set_text(ui_TextArea1, please.c_str()); // 输入提示语

        vTaskDelay(250); //
        // networkScanner();
        // vTaskDelete(NULL);  // 结束连接WiFi
    }

    networkScanner(); // 启动搜索WiFi
    vTaskDelete(NULL);// 结束连接WiFi
}

// 断开连接
void Cancel_WiFi(void)
{
    // 如果连接到WiFi了 则断开WiFi 并重新搜索WiFi
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.disconnect();                           // 断开WiFi
        lv_label_set_text(ui_Label1, "");            // 清空文本输入框
        lv_label_set_text(ui_Label1, "Select WiFi"); // 清空文本输入框
        vTaskDelay(300);                             // 延时 300ms
        vTaskDelete(WIFIConnect);                    // 结束连接WiFi 进程
        vTaskDelay(300);                             // 延时 300ms
        networkScanner();                            // 启动搜索WiFi
    }
    // 如果没有连接到WiFi 则搜索WiFi
    else if (WiFi.status() != WL_CONNECTED)
    {
        lv_textarea_set_text(ui_TextArea1, "");      // 清空文本输入框
        lv_label_set_text(ui_Label1, "");            // 清空文本输入框
        lv_label_set_text(ui_Label1, "Select WiFi"); // 显示Select WiFi

        vTaskDelete(WIFISerch); // 结束上次的搜索WiFi 进程
        vTaskDelay(300);        // 延时 300ms
        networkScanner();       // 启动新的 搜索WiFi
    }
}
