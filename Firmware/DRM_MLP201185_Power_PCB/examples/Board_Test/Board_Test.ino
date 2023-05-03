#include <WiFiClient.h>               // WiFi client

// I/O items
#define Network_LED 14
#define Run_LED 16
#define Speaker 13

// sound library
// https://github.com/Mottramlabs/ESP8266-Tone-Generator
//#include <ESP8266_Tones.h>
//ESP8266_Tones Mytone(Speaker);

// mux connections
#define Select_A 4
#define Select_B 5
#define Cal_value 1500

// library for the MLP201136 PCB
#include "MLP201185.h"
// make an instance of MLP201136
MLP201136 My_PCB(Select_A, Select_B, Cal_value);

int8_t ADS_Input = 0;                               // A/D channel select
double Value[4] = {0, 0, 0, 0};                     // array for results

void setup() {

  // start the serial connection
  Serial.begin(115200); Serial.println(""); Serial.println(""); Serial.println("Up and Clackin!");
  Serial.println(__FILE__);

  // setup I/O
  pinMode(Run_LED, OUTPUT); digitalWrite(Run_LED, HIGH);
  pinMode(Network_LED, OUTPUT); digitalWrite(Network_LED, HIGH);

  Mytone.Tone_Up(Speaker);

} // end of setup


void loop() {

  // read A/D values and store in array Value[]
  // these values are representations of Amps (RMS) measured, and still require some calibration
  digitalWrite(Run_LED, LOW);
  // sampling each channel takes around 400mS. 400 samples (20 cycles @50Hz) with a 1mS per A/D sample.
  // higher sampling rates can have issues when WiFi enabled on the ESP8266
  (Value[ADS_Input] = My_PCB.power_sample(ADS_Input)) / 100;
  digitalWrite(Run_LED, HIGH);

  // inc ready for next A/D channel
  ADS_Input++;
  if (ADS_Input > 3) {
    ADS_Input = 0;
  } // end if

  // report results
  delay(100);

  // print the values
  digitalWrite(Network_LED, HIGH);

  String Report = String(Value[0]) + ", " + String(Value[1]) + ", " + String(Value[2]) + ", " + String(Value[3]) + "      ";
  Serial.println(Report);

  // only used to make the LED flash visable
  delay(10);

  digitalWrite(Network_LED, LOW);

} // end of loop
