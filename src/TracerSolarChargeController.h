#ifndef TracerSolarChargeController_h
#define TracerSolarChargeController_h
#include <Arduino.h>
#include <SomeSerial.h>

#define TRACER_SERIAL_SPEED 9600

class TracerSolarChargeController {
  public:
  TracerSolarChargeController(HardwareSerial* hardSerial);
  TracerSolarChargeController(SoftwareSerial* softSerial);
  TracerSolarChargeController(uint8_t rx, uint8_t tx);
  void begin();
  bool update();
  void printInfo(HardwareSerial* serial);
#ifdef __USB_SERIAL_AVAILABLE__
  void printInfo(Serial_* serial);
#endif

  float batteryVolt;
  float panelVolt;
  float loadCurrent;
  float overDischarge;
  float batteryMax;
  uint8_t full;
  uint8_t charging;
  int8_t temp;
  float chargeAmp;

  private:
  void initValues();
  void printInfo(SomeSerial* serial);
  SomeSerial* thisSerial;

  //static const uint8_t* start;
  uint8_t id;
  //static const uint8_t* cmd;
  uint8_t buff[128];
  float toFloat(uint8_t* buffer, int offset);
};

#endif
