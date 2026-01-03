# ADXL345 Accelerometer Tutorial

The **ADXL345** is a 3-axis digital accelerometer that can communicate with microcontrollers via **I²C** or **SPI**. This tutorial explains how to set up and use both communication methods.

---

## 1. Introduction to Communication Protocols

### 1.1 I²C (Inter-Integrated Circuit)

* Uses **2 wires**: SDA (data) and SCL (clock).
* Devices have **unique addresses**.
* Slower than SPI but uses fewer pins.

### 1.2 SPI (Serial Peripheral Interface)

* Uses **4 wires**: MOSI (Master Out), MISO (Master In), SCK (Clock), CS (Chip Select).
* Faster than I²C, full-duplex communication.
* Requires a separate **CS** pin for each device.

> **Note:** The ADXL345 selects the interface via the **CS pin**:
>
> * `CS = HIGH` → I²C mode
> * `CS = LOW` → SPI mode

---

## 2. Wiring the ADXL345

### 2.1 I²C Connections

| ADXL345 Pin | Microcontroller Pin           |
| ----------- | ----------------------------- |
| VCC         | 3.3V / 5V                     |
| GND         | GND                           |
| SDA         | MCU SDA                       |
| SCL         | MCU SCL                       |
| CS          | HIGH (or VCC)                 |
| SDO         | Optional: selects I²C address |

> Pull-up resistors (~4.7kΩ) may be required on SDA and SCL lines.

### 2.2 SPI Connections

| ADXL345 Pin | Microcontroller Pin           |
| ----------- | ----------------------------- |
| VCC         | 3.3V / 5V                     |
| GND         | GND                           |
| SDA         | SPI MOSI                      |
| SDO         | SPI MISO                      |
| SCL         | SPI SCK                       |
| CS          | SPI CS (LOW to select device) |

---

## 3. Arduino I²C Example

```cpp
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(9600);
  Wire.begin(); // Start I²C
  if (!accel.begin()) {
    Serial.println("Could not find ADXL345 on I²C");
    while (1);
  }
  Serial.println("ADXL345 initialized in I²C mode");
}

void loop() {
  sensors_event_t event; 
  accel.getEvent(&event);
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" m/s² ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" m/s² ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s²");
  delay(500);
}
```

---

## 4. Arduino SPI Example

```cpp
#include <SPI.h>
#include <Adafruit_ADXL345_U.h>

#define CS_PIN 10  // Chip Select pin

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void setup() {
  Serial.begin(9600);
  SPI.begin();               // Start SPI
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // Deselect device by default

  if (!accel.begin_SPI(CS_PIN)) {
    Serial.println("Could not find ADXL345 on SPI");
    while (1);
  }
  Serial.println("ADXL345 initialized in SPI mode");
}

void loop() {
  sensors_event_t event; 
  accel.getEvent(&event);
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" m/s² ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" m/s² ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s²");
  delay(500);
}
```

---

## 5. Tips and Notes

* Always **set CS pin correctly before powering** the device.
* Default I²C address: `0x53` if SDO = LOW, `0x1D` if SDO = HIGH.
* SPI is preferred for **high-speed data acquisition**.
* I²C is simpler for **multi-device setups with fewer pins**.
* Ensure voltage compatibility with your microcontroller (3.3V recommended).

---

This tutorial provides a **complete but simple guide** to using ADXL345 with either I²C or SPI.

---

