#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

const char* PARAM_INPUT_1 = "username";
const char* PARAM_INPUT_2 = "ssid";
const char* PARAM_INPUT_3 = "pass";
const char* PARAM_INPUT_4 = "ip";
const char* PARAM_INPUT_5 = "gateway";

String username;
String ssid;
String pass;
String ip;
String gateway;

const char* usernamePath = "/settings/username.txt";
const char* ssidPath = "/settings/ssid.txt";
const char* passPath = "/settings/pass.txt";
const char* ipPath = "/settings/ip.txt";
const char* gatewayPath = "/settings/gateway.txt";

unsigned long previousMillis = 0;
const long interval = 10000;

bool editWiFi = true;

AsyncWebServer server(80);

IPAddress localIP;
IPAddress localGateway;

IPAddress subnet(255, 255, 0, 0);

void initSDCard(){
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

String readFile(const char* path){
  Serial.print("Reading file: ");
  Serial.println(path);
  
  File file = SD.open(path);
  if (!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while (file.available()){
    fileContent += file.read();
  }
  file.close();
  return fileContent;
}

void writeFile(const char* path, String message){
  Serial.print("Writing to file: ");
  Serial.println(path);
  
  File file = SD.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
    return;
  }
  Serial.println("- write failed");
}

void loadSettings(){
  username = readFile(usernamePath);
  ssid = readFile(ssidPath);
  pass = readFile(passPath);
  ip = readFile(ipPath);
  gateway = readFile(gatewayPath);
}

bool initWiFi() {
  if (ssid == "" || pass == "" || editWiFi) return false;
  WiFi.mode(WIFI_STA);
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());

  if (!WiFi.config(localIP, localGateway, subnet)){
    Serial.println("STA Failed to configure");
    return false;
  }
  
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Failed to connect.");
      return false;
    }
  }

  Serial.println(WiFi.localIP());
  return true;
}

void initAP(){
  Serial.println("Setting AP (Access Point)");
  WiFi.softAP("IRIS-WiFi", NULL);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void host_server(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/init/index.html", "text/html");
  });
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
    int params = request->params();
    for (int i = 0; i < params; i++){
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()){
        if (p->name() == PARAM_INPUT_1){
          username = p->value().c_str();
          Serial.print("Username set to: ");
          Serial.println(username);
          writeFile(usernamePath, username);
        }
        if (p->name() == PARAM_INPUT_2){
          ssid = p->value().c_str();
          Serial.print("SSID set to: ");
          Serial.println(ssid);
          writeFile(ssidPath, ssid);
        }
        if (p->name() == PARAM_INPUT_3){
          pass = p->value().c_str();
          Serial.print("Password set to: ");
          Serial.println(pass);
          writeFile(passPath, pass);
        }
        if (p->name() == PARAM_INPUT_4){
          ip = p->value().c_str();
          Serial.print("IP Address set to: ");
          Serial.println(ip);
          writeFile(ipPath, ip);
        }
        if (p->name() == PARAM_INPUT_5){
          gateway = p->value().c_str();
          Serial.print("Gateway set to: ");
          Serial.println(gateway);
          writeFile(gatewayPath, gateway);
        }
        request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + ip);
        delay(3000);
        ESP.restart();
      }
    }
  });
  server.onNotFound(notFound);

  server.serveStatic("/", SD, "/init/");

  server.begin();
}

void setup() {
  Serial.begin(115200);
  initSDCard();
  loadSettings();

  if (!initWiFi()) initAP();

  host_server();
}

void loop() {
  
}
