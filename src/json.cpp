#include <iostream>
#include "json.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string>

using namespace std;

int countBuses(JsonArray &busArray)
{
    return busArray.size(); // warn - this is O(n)
}

void calculateBusLocations(JsonArray &busArray, int numBuses){
    for (int i = 0; i < numBuses; i++){
        double busDist = busArray[i]["pdist"]; 
        string busId = busArray[i]["vid"];

        bool headingNorth = (busArray[i]["des"] == "Baits I");

        string goingFrom = headingNorth ? "CCTC" : "Lot NC32";

        Serial.printf("Distance of bus %s from %s: %f\n", busId.c_str(), goingFrom.c_str(), busDist);
       
        //Determine location on board
    }

}
