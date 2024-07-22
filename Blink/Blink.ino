// the setup function runs once when you press reset or power the board
#include <ESP8266WiFi.h>
const char* ssid = "AA-616";
const char* password = "1234567890";


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(115200);
  // delay(100);

  // WiFi.begin(ssid,password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("COnnecting to WiFi...");
  //   assert(WiFi.status() == WL_CONNECTED);
  // }

  // Serial.println("Connected to the WiFinetword");
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
