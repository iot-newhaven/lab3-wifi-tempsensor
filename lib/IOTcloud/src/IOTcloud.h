// IOT Cloud connectivity class

#ifndef IOTCLOUD_H
#define IOTCLOUD_H

#include <stdint.h>
#include <stdbool.h>

#include "application.h"

/* code */
class IOTcloud
{
public:
    IOTcloud();

    void start(double init_temp_F);

    bool isReady(void);

    void update(double temp_F);

    void pushTempAlarm(double temp_F);

private:
    // IOT parameters
    bool iot_cloud_enabled;
    double iot_temperature_F;
};

#endif