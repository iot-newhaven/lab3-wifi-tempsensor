#include "IOTcloud.h"

// Class constructor
IOTcloud::IOTcloud()
{
    iot_cloud_enabled = false;
    iot_temperature_F = 0.0;
}

void IOTcloud::start(double init_temp_F)
{
    if (!iot_cloud_enabled)
    {
        // Register/Expose Variable
        iot_temperature_F = init_temp_F;

        Particle.variable("tempF", iot_temperature_F);

        // Start Particle cloud connection
        Particle.connect();

        iot_cloud_enabled = true;
    }
}

bool IOTcloud::isReady(void)
{
    // Check status on Cloud connection
    return (Particle.connected());
}

void IOTcloud::update(double temp_F)
{
    // Update Cloud variables
    // Update Temperature
    iot_temperature_F = temp_F;
}

void IOTcloud::pushTempAlarm(double temp_F)
{
    if(Particle.connected())
    {
        Particle.publish("temp alarm", String(temp_F));
    }
}