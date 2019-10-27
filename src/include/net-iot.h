#ifndef _NET_IOT_H
#define _NET_IOT_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "application.h"

#include "include/sensor.h"

void NetIotInit(void);

void NetIoTstart(void);

bool NetIoTready(void);

void NetIoTStop(void);

void NetIoTupdate(void);

#endif