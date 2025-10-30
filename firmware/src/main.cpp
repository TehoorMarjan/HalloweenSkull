#include <Arduino.h>
#include <Eyes.h>

// Define hardware type (use GENERIC if unsure, or FC16_HW for common modules)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Define SPI pins for ESP32
#define CLK_PIN   18  // Clock pin
#define DATA_PIN  23  // MOSI pin (Data In)
#define CS_PIN    5   // Chip Select pin

// Create Eyes object
Eyes eyes(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN);

uint8_t positionUpDown = 6; // Start with eyes looking down
uint8_t positionLeftRight = 3; // Center horizontally
uint8_t count = 0;
EyeMode currentMode = NORMAL;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("Eyes Test - Position (3, 0)");
  eyes.begin();
  eyes.immediatePosition(3, 0); // Look down
  eyes.setBrightness(8); // Medium brightness
  eyes.requestPosition(positionLeftRight, positionUpDown); // Look up (animate)
}

void loop() {
  // Update the display
  eyes.update();

  // Animate position between top and bottom
  if (!eyes.isAnimating()) {
    delay(1000); // Hold position for a second
    if (positionUpDown == 0 and positionLeftRight == 3) {
      positionLeftRight = 0; // Move left
      positionUpDown = 3;
    } else if (positionUpDown == 3 and positionLeftRight == 0) {
      positionLeftRight = 6; // Move right
      positionUpDown = 3;
    } else if (positionUpDown == 3 and positionLeftRight == 6) {
      positionLeftRight = 3; // Move down
      positionUpDown = 6;
    } else {
      positionLeftRight = 3; // Move up
      positionUpDown = 0;
    }
    eyes.requestPosition(positionLeftRight, positionUpDown);
    count++;
    // Close eyes every 5 cycles, reopen after 2 cycles
    if (count >= 5 && currentMode == NORMAL) {
      count = 0;
      currentMode = CLOSED;
      eyes.requestMode(currentMode);
    } else if (count >= 2 && currentMode == CLOSED) {
      count = 0;
      currentMode = NORMAL;
      eyes.requestMode(currentMode);
    }
  }
  
  delay(25); // Small delay to avoid excessive updates
}