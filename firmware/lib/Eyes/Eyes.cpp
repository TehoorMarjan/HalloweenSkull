#include "Eyes.h"

/**
 * @brief Construct a new Eyes object
 *
 * Initializes the MD_MAX72XX object and sets up default values for
 * iris positions and modes.
 */
Eyes::Eyes(uint8_t hardwareType, uint8_t dataPin, uint8_t clkPin, uint8_t csPin)
    : mx((MD_MAX72XX::moduleType_t)hardwareType, dataPin, clkPin, csPin, MAX_DEVICES)
{
    // Initialize current and target positions to (3, 0) for testing orientation
    currentLeft.x = 3;
    currentLeft.y = 3;
    targetLeft.x = 3;
    targetLeft.y = 3;

    currentRight.x = 3;
    currentRight.y = 3;
    targetRight.x = 3;
    targetRight.y = 3;

    // Initialize modes
    currentMode = NORMAL;
    targetMode = NORMAL;

    // Initialize effect step counter
    step = 0;
    lastAnimationStepTimeNormal = 0;

    // Initialize eye buffers (all LEDs OFF initially)
    for (uint8_t i = 0; i < 8; i++)
    {
        leftEyeBuffer[i] = 0x00;
        rightEyeBuffer[i] = 0x00;
    }
}

/**
 * @brief Initialize the Eyes display
 * Must be called in setup() before using other methods
 */
void Eyes::begin()
{
    // Initialize the display
    mx.begin();
    mx.control(MD_MAX72XX::INTENSITY, DEFAULT_BRIGHTNESS);
};

/**
 * @brief Check if an animation is in progress
 *
 * @return true if animating, false if idle
 */
bool Eyes::isAnimating()
{
    return (currentLeft.x != targetLeft.x) ||
           (currentLeft.y != targetLeft.y) ||
           (currentRight.x != targetRight.x) ||
           (currentRight.y != targetRight.y) ||
           (currentMode != targetMode);
}

/**
 * @brief Set the display brightness
 *
 * @param brightness Brightness level (0-15)
 */
void Eyes::setBrightness(uint8_t brightness)
{
    brightness = constrain(brightness, 0, 15);
    mx.control(MD_MAX72XX::INTENSITY, brightness);
};

/**
 * @brief Set target position for both irises (synchronized movement)
 *
 * @param x Target x-coordinate (0-6)
 * @param y Target y-coordinate (0-6)
 */
void Eyes::requestPosition(uint8_t x, uint8_t y)
{
    requestPosition(x, y, x, y);
};

/**
 * @brief Set target positions for left and right irises independently
 *
 * @param xl Target x-coordinate for left iris (0-6)
 * @param yl Target y-coordinate for left iris (0-6)
 * @param xr Target x-coordinate for right iris (0-6)
 * @param yr Target y-coordinate for right iris (0-6)
 */
void Eyes::requestPosition(uint8_t xl, uint8_t yl, uint8_t xr, uint8_t yr)
{
    targetLeft.x = constrain(xl, 0, 6);
    targetLeft.y = constrain(yl, 0, 6);
    targetRight.x = constrain(xr, 0, 6);
    targetRight.y = constrain(yr, 0, 6);
};

/**
 * @brief Set position for both irises, no interpolation (synchronized movement)
 *
 * Note: you still need to call update() to refresh the display
 *
 * @param x Target x-coordinate (0-6)
 * @param y Target y-coordinate (0-6)
 */
void Eyes::immediatePosition(uint8_t x, uint8_t y)
{
    immediatePosition(x, y, x, y);
}

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
void Eyes::immediatePosition(uint8_t xl, uint8_t yl, uint8_t xr, uint8_t yr)
{
    currentLeft.x = constrain(xl, 0, 6);
    currentLeft.y = constrain(yl, 0, 6);
    currentRight.x = constrain(xr, 0, 6);
    currentRight.y = constrain(yr, 0, 6);
    targetLeft.x = constrain(xl, 0, 6);
    targetLeft.y = constrain(yl, 0, 6);
    targetRight.x = constrain(xr, 0, 6);
    targetRight.y = constrain(yr, 0, 6);
}

/**
 * @brief Set the target eye mode
 *
 * @param mode Target mode (NORMAL, CLOSED, CROSS, SILLY)
 *
 * @return true if the mode change request was accepted
 */
bool Eyes::requestMode(EyeMode mode)
{
    if (targetMode != currentMode)
    {
        return false; // Reject new mode request if an animation is in progress
    }
    targetMode = mode;
    step = 0; // Reset effect step counter
    return true;
}

/**
 * @brief Set the eye mode immediately, no animation
 *
 * @param mode Target mode (NORMAL, CLOSED, CROSS, SILLY)
 */
void Eyes::immediateMode(EyeMode mode)
{
    currentMode = mode;
    targetMode = mode;
    step = 0; // Reset effect step counter
}

/**
 * @brief Send the internal buffers to the physical displays
 *
 * Transfers the leftEyeBuffer and rightEyeBuffer to the corresponding
 * MAX7219 devices. Device 0 is the right eye, Device 1 is the left eye
 * (due to daisy-chaining order).
 */
void Eyes::send()
{
    // Disable display updates while we update all rows
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);

    // Send right eye buffer to device 0
    for (uint8_t row = 0; row < 8; row++)
    {
        mx.setRow(0, row, rightEyeBuffer[row]);
    }

    // Send left eye buffer to device 1
    for (uint8_t row = 0; row < 8; row++)
    {
        mx.setRow(1, row, leftEyeBuffer[row]);
    }

    // Re-enable display updates
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

/**
 * @brief Update the display (temporary implementation)
 *
 * For now, just calls send() to refresh the display.
 * Will later handle interpolation and animations.
 */
void Eyes::update()
{
    animate();
    send();
}

/**
 * @brief Generate eye patterns with irises at target positions
 *
 * Creates the internal buffer representation of both eyes with the irises
 * (2x2 squares of OFF LEDs) at the positions specified by targetLeft and
 * targetRight. The rest of the LEDs are ON to represent the white of the eye.
 */
void Eyes::makeEyes()
{
    // Start with the eyes:
    // ..XXXXX..
    // .XXXXXXX.
    // XXXXXXXXX
    // XXXXXXXXX
    // XXXXXXXXX
    // .XXXXXXX.
    // ..XXXXX..
    for (uint8_t i = 0; i < 8; i++)
    {
        if (i == 0 || i == 7)
        {
            leftEyeBuffer[i] = 0x3C;  // 00111100
            rightEyeBuffer[i] = 0x3C; // 00111100
        }
        else if (i == 1 || i == 6)
        {
            leftEyeBuffer[i] = 0x7E;  // 01111110
            rightEyeBuffer[i] = 0x7E; // 01111110
        }
        else
        {
            leftEyeBuffer[i] = 0xFF;
            rightEyeBuffer[i] = 0xFF;
        }
    }

    // Create 2x2 iris (OFF pixels) for left eye
    // The iris occupies rows [leftY, leftY+1] and columns [leftX, leftX+1]
    for (uint8_t row = currentLeft.x; row < currentLeft.x + 2; row++)
    {
        if (row < 8)
        {
            // Clear the two bits representing the iris columns
            leftEyeBuffer[row] &= ~(0x03 << currentLeft.y);
        }
    }

    // Create 2x2 iris (OFF pixels) for right eye
    // The iris occupies rows [rightY, rightY+1] and columns [rightX, rightX+1]
    for (uint8_t row = currentRight.x; row < currentRight.x + 2; row++)
    {
        if (row < 8)
        {
            // Clear the two bits representing the iris columns
            rightEyeBuffer[row] &= ~(0x03 << currentRight.y);
        }
    }
}

/**
 * @brief Animation function
 *
 * Handles the animation logic for the eyes, including
 * interpolation between current and target positions/modes.
 */
bool Eyes::animate()
{
    // Closed handles both closing and opening
    if (currentMode == CLOSED || targetMode == CLOSED)
    {
        return effectClosed();
    }

    // Default
    return effectNormal();
}

bool Eyes::effectNormal()
{
    unsigned long now = millis();

    if (now - lastAnimationStepTimeNormal < ANIMATION_NORMAL_DELAY)
    {
        return false; // Not enough time has passed
    }

    // Animate the move of the irises to the target positions
    if (currentLeft.x < targetLeft.x)
        currentLeft.x++;
    if (currentLeft.x > targetLeft.x)
        currentLeft.x--;
    if (currentLeft.y < targetLeft.y)
        currentLeft.y++;
    if (currentLeft.y > targetLeft.y)
        currentLeft.y--;

    if (currentRight.x < targetRight.x)
        currentRight.x++;
    if (currentRight.x > targetRight.x)
        currentRight.x--;
    if (currentRight.y < targetRight.y)
        currentRight.y++;
    if (currentRight.y > targetRight.y)
        currentRight.y--;

    // Update the display with the new iris positions
    makeEyes();

    lastAnimationStepTimeNormal = now;

    return true;
}

/**
 * @brief Closed eyes effect
 *
 * Animates the eyes closing (all LEDs off or partial closure)
 */
bool Eyes::effectClosed()
{
    bool normalAnimated;
    bool delayElapsed = true;
    unsigned long now = millis();

    if (step == 0)
    {
        lastAnimationStepTimeClosed = now;
        step = 1; // Start from step 1
    }

    normalAnimated = effectNormal();

    if (now - lastAnimationStepTimeClosed < ANIMATION_CLOSED_DELAY)
    {
        delayElapsed = false;
    }

    if (delayElapsed)
    {
        // Enough time has passed, close/open more
        if (step < 3)
        {
            step += 1;
        }
        else
        {
            // Animation complete
            currentMode = targetMode;
        }

        if (!normalAnimated)
        {
            // If normal didn't animate (irises already at target), need to redraw eyes
            makeEyes();
        }

        lastAnimationStepTimeClosed = now;
    }

    if (delayElapsed || normalAnimated)
    {
        // Draw the eyelid effect
        // Note: when called, step is between 1 and 4

        // Normal has redrawn the eyes, need to update the lids
        if (targetMode == CLOSED && currentMode != CLOSED)
        {
            // Closing animation step
            // Gradually turn off columns from top and bottom towards center
            // Because displays are tilted, so the algorithm needs to create "curtains"
            uint8_t mask = (0xFF >> (step << 1)) << step;
            for (uint8_t i = 0; i < 8; i++)
            {
                leftEyeBuffer[i] &= mask;
                rightEyeBuffer[i] &= mask;
            }
        }
        else if (targetMode != CLOSED && currentMode == CLOSED)
        {
            // Opening animation step
            // Gradually not turn off columns from top and bottom towards center
            // Because displays are tilted, so the algorithm needs to create "curtains"
            uint8_t invstep = 4 - step;
            uint8_t mask = (0xFF >> (invstep << 1)) << invstep;
            for (uint8_t i = 0; i < 8; i++)
            {
                leftEyeBuffer[i] &= mask;
                rightEyeBuffer[i] &= mask;
            }
        }
        else if (targetMode == CLOSED && currentMode == CLOSED)
        {
            // Fully closed
            for (uint8_t i = 0; i < 8; i++)
            {
                leftEyeBuffer[i] = 0x00;
                rightEyeBuffer[i] = 0x00;
            }
        }
        else
        {
            // Fully open, keep eye "intact"
        }
    }

    return delayElapsed || normalAnimated;
}

/**
 * @brief Cross-eyed effect
 *
 * Animates both irises moving toward the center (cross-eyed look)
 */
bool Eyes::effectCross()
{
    return false;
}

/**
 * @brief Silly eyes effect
 *
 * Creates a silly/goofy eye animation
 */
bool Eyes::effectSilly()
{
    return false;
}
