#include <Arduino.h>
#include <esp_sleep.h>
#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>
#include <Eyes.h>
#include <Sounds.h>
#include "config.h"

void animateEyes();
void maybePlaySound(bool yawn = false);

static const SoundsConfig soundConfig = DFPLAYER_CONFIG;

// Create Eyes object
Eyes eyes(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN);

// Create DFPlayer object
Sounds sounds(DFPLAYER_RX, DFPLAYER_TX);

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

unsigned long lastSoundTime = 0;
unsigned long soundDelay = 0;

bool dfPlayerAvailable = false;

void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize DFPlayer
  sounds.begin(soundConfig);
  
  // Initialize eyes
  eyes.begin();
  eyes.immediatePosition(3, 3); // Center
  eyes.setBrightness(EYES_BRIGHTNESS);
  eyes.immediateMode(CLOSED);
  eyes.requestMode(NORMAL); // Start with animation of opening eyes
}

void loop()
{
  animateEyes();
  maybePlaySound();
  delay(25);
  // Below breaks dfPlayer operation
  //esp_sleep_enable_timer_wakeup(25 * 1000); // 25ms in microseconds
  //esp_light_sleep_start();
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
      maybePlaySound(true);
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

void maybePlaySound(bool yawn)
{
  unsigned long now = millis();
  if (now - lastSoundTime < soundDelay)
  {
    if (!yawn)
    {
      return; // Not time yet
    }
    else
    {
      // Allow yawns to proceed even if sound delay not yet passed
      // But only if minimum yawning interval has passed
      if (now - lastSoundTime < MIN_YAWNING_INTERVAL)
      {
        return; // Not enough time since last sound, cancel yawn
      }
    }
  }

  lastSoundTime = now;
  if (yawn)
  {
    sounds.playYawningSound();
  }
  else
  {
    sounds.playSpeechOrEffectSound();
  }

  soundDelay = random(MIN_SOUND_DELAY, MAX_SOUND_DELAY);
}