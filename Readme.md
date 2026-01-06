# AS7343 Arduino Library

Arduino driver for the **AMS AS7343 spectral sensor**. This library provides simple, low-level register access as well as high-level helpers to efficiently read the full spectral data set.

---

## Features

* Read single 8‑bit registers (status, IDs, flags)
* Read 16‑bit data values (auto‑increment, little‑endian)
* Burst read of **all spectral channels** (efficient, latched)
* Optional single‑channel access helpers
* AVR‑safe `Wire` usage (repeated START, explicit casting)


---

## Installation

### Option 1: Install as an Arduino Library (Recommended)

1. Download or clone this repository
2. Rename the folder to **AS7343** (important)
3. Copy it to your Arduino libraries directory:

* **Windows**: `Documents/Arduino/libraries/`
* **Linux**: `~/Arduino/libraries/`
* **macOS**: `~/Documents/Arduino/libraries/`

Final structure should look like:

```
Arduino/
└── libraries/
    └── AS7343/
        ├── src/
        │   ├── AS7343.h
        │   └── AS7343.cpp
        └── README.md
```

4. Restart the Arduino IDE

---

### Option 2: Install using ZIP (Arduino IDE)

1. Download the library as a **ZIP file**
2. Open Arduino IDE
3. Go to **Sketch → Include Library → Add .ZIP Library…**
4. Select the downloaded ZIP file
5. Restart the IDE if required

---

### Required Dependencies

* Arduino **Wire** library (built-in)
* Supported boards: **AVR (UNO, Nano)**

---

## Channel Mapping (18-Channels ~ 14 unique channels)
When calling `readAllChannels()`, the `uint16_t` array is populated in the order dictated by the hardware's `auto_smux` Mode 3:

| Index | Label | Peak Wavelength | Index | Label | Peak Wavelength |
|:---:|:---:|:---:|:---:|:---:|:---:|
| **0** | FZ | 450nm | **9** | F6 | 640nm |
| **1** | FY | 555nm | **10** | VIS | (Clear) |
| **2** | FXL | 600nm | **11** | FD | (Flicker) |
| **3** | NIR | 855nm | **12** | F1 | 405nm |
| **4** | VIS | (Clear) | **13** | F7 | 690nm |
| **5** | FD | (Flicker) | **14** | F8 | 745nm |
| **6** | F2 | 425nm | **15** | F5 | 550nm |
| **7** | F3 | 475nm | **16** | VIS | (Clear) |
| **8** | F4 | 515nm | **17** | FD | (Flicker) |

---

## Basic Usage

```cpp
#include <Wire.h>
#include "AS7343.h"

AS7343 sensor(0x39);

void setup() {
  Serial.begin(115200);

  if (sensor.begin() < 0) {
    Serial.println("AS7343 not detected");
    while (1);
  }

  Serial.println("AS7343 initialized");
}

void loop() {
  delay(1000);
}
```

### 1. `writeRegister(reg, val)`
Used to modify specific hardware settings.
**Example: Manually turning on the internal LED.**

```cpp
uint8_t errorCode = sensor.writeRegister(LED, 0x80); 

if (errorCode == 0) {
    Serial.println("LED turned on successfully");
} else {
    Serial.print("I2C Error: ");
    Serial.println(errorCode);
}
```
---

## 2. `readRegister(reg)`

Used to read a **single 8‑bit register** such as status flags or identification registers.

### Example: Reading the Device ID

```cpp
// 0x5A is the Device ID register for AS7343 which 0x81(different from device address)

//To access register with address below 0x80 , 4th bit of CFG0 must be set to 1

sensor.writeRegister(CFG0,16);
uint8_t deviceID = sensor.readRegister(0x5A);

Serial.print("Device ID (should be 0x81 for AS7343): 0x");
Serial.println(deviceID, HEX);
```

---

## 3. `read16(reg)`

Reads **two consecutive registers** and combines them into a **16‑bit value**.

* Low byte is read first
* Register address auto‑increments

### Example: Reading a raw data register manually

```cpp
// Reading DATA_0 (Registers 0x95 and 0x96)
uint16_t rawData0 = sensor.read16(0x95);

Serial.print("Raw Channel 0 Value: ");
Serial.println(rawData0);
```

---

## 4. `readAllChannels(data*)`

The **most efficient method** to capture the full spectrum.

* Uses a single I²C burst read
* Reads `ASTATUS` first to **latch all data registers**
* Handles the internal SMUX sequencing
* Avoids I²C buffer overflows

### Example: Reading the full spectrum

```cpp
uint16_t spectrum[18];

void loop() {
  if (sensor.readAllChannels(spectrum)) {
    // Access channels using the mapping table
    uint16_t f1_380nm      = spectrum[12];
    uint16_t nir_855nm     = spectrum[3];
    uint16_t visible_clear = spectrum[4];

    Serial.print("380nm (UV/Violet): ");
    Serial.println(f1_380nm);

    Serial.print("855nm (NIR): ");
    Serial.println(nir_855nm);
  }

  delay(1000);
}
```

---

## 5. `readChannel(n)`

Triggers a measurement and returns the **16‑bit value of a single channel**.

* Channel index range: `0 – 17`
* Internally performs a full measurement cycle

### Example: Simple NIR detector

```cpp
// In 18‑channel mode, NIR is located at index 3
uint16_t nirLevel = sensor.readChannel(3);

if (nirLevel > 5000) {
  Serial.println("High Infrared intensity detected!");
}
```

---

## 6. `printData(data*, n)`

Helper function to quickly dump spectral data to the **Serial Monitor**.

Useful for debugging and calibration.

### Example: Printing all channels

```cpp
uint16_t myData[18];

if (sensor.readAllChannels(myData)) {
  // Prints all 18 values with indices and labels
  sensor.printData(myData, 18);
}
```

---

## Notes & Best Practices

* Always read `ASTATUS (0x94)` before bulk data reads
* Data registers are **little‑endian** (LOW byte first)
* Do not shift the I²C address (use 7‑bit `0x39` only)
* On AVR, always use `endTransmission(false)` for repeated START

---





