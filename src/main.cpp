#include "allhead.h"

void setup()
{
	lvgl_PinnedToCore(); // UI 进程
	networkScanner();	 // 搜索WiFi进
}

void loop()
{
}
