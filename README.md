# TracerSolarChargeController
This is a library to read data from EP Solar Tracer Solar Charge Controller with using Arduino.

# Requirement
To use this library, [SomeSerial](https://github.com/asukiaaa/SomeSerial) library is needed.
Please install also that.

# Connection between charge controller and arduino.
Tracer Solar Charge Controller has a LAN cable port.

Pins of the ports have the following role.

| Pin num | Role      |
| ------- | --------- |
| 1       | +12v      |
| 2       | ?         |
| 3       | +12V      |
| 4       | GND       |
| 5       | TXD(3.3V) |
| 6       | RXD(3.3V) |
| 7       | GND       |
| 8       | GND       |

You can connect the pins and arduino like this.

For 5v Arduino.
- +12v -> 5v regulator -> Arduino VIN
- GND -> Arduino GND
- TXD -> Arduino RX
- /- Arduino TX (5v)<br>
  [ 1K register ]<br>
  |- RXD (3.3v)<br>
  [ 2K register ]<br>
  \\- GND<br>

For 3.3v Arduino.
- +12v -> 3.3v regulator -> Arduino VIN
- GND -> Arduino GND
- TXD -> Arduino RX
- RXD -> Arduino TX

# Useage
## Include
```c
#include "TracerSolarChargeController.h"
```

## Definition
### As SoftwareSerial
```c
TracerSolarChargeController chargeController(10, 11); // RX, TX
```

### As HardwareSerial
```c
TracerSolarChargeController chargeController(&Serial);
```

## Update
```c
if (chargeController.update()) {
  // succeeded process
} else {
  // failed process
}
```
The instance communicates with charge controller and updates its values.

## Print info
```c
chargeController.printInfo(&Serial);
```
You can see wall values of charge controller via serial output.
````
Load is on
Load current: 0.02
Battery level: 25.07/29.16
Battery full: no
Temperature: 14
Panel voltage: 0.30
Charging: no
Charge current: 0.00
````

## Public values
```c
float current_voltage = carge_controller.battery;
```
You can check public values.
```c
float   batteryVolt;   // Current battery voltage.
float   panelVolt;     // Current panel voltage.
float   loadCurrent;   // ?
float   overDischarge; // ?
float   batteryMax;    // Voltage to stop charging by the controller.
uint8_t full;          // Return true if the battery is full.
uint8_t charging;      // Return true if the controller is charging.
int8_t  temp;          // Temperature of the controller.
float   chargeAmp;     // Charging ampere.
```

# Example
```c
#include "TracerSolarChargeController.h"

TracerSolarChargeController chargeController(10, 11); // RX, TX

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
```

# License
MIT

# References
- https://github.com/xxv/tracer
- [Tracer-2210RN / Tracer-2215RN](http://www.epsolarpv.com/en/index.php/Product/pro_content/id/157/am_id/136)
