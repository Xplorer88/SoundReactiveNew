#include <FastLED.h>
#include <WiFi.h>
#include "AsyncUDP.h"

#define READ_PIN 34

// IPAddress broadcast;
IPAddress multicastIP(239, 255, 255, 250);                  // Senden an alle Netzwerkteilnehmer mit dieser Multicastadresse.
constexpr uint16_t PORT = 8266; 

const char *ssid = "SoundReactive"; // The SSID (name) of the Wi-Fi network you want to connect to
const char *password = "123456789";  // The password of the Wi-Fi network

const int checkDelay = 250;

int led_data;
int testCounter = 0;

AsyncUDP UDP;


void setup()
{
  
  /* WiFi Part */
  Serial.begin(115200);
  Serial.println();
  /* WiFi.persistent(false); */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nVerbunden mit: %s\n\n", WiFi.SSID().c_str());

  if (UDP.connect(multicastIP, PORT))
  {
    Serial.println("UDP connected");
  }

}


void loop()
{
  
  led_data = analogRead(READ_PIN);
  Serial.print(led_data);
  Serial.println(" dB");
  
  UDP.print(led_data);
  //UDP.writeTo((uint8_t *)(&testCounter), sizeof(testCounter), multicastIP, PORT);

  delay(checkDelay);
}
