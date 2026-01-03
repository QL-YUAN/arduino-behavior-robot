---
title: Arduino Behavior-Based Robot Tutorial
---
# High-Level Robot Programming on Arduino  
## Behavior-Tree Thinking Without a Framework

---

## 1. Objective

This tutorial teaches how to program a mobile robot using **high-level logic** instead of writing a long sequence of instructions.

Students will learn how to:
- Separate **decision logic**, **actions**, and **hardware control**
- Write Arduino code that is **easy to read and extend**
- Implement behavior-tree-style logic using normal Arduino C++

---

## 2. Robot Behavior Description

The robot must behave as follows:

> IF an obstacle is detected  
> → turn to avoid it  
> ELSE  
> → move forward  

This structure is equivalent to a **Selector** in a behavior tree.

---

## 3. Hardware Components

- Arduino UNO
- L298N motor driver
- 2 DC motors
- HC-SR04 ultrasonic distance sensor
- External battery (6–9V)
- Jumper wires

---

## 4. Hardware Connections

### 4.1 Motor Driver (L298N)

| L298N Pin | Arduino Pin |
|----------|------------|
| ENA | 5 |
| IN1 | 8 |
| IN2 | 9 |
| ENB | 6 |
| IN3 | 10 |
| IN4 | 11 |

![Pinout Diagram](assets/L298N.png)
<p align="center">
  <img src="assets/L298N.png" width="700"><br>
  <em>Figure 1: Pinout Diagram</em>
</p>
- Connect motors to OUT1–OUT4
- Remove ENA and ENB jumpers
- Motor power must come from an external battery
- Arduino GND and L298N GND **must be connected together**

---

### 4.2 Ultrasonic Sensor (HC-SR04)

| Sensor Pin | Arduino Pin |
|-----------|------------|
| VCC | 5V |
| GND | GND |
| TRIG | 2 |
| ECHO | 3 |

---

## 5. Programming Rule (Very Important)

> The `loop()` function must only describe **robot behavior**.  
> It must not directly control motors or sensors.

---

## 6. Software Structure

The program is divided into four layers:

1. **Main Loop** – calls the robot behavior
2. **Behavior Layer** – decision logic
3. **Action Layer** – robot actions
4. **Hardware Layer** – motors and sensors

---

## 7. Main Program Structure

```cpp
void loop() {
  robotBehavior();
}
````

This ensures all logic is centralized and readable.

---

## 8. Behavior Logic

```cpp
void robotBehavior() {
  if (obstacleDetected()) {
    avoidObstacle();
  } else {
    moveForward();
  }
}
```

This code describes *what* the robot does, not *how*.

---

## 9. Condition Function

```cpp
bool obstacleDetected() {
  int distance = readDistanceCM();
  return distance < 20;
}
```

Conditions always return **true or false**.

---

## 10. Action Functions

### Move Forward

```cpp
void moveForward() {
  setMotorSpeed(120, 120);
}
```

### Avoid Obstacle

```cpp
void avoidObstacle() {
  setMotorSpeed(-100, 100);
  delay(300);
}
```

Actions may contain delays and motor control.

---

## 11. Hardware Control

### Motor Control

```cpp
void setMotorSpeed(int left, int right) {
  digitalWrite(IN1, left >= 0 ? HIGH : LOW);
  digitalWrite(IN2, left >= 0 ? LOW : HIGH);
  analogWrite(ENA, abs(left));

  digitalWrite(IN3, right >= 0 ? HIGH : LOW);
  digitalWrite(IN4, right >= 0 ? LOW : HIGH);
  analogWrite(ENB, abs(right));
}
```

---

### Ultrasonic Distance Measurement

```cpp
int readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration * 0.034 / 2;
}
```

---

## 12. Behavior Tree Mapping

```
Selector
 ├─ Condition: obstacleDetected
 │    └─ Action: avoidObstacle
 └─ Action: moveForward
```

---

## 13. Student Exercise

Modify the behavior so that:

* Distance < 10 cm → stop
* Distance 10–20 cm → turn
* Distance > 20 cm → move forward

Only the **behavior function** should be modified.

---

## 14. Key Takeaway

> If you can understand robot behavior without reading hardware code,
> the program is well designed.
[Download Arduino Code](behavior_robot.ino)

---

# 🤖 2️⃣ Final Arduino Source Code  
### **`behavior_robot.ino`**

This is the **exact file** you prepare for students.

```cpp
// ==================================================
// High-Level Robot Programming Example
// Behavior-Tree Style Logic on Arduino
// ==================================================

// ===== Motor Pins (L298N) =====
const int ENA = 5;
const int IN1 = 8;
const int IN2 = 9;

const int ENB = 6;
const int IN3 = 10;
const int IN4 = 11;

// ===== Ultrasonic Sensor Pins =====
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// ==================================================
// Setup
// ==================================================
void setup() {
  initMotors();
  initUltrasonic();
}

// ==================================================
// Main Loop (High-Level Only)
// ==================================================
void loop() {
  robotBehavior();
}

// ==================================================
// Behavior Layer (Decision Logic)
// ==================================================
void robotBehavior() {
  if (obstacleDetected()) {
    avoidObstacle();
  } else {
    moveForward();
  }
}

// ==================================================
// Condition
// ==================================================
bool obstacleDetected() {
  int distance = readDistanceCM();
  return distance < 20;
}

// ==================================================
// Actions
// ==================================================
void moveForward() {
  setMotorSpeed(120, 120);
}

void avoidObstacle() {
  setMotorSpeed(-100, 100);
  delay(300);
}

// ==================================================
// Hardware Layer
// ==================================================

// ----- Motor Control -----
void initMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void setMotorSpeed(int left, int right) {
  // Left motor
  digitalWrite(IN1, left >= 0 ? HIGH : LOW);
  digitalWrite(IN2, left >= 0 ? LOW : HIGH);
  analogWrite(ENA, abs(left));

  // Right motor
  digitalWrite(IN3, right >= 0 ? HIGH : LOW);
  digitalWrite(IN4, right >= 0 ? LOW : HIGH);
  analogWrite(ENB, abs(right));
}

// ----- Ultrasonic Sensor -----
void initUltrasonic() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

int readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  int distance = duration * 0.034 / 2;

  return distance;
}
````
## More Tutorials

- [Arduino I²C Tutorial](i2c-tutorial.md)
- [ADXL345Tutorial](ADXL345-tutorial.md)
## References

1. lakshyajhalani56, L298n Motor driver Arduino | Motors | Motor Driver | L298n
   https://projecthub.arduino.cc/lakshyajhalani56/l298n-motor-driver-arduino-motors-motor-driver-l298n-7e1b3b

2. Isaac100
   https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
