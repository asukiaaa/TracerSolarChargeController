#include "TracerSolarChargeController.h"

static const uint8_t tracerStart[] =
  { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
    0xEB, 0x90, 0xEB, 0x90, 0xEB, 0x90 };
static const uint8_t tracerCmd[] = { 0xA0, 0x00, 0xB1, 0xA7, 0x7F };

TracerSolarChargeController::TracerSolarChargeController(HardwareSerial* hardSerial) {
  thisSerial = new SomeSerial(hardSerial);
  initValues();
}

TracerSolarChargeController::TracerSolarChargeController(SoftwareSerial* softSerial) {
  thisSerial = new SomeSerial(softSerial);
  initValues();
}

TracerSolarChargeController::TracerSolarChargeController(uint8_t rx, uint8_t tx) {
  thisSerial = new SomeSerial(rx, tx);
  initValues();
}

void TracerSolarChargeController::initValues() {
  //start = tracerStart;
  id = 0x16;
  //cmd = tracerCmd;

  // init values
  batteryVolt = 0;
  panelVolt = 0;
}

void TracerSolarChargeController::begin() {
  thisSerial->begin(TRACER_SERIAL_SPEED);
}

float TracerSolarChargeController::toFloat(uint8_t* buffer, int offset){
  unsigned short full = buffer[offset+1] << 8 | buff[offset];
  return full / 100.0;
}


bool TracerSolarChargeController::update() {
  // Listen to thisSerial if it is SoftwareSerial
  if (thisSerial->isSoftwareSerial()) {
    thisSerial->thisSoftwareSerial->listen();
  }

  // Crear buffer
  if (thisSerial->available()) {
    thisSerial->read();
  }

  thisSerial->write(tracerStart, sizeof(tracerStart));
  thisSerial->write(id);
  thisSerial->write(tracerCmd, sizeof(tracerCmd));
  delay(100);

  int read = 0;
  int i;

  for ( i = 0; i < 255; i++) {
    if (thisSerial->available()) {
      buff[read] = thisSerial->read();
      read++;
      delay(2);
    }
  }

  if (read < 30) {
    return false;
  }

  batteryVolt = toFloat(buff, 9);
  panelVolt = toFloat(buff, 11);
  //13-14 reserved
  loadCurrent = toFloat(buff, 15);
  overDischarge = toFloat(buff, 17);
  batteryMax = toFloat(buff, 19);
  // 21 load on/off
  // 22 overload yes/no
  // 23 load short yes/no
  // 24 reserved
  // 25 battery overload
  // 26 over discharge yes/no
  full = buff[27];
  charging = buff[28];
  temp = buff[29] - 30;
  chargeAmp = toFloat(buff, 30);

  return true;
}

void TracerSolarChargeController::printInfo(HardwareSerial* serial) {
  // Causes memory overflow?
  printInfo(new SomeSerial(serial));
}

#ifdef __USB_SERIAL_AVAILABLE__
void TracerSolarChargeController::printInfo(Serial_* serial) {
  // Causes memory overflow?
  printInfo(new SomeSerial(serial));
}
#endif

void TracerSolarChargeController::printInfo(SomeSerial* serial) {
  serial->print("Load is ");
  serial->println(buff[21] ? "on" : "off");

  serial->print("Load current: ");
  serial->println(loadCurrent);

  serial->print("Battery level: ");
  serial->print(batteryVolt);
  serial->print("/");
  serial->println(batteryMax);

  serial->print("Battery full: ");
  serial->println(full ? "yes " : "no" );

  serial->print("Temperature: ");
  serial->println(temp);

  serial->print("Panel voltage: ");
  serial->println(panelVolt);

  serial->print("Charging: ");
  serial->println(charging ? "yes" : "no" );

  serial->print("Charge ampere: ");
  serial->println(chargeAmp);
}
