#ifndef TracerSolarChargeController_h
#define TracerSolarChargeController_h
#include <Arduino.h>
#include <SomeSerial.h>

#define TRACER_SERIAL_SPEED 9600

class TracerSolarChargeController {
  public:
  TracerSolarChargeController(HardwareSerial* hardSerial);
  TracerSolarChargeController(SoftwareSerial* softSerial);
  TracerSolarChargeController(uint8_t rx_pin, uint8_t tx_pin);
  void begin();
  void update();
  void serial_out(HardwareSerial* serial);
#ifdef __USB_SERIAL_AVAILABLE__
  void serial_out(Serial_* serial);
#endif

  float battery;
  float battery_history[3];
  float pv;
  float load_current;
  float over_discharge;
  float battery_max;
  uint8_t full;
  uint8_t charging;
  int8_t temp;
  float charge_current;

  private:
  void initValues();
  void serial_out(SomeSerial* serial);
  SomeSerial* my_serial;

  //static const uint8_t* start;
  uint8_t id;
  //static const uint8_t* cmd;
  uint8_t buff[128];
  float to_float(uint8_t* buffer, int offset);
};

#endif
