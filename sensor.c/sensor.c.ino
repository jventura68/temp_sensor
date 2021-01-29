#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"

// Uncomment one of the lines below for whatever DHT sensor type you're using!
//#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

/*Put your SSID & Password*/
const char* ssid = "iot_home";  // Enter SSID here
const char* password = "@j0o0s0e0_iot";  //Enter Password here



// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.1.50";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


float temperature = 0;
float humidity = 0;



// DHT Sensor
const int DHTPin = 15; 
const int ledPin = 2;
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

 
void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(DHTPin, INPUT);
  pinMode(ledPin, OUTPUT);
  dht.begin();              

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  digitalWrite (ledPin, HIGH);
  temperature = dht.readTemperature(); // Gets the values of the temperature
  humidity = dht.readHumidity(); // Gets the values of the humidity 
  
  // Convert the value to a char array
  char tempString[8];
  dtostrf(temperature, 4, 1, tempString);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  client.publish("/home/salon/01/temperature", tempString);

  // Convert the value to a char array
  char humString[8];
  dtostrf(humidity, 4, 1, humString);
  Serial.print("Humidity: ");
  Serial.println(humString);
  client.publish("/home/salon/01/humidity", humString);
  delay(1000);
  digitalWrite (ledPin, LOW);
  delay(4000);
}


  
