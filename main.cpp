#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TD5580A.h>

/*
 * Connected to ESP32-WROOM device
 *  using pins 16 and 17
 */
uint8_t dfpRxdPin = 16;
uint8_t dfpTxdPin = 17;

SoftwareSerial dfpSerial(dfpRxdPin, dfpTxdPin);
TD5580A dfp;

void setup() {
  Serial.begin(115200);     // start USB serial interface
  Serial.println("Entered setup()...");

  dfp.showDebug(true);      // print what we are sending to the dfplayer board.

  Serial.println("Starting DFPlayer serial comms...");
  dfpSerial.begin(9600, SWSERIAL_8N1);        // start dfplayer serial port
  delay(2500);                                // wait for init

  Serial.println("Beginning DFPlayer serial comms...");
  dfp.begin(dfpSerial);     // begin dfp serial communication
  delay(700);

  //Serial.println("Resetting DFPlayer processor...");
  //dfp.reset();              // reset the dfp device 
  //delay(700);               // - causes an audible pop

  Serial.println("Selecting SD card as source...");
  dfp.setSource(2);         //select sd-card
  delay(700);

  Serial.println("Setting volume to 20...");
  dfp.setVol(20);
  delay(700);

  byte tknum = 3;
  Serial.println("Playing track # " + String(tknum));
  dfp.play(tknum);
  delay(0);
}

void loop() {
    delay(0);
}
