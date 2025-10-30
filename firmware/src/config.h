#ifndef CONFIG_H
#define CONFIG_H

// Define SPI pins for ESP32
#define CLK_PIN 18  // Clock pin
#define DATA_PIN 23 // MOSI pin (Data In)
#define CS_PIN 5    // Chip Select pin

#define CLOSED_MODE_PROBABILITY 10 // Probability of entering CLOSED mode each update cycle (percent)

#define MIN_RANDOM_DELAY 1000 // Minimum random delay between animations (ms)
#define MAX_RANDOM_DELAY 5000 // Maximum random delay between animations (ms)

#define MIN_RANDOM_DELAY_CLOSED 3000 // Minimum random delay between CLOSED mode animations (ms)
#define MAX_RANDOM_DELAY_CLOSED 6000 // Maximum random delay between CLOSED mode animations (ms)

#endif // CONFIG_H
