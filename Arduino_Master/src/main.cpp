#include <Arduino.h>
#include <Wire.h>
#include "bluetooth.h"

#define SLAVE_ID 0
#define PACKET_SIZE sizeof(float)*2

static void blinkBuiltInLedSuccess(void);
static void blinkBuiltInLedFail(void);
static void unpackI2CBytes(uint8_t *bytesToUnpack, uint8_t *unpackOne, uint8_t *unpackTwo);
static float bytesToFloat(uint8_t *bytesOfFloat);

static float potOneVal, potTwoVal;
static bool potValUpdated;

void setup()
{
   pinMode(LED_BUILTIN, OUTPUT);

   Serial.begin(9600);
   delay(2500);

   Serial.println("Entering setup function.");
   if (!Bt_init()) {
      // busy wait indefinitely if Bluetooth init fails
      while (1) { 
         blinkBuiltInLedFail();
      }
   }

   // Notify that setup function successfully run
   blinkBuiltInLedSuccess();

   potOneVal = 0;
   potTwoVal = 0;
   potValUpdated = false;
}

void loop()
{
   if (Bt_connect()) { // note that this call is non-blocking
      delay(200);
      while (Bt_isConnected()) {
         uint8_t potOneValBuf[sizeof(float)]; // this is 4 bytes in an Arduino
         uint8_t potTwoValBuf[sizeof(float)];
         uint8_t i2cBuf[PACKET_SIZE];

         Wire.requestFrom(SLAVE_ID, PACKET_SIZE);
         uint8_t bytePos = 0;
         while (Wire.available()) {
            byte rxByte = Wire.read();
            i2cBuf[bytePos] = rxByte;
            potValUpdated = true;
            bytePos++;
         }

         if (potValUpdated) {
            unpackI2CBytes(i2cBuf, potOneValBuf, potTwoValBuf);
            potOneVal = bytesToFloat(potOneValBuf);
            potTwoVal = bytesToFloat(potTwoValBuf);
         }

         Bt_update(potOneVal, potTwoVal);
         potValUpdated = false;
      }
      Serial.println("Disconnected from central device.");
   }
}

static void blinkBuiltInLedSuccess(void)
{
   for (uint8_t timesToBlink = 0; timesToBlink < 2; timesToBlink++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(200);
      digitalWrite(LED_BUILTIN, LOW);
      delay(200);
   }
}

static void blinkBuiltInLedFail(void)
{
   digitalWrite(LED_BUILTIN, HIGH);
   delay(1000);
   digitalWrite(LED_BUILTIN, LOW);
   delay(1000);
}

static void unpackI2CBytes(uint8_t *bytesToUnpack, uint8_t *unpackOne, uint8_t *unpackTwo)
{
    // Bytes 0-3 go to unpackOne, bytes 4-7 go to unpackTwo
    for (uint8_t arrPos = 0; arrPos < 4; arrPos++) {
       unpackOne[arrPos] = bytesToUnpack[arrPos];
    }
    for (uint8_t arrPos = 4; arrPos < 8; arrPos++) {
        unpackTwo[arrPos % 4] = bytesToUnpack[arrPos];
    }
}

static float bytesToFloat(uint8_t *bytesOfFloat)
{
    float toReturn;
    uint8_t *toReturnPtr = (uint8_t*)&toReturn;
    
    // Arduino runs on little endian
    for (uint8_t pos = 0; pos < 4; pos++) {
        toReturnPtr[pos] = bytesOfFloat[pos];
    }
    
    return toReturn;
}
