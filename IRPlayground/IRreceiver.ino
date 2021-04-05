#include <IRremote.h>
#include <CmdMessenger.h>

int MY_IR_SENDER_PIN = 8;
int IR_RECEIVE_PIN = 10;
int BUTTON_PIN = 2;

enum
{
  kAcknowledge,
  kError,
  kSwitch,
  kTest
};
//--------------------------------------------------------

// Attach a new CmdMessenger object to the default Serial port
CmdMessenger cmdMessenger = CmdMessenger(Serial);
IRsend mysender;

void OnSwitch() {
    Serial.println("Send IR symbol");
    mysender.sendSamsung(0x707, 0x2, 3);
    delay(1000);
    mysender.sendNECRaw(0xFF009D60, 0, false);
    delay(1000);
}
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

void setup()
{
  Serial.begin(115200);
  cmdMessenger.printLfCr();
  cmdMessenger.attach(OnUnknownCommand);
  cmdMessenger.attach(kTest, OnArduinoReady);
  cmdMessenger.attach(kSwitch, OnSwitch);
  cmdMessenger.sendCmd(kAcknowledge, "Arduino has started!");
  // recv = IRrecv(IR_RECEIVE_PIN);
  // recv.enableIRIn();
  // pinMode(4, OUTPUT);

  // IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver, enable feedback LED, take LED feedback pin from the internal boards definition
  // IrSender.begin(MY_IR_SENDER_PIN, false);

  mysender = IRsend();
  mysender.sendPin = MY_IR_SENDER_PIN;
  mysender.begin(MY_IR_SENDER_PIN, false);
  mysender.sendSamsung(0x707, 0x2, 1);
}

void loop() {
  delay(4000);
  Serial.println("Send IR symbol");
  mysender.sendSamsung(0x707, 0x2, 3);
  cmdMessenger.sendCmd(kAcknowledge, mysender.sendPin);
  delay(2000);
  Serial.println("Sending second IR symbol");
  mysender.sendNECRaw(0xFF009D60, 0, false);
  // digitalWrite(4, LOW);
  // delay(2000);
  // digitalWrite(4, HIGH);
  // delay(2000);
  // cmdMessenger.feedinSerialData();
  // if (IrReceiver.decode()) // have we received an IR signal?
  // {
  //   IrReceiver.printIRResultShort(&Serial);
  //   IrReceiver.resume(); // receive the next value
  // }
    // sender.sendSamsung(0x707, 0x2, 1);
}