/*
 * ATtiny85-SSD1306-Spectrum v1.0
 *
 * Copyright (c) 2021 nori.dev.akg
 *
 * Released under the MIT license.
 *
 * <Special Thanks>
 *   Original Project: @colonelwatch
 *    attiny85-spectrum: https://github.com/colonelwatch/attiny85-spectrum
 * 
 *   SSD1306 Library @SASAPEA’S LAB
 *     https://lab.sasapea.mydns.jp/2018/07/28/graphics85-2/
 * 
 *   Fix_fft library @kosme
 *     https://github.com/kosme/fix_fft
 * 
*/

#include "fix_fft.h" // 8-bit FFT library modified for Arduino
#include "USITWI.h"
#include "SSD1306.h"

// These are user-adjustable
//#define LOG_OUTPUT              // Uncomment to enable logarithmic output (exchanges absolute resoluton for more readable output; may require different below params)
#define SAMPLING_FREQUENCY 15000 // Sampling frequency (Actual max measured frequency captured is half)
#define TIME_FACTOR 2            // Smoothing factor (lower is more dynamic, higher is smoother) ranging from 1 to 10+ default:3
#define SCALE_FACTOR 8           // Direct scaling factor (raise for higher bars, lower for shorter bars) default:8

#ifdef LOG_OUTPUT
const float log_scale = 64. / log(64. / SCALE_FACTOR + 1.); // Attempts to create an equivalent to SCALE_FACTOR for log function
#endif
const float coeff = 1. / TIME_FACTOR; // Time smoothing coefficients (used to factor in previous data)
const float anti_coeff = (TIME_FACTOR - 1.) / TIME_FACTOR;
const unsigned int sampling_period_us = round(1000000 * (2.0 / SAMPLING_FREQUENCY)); // Sampling period (doubled to account for overclock)

int8_t data[64], buff[32], peak[32]; // used to store FFT input/output and past data
unsigned long microseconds;          // used for timekeeping
int summ, avg;                       // used for DC bias elimination

const char SPLASH_TITLE[] = "SPECTRUM SPEAKER\n @nori-dev-akg";

// Tiny85
#define OLED_ADDR 0x3C
#define TWI_BUS USITWI<USITWI_400K>
TWI_BUS bus;
SSD1306<TWI_BUS, bus, OLED_ADDR> lcd;

// peak bar
unsigned long t0;

void setup()
{
  OSCCAL = 250; // Overclocks the MCU to around 30 MHz, set lower if this causes instability, raise if you can/want

  ADCSRA &= ~(bit(ADPS0) | bit(ADPS1) | bit(ADPS2)); // clear ADC prescaler bits
  ADCSRA |= bit(ADPS2);                              // sets ADC clock in excess of 10kHz
  ADCSRA |= bit(ADPS0);

  // Tiny85
  bus.begin();
  lcd.begin();
  lcd.clear();

  // splash screen
  lcd.cursor(0, 0);
  lcd.printStr((char*)SPLASH_TITLE);
  lcd.flush();

  delay(10000);
  lcd.clear();

  t0 = millis();
};

void loop()
{
  summ = 0;
  for (int i = 0; i < 64; i++)
  {
    microseconds = micros();

    int val = analogRead(A3); // * 53;

    data[i] = (val >> 2) - 128; // Fitting analogRead data (range:0 - 1023) to int8_t array (range:-128 - 127)
    summ += data[i];
    while (micros() < (microseconds + sampling_period_us))
    { // Timing out uC ADC to fulfill sampling frequency requirement
    }
  }

  // Eliminating remaining DC component (produces usable data in FFT bin #0, which is usually swamped by DC bias)
  avg = summ / 64;
  for (int i = 0; i < 64; i++)
  {
    data[i] -= avg;
  }

  fix_fftr(data, 6, 0); // Performing real FFT

  // Reordering the data so that the left/right halves become odd/even, based on apparent fix_fftr output
  // Seems to make it more accurate (proper response to a frequency sweep with some aliasing)
  {
    int8_t reordered[32]; // <-- This array as a global variable causes instablity, so it's declared in a minimal scope
    for (int i = 0; i < 32; i++)
    {
      if (i % 2 != 0)
        reordered[i] = data[i / 2 + 16];
      else
        reordered[i] = data[i / 2];
    }
    for (int i = 0; i < 32; i++)
      data[i] = reordered[i];
  }

  // Time smoothing by user-determined factor and user-determined scaling
  for (int count = 0; count < 32; count++)
  {
    if (data[count] < 0)
      data[count] = 0; // Eliminating negative output of fix_fftr
#ifdef LOG_OUTPUT
    else
      data[count] = log_scale * log((float)(data[count] + 1)); // Logarithmic function equivalent to SCALING_FACTOR*log2(x+1)
#else
    else
      data[count] *= SCALE_FACTOR; // Linear scaling up according to SCALE_FACTOR
#endif
    data[count] = (float)buff[count] * anti_coeff + (float)data[count] * coeff; // Smoothing by factoring in past data
    buff[count] = data[count];                                                  // Storing current output as next frame's past data
    if (data[count] > 63)
      data[count] = 63; // Capping output at screen height
  }

  // Output to SSD1306
  uint8_t x, y;
  lcd.beginPaint();
  do
  {
    int d;
    for (int i = 0; i < 32; i++)
    {
      d = data[i] / 2;
      x = i * 4;
      y = 32 - (d + 1);
      lcd.fillRect(x, y, 3, 30 - y); // draw 1band bar

      // draw peak bar
      peak[i] = (peak[i] > y || peak[i] == 0) ? y : peak[i];
      lcd.moveto(x, peak[i]);
      lcd.lineto(x + 2, peak[i]);
    }

    // update peak bar
    unsigned long t = millis();
    if (t - t0 > 200)
    {
      for (int i = 0; i < 32; i++)
      {
        peak[i]++;
        if (peak[i] > 31)
          peak[i] = 31;
      }
      t0 = t;
    }

  } while (!lcd.endPaint());
  //lcd.flush();
}
