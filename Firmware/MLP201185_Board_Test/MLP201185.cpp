/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201185 - 4-channel ESP32 CT sesnor boards
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    25th March 2023
*/
/***************************************************************************************************************************/

#include "Arduino.h"
#include "MLP201185.h"

// -----------------------------------------------------------------------------------------------------------------------------
// constructor (A/D input 1, A/D input 2, A/D input 3, A/D input 4, A/D samples, delay in uS between samples, basic calibration)
// -----------------------------------------------------------------------------------------------------------------------------
MLP201185::MLP201185(int _AD_1, int _AD_2, int _AD_3, int _AD_4, int _Samples_Taken, int _AD_delay, double _ICAL) {

  AD_Channel_1 = _AD_1;
  AD_Channel_2 = _AD_2;
  AD_Channel_3 = _AD_3;
  AD_Channel_4 = _AD_4;
  Samples_Taken = _Samples_Taken;
  AD_delay = _AD_delay;
  ICAL = _ICAL;

}  // end of function


// report settings used
void MLP201185::report() {

  Serial.println("Report settings sent to library");
  Serial.println("-------------------------------");
  Serial.print("A/D Channel 1 = GPIO");
  Serial.println(AD_Channel_1);
  Serial.print("A/D Channel 2 = GPIO");
  Serial.println(AD_Channel_2);
  Serial.print("A/D Channel 3 = GPIO");
  Serial.println(AD_Channel_3);
  Serial.print("A/D Channel 4 = GPIO");
  Serial.println(AD_Channel_4);
  Serial.print("Samples = ");
  Serial.println(Samples_Taken);
  Serial.print("A/D delay(uS) = ");
  Serial.println(AD_delay);
  Serial.print("ICAL = ");
  Serial.println(ICAL);
  Serial.println("-------------------------------");

}  // end of function


/*
  added an offset per channel to improve channel seperation
*/
double MLP201185::power_sample(int Input_Channel) {

  // reset accumulators
  sumI = 0;

  // A/D engine
  for (unsigned int zz = 0; zz < Samples_Taken; zz++) {

    // select A/D input
    switch (Input_Channel) {
      case (0):
        // sample A/D value
        sampleI = analogRead(AD_Channel_1);
        break;
      case (1):
        // sample A/D value
        sampleI = analogRead(AD_Channel_2);
        break;
      case (2):
        // sample A/D value
        sampleI = analogRead(AD_Channel_3);
        break;
      case (3):
        // sample A/D value
        sampleI = analogRead(AD_Channel_4);
        break;
    }  //end selection

    // delay to slow A/D reading speed
    delayMicroseconds(AD_delay);

    // digital low pass filter extracts the 1.65 V (0-3.3V) dc offset, stored as offsetI[Input_Channel].
    // multichannel version - added is a seperate offset for each channel, this improves the filtering and channel seperation
    offsetI[Input_Channel] = (offsetI[Input_Channel] + (sampleI - offsetI[Input_Channel]) / 1024);

    // then subtract this - signal is now centered on 0 counts.
    filteredI = sampleI - offsetI[Input_Channel];

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum (add sqI to sumI)
    sumI += sqI;

  }  // end for samples loop

  /* measured
2.5V (10V) P/P sumI = 677394339.59
1.4V (5V) P/P sumI = 175530663.70
0.38V (1V) P/P sumI = 6810698.89
*/

  // ICAL=1500 - SupplyVoltage = 3.3V - ADC_COUNTS = 4096
  double I_RATIO = ICAL * (SupplyVoltage / ADC_COUNTS);

  /* measured
2.5V (10V) P/P I_RATIO = 4.83
1.4V (5V) P/P I_RATIO = 4.83
0.38V (1V) P/P I_RATIO = 4.83
*/

  // Number_of_Samples = 400 (default)
  Irms = I_RATIO * sqrt(sumI / Samples_Taken);

  /* measured
2.5V (10V) P/P Irms = 5654.06
1.4V (5V) P/P Irms =  2876.06
0.38V (1V) P/P  Irms = 566.72
*/

  //--------------------------------------------------------------------------------------
  return Irms;

}  // end of function
