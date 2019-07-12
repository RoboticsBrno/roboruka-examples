#include <Arduino.h>
#include "roboruka.h"


void setup() {
    Serial.begin(115200);
    rkConfig cfg;
    // Upravte nastavení, například:
    cfg.motor_max_power_pct = 40; // limit výkonu motorů na 40%
    rkSetup(cfg);
}

void loop() {
    rkLedBlue(false);
    if (rkButtonIsPressed(2, false) && rkButtonIsPressed(3, false)) { // dozadu
        rkMotorsSetPower(-100, -100);
    } else if (rkButtonIsPressed(1, false)) { // dopředu
        rkMotorsSetPower(100, 100);
        rkLedBlue();
        rkLedRed(0);
    } else if (rkButtonIsPressed(2, false)) { // doleva
        rkMotorsSetPower(-100, 100);
        rkLedGreen();
        rkLedRed(0);
    } else if (rkButtonIsPressed(3, false)) { // doprava
        rkMotorsSetPower(100, -100);
        rkLedYellow();
        rkLedRed(0);
    } else {
        rkMotorsSetPower(0, 0);
        rkLedAll(0);
        rkLedRed(1);
    }
}
