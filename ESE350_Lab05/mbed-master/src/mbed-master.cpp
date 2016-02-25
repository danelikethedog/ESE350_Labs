#include "mbed.h"
#include "MRF24J40.h"

#include <string>

// RF tranceiver to link with handheld.
MRF24J40 mrf(p11, p12, p13, p14, p21);

// LEDs you can treat these as variables (led2 = 1 will turn led2 on!)
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// Timer
Timer delayTimer;
Timer sendTimer;
Timer resetTimer;

// Serial port for showing RX data.
Serial pc(USBTX, USBRX);

// Used for sending and receiving
char txBuffer[128];
char rxBuffer[128];
int rxLen;
int moleReset;
int waitState;
int pause;
int score;


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

/*
    Codes to send are as follows
    *00 = off led;
    *01 = user turned led off;
    *11 = led on;
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

void randomMole()
{
    int random = rand() % 20;
    if (random < 5) {
        strcpy(txBuffer, "11");
        led4 = 1;
    } else {
        strcpy(txBuffer, "10");
        led4 = 0;
    }

}


//***************** You can start coding here *****************//
int main (void)
{
    uint8_t channel = 12;

    //Set the Channel. 0 is default, 15 is max
    mrf.SetChannel(channel);

    //Start the timer
    sendTimer.start();
    delayTimer.start();
    resetTimer.start();

    //Set Variables
    pause = 0;
    waitState = 1;

    while(true) {
        //Try to receive some data
        rxLen = rf_receive(rxBuffer, 128);
        if(rxLen > 0) {

            if (strcmp(rxBuffer, "101") == 0) {
                score++;
                waitState = 0;
                led3 = 0;
                delayTimer.reset();
            } else if (strcmp(rxBuffer, "110") == 0) {
                score--;
                waitState = 0;
                led3 = 0;
                delayTimer.reset();
            } else {
                if (resetTimer.read_ms() > 1500) {
                    waitState = 1;
                    pause = 0;
                }
                led3 = 1;
            }
            pc.printf("Received: %s\r\n", rxBuffer);
        }


        if (waitState) {
            if (pause){}
            else {
                randomMole();
                pause = 1;
                resetTimer.reset();
            }

        } else {
            if (delayTimer.read_ms() < 1000) {
            } else {
                waitState = 1;
                pause = 0;
            }
        }





        //Send some data every half second
        if(sendTimer.read_ms() >= 500) {

            moleReset++;
            //Reset the timer to 0
            sendTimer.reset();


            //Send the buffer
            rf_send(txBuffer, strlen(txBuffer) + 1);
            pc.printf("MSent: %s\r\n", txBuffer);
            pc.printf("Score: %i\r\n", score);
        }
    }
}
