#include "IOTcloud.h"

static iot_cmd_func_t _cmd_func = NULL;

// Class constructor
IOTcloud::IOTcloud()
{
    iot_cloud_enabled = false;
    iot_temperature_F = 0.0;
}

void IOTcloud::start(double init_temp_F, iot_cmd_func_t cmd_func)
{
    if (!iot_cloud_enabled)
    {
        // Register/Expose Variable
        iot_temperature_F = init_temp_F;

        Particle.variable("tempF", iot_temperature_F);

        Particle.function("command", &IOTcloud::commandHandler, this);

        // Start Particle cloud connection
        Particle.connect();

        _cmd_func = cmd_func;

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

int IOTcloud::commandHandler(String command)
{
    if (_cmd_func != NULL)
    {
        _cmd_func(command);
    }
        
    return 1;
}