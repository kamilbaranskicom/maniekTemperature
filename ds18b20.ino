#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

// Arduino Nano, ATmega328P (Old Bootloader), COM3.

OneWire oneWire(10);
DallasTemperature sensors(&oneWire); //Przekazania informacji do biblioteki
int deviceCount = 0;
int interval = 1;
boolean autoshow = true;
unsigned long previousMillis = 0;

DeviceAddress Thermometer;
void setup(void) {
  Serial.begin(9600);
  sensors.begin(); //Inicjalizacja czujnikow
  autoshow = EEPROM.read(0);
  interval = EEPROM.read(1);

  if (Serial.available())
    PrintHelp();
}


void saveSettings()
{
  EEPROM.write(0, autoshow);
  EEPROM.write(1, interval);
  Serial.println(F("Ustawienia zapisane"));
}


// funkcje
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    //Serial.print("0x");
    if (deviceAddress[i] < 0x10) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
    // if (i < 7) Serial.print(", ");
  }
}


void ShowTemp()
{
  for ( int i = 0; i < deviceCount; i++)
  {
    Serial.print("sensor_");
    sensors.getAddress(Thermometer, i);
    printAddress(Thermometer);
    Serial.print("=");
    Serial.print(sensors.getTempCByIndex(i));
    if (i < deviceCount - 1)
      Serial.print(";");
  }
  Serial.println();
}


void ShowAddr()
{
  Serial.println("Printing addresses...");
  for (int i = 0;  i < deviceCount;  i++)
  {
    Serial.print("Sensor ");
    Serial.print(i + 1);
    Serial.print(": ");
    sensors.getAddress(Thermometer, i);
    printAddress(Thermometer);
    Serial.println(";");
  }
}


void PrintHelp()
{
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

  if (autoshow == true)
  {
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
    if (input.indexOf("delay=") >= 0)
    {
      String d = input.substring(6, input.length());
      int nowydelay = d.toInt();
      if (nowydelay > 0 && nowydelay < 256)
      {
        interval = nowydelay;
        Serial.print(F("Ustawiono nowe opoxnienie na s:"));
        Serial.println(d);
      } else Serial.println(F("Blad"));
    }
  }


}
