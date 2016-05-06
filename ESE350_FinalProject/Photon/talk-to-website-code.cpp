// This #include statement was automatically added by the Particle IDE.
#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "HttpClient/HttpClient.h"

#include <sstream>
#include <string>

HttpClient http;
// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;
String test;

void setup() {
    Serial.begin(9600);
    request.hostname = "www.isoplantation.xyz";
    request.port = 80;
    request.path = "/api/get/plants";
    http.get(request, response, headers);
}

/**
 * 
 *  Assumes the id exists, check by doing a simple if statement 
 * 
 * 
 */
void update_plant(int id, int current_water, int current_light) {
    char intToStringBuffer[100];
    sprintf(intToStringBuffer, "/api/update/%d/current_light=%d&current_water=%d", id,current_light,current_water);
    String path(intToStringBuffer);
    request.path = path;
    http.get(request,response, headers);
}

void set_lights(int id, int value) {
    char intToStringBuffer[100];
    sprintf(intToStringBuffer, "/api/update/%d/light=%d", id, value);
    String path(intToStringBuffer);
    request.path = path;
    http.get(request,response, headers);
}

void set_ambient(int current_humidity, int current_temperature) {
    char intToStringBuffer[100];
    sprintf(intToStringBuffer, "/api/update/planter/current_temperature=%d&current_humidity=%d", current_temperature, current_humidity);
    String path(intToStringBuffer);
    request.path = path;
    http.get(request,response, headers);
}

JsonObject& makeJSONParser(String json) {
    char jsontest[1500];
    strcpy(jsontest, json.c_str()); //convert from const
    StaticJsonBuffer<1500> jsonBuffer;
    return jsonBuffer.parseObject(jsontest);
}


void loop() {
   JsonObject& root = makeJSONParser(response.body);
   test = root["0"][0]["plant"];
   Serial.print(test);

    delay(10000);

}