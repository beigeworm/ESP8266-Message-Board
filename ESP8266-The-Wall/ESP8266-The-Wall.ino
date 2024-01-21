// 'THE WALL' [ESP8266 Public Message Board] (CAPTIVE)
//Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
//Simply connect to the Access Point that is created and navigate to http://wall.local OR http://192.168.4.1
// On Android Devices a login pop up should appear

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <DNSServer.h>

const char *ssid = "📣 The Wall 📣";
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
  String html = "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><link rel=\"stylesheet\" href=\"https://fonts.googleapis.com/css?family=Open+Sans\"></head>";
  html += "<body style=\"background:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAMAAAD04JH5AAACVVBMVEVHcEz///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////84/////////////////////////////////////////////////////////////////////////////v////79///+//8JM9G3AAAAwnRSTlMARAMM/AsF/gIB9w39FPsHHhvREvUI+gS+2wkm8IxYCi3vBryvzSLsDjAg5qM52nK6Lh2s+BBMXqWTQiQP1sVOSUpBVePK3amCPlmdwYTY52ZWfinINDOft1/hL7O2K/aGMTex9IX5YcM81O1ceVtAanQc5Xh9fL9di5o4da070NWclj0yF6Zn4iMTiYjAJYHJf0Y6dsJ3h1JogNmQuKuZFo9a8qRQsM7onukZzEhrJwlRg40qZSyu3FNx68bq5GOgqnHbEFAAAAN5SURBVBgZ7cFVdxtXAEbRT1DNjJkZ4oAhdhw7DjMzMzNjmduUmZmZmRlP0v6ujmSQYrnRQ9bc+zJ7KxQKhUKhUCgUumIlnqzaRY9smgRMlD3tJJXIlmJSCmRJeZx+41xZMZlBD8iGKtL2yrhYC/3OtwFOiUyrZcCdWg485MqsCgY1S9EeqJVRtzLEVVJTzf0yaS1DKmUDabNkwW2kzZAFV5M2VxaQabTMI9M1Mi7BJWTcWC5xgyvD1jHMCpmVxzAFMmo2FximTSbtJ9t6GXQHIxglcxjRtTKllRE5R26cd2BWyaSYgjaTy7tXASsjh/EKFrnE6xQocupTcLzeJVzeP3fdXaqglDaSQ8GahAKzu4VcipoVnNv5HxcZ1K0gdZJTqQI0hcv4qPA4sFRBijAy56eeE/JtcVim4HiKk63mi2OTEurn9jJRQfKilc1XpdUllPLH31LxuabPP5WWr5Y5vVOnTp3gyT3Jkd9IeVd6QaZ8Q79YH77fp/9y+vR6V0aMj8cbmo5P+ar+FDBZ8FexJ6MmkvR1Eb6DRxt17uyfP/8gs9zvvuVLYLt04Fd8hz0ZdQam7ZrG2nxPBfjiMuzsvtouRQBnh3vmx0P7FftM5kVIiu/5Xnt3F30s8yKkfSILIqQ4ON2NsiFCSoNsiZDiyJYISY/VS3JXPtp3U9f198moFfgSkkY/zoDF42XSqs5V8o0hw9ioTJjwRryiTgMeccjQoOAlXsVXqKQF+55S+xgytCho80n6UFM2qXo1SU8+XD5/x4T61votG08upcxToEaRtEEnJie0kyHOnEPLto87LGlRQoHCty2h950P1Emmf/G1Kmhl4LwslUP0PbK9qIC5VdQWSiqAUkaQV61gPcviakmboWMNWZocnlewnu6WLx8oJ9sT3jgWKVDPxeS7B4iSJU/SgjH5Cp4DeGRplCGjgLgYLs+VITcDFSLTRWCDTKnBJ4bpkjEN+DwuNUPmOPii08k0TQZdh69wIRnyZNJYfFu9ozgM2iST5uEr0jY6GLBHRnWQJL1WzABPRo2Gg3BMEv3myLAlK10ok+SQMluGvZKvN6FQep0UWVAdZ7r0Nkm3yIZ3oFIxkmbKip20SVX4XFlRuY6tegufLGmvOiXVQFzWVGxUcRHImrrNCb3EBVdWPcNC2fXgXFmWr1AoFAqFQqFQ6Ar8B7oMBQn0DQfKAAAAAElFTkSuQmCC), linear-gradient(to bottom left, #fa711b, #8104c9)\">";
  html += "<div align='center'>";
  html += "<h1 style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 36px;\"> 📣 THE WALL 📣</h1>";
  html += "<h2 style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 24px;\"> Public Message Board</h2>";
  html += "<form id='messageForm' action='/post' method='post' style=\"border-radius: 5px; background-color: #404040; color: white;\">";
  html += "<input type='text' name='message' style=\"border-radius: 5px; font-size: 24px;\" placeholder='Enter your message' autofocus>";
  html += "<input type='submit' value='Post' style=\"border-radius: 5px; background-color: #00cc00; color: white; font-weight: bold; font-size: 26px;\">";
  html += "<p style=\"border-radius: 5px; background-color: #404040; color: white; font-weight: bold; font-size: 16px;\"> Uptime: " + getUptime() + "</p>";
  html += "</form></div>";
  html += "<h2 align='center' style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 30px;\"> Recent Messages:</h2>";
  html += "<div id='messageBoard' style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 24px;\">" + messageBoard + "</div>";
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
  String html = "<html><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></head><body style=\"background:url(data:image/png;base64,), linear-gradient(to bottom left, #fa711b, #8104c9)\">";
  html += "<div align='center'>"; 
//  html += "<img src=\"data:image/gif;base64,\">";
  html += "<h1 style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 36px;\"> 📣 THE WALL 📣</h1>";
  html += "<h2 style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 24px;\"> Public Message Board</h2>";
  html += "<p style=\"border-radius: 5px; background-color: #404040; color: white; font-weight: bold; font-size: 16px;\"> Uptime: " + getUptime() + "</p>";
  html += "<h3 style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 20px;\"> If you could say anything to anyone who will listen.. What would you say? Enter the chat to find out what others said or say something yourself!</h3>";
  html += "<input type='button' value=' Enter Chat ' style=\"border-radius: 10px; background-color: #00cc00; color: white; font-weight: bold; font-size: 64px;\" onclick=\"window.location.href = '/';\">";
  html += "<h3 style=\"border-radius: 5px; background-color: #404040; color: white; font-size: 20px;\"> If you lose this chat, simply reconnect or go to: http://wall.local in your browser</h3>";
  html += "</form></div>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleNotFound() {
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

  if (!SPIFFS.begin()) {
    Serial.println("Error initializing SPIFFS");
    return;
  }
  
  loadMessagesFromFile();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/login", HTTP_GET, handleLogin);
  server.on("/messages", HTTP_GET, handleMessages);
  server.on("/post", HTTP_POST, handlePost);
  server.onNotFound(handleNotFound);
  
  startTime = millis();
  
  server.begin();
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
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
