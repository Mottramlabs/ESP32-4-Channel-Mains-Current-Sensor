/***************************************************************************************************************************/
/*
    A library test using either a MLP201185 or MLP201188 ESP32 4-Channel CT Sensor Board
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    16th May 2023
*/
/***************************************************************************************************************************/

// I/O items
#define Run_LED 4

// library for the ESP32 Boards (12-bit A/D)
#define AD_1 34          // GPIO pin for A/D input channel 1
#define AD_2 35          // GPIO pin for A/D input channel 2
#define AD_3 36          // GPIO pin for A/D input channel 3
#define AD_4 39          // GPIO pin for A/D input channel 4
#define Cal_value 6000//1500   // calibration value
#define AD_Samples 100//500  // A/D samples taken per channel read
#define Sample_rate 500   // delay in uS between A/D sample readings

/* Measured results (with an Owon HDS272S): 

Samples versus sample rate. Using 50Hz each AC cycles takes 20mS

Test 1: AD_Samples = 1000, Sample_rate = 10uS (result = OK)
96mS per channel input measurement sampling 4.8 AC cycles, 208 samples per AC cycle.

Test 2: AD_Samples = 400, Sample_rate = 100uS (result = a little better)
75mS per channel input measurement sampling 3.7 AC cycles, 108 samples per AC cycle.

Test 3: AD_Samples = 500, Sample_rate = 500uS (result = better)
298mS per channel input measurement sampling 14.9 AC cycles, 33 samples per AC cycle.

Test 4: AD_Samples = 400, Sample_rate = 1000uS (result = most stable)
436mS per channel input measurement sampling 21.8 AC cycles, 18 samples per AC cycle.

Looks like sampling more AC cycles even with fewer samples per cycle is the most stable (Test 4), but does take 0.4 seconds.
Test 3 looks like a good compromise between stability and sample read time.

*/

#include <ESP32_4CH_CT.h>
// make an instance of ESP32_4CH_CT
ESP32_4CH_CT My_PCB(AD_1, AD_2, AD_3, AD_4, AD_Samples, Sample_rate, Cal_value);

int8_t ADS_Input = 0;              // A/D channel select
double Value[4] = { 0, 0, 0, 0 };  // array for results

void setup() {

  // setup I/O
  pinMode(Run_LED, OUTPUT);

  // start the serial connection
  Serial.begin(115200);
  Serial.println("");
  Serial.println("");
  Serial.println("Up and Clackin!");
  Serial.println(__FILE__);

  // echo settings values sent to library
  digitalWrite(Run_LED, HIGH);
  My_PCB.report();
  delay(5000);
  digitalWrite(Run_LED, LOW);

}  // end of setup


void loop() {

  // read A/D values and store in array Value[]
  // these values are representations of Amps (RMS) measured, and still require some calibration
  digitalWrite(Run_LED, HIGH);
  // sampling each channel takes around 400mS. 400 samples (20 cycles @50Hz) with a 1mS per A/D sample.
  // higher sampling rates can have issues when WiFi enabled on the ESP8266
  (Value[ADS_Input] = My_PCB.power_sample(ADS_Input)) / 100;
  digitalWrite(Run_LED, LOW);

  // inc ready for next A/D channel
  ADS_Input++;
  if (ADS_Input > 3) {
    ADS_Input = 0;
  }  // end if

  // report results
  delay(100);

  String Report = String(Value[0]) + ", " + String(Value[1]) + ", " + String(Value[2]) + ", " + String(Value[3]) + "      ";
  Serial.println(Report);

}  // end of loop
