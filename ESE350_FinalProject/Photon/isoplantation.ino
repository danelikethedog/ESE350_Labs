#include "Adafruit_DHT.h"

#include <math.h>


int led2 = D7;

DHT dht(1,DHT11);

int once = 0;

SYSTEM_MODE(AUTOMATIC);

void setup() {

    pinMode(led2, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    dht.begin();

}

void loop() {
    int temp = analogRead(A0);
    float millivolts= (temp/4096.0) * 3300.0;
    float fahrenheit= (millivolts)/10;
    Serial.print(fahrenheit);
    Serial.println(" degrees Fahrenheit, ");

    int publishTemp = round(fahrenheit);
    char stringConv[15];
    sprintf(stringConv, "%d", publishTemp);
    
    float humidity = dht.getHumidity();
    Serial.print("Humidity is ");
    Serial.println(humidity);
    float humSensTemp = dht.getTempFarenheit();

    int humidityRound = round(humidity);
    char humidityConv[15];
    sprintf(humidityConv, "%d",humidityRound);



    float light = analogRead(A1);
    int lightRound = round(light);
    Serial.print("Light Intensity is ");
    Serial.println(light);

    char json[63];
    sprintf(json, "{\"t\": %i, \"h\": %i, \"l\": %i}", publishTemp, humidityRound,lightRound);

    if (!once) {
        Particle.publish("curState", json);
        //Particle.publish("humidity", humidityConv);
        once = 1;
    }
    

    digitalWrite(led2, HIGH);
    

    delay(1000);

    digitalWrite(led2, LOW);


    delay(1000);

}