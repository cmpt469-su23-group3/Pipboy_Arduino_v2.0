#include <Arduino.h>
#include "potentiometer.h"

float Pot_potPercentage(uint16_t value)
{
   return (float)value/1023;
}

void Pot_printPotInfo(uint16_t firstVal, uint16_t secondVal)
{
   float potOneVal = Pot_potPercentage(firstVal);
   float potTwoVal = Pot_potPercentage(secondVal);

   Serial.print("POT 1 Value: ");
   Serial.print(potOneVal);
   Serial.print(" | POT 2 Value: ");
   Serial.println(potTwoVal);
}
