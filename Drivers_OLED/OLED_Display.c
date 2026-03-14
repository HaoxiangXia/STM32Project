#include "OLED_Display.h"
#include "ssd1306.h"
#include <stdio.h>
#include <string.h>

void OLED_Display_Init(OLED_Display_t *oled) {
    ssd1306_Init();
    OLED_Display_ClearScrollArea(oled);
}

void OLED_Display_ClearScrollArea(OLED_Display_t *oled) {
    oled->scrollCount = 0;
    oled->msgCounter = 0;
    memset(oled->scrollBuffer, 0, sizeof(oled->scrollBuffer));
    
    // 立即刷新清屏显示
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Serial Monitor:", Font_6x8, White);
    ssd1306_SetCursor(0, 8);
    ssd1306_WriteString("----------------", Font_6x8, White);
    ssd1306_UpdateScreen();
}

void OLED_Display_Println(OLED_Display_t *oled, const char *text) {
    char numberedText[CHAR_PER_LINE];
    snprintf(numberedText, CHAR_PER_LINE, "%d. %s", oled->msgCounter++, text);
    
    // 滚动逻辑
    if (oled->scrollCount >= SCROLL_LINES) {
        for (int i = 1; i < SCROLL_LINES; i++) {
            strncpy(oled->scrollBuffer[i - 1], oled->scrollBuffer[i], CHAR_PER_LINE);
        }
        strncpy(oled->scrollBuffer[SCROLL_LINES - 1], numberedText, CHAR_PER_LINE);
    } else {
        strncpy(oled->scrollBuffer[oled->scrollCount], numberedText, CHAR_PER_LINE);
        oled->scrollCount++;
    }
    
    // 刷新屏幕
    ssd1306_Fill(Black);
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString("Serial Monitor:", Font_6x8, White);
    ssd1306_SetCursor(0, 8);
    ssd1306_WriteString("----------------", Font_6x8, White);
    
    for (int i = 0; i < oled->scrollCount; i++) {
        ssd1306_SetCursor(0, 16 + (i * 8));
        ssd1306_WriteString(oled->scrollBuffer[i], Font_6x8, White);
    }
    ssd1306_UpdateScreen();
}