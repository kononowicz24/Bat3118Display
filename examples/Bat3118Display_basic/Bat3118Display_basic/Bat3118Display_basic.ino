#include <Bat3118Display.h>

int pins[] = {3,5,6,9,10,11};
//corresponding pins of an Arduino to the pins of the 3118 display

Bat3118Display indicator = Bat3118Display(pins, COMMON_ANODE, (DIMMING | CHARGE_BORDER), 60);
//see Bat3118Display.h for further info for the parameters.

long lastReadingMillis=1;

void setup() {
  indicator.setCharging(0); //setting the charging segment on
}

void loop() {
  static int i = 0;
  indicator.refreshDisplay(); //the code should be free from delays to make it work properly.
  if (millis() > lastReadingMillis + 300) { //refreshing the value of i every 300ms
    i++;
    indicator.setValue(i);
    lastReadingMillis = millis();
    if (i>=100) i=0;
  }
}
