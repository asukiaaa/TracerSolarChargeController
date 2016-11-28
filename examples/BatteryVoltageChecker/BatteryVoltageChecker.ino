#include "TracerSolarChargeController.h"

TracerSolarChargeController chargeController(10, 11); // RX, TX

// For Arduino Leonardo etc
// TracerSolarChargeController chargeController(&Serial1);

void setup() {
  Serial.begin(57600);
}

void loop() {
  if (chargeController.update()) {
    chargeController.printInfo(&Serial);

    if ( chargeController.batteryVolt > 26.3 ) {
      Serial.println('Battery voltage is high!');
    } else if ( chargeController.batteryVolt < 24.0 ) {
      Serial.println('Battery voltage is low!');
    } else {
      Serial.println('Battery voltage is normal.');
    }

  } else {
    Serial.println("failed to update");
  }

  delay(5000);
}
