
// 'THE WALL' [ESP8266 Message Board]
//Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
//Simply connect to the Access Point that is created (SSID = 'The Wall') and navigate to http://thewall.local

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h> // Include the SPIFFS library

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
    html += "<input type='submit' value='Post' style=\"padding: 10px 20px; border-radius: 5px; background-color: #00cc00; color: white; font-weight: bold; font-size: 24px;\">";
    html += "</form>";
    html += "<h2 style=\"font-size: 36px;\">Recent Messages:</h2>";
    html += "<div id='messageBoard' style=\"padding: 10px 20px; border-radius: 5px; background-color: #404040; color: white; font-size: 24px;\">" + messageBoard + "</div>";
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
      saveMessagesToFile(); // Save messages to file
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

void loadMessagesFromFile() {
  // Load messages from file
  File file = SPIFFS.open("/messages.json", "r");
  if (!file) {
    Serial.println("Error opening messages file for reading");
    return;
  }

  // Read content from file
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

  // Write content to file
  file.print(messageBoard);
  file.close();
}
