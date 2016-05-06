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

//Plant Structs
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

//House Variables
int houseTargetTemp;
int houseMinTemp;
int houseMaxTemp;



//Variables For Valves
int valveOne = D0;
int valveTwo = D1;
int valveThree = D2;

//Variables for PowerSwitches
int powerSwitchOne = D3;
int powerSwitchTwo = D4;
int powerSwitchThree = D5;

//Turn on and off WaterSensors
int wSensorIO = D6;

//Humidity Sensor Pins
int humOnePin = A5;
int humTwoPin = A6;

//Analog Mux Select
int selectZero = A0;
int selectOne = A1;
int selectTwo = A2;
int selectThree = A3;

//Sensor In from Mux
int inSensor = A4;

//Initiate DHT Humidity
DHT dht1(humOnePin,DHT11);
DHT dht2(humTwoPin,DHT11);

//Variables for Plot
int waterOne = 0;
int waterTwo = 0;
int waterThree = 0;
int waterFour = 0;
float lightOne = 0.0;
float lightTwo = 0.0;
int tempOne = 0;
int tempTwo = 0;
float humOne = 0.0;
float humTwo = 0.0;
float sunVal = 0.0;

int once = 0;
int curMinute = 0;

//Values to publish
int publishTemp = 0;
int publishHumidity = 0;
int publishLight = 0;

//Value for led toggle
int state = 0;

//Boolean to Know if IP Posted
bool ipSuccess = 0;




SYSTEM_MODE(AUTOMATIC);

void setup() {

    //Define Pins
    pinMode(valveOne, OUTPUT);
    pinMode(valveTwo, OUTPUT);
    pinMode(valveThree, OUTPUT);
    pinMode(powerSwitchOne, OUTPUT);
    pinMode(powerSwitchTwo, OUTPUT);
    pinMode(powerSwitchThree, OUTPUT);
    pinMode(wSensorIO, OUTPUT);
    pinMode(selectZero, OUTPUT);
    pinMode(selectOne, OUTPUT);
    pinMode(selectTwo, OUTPUT);
    pinMode(selectThree, OUTPUT);
    pinMode(inSensor, INPUT);


    //Publish Functions
    Particle.function("flipLight", flipLight);
    Particle.function("setPlant", setPlant);

    //Begin DHT Humidity Sensor and Set Plants
    dht1.begin();
    dht2.begin();
    definePlants();
    defaultPlants();

}


void loop() {

    if(!ipSuccess) {
        postCurIP();
    }

    minuteUpdate();

    delay(5000);

    updateValues();

    serialPrint();

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

void getTemps() {

    //Set Select to 0101 for tempOne
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, HIGH);
    digitalWrite(selectOne, LOW);
    digitalWrite(selectZero, HIGH);
    tempOne = analogRead(inSensor);
    tempOne = (tempOne/4096.0) * 3300.0;
    tempOne = (tempOne)/10;    

    getSecondTemp();
}

void getSecondTemp() {

    //Set Select to 1000 for tempTwo
    digitalWrite(selectThree, HIGH);
    digitalWrite(selectTwo, LOW);
    digitalWrite(selectOne, LOW);
    digitalWrite(selectZero, LOW);
    tempTwo = analogRead(inSensor);
    tempTwo = (tempTwo/4096.0) * 3300.0;
    tempTwo = (tempTwo)/10;
}

void getHumidities() {

    humOne = dht1.getHumidity();

    humTwo = dht2.getHumidity();

}

void getLights() {

    //Set Select to 0100 for lightOne
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, HIGH);
    digitalWrite(selectOne, LOW);
    digitalWrite(selectZero, LOW);
    lightOne = analogRead(inSensor);
    if (lightOne > sunVal) {
        sunVal = lightOne;
    }
    lightOne = lightOne * 100 / sunVal;


    //Set Select to 0111 for lightTwo
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, HIGH);
    digitalWrite(selectOne, HIGH);
    digitalWrite(selectZero, HIGH);
    lightTwo = analogRead(inSensor);
    if (lightTwo > sunVal) {
        sunVal = lightTwo;
    }
    lightTwo = lightTwo * 100 / sunVal;

}



void getWaters() {

    //Supply power to the sensors
    digitalWrite(wSensorIO, HIGH);

    //Set Select to 0000 for waterOne
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, LOW);
    digitalWrite(selectOne, LOW);
    digitalWrite(selectZero, LOW);
    waterOne = analogRead(inSensor);

    //Set Select to 0001 for waterTwo
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, LOW);
    digitalWrite(selectOne, LOW);
    digitalWrite(selectZero, HIGH);
    waterTwo = analogRead(inSensor);

    //Set Select to 0010 for waterThree
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, LOW);
    digitalWrite(selectOne, HIGH);
    digitalWrite(selectZero, LOW);
    waterThree = analogRead(inSensor);

    //Set Select to 0011 for waterFour
    digitalWrite(selectThree, LOW);
    digitalWrite(selectTwo, LOW);
    digitalWrite(selectOne, HIGH);
    digitalWrite(selectZero, HIGH);
    waterFour = analogRead(inSensor);
}

void updateValues() {

    getTemps();
    getHumidities();
    getLights();
    getWaters();

    /*
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
    */

}


/*------Functions for Setting Each Controller--------*/

void setFan(int io) {
    if (io) {
        digitalWrite(powerSwitchThree, HIGH);
    } else {
        digitalWrite(powerSwitchThree, LOW);
    }
}

void setLights(int io) {
    if (io) {
        digitalWrite(powerSwitchOne, HIGH);
    } else {
        digitalWrite(powerSwitchOne, LOW);
    }
}

void setValveOne(int io) {
    if (io) {
        digitalWrite(valveOne, HIGH);
    } else {
        digitalWrite(valveOne, LOW);
    }
}

void setValveTwo(int io) {
    if (io) {
        digitalWrite(valveTwo, HIGH);
    } else {
        digitalWrite(valveTwo, LOW);
    }
}

void setValveThree(int io) {
    if (io) {
        digitalWrite(valveThree, HIGH);
    } else {
        digitalWrite(valveThree, LOW);
    }
}



//Internet Stuffs
void sendDashWebhook() {
    char json[63];
    sprintf(json, "{\"t\": %i, \"h\": %i, \"l\": %i}", publishTemp, publishHumidity,publishLight);
    Particle.publish("curState", json);
}

void sendPlantWebhook() {
    char json[63];
    sprintf(json, "{\"I\": %i, \"II\": %i, \"III\": %i, \"IV\": %i}", 
                                                (*quadOne).ID, (*quadTwo).ID, (*quadThree).ID, (*quadFour).ID);
    Particle.publish("plants", json);
}

int postCurIP() {
    char addr[30];
    IPAddress localIP = WiFi.localIP();
    sprintf(addr, "%u.%u.%u.%u", localIP[0], localIP[1], localIP[2], localIP[3]);
    Spark.variable("Address", addr);
    return 1;
}








void serialPrint() {
    Serial.print("Temp: ");
    Serial.print(tempOne);
    Serial.print(" | Hum: ");
    Serial.print(humOne);
    Serial.print(" | Light: ");
    Serial.println(lightOne);


    Serial.print("Temp: ");
    Serial.print(tempTwo);
    Serial.print(" | Hum: ");
    Serial.print(humTwo);
    Serial.print(" | Light: ");
    Serial.println(lightTwo);
}

int flipLight(String arg) {
    state = !state;
    digitalWrite(valveOne, (state) ? HIGH : LOW);
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


void setHouseTemp() {

    int oneMid = (quadOne->maxT - quadOne->minT)/2;
    int twoMid = (quadTwo->maxT - quadTwo->minT)/2;
    int threeMid = (quadThree->maxT - quadThree->minT)/2;
    int fourMid = (quadFour->maxT - quadFour->minT)/2;
    int avgMid = (oneMid + twoMid + threeMid + fourMid)/4;
    houseTargetTemp = avgMid;

    int curMin = 150;
    if (curMin > quadOne->minT) {curMin = quadOne->minT;}
    if (curMin > quadTwo->minT) {curMin = quadTwo->minT;}
    if (curMin > quadThree->minT) {curMin = quadThree->minT;}
    if (curMin > quadFour->minT) {curMin = quadFour->minT;}
    houseMinTemp = curMin;

    int curMax = 0;
    if (curMax < quadOne->maxT) {curMax = quadOne->maxT;}
    if (curMax < quadTwo->maxT) {curMax = quadTwo->maxT;}
    if (curMax < quadThree->maxT) {curMax = quadThree->maxT;}
    if (curMax < quadFour->maxT) {curMax = quadFour->maxT;}
    houseMaxTemp = curMax;

}

void defaultPlants() {
    quadOne = &lettuce;
    quadTwo = &strawberry;
    quadThree = &tomato;
    quadFour = &bean;
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





