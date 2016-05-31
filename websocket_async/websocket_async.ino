

/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <WebSocketsServer.h>
#include <Hash.h>
#include <Metro.h> // Include the Metro library
#include <AsyncPrinter.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncTCPbuffer.h>
#include <SyncClient.h>
Metro callBack = Metro(25);  // Instantiate an instanc
Metro printData = Metro(7000);
/* Set these to your desired credentials. */
const char *ssid = "ESPap2";
const char *password = "123123123123";
WebSocketsServer webSocket = WebSocketsServer(81);
unsigned long lastTimeRefresh = 0;
int received =  0;
int powerPin = 15; // analog



void sendMessage(uint8_t num) {
  webSocket.sendTXT(num, "received some stuff dude");
}

void pushPower(byte value) {
   analogWrite(powerPin, value);
}
// WebSOcket Events 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
      }
      break;
      
    case WStype_TEXT:
      { 
        String value = String((char *) &payload[0]);
        String mem = String(ESP.getFreeHeap());
        byte power = (byte)value.toInt();
        Serial.println(power);
        pushPower(power);
        //webSocket.sendTXT(num, "RAM: " + mem + "i = " + text + " ESPreceived = " + String(received) );
       
      }
      break;

    case WStype_BIN:
      hexdump(payload, length);
      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
  }
}


void setup() {
  Serial.begin(115200);
//  Serial.println();
//  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
//  Serial.print("Configuring access point...");
  IPAddress address(10, 10, 100, 254);
  IPAddress subnet(255, 255, 255, 0);
  byte channel = 11;
  float wifiOutputPower = 20.5; //Max power
  WiFi.setOutputPower(wifiOutputPower);
  WiFi.setPhyMode(WIFI_PHY_MODE_11B);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);

  WiFi.persistent(false);
  WiFi.softAPConfig(address, address, subnet);
  WiFi.softAP(ssid, password, channel);
  IPAddress myIP = WiFi.softAPIP();

  analogWriteRange(100); // CHANGE PWM RANGE from 1023 to 100.

//  Serial.print("AP IP address: ");
//  Serial.println(myIP);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {

   

}
