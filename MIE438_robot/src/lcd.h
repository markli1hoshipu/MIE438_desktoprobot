// lcd.h - LCD显示控制模块
#ifndef LCD_H
#define LCD_H

#include <TFT_eSPI.h>
#include <Arduino.h>

class LCDController {
public:
    LCDController();
    void begin();
    
    // 基本显示功能
    void clear();
    void displayWelcome();
    void displayStatus(bool connected, int battery = 100);
    void displayCommand(const String &command);
    
    // 运动状态显示
    void showForward();
    void showBackward();
    void showLeft();
    void showRight();
    void showStop();
    void showStandby();
    
    // 其他功能显示
    void showBatteryLevel(int percentage);
    void showConnectionStatus(bool connected);
    void showSensorValue(const String &sensorName, int value);
    
    // 进度条和动画
    void drawProgressBar(int x, int y, int width, int height, int percentage);
    void drawCompass(int heading);
    
    // 更新屏幕信息
    void updateScreen();

private:
    TFT_eSPI _tft;
    
    // 屏幕布局区域定义
    static const int STATUS_AREA_HEIGHT = 30;
    static const int COMMAND_AREA_HEIGHT = 50;
    static const int INFO_AREA_HEIGHT = 150;
    static const int CONTROL_AREA_HEIGHT = 90;
    
    // 内部状态
    bool _isConnected;
    int _batteryLevel;
    String _currentCommand;
    String _statusMessage;
    unsigned long _lastUpdate;
    
    // 内部辅助函数
    void drawStatusBar();
    void drawCommandArea();
    void drawInfoArea();
    void drawControlArea();
    void rainbow_fill();
};

#endif