#include "Animation.h"
//

//--------------------------------------------------
void ColorWipeAnimation::step()
{
    if (currentStep == pixels.numPixels())
    {
        restart();
    }
    else
    {
        pixels.setPixelColor(currentStep++, color);
    }
}
void ColorWipeAnimation::restart()
{
    currentStep = 0;
    pixels.fill(COLOR_OFF);
    // for (uint16_t i = 0; i < pixels.numPixels(); i++)
    // {
    //     pixels.setPixelColor(i, COLOR_OFF);
    // }
}
//--------------------------------------------------
void RainbowAnimation::step() {
    if(currentStep == 1280) {
        restart();
    }
    else {
        pixels.fill(pixels.gamma32(pixels.ColorHSV(currentStep * 256)));
        currentStep++;
    }
}

void RainbowAnimation::restart() {
    currentStep = 0;
    pixels.fill(pixels.gamma32(pixels.ColorHSV(0)));
}

uint32_t RainbowAnimation::updateDelay()
{
    return 50;
}