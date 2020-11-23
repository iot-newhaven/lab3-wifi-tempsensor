// IOT Cloud connectivity class

#ifndef IOTCLOUD_H
#define IOTCLOUD_H

#include <stdint.h>
#include <stdbool.h>

#include "application.h"

typedef void (*iot_cmd_func_t)(String cmd);

/* code */
class IOTcloud
{
public:
    IOTcloud();

    void start(double init_temp_F, iot_cmd_func_t cmd_func);

    bool isReady(void);

    void update(double temp_F);

    void pushTempAlarm(double temp_F);

private:
    // IOT parameters
    bool iot_cloud_enabled;
    double iot_temperature_F;

    int commandHandler(String command);
};

#endif