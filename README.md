# MIE438 Robot Controller Project

## Overview
This project consists of an ESP32-based robot controlled by an Android application via Bluetooth. The robot features servo motor control and an LCD display for visual feedback.

## Repository Structure
```
MIE438_Project/
├── MIE438_robot/                # ESP32 Arduino code
│   ├── MIE438_robot.ino         # Main Arduino sketch
│   ├── config.h                 # Hardware configuration
│   └── src/
│       ├── motor.h/cpp          # Motor control module
│       └── lcd.h/cpp            # LCD display module
└── MIE438RobotController/       # Android application
    ├── AndroidManifest.xml      # App permissions and settings
    └── MainActivity.kt          # Main Android activity
```

## Hardware Components
- ESP32 microcontroller
- 4 servo motors (left front, left back, right front, right back)
- TFT LCD display
- Bluetooth module (integrated in ESP32)

## ESP32 Robot Features

### Motor Control
- Eight directional movements: forward, backward, left, right, forward-left, forward-right, backward-left, backward-right
- Speed control (0-100%)
- Emergency stop function
- Standby and special "Saki" modes for autonomous movement

### LCD Display
- Welcome animation with rainbow effect and progress bar
- Emotion display with multiple emoji options: smile, frown, sleep, and special
- Standby mode display

### Bluetooth Communication
- Device name: ESP32_MIE438Robot
- Auto standby mode after 10 seconds of inactivity

## Android Application

### Features
- Bluetooth device discovery and connection
- Direction control buttons
- Speed control via seekbar
- Command history view
- Menu for selecting LCD display modes

### Requirements
- Android device with Bluetooth support
- Android 12+ compatibility
- Permission handling for Bluetooth connectivity

## Command Protocol

### Direction Control
- `FORWARD` - Move forward
- `BACKWARD` - Move backward
- `LEFT` - Rotate counterclockwise
- `RIGHT` - Rotate clockwise
- `STOP` - Emergency stop
- `FLEFT` - Forward left turn
- `FRIGHT` - Forward right turn
- `BLEFT` - Backward left turn
- `BRIGHT` - Backward right turn

### Speed Control
- `SPEED:[0-100]` - Set speed percentage (e.g., `SPEED:75`)

### Display Menu Commands
- `MENU:Smile` - Display smile emoji
- `MENU:Frown` - Display frown emoji
- `MENU:Sleep` - Display sleep emoji
- `MENU:Default` - Return to default state
- `MENU:SakiSakiSaki` - Activate special mode

## Setup Instructions

### ESP32 Setup
1. Install required libraries:
   - ESP32Servo
   - TFT_eSPI
   - BluetoothSerial
2. Configure TFT_eSPI library for your display in User_Setup.h
3. Connect hardware according to pin definitions in config.h
4. Upload MIE438_robot.ino to your ESP32

### Android Setup
1. Open the project in Android Studio
2. Ensure your device has developer options and USB debugging enabled
3. Build and install the application
4. Grant Bluetooth permissions when prompted

## Usage
1. Power on the robot
2. Open the Android application
3. Connect to "ESP32_MIE438Robot" from the device selection dialog
4. Use the directional buttons to control the robot
5. Adjust speed using the seekbar
6. Access additional display options from the menu button

## Notes
- Commands are case-sensitive
- All commands end with a newline character `\n`
- The robot enters standby mode after 10 seconds of inactivity
- The LCD display provides visual feedback of the robot's state

## Troubleshooting
- If connection fails, ensure Bluetooth is enabled on your Android device
- Check that the ESP32 is powered and the correct device name appears
- For Android 12+, ensure all Bluetooth permissions are granted
- Verify the hardware connections if motors or display are not responding

## Future Improvements
- Add sensor integration for obstacle avoidance
- Improve autonomous movement algorithms
- Implement joystick control in the Android app
- Add battery level monitoring
