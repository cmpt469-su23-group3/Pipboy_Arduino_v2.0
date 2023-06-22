#include <Arduino.h>
#include <ArduinoBLE.h>
#include "bluetooth.h"

static const char* POT_SERVICE_UUID = "bde52184-064d-11ee-be56-0242ac120002";
static const char* POT_ONE_CHARACTERISTIC_UUID = "579ff808-064e-11ee-be56-0242ac120002";
static const char* POT_TWO_CHARACTERISTIC_UUID = "5eba6056-064e-11ee-be56-0242ac120002";
static const char* POT_THREE_CHARACTERISTIC_UUID = "b0f7abf6-0e5c-11ee-be56-0242ac120002";

static BLEService Bt_potService(POT_SERVICE_UUID);
static BLEFloatCharacteristic Bt_potOneCharacteristic(POT_ONE_CHARACTERISTIC_UUID, BLERead | BLENotify);
static BLEFloatCharacteristic Bt_potTwoCharacteristic(POT_TWO_CHARACTERISTIC_UUID, BLERead | BLENotify);
static BLEFloatCharacteristic Bt_potThreeCharacteristic(POT_THREE_CHARACTERISTIC_UUID, BLERead | BLENotify);

static BLEDevice central;
static float potOneVal, potTwoVal, potThreeVal;

bool Bt_init(void)
{
   Serial.println("Initializing bluetooth...");
   if (!BLE.begin()) {
      Serial.println("Bluetooth failed to initiate. Will need a reset.");
      return false;
   }

   BLE.setLocalName("Arduino Pip-boy Project");
   BLE.setAdvertisedService(Bt_potService);
   Bt_potService.addCharacteristic(Bt_potOneCharacteristic);
   Bt_potService.addCharacteristic(Bt_potTwoCharacteristic);
   Bt_potService.addCharacteristic(Bt_potThreeCharacteristic);
   BLE.addService(Bt_potService);

   BLE.advertise();
   Serial.println("Bluetooth has now been advertised.");
   return true;
}

bool Bt_connect(void)
{
   central = BLE.central();
   if (central) {
      Serial.print("Connected to central device with MAC address: ");
      Serial.println(central.address());
      return true;
   }
   else {
      return false;
   }
}

bool Bt_isConnected(void)
{
   return central.connected();
}

void Bt_update(float newPotOneVal, float newPotTwoVal, float newPotThreeVal)
{
   potOneVal = newPotOneVal;
   potTwoVal = newPotTwoVal;
   potThreeVal = newPotThreeVal;

   Bt_potOneCharacteristic.writeValue(potOneVal);
   Bt_potTwoCharacteristic.writeValue(potTwoVal);
   Bt_potThreeCharacteristic.writeValue(potThreeVal);
}
