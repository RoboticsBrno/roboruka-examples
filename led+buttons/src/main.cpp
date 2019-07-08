#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Napište setup kód, provede se jednou po startu robota
    rkLedRed(true);
    rkLedYellow(true);
}

void loop() {
    // Obsah této metody se provádí neustále dokola

    if (rkButtonIsPressed(1)) { // Je tlačítko SW1 stisknuté?
        rkLedGreen(false);
        rkLedBlue(true);
    } else {
        rkLedGreen(true);
        rkLedBlue(false);
    }
}
