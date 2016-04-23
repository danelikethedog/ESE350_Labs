#include "Adafruit_DHT.h"

#include <math.h>



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



SYSTEM_MODE(AUTOMATIC);

void setup() {

    pinMode(led2, OUTPUT);
    pinMode(D0, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    digitalWrite(led2, LOW);
    Particle.function("flipLight", flipLight);
    dht.begin();

}


void loop() {
    /*
    int temp = analogRead(A0);
    float millivolts= (temp/4096.0) * 3300.0;
    float fahrenheit= (millivolts)/10;
    Serial.print(fahrenheit);
    Serial.println(" degrees Fahrenheit, ");

    int publishTemp = round(fahrenheit);

    
    float humidity = dht.getHumidity();
    Serial.print("Humidity is ");
    Serial.println(humidity);
    float humSensTemp = dht.getTempFarenheit();

    int humidityRound = round(humidity);




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
    */
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
    }
    updateValues();

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





