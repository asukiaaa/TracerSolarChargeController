#ifndef TracerSolarChargeController_h
#define TracerSolarChargeController_h
#include <Arduino.h>
#include <SoftwareSerial.h>

class TracerSolarChargeController {
  private:
  //static const uint8_t* start;
  uint8_t id;
  //static const uint8_t* cmd;
  uint8_t buff[128];
  float to_float(uint8_t* buffer, int offset);
  SoftwareSerial* my_serial;
  int my_serial_speed;

  public:
  TracerSolarChargeController(uint8_t tx_pin, uint8_t rx_pin);
  void update();
  void serial_out(HardwareSerial* serial);

  float battery;
  float battery_history[3];
  float pv;
  float load_current;
  float over_discharge;
  float battery_max;
  uint8_t full;
  uint8_t charging;
  int8_t battery_temp;
  float charge_current;
};

#endif
