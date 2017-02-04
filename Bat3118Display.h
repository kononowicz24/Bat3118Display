#ifndef __BAT3118DISPLAY_H__
#define __BAT3118DISPLAY_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define SEGMENTS 5

#define COMMON_CATHODE 0 //common 0
#define COMMON_ANODE 1

#define FLASHING 1 //always flashing depending on percentage
#define DIMMING 2 //always dimming corresponding segments to 0 based on percentage
#define SHOW_LOWER 4 //always showing lower segment, no dimming nor flashing
#define SHOW_HIGHER 8//always showing higher segment, no dimming nor flashing
#define HIGHER_FLASH20 16//shows higher segment, when less than 20% blinks B segment based on percentage
#define HIGHER_DIM20 32//shows higher segment, when less than 20% dims B segment to 0
#define CHARGE_BORDER 64//shows red border while charging
#define CHARGE_FLASHING 128//flashes up the segments till 100% and starts from current percentage again
//CHARGE_BORDER and CHARGE_FLASHING can be set simultaneously by using | operator
//Dimming can only be done when connecting the segment to the PWM pin of the Arduino


/*Connect the commons directly either to ground or power rail.
Always put resistors between the diodes and the Arduino!
The border segment (A) consists of seven LEDs in paralel
multiplying the flowing current by 7
You don't want to fry neither the Arduino nor your brand new display, do you?*/

class Bat3118Display {
private:


  bool segmentOn, segmentOff;

  bool charging;
  bool charged;
  int percentage;
  int pins[SEGMENTS+1];
  bool hardwareConfig;
  byte flashConfig;//
  long flashTime;// the duration of a single flash in ms
  //one percent of battery = one unit, default 60ms, generic flash = 4xflashTime

  void setSegment(byte segment, bool state);
  void setSegmentFlashing(byte segment, byte value);
  void setSegmentDimming(byte segment, byte value);
  void refreshDisplay(byte mode);

public:
  Bat3118Display(int* pinsc, bool hwConfig, byte flashConfigc, long flashTimec);
  //corresponding Arduino pins to display pins 1..6, is display common anode or cathode
  // - - configuration of flashing and dimming, the base time of flash
  Bat3118Display(int* pinsc, bool hwConfig, byte flashConfigc);
  Bat3118Display(int* pinsc, bool hwConfig);
  Bat3118Display(int* pinsc);
  Bat3118Display();
  virtual ~Bat3118Display();

  void setValue(int value);//int in range of 0-100
  void setValue(double value);//double or float in range 0..1

  void setCharging(bool chargingc);
  void setCharged(bool chargedc);

  void refreshDisplay();
  byte getMode();
};

#endif // __BAT3118DISPLAY_H__
