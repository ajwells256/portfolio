#include <math.h>

/*
  [Ground] -- [50k-resistor] -- | -- [(503)thermistor] --[Vcc (5v)]
                              |
                         Analog Pin 0
*/

#define ANALOGUE_READ 7

#define EPISCO_K164_10k 4300.0f,298.15f,50000.0f  // B,T0,R0  

float Temperature(float B, float T0, float R0, float R_Balance)
{
  float R, T;

  //  R=1024.0f*R_Balance/float(analogRead(AnalogInputNumber)))-R_Balance;
  R = R_Balance * (1024.0f / (float)analogRead(ANALOGUE_READ) - 1);

  T = 1.0f / (1.0f / T0 + (1.0f / B) * log(R / R0));

  // celcius
  T -= 273.15f;
  return T;
}

//example of use #2
// using numbers instead of episco k164 definition
// this time reading from analog input 2
// getting result in fahrenheit
