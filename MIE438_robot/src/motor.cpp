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
    delay(duration);
    allStop();
}

void MotorController::moveBackward(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    right_front.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    delay(duration);
    allStop();
}

void MotorController::forwardRightTurn(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_FWD_ANGLE, speed));
    right_front.write(STOP_ANGLE);
    right_back.write(STOP_ANGLE);
    delay(duration);
    allStop();
}

void MotorController::backwardRightTurn(int duration, int speed) {
    left_front.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    left_back.write(angleWithSpeed(LEFT_BWD_ANGLE, speed));
    right_front.write(STOP_ANGLE);
    right_back.write(STOP_ANGLE);
    delay(duration);
    allStop();
}

void MotorController::forwardLeftTurn(int duration, int speed) {
    left_front.write(STOP_ANGLE);
    left_back.write(STOP_ANGLE);
    right_front.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_FWD_ANGLE, speed));
    delay(duration);
    allStop();
}

void MotorController::backwardLeftTurn(int duration, int speed) {
    left_front.write(STOP_ANGLE);
    left_back.write(STOP_ANGLE);
    right_front.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    right_back.write(angleWithSpeed(RIGHT_BWD_ANGLE, speed));
    delay(duration);
    allStop();
}

void MotorController::standbyMode() {
    // 更新standbyMode以使用新的速度参数
    moveForward(MOVE_DURATION, 30);
    delay(STOP_DURATION);
    moveBackward(MOVE_DURATION, 30);
    delay(STOP_DURATION);
    forwardRightTurn(TURN_DURATION, 20);
    delay(STOP_DURATION);
    backwardRightTurn(TURN_DURATION, 20);
    delay(STOP_DURATION);
    forwardLeftTurn(TURN_DURATION, 20);
    delay(STOP_DURATION);
    backwardLeftTurn(TURN_DURATION, 20);
    delay(STOP_DURATION);
}