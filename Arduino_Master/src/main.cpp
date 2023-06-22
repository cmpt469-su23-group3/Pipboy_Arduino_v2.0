#include <Arduino.h>
#include <Wire.h>
#include "bluetooth.h"

#define SLAVE_ID 0
#define PACKET_SIZE sizeof(float)*3

static void blinkBuiltInLedSuccess(void);
static void blinkBuiltInLedFail(void);
static void unpackI2CBytes(uint8_t *bytesToUnpack, uint8_t *unpackOne, uint8_t *unpackTwo, uint8_t *unpackThree);
static float bytesToFloat(uint8_t *bytesOfFloat);

static float potOneVal, potTwoVal, potThreeVal;
static bool potValUpdated;

void setup()
{
   pinMode(LED_BUILTIN, OUTPUT);

   Wire.begin();
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
         uint8_t potThreeValBuf[sizeof(float)];
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
            unpackI2CBytes(i2cBuf, potOneValBuf, potTwoValBuf, potThreeValBuf);
            potOneVal = bytesToFloat(potOneValBuf);
            potTwoVal = bytesToFloat(potTwoValBuf);
            potThreeVal = bytesToFloat(potThreeValBuf);
         }

         Bt_update(potOneVal, potTwoVal, potThreeVal);
         potValUpdated = false;
      }
      Serial.println("Disconnected from central device.");
   }

   // uint8_t potOneValBuf[sizeof(float)]; // this is 4 bytes in an Arduino
   // uint8_t potTwoValBuf[sizeof(float)];
   // uint8_t potThreeValBuf[sizeof(float)];
   // uint8_t i2cBuf[PACKET_SIZE];

   // Wire.requestFrom(SLAVE_ID, PACKET_SIZE);
   // uint8_t bytePos = 0;
   // while (Wire.available()) {
   //    byte rxByte = Wire.read();
   //    i2cBuf[bytePos] = rxByte;
   //    potValUpdated = true;
   //    bytePos++;
   // }

   // if (potValUpdated) {
   //    unpackI2CBytes(i2cBuf, potOneValBuf, potTwoValBuf, potThreeValBuf);
   //    potOneVal = bytesToFloat(potOneValBuf);
   //    potTwoVal = bytesToFloat(potTwoValBuf);
   //    potThreeVal = bytesToFloat(potThreeValBuf);

   //    Serial.print("Pot 1: ");
   //    Serial.print(potOneVal);
   //    Serial.print(" | Pot 2: ");
   //    Serial.print(potTwoVal);
   //    Serial.print(" | Pot 3: ");
   //    Serial.println(potThreeVal);
   // }

   // potValUpdated = false;
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

static void unpackI2CBytes(uint8_t *bytesToUnpack, uint8_t *unpackOne, uint8_t *unpackTwo, uint8_t *unpackThree)
{
    // Bytes 0-3 go to unpackOne, bytes 4-7 go to unpackTwo, bytes 8-11 go to unpackThree
    for (uint8_t arrPos = 0; arrPos < 4; arrPos++) {
       unpackOne[arrPos] = bytesToUnpack[arrPos];
    }
    for (uint8_t arrPos = 4; arrPos < 8; arrPos++) {
       unpackTwo[arrPos % 4] = bytesToUnpack[arrPos];
    }
    for (uint8_t arrPos = 8; arrPos < 12; arrPos++){
       unpackThree[arrPos % 4] = bytesToUnpack[arrPos];
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
