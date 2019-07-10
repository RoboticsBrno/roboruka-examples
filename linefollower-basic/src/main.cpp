#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    cfg.motor_max_power_pct = 40; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Napište setup kód, provede se jednou po startu robota
}

static bool gFollowing = false;
static float gPreviousPos = 0.f;

void loop() {
    // Obsah této metody se provádí neustále dokola

    if(rkButtonIsPressed(1)) {         // Tlačítko 1: Kalibruj senzory
        usleep(500000);
        rkLineCalibrate();
    } else if(rkButtonIsPressed(2)) {  // Tlačítko 2: Sleduj čáru
        if(gFollowing) {
            rkMotorsSetPower(0, 0); // zastavit robota, aby přestal sledovat
            gFollowing = false;
        } else {
            gFollowing = true;
        }
        usleep(500000); // prodleva, abyste stihli uhnout rukou
    }

    // Načti pozici čáry
    float pos = rkLinePosition();

    // Rozsvítíme ledky podle toho, kde čára zrovna je
    int ledId = int(round((pos + 1.f)/2*3)); // převod z rozsahu <-1;1> na číslo od 0 do 3,
    rkLedRed(ledId == 0);
    rkLedYellow(ledId == 1);
    rkLedGreen(ledId == 2);
    rkLedBlue(ledId == 3);

    // Pokud jsme mimo čáru, použijeme pozici z poslední iterace,
    // jinak si pozici uložíme.
    if(isnan(pos)) {
        // Pokud předchozí pozice byla vlevo (<0), zatočíme co nejvíc doleva (-1),
        // v opačném případě doprava (1).
        if(gPreviousPos < 0) {
            pos = -1;
        } else {
            pos = 1;
        }
    } else {
        gPreviousPos = pos;
    }

    // Sledujeme teď čáru?
    if(gFollowing) {

        // Zatáčení doleva
        if(pos < -0.75) {
            rkMotorsSetPower(0, 100);
        } else if(pos < -0.25) {
            rkMotorsSetPower(50, 100);
        }

        // Rovně
        if(pos > -0.25 && pos < 0.25) {
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
