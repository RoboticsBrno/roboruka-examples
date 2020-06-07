#include "roboruka.h"
#include <Arduino.h>

void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    cfg.motor_max_power_pct = 40; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Sem patří váš kód

    bool following = false;
    float previousPos = 0.f;

    while (true) {
        if (rkButtonIsPressed(1, true)) { // Tlačítko 1: Kalibruj senzory
            delay(500);
            rkLineCalibrate();
        } else if (rkButtonIsPressed(2, true)) { // Tlačítko 2: Sleduj čáru
            if (following) {
                rkMotorsSetPower(0, 0); // zastavit robota, aby přestal sledovat
                following = false;
            } else {
                following = true;
            }
            delay(500); // prodleva, abyste stihli uhnout rukou
        }

        // Načti pozici čáry
        float pos = rkLineGetPosition();

        // Rozsvítíme ledky podle toho, kde čára zrovna je
        int ledId = int(round((pos + 1.f) / 2 * 3)); // převod z rozsahu <-1;1> na číslo od 0 do 3,
        rkLedRed(ledId == 0);
        rkLedYellow(ledId == 1);
        rkLedGreen(ledId == 2);
        rkLedBlue(ledId == 3);

        // Pokud jsme mimo čáru, použijeme pozici z poslední iterace,
        // jinak si pozici uložíme.
        if (isnan(pos)) {
            // Pokud předchozí pozice byla vlevo (<0), zatočíme co nejvíc doleva (-1),
            // v opačném případě doprava (1).
            if (previousPos < 0) {
                pos = -1;
            } else {
                pos = 1;
            }
        } else {
            previousPos = pos;
        }

        // Sledujeme teď čáru?
        if (following) {

            // Zatáčení doleva
            if (pos < -0.75) {
                rkMotorsSetPower(0, 100);
            } else if (pos < -0.25) {
                rkMotorsSetPower(50, 100);
            }

            // Rovně
            if (pos > -0.25 && pos < 0.25) {
                rkMotorsSetPower(100, 100);
            }

// Doprava
// DOPLŇ MĚ!! Dopište tuto část kódu, aby robot zatáčel
// doprava pokud je pos > 0.25 :)
// Následující řádek smaž.
#warning "Je třeba dopsat kód! Nejprve Smaž tento řádek (s #warning ...) a pak dopiš kód pro zatáčení doprava."
        }

        usleep(10000); // mezi iteracemi čekáme 10ms == 10000us
    }
}
