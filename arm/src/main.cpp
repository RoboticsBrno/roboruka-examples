#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Napište setup kód, provede se jednou po startu robota
}

void loop() {
    // Obsah této metody se provádí neustále dokola

    // SW1: přesunout ruku před robota
    if(rkButtonIsPressed(1)) {
        rkArmMoveTo(150, -45);
        usleep(800000);
        rkArmMoveTo(200, 18);
        usleep(600000);
        rkArmMoveTo(155, 67);
        usleep(600000);
    }

    // SW2: schovat ruku na robota
    if(rkButtonIsPressed(2)) {
        rkArmMoveTo(150, -45);
        usleep(800000);
        rkArmMoveTo(45, 10);
        usleep(600000);
    }

    // SW3: otevřít/zavřít prsty
    if(rkButtonIsPressed(3)) {
        rkLedBlue(!rkArmIsGrabbing());
        rkArmSetGrabbing(!rkArmIsGrabbing());
        usleep(300000);
    }
}
