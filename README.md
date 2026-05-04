# 🤖 Line Follower Robot — RoboChallenge 2025

> A 5-sensor PID-controlled line following robot built on the STM32 platform, with a fully 3D-printed PLA chassis. Competed at **RoboChallenge 2025**.

![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![STM32](https://img.shields.io/badge/Platform-STM32-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white)
![L298N](https://img.shields.io/badge/Motor_Driver-L298N-FF6F00?style=for-the-badge&logo=hackster&logoColor=white)
![PID](https://img.shields.io/badge/Control-PID_Controller-6A0DAD?style=for-the-badge&logo=cachet&logoColor=white)
![3D Printed](https://img.shields.io/badge/Chassis-3D_Printed_PLA-2ECC71?style=for-the-badge&logo=printables&logoColor=white)
![RoboChallenge](https://img.shields.io/badge/Competition-RoboChallenge_2025-E74C3C?style=for-the-badge&logo=robotframework&logoColor=white)
![Team Project](https://img.shields.io/badge/Built_By-Team-F39C12?style=for-the-badge&logo=github&logoColor=white)

---

## 📸 Overview

This project is an autonomous line follower robot designed and built as a team for the [RoboChallenge 2025](https://robochallenge.ro/) competition. The robot uses an array of 5 IR sensors and a PID controller to smoothly follow a black line on a white surface, dynamically adjusting motor speeds to minimize tracking error.


<img width="2048" height="1536" alt="front" src="https://github.com/user-attachments/assets/0aad564d-f375-444d-917b-5ffb4d4fd6a2" />
<img width="1536" height="2048" alt="top" src="https://github.com/user-attachments/assets/5f2ff9ad-6209-4f96-90a2-6669970a202d" />


---

## ⚙️ Hardware

| Component           | Details                              |
| ------------------- | ------------------------------------ |
| **Microcontroller** | STM32 (Arduino-compatible framework) |
| **Motor Driver**    | L298N Dual H-Bridge                  |
| **Sensors**         | 5× IR analog sensors (Sensor Array)  |
| **Motors**          | 2× DC motors (differential drive)    |
| **Chassis**         | Fully 3D printed — PLA filament      |
| **Communication**   | UART Serial (Serial1 @ 9600 baud)    |

### Sensor Pinout

| Sensor | Pin | Position     |
| ------ | --- | ------------ |
| S1     | PA0 | Far Left     |
| S2     | PA1 | Center Left  |
| S3     | PA2 | Center       |
| S4     | PA3 | Center Right |
| S5     | PA4 | Far Right    |

### Motor Driver Pinout

| Signal      | Pin  |
| ----------- | ---- |
| Motor A IN1 | PB0  |
| Motor A IN2 | PB1  |
| Motor A PWM | PB6  |
| Motor B IN3 | PB10 |
| Motor B IN4 | PB11 |
| Motor B PWM | PB7  |

---

## 🧠 How It Works

### Sensor Reading

Each sensor outputs an analog value. A reading **below the threshold** (`565`) means the sensor is **over the line**.

```
S1   S2   S3   S4   S5
-4   -2    0   +2   +4   ← positional weights
```

The weighted average of active sensors gives a normalized **error value** ranging from `-4` (far left) to `+4` (far right). A value of `0` means the robot is perfectly centered.

### PID Controller

```
adjustment = (Kp × error) + (Ki × integral) + (Kd × derivative)
```

| Parameter | Value | Role                                     |
| --------- | ----- | ---------------------------------------- |
| `Kp`      | 32    | Reacts proportionally to current error   |
| `Ki`      | 0.05  | Corrects accumulated drift over time     |
| `Kd`      | 35    | Dampens oscillation using rate of change |

Motor speeds are then computed as:

```
leftSpeed  = baseSpeed + adjustment
rightSpeed = baseSpeed - adjustment
```

Both speeds are clamped to `[0, 255]` via `constrain()`.

### Lost Line Recovery

If **no sensors** detect the line (`numOnLine == 0`), the robot spins in the direction of the last known error to recover the line, and the integral is reset to prevent windup.

---

## Tuning

You can adjust the following parameters directly in the sketch:

```cpp
uint32_t threshold = 565;  // Sensor line detection threshold
int baseSpeed = 220;       // Base motor PWM speed (0–255)
float Kp = 32;             // Proportional gain
float Ki = 0.05;           // Integral gain
float Kd = 35;             // Derivative gain
```

---

## 🖨️ 3D Printed Chassis

The entire chassis was designed in-house and printed using **PLA filament**. The design was optimized for:

- Low weight
- Sensor mounting at the correct height and angle
- Easy access to electronics and wiring

---
