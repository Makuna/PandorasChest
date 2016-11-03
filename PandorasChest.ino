#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>
#include <AnalogKeypad.h>

const char* ssid = "AP001";
const char* password = "ItIsABadDay";

const uint8_t c_ledHighPower = D1; // using esp8266 board pin, AKA GPIO5, 

PandoraServer book;

void OnBookCommand(const BookCommand& command)
{
    
  Serial.print(command.command);
  Serial.print(" ");
  Serial.println(command.param);
  
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
      digitalWrite(c_ledHighPower, digitalRead(c_ledHighPower) == HIGH ? LOW : HIGH); // off
      
    }
  }

}

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial attach
  Serial.println();

  // setup onboard LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // off

  // setup external high power LED
  pinMode(c_ledHighPower, OUTPUT);
  digitalWrite(c_ledHighPower, LOW); // off
  
  book.begin(ssid, password);
}

void loop() {
  delay(random(10) + 5); // simulating other work
  
  book.loop(OnBookCommand);
}

