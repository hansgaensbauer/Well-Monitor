/*
 * Engineers Without Borders San Francisco Professional Chapter
 * 
 * Well Monitor Firmware with well depth sensing and mains power
 * 
 * See https://github.com/hansgaensbauer/Well-Monitor
 */

 #ifndef WDMP_H_
 #define WDMP_H_

  //Pin connections
  #define LEDPIN 8
  #define DEPTH_SENSE_INPUT A0
  #define GPIO_0 A2
  #define GPIO_1 A3
  #define GPIO_2 A4
  #define GPIO_3 A5
  #define GPIO_4 3
  #define GPIO_5 4
  #define GPIO_6 5
  #define GPIO_7 6
  #define SD_DET_N 7
  #define SD_CS_N 10
  #define RTC_MFP 2

  #define PRESSURE_RANGE 100 //maximum measurable pressure in psi
  #define SENSE_RESISTANCE 150.0
  #define VCC 3.3
  #define ADC_BITS 1023.0
  #define ZERO_SCALE_CURRENT 0.004
  #define FULL_SCALE_CURRENT 0.020

  #define PSI_TO_MH2O 0.703070

 #endif //WDMP_H_
