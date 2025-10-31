# 👁️ HalloweenSkull Firmware 🎃

Welcome to the **HalloweenSkull** firmware! This is the embedded software that brings your animated skull to life with expressive googly eyes and spooky sounds.

## 🚀 Overview

This firmware is designed to run on an **ESP32** microcontroller and controls:
- **Two 8x8 LED matrices** (MAX7219-based) for animated googly eyes
- **DFPlayer Mini** audio module for playing sounds from an SD card

The skull autonomously animates its eyes with realistic movements and randomly plays sounds to create an immersive Halloween experience.

## 🛠️ Hardware Requirements

- **ESP32 Development Board** (e.g., AZ-Delivery DevKit v4)
- **2x MAX7219 8x8 LED Dot Matrix Modules** (daisy-chained)
- **DFPlayer Mini MP3 Player Module**
- **MicroSD Card** (for audio files)
- **Speaker** (connected to DFPlayer Mini)
- **Power Supply** (5V recommended)

### 📌 Pin Connections

#### LED Matrices (SPI)
- **CLK** → GPIO 18
- **DATA (MOSI)** → GPIO 23
- **CS** → GPIO 5

#### DFPlayer Mini (UART)
- **RX** (ESP32 TX2) → GPIO 17
- **TX** (ESP32 RX2) → GPIO 16

> You can modify pin assignments in `src/config.h`

## 📁 Project Structure

```
firmware/
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp           # Main program logic
│   └── config.h           # Configuration constants
└── lib/
    ├── Eyes/              # Eye animation library
    │   ├── Eyes.h
    │   └── Eyes.cpp
    └── Sounds/            # Sound playback library
        ├── Sounds.h
        └── Sounds.cpp
```

## 🎨 Features

### Eye Animations
- **Smooth movement** between positions (top, bottom, left, right, center, diagonals)
- **Blinking/closing** animation with configurable probability
- **Interpolation** for natural eye movement
- **Adjustable brightness** (0-15)

### Sound Effects
The firmware plays three types of sounds from the SD card:
- **Yawning sounds** (folder 02) - triggered when eyes close
- **Speech sounds** (folder 01) - random spooky phrases
- **Effect sounds** (folder 03) - atmospheric effects

Sounds are played randomly with configurable delays to keep the experience unpredictable.

## 🔧 Building & Uploading

### Prerequisites
1. Install [PlatformIO](https://platformio.org/install) (VS Code extension recommended)
2. Prepare your SD card with sound files organized in folders:
   - `01/` - Speech sounds (001.mp3, 002.mp3, ...)
   - `02/` - Yawning sounds
   - `03/` - Effect sounds

### Build Commands

```bash
# Build the project
pio run

# Upload to ESP32
pio run --target upload

# Open serial monitor
pio device monitor
```

Or use the PlatformIO buttons in VS Code! 🔘

## 🎃 Customization Ideas

- Add new eye modes (cross-eyed, silly animations)
- Implement motion sensor triggers
- Add remote control via WiFi/Bluetooth
- Create synchronized light effects
- Add more complex sound sequences

## 📜 License

This project is part of the HalloweenSkull repository.

License is MIT. See the main [LICENSE](../LICENSE) file for details.

## 🤝 Contributing

Contributions are welcome! Feel free to:
- Report bugs via GitHub issues
- Submit pull requests with improvements
- Share your customizations and modifications

## 💀 Happy Haunting! 👻
