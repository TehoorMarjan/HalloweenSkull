#ifndef SOUNDS_H
#define SOUNDS_H

#include <DFRobotDFPlayerMini.h>
#include <HardwareSerial.h>

// Structure for folder configuration
typedef struct
{
    uint8_t volume;       // Volume level (0-30)
    uint8_t yawningFolder; // Folder number for yawning sounds
    uint8_t speechFolder;  // Folder number for speech sounds
    uint8_t effectFolder;  // Folder number for effect sounds
    uint8_t yawningNbSounds; // Number of yawning sound files
    uint8_t speechNbSounds;  // Number of speech sound files
    uint8_t effectNbSounds;  // Number of effect sound files
} SoundsConfig;

class Sounds
{
public:
    Sounds(int8_t rxPin, int8_t txPin);

    void begin(const SoundsConfig &config);

    bool playYawningSound();
    bool playSpeechOrEffectSound();

private:
    HardwareSerial serial;
    DFRobotDFPlayerMini dfPlayer;
    bool dfPlayerAvailable;
    int8_t rxPin;
    int8_t txPin;
    SoundsConfig config;
};

#endif // SOUNDS_H
