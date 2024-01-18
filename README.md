# 'THE WALL' [ESP8266 Message Board]

![Screenshot_2](https://github.com/beigeworm/ESP8266-Message-Board/assets/93350544/aa7f49d8-6756-406e-909e-72017144eb40)

**ESP8266 Message Board**
- Creates an AP named 'THE WALL' and serves a simple webpage where anyone can leave an annoymous message.
- Simply connect to the Access Point that is created and navigate to http://wall.local OR http://192.168.4.1
- (CAPTIVE Only) : On an Android device a pop up should appear upon connecting to the AP 

# INSTALL INSTRUCTIONS
**Install from source**

You need to install the Arduino IDE, version 1.8.16

From there you need a couple installations clicking the upload button. First got File -> Preferences. At the bottom of the window, you should see Additional Boards Manager URLS. Click the little icon at the end at the end of that line. A new window should show up asking you to Enter additional URLS, one for each row. Add:

> https://arduino.esp8266.com/stable/package_esp8266com_index.json

And click OK.

Install the libraries. Go to Sketch -> Include Library -> Add .ZIP Library and add the supplied libraries.

Now you only need to install the board. Go to > Tools -> Board: "<SOME BOARD NAME>" -> Boards Manager. In the search bar, type `esp8266` by ESP8266 Community. install version 2.7.3.

Go to > Tools -> Board: "SOME BOARD NAME"

Go to > Tools -> Erase Flash -> 'All Flash Contents'.

![Screenshot_1](https://github.com/beigeworm/ESP8266-Message-Board/assets/93350544/c3a6672d-0c44-4068-b4a5-61b9d2dcd31a)

Click Upload Script.

