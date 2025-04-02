// motor.h
#ifndef MOTOR_H
#define MOTOR_H

#include <ESP32Servo.h>
#include "../config.h"

class MotorController {
public:
    MotorController();
    void begin();
    void standbyMode();
    void SakiMode();
    
    // 更新后的运动函数
    void moveForward(int duration, int speed = 10);
    void moveBackward(int duration, int speed = 10);
    void forwardRightTurn(int duration, int speed = 10);
    void backwardRightTurn(int duration, int speed = 10);
    void forwardLeftTurn(int duration, int speed = 10);
    void backwardLeftTurn(int duration, int speed = 10);
    void leftRotate(int duration, int speed = 10); //counter-clockwise
    void rightRotate(int duration, int speed = 10); //clockwise
    void allStop();
    
private:
    Servo left_front;
    Servo left_back;
    Servo right_front;
    Servo right_back;
    
    int constrainSpeed(int speed); // 速度限制函数
    int angleWithSpeed(int baseAngle, int speed); // 角度计算函数
};

#endif