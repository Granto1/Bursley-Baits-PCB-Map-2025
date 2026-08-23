#include <FastLED.h>
#include <ArduinoJson.h>
#include <iostream>
#include <string>
#include <HTTPClient.h>
#include <WiFi.h>

const char *ssid = "Grant's Galaxy S21 Ultra 5G";
const char *password = "ylga096/";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

#define LED_PIN 19  // D8 maps to GPIO15
#define NUM_LEDS 62
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 10


CRGB leds[NUM_LEDS];

const char *url = "https://mbus.ltp.umich.edu/bustime/api/v3/getvehicles?rt=BB&key=PVEAKStQa5ys9D4xcFDzxRz4W&format=json";

int countBuses(JsonArray &busArray) {
  return busArray.size();  // warn - this is O(n)
}

struct Pair {
  double latitude;
  double longitude;
};

Pair coordinates[] = {
  { 42.28129, -83.737184 },
  { 42.280086, -83.737184 },  // Rackham
  { 42.278662, -83.737184 },
  { 42.278611, -83.736006 },
  { 42.277818, -83.734847 },  // CCTC
  //outbound
  { 42.277388, -83.733407 },
  // nighttime
  { 42.277452, -83.733407 },
  { 42.277536, -83.733407 },  // Observatory
  { 42.278775, -83.731031 },  // Stockwell
  { 42.280451, -83.731092 },
  { 42.28139, -83.731092 },  // Lloyd/Observatory
  { 42.281444, -83.73043 },
  // normal op
  { 42.277519, -83.730991 },
  { 42.279736, -83.731073 },
  { 42.281381, -83.731073 },
  // hospital region (couzens zina pitcher)
  { 42.281409, -83.728824 },  // Couzens/Zina Pitcher
  { 42.282027, -83.726196 },
  { 42.284754, -83.727618 },  // Medical School
  { 42.285347, -83.730048 },
  { 42.286451, -83.732441 },
  // onto fuller next to river
  { 42.286941, -83.727836 },
  { 42.287103, -83.724681 },
  { 42.287152, -83.722926 },  // Mitchell
  { 42.287217, -83.720365 },
  { 42.287342, -83.71907 },
  { 42.288787, -83.719248 },
  { 42.289995, -83.718976 },  // corner
  { 42.290163, -83.718395 },
  { 42.290837, -83.71846 },
  { 42.291386, -83.718535 },  // pierpont
  { 42.292704, -83.718739 },
  { 42.294023, -83.718993 },
  { 42.294891, -83.719274 },
  { 42.294659, -83.72048 },
  { 42.294594, -83.721934 },  // Bursley
  { 42.294489, -83.723525 },
  { 42.294175, -83.724195 },  // Baits
  { 42.2936, -83.724533 },
  { 42.292766, -83.724482 },
  { 42.292064, -83.724193 },
  { 42.291579, -83.723658 },
  { 42.291836, -83.723189 },  // NC32
  // inbound
  { 42.29275, -83.723926 },
  { 42.293777, -83.72447 },  // Baits
  { 42.294528, -83.723203 },
  { 42.294598, -83.721777 },  // Bursley
  { 42.294848, -83.719244 },
  { 42.292876, -83.718838 },
  { 42.291093, -83.718554 },  // Pierpont
  { 42.290427, -83.718498 },
  { 42.290011, -83.719614 },
  { 42.288692, -83.719539 },
  { 42.287332, -83.719183 },
  { 42.287133, -83.723156 },  // Mitchell
  { 42.287121, -83.725167 },
  { 42.28698, -83.728779 },
  { 42.286508, -83.732476 },
  // hospital detour
  { 42.285174, -83.719126 },
  { 42.281758, -83.726368 },
  { 42.281415, -83.731094 },  // Glen Catherine
  { 42.281291, -83.733958 },
  { 42.281282, -83.737193 },
};

double distanceFormula(double x1, double x2, double y1, double y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}


void calculateBusLocations(JsonArray &busArray, int numBuses) {
  for (int i = 0; i < numBuses; i++) {
    double busDist = busArray[i]["pdist"];
    String busId = busArray[i]["vid"];
    double lat = busArray[i]["lat"];
    double lon = busArray[i]["lon"];

    bool headingNorth = (busArray[i]["des"] == "Baits I");

    String goingFrom = headingNorth ? "CCTC" : "Lot NC32";

    Serial.printf("Distance of bus %s from %s: %f, %f, %f \n", busId, goingFrom, busDist, lat, lon);


    double closest_distance = 1000;
    int closest = -1;

    if (headingNorth) {
      // ignore 6-11
      for (int a = 4; a < 42; a++) {
        double dist = distanceFormula(lat, coordinates[a].latitude, lon, coordinates[a].longitude);
        if (dist < closest_distance) {
          closest = a;
          closest_distance = dist;
        }

        // skip the normal operation chunk as it looks odd
        if (a == 11) {
          a += 3;
        }
      }
    } else {
      for (int a = 0; a < 5; a++) {
        double dist = distanceFormula(lat, coordinates[a].latitude, lon, coordinates[a].longitude);
        if (dist < closest_distance) {
          closest = a;
          closest_distance = dist;
        }
      }
      for (int b = 42; b < 62; b++) {
        double dist = distanceFormula(lat, coordinates[b].latitude, lon, coordinates[b].longitude);
        if (dist < closest_distance) {
          closest = b;
          closest_distance = dist;
        }
      }
    }
    if (closest != -1) {
      if (leds[closest] != CRGB::Black) {
        leds[closest] = CRGB::Red;
      } else {
        leds[closest] = (busId == "3031" || busId == "3069") ? CRGB::Yellow : CRGB::Blue;
      }
      Serial.printf("closest was index %d with distance %f\n", closest, closest_distance);
    }
  }
}


void setup() {

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(115200);
  delay(7500);

  leds[0] = CRGB::Green;
  FastLED.show();


  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    leds[1] = CRGB::Green;
    FastLED.show();
  } else {
    Serial.println("Failed to connect to WiFi");
    int16_t results = WiFi.scanNetworks(false, true, false, 1000, 0, nullptr, nullptr);
    return;
  }

  // Init and get the time
  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // printLocalTime();

  // struct tm timeinfo;
  // if (!getLocalTime(&timeinfo)) {
  //   Serial.println("Failed to obtain time");
  //   return;
  // }

  // leds[2] = CRGB::Green;
  // FastLED.show();

  // Serial.print("Hour: ");
  // Serial.println(&timeinfo, "%H");
  // strftime(timeHour, 3, "%H", &timeinfo);
  // Serial.println(timeHour);
}

void loop() {
  delay(15000);

  FastLED.clear();

  HTTPClient http;

  Serial.println("Sending GET request...");
  http.begin(url);
  int httpCode = http.GET();

  String payload;

  if (httpCode > 0) {
    //Serial.printf("Response code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      /*  Serial.println("Response:");
      Serial.println(payload); */
    }
  } else {
    Serial.printf("HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
    return;
  }

  http.end();

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    //trigger something visible?
    Serial.println("failure deserializing json");
  }

  JsonArray buses = doc["bustime-response"]["vehicle"];

  if (buses == nullptr) {
    // something visible idk
    Serial.println("Seems like parsing failed");
  }


  int numBuses = countBuses(buses);

  Serial.printf("# of buses running right now on BB: %d\n", numBuses);


  FastLED.clear(false);
  calculateBusLocations(buses, numBuses);
  FastLED.show();

  // fill_rainbow(leds, NUM_LEDS, millis() / 10);
  // FastLED.show();
  // delay(20);
}


//16297