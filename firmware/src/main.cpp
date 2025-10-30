#include <Arduino.h>
#include <esp_sleep.h>
#include <Eyes.h>
#include "config.h"

// Define hardware type (use GENERIC if unsure, or FC16_HW for common modules)
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

// Create Eyes object
Eyes eyes(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN);

enum EyePosition
{
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
  CENTER,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  CENTER_LEFT,
  CENTER_RIGHT,
  TOP_CENTER,
  BOTTOM_CENTER,
};

typedef struct
{
  uint8_t x;
  uint8_t y;
} Position;

Position eyePositions[] = {
    {3, 6}, // TOP
    {3, 0}, // BOTTOM
    {0, 3}, // LEFT
    {6, 3}, // RIGHT
    {3, 3}, // CENTER
    {1, 5}, // TOP_LEFT
    {5, 5}, // TOP_RIGHT
    {1, 1}, // BOTTOM_LEFT
    {5, 1}, // BOTTOM_RIGHT
    {4, 3}, // CENTER_LEFT
    {2, 3}, // CENTER_RIGHT
    {3, 4}, // TOP_CENTER
    {3, 2}  // BOTTOM_CENTER
};

EyeMode currentMode = NORMAL;

unsigned long lastAnimationEndTime = 0;
unsigned long randomDelay = 0;

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  eyes.begin();
  eyes.immediatePosition(3, 3); // Center
  eyes.setBrightness(EYES_BRIGHTNESS);
  eyes.immediateMode(CLOSED);
  eyes.requestMode(NORMAL); // Start with animation of opening eyes
}

void animateEyes()
{
  // Update the display
  eyes.update();

  if (eyes.isAnimating())
  {
    return; // Let animation finish
  }

  // Marker for end of animation, start of new delay
  if (lastAnimationEndTime == 0)
  {
    lastAnimationEndTime = millis();
    if (currentMode == CLOSED)
    {
      randomDelay = random(MIN_RANDOM_DELAY_CLOSED, MAX_RANDOM_DELAY_CLOSED);
    }
    else
    {
      randomDelay = random(MIN_RANDOM_DELAY, MAX_RANDOM_DELAY); // Initial random delay between 1 to 5 seconds
    }
  }

  // Wait until delay expires
  if (millis() - lastAnimationEndTime < randomDelay)
  {
    return; // Wait for delay to expire
  }

  // If we reach here, we can start a new animation
  lastAnimationEndTime = 0;

  // Decide next mode
  if (currentMode == CLOSED)
  {
    // If it was closed, open
    currentMode = NORMAL;
  }
  else
  {
    // Maybe close eyes, depending on probability
    int action = random(0, 100);
    if (action < CLOSED_MODE_PROBABILITY)
    {
      currentMode = CLOSED;
    }
  }
  eyes.requestMode(currentMode);

  if (currentMode == NORMAL)
  {
    // Pick a random position
    EyePosition pos = static_cast<EyePosition>(random(0, sizeof(eyePositions) / sizeof(eyePositions[0])));
    eyes.requestPosition(eyePositions[pos].x, eyePositions[pos].y);
  }
}

void loop()
{
  animateEyes();

  esp_sleep_enable_timer_wakeup(25 * 1000); // 25ms in microseconds
  esp_light_sleep_start();
}