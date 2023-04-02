/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201185 - 4-channel ESP32 CT sesnor boards
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    25th March 2023
*/
/***************************************************************************************************************************/

#ifndef MLP201185_Lib
#define MLP201185_Lib

// check of legacy IDE
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MLP201185
{
  public:

    MLP201185(int _AD_1, int _AD_2, int _AD_3, int _AD_4, int _Samples_Taken, int _AD_delay, double _ICAL);
    void report();
    double power_sample(int Input_Channel);

  private:

    // ESP32 A/D inputs
    int AD_Channel_1 = 1;                                       // A/D Input GPIO Pin
    int AD_Channel_2 = 1;                                       // A/D Input GPIO Pin
    int AD_Channel_3 = 1;                                       // A/D Input GPIO Pin
    int AD_Channel_4 = 1;                                       // A/D Input GPIO Pin

    int Samples_Taken = 1;                                      // sample to be taken per channel, 1000 is approx 100mS or 5 cycles @ 50Hz
    int8_t ADS_Input = 0;                                       // A/D channel select
    const float Calibration = 1;                                // calibration value
    const float Volts_Conv = ((2.048 / 0.512) / Calibration);   // volts conversion value A/D 12-bit

    int AD_delay = 1;                                           // A/D delay in uS 

    // new bits
    double ICAL;
    int sampleI;                                                // sample_ holds the raw analog read value

    double offsetI[4] = {320, 320, 320, 320} ;                  // low-pass filter output, one per channel. 320 is a good place to start
    double Irms;                                                // useful value variables
    double filteredI;                                           // filtered_ is the raw analog value minus the DC offset
    double sqI, sumI;                                           // sq = squared, sum = Sum
    #define SupplyVoltage 3.3                                   // supply voltage 3.3V
    #define ADC_COUNTS 4096                                     // 12-bit A/D

};
#endif
