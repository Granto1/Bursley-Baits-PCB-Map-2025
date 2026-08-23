
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "main.h"
#include "json.h"

using namespace std;

const char *ssid = "Lukehotspot";
const char *password = "helloworld";

/*
  Some notes: 

  pdist property to find distance from the 2 endpoints: Baits I & CCTC (Baits I is really the NC32 parking lot).
  Distance is measured in feet from CCTC or Baits I; and gets reset to 0 at each one. 
*/

const char *url = "https://mbus.ltp.umich.edu/bustime/api/v3/getvehicles?rt=BB&key=PVEAKStQa5ys9D4xcFDzxRz4W&format=json";


void setup()
{
  Serial.begin(115200);
  delay(7500);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20)
  {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Failed to connect to WiFi");
    int16_t results = WiFi.scanNetworks(false, true, false, 1000, 0, nullptr, nullptr);
    return;
  }
}

void loop()
{
  delay(15000);

  HTTPClient http;

  Serial.println("Sending GET request...");
  http.begin(url);
  int httpCode = http.GET();

  String payload;

  if (httpCode > 0)
  {
    //Serial.printf("Response code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK)
    {
      payload = http.getString();
     /*  Serial.println("Response:");
      Serial.println(payload); */
    }
  }
  else
  {
    Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
    return;
  }

  http.end();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if(error){
    //trigger something visible?
    Serial.println("failure deserializing json"); 
  }

  JsonArray buses = doc["bustime-response"]["vehicle"];

  if (buses == nullptr)
  {
    // something visible idk
    Serial.println("Seems like parsing failed");
  }


  int numBuses = countBuses(buses);

  Serial.printf("# of buses running right now on BB: %d\n", numBuses);


  calculateBusLocations(buses, numBuses);

}

/* platform io test && json decoding */