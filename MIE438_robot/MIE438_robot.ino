// main.ino - 主程序入口

#include <BluetoothSerial.h>
#include "config.h"
#include "src/motor.h"
#include "src/lcd.h"

// 创建控制器对象
MotorController motor;
LCDController lcd;

// 蓝牙通信
BluetoothSerial SerialBT;
bool isConnected = false; // 是否连接到Android设备
unsigned long lastCommandTime = 0; // 上次收到指令的时间
const unsigned long standbyTimeout = 5000; // 5秒无指令进入待机状态

// 电池电量模拟
int batteryLevel = 100;
unsigned long lastBatteryUpdate = 0;

// 处理蓝牙命令
void processCommand(String command);

void setup() {
  // 初始化串口
  Serial.begin(115200);
  
  // 初始化液晶显示
  lcd.begin();
  
  // 初始化蓝牙
  SerialBT.begin("ESP32_MIE438Robot"); // 蓝牙设备名称
  
  // 初始化电机控制器
  motor.begin();
  
  Serial.println("ESP32 Ready - MIE438 Robot");
  
  // 更新LCD显示状态
  lcd.displayStatus(isConnected, batteryLevel);
}

void loop() {
  // 检查蓝牙连接状态
  if (SerialBT.connected()) {
    if (!isConnected) {
      isConnected = true;
      Serial.println("Connected to Android");
      lcd.showConnectionStatus(true);
    }
    
    // 处理蓝牙数据
    if (SerialBT.available()) {
      String command = SerialBT.readStringUntil('\n');
      command.trim(); // 去除换行符和空格
      
      // 处理命令
      processCommand(command);
      
      // 更新命令时间
      lastCommandTime = millis();
    }
  } else {
    if (isConnected) {
      isConnected = false;
      Serial.println("Disconnected from Android");
      lcd.showConnectionStatus(false);
    }
  }

  // 如果未连接或超时未收到指令，进入待机状态
  if (!isConnected || millis() - lastCommandTime > standbyTimeout) {
    static unsigned long lastStandbyTime = 0;
    
    // 每10秒执行一次待机模式动作，避免频繁动作
    if (millis() - lastStandbyTime > 10000) {
      Serial.println("Entering standby mode");
      motor.standbyMode();
      lcd.showStandby();
      lastStandbyTime = millis();
    }
  }
  
  // 模拟电池电量下降（每分钟下降1%）
  if (millis() - lastBatteryUpdate > 60000) {
    batteryLevel = max(0, batteryLevel - 1);
    lcd.showBatteryLevel(batteryLevel);
    lastBatteryUpdate = millis();
  }
}

// 处理蓝牙命令
void processCommand(String command) {
  Serial.print("Command received: ");
  Serial.println(command);
  
  // 将命令显示在LCD上
  lcd.displayCommand(command);
  
  // 动作指令
  if (command == "FORWARD") {
    lcd.showForward();
    motor.moveForward(MOVE_DURATION, 70);
    lcd.showStop();
  } 
  else if (command == "BACKWARD") {
    lcd.showBackward();
    motor.moveBackward(MOVE_DURATION, 70);
    lcd.showStop();
  }
  else if (command == "LEFT") {
    lcd.showLeft();
    motor.forwardLeftTurn(TURN_DURATION, 60);
    lcd.showStop();
  }
  else if (command == "RIGHT") {
    lcd.showRight();
    motor.forwardRightTurn(TURN_DURATION, 60);
    lcd.showStop();
  }
  else if (command == "STOP") {
    motor.allStop();
    lcd.showStop();
  }
  
  // 速度控制命令 (格式：SPEED:X，X=1-100)
  else if (command.startsWith("SPEED:")) {
    int speed = command.substring(6).toInt();
    if (speed >= 1 && speed <= 100) {
      // 这里可以保存速度设置供后续命令使用
      SerialBT.println("Speed set to " + String(speed));
    }
  }
  
  // 状态查询命令
  else if (command == "STATUS") {
    String status = "Robot status: ";
    status += isConnected ? "Connected" : "Disconnected";
    status += ", Battery: " + String(batteryLevel) + "%";
    SerialBT.println(status);
  }
  else {
    SerialBT.println("Unknown command: " + command);
  }
}
