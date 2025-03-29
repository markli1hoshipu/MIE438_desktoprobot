// main.ino - 主程序入口

#include <BluetoothSerial.h>
#include "config.h"
#include "src/motor.h"



MotorController motor;

BluetoothSerial SerialBT;
bool isConnected = false; // 是否连接到 Android 设备
unsigned long lastCommandTime = 0; // 上次收到指令的时间
const unsigned long standbyTimeout = 5000; // 5 秒无指令进入待机状态

void setup() {
  // 初始化串口
  Serial.begin(115200);
  SerialBT.begin("ESP32_MIE438Robot"); // 蓝牙设备名称
  motor.begin();
  Serial.println("ESP32 Ready");
}

void loop() {

  // 检查蓝牙连接状态
  if (SerialBT.connected()) {
    if (!isConnected) {
      isConnected = true;
      Serial.println("Connected to Android");
    }
    lastCommandTime = millis(); // 更新上次收到指令的时间
  } else {
    if (isConnected) {
      isConnected = false;
      Serial.println("Disconnected from Android");
    }
  }

  // 如果未连接或超时未收到指令，进入待机状态
  if (!isConnected || millis() - lastCommandTime > standbyTimeout) {
    motor.standbyMode();; // 待机状态
  } else {
    // 运行状态：处理 Android 指令
    if (SerialBT.available()) {
      String command = SerialBT.readStringUntil('\n');
      command.trim(); // 去除换行符和空格

      // 处理指令
      // if (command == "FORWARD") {
      //   moveServos(180); // 前进
      // } else if (command == "BACKWARD") {
      //   moveServos(0); // 后退
      // } else if (command.startsWith("LED")) {
      //   int ledIndex = command.charAt(3) - '0'; // 获取 LED 编号
      //   if (ledIndex >= 0 && ledIndex < 6) {
      //     toggleLED(ledIndex); // 切换 LED 状态
      //   }
      // }
    }
  }


}
