// 'THE WALL' [ESP8266 Message Board]
//Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
//Simply connect to the Access Point that is created (SSID = 'The Wall') and navigate to http://wall.local OR http://192.168.4.1

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
    String html = "<html><body style=\"background-color: #bfbfbf; background-image: url(data:image/png;base64,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\">";
    html += "<h1 style=\"font-size: 36px;\">THE WALL | Public Message Board</h1>";
    html += "<form id='messageForm' action='/post' method='post'>";
    html += "<input type='text' name='message' style=\"font-size: 24px;\" placeholder='Enter your message' autofocus>";
    html += "<input type='submit' value='Post' style=\"border-radius: 5px; background-color: #00cc00; color: white; font-weight: bold; font-size: 24px;\">";
    html += "</form>";
    html += "<h2 style=\"font-size: 36px;\">Recent Messages:</h2>";
    html += "<div id='messageBoard' style=\"background-color: #404040; color: white; font-size: 24px;\">" + messageBoard + "</div>";
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
