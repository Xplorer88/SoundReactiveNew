#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define READ_PIN 0
#define BUTTON_PIN 5

#define NUMBER_OF_CLIENTS 2

// IPAddress broadcast;
IPAddress multicastIP(230, 120, 10, 1);                  // Senden an alle Netzwerkteilnehmer mit dieser Multicastadresse.
constexpr uint16_t PORT = 8266; 

const char *ssid = "SoundReactive"; // The SSID (name) of the Wi-Fi network you want to connect to
const char *password = "123456789";  // The password of the Wi-Fi network

const int checkDelay = 5000;
const int buttonDoubleTapDelay = 200;
const int numOpModes = 3;

unsigned long lastChecked;
unsigned long buttonChecked;
bool buttonClicked = false;
bool queueDouble = false;
bool clickTrigger;
bool doubleTapped;
WiFiUDP UDP;

struct led_command {
  uint8_t opmode;
  uint32_t data;
};

static int opMode = 1;

void setup()
{
  pinMode(READ_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT );

  /* WiFi Part */
  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting soft-AP ... ");
  /* WiFi.persistent(false); */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  WiFi.begin(ssid, password);
  

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nVerbunden mit: %s\n\n", WiFi.SSID().c_str());

  UDP.beginMulticast(WiFi.localIP(), multicastIP, PORT);
  
  buttonChecked = 0;
}


void loop()
{
  uint32_t analogRaw;
  buttonCheck();

  switch (opMode) {
    case 1:
      analogRaw = analogRead(READ_PIN);
      if (analogRaw <= 3)
        break;
      sendLedData(analogRaw, opMode);
      break;
    case 2:
      sendLedData(0, opMode);
      delay(10);
      break;
    case 3:
      sendLedData(0, opMode);
      delay(10);
      break;
  }
  delay(4);
}

void sendLedData(uint32_t data, uint8_t op_mode) 
{
 struct led_command send_data;
 send_data.opmode = op_mode; 
 send_data.data = data; 

 UDP.beginPacketMulticast(multicastIP, PORT, WiFi.localIP());
 UDP.write((char*)&send_data,sizeof(struct led_command));
 UDP.endPacket();
}

void buttonCheck()
{
  int but = digitalRead(BUTTON_PIN);
  if (but == 0) {
    if (millis() - buttonChecked < buttonDoubleTapDelay && buttonClicked == false ) {
      doubleClicked();
      doubleTapped = true;
    }
    clickTrigger = true;
    buttonClicked = true; 
    buttonChecked = millis();
  }

  else if (but == 1) {
    if (millis() - buttonChecked > buttonDoubleTapDelay && clickTrigger) {
      if (!doubleTapped) {
        clicked();
      }
      clickTrigger = false;
      doubleTapped = false;
    }
    buttonClicked = false;
  }
}

void clicked() {
  if (opMode == numOpModes)
    opMode = 1;
  else
    opMode++;
  Serial.printf("Setting opmode %d \n", opMode);
}

void doubleClicked() {

}
