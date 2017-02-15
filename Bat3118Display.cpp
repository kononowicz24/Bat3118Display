#include "Bat3118Display.h"
#include <math.h>

#define logb(a,b) log(a)/log(b)

Bat3118Display::Bat3118Display(int* pinsc, bool hwConfig, byte flashConfigc, long flashTimec) {
  hardwareConfig = hwConfig;
  flashConfig = flashConfigc;
  flashTime = flashTimec;

  if (hardwareConfig) { //assigning the pins whether active low or active high
    segmentOn = LOW;
  } else {
    segmentOn = HIGH;
  }
  segmentOff = !segmentOn;

  for (byte segment=0; segment<SEGMENTS+1; ++segment) {//copying corresponding pins
    pins[segment]=pinsc[segment];
  }

  for (byte segment=0; segment<SEGMENTS+1; ++segment) {
    pinMode(pins[segment], OUTPUT);
    setSegment(segment, 0);
  }
}

void Bat3118Display::setSegment(byte segment, bool state) {
  digitalWrite(pins[segment], (state?segmentOn:segmentOff));
}

void Bat3118Display::setSegmentFlashing(byte segment, byte value) {
  float for_segment = 100/SEGMENTS;
  int millis_for_one_unit = for_segment*flashTime;
  int phase = millis()%millis_for_one_unit;
  int cycle = phase/flashTime;
  if (cycle<=value) setSegment(segment, 1);
  else setSegment(segment, 0);
}

void Bat3118Display::setSegmentDimming(byte segment, byte value) {
  analogWrite(pins[segment], abs(logb(value+1), pow(21, 1/255.0f)) );//TODO: optimize//already optimized?
}

void Bat3118Display::setValue(int value) {
  percentage = value;
}

void Bat3118Display::setValue(double value) {
  percentage = (int)(value*100);
}

void Bat3118Display::setCharged(bool chargedc) {
  charged = chargedc;
}

void Bat3118Display::setCharging(bool chargingc) {
  charging = chargingc;
}

void Bat3118Display::refreshDisplay() {
  if ((!charging && !charged) || !(flashConfig & CHARGE_FLASHING))) {
    if (flashConfig & FLASHING) {
      refreshDisplay(FLASHING);
    } else if (flashConfig & DIMMING) {
      refreshDisplay(DIMMING);
    } else if (flashConfig & SHOW_LOWER) {
      refreshDisplay(SHOW_LOWER);
    } else if (flashConfig & SHOW_HIGHER) {
      refreshDisplay(SHOW_HIGHER);
    } else if (flashConfig & HIGHER_FLASH20) {
      refreshDisplay(HIGHER_FLASH20);
    } else if (flashConfig & HIGHER_DIM20) {
      refreshDisplay(HIGHER_DIM20);
    }
  }
  if (charging && (flashConfig & CHARGE_BORDER)) setSegment(0, 1);
  if (charged) setSegmentFlashing(0, 10);
  if (charging && (flashConfig & CHARGE_FLASHING)) {
    refreshDisplay(CHARGE_FLASHING);
  }
}

void Bat3118Display::refreshDisplay(byte mode) {
  int a = percentage/(100/SEGMENTS);
  int b = percentage % (100/SEGMENTS);
  switch (mode) {
    case FLASHING:
      for (int i=1; i<=SEGMENTS; ++i) {
        if (i<=a) setSegment(i, 1); else
        if (i==a+1) setSegmentFlashing(i, b); else
        setSegment(i, 0);
      }
      break;

    case DIMMING:
      for (int i=1; i<=SEGMENTS; ++i) {
        if (i<=a) setSegment(i, 1); else
        if (i==a+1) setSegmentDimming(i, b); else
        setSegment(i, 0);
      }
      break;

    case SHOW_LOWER:
      for (int i=1; i<=SEGMENTS; ++i) {
        setSegment(i, 1);
      }
      break;

    case SHOW_HIGHER:
      for (int i=1; i<=SEGMENTS; ++i) {
        setSegment(i, 1);
      }
      break;

    case HIGHER_FLASH20:
      if (a) {
        for (int i=1; i<=SEGMENTS; ++i) {
          setSegment(i, 1);
        }
      } else setSegmentFlashing(1, b);
      break;

    case HIGHER_DIM20:
      if (a) {
        for (int i=1; i<=SEGMENTS; ++i) {
          setSegment(i, 1);
        }
      } else setSegmentDimming(1, b);
      break;

  }
}

Bat3118Display::~Bat3118Display() {
  ;
}

byte Bat3118Display::getMode() {
  return flashConfig;
}
