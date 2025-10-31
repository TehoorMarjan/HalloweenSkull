#include "Sounds.h"

Sounds::Sounds(int8_t rxPin, int8_t txPin): serial(2), dfPlayer()
{
    // Initialize DFPlayer Mini
    this->rxPin = rxPin;
    this->txPin = txPin;
}

void Sounds::begin(const SoundsConfig &config)
{
    this->config = config;
    serial.begin(9600, SERIAL_8N1, rxPin, txPin);
    delay(1000);  // Give DFPlayer time to initialize
    if (dfPlayer.begin(serial)) {
        dfPlayer.setTimeOut(500); // Set serial timeout to 500ms
        dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
        dfPlayer.volume(config.volume);      // Set volume to a reasonable level
        dfPlayerAvailable = true;
    } else {
        dfPlayerAvailable = false;
    }
}

bool Sounds::playYawningSound()
{
  if (dfPlayerAvailable && (dfPlayer.readState() != 1))
  {
    uint8_t soundIndex = random(1, config.yawningNbSounds + 1);
    dfPlayer.playFolder(config.yawningFolder, soundIndex);
    return true;
  }
  return false;
}

bool Sounds::playSpeechOrEffectSound()
{
  if (dfPlayerAvailable && (dfPlayer.readState() != 1))
  {
    uint8_t speechoreffect = random(0, 2);
    uint8_t folderNumber;
    uint8_t nbSounds;
    if (speechoreffect == 0 || config.speechNbSounds == 0)
    {
      folderNumber = config.speechFolder;
      nbSounds = config.speechNbSounds;
    }
    else
    {
      folderNumber = config.effectFolder;
      nbSounds = config.effectNbSounds;
    }
    uint8_t soundIndex = random(1, nbSounds + 1);
    dfPlayer.playFolder(folderNumber, soundIndex);
    return true;
  }
  return false;
}