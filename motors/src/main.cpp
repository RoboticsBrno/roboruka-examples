#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Sem patří váš kód

    rkLedBlue();

    while(true) {
        delay(2000);
        rkLedRed(true);

        // Dopředu na 100%
        rkMotorsSetPower(100, 100);
        delay(2000);

        // Otočka na místě doprava
        rkMotorsSetPower(100, -100);
        delay(2000);

        // Otočka na místě doleva
        rkMotorsSetPower(-100, 100);
        delay(2000);

        // Dozadu
        rkMotorsSetPower(-100, -100);
        delay(2000);

        // Zastavit
        rkLedRed(false);
        rkMotorsSetPower(0, 0);
        delay(2000);
    }
}
