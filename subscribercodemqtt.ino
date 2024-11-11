// SUBSCRIBER
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "oplus_co_apdmkd";
const char* password = "tohm3279";

// MQTT broker details
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* mqttTopicWave = "SIT210/waves";


// LED pin
const int ledPin = 6;

// Client declarations
WiFiClient espClient;
PubSubClient client(espClient);

// Function declarations
void connectToWiFi();
void connectToMQTT();
void messageReceived(char* topic, byte* payload, unsigned int length);
void flashLED(int times);

void setup() {
  Serial.begin(115200);         // Initialize serial monitor
  pinMode(ledPin, OUTPUT);       // Initialize LED pin

  connectToWiFi();               // Connect to WiFi

  client.setServer(mqttServer, mqttPort); // Set MQTT server and port
  client.setCallback(messageReceived);    // Set callback function

  connectToMQTT();               // Connect to MQTT
}

void loop() {
  if (!client.connected()) {     // Reconnect if MQTT is disconnected
    connectToMQTT();
  }
  client.loop();                 // Process incoming messages
}

// Connect to WiFi
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

// Connect to MQTT broker and subscribe to topics
void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ReceiverClient")) {
      Serial.println("Connected to MQTT");
      client.subscribe(mqttTopicWave);
      
    } else {
      delay(5000);
    }
  }
}

// Handle incoming MQTT messages
void messageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic ");
  Serial.print(topic);
  Serial.print(": ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, mqttTopicWave) == 0) {
    Serial.println("Wave message received. Flashing LED 3 times...");
    flashLED(3);
  } else if (strcmp(topic, mqttTopicWave) == 0) {
    Serial.println("Pat message received. Flashing LED 5 times...");
    flashLED(5);
  }
}

// Flash LED for specified times
void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}
