#pragma once

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>

// 按键扫描函数，用于区分哪个按键按下了
uint8_t Key_Scan();

// 按键初始化函数
void my_key_init();

// 按键状态更新函数
void my_key_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

/// lvgl 初始化
void lvgl_Init(void);

// 组管理
void Group_Init(void);