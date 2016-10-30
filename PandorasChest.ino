#include <ESP8266WiFi.h>
#include <PandoraCommunications.h>

const char* ssid = "AP001";
const char* password = "ItIsABadDay";

PandoraServer book;

void OnBookCommand(const BookCommand& command)
{
  Serial.print(command.command);
  Serial.print(" ");
  Serial.println(command.param);
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial attach
  Serial.println();

  book.begin(ssid, password);
}

void loop() {
  delay(random(10) + 5); // simulating other work
  
  book.loop(OnBookCommand);
}

