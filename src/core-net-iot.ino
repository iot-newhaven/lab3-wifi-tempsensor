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
    Particle.connect();
}

bool NetIoTready(void)
{
    return(Particle.connected());
}

void NetIoTStop(void)
{

}

void NetIoTupdate(void)
{
    // Update Temperature
    _tempC = (double)SensorTempGetAverage();
}

