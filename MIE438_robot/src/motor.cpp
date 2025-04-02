// motor.cpp
#include "motor.h"
#include "../config.h"
#include <Arduino.h>

MotorController::MotorController() {}

void MotorController::begin() {
    left_front.attach(LEFT_FRONT_PIN);
    left_back.attach(LEFT_BACK_PIN);
    right_front.attach(RIGHT_FRONT_PIN);
    right_back.attach(RIGHT_BACK_PIN);
    allStop();
}

int MotorController::constrainSpeed(int speed) {
    return constrain(speed, 0, 100); // 限制速度在0-100%
}

int MotorController::angleWithSpeed(int baseAngle, int speed) {
    speed = constrainSpeed(speed);
    if(baseAngle == STOP_ANGLE) return STOP_ANGLE;
    
    // 计算带速度的角度 (90 ± (差量×速度百分比))
    int diff = baseAngle - STOP_ANGLE;
    return STOP_ANGLE + (diff * speed / 100);
}

void MotorController::allStop() {
    left_front.write(STOP_ANGLE);
    left_back.write(STOP_ANGLE);
    right_front.write(STOP_ANGLE);
    right_back.write(STOP_ANGLE);
    delay(500);
}

void MotorController::moveForward(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
}

void MotorController::moveBackward(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
}

void MotorController::forwardRightTurn(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed/4));
    right_back.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed/4));
}

void MotorController::backwardRightTurn(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed/4));
    right_back.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed/4));
}

void MotorController::forwardLeftTurn(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_FWD_ANGLE, speed/4));
    left_back.write(angleWithSpeed(LEFT_FWD_ANGLE, speed/4));
    right_front.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
}

void MotorController::backwardLeftTurn(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_BWD_ANGLE, speed/4));
    left_back.write(angleWithSpeed(LEFT_BWD_ANGLE, speed/4));
    right_front.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
}

void MotorController::leftRotate(int duration, int speed) { //counter-clockwise
    left_front.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
}

void MotorController::rightRotate(int duration, int speed) { //clockwise
    left_front.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
}


void MotorController::standbyMode() {
    // 随机选择动作模式 (0或1)
    int mode = random(0, 2); 
    
    if (mode == 0) {
        // 模式1：前进->左转->后退右转->右转
        moveForward(MOVE_DURATION, 30);
        delay(STOP_DURATION);
        
        leftRotate(MOVE_DURATION, 30);
        delay(STOP_DURATION);
        
        backwardRightTurn(TURN_DURATION, 20);
        delay(STOP_DURATION);
        
        rightRotate(MOVE_DURATION, 20);
    } else {
        // 模式2：右转->后退->前左转->左转
        rightRotate(MOVE_DURATION, 30);
        delay(STOP_DURATION);
        
        moveBackward(MOVE_DURATION, 30);
        delay(STOP_DURATION);
        
        forwardLeftTurn(TURN_DURATION, 20);
        delay(STOP_DURATION);
        
        leftRotate(MOVE_DURATION, 20);
    }
    
    delay(STOP_DURATION);
    allStop();
}

void MotorController::SakiMode() {
    // crazy mode - 无限制一直乱动
    int crazy_speed = 80;
    
    // 随机选择动作
    int action = random(0, 8); // 0-7共8种动作
    
    switch(action) {
        case 0:
            moveForward(MOVE_DURATION, crazy_speed);
            break;
        case 1:
            moveBackward(MOVE_DURATION, crazy_speed);
            break;
        case 2:
            forwardLeftTurn(TURN_DURATION, crazy_speed);
            break;
        case 3:
            forwardRightTurn(TURN_DURATION, crazy_speed);
            break;
        case 4:
            backwardLeftTurn(TURN_DURATION, crazy_speed);
            break;
        case 5:
            backwardRightTurn(TURN_DURATION, crazy_speed);
            break;
        case 6:
            leftRotate(MOVE_DURATION, crazy_speed);
            break;
        case 7:
            rightRotate(MOVE_DURATION, crazy_speed);
            break;
    }
    
    // 随机停顿时间
    delay(random(50, 200));
    
}