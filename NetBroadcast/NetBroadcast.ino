#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "AA-616"
#define STAPSK "1234567890"
#endif

WiFiUDP udp;
IPAddress broadcastIP(255,255,255,255);
unsigned int localPort = 5000;


void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.printf("WiFi Connected! SSID: %s\n", STASSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendUDP(const char* message) {
    udp.beginPacket(broadcastIP, localPort);
    udp.write(message);
    udp.endPacket();
}

void loop() {
    sendUDP("Helloo???");
    Serial.println("Hello");
    delay(1000);
}