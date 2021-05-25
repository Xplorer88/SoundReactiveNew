#include <ESP8266WiFi.h>
void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("Konfiguriere soft-AP ... ");
  boolean result = WiFi.softAP("SoundReactive", "123456789");
  
  Serial.print("Verbindung wurde ");
  if(result == false){
    Serial.println("NICHT ");
  }
  Serial.print("erfolgreich aufgebaut!");
}
void loop(){
  Serial.printf("Anzahl der Verbundenen Geräte= %d\n", WiFi.softAPgetStationNum());
  delay(3000);
}
