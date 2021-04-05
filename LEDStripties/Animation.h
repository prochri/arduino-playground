#include <Adafruit_NeoPixel.h>
#define COLOR_OFF 0

class Animation {
public:
    uint64_t currentStep;
    unsigned long delay;
    Adafruit_NeoPixel& pixels;
public:
    Animation(Adafruit_NeoPixel& pixels) : currentStep(0), delay(500), pixels(pixels) {}
    virtual void step() = 0;
    virtual void restart() = 0;
    virtual uint32_t updateDelay() { return delay; };
};

class ColorWipeAnimation : public Animation {
private:
    uint32_t color;
public:
    ColorWipeAnimation(Adafruit_NeoPixel& pixels, uint32_t color = 255) : Animation(pixels), color(color) {}
    void step() override;
    void restart() override;
};

class RainbowAnimation : public Animation {
public:
    RainbowAnimation(Adafruit_NeoPixel& pixels) : Animation(pixels) {}
    void step() override;
    void restart() override;
    uint32_t updateDelay() override;
};