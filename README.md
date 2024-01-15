# 'THE WALL' [ESP8266 Message Board]

**ESP8266 Message Board**
- Creates an AP named 'The Wall' and serves a simple webpage where anyone can leave an annoymous message.
- Simply connect to the Access Point that is created (SSID = 'The Wall') and navigate to http://thewall.local

# INSTALL INSTRUCTIONS
**Install from source**

You need to install the Arduino IDE 

From there you need a couple installations clicking the upload button. First got File -> Preferences. At the bottom of the window, you should see Additional Boards Manager URLS. Click the little icon at the end at the end of that line. A new window should show up asking you to Enter additional URLS, one for each row. Add:

> https://arduino.esp8266.com/stable/package_esp8266com_index.json

And click OK.

Now you only need to install the board. Tools -> Board: "<SOME BOARD NAME>" -> Boards Manager. In the search bar, type

`esp8266`

by ESP8266 Community. I installed version 2.5.5 2.6.0.
