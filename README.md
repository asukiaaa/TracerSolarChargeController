# TracerSolarChargeController
This is a library to read data from EP Solar Tracer Solar Charge Controller with using Arduino.

# Connection between charge controller and arduino.
Tracer Solar Charge Controller has a LAN cable port.

Pins of the ports have the following role.

| Pins   | Role      |
| ------ | --------- |
| 1      | +12v      |
| 2      | GND       |
| 3      | +12V      |
| 4      | GND       |
| 5      | TXD(3.3V) |
| 6      | RXD(3.3V) |
| 7      | GND       |
| 8      | GND       |

You can connect the pins and arduino like this.
- +12v -> 3.3v regulator -> Arduino 3.3v
- GND -> Arduino GND
- TXD -> Arduino software serial RX pin
- RXD -> Arduino software serial TX pin

# Useage
## Include
```c
#include <SoftwareSerial.h>
#include "TracerSolarChargeController.h"
```
This library uses SoftwareSerial, so please include also that.

## Definition
```c
TracerSolarChargeController charge_controller(10, 11); // TX, RX
```
TX and RX pin numbers are needed to create an instance.

## Update
```c
charge_controller.update();
```
The instance communicates with charge controller and updates its values.

## Serial out
```c
charge_controller.serial_out(&Serial);
```
You can see wall values of charge controller via serial output.
````
Load is on
Load current: 0.02
Battery level: 25.07/29.16
Battery full: no
Temperature: 14
PV voltage: 0.30
Charging: no
Charge current: 0.00
````

## Public values
```c
float current_voltage = carge_controller.battery;
```
You can check public values.
```c
float   battery;            // Current battery voltage.
float   battery_history[3]; // History of battery voltage.
float   pv;                 // ?
float   load_current;       // ?
float   over_discharge;     // ?
float   battery_max;        // Voltage to stop charging by the controller.
uint8_t full;               // Return true if the battery is full.
uint8_t charging;           // Return true if the controller is charging.
int8_t  temp;               // Temperature of the controller.
float   charge_current;     // Voltage of the solar panel.
```

# Example
```c
#include <SoftwareSerial.h>
#include "TracerSolarChargeController.h"

TracerSolarChargeController charge_controller(10, 11); // TX, RX

void setup() {
  Serial.begin(57600);
}

void loop() {
  charge_controller.update();
  charge_controller.serial_out(&Serial);

  if ( charge_controller.battery > 26.3 ) {
    Serial.println('Battery voltage is high!')
  } else if ( charge_controller.battery < 24.0 ) {
    Serial.println('Battery voltage is low!')
  } else {
    Serial.println('Battery voltage is normal.')
  }

  delay(5000);
}
```

# License
MIT

# References
https://github.com/xxv/tracer
