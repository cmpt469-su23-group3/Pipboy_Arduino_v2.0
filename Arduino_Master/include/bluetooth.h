#include <Arduino.h>
#include <ArduinoBLE.h>

// Initializes the Bluetooth functionality on the Arduino.
// Returns true if initialized and false otherwise.
bool Bt_init(void);

// Function to connect to a central (master) device.
// Returns true when there is a device connected and false otherwise.
bool Bt_connect(void);

// Query that checks whether a central device is still connceted or not.
// Returns true if connected and false otherwise.
bool Bt_isConnected(void);

// Update the value of the potentiometers to send to the connected device.
void Bt_update(float newPotOneVal, float newPotTwoVal, float newPotThreeVal);
