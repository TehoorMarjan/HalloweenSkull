#ifndef EYES_H
#define EYES_H

#include <Arduino.h>
#include <MD_MAX72xx.h>

/**
 * @brief Eye animation modes
 */
enum EyeMode
{
    NORMAL, // Normal eyes with visible iris
    CLOSED, // Eyes closed
    CROSS,  // Cross-eyed effect
    SILLY   // Silly eye animation
};

/**
 * @brief Eyes class for controlling googly eyes on two 8x8 LED matrices
 *
 * This class abstracts the complexity of displaying animated eyes on two
 * daisy-chained MAX7219-driven 8x8 LED matrices. The eyes are represented
 * as all LEDs on except for a 2x2 square (the iris) that is off.
 */
class Eyes
{
public:
    /**
     * @brief Construct a new Eyes object
     *
     * @param hardwareType MD_MAX72XX hardware type (e.g., MD_MAX72XX::FC16_HW)
     * @param dataPin MOSI/Data pin for SPI communication
     * @param clkPin Clock pin for SPI communication
     * @param csPin Chip Select pin
     */
    Eyes(uint8_t hardwareType, uint8_t dataPin, uint8_t clkPin, uint8_t csPin);

    /**
     * @brief Initialize the Eyes display
     * Must be called in setup() before using other methods
     */
    void begin();

    /**
     * @brief Set the display brightness
     *
     * @param brightness Brightness level (0-15)
     */
    void setBrightness(uint8_t brightness);

    /**
     * @brief Set target position for both irises (synchronized movement)
     *
     * @param x Target x-coordinate (0-6)
     * @param y Target y-coordinate (0-6)
     */
    void requestPosition(uint8_t x, uint8_t y);

    /**
     * @brief Set target positions for left and right irises independently
     *
     * @param xl Target x-coordinate for left iris (0-6)
     * @param yl Target y-coordinate for left iris (0-6)
     * @param xr Target x-coordinate for right iris (0-6)
     * @param yr Target y-coordinate for right iris (0-6)
     */
    void requestPosition(uint8_t xl, uint8_t yl, uint8_t xr, uint8_t yr);

    /**
     * @brief Set position for both irises, no interpolation (synchronized movement)
     *
     * Note: you still need to call update() to refresh the display
     *
     * @param x Target x-coordinate (0-6)
     * @param y Target y-coordinate (0-6)
     */
    void immediatePosition(uint8_t x, uint8_t y);

    /**
     * @brief Set target positions for left and right irises independently, no interpolation
     *
     * Note: you still need to call update() to refresh the display
     *
     * @param xl Target x-coordinate for left iris (0-6)
     * @param yl Target y-coordinate for left iris (0-6)
     * @param xr Target x-coordinate for right iris (0-6)
     * @param yr Target y-coordinate for right iris (0-6)
     */
    void immediatePosition(uint8_t xl, uint8_t yl, uint8_t xr, uint8_t yr);

    /**
     * @brief Set the target eye mode
     *
     * @param mode Target mode (NORMAL, CLOSED, CROSS, SILLY)
     *
     * @return true if the mode change request was accepted
     */
    bool requestMode(EyeMode mode);

    /**
     * @brief Set the eye mode immediately, no animation
     *
     * @param mode Target mode (NORMAL, CLOSED, CROSS, SILLY)
     */
    void immediateMode(EyeMode mode);

    /**
     * @brief Update the display (call this in loop())
     *
     * Handles interpolation between current and target positions/modes,
     * updates the internal display buffer, and sends data to the matrices.
     */
    void update();

    /**
     * @brief Check if an animation is in progress
     *
     * @return true if animating, false if idle
     */
    bool isAnimating();

private:
    // MD_MAX72XX object for controlling the displays
    MD_MAX72XX mx;

    // Current and target iris positions (0-6 range due to 2x2 iris size)
    struct IrisPosition
    {
        uint8_t x; // Use float for smooth interpolation
        uint8_t y;
    };

    IrisPosition currentLeft;
    IrisPosition targetLeft;
    IrisPosition currentRight;
    IrisPosition targetRight;

    // Current and target modes
    EyeMode currentMode;
    EyeMode targetMode;

    // Internal display buffers for each eye (8 bytes per eye)
    uint8_t leftEyeBuffer[8];
    uint8_t rightEyeBuffer[8];

    // Effect step counter for stateful animations
    int step;
    // Store the last time an animation step was done
    unsigned long lastAnimationStepTimeNormal;
    unsigned long lastAnimationStepTimeClosed;
    unsigned long lastAnimationStepTimeCross;
    unsigned long lastAnimationStepTimeSilly;

    // Default number of devices
    static const uint8_t MAX_DEVICES = 2;
    static const uint8_t DEFAULT_BRIGHTNESS = 4;

    static const unsigned long ANIMATION_NORMAL_DELAY = 50; // ms between animation steps of normal effect
    static const unsigned long ANIMATION_CLOSED_DELAY = 75; // ms between animation steps of closed effect
    static const unsigned long ANIMATION_CROSS_DELAY = 100; // ms between animation steps of cross effect
    static const unsigned long ANIMATION_SILLY_DELAY = 125; // ms between animation steps of silly effect

    /**
     * @brief Generate eye patterns with irises at target positions
     *
     * Creates the internal buffer representation of both eyes with
     * the irises at the positions specified by targetLeft and targetRight.
     */
    void makeEyes();

    /**
     * @brief Send the internal buffers to the physical displays
     *
     * Transfers the leftEyeBuffer and rightEyeBuffer to the
     * corresponding MAX7219 devices.
     */
    void send();

    /**
     * @brief Normal eyes effect
     *
     * Animates the move of the irises to the target positions
     *
     * @return true if an animation step was performed
     */
    bool effectNormal();

    /**
     * @brief Closed eyes effect
     *
     * Animates the eyes closing (all LEDs off or partial closure)
     *
     * @return true if an animation step was performed
     */
    bool effectClosed();

    /**
     * @brief Cross-eyed effect
     *
     * Animates both irises moving toward the center (cross-eyed look)
     *
     * @return true if an animation step was performed
     */
    bool effectCross();

    /**
     * @brief Silly eyes effect
     *
     * Creates a silly/goofy eye animation
     *
     * @return true if an animation step was performed
     */
    bool effectSilly();

    /**
     * @brief Animation function
     *
     * Handles the animation logic for the eyes, including
     * interpolation between current and target positions/modes.
     */
    bool animate();
};

#endif // EYES_H
