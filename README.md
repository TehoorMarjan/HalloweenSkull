# 🎃 Animated Skull Eyes - Halloween 2025

An ESP32-powered animated skull with googly eyes using two 8x8 LED matrices
controlled by MAX7219 drivers. Perfect for creating a spooky, interactive
Halloween decoration!

<p align="center">
    <img src="docs/assets/skull.jpg" alt="Skull Implementation" style="max-width:800px; width:100%; height:auto;" />
</p>

## 📖 Overview

This project brings life to a decorative skull using animated LED matrix eyes.
The eyes feature smooth animations including looking around, blinking, and
special effects like cross-eyed and silly modes. Built for Halloween 2025, this
creates an engaging and creepy decoration that watches visitors!

## 🛠️ Hardware

### Components

- **Microcontroller**: ESP32 development board
- **LED Matrices**: 2x MAX7219 8x8 LED dot matrix modules (daisy-chained)
- **Base Skull**:
  [5 Pcs Halloween Skeleton Head and Arm Set](https://www.amazon.fr/dp/B0FBW7PP8G) -
  A realistic skeleton skull decoration kit
- **Power Supply**: USB or external 5V power source

### Wiring

| ESP32 Pin | MAX7219 Pin | Description       |
| --------- | ----------- | ----------------- |
| GPIO 18   | CLK         | Clock signal      |
| GPIO 23   | DIN         | Data input (MOSI) |
| GPIO 5    | CS          | Chip select       |
| 5V        | VCC         | Power supply      |
| GND       | GND         | Ground            |

The two MAX7219 modules are daisy-chained together:

- Device 0 (first in chain) = Right eye
- Device 1 (second in chain) = Left eye

## ✨ Features

### Eye Animations

- **Googly Eyes**: Eyes appear as filled circles with a 2x2 pixel "iris" that
  moves around
- **Smooth Movement**: Interpolated position changes for natural eye movements
- **Multiple Modes**:
  - `NORMAL`: Standard eyes with moveable irises
  - `CLOSED`: Blinking/sleeping effect
  - `CROSS`: Cross-eyed look _(Not implemented yet)_
  - `SILLY`: Goofy animation _(Not implemented yet)_

## 🚀 Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- USB cable for ESP32 programming
- Basic soldering skills for connections

### Installation

1. **Clone the repository**:

   ```bash
   git clone [<repository-url>](https://github.com/TehoorMarjan/HalloweenSkull.git)
   cd HalloweenSkull/firmware
   ```

2. **Install dependencies**: The `MD_MAX72xx` library will be automatically
   installed via PlatformIO.

3. **Configure pins** (if different): Edit `firmware/src/main.cpp` and adjust
   pin definitions:

   ```cpp
   #define CLK_PIN   18
   #define DATA_PIN  23
   #define CS_PIN    5
   ```

4. **Build and upload**:

   ```bash
   cd firmware
   platformio run --target upload
   ```

5. **Monitor serial output** (optional):
   ```bash
   platformio device monitor
   ```

### Hardware Assembly

1. **Mount the LED matrices** inside the skull's eye sockets
2. **Secure the ESP32** inside the skull cavity
3. **Connect wiring** according to the wiring table above
4. **Power the system** via USB or external 5V supply
5. **Close up the skull** and enjoy your creation!

### Building from Source

```bash
cd firmware
platformio run
```

## 📝 License

This project is under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🎃 Happy Halloween! 🎃

---

_Created for Halloween 2025 - May your decorations be spooky and your code
bug-free! 👻_
