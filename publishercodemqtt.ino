// PUBLISHER
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT server configuration
const char* ssid = "oplus_co_apdmkd";
const char* password = "tohm3279";
const char* mqttServer = "broker.emqx.io";
const int mqttPort = 1883;
const char* topicWave = "SIT210/waves";


// WiFi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

// Ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

// Threshold distance for pat detection (in centimeters)
const int thresholdDistance = 20;

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Initialize sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // Publish message based on pat detection
  if (detectPat()) {
    client.publish(topicWave, "Pat detected");
  } else {
    client.publish(topicWave, "Wave detected");
  }

  delay(1000);  // Delay between checks
}

// Function to connect to WiFi
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}

// Function to connect to MQTT server
void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("SenderClient")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed to connect. Attempting again in 5 seconds.");
      delay(5000);
    }
  }
}

// Function to detect a pat (returns true if detected, otherwise false)
bool detectPat() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Calculate distance in cm

  // Return true if distance is above threshold, false otherwise
  return (distance > thresholdDistance);
}
