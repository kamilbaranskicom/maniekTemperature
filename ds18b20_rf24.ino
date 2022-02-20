/*
   maniekTemperature
   +transmitter.

   checks temperatures and outputs info on serial port and on nRF24L01
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include "RF24.h"
#include "printf.h"

OneWire oneWire(7);                   // connected to pin 7
DallasTemperature sensors(&oneWire);  // Przekazania informacji do biblioteki
uint8_t deviceCount = 0;
uint8_t interval = 1;
boolean autoshow = true;
unsigned long previousMillis = 0;

DeviceAddress Thermometer;

const uint8_t radioAddress[6] = "manie";
RF24 radio(9, 8);

void setup(void) {
  Serial.begin(9600); //bylo: 9600
  Serial.println(F("maniekTemperature 1.1 + RF24 send"));

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    // while (1) {} // hold in infinite loop
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.enableDynamicPayloads();
  radio.openWritingPipe(radioAddress);
  radio.stopListening();

  // printf_begin();
  // radio.printDetails();
  // radio.printPrettyDetails();

  sensors.begin(); //Inicjalizacja czujnikow
  autoshow = EEPROM.read(0);
  interval = EEPROM.read(1);

  if (Serial.available()) {
    PrintHelp();
  }
}


void saveSettings() {
  EEPROM.write(0, autoshow);
  EEPROM.write(1, interval);
  Serial.println(F("Ustawienia zapisane"));
}

String formatAddressAsHex(DeviceAddress deviceAddress) {
  String address = "";
  uint8_t oneByte;
  char hexChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  for (uint8_t i = 0; i < 8; i++) {
    oneByte = deviceAddress[i];
    address += hexChars[(oneByte & 0xF0) >> 4];
    address += hexChars[oneByte & 0x0F];
  }
  return address;
}


void ShowTemp() {
  String message = "";
  String tmpString = "";

  for (uint8_t deviceNumber = 0; deviceNumber < deviceCount; deviceNumber++) {
    sensors.getAddress(Thermometer, deviceNumber);
    message += "sensor_" + formatAddressAsHex(Thermometer) + "=" + (String)sensors.getTempCByIndex(deviceNumber);
    if (deviceNumber < deviceCount - 1) {
      message += ";";
    }
  }
  message += "\r\n";
  Serial.print(message);

  for (unsigned int i = 0; i < message.length(); i = i + 32) {
    char tmp[33] = { 0 }; // 32 bytes + NUL
    tmpString = message.substring(i, i + 32);
    tmpString.toCharArray(tmp, tmpString.length() + 1);
    // Serial.print('#');
    // Serial.print(tmp);
    radio.write(&tmp, tmpString.length());
  }
}


void ShowAddr() {
  Serial.println(F("Printing addresses..."));
  for (uint8_t deviceNumber = 0; deviceNumber < deviceCount; deviceNumber++) {
    Serial.print("Sensor ");
    Serial.print(deviceNumber + 1);
    Serial.print(": ");
    sensors.getAddress(Thermometer, deviceNumber);
    Serial.print(formatAddressAsHex(Thermometer));
    Serial.println(";");
  }
  Serial.println("---");
}


void PrintHelp() {
  Serial.println(F("=========================================================="));
  Serial.println(F("Pomoc dla czujnika temperatury"));
  Serial.println(F("showtemp - pokazuje temperature"));
  Serial.println(F("showaddr - pokazuje adresy czujników temp. w kolejności"));
  Serial.println(F("start - automatyczne włączenie wypisywania temperatur"));
  Serial.println(F("stop - zatrzymanie automatycznego wypisywania temperatur"));
  Serial.println(F("delay=<s> - czas odczytów w tyrbie auto od 1 do 255 (sekund)"));
  Serial.println(F("save - zapisuje aktualne ustawienia"));
  Serial.println(F("=========================================================="));
}



// main
void loop(void) {
  deviceCount = sensors.getDeviceCount();
  //Serial.print(deviceCount, DEC);
  sensors.requestTemperatures(); //Pobranie temperatury czujnika

  if (autoshow == true) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= (interval * 1000)) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;
      ShowTemp();
    }
  }


  if (Serial.available() > 0) {
    String input = Serial.readString();

    if (input.indexOf("showtemp") >= 0)
      ShowTemp();

    if (input.indexOf("showaddr") >= 0)
      ShowAddr();

    if (input.indexOf("help") >= 0)
      PrintHelp();

    if (input.indexOf("save") >= 0)
      saveSettings();

    if (input.indexOf("stop") >= 0)
      autoshow = false;

    if (input.indexOf("start") >= 0)
      autoshow = true;

    if (input.indexOf("delay=") >= 0) {
      String d = input.substring(6, input.length());
      int nowydelay = d.toInt();
      if (nowydelay > 0 && nowydelay < 256) {
        interval = nowydelay;
        Serial.print(F("Ustawiono nowe opoxnienie na s:"));
        Serial.println(d);
      } else {
        Serial.println(F("Blad"));
      }
    }
  } // if (serial.available() > 0)
} // void loop()
