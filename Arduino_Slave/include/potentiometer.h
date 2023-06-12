#include <Arduino.h>

#define POT_ONE_PIN A0
#define POT_TWO_PIN A1

// Function for maping the output of analogRead (0 to 1023)
// to a percentage value.
float Pot_potPercentage(uint16_t value);

// Function to print pot info over the serial interface.
void Pot_printPotInfo(uint16_t firstVal, uint16_t secondVal);
