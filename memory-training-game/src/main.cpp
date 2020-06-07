#include <Arduino.h>
#include "roboruka.h"


void setup() {
    rkConfig cfg;
    // Upravte nastavení, například:
    // cfg.motor_max_power_pct = 30; // limit výkonu motorů na 30%
    rkSetup(cfg);

    // Sem patří váš kód

    randomSeed(esp_random()); // Inicializace generátoru náhodných čísel
    rkLedBlue(true);

    // Kolik kroků má sekvence, postupně se zvyšuje
    int steps = 3;
    // Vzorová sekvence. std::vector == pole, jedno číslo za druhým v paměti
    std::vector<int> sequence_template;
    // Sekvence, kterou zadal uživatel
    std::vector<int> sequence_user;

    // Stav:
    // * 0: čekáme na stisknutí SW1 abychom vygenerovali sekvenci a začli hru.
    // * 1: vyblikáváme vzorovou sekvenci
    // * 2: uživatel mačká tlačíka a my je nahráváme do sequence_user
    // * 3: zkontrolujeme zadanou sekvenci a vyblikáme výsledek
    int state = 0;

    while(true) {
        if(state == 0) {
            // Čekáme na SW1
            if(rkButtonIsPressed(1, true)) {
                // Vygenerujeme sekvenci náhodných čísel 1 až 3
                for(int s = 0; s < steps; ++s) {
                    sequence_template.push_back(random(1, 4)); // vrchní limit random() není včetně, proto 4 místo 3
                }

                // zhasneme modrou a přejdeme do dalšího stavu
                rkLedBlue(false);
                state = 1;
            }
        } else if(state == 1) {
            // Vyblikáme vzorovou sekvenci
            rkLedAll(false);
            for(int i = 0; i < steps; ++i) {
                delay(400);
                rkLedById(sequence_template[i]);
                delay(400);
                rkLedAll(false);
            }

            // Přejdeme do dalšího stavu
            state = 2;
        } else if(state == 2) {
            // Vyčítáme všechny tlačítka od 1 do 3 včetně
            for(int tl = 1; tl <= 3; ++tl) {
                // Jedno z nich (tl) je stisknuto
                if(rkButtonIsPressed(tl, true)) {
                    // Zapíšeme jeho id do uživatelské sekvence
                    sequence_user.push_back(tl);

                    // Zkontrolujeme, kolik uživatel zadal kroků
                    if(sequence_user.size() >= steps) {
                        // Pokud dost, přejdeme do dalšího stavu
                        state = 3;
                        break;
                    }
                }
            }
        } else if(state == 3) {
            // Kontrola sekvence, kterou zadal uživatel proti vzoru
            bool correct = true;
            for(int s = 0; s < steps; ++s) {
                if(sequence_template[s] != sequence_user[s]) {
                    correct = false;
                    break;
                }
            }

            // Vyblikáme výsledek
            bool on = true;
            for(int i = 0; i < 8; ++i) {
                if(correct) {
                    rkLedGreen(on);
                } else {
                    rkLedRed(on);
                }
                on = !on;
                delay(300);
            }

            // Pro další hru zvýšíme obtížnost, přidáme další prvek do sekvence
            if(correct) {
                steps += 1;
            }

            // Vyčistíme sekvence pro další hru
            sequence_template.clear();
            sequence_user.clear();

            // Přejdeme zpět do stavu 0, kdy čekáme na SW s rozsvícenou modrou LED.
            state = 0;
            rkLedBlue(true);
        }
    }
}
