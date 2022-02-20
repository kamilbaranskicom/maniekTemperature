/*
   maniekTemperature
   receiver.

   prints everything it gets on the serial port.

*/

#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(9, 8); // using pin 9 for the CE pin, and pin 8 for the CSN pin
uint8_t address[6] = "manie";

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    // some boards need to wait to ensure access to serial over USB
  }

  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  radio.setPALevel(RF24_PA_LOW);    // RF24_PA_MAX is default.
  radio.setDataRate(RF24_250KBPS);  // that's enough.
  radio.enableDynamicPayloads();

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(0, address);

  radio.startListening();     // put radio in RX mode

  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data

} // setup

void loop() {

  char payload[32] = {0};

  uint8_t pipe;
  if (radio.available(&pipe)) {               // is there a payload? get the pipe number that received it
    uint8_t length = radio.getDynamicPayloadSize();  // or radio.getPayloadSize() for static payload sizes
    radio.read(&payload, length);

    // Serial.print(F("Received "));
    // Serial.print(length);
    // Serial.print(F(" bytes on pipe "));
    // Serial.print(pipe);                    // print the pipe number
    // Serial.print(F(": "));

    payload[length] = 0;                      // NUL character at the end
    Serial.print(payload);                    // print the payload's value
  }

} // loop
