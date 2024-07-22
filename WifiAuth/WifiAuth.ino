/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
// #define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "http://10.10.16.12/api/portal/v1/login"

#ifndef STASSID
#define STASSID "AA-616"
#define STAPSK "1234567890"
#endif

WiFiUDP udp;
IPAddress broadcastIP(255, 255, 255, 255);
unsigned int port = 5000;
int counter = 0;


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

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    // GET request from connection test server
    WiFiClient getClient;
    HTTPClient getHttp;
    Serial.println("[HTTP] start GET testing...");

    bool getSuccess = false;

    if (getHttp.begin(getClient, "http://httpbin.org/get")) {

      Serial.println("[HTTP] GET requesting...");
      int httpCode = getHttp.GET();

      if (httpCode > 0) {
        Serial.printf("[HTTP] GET ... httpCode: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          getSuccess = true;
          counter = counter + 1;
          sendUDP("[HTTP] GET succsess");
          Serial.printf("[HTTP] GET succsess. Count: %d\n", counter);
          delay(60000);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", getHttp.errorToString(httpCode).c_str());
      }
    } else {
      Serial.printf("[HTTP] Unable to connect http");
    }


    // POST request to auth page
    if (getSuccess == false) {

      WiFiClient client;
      HTTPClient http;

      sendUDP("[HTTP] start POST request...");
      Serial.println("[HTTP] start POST request...\n");
      // configure traged server and url
      http.begin(client, SERVER_IP);  // HTTP
      http.addHeader("Content-Type", "application/json");

      // start connection and send HTTP header and body
      int httpCode = http.POST("{\"domain\":\"cmcc\", \"username\": \"18888888888\",\"password\":\"888888\"}");

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST returns code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          const String& payload = http.getString();
          sendUDP("[HTTP] Campus network authenatic success");
          Serial.println("received payload:\n<<");
          Serial.println(payload);
          Serial.println(">>");
        }
      } else {
        sendUDP("[HTTP] POST... failed: auth failed");
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    }
  }

  delay(10000);
}

void sendUDP(const char* message) {
  udp.beginPacket(broadcastIP, port);
  udp.write(message);
  udp.endPacket();
}
