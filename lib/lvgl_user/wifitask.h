#ifndef _wifitask_h_
#define _wifitask_h_
#include <WiFi.h>
#include <Arduino.h>

#include "ui.h"
#include <lvgl_task.h>

void scanWIFITask(void *pvParameters);      // 搜索WiFi进程
void networkScanner();                      // 回调函数 ---> 搜索WiFi

void connectWIFI();                         // 连接WiFi进程
void beginWIFITask(void *pvParameters);     // 回调函数 ---> 连接WiFi

void Cancel_WiFi(void);                     // 断开WiFi

#endif