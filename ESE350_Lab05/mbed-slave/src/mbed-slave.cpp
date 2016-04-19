#include "D:\Google Drive\School Work\ESE350\ESE350_Labs\ESE350_Lab05\mbed-slave\.pioenvs\lpc1768\FrameworkMbedInc-250686161\mbed.h"
#include "MRF24J40.h"
#include <string>

// RF tranceiver to link with handheld.
MRF24J40 mrf(p11, p12, p13, p14, p21);

// LEDs you can treat these as variables (led2 = 1 will turn led2 on!)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

AnalogIn lightSensor(p20);

// Timer
Timer wackTimer;
Timer sendTimer;

// Serial port for showing RX data.
Serial pc(USBTX, USBRX);

// Used for sending and receiving
char txBuffer[128];
char rxBuffer[128];
int rxLen;
int waitState;

//***************** Do not change these methods (please) *****************//

/**
* Receive data from the MRF24J40.
*
* @param data A pointer to a char array to hold the data
* @param maxLength The max amount of data to read.
*/
int rf_receive(char *data, uint8_t maxLength)
{
    uint8_t len = mrf.Receive((uint8_t *)data, maxLength);
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};

    if(len > 10) {
        //Remove the header and footer of the message
        for(uint8_t i = 0; i < len-2; i++) {
            if(i<8) {
                //Make sure our header is valid first
                if(data[i] != header[i])
                    return 0;
            } else {
                data[i-8] = data[i];
            }
        }

        //pc.printf("Received: %s length:%d\r\n", data, ((int)len)-10);
    }
    return ((int)len)-10;
}

/**
* Send data to another MRF24J40.
*
* @param data The string to send
* @param maxLength The length of the data to send.
*                  If you are sending a null-terminated string you can pass strlen(data)+1
*/
void rf_send(char *data, uint8_t len)
{
    //We need to prepend the message with a valid ZigBee header
    uint8_t header[8]= {1, 8, 0, 0xA1, 0xB2, 0xC3, 0xD4, 0x00};
    uint8_t *send_buf = (uint8_t *) malloc( sizeof(uint8_t) * (len+8) );

    for(uint8_t i = 0; i < len+8; i++) {
        //prepend the 8-byte header
        send_buf[i] = (i<8) ? header[i] : data[i-8];
    }
    //pc.printf("Sent: %s\r\n", send_buf+8);

    mrf.Send(send_buf, len+8);
    free(send_buf);
}


//***************** You can start coding here *****************//
int main (void)
{
    uint8_t channel = 12;

    //Set the Channel. 0 is default, 15 is max
    mrf.SetChannel(channel);

    led4=1;


    /*
    //Start the timer
    sendTimer.start();
    wackTimer.start();

    //Set waitState initially to 0
    waitState = 0;

    while(true) {

        //Try to receive some data
        rxLen = rf_receive(rxBuffer, 128);
        if(rxLen > 0) {
            //Toggle the Led
            //led1 = led1^1;
            if (strcmp(rxBuffer, "11") == 0) {
                led4 = 1;
            } else {
                led4 = 0;
            }
            pc.printf("Received: %s\r\n", rxBuffer);
        }

        //Either got command to turn on or waiting
        if (waitState) {
            //Set timer of 1.25 seconds to cover
            if (wackTimer.read_ms() < 1250) {
                if (lightSensor.read() < 0.5f) {
                    //User was able to cover
                    strcpy(txBuffer, "101");
                    led4 = 0;
                    led2 = 1;
                    waitState = 0;
                }
            } else {
                wackTimer.reset();
                //User missed cover
                strcpy(txBuffer, "110");
                waitState = 0;
                led2 = 0;
            }
        } else {
            if (strcmp(rxBuffer, "11") == 0) {
                //Got a signal to turn LED on and commence wait
                led4 = 1;
                strcpy(txBuffer, "111");
                waitState = 1;
                wackTimer.reset();
            } else if (strcmp(rxBuffer, "10") == 0) {
                //Not turn to turn on;
                led4 = 0;
                strcpy(txBuffer, "100");
            }
        }

        //Send some data every quarter second
        if(sendTimer.read_ms() >= 250) {
            //Reset the timer to 0
            sendTimer.reset();

            //Send the buffer
            rf_send(txBuffer, strlen(txBuffer) + 1);
            //pc.printf("SSent: %s\r\n", txBuffer);
            pc.printf("Analog: %1.4f\r\n", lightSensor.read());

        }
    } */
}
