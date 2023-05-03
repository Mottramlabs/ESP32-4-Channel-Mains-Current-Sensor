/*
    Title:        MQTT 4 Way Mains Power Sensor Demo
    Date:         31st August 2021
    Version:      1
    Description:  Sample code
    Device:       ESP8266
    WiFi:		  See "WiFi_Settings.h" for wifi AP and password
    MQTT:		  See "MQTT_Settings.h" for MQTT broker and topic
*/

/* ********************************** Compiler settings, un-comment to use ************************** */
//#define Fixed_IP                      // un-comment to use a fixed IP address to speed up development
//#ifdef Watchdog_ON                    // watchdog items, comment out if not used
//#define Print_Report_Level_1          // un-comment for option
#define Print_Report_Level_2          // un-comment for option, report received MQTT message
#define Print_Report_Level_3          // un-comment for option
/* ************************************************************************************************** */

#include <ESP8266WiFi.h>              // needed for EPS8266
#include <WiFiClient.h>               // WiFi client

// custom settings files
#include "Wifi_Settings.h"            // custom Wifi settings
#include "MQTT_Settings.h"            // MQTT broker and topic
#include "Project_Settings.h"         // board specific details.

// speaker
// https://github.com/Mottramlabs/ESP8266-Tone-Generator
#include <ESP8266_Tones.h>
ESP8266_Tones Mytone(Speaker);

// incude WiFi and MQTT functions
WiFiClient espClient;                 // for ESP8266 boards
#include <PubSubClient.h>             // http://pubsubclient.knolleary.net/api.html
PubSubClient client(espClient);       // ESP pubsub client
#include "WiFi_Functions.h"           // read wifi data
#include "MQTT_Functions.h"           // MQTT Functions

// library for the MLP201136 PCB
#include <MLP201185.h>
// make an instance of MLP201136
MLP201136 My_PCB(Select_A, Select_B, Cal_value);


void setup() {

#ifdef Watchdog_ON
  // watchdog items, comment out if not used
  secondTick.attach(1, ISRwatchdog);
#endif

  // I/O
  pinMode(Run_LED, OUTPUT);
  pinMode(Network_LED, OUTPUT);
  digitalWrite(Run_LED, LOW);
  digitalWrite(Network_LED, LOW);

  Mytone.Chirp(Speaker); pinMode(Speaker, OUTPUT); digitalWrite(Speaker, LOW);

  // connect to WiFi access point
  Get_Wifi();

  // connect to the MQTT broker
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // reset heartbeat timer
  LastMsg = millis();

} // end of setup


void loop() {

  if (!client.connected()) {
    reconnect();
  } // end if
  client.loop();

  // read A/D values and store in array Value[]
  // these values are representations of Amps (RMS) measured, and still require some calibration
  digitalWrite(Run_LED, LOW);
  // sampling each channel takes around 400mS. 400 samples (20 cycles @50Hz) with a 1mS per A/D sample.
  // higher sampling rates can have issues when WiFi enabled on the ESP8266
  Value[ADS_Input] = My_PCB.power_sample(ADS_Input);
  digitalWrite(Run_LED, HIGH);

  // inc ready for next A/D channel
  ADS_Input++;
  if (ADS_Input > 3) {
    ADS_Input = 0;
  } // end if

  // headbeat or report requested
  if (millis() - LastMsg > Heatbeat || Report_Request == 1) {

    LastMsg = millis();
    Report_Request = 0;

    // update event progress counter
    ++Heart_Value;
    if (Heart_Value > Heartbeat_Range) {
      Heart_Value = 1;
    } // end if

    // heartbeat timed out or report message requested

    // get a report make and make as an array
    String Report = Status_Report();
    char Report_array[(Report.length() + 1)];
    Report.toCharArray(Report_array, (Report.length() + 1));

#ifdef Print_Report_Level_2
    // display a report when publishing
    Serial.print("Published To topic: "); Serial.print(InStatus); Serial.print("  -  Report Sent: "); Serial.println(Report_array);
#endif

    digitalWrite(Network_LED, HIGH);
    // send a status report
    client.publish(InStatus, Report_array);

    // only used to make the LED flash visable
    delay(10);

    digitalWrite(Network_LED, LOW);

  } // end of heartbeat timer

} // end of loop
