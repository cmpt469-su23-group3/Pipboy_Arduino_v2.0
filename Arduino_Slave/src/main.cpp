#include <Arduino.h>
#include <Wire.h>
#include "potentiometer.h"

#define SLAVE_ID 0
#define PACKET_SIZE sizeof(float)*3

// From: https://forum.arduino.cc/t/union-and-struct-to-make-byte-array/155199
typedef struct {
   float potValOne;
   float potValTwo;
   float potValThree;
} PotVal_t;

typedef union {
   PotVal_t potVal;
   uint8_t bytes[PACKET_SIZE];
} I2C_Packet_t;

static void requestEvent(void);

void setup()
{
   Serial.begin(9600);
   Wire.begin(SLAVE_ID);
   Wire.onRequest(requestEvent);
}

void loop()
{
   delay(100);
}

static void requestEvent(void)
{
   Serial.println("Received request to send POT readings.");
   float potOneVal = Pot_potPercentage(analogRead(POT_ONE_PIN));
   float potTwoVal = Pot_potPercentage(analogRead(POT_TWO_PIN));
   float potThreeVal = Pot_potPercentage(analogRead(POT_THREE_PIN));

   PotVal_t potValues;
   potValues.potValOne = potOneVal;
   potValues.potValTwo = potTwoVal;
   potValues.potValThree = potThreeVal;

   I2C_Packet_t packet;
   packet.potVal = potValues;

   Wire.write(packet.bytes, sizeof(packet.bytes));

   // Blink the LED after writing
   digitalWrite(LED_BUILTIN, HIGH);
   delay(50);
   digitalWrite(LED_BUILTIN, LOW);
}
