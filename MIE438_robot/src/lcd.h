// lcd.h - LCD显示控制模块
#ifndef LCD_H
#define LCD_H

#include <TFT_eSPI.h>
#include <Arduino.h>

// 表情定义常量
#define LCD_EMOJI_DEFAULT 0
#define LCD_EMOJI_SMILE 1
#define LCD_EMOJI_FROWN 2
#define LCD_EMOJI_SLEEP 3
#define LCD_EMOJI_SPECIAL 4

class LCDController {
public:
    LCDController();
    void begin();
    
    // 基本显示功能
    void clear();
    void displayWelcome();
        
    void showStandby();

    void showEmoji(int emojiType);
    

private:
    TFT_eSPI _tft;
    
    // 屏幕布局区域定义
    static const int STATUS_AREA_HEIGHT = 30;
    static const int COMMAND_AREA_HEIGHT = 50;
    static const int INFO_AREA_HEIGHT = 150;
    static const int CONTROL_AREA_HEIGHT = 90;
    
    // 内部辅助函数
    void rainbow_fill();
    void drawProgressBar(int x, int y, int width, int height, int percentage);
    void drawEmoji(int emojiType, int x, int y, int size);
};

#endif