#include <Arduino.h>
#include "roboruka.h"


void setup() {
    Serial.begin(115200);
    rkConfig cfg;
    // Upravte nastavení, například:
    cfg.motor_max_power_pct = 60; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Napište setup kód, provede se jednou po startu robota
    rkLedBlue();
}

void loop() {
    rkLedBlue(false);
    if (rkButtonIsPressed(1, false)) {
        rkMotorsSetPower(100, 100);
    }
    else if (rkButtonIsPressed(2, false)) {
        rkMotorsSetPower(100, -100);
    }
    else {
        rkMotorsSetPower(0, 0);
    }
}
