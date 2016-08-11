# WiFiDHT
Arduino sketch for the ESP8266 and a DHT22 sensor to allow collection of temp/humidity readings via HTTP.

Configure the WiFi and IP address details found in the sketch and flash to your device.  Once the device is attached to your network, temperature and humidity readings can be accessed via HTTP per the following examples:

`curl http://192.168.0.101` returns `78.9,56`

`curl http://192.168.0.101/json` returns `{"temperature":"78.9", "humidity":"56"}`

`curl http://192.168.0.101/temp` returns `78.9`

`curl http://192.168.0.101/humid` returns `56`

`curl http://192.168.0.101/html` returns a web page with the current sensor readings in HTML format

### Requirements
- A Windows, OSX, or Linux computer with an available USB port
- A micro-USB cable
- An ESP8266 development board (sketch is configured for the [WeMos D1 Mini](http://www.wemos.cc/Products/d1_mini.html))
- A [DHT22 Humidty/Temperature sensor compatible with your ESP8266 board](http://www.wemos.cc/Products/dht_pro_shield.html).

### Setup Procedure
##### 1. Install the Arduino IDE
Download the Arduino IDE for your operating system and install with the instructions linked below

- [Software download](http://www.arduino.cc/en/main/software)
- [Installation instructions](http://www.arduino.cc/en/Guide/HomePage)

##### 2. Configure the ESP8266 Board Manager
Once the Arduino IDE is available we can add support for the development boards we'll be using.

1. Launch the Arduino application
2. Select the "File" menu ("Arduino" menu on OSX), then "Preferences"
3. Paste the following URL in the box labeled "Additional Boards Manager URLs": `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
The page should look something like this:
![Board Manager screenshot](https://github.com/aderusha/WiFiDHT/blob/master/Images/BoardManagerScreenshot.png)
4. While we're here, check "Display line numbers"
5. Click OK to accept the changes
6. Select the "Tools" menu, "Board" menu, then "Boards Manager"
7. Search for ESP8266, click on the "esp8266" board and select "Install"
