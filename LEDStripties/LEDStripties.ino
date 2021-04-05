#define PIN 33
#define NUMPIXELS 5
#include <Adafruit_NeoPixel.h>
#include <CmdMessenger.h>
#include "Animation.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 50


enum
{
  kAcknowledge,
  kError,
  kSetLED,
  kSetAnimation,
  kTest
};
//--------------------------------------------------------

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger(Serial);
ColorWipeAnimation cwa(pixels);
RainbowAnimation rba(pixels);
Animation *animation = nullptr;
bool pixelUpdate = false;
uint64_t lastUpdateTime;

// ------------------  C A L L B A C K S -----------------------

// Called when a received command has no attached function
void OnUnknownCommand()
{
  cmdMessenger.sendCmd(kError, "Command without attached callback");
}

// Callback function that responds that Arduino is ready (has booted up)
void OnArduinoReady()
{
  cmdMessenger.sendCmd(kAcknowledge, "Arduino ready");
}

void OnSetLED()
{
  uint8_t r, g, b, w;
  uint16_t ledID = cmdMessenger.readInt16Arg();
  r = cmdMessenger.readInt16Arg();
  g = cmdMessenger.readInt16Arg();
  b = cmdMessenger.readInt16Arg();
  if (cmdMessenger.available())
  {
    w = cmdMessenger.readInt16Arg();
  }
  pixels.setPixelColor(ledID, pixels.Color(r, g, b, w));
  animation = nullptr;
  pixelUpdate = true;
}

void OnSetAnimation()
{
  cmdMessenger.sendCmd(kAcknowledge, "Changing");
  char *s = cmdMessenger.readStringArg();
  cmdMessenger.sendCmd(kAcknowledge, s);
  if (strncmp(s, "colorWipe", 20) == 0)
  {
    animation = &cwa;
  }
  else if (strncmp(s, "rainbow", 20) == 0)
  {
    animation = &rba;
  }
  else
  {
    cmdMessenger.sendCmd(kAcknowledge, "c");
    animation = nullptr;
    pixels.fill(COLOR_OFF);
  }
  if(animation) animation->restart();
  pixelUpdate = true;
}

//--------------------------------------------------------
// setup
void CmdMessengerSetup()
{
  cmdMessenger.printLfCr();
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kTest, OnArduinoReady);
  cmdMessenger.attach(kSetLED, OnSetLED);
  cmdMessenger.attach(kSetAnimation, OnSetAnimation);
  cmdMessenger.sendCmd(kAcknowledge, "Arduino has started!");
}

void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(115200);
  CmdMessengerSetup();
  pixels.begin();
  pixels.clear();
  pixels.show();
  if(animation)
    animation->restart();
}

// ------------ main loop and functions ---------------
// Returns if it has been more than interval (in ms) ago. Used for periodic actions
bool hasExpired(unsigned long interval)
{
  return millis() - lastUpdateTime > interval;
}

void loop()
{
  cmdMessenger.feedinSerialData();
  if (pixelUpdate)
  { // TODO maybe add timer
    pixels.show();
    pixelUpdate = false;
    lastUpdateTime = millis();
  }
  else if (animation && hasExpired(animation->updateDelay()))
  {
    animation->step();
    pixels.show();
    lastUpdateTime = millis();
  }
}