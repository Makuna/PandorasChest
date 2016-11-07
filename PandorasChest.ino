#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>
#include <AnalogKeypad.h>
#include <Servo.h>
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
      // toggle built in LED
      digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) == HIGH ? LOW : HIGH); // off
      break;

      case ButtonId_2:
      // toggle High Power LED
      digitalWrite(c_pinLedHighPower, digitalRead(c_pinLedHighPower) == HIGH ? LOW : HIGH); // off
      break;
      
      case ButtonId_3:
      bookRelease.trigger();
      break;
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  while (!Serial); // wait for serial attach
  Serial.println();

  // setup onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // off

  // setup external high power LED
  pinMode(c_pinLedHighPower, OUTPUT);
  digitalWrite(c_pinLedHighPower, LOW); // off

  bookRelease.begin();
    
  book.begin(ssid, password);

  Serial.println("=============================");
  WiFi.printDiag(Serial);
  Serial.println("=============================");
}

void onServerStatusChange(const PandoraServerStatus& status)
{
  switch (status.state)
  {
  case PandoraServerState_ClientConnected:
    Serial.println("client connected");
    break;

  case PandoraServerState_CommandReadError:
    Serial.println("--- read command error ---");
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

