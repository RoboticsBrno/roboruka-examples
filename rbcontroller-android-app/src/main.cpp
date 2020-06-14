#include <Arduino.h>

#include "roboruka.h"

#define GRIDUI_LAYOUT_DEFINITION
#include "layout.h"

void setup() {
    rkConfig cfg;
    cfg.owner = "FrantaFlinta"; // Ujistěte se, že v aplikace RBcontrol máte nastavené stejné
    cfg.name = "SuperRuka";

    // Ve výchozím stavu lze WiFi na robotovi nastavit pomocí Android aplikace
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

    auto builder = Layout.begin();
    builder.Arm1
        .onPositionChanged([&](Arm& arm) {
            rkArmMoveTo(arm.x(), arm.y());
        })
        .onGrab([&](Arm&) {
            rkArmSetGrabbing(!rkArmIsGrabbing());
        });

    builder.Joystick1
        .onPositionChanged([&](Joystick& joy) {
            rkMotorsJoystick(joy.x(), joy.y());
        });

    builder.commit();

    fmt::print("{}'s roboruka '{}' started!\n", cfg.owner, cfg.name);

    for (int i = 0; true; ++i) {
        // Send text to the android application
        rkControllerSendLog(fmt::format("Tick #{}, battery at {}%, {}mv\n",
            i, rkBatteryPercent(), rkBatteryVoltageMv()));
        delay(1000);
    }
}
