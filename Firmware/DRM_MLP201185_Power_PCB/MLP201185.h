/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201136 - 4-channel CT sesnor boards
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    31st August 2021
*/
/***************************************************************************************************************************/

#ifndef MLP201136_Lib
#define MLP201136_Lib

// check of legacy IDE
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MLP201136
{
  public:

    MLP201136(int SelA, int SelB, double _ICAL);
    double power_sample(int mux_channel);

  private:

    int _Select_A;                                              // mux select pin
    int _Select_B;                                              // mux select pin

    #define Samples_Taken 400                                   // sample to be taken per channel, 1000 is approx 100mS or 5 cycles @ 50Hz
    int8_t ADS_Input = 0;                                       // A/D channel select
    const float Calibration = 1;                                // calibration value
    const float Volts_Conv = ((2.048 / 0.512) / Calibration);   // volts conversion value A/D 12-bit

    #define Mux_delay 500                                       // delay in uS to allow mux to settle
    #define AD_delay 1000                                       // A/D delay in uS 

    // new bits
    double ICAL;
    int sampleI;                                                // sample_ holds the raw analog read value

    double offsetI[4] = {320, 320, 320, 320} ;                  // low-pass filter output, one per channel. 320 is a good place to start
    double Irms;                                                // useful value variables
    double filteredI;                                           // filtered_ is the raw analog value minus the DC offset
    double sqI, sumI;                                           // sq = squared, sum = Sum
    #define SupplyVoltage 3.3                                   // supply voltage 3.3V
    #define ADC_COUNTS 1024                                     // 10-bit A/D

};
#endif
