/*
  Based on "HelloServer" by Ivan Grokhotkov
  This example code is in the public domain.

  Query DHT: http://<ip>/html
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// setup the DHT22
#include "DHT.h"
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT22 // DHT 22: AM2302, AM2321 - can also use DHT11 or DHT21 here

// Set to true to return degrees fahrenheit, otherwise return degrees centigrade
const boolean usedegF = true;

// Set WiFi SSID and password
const char* ssid = "linksys";
const char* password = "default";

// Set to true to use a static IP, false for DHCP
const boolean useStaticIP = true;
// Define static address details.
const IPAddress ip(192, 168, 0, 200);
const IPAddress dns(8, 8, 8, 8);
const IPAddress subnet(255, 255, 255, 0);
const IPAddress gateway(192, 168, 0, 1);

// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

ESP8266WebServer server(LISTEN_PORT);
DHT dht(DHTPIN, DHTTYPE);

// Create callbacks for REST commands

// handler for /
// Returns temp,humidty for use by Artisan
void handleArtisan() {
  // Read current values
  float dhtTemp = dht.readTemperature(usedegF);
  float dhtHumid = dht.readHumidity();
  // Convert floats to string with 1 decimal place
  String str_dhtTemp = String (dhtTemp,1);
  String str_dhtHumid = String (dhtHumid,0);
  // Assemble the JSON object
  String message = str_dhtTemp;
  message += ",";
  message += str_dhtHumid;
  // Output the requested values
  server.send(200, "text/plain", message);
  Serial.println(message);
}

// handler for /html
// Returns temp and humidty in HTML format
void handleHTML() {
  // Read current values
  float dhtTemp = dht.readTemperature(usedegF);
  float dhtHumid = dht.readHumidity();
  // Convert floats to string with decimal place based on device accuracy
  String str_dhtTemp = String (dhtTemp,1);
  String str_dhtHumid = String (dhtHumid,0);
  
  String message = "<html><body><h1>DHT22</h1>\n\n";
  if (usedegF) {
    message += "<b>Temperature &deg;F: </b>";
  }
  else {
    message += "<b>Temperature &deg;C: </b>";
  }
  message += str_dhtTemp;
  message += "<br>\n";
  message += "<b>Humidity %: </b>";
  message += str_dhtHumid;
  message += "<br>\n";
  message += "</body></html>";
  server.send(200, "text/html", message);
}

// handler for /json
// Return a simple JSON object with the DHT readings
void handleJSON() {
  // Read current values
  float dhtTemp = dht.readTemperature(usedegF);
  float dhtHumid = dht.readHumidity();
  // Convert floats to string with decimal place based on device accuracy
  String str_dhtTemp = String (dhtTemp,1);
  String str_dhtHumid = String (dhtHumid,0);
  // Assemble the JSON object
  String message = "{\"temperature\":\"";
  message += str_dhtTemp;
  message += "\", \"humidity\":\"";
  message += str_dhtHumid;
  message += "\"}";
  // Output the requested values
  server.send(200, "text/plain", message);
  Serial.println(message);
}

// handler for /temp
// Return the temperature reading
void handleTemp() {
  // Read current values
  float dhtTemp = dht.readTemperature(usedegF);
  // Convert floats to string with 1 decimal place
  String str_dhtTemp = String (dhtTemp,1);
  // Output the requested value
  server.send(200, "text/plain", str_dhtTemp);
  Serial.println(str_dhtTemp);
}

// handler for /humid
// Return the humidty reading
void handleHumid() {
  // Read current values
  float dhtHumid = dht.readHumidity();
  // Convert floats to string with 0 decimal place(s)
  String str_dhtHumid = String (dhtHumid,0);
  // Output the requested value
  server.send(200, "text/plain", str_dhtHumid);
  Serial.println(str_dhtHumid);
}

// Send the user a 404 if we've been sent something we can't handle
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setup(void) {
  Serial.begin(115200);

  dht.begin();
  if (useStaticIP) {
    WiFi.config(ip, dns, gateway, subnet);
  }
  WiFi.begin(ssid, password);
  Serial.println("");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Attempting connection to wireless network ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.on("/", handleArtisan);
  server.on("/html", handleHTML);
  server.on("/json", handleJSON);
  server.on("/temp", handleTemp);
  server.on("/humid", handleHumid);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.print("Server started @ http://");

  // Print the IP address
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(LISTEN_PORT);
}

void loop(void) {
  // run the HTTP server loop
  server.handleClient();
}
