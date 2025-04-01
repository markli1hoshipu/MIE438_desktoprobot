// lcd.cpp - LCD显示控制模块实现
#include "lcd.h"

LCDController::LCDController() : 
    _isConnected(false), 
    _batteryLevel(100), 
    _currentCommand(""), 
    _statusMessage("待机状态"),
    _lastUpdate(0) {
}

void LCDController::begin() {
    _tft.init();
    _tft.setRotation(2); // 根据实际安装情况调整
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
    _tft.drawCentreString("初始化中...", 120, 100, 2);
    
    // 启动动画 - 进度条
    for(int i = 0; i <= 100; i += 5) {
        drawProgressBar(40, 150, 160, 20, i);
        delay(50);
    }
    
    _tft.drawCentreString("就绪!", 120, 180, 2);
    delay(1000);
    
    // 初始化显示
    updateScreen();
}

void LCDController::updateScreen() {
    clear();
    drawStatusBar();
    drawCommandArea();
    drawInfoArea();
    drawControlArea();
    _lastUpdate = millis();
}

void LCDController::drawStatusBar() {
    // 状态栏背景
    _tft.fillRect(0, 0, 240, STATUS_AREA_HEIGHT, TFT_NAVY);
    
    // 显示连接状态
    _tft.setTextColor(TFT_WHITE);
    _tft.setTextFont(2);
    if (_isConnected) {
        _tft.drawString("已连接", 10, 5);
    } else {
        _tft.drawString("未连接", 10, 5);
    }
    
    // 显示电池电量
    String battery = "电量: " + String(_batteryLevel) + "%";
    _tft.drawString(battery, 120, 5);
    
    // 显示时间
    unsigned long uptime = millis() / 1000;
    String time = String(uptime) + "s";
    _tft.drawRightString(time, 230, 5, 2);
}

void LCDController::drawCommandArea() {
    int yPos = STATUS_AREA_HEIGHT;
    
    // 命令区域背景
    _tft.fillRect(0, yPos, 240, COMMAND_AREA_HEIGHT, TFT_DARKCYAN);
    
    // 显示当前命令
    _tft.setTextColor(TFT_WHITE);
    _tft.setTextFont(4);
    _tft.setCursor(10, yPos + 10);
    _tft.print("命令: ");
    
    if (_currentCommand != "") {
        _tft.setCursor(10, yPos + 25);
        _tft.print(_currentCommand);
    } else {
        _tft.setCursor(10, yPos + 25);
        _tft.print("等待指令...");
    }
}

void LCDController::drawInfoArea() {
    int yPos = STATUS_AREA_HEIGHT + COMMAND_AREA_HEIGHT;
    
    // 信息区域背景
    _tft.fillRect(0, yPos, 240, INFO_AREA_HEIGHT, TFT_BLACK);
    
    // 显示状态信息
    _tft.setTextColor(TFT_GREEN);
    _tft.setTextFont(2);
    _tft.drawCentreString("状态信息", 120, yPos + 5, 2);
    
    _tft.setTextColor(TFT_WHITE);
    _tft.setTextFont(2);
    _tft.drawCentreString(_statusMessage, 120, yPos + 30, 2);
    
    // 罗盘示例（表示方向）
    drawCompass(0);  // 默认朝北
}

void LCDController::drawControlArea() {
    int yPos = STATUS_AREA_HEIGHT + COMMAND_AREA_HEIGHT + INFO_AREA_HEIGHT;
    
    // 控制区域背景
    _tft.fillRect(0, yPos, 240, CONTROL_AREA_HEIGHT, TFT_DARKGREY);
    
    // 画出控制按钮
    int btnWidth = 70;
    int btnHeight = 40;
    int btnMargin = 10;
    
    // 前进按钮
    _tft.fillRoundRect(120 - btnWidth/2, yPos + btnMargin, 
                      btnWidth, btnHeight, 5, TFT_BLUE);
    _tft.setTextColor(TFT_WHITE);
    _tft.drawCentreString("前进", 120, yPos + btnMargin + 12, 2);
    
    // 后退按钮
    _tft.fillRoundRect(120 - btnWidth/2, yPos + btnHeight + btnMargin*2, 
                      btnWidth, btnHeight, 5, TFT_BLUE);
    _tft.drawCentreString("后退", 120, yPos + btnHeight + btnMargin*2 + 12, 2);
    
    // 左转按钮
    _tft.fillRoundRect(120 - btnWidth/2 - btnWidth - btnMargin, 
                      yPos + btnMargin + btnHeight/2, 
                      btnWidth, btnHeight, 5, TFT_BLUE);
    _tft.drawCentreString("左转", 120 - btnWidth/2 - btnMargin - btnWidth/2, 
                         yPos + btnMargin + btnHeight/2 + 12, 2);
    
    // 右转按钮
    _tft.fillRoundRect(120 + btnWidth/2 + btnMargin, 
                      yPos + btnMargin + btnHeight/2, 
                      btnWidth, btnHeight, 5, TFT_BLUE);
    _tft.drawCentreString("右转", 120 + btnWidth/2 + btnMargin + btnWidth/2, 
                         yPos + btnMargin + btnHeight/2 + 12, 2);
}

void LCDController::displayStatus(bool connected, int battery) {
    _isConnected = connected;
    _batteryLevel = battery;
    updateScreen();
}

void LCDController::displayCommand(const String &command) {
    _currentCommand = command;
    
    // 设置相应的状态信息
    if (command == "FORWARD") {
        _statusMessage = "机器人正在前进";
    } else if (command == "BACKWARD") {
        _statusMessage = "机器人正在后退";
    } else if (command == "LEFT") {
        _statusMessage = "机器人正在左转";
    } else if (command == "RIGHT") {
        _statusMessage = "机器人正在右转";
    } else if (command == "STOP") {
        _statusMessage = "机器人已停止";
    } else if (command.startsWith("LED")) {
        _statusMessage = "控制LED: " + command;
    } else if (command.startsWith("SPEED")) {
        _statusMessage = "设置速度: " + command.substring(6);
    } else {
        _statusMessage = "执行命令: " + command;
    }
    
    updateScreen();
}

void LCDController::showForward() {
    _statusMessage = "机器人正在前进";
    _currentCommand = "FORWARD";
    
    // 更新屏幕，只刷新必要部分
    drawCommandArea();
    drawInfoArea();
}

void LCDController::showBackward() {
    _statusMessage = "机器人正在后退";
    _currentCommand = "BACKWARD";
    
    // 更新屏幕，只刷新必要部分
    drawCommandArea();
    drawInfoArea();
}

void LCDController::showLeft() {
    _statusMessage = "机器人正在左转";
    _currentCommand = "LEFT";
    
    // 更新屏幕，只刷新必要部分
    drawCommandArea();
    drawInfoArea();
}

void LCDController::showRight() {
    _statusMessage = "机器人正在右转";
    _currentCommand = "RIGHT";
    
    // 更新屏幕，只刷新必要部分
    drawCommandArea();
    drawInfoArea();
}

void LCDController::showStop() {
    _statusMessage = "机器人已停止";
    _currentCommand = "STOP";
    
    // 更新屏幕，只刷新必要部分
    drawCommandArea();
    drawInfoArea();
}

void LCDController::showStandby() {
    _statusMessage = "待机状态";
    _currentCommand = "";
    
    // 更新屏幕，只刷新必要部分
    drawCommandArea();
    drawInfoArea();
}

void LCDController::showBatteryLevel(int percentage) {
    _batteryLevel = percentage;
    drawStatusBar();
}

void LCDController::showConnectionStatus(bool connected) {
    _isConnected = connected;
    drawStatusBar();
}

void LCDController::showSensorValue(const String &sensorName, int value) {
    int yPos = STATUS_AREA_HEIGHT + COMMAND_AREA_HEIGHT;
    
    // 清除传感器显示区域
    _tft.fillRect(0, yPos + 60, 240, 30, TFT_BLACK);
    
    // 显示传感器信息
    _tft.setTextColor(TFT_YELLOW);
    _tft.setTextFont(2);
    String sensorInfo = sensorName + ": " + String(value);
    _tft.drawCentreString(sensorInfo, 120, yPos + 65, 2);
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

void LCDController::drawCompass(int heading) {
    int centerX = 120;
    int centerY = STATUS_AREA_HEIGHT + COMMAND_AREA_HEIGHT + 90;
    int radius = 40;
    
    // 清除原有罗盘区域
    _tft.fillCircle(centerX, centerY, radius + 10, TFT_BLACK);
    
    // 绘制罗盘外圈
    _tft.drawCircle(centerX, centerY, radius, TFT_WHITE);
    
    // 绘制方向标记
    _tft.setTextColor(TFT_WHITE);
    _tft.setTextFont(2);
    _tft.drawCentreString("N", centerX, centerY - radius - 15, 2);
    _tft.drawCentreString("E", centerX + radius + 5, centerY, 2);
    _tft.drawCentreString("S", centerX, centerY + radius + 5, 2);
    _tft.drawCentreString("W", centerX - radius - 15, centerY, 2);
    
    // 计算指针位置
    float radians = heading * 0.0174532925; // 角度转弧度
    int pointerX = centerX + sin(radians) * (radius - 5);
    int pointerY = centerY - cos(radians) * (radius - 5);
    
    // 绘制指针
    _tft.drawLine(centerX, centerY, pointerX, pointerY, TFT_RED);
    _tft.fillCircle(centerX, centerY, 5, TFT_RED);
    
    // 显示角度数值
    String headingText = String(heading) + "°";
    _tft.drawCentreString(headingText, centerX, centerY + radius + 20, 2);
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