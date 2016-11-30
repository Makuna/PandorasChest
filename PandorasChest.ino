#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>
#include <AnalogKeypad.h>
#include <Servo.h>
#include <DFPlayer_Mini_Mp3.h>
#include <NeoPixelBus.h>
#include "BookRelease.h"

const char* ssid = "AP001";
const char* password = "ItIsABadDay";

// D1 Mini Pins
// D0 / GPIO16 
// D1 / GPIO5         = AC Relay 1
// D2 / GPIO4         = AC Relay 2
// D3 / GPIO0         = (AC Relay 3/Book Release?)
// D4 / GPIO2         = (AC Realy 4/Book Release?)
// D5 / GPIO14        = 3W LED 1
// D6 / GPIO12        = 3W LED 2
// D7 / GPIO13 (RXD2) = MP3
// D8 / GPIO15 (TXD2) = MP3
// RX / GPIO3         = NeoPixelStrip
// TX / GPIO1

const uint8_t c_pin3wLed1 = D5;
const uint8_t c_pin3wLed2 = D6;
const uint8_t c_pinServoBookRelease = D4;

const uint8_t c_pinAcSw1 = D1; 
const uint8_t c_pinAcSw2 = D2; 
const uint8_t c_pinAcSw3 = D3; 
const uint8_t c_pinAcSw4 = D4; 

// BookRelease bookRelease(c_pinServoBookRelease);
NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> strip(17); // GPIO3 (RXD0)

PandoraServer book;

void toggleAcSwitch(uint8_t pin)
{
  // toggle Ac Switch
  if (digitalRead(pin) == HIGH)
  {
    digitalWrite(pin, LOW); // off
  }
  else
  {
    digitalWrite(pin, HIGH); // on
  }
}

void onBookCommand(const BookCommand& command)
{
  if (command.param == ButtonState_Click)
  {
    switch (command.command)
    {
      case ButtonId_1:
      // toggle High Power LED
      digitalWrite(c_pin3wLed1, digitalRead(c_pin3wLed1) == HIGH ? LOW : HIGH); // off
      break;

      case ButtonId_2:
      toggleAcSwitch(c_pinAcSw1);
      mp3_play(0);
      break;
      
      case ButtonId_3:
      toggleAcSwitch(c_pinAcSw2);
      mp3_play(1);
      break;

      case ButtonId_4:
      toggleAcSwitch(c_pinAcSw3);
      mp3_play(random(9));
      break;

      case ButtonId_5:
      toggleAcSwitch(c_pinAcSw4);
//      bookRelease.trigger();
      break;
    }
  }
}

void setup() 
{
  // use Serial for MP3 playback
  // move serial to GPIO15(TX) and GPIO13 (RX) for
  // the mp3 module, we loose the debug monitor though
  Serial.begin(9600);
  Serial.swap();
  
  mp3_set_serial(Serial);
  mp3_set_volume(15);

  strip.Begin();
  strip.Show();
  
  // setup onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // off

  // setup external high power LED
  pinMode(c_pin3wLed1, OUTPUT);
  digitalWrite(c_pin3wLed1, LOW); // off

  // setup external high power LED
  pinMode(c_pin3wLed2, OUTPUT);
  digitalWrite(c_pin3wLed2, LOW); // off
  
//  bookRelease.begin();
    
  book.begin(ssid, password);

//  Serial1.println();
//  Serial1.println("=============================");
//  WiFi.printDiag(Serial1);
//  Serial1.println("=============================");


  // setup AC Relay Switches
  pinMode(c_pinAcSw1, OUTPUT);
  digitalWrite(c_pinAcSw1, LOW); // off
  pinMode(c_pinAcSw2, OUTPUT);
  digitalWrite(c_pinAcSw2, LOW); // off
  pinMode(c_pinAcSw3, OUTPUT);
  digitalWrite(c_pinAcSw3, LOW); // off
  pinMode(c_pinAcSw4, OUTPUT);
  digitalWrite(c_pinAcSw4, LOW); // off
  
}

void onServerStatusChange(const PandoraServerStatus& status)
{
  switch (status.state)
  {
  case PandoraServerState_ClientConnected:
    // Serial1.println("client connected");
    break;

  case PandoraServerState_CommandReadError:
    // Serial1.println("--- read command error ---");
    break;
  }
}

void loop() 
{
  // simulating other work, remove for final
  delay(random(10) + 5); 
  
  book.loop(onBookCommand, onServerStatusChange);
//  bookRelease.loop();

  // candle simulation testing
  //
  uint8_t level = random(60) + 220;
  RgbwColor color = RgbwColor(20, 20, 0, level);
  RgbwColor color1 = RgbwColor(level - 50, level - 50, 0, 0);
  RgbwColor color2 = RgbwColor(level - 100, 0, 0, 0);
  
//  strip.ClearTo(color, 0, 16);
//  strip.SetPixelColor(1, color);
//  strip.SetPixelColor(3, color);
//  strip.SetPixelColor(5, color);
//  strip.SetPixelColor(7, color);
//  strip.SetPixelColor(9, color);
  //strip.SetPixelColor(12, color2);
  strip.SetPixelColor(14, color);
  strip.SetPixelColor(16, color1);
  
  strip.Show();
}

