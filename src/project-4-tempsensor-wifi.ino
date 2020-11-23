/*
 * Project project-4-tempsensor-wifi
 * Description:
 * Author:
 * Date:
 */

SYSTEM_THREAD(ENABLED);

SYSTEM_MODE(SEMI_AUTOMATIC);

#include "config.h"
#include "IOTboard.h"

IOTboard iot_kit(true);

enum app_state_t {
    APP_STATE_INIT,
    APP_STATE_NET_CONNECT,
    APP_STATE_NET_WAIT,
    APP_STATE_NET_READY
};

static app_state_t _app_state = APP_STATE_INIT;

void appStateMachine(void)
{
    switch(_app_state)
    {
        case APP_STATE_INIT:
            _app_state = APP_STATE_NET_CONNECT;

            iot_kit.printToSerialOut("IOT app started");
            break;

        case APP_STATE_NET_CONNECT:
            // Start WiFi connection
            iot_kit.disconnectWiFi();

            iot_kit.printToSerialOut("Connecting to %s...", WIFI_AP_SSID);

            iot_kit.connectToWiFi(WIFI_AP_SSID, WIFI_AP_PASSW);

            _app_state = APP_STATE_NET_WAIT;
            break;

        case APP_STATE_NET_WAIT:
            // Check if we are connected to WiFi AP
            if (iot_kit.isWiFiReady())
            {
                iot_kit.printToSerialOut("Connected!");

                iot_kit.printToSerialOut("Local IP = %s", iot_kit.getWiFiIP());

                _app_state = APP_STATE_NET_READY;
            }
            break;

        case APP_STATE_NET_READY:
            // Connected to WiFi AP
            break;
    }

    // Run the Serial Monitor processor
    // Check for incoming commands from serial port
}

// setup() runs once, when the device is first turned on.
void setup()
{
    iot_kit.start();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    // The core of your code will likely live here.
    appStateMachine();
}