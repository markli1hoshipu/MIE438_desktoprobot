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
const unsigned long standbyTimeout = 10000; // 10秒无指令进入待机状态


// 全局速度设置
int globalSpeed = 70; // 默认速度为70%

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
  
}

void loop() {
  // 检查蓝牙连接状态
  if (SerialBT.connected()) {
    if (!isConnected) {
      isConnected = true;
      Serial.println("Connected to Android");
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
    }
  }

  // 如果未连接或超时未收到指令，进入待机状态
  if (!isConnected || millis() - lastCommandTime > standbyTimeout) {
    static unsigned long lastStandbyTime = 0;
    
    // 每10秒执行一次待机模式动作，避免频繁动作
    if (millis() - lastStandbyTime > 60000) { // 每分钟进行一次待机模式动作
      Serial.println("Entering standby mode");
      motor.standbyMode();
      lcd.showStandby();
      lastStandbyTime = millis();
    }
  }
  
}

// 处理蓝牙命令
void processCommand(String command) {
  Serial.print("Command received: ");
  Serial.println(command);
  
  // 动作指令
  if (command == "FORWARD") {
    motor.moveForward(MOVE_DURATION, globalSpeed);
  } 
  else if (command == "BACKWARD") {
    motor.moveBackward(MOVE_DURATION, globalSpeed);
  }
  else if (command == "LEFT") {
    motor.leftRotate(TURN_DURATION, globalSpeed);
  }
  else if (command == "RIGHT") {
    motor.rightRotate(TURN_DURATION, globalSpeed);
  }
  else if (command == "STOP") {
    motor.allStop();
  }
  else if (command == "FLEFT") {
    motor.forwardLeftTurn(TURN_DURATION, globalSpeed);
  }
  else if (command == "FRIGHT") {
    motor.forwardRightTurn(TURN_DURATION, globalSpeed);
  }
  else if (command == "BLEFT") {
    motor.backwardLeftTurn(TURN_DURATION, globalSpeed);
  }
  else if (command == "BRIGHT") {
    motor.backwardRightTurn(TURN_DURATION, globalSpeed);
  }
  
  // 速度控制命令 (格式：SPEED:X，X=1-100)
  else if (command.startsWith("SPEED:")) {
    int speed = command.substring(6).toInt();
    if (speed >= 0 && speed <= 100) {
      globalSpeed = speed;
      SerialBT.println("Speed set to " + String(speed));
    } else {
      SerialBT.println("Invalid speed value. Use 0-100");
    }
  }
  
  // 菜单命令处理
  else if (command.startsWith("MENU:")) {
    String menuOption = command.substring(5);
    if (menuOption == "Smile") {
      lcd.showEmoji(LCD_EMOJI_SMILE);
    }
    else if (menuOption == "Frown") {
      lcd.showEmoji(LCD_EMOJI_FROWN);
    }
    else if (menuOption == "Sleep") {
      lcd.showEmoji(LCD_EMOJI_SLEEP);
    }
    else if (menuOption == "Default") {
      motor.standbyMode();
      lcd.showStandby();
    }
    else if (menuOption == "SakiSakiSaki") {
      motor.SakiMode();
      lcd.showEmoji(LCD_EMOJI_SPECIAL);
    }
    else {
      SerialBT.println("Unknown menu option: " + menuOption);
    }
  }

  else {
    SerialBT.println("Unknown command: " + command);
  }
}