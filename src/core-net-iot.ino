#include "include/net-iot.h"

static double _tempC = 0;

void NetIotInit(void)
{
    // Register/Expose Variable
    _tempC = SensorTempGetAverage();

    Particle.variable("temp", _tempC);
}

void NetIoTstart(void)
{
    // Start Particle cloud connection
    Particle.connect();
}

bool NetIoTready(void)
{
    // Check status on Cloud connection
    return(Particle.connected());
}

void NetIoTStop(void)
{
    //TO-DO
}

void NetIoTupdate(void)
{
    // Update Cloud variables
    // Update Temperature
    _tempC = (double)SensorTempGetAverage();
}

