#include "roboruka.h"
#include <Arduino.h>

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Sem patří váš kód

    rkLedRed(true);
    rkLedYellow(true);

    while (true) {
        if (rkButtonIsPressed(1)) { // Je tlačítko SW1 stisknuté?
            rkLedGreen(false);
            rkLedBlue(true);
        } else {
            rkLedGreen(true);
            rkLedBlue(false);
        }
    }
}
