#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "main.h"

#define SCROLL_LINES 6
#define CHAR_PER_LINE 20 // 根据屏幕宽度设定，128/6 = 21左右

typedef struct {
    char scrollBuffer[SCROLL_LINES][CHAR_PER_LINE];
    int scrollCount;
    int msgCounter;
} OLED_Display_t;

// 函数声明
void OLED_Display_Init(OLED_Display_t *oled);
void OLED_Display_ClearScrollArea(OLED_Display_t *oled);
void OLED_Display_Println(OLED_Display_t *oled, const char *text);

#endif