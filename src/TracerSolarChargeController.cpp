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
  battery = 0;
  for(int i = 0; i < 3; i++) {
    batteryHistory[i] = 0;
  }
}

void TracerSolarChargeController::begin() {
  thisSerial->begin(TRACER_SERIAL_SPEED);
}

float TracerSolarChargeController::toFloat(uint8_t* buffer, int offset){
  unsigned short full = buffer[offset+1] << 8 | buff[offset];
  return full / 100.0;
}


void TracerSolarChargeController::update() {
  if (thisSerial->isSoftwareSerial()) {
    thisSerial->thisSoftwareSerial->listen();
  }
  thisSerial->write(tracerStart, sizeof(tracerStart));
  thisSerial->write(id);
  thisSerial->write(tracerCmd, sizeof(tracerCmd));

  int read = 0;
  int i;

  for ( i = 0; i < 255; i++) {
    if (thisSerial->available()) {
      buff[read] = thisSerial->read();
      read++;
    }
  }
  // Close each software serial after communication
  // to activate multiple software serial
  thisSerial->end();

  //Serial.print("Read ");
  //Serial.print(read);
  //Serial.println(" bytes");
  //for (int i = 0; i < read; i++){
  //   Serial.print(buff[i], HEX);
  //   Serial.print(" ");
  //}
  //Serial.println();

  // update history
  for ( i = 2; i > 0; i--) {
    batteryHistory[i] = batteryHistory[i-1];
    //Serial.print(i);
  }
  batteryHistory[0] = battery;

  battery = toFloat(buff, 9);
  pv = toFloat(buff, 11);
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
  chargeCurrent = toFloat(buff, 30);

  //delay(1000);
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
  serial->print(battery);
  serial->print("/");
  serial->println(batteryMax);

  serial->print("Battery full: ");
  serial->println(full ? "yes " : "no" );

  serial->print("Temperature: ");
  serial->println(temp);

  serial->print("PV voltage: ");
  serial->println(pv);

  serial->print("Charging: ");
  serial->println(charging ? "yes" : "no" );

  serial->print("Charge current: ");
  serial->println(chargeCurrent);
}
