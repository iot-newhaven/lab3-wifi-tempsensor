// IOT Display class

#ifndef IOTBOARD_H
#define IOTBOARD_H

#include <stdint.h>
#include <stdbool.h>

#include "application.h"
// Include MicroOLED library
#include "SparkFunMicroOLED.h" 
#include "math.h"

#include "SparkFunTMP102.h"

#define BOARD_SYSTEM_LED      D7
#define BOARD_HEARTBEAT_MS    (1000)

enum iot_wifi_mode_t {
   IOT_WIFI_WPA2,
   IOT_WIFI_ENTERPRISE
};

/* code */
class IOTboard
{
public:
   IOTboard(bool wait_for_serial = false);

   void start();

   void printToDisplay(const char* text);

   float getTempF();

   float getTempC();

   void printToSerialOut(const char *fmt, ...);

   char getSerialInput();

   void connectToWiFi(const char *SSID,
                      const char *password,
                      iot_wifi_mode_t mode);

   void disconnectWiFi(void);

   bool isWiFiReady(void);

   const char *getWiFiIP();

private:
   char serial_out_buffer[255];
   char iot_ip_addr[32];
   char input_cmd;
   bool wait_serial_monitor;
};

#endif