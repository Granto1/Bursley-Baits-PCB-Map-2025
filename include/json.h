#pragma once

#include <ArduinoJson.h>


int countBuses(JsonArray &busArray);
void calculateBusLocations(JsonArray &busArray, int numBuses);