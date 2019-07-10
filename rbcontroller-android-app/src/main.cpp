#include <Arduino.h>

#include "roboruka.h"

void handleMessage(const std::string& cmd, rbjson::Object *pkt) {
    if(cmd == "joy") {
        const rbjson::Array *data = pkt->getArray("data");
        if(data->size() >= 1) {
            const rbjson::Object *joy = data->getObject(0);
            int x = joy->getInt("x");
            int y = joy->getInt("y");

            rkMotorsJoystick(x, y);
        }
    } else if(cmd == "arm") {
        const double x = pkt->getDouble("x");
        const double y = pkt->getDouble("y");
        rkArmMoveTo(x, y);
    } else if(cmd == "grab") {
        rkArmSetGrabbing(!rkArmIsGrabbing());
    }
}

void setup() {
    rkConfig cfg;
    cfg.owner = "FrantaFlinta";
    cfg.name = "SuperRuka";

    cfg.wifi_name = "RukoKraj";
    cfg.wifi_password = "PlnoRukou";

    cfg.motor_enable_failsafe = true;
    cfg.rbcontroller_app_enable = true;
    cfg.rbcontroller_message_callback = handleMessage;
    rkSetup(cfg);

    printf("%s's roboruka '%s' started!\n", cfg.owner, cfg.name);
}

static int gIter = 0;

void loop() {
    // Send text to the android application
    rkControllerSendLog("Tick #%d, battery at %d%%, %dmv %d\n", gIter++, rkBatteryPercent(),
        rkBatteryVoltageMv(), rkButtonIsPressed(1));
    sleep(1);
}
