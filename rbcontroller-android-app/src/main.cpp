#include <Arduino.h>

#include "roboruka.h"

void setup() {
    rkConfig cfg;
    cfg.owner = "FrantaFlinta"; // Ujistěte se, že v aplikace RBcontrol máte nastavené stejné
    cfg.name = "SuperRuka";

    // Ve výchozím lze WiFi na robotovi nastavit pomocí Android aplikace
    // RBControl (verze 1.0 nebo novější) přes Bluetooth.
    // Robot si toto nastavení pamatuje, a znovu ho použije při dalším zapnutí.

    // Můžete WiFi i přímo nastavit zde v kódu, v takovém případě se nastavení
    // nedá z aplikace změnit.

    // Zde v kódu můžete BUĎTO připojit robota na WiFi...
    //cfg.wifi_name = "RukoKraj";
    //cfg.wifi_password = "PlnoRukou";

    // A NEBO vytvořit vlastní WiFi (odkomentovat další dva řádky)
    //cfg.wifi_default_ap = true;
    //cfg.wifi_ap_password = "flusflus";

    cfg.motor_enable_failsafe = true;
    cfg.rbcontroller_app_enable = true;
    rkSetup(cfg);

    UI.arm(0, 0, 12, 9, rkArmGetInfo())
        .onPositionChanged([](Arm& arm) {
            rkArmMoveTo(arm.getX(), arm.getY());
        })
        .onGrab([](Arm&) {
            rkArmSetGrabbing(!rkArmIsGrabbing());
        })
        .finish();
    
    UI.joystick(6, 12, 5, 5, "blue")
        .onPositionChanged([](Joystick& joy) {
            rkMotorsJoystick(joy.getX(), joy.getY());
        })
        .finish();
    
    UI.commit();

    printf("%s's roboruka '%s' started!\n", cfg.owner, cfg.name);
}

static int gIter = 0;

void loop() {
    // Send text to the android application
    rkControllerSendLog("Tick #%d, battery at %d%%, %dmv %d\n", gIter++, rkBatteryPercent(),
        rkBatteryVoltageMv(), rkButtonIsPressed(1, false));
    delay(1000);
}
