#include "Adafruit_DHT.h"

#include <math.h>
#include <sstream>


struct Plant {
    int ID;
    int minT;
    int maxT;
    int lightMinTime;
    int soilWaterMin;
    int soilWaterMax;
    int spread;
};

struct Plant cabbage;
struct Plant bean;
struct Plant lettuce;
struct Plant tomato;
struct Plant strawberry;
struct Plant carrot;
struct Plant radish;
struct Plant basil;
struct Plant chives;
struct Plant mint;
struct Plant parsley;

struct Plant *quadOne;
struct Plant *quadTwo;
struct Plant *quadThree;
struct Plant *quadFour;



int led2 = D7;

DHT dht(1,DHT11);

int once = 0;
int curMinute = 0;

//Values to publish
int publishTemp = 0;
int publishHumidity = 0;
int publishLight = 0;

//Value for led toggle
int state = 0;

//Plant Structs



SYSTEM_MODE(AUTOMATIC);

void setup() {

    pinMode(led2, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    digitalWrite(led2, LOW);
    Particle.function("flipLight", flipLight);
    Particle.function("setPlant", setPlant);
    dht.begin();
    definePlants();
    defaultPlants();

}


void loop() {

    minuteUpdate();

    digitalWrite(led2, HIGH);
    

    delay(1000);

    digitalWrite(led2, LOW);

    delay(1000);

}

int minuteUpdate() {
    int newMinute = Time.minute();
    if (newMinute != curMinute) {
        curMinute = newMinute;
        updateValues();
        sendDashWebhook();
        sendPlantWebhook();
    }
    return 1;
}

void updateValues() {

    //Get new temp
    int temp = analogRead(A0);
    float millivolts= (temp/4096.0) * 3300.0;
    float fahrenheit= (millivolts)/10;
    publishTemp = round(fahrenheit);

    //Get new humidity
    float humidity = dht.getHumidity();
    publishHumidity = round(humidity);

    //Get new light 
    float light = analogRead(A1);
    publishLight = round(light);
    serialPrint(publishTemp, publishHumidity, publishLight);

}


void sendDashWebhook() {
    char json[63];
    sprintf(json, "{\"t\": %i, \"h\": %i, \"l\": %i}", publishTemp, publishHumidity,publishLight);
    Particle.publish("curState", json);
}

void sendPlantWebhook() {
    char json[63];
    sprintf(json, "{\"1\": %i, \"2\": %i, \"3\": %i, \"4\": %i}", 
                                                *quadOne.ID, *quadTwo.ID, *quadThree.ID, *quadFour.ID);
    Particle.publish("plants", json);
}

void serialPrint(int t, int h, int l) {
    Serial.print("Temp: ");
    Serial.println(t);
    Serial.print("Hum: ");
    Serial.println(h);
    Serial.print("Light: ");
    Serial.println(l);
}

int flipLight(String arg) {
    state = !state;
    digitalWrite(D0, (state) ? HIGH : LOW);
    return 1;
}

int setPlant(String arg) {
    char quad = arg[strlen(arg) - 1];
    char plant[2];
    strncpy(plant, arg, 2);
    int plantId;
    std::stringstream(plant) >> plantId;
    struct Plant *inPlant;
    switch(plantId) {
        case 1 :
            inPlant = &cabbage;
            break;
        case 2 :
            inPlant = &bean;
            break;
        case 3 :
            inPlant = &lettuce;
            break;
        case 4 :
            inPlant = &tomato;
            break;
        case 5 :
            inPlant = &strawberry;
            break;
        case 6 :
            inPlant = &carrot;
            break;
        case 7 :
            inPlant = &radish;
            break;
        case 8 :
            inPlant = &basil;
            break;
        case 9 :
            inPlant = &chives;
            break;
        case 10 :
            inPlant = &mint;
            break;
        case 11 : 
            inPlant = &parsley;
            break;
    }
    switch(quad) {
        case '1' :
            quadOne = inPlant;
            break;
        case '2' :
            quadTwo = inPlant;
            break;
        case '3' :
            quadThree = inPlant;
            break;
        case '4' :
            quadFour = inPlant;
            break;
    }
    return 1;
}

void defaultPlants() {
    quadOne = &lettuce;
    quadTwo = &lettuce;
    quadThree = &lettuce;
    quadFour = &lettuce;
}

void definePlants() {
    cabbage.ID = 1;
    cabbage.minT = 60;
    cabbage.maxT = 65;
    cabbage.lightMinTime = 4;
    cabbage.soilWaterMin = 0;
    cabbage.soilWaterMax = 0;
    cabbage.spread = 12;

    bean.ID = 2;
    bean.minT = 70;
    bean.maxT = 80;
    bean.lightMinTime = 10;
    bean.soilWaterMin = 0;
    bean.soilWaterMax = 0;
    bean.spread = 4;

    lettuce.ID = 3;
    lettuce.minT = 55;
    lettuce.maxT = 80;
    lettuce.lightMinTime = 4;
    lettuce.soilWaterMin = 0;
    lettuce.soilWaterMax = 0;
    lettuce.spread = 6;

    tomato.ID = 4;
    tomato.minT = 70;
    tomato.maxT = 90;
    tomato.lightMinTime = 10;
    tomato.soilWaterMin = 0;
    tomato.soilWaterMax = 0;
    tomato.spread = 12;

    strawberry.ID = 5;
    strawberry.minT = 60;
    strawberry.maxT = 85;
    strawberry.lightMinTime = 6;
    strawberry.soilWaterMin = 0;
    strawberry.soilWaterMax = 0;
    strawberry.spread = 6;

    carrot.ID = 6;
    carrot.minT = 55;
    carrot.maxT = 80;
    carrot.lightMinTime = 8;
    carrot.soilWaterMin = 0;
    carrot.soilWaterMax = 0;
    carrot.spread = 3;

    radish.ID = 7;
    radish.minT = 60;
    radish.maxT = 80;
    radish.lightMinTime = 8;
    radish.soilWaterMin = 0;
    radish.soilWaterMax = 0;
    radish.spread = 4;

    basil.ID = 8;
    basil.minT = 65;
    basil.maxT = 85;
    basil.lightMinTime = 6;
    basil.soilWaterMin = 0;
    basil.soilWaterMax = 0;
    basil.spread = 12;

    chives.ID = 9;
    chives.minT = 45;
    chives.maxT = 95;
    chives.lightMinTime = 6;
    chives.soilWaterMin = 0;
    chives.soilWaterMax = 0;
    chives.spread = 12;

    mint.ID = 10;
    mint.minT = 60;
    mint.maxT = 75;
    mint.lightMinTime = 6;
    mint.soilWaterMin = 0;
    mint.soilWaterMax = 0;
    mint.spread = 15;

    parsley.ID = 11;
    parsley.minT = 60;
    parsley.maxT = 77;
    parsley.lightMinTime = 6;
    parsley.soilWaterMin = 0;
    parsley.soilWaterMax = 0;
    parsley.spread = 6;

}





