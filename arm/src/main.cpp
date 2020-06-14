#include "roboruka.h"
#include <Arduino.h>

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Sem patří váš kód

    while (true) {
        // SW1: přesunout ruku před robota
        if (rkButtonIsPressed(1, true)) {
            rkArmMoveTo(150, -45);
            delay(800);
            rkArmMoveTo(200, 18);
            delay(600);
            rkArmMoveTo(155, 67);
            delay(600);
        }

        // SW2: schovat ruku na robota
        if (rkButtonIsPressed(2, true)) {
            rkArmMoveTo(150, -45);
            delay(800);
            rkArmMoveTo(45, 10);
            delay(600);
        }

        // SW3: otevřít/zavřít prsty
        if (rkButtonIsPressed(3, true)) {
            rkLedBlue(!rkArmIsGrabbing());
            rkArmSetGrabbing(!rkArmIsGrabbing());
            delay(300);
        }
    }
}
