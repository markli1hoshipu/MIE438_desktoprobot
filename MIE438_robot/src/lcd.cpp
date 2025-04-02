// lcd.cpp - LCD显示控制模块实现
#include "lcd.h"
#include "../config.h"
#include <Arduino.h>

LCDController::LCDController() {
}

void LCDController::begin() {
    _tft.init();
    _tft.setRotation(3); // 根据实际安装情况调整
    _tft.fillScreen(TFT_BLACK);
    displayWelcome();
}

void LCDController::clear() {
    _tft.fillScreen(TFT_BLACK);
}

void LCDController::displayWelcome() {
    clear();
    
    // 彩虹背景
    rainbow_fill();
    
    // 标题和欢迎信息
    _tft.setTextColor(TFT_WHITE, TFT_BLACK);
    _tft.drawCentreString("MIE438 Robot", 120, 40, 4);
    _tft.drawCentreString("I love MIE438!", 120, 40, 4);
    _tft.drawCentreString("Initializing...", 120, 100, 2);
    
    // 启动动画 - 进度条
    for(int i = 0; i <= 100; i += 5) {
        drawProgressBar(40, 150, 160, 20, i);
        delay(50);
    }

    _tft.setTextColor(TFT_WHITE, TFT_BLACK);
    _tft.drawCentreString("Ready!", 120, 180, 2);
    delay(1000);

}

// 新增表情功能
void LCDController::showEmoji(int emojiType) {



        
    // 清除表情区域
    // left top corner x, y, width, height
    _tft.fillRect(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, TFT_BLACK);

    // 绘制新表情
    int size = 100 ;
    drawEmoji(emojiType, SCREEN_HEIGHT/2 - size/2, SCREEN_WIDTH/2 - size/2, size);

    

}

void LCDController::showStandby() {
    _tft.setTextColor(TFT_WHITE, TFT_BLACK);
    _tft.drawCentreString("I love MIE438!", 120, 40, 4);
    _tft.drawCentreString("Standby Mode...", 120, 100, 2);
}



void LCDController::drawProgressBar(int x, int y, int width, int height, int percentage) {
    // 边框
    _tft.drawRect(x, y, width, height, TFT_WHITE);
    
    // 清除进度条内部（以便更新）
    _tft.fillRect(x+2, y+2, width-4, height-4, TFT_BLACK);
    
    // 计算进度宽度
    int progressWidth = (width - 4) * percentage / 100;
    
    // 绘制进度
    _tft.fillRect(x+2, y+2, progressWidth, height-4, TFT_GREEN);
    
    // 显示百分比文字
    _tft.setTextColor(TFT_WHITE);
    _tft.setTextFont(2);
    String percentText = String(percentage) + "%";
    _tft.drawCentreString(percentText, x + width/2, y + height/2 - 8, 2);
}

// 彩虹填充函数（从示例代码调整）
void LCDController::rainbow_fill() {
    byte red = 31;
    byte green = 0;
    byte blue = 0;
    byte state = 0;
    unsigned int colour = red << 11;
    
    for (int i = 319; i > 0; i--) {
        // 绘制一条1像素宽的垂直线，使用选定的颜色
        _tft.drawFastHLine(0, i, _tft.width(), colour);
        
        // 这是一个"状态机"，依次调整颜色亮度
        switch (state) {
            case 0:
                green++;
                if (green == 64) {
                    green = 63;
                    state = 1;
                }
                break;
            case 1:
                red--;
                if (red == 255) {
                    red = 0;
                    state = 2;
                }
                break;
            case 2:
                blue++;
                if (blue == 32) {
                    blue = 31;
                    state = 3;
                }
                break;
            case 3:
                green--;
                if (green == 255) {
                    green = 0;
                    state = 4;
                }
                break;
            case 4:
                red++;
                if (red == 32) {
                    red = 31;
                    state = 5;
                }
                break;
            case 5:
                blue--;
                if (blue == 255) {
                    blue = 0;
                    state = 0;
                }
                break;
        }
        colour = red << 11 | green << 5 | blue;
    }
}

// 表情绘制辅助函数

void LCDController::drawEmoji(int emojiType, int x, int y, int size) {
    // 清除旧表情区域
    _tft.fillCircle(x, y, size, TFT_BLACK);
    
    switch (emojiType) {
        case LCD_EMOJI_DEFAULT:
            // 默认表情 - 中性
            _tft.fillCircle(x-size/3, y-size/5, size/8, TFT_BLUE);
            _tft.fillCircle(x+size/3, y-size/5, size/8, TFT_BLUE);
            _tft.fillRect(x - size/4, y + size/3, size/2, size/12, TFT_BLUE);
            break;
            
        case LCD_EMOJI_SMILE:
            // 微笑表情
            _tft.fillCircle(x-size/3, y-size/5, size/8, TFT_BLUE);
            _tft.fillCircle(x+size/3, y-size/5, size/8, TFT_BLUE);
            _tft.drawLine(x - size/4, y + size/3, x, y + size/2, TFT_BLUE);
            _tft.drawLine(x, y + size/2, x + size/4, y + size/3, TFT_BLUE);
            break;
            
        case LCD_EMOJI_FROWN:
            // 皱眉表情
            _tft.fillCircle(x-size/3, y-size/5, size/8, TFT_BLUE);
            _tft.fillCircle(x+size/3, y-size/5, size/8, TFT_BLUE);
            _tft.drawLine(x - size/4, y + size/2, x, y + size/3, TFT_BLUE);
            _tft.drawLine(x, y + size/3, x + size/4, y + size/2, TFT_BLUE);
            break;
            
        case LCD_EMOJI_SLEEP:
            // 睡眠表情
            _tft.drawLine(x-size/3, y-size/5, x-size/6, y-size/5, TFT_BLUE);
            _tft.drawLine(x+size/3, y-size/5, x+size/6, y-size/5, TFT_BLUE);
            _tft.fillCircle(x, y + size/3, size/6, TFT_BLUE);
            break;
            
        case LCD_EMOJI_SPECIAL:
            // 特殊表情 - 星星眼
            _tft.drawLine(x-size/3-size/8, y-size/5-size/8, x-size/3+size/8, y-size/5+size/8, TFT_RED);
            _tft.drawLine(x-size/3-size/8, y-size/5+size/8, x-size/3+size/8, y-size/5-size/8, TFT_RED);
            _tft.drawLine(x+size/3-size/8, y-size/5-size/8, x+size/3+size/8, y-size/5+size/8, TFT_RED);
            _tft.drawLine(x+size/3-size/8, y-size/5+size/8, x+size/3+size/8, y-size/5-size/8, TFT_RED);
            _tft.fillRect(x - size/3, y + size/5, size*2/3, size/5, TFT_RED);
            break;
    }
}
