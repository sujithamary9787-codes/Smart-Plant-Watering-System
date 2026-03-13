// Smart Plant Watering System
// Developed by Sujitha Mary
// This program monitors soil moisture and automatically waters plants.
// A notification is also sent to the Blynk mobile application.

#define BLYNK_TEMPLATE_ID "TMPLxxxxxx"
#define BLYNK_TEMPLATE_NAME "PlantWaterSystem"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi details
char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

// Pin connections
#define SOIL_PIN 34
#define RELAY_PIN 26

int moistureValue = 0;
int moistureLimit = 2500;   // moisture threshold

BlynkTimer timer;

// function to read soil moisture
void readSoilData()
{
  moistureValue = analogRead(SOIL_PIN);

  Serial.print("Moisture Level : ");
  Serial.println(moistureValue);

  // send value to Blynk dashboard
  Blynk.virtualWrite(V0, moistureValue);

  // condition to control pump
  if (moistureValue > moistureLimit)
  {
    Serial.println("Soil is dry - pump ON");
    digitalWrite(RELAY_PIN, LOW);

    // mobile notification
    Blynk.logEvent("soil_dry", "Soil moisture is low. Pump activated.");
  }
  else
  {
    Serial.println("Soil moisture is sufficient - pump OFF");
    digitalWrite(RELAY_PIN, HIGH);
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  // connecting ESP32 with Blynk cloud
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // check soil condition every 5 seconds
  timer.setInterval(5000L, readSoilData);
}

void loop()
{
  Blynk.run();
  timer.run();
}
