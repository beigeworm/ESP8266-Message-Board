
// 'THE WALL' [ESP8266 Message Board]
//Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
//Simply connect to the Access Point that is created (SSID = 'The Wall') and navigate to http://thewall.local

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>

const char *ssid = "The Wall";
const char *password = "";

AsyncWebServer server(80);

String messageBoard;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Configure mDNS
  if (!MDNS.begin("thewall")) {
    Serial.println("Error setting up mDNS");
  } else {
    Serial.println("mDNS responder started");
  }

  // Print the mDNS address
  Serial.println("Open a browser and enter http://thewall.local");

  // Serve HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body>";
    html += "<div width=100% margin=0 style=\"font-size: 46px;\" align=center><h1 style=\"font-size: 56px;\">Message Board</h1>";
    html += "<form action='/post' method='post'>";
    html += "<input type='text' name='message' style=\"font-size: 46px;\" placeholder='Enter your message'>";
    html += "<input type='submit' value='Post' style=\"font-size: 46px;\">";
    html += "</form>";
    html += "<h2 style=\"font-size: 56px;\">Messages:</h2>";
    html += "<div style=\"font-size: 46px;\">" + messageBoard + "</div>";
    html += "</div></body></html>";
    request->send(200, "text/html", html);
  });

  // Handle message posting
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    String message = request->arg("message");
    if(message != ""){
      messageBoard += "<p>" + message + "</p>";
    }
    request->redirect("/");
  });

  // Start server
  server.begin();
}

void loop() {
  MDNS.update();
  // Other loop tasks, if any
}
