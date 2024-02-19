# 'THE WALL' [ESP8266/32 Message Board]

![Screenshot_2](https://github.com/beigeworm/ESP8266-Message-Board/assets/93350544/aa7f49d8-6756-406e-909e-72017144eb40)

**ESP8266/ESP32 Message Board**
- Creates an AP named 'THE WALL' and serves a simple webpage where anyone can leave an annoymous message.
- Simply connect to the Access Point that is created and navigate to http://wall.local OR http://192.168.4.1
- On Android Devices or Windows Laptops a login pop up should appear. (WORKS: Oneplus, Honor | NOT WORKING: Samsung)
- If this doesnt work, navigate to http://wall.local OR http://192.168.4.1
- Posted messages are saved and persisent on restarts. 

**EXTRAS** (ESP32 ONLY)
- Use http://wall.local/logs to see connected devices information.

# INSTALL INSTRUCTIONS
**Install from source**

You need to install the Arduino IDE, version 1.8.16

From there you need a couple installations clicking the upload button. First got File -> Preferences. At the bottom of the window, you should see Additional Boards Manager URLS. Click the little icon at the end at the end of that line. A new window should show up asking you to Enter additional URLS, one for each row. Add:

**For ESP8266**
> https://arduino.esp8266.com/stable/package_esp8266com_index.json

**For ESP32**
> https://espressif.github.io/arduino-esp32/package_esp32_index.json

And click OK.

-------------------------------------------------------------------------------------------------------------


**For ESP8266**
- Install the libraries. Go to Sketch -> Include Library -> Add .ZIP Library and add the supplied libraries.
- Now you only need to install the board. Go to > Tools -> Board: "<SOME BOARD NAME>" -> Boards Manager. In the search bar, type `esp8266` by ESP8266 Community. install version 2.7.3.
- Go to > Tools -> Board: "SOME BOARD NAME"
- Go to > Tools -> Erase Flash -> 'All Flash Contents'.

![Screenshot_1](https://github.com/beigeworm/ESP8266-Message-Board/assets/93350544/c3a6672d-0c44-4068-b4a5-61b9d2dcd31a)

Click Upload Script.

-------------------------------------------------------------------------------------------------------------

**For ESP32**

Go to Tools -> Manage Libraries 
- ESPAsyncWebServer
- ESPmDNS
- DNSServer
- ArduinoJson (Version 6.0 or above)

![Screenshot_1](https://github.com/beigeworm/ESP8266-and-ESP32-Message-Board/assets/93350544/46833ec5-c7ee-4317-8864-9c002a823b2d)

Make sure to select 'Erase all flash contents' to ENABLED.

Click Upload Script.

-------------------------------------------------------------------------------------------------------------

**For JS Server** (on debian based linux)

Start an ssh connection to your server and login (I used linode for testing)

> mkdir jsServer

> cd jsServer

Place the `server.js` file in this new folder.

> sudo apt install nodejs npm

> npm init -y

> npm install express body-parser

> npm install axios

> npm install multer

> npm install express-fileupload

> npm install express-session

> npm install -g pm2

> pm2 start server.js

(if something is already using port 80 you will need to change this in `server.js`)

The Wall should now be reachable at your server's ip address.


(to stop the server..)
> pm2 stop server.js
