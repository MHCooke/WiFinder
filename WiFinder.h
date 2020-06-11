#ifndef WiFinder_H_
#define WiFinder_H_

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <WiFiAP.h>
#include <Arduino.h>
#include <DNSServer.h>

struct SSIDList {
  String ssid;
  bool encrypted;
};

String urlDecoder(String url);

class WiFinder {
  private:
    String ssid;
    String password;
    SSIDList SSIDs[10];
  
  public:
    int begin(String known_ssid = "", String known_pass = "", String guest_ssid = "ESP_Authentication");
    const char* get_ssid();
    const char* get_password();
};

#endif /* WiFinder included */
