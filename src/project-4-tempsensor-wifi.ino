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
#include "IOTcloud.h"

#define IOT_CLOUD_UPDATE_MS (1000)
#define IOT_CLOUD_EVENT_MS (15000)

IOTboard iot_kit(true);

IOTcloud cloud_kit;

enum app_state_t {
    APP_STATE_INIT,
    APP_STATE_NET_CONNECT,
    APP_STATE_NET_WAIT,
    APP_STATE_NET_READY,
    APP_STATE_CLOUD_WAIT,
    APP_STATE_CLOUD_READY
};

static app_state_t _app_state = APP_STATE_INIT;
static unsigned long _last_cloudupdate_tick = 0;
static unsigned long _last_event_tick = 0;

void appCommandHandler(String command)
{
    iot_kit.printToSerialOut(command.c_str());
}

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
            // Start Cloud link
            iot_kit.printToSerialOut("Connect to Cloud server:");

            // Setup cloud connection
            cloud_kit.start((double)iot_kit.getTempF(), appCommandHandler);

            _app_state = APP_STATE_CLOUD_WAIT;

            iot_kit.printToSerialOut("waiting...");
            break;

        case APP_STATE_CLOUD_WAIT:
            if(cloud_kit.isReady())
            {
                iot_kit.printToSerialOut("Cloud ready!");

                _last_cloudupdate_tick = millis();

                _last_event_tick = millis();

                _app_state = APP_STATE_CLOUD_READY;
            }
            break;

        case APP_STATE_CLOUD_READY:
            if (millis() - _last_cloudupdate_tick >= IOT_CLOUD_UPDATE_MS)
            {
                cloud_kit.update(iot_kit.getTempF());

                _last_cloudupdate_tick = millis();
            }

            if (millis() - _last_event_tick >= IOT_CLOUD_EVENT_MS)
            {
                cloud_kit.pushTempAlarm(iot_kit.getTempF());
                _last_event_tick = millis();
            }
            break;

        default:
            break;
    }

    // Run the Serial Monitor processor
    // Check for incoming commands from serial port
}

// setup() runs once, when the device is first turned on.
void setup()
{
    // Start the board
    iot_kit.start();
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
    // Execute app state machine
    appStateMachine();
}