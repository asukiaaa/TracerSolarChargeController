#include "TracerSolarChargeController.h"

static const uint8_t tracer_start[] =
  { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,
    0xEB, 0x90, 0xEB, 0x90, 0xEB, 0x90 };
static const uint8_t tracer_cmd[] = { 0xA0, 0x00, 0xB1, 0xA7, 0x7F };

TracerSolarChargeController::TracerSolarChargeController(HardwareSerial* hardSerial) {
  my_serial = new SomeSerial(hardSerial);
  initValues();
}

TracerSolarChargeController::TracerSolarChargeController(SoftwareSerial* softSerial) {
  my_serial = new SomeSerial(softSerial);
  initValues();
}

TracerSolarChargeController::TracerSolarChargeController(uint8_t rx_pin, uint8_t tx_pin) {
  my_serial = new SomeSerial(rx_pin, tx_pin);
  initValues();
}

void TracerSolarChargeController::initValues() {
  //start = tracer_start;
  id = 0x16;
  //cmd = tracer_cmd;

  // init values
  battery = 0;
  for(int i = 0; i < 3; i++) {
    battery_history[i] = 0;
  }
}

void TracerSolarChargeController::begin() {
  my_serial->begin(TRACER_SERIAL_SPEED);
}

float TracerSolarChargeController::to_float(uint8_t* buffer, int offset){
  unsigned short full = buffer[offset+1] << 8 | buff[offset];
  return full / 100.0;
}


void TracerSolarChargeController::update() {
  if (my_serial->isSoftwareSerial()) {
    my_serial->thisSoftwareSerial->listen();
  }
  my_serial->write(tracer_start, sizeof(tracer_start));
  my_serial->write(id);
  my_serial->write(tracer_cmd, sizeof(tracer_cmd));

  int read = 0;
  int i;

  for ( i = 0; i < 255; i++) {
    if (my_serial->available()) {
      buff[read] = my_serial->read();
      read++;
    }
  }
  // Close each software serial after communication
  // to activate multiple software serial
  my_serial->end();

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
    battery_history[i] = battery_history[i-1];
    //Serial.print(i);
  }
  battery_history[0] = battery;

  battery = to_float(buff, 9);
  pv = to_float(buff, 11);
  //13-14 reserved
  load_current = to_float(buff, 15);
  over_discharge = to_float(buff, 17);
  battery_max = to_float(buff, 19);
  // 21 load on/off
  // 22 overload yes/no
  // 23 load short yes/no
  // 24 reserved
  // 25 battery overload
  // 26 over discharge yes/no
  full = buff[27];
  charging = buff[28];
  temp = buff[29] - 30;
  charge_current = to_float(buff, 30);

  //delay(1000);
}

void TracerSolarChargeController::serial_out(HardwareSerial* serial) {
  // Causes memory overflow?
  serial_out(new SomeSerial(serial));
}

#ifdef __USB_SERIAL_AVAILABLE__
void TracerSolarChargeController::serial_out(Serial_* serial) {
  // Causes memory overflow?
  serial_out(new SomeSerial(serial));
}
#endif


void TracerSolarChargeController::serial_out(SomeSerial* serial) {
  serial->print("Load is ");
  serial->println(buff[21] ? "on" : "off");

  serial->print("Load current: ");
  serial->println(load_current);

  serial->print("Battery level: ");
  serial->print(battery);
  serial->print("/");
  serial->println(battery_max);

  serial->print("Battery full: ");
  serial->println(full ? "yes " : "no" );

  serial->print("Temperature: ");
  serial->println(temp);

  serial->print("PV voltage: ");
  serial->println(pv);

  serial->print("Charging: ");
  serial->println(charging ? "yes" : "no" );

  serial->print("Charge current: ");
  serial->println(charge_current);
}
