
// 'THE WALL' [ESP8266 Message Board]
//Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
//Simply connect to the Access Point that is created (SSID = 'The Wall') and navigate to http://wall.local

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

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
  if (!MDNS.begin("wall")) {
    Serial.println("Error setting up mDNS");
  } else {
    Serial.println("mDNS responder started");
  }

  // Print the mDNS address
  Serial.println("Open a browser and enter http://wall.local");

  if (!SPIFFS.begin()) {
    Serial.println("Error initializing SPIFFS");
    return;
  }

  // Load messages from file
  loadMessagesFromFile();

  // Serve HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body>";
    html += "<h1 style=\"font-size: 56px;\">THE WALL | Public Message Board</h1>";
    html += "<form id='messageForm' action='/post' method='post'>";
    html += "<input type='text' name='message' style=\"font-size: 30px;\" placeholder='Enter your message' autofocus>";
    html += "<input type='submit' value='Post' style=\"font-size: 30px;\">";
    html += "</form>";
    html += "<h2 style=\"font-size: 36px;\">Recent Messages:</h2>";
    html += "<div id='messageBoard' style=\"font-size: 30px;\">" + messageBoard + "</div>";
    html += "<script>function refreshMessages() {";
    html += "var xhr = new XMLHttpRequest();";
    html += "xhr.onreadystatechange = function() {";
    html += "if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "document.getElementById('messageBoard').innerHTML = xhr.responseText;";
    html += "}";
    html += "};";
    html += "xhr.open('GET', '/messages', true);";
    html += "xhr.send();";
    html += "} setInterval(refreshMessages, 5000); </script>";
    html += "</body></html>";
    request->send(200, "text/html", html);
  });
  
  // Serve messages separately for AJAX requests
  server.on("/messages", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", messageBoard);
  });
  
  // Handle message posting
  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    String message = request->arg("message");
    if(message != ""){
      messageBoard = "<p>" + message + "</p>" + messageBoard;
      saveMessagesToFile();
    }
    request->redirect("/");
  });
  // Start server
  server.begin();
}

void loop() {
  MDNS.update();
}

void loadMessagesFromFile() {
  // Load messages from file
  File file = SPIFFS.open("/messages.json", "r");
  if (!file) {
    Serial.println("Error opening messages file for reading");
    return;
  }
  messageBoard = file.readString();
  file.close();
}

void saveMessagesToFile() {
  // Save messages to file
  File file = SPIFFS.open("/messages.json", "w");
  if (!file) {
    Serial.println("Error opening messages file for writing");
    return;
  }
  file.print(messageBoard);
  file.close();
}
