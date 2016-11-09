#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>
#include <AnalogKeypad.h>
#include <Servo.h>
#include <DFPlayer_Mini_Mp3.h>
#include "BookRelease.h"

const char* ssid = "AP001";
const char* password = "ItIsABadDay";

const uint8_t c_pinLedHighPower = D1; // using esp8266 board pin, AKA GPIO5, 
const uint8_t c_pinServoBookRelease = D5;

BookRelease bookRelease(c_pinServoBookRelease);

PandoraServer book;

void onBookCommand(const BookCommand& command)
{
  if (command.param == ButtonState_Click)
  {
    switch (command.command)
    {
      case ButtonId_1:
      // toggle High Power LED
      digitalWrite(c_pinLedHighPower, digitalRead(c_pinLedHighPower) == HIGH ? LOW : HIGH); // off
      break;

      case ButtonId_2:
      mp3_play(0);
      break;
      
      case ButtonId_3:
      mp3_play(1);
      break;

      case ButtonId_4:
      mp3_play(random(9));
      break;

      case ButtonId_5:
      bookRelease.trigger();
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

  // setup onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // off

  // setup external high power LED
  pinMode(c_pinLedHighPower, OUTPUT);
  digitalWrite(c_pinLedHighPower, LOW); // off

  bookRelease.begin();
    
  book.begin(ssid, password);

//  Serial1.println();
//  Serial1.println("=============================");
//  WiFi.printDiag(Serial1);
//  Serial1.println("=============================");
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
  bookRelease.loop();
}

