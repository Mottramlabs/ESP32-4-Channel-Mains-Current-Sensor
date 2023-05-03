/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201136 - 4-channel CT sesnor boards
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    31st August 2021
*/
/***************************************************************************************************************************/

#include "Arduino.h"
#include "MLP201136.h"

// ------------------------------------------------------------------------------------------------------------------------
// constructor
// ------------------------------------------------------------------------------------------------------------------------
MLP201136::MLP201136(int SelA, int SelB, double _ICAL) {

  // local variables
  _Select_A = SelA;
  _Select_B = SelB;

  pinMode(_Select_A, OUTPUT); digitalWrite(_Select_A, LOW);
  pinMode(_Select_B, OUTPUT); digitalWrite(_Select_B, LOW);

  ICAL = _ICAL;

} // end of function


/*
  Added an offset per channel to improve channel seperation
*/
double MLP201136::power_sample(int mux_channel) {

  // select mux and continuous conversion
  switch (mux_channel) {
    case (0):
      digitalWrite(_Select_A, LOW);
      digitalWrite(_Select_B, LOW);
      break;
    case (1):
      digitalWrite(_Select_A, LOW);
      digitalWrite(_Select_B, HIGH);
      break;
    case (2):
      digitalWrite(0, HIGH);
      digitalWrite(_Select_A, HIGH);
      digitalWrite(_Select_B, LOW);
      break;
    case (3):
      digitalWrite(_Select_A, HIGH);
      digitalWrite(_Select_B, HIGH);
      break;
  } //end selection

  // delay to clear the mux
  delayMicroseconds(Mux_delay);

  // reset accumulators
  sumI = 0;


  // A/D engine
  for (unsigned int zz = 0; zz < Samples_Taken; zz++) {

    // sample A/D value
    sampleI = analogRead(A0);

    // delay to slow A/D reading speed
    delayMicroseconds(AD_delay);

    // digital low pass filter extracts the 1.65 V (0-3.3V) dc offset, stored as offsetI[mux_channel].
    // multichannel version - added is a seperate offset for each channel, this improves the filtering and channel seperation
    offsetI[mux_channel] = (offsetI[mux_channel] + (sampleI - offsetI[mux_channel]) / 1024);

    // then subtract this - signal is now centered on 0 counts.
    filteredI = sampleI - offsetI[mux_channel];

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum (add sqI to sumI)
    sumI += sqI;

  } // end for samples loop


  // ICAL=1500 - SupplyVoltage = 3.3V - ADC_COUNTS = 1024
  double I_RATIO = ICAL * (SupplyVoltage / ADC_COUNTS);

  // Number_of_Samples = 400
  Irms = I_RATIO * sqrt(sumI / Samples_Taken);

  //--------------------------------------------------------------------------------------

  return Irms;

} // end of function
