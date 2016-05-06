// This #include statement was automatically added by the Particle IDE.
#include "SparkJson/SparkJson.h"

// This #include statement was automatically added by the Particle IDE.
#include "HttpClient/HttpClient.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_DHT/Adafruit_DHT.h"

#include <math.h>
#include <sstream>

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

//Value for led toggle
int state = 0;



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
    
    Particle.function("setLights", setLights);
    /*Particle.function("setWater1", setValveOne);
    Particle.function("setWater2", setValveTwo);
    Particle.function("setWater3", setValveThree); */

    //Begin DHT Humidity Sensor and Set Plants
    dht1.begin();
    dht2.begin();
}


void loop() {
	/*
    if(!ipSuccess) {
        postCurIP();
    }

    minuteUpdate();

    delay(5000);

    updateValues();

    serialPrint(); */
    Serial.print("test");
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

int setLights(String io) {
    if (io) {
        digitalWrite(powerSwitchOne, HIGH);
    } else {
        digitalWrite(powerSwitchOne, LOW);
    }
    return 1;
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
/*
void setHouseTemp() {

    int oneMid = (->maxT - quadOne->minT)/2;
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

}*/