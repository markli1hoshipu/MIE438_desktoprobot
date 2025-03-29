// config.h - 硬件配置和常量定义

#ifndef CONFIG_H
#define CONFIG_H

// 引脚配置
#define LEFT_FRONT_PIN 23
#define LEFT_BACK_PIN 18
#define RIGHT_FRONT_PIN 22
#define RIGHT_BACK_PIN 19

// 根据官方release, 取0-180；
// 舵机角度常量
#define STOP_ANGLE 90       // 停止角度
#define LEFT_FWD_ANGLE 180  // 左侧前进角度
#define LEFT_BWD_ANGLE 0    // 左侧后退角度
#define RIGHT_FWD_ANGLE 0   // 右侧前进角度
#define RIGHT_BWD_ANGLE 180 // 右侧后退角度

// 运动持续时间(ms)
#define MOVE_DURATION 2000
#define TURN_DURATION 2000
#define STOP_DURATION 1000

#endif