#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    // Modify the cfg...
    rkSetup(cfg);

    // Write your setup

    rkLedRed(true);
    rkLedYellow(true);
}

void loop() {
    if (rkButtonIsPressed(1)) { // Je tlačítko SW1 stisknuté?
        rkLedGreen(false);
        rkLedBlue(true);
    } else {
        rkLedGreen(true);
        rkLedBlue(false);
    }
}
