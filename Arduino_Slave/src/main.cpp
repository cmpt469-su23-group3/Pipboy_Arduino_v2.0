#include <Arduino.h>
#include <Wire.h>
#include "potentiometer.h"

#define SLAVE_ID 0
#define PACKET_SIZE sizeof(float)*2

// From: https://forum.arduino.cc/t/union-and-struct-to-make-byte-array/155199
typedef struct {
   float potValOne;
   float potValTwo;
} PotVal_t;

typedef union {
   PotVal_t potVal;
   uint8_t bytes[PACKET_SIZE];
} I2C_Packet_t;

void requestEvent(void);

void setup()
{
   Wire.begin(SLAVE_ID);
   Wire.onRequest(requestEvent);
}

void loop()
{
}

void requestEvent(void)
{
   float potOneVal = Pot_potPercentage(analogRead(POT_ONE_PIN));
   float potTwoVal = Pot_potPercentage(analogRead(POT_TWO_PIN));

   PotVal_t potValues;
   potValues.potValOne = potOneVal;
   potValues.potValTwo = potTwoVal;

   I2C_Packet_t packet;
   packet.potVal = potValues;

   Wire.write(packet.bytes, sizeof(float));
}
