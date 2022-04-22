// #include "ESP8266WiFi.h"
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"

AsyncWebServer server(80);

const char* ssid = "";
const char* password = "";

const char* PARAM_USERNAME = "username";

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE html>
    <html>
        <head>
            <title>IRIS Setup</title>
        </head>
        <body>
            <h1>IRIS Setup</h1>
            <p>Thank you for getting an IRIS!</p>
            <p>Set up your IRIS by filling in your info below.</p>

            <form action="/get">
                Username: <input type="text" name="username" />
                <input type="submit" value="Submit" />
            </form>

            <style>
                html {
                    font-family: Helvetica;
                    text-align: center;
                }
                body {
                background-color: darkblue; 
                color: white;
                }
            </style>
        </body>
    </html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void host_server() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });

    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        String inputMessage;
        String inputParam;
        if (request->hasParam(PARAM_USERNAME)) {
            inputMessage = request->getParam(PARAM_USERNAME)->value();
            inputParam = PARAM_USERNAME;
            Serial.printf("Username: %s\n", inputMessage.c_str());
            request->send(200, "text/plain", "Username: " + inputMessage);
        }
    });
    server.onNotFound(notFound);
    server.begin();
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("ip address: ");
    Serial.println(WiFi.localIP());
    host_server();
}

void loop() {
    
}