#include "WiFinder.h"

String urlDecoder(String url) {
  String decodedString = "";
  int charCounter = 0;
  while(charCounter < url.length()) {
    if (url[charCounter] != '%') {
      decodedString += url[charCounter];
      charCounter += 1;
    } else {
      char newChar = (int(url[charCounter+1]) << 4) & (int(url[charCounter+2]));
      decodedString += newChar;
      charCounter += 3;
    }
  }
}

/*
 * Return codes:
 *    0 - Known network found
 *    1 - Networks found
 *    2 - No networks found
 */
int WiFinder::begin(String known_ssid, String known_pass, String guest_ssid) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.println("Scanning for WiFi networks");
  int num_of_networks = WiFi.scanNetworks();
  
  // Check that there are networks
  if (num_of_networks == 0) {
    Serial.println("No networks found, exiting");
    return 2;
  }

  // See if we know the password to any
  Serial.print("Found "); Serial.print(num_of_networks); Serial.println(" networks");
  for (int i = 0; i < num_of_networks; i++) {
    SSIDs[i].ssid = WiFi.SSID(i);
    SSIDs[i].encrypted = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);

    if (SSIDs[i].ssid == known_ssid) {
      Serial.print("Found recognised network: "); Serial.println(known_ssid);
      ssid = known_ssid;
      password = known_pass;
      return 0;
    }
  }

  // Need to ask user for password
  Serial.println("No recognised networks found, starting SoftAP");
  WiFi.mode(WIFI_AP);
  WiFiServer server(80);
  IPAddress apIP(192,168,0,1);
  DNSServer dns;
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0));
  WiFi.softAP(guest_ssid.c_str());
  dns.start(byte(53), "*", apIP);
  server.begin();

  Serial.println("SoftAP started, waiting for requests");

  String serverHTML = "<!DOCTYPE html><html><head><title>";
  serverHTML += guest_ssid;
  serverHTML += "</title></head><body><h1>Please choose the WiFi network you want to connect to:</h1>";
  serverHTML += "<form method='post'><label for='ssid'>SSID:</label><select name='ssid' id='ssid'>";

  for (int i = 0; i < num_of_networks; i++) {
    serverHTML += "<option value='";
    serverHTML += SSIDs[i].ssid;
    serverHTML += "'>";
    serverHTML += SSIDs[i].ssid;
    serverHTML += "</option>";
  }

  serverHTML += "</select><label for='password'>Password:</label><input type='text' name='password' id='password' />";
  serverHTML += "<input type='submit' value='Submit' /></form></body></html>";

  bool needPassword = true;
  while(needPassword) {
    dns.processNextRequest();
    WiFiClient client = server.available();    
    if (client) {
      Serial.println("New client connected");
      while (client.connected()) {
        if (client.available()) {
            String request = client.readString().trim();
            Serial.println("Request recieved:\n------ Request Body Start -----");
            Serial.println(request);
            Serial.println("------ Request Body End -----");
            String reqType = request.substring(0,4);
            Serial.print("Request type: ");
            Serial.println(reqType);
            if (reqType == "GET ") {
              Serial.println("Get request, sending wifi choice page");
              client.print("HTTP/1.1 200 OK\nContent-type:text/html\n\n");
              client.print(serverHTML);
              Serial.println("Sent");
            } else if (reqType == "POST") {
              Serial.println("Post request");
              String reqArgs = request.substring(request.lastIndexOf('\n')+1)
              
            }
          
//          String request = client.readStringUntil(' '); // will either contain POST or GET
//          if (request == "GET") {
//            client.print("HTTP/1.1 200 OK\nContent-type:text/html\n\n");
//            client.print(serverHTML);
//          } else if (request == "POST") {
//            client.readStringUntil('='); // This will discard everything up to the SSID
//            ssid = client.readStringUntil('&');
//            client.readStringUntil('='); // discards the text "password"
//            password = urlDecoder(client.readStringUntil('\n'));
//            needPassword = false;
//          }
        }
      }
      client.stop();
    }
  }
  WiFi.mode(WIFI_STA);
}

const char* WiFinder::get_ssid() {
  return ssid.c_str();
};
const char* WiFinder::get_password() {
  return password.c_str();
};
