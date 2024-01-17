// 'THE WALL' [ESP8266 Message Board] (CAPTIVE)
//Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
//Simply connect to the Access Point that is created and navigate to http://wall.local OR http://192.168.4.1
// On Android Devices a login pop up should appear

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <DNSServer.h>

const char *ssid = "THE WALL: http://wall.local";
const char *password = "";

ESP8266WebServer server(80);
DNSServer dnsServer;
String messageBoard;
unsigned long startTime;

String getUptime() {
  unsigned long currentTime = millis();
  unsigned long uptimeInSeconds = (currentTime - startTime) / 1000;
  unsigned long days = uptimeInSeconds / 86400;
  uptimeInSeconds %= 86400;
  unsigned long hours = uptimeInSeconds / 3600;
  uptimeInSeconds %= 3600;
  unsigned long minutes = uptimeInSeconds / 60;
  unsigned long seconds = uptimeInSeconds % 60;
  return String(days) + "d " + String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
}

void handleRoot() {
  String html = "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head>";
  html += "<body style=\"background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAASwAAAEsCAMAAABOo35HAAAC0FBMVEVHcEz//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////v////3///62Q0cJAAAA7HRSTlMABQjzAgn8Af4DDwYM9/3p8vEECxr2FwdK+QoTDRY35FDtHeviI/QV2ercEDCJLvDv6PXhJcgO+idqIOw2Q1zneMzF1dA+XvhFqLLdZCuwGS1xETsbcOVdhVRElm/bnZtRuGBJ7h5WglIxLzzOnxJs+6EY3rN6OjPXvX7GIuBT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), linear-gradient(to bottom left, #fa711b, #e71bfa)\">";
  html += "<h1 style=\"border-radius: 10px; background-color: #404040; color: white; font-size: 36px;\"> THE WALL | Public Message Board</h1>";
  html += "<form id='messageForm' action='/post' method='post' style=\"border-radius: 10px; background-color: #404040; color: white;\">";
  html += "<input type='text' name='message' style=\"border-radius: 10px; font-size: 20px;\" placeholder='Enter your message' autofocus>";
  html += "<input type='submit' value='Post' style=\"border-radius: 10px; background-color: #00cc00; color: white; font-weight: bold; font-size: 30px;\">";
  html += "<p style=\"border-radius: 10px; background-color: #404040; color: white; font-weight: bold; font-size: 16px;\"> Uptime: " + getUptime() + "</p>";
  html += "</form>";
  html += "<h2 style=\"border-radius: 10px; background-color: #404040; color: white; font-size: 30px;\"> Recent Messages:</h2>";
  html += "<div id='messageBoard' style=\"background-color: #404040; color: white; font-size: 20px;\">" + messageBoard + "</div>";
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
  server.send(200, "text/html", html);
}

void handleMessages() {
  server.send(200, "text/plain", messageBoard);
}

void handlePost() {
  if (server.hasArg("message")) {
    String newMessage = server.arg("message");
    if (newMessage != "") {
      messageBoard = "<p>" + newMessage + "</p>" + messageBoard;
      saveMessagesToFile();
    }
  }
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleLogin() {
  String html = "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body style=\"background-color: #000000; background-image: url(data:image/png;base64,\">";
  html += "<h1 style=\"border-radius: 10px; background-color: #404040; color: white; font-size: 36px;\"> THE WALL | Public Message Board</h1>";
  html += "<p style=\"border-radius: 10px; background-color: #404040; color: white; font-weight: bold; font-size: 16px;\"> Uptime: " + getUptime() + "</p>";
  html += "<input allign='center' type='button' value='login' style=\"border-radius: 20px; background-color: #d62020; color: white; font-weight: bold; font-size: 64px;\" onclick=\"window.location.href = '/';\">";
  html += "<h3 style=\"border-radius: 10px; background-color: #404040; color: white; font-size: 20px;\"> If you lose this page - Go to: http://wall.local in your browser</h3>";
  html += "</form>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleNotFound() {
  // Redirect all requests to the captive portal
  server.sendHeader("Location", String("http://wall.local/login"), true);
  server.send(302, "text/plain", "");
}

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
  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_GET, handleLogin);
  server.on("/messages", HTTP_GET, handleMessages);
  server.on("/post", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);
  
  startTime = millis();

  // Start server
  server.begin();
  dnsServer.start(53, "*", WiFi.softAPIP());
  Serial.println("Server started");
}

void loop() {
  MDNS.update();
  dnsServer.processNextRequest();
  server.handleClient();
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
