#ifndef CONFIG_H
#define CONFIG_H

// Define hardware type (use GENERIC if unsure, or FC16_HW for common modules)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
// Define SPI pins for ESP32
#define CLK_PIN 18  // Clock pin
#define DATA_PIN 23 // MOSI pin (Data In)
#define CS_PIN 5    // Chip Select pin

#define CLOSED_MODE_PROBABILITY 10 // Probability of entering CLOSED mode each update cycle (percent)

#define MIN_RANDOM_DELAY 1000 // Minimum random delay between animations (ms)
#define MAX_RANDOM_DELAY 5000 // Maximum random delay between animations (ms)

#define MIN_RANDOM_DELAY_CLOSED 3000 // Minimum random delay between CLOSED mode animations (ms)
#define MAX_RANDOM_DELAY_CLOSED 6000 // Maximum random delay between CLOSED mode animations (ms)

#define EYES_BRIGHTNESS 8 // Default display brightness (0-15)

// DFPlayer Mini configuration
#define DFPLAYER_RX 16  // ESP32 RX2 → DFPlayer TX
#define DFPLAYER_TX 17  // ESP32 TX2 → DFPlayer RX

#define DFPLAYER_CONFIG { \
    .volume = 30, \
    .yawningFolder = 2, \
    .speechFolder = 1, \
    .effectFolder = 3, \
    .yawningNbSounds = 3, \
    .speechNbSounds = 9, \
    .effectNbSounds = 6 \
}

#define MIN_SOUND_DELAY 20000 // Minimum delay between sounds (ms)
#define MAX_SOUND_DELAY 60000 // Maximum delay between sounds (ms)
#define MIN_YAWNING_INTERVAL 20000 // Yawning may come sooner than other sounds (ms)

#endif // CONFIG_H
