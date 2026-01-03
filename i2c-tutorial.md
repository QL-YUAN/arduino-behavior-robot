# 📘 Arduino I²C Tutorial

This tutorial explains **I²C communication** on Arduino, including device addressing, registers, and a concrete example using the **ADXL345 accelerometer**.

---

## 1️⃣ What is I²C?

**I²C (Inter-Integrated Circuit)** is a **synchronous serial communication protocol** that allows multiple devices to communicate using only **two wires**:

- **SDA** – Serial Data
- **SCL** – Serial Clock

**Key features:**

- Supports multiple devices on the same bus
- Requires pull-up resistors for proper signaling
- Widely used for sensors, displays, and memory ICs

---

## 2️⃣ Arduino I²C Pins

| Arduino Board | SDA | SCL |
|---------------|-----|-----|
| UNO           | A4  | A5  |
| Mega          | 20  | 21  |
| Nano / others | Check labeling near USB |

> SDA and SCL may also be marked directly on the board.

---

## 3️⃣ Voltage Levels and Logic Level Converters

- Arduino UNO: 5V logic  
- Many sensors (e.g., ADXL345): 3.3V logic  

**Solution:** Use a **logic level converter** to safely connect 5V Arduino to 3.3V sensors.

**Example:**

```text
Arduino 5V → HV
Arduino GND → GND
Converter LV → 3.3V
Converter GND → GND
Arduino SDA → A1 → B1 → Sensor SDA
Arduino SCL → A2 → B2 → Sensor SCL
````

---

## 4️⃣ Pull-Up Resistors

I²C lines are **open-drain**, so pull-ups are required:

```
SDA ── 4.7kΩ ── VCC
SCL ── 4.7kΩ ── VCC
```

✅ Many breakout boards include pull-ups.
❌ Avoid multiple strong pull-ups in parallel.

---

## 5️⃣ 7-Bit Device Addressing

* Each I²C device has a **7-bit address** (0–127)
* The **8th bit** in the transmitted byte is **Read/Write (R/W)**

### How it works

| Byte on bus | Description           |
| ----------- | --------------------- |
| 1010011 0   | Write to device 0x53  |
| 1010011 1   | Read from device 0x53 |

* `0x53` = `1010011₂` = 83 decimal
* Arduino libraries use the **7-bit address**, **not** the R/W bit

**Reserved addresses:** 0x00, 0x01–0x07, 0x78–0x7F
**Usable addresses ≈ 112 devices**

**Why 7 bits?**
Because the 8th bit is used for **read/write control**, making hardware simpler.

---

## 6️⃣ Using I²C on Arduino

### I²C Scanner

Use this first to detect devices and addresses:

```cpp
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Scanner");
}

void loop() {
  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Device found at 0x");
      Serial.println(address, HEX);
      nDevices++;
    }
  }

  if (nDevices == 0) Serial.println("No I2C devices found");
  delay(5000);
}
```

---

### Writing to a device

```cpp
Wire.beginTransmission(0x53); // ADXL345 device address
Wire.write(0x2D);             // POWER_CTL register address
Wire.write(0x08);             // Enable measurement mode
Wire.endTransmission();
```

**Explanation:**

| Command | Meaning                           |
| ------- | --------------------------------- |
| `0x53`  | Device address (ADXL345)          |
| `0x2D`  | POWER_CTL register inside ADXL345 |
| `0x08`  | Data written → set MEASURE = 1    |

> Think of **device address** as the apartment building number, **register address** as the apartment number, and **data** as what you store inside.

---

### Reading from a device

```cpp
Wire.beginTransmission(0x53);
Wire.write(0x32);             // DATAX0 register (X-axis LSB)
Wire.endTransmission(false);  // Repeated start

Wire.requestFrom(0x53, 6);    // Read 6 bytes (X, Y, Z)
int x = Wire.read() | (Wire.read() << 8);
int y = Wire.read() | (Wire.read() << 8);
int z = Wire.read() | (Wire.read() << 8);
```

**Explanation:**
#### 🧮 Reading Multi-Byte Data from I²C Devices

When using I²C sensors like the **ADXL345 accelerometer**, you often need to read **16-bit values** that are split into two 8-bit registers: a **Low Byte** and a **High Byte**.  

For example, the X-axis acceleration is stored in:

- `DATAX0` → Low byte (0x32)
- `DATAX1` → High byte (0x33)

---

1️⃣ The Code

```cpp
int x = Wire.read() | (Wire.read() << 8);
````

---

2️⃣ Step-by-Step Explanation

#### Step 1: Read the low byte

```cpp
Wire.read()
```

* Reads the first byte from the I²C device (DATAX0)
* Example: `0x34` → 52 decimal

#### Step 2: Read the high byte and shift

```cpp
Wire.read() << 8
```

* Reads the second byte from the device (DATAX1)
* Shifts it 8 bits to the left to **become the high byte**
* Example: `0x12 << 8` → `0x1200` → 4608 decimal

#### Step 3: Combine using bitwise OR `|`

```cpp
0x34 | 0x1200 = 0x1234
```

✅ Now `x` contains the full **16-bit signed value** from the sensor.

---

3️⃣ Why Use Bitwise OR?

* Each byte is 8 bits. A 16-bit value = High Byte + Low Byte
* OR-ing combines them without overwriting each other:

```
0x1200
OR
0x0034
=
0x1234
```

---
| Register    | Purpose                                     |
| ----------- | ------------------------------------------- |
| `0x32`      | X-axis LSB (auto-increments for next bytes) |
| `0x33`      | X-axis MSB                                  |
| `0x34–0x36` | Y/Z-axis LSB/MSB                            |

---

### Quick Reference Table

| Concept                  | Example / Code                 | Meaning                          |
| ------------------------ | ------------------------------ | -------------------------------- |
| I²C device address       | `0x53`                         | ADXL345 I²C address (7-bit)      |
| Register address         | `0x2D`                         | POWER_CTL register               |
| Register value           | `0x08`                         | Set measurement mode             |
| Data register            | `0x32`                         | Start of X/Y/Z acceleration data |
| Wire.beginTransmission() | `Wire.beginTransmission(0x53)` | Start I²C communication          |
| Wire.endTransmission()   | `Wire.endTransmission()`       | End I²C communication            |
| Wire.requestFrom()       | `Wire.requestFrom(0x53, 6)`    | Request bytes from device        |

---

## 7️⃣ Summary

* I²C uses **7-bit device addresses**; the 8th bit is for read/write.
* Each device has **internal registers** addressed separately.
* Use `Wire.write(register)` to select a register, `Wire.write(value)` to write.
* Use `Wire.requestFrom()` to read multiple bytes.
* **ADXL345 example**: POWER_CTL register = `0x2D`, enable measurement = `0x08`, read data starting at `0x32`.

---

> I²C is a simple yet powerful protocol once you understand **device addresses vs register addresses**.
> Think: **Device = building, Register = apartment, Data = contents**.

---


