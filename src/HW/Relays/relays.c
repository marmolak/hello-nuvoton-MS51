#include "HW/Relays/relays.h"
#include "MS51_16K.h"
#include "config.h"

#define _UNROLL_RELAY_PUSHPULL(RELAY_ID) RELAY_ID ## _PUSHPULL_MODE
#define _UNROLL_RELAY_SETLOW(RELAY_ID) RELAY_ID = 0
#define SET_PIN_FOR(RELAY_ID) do {  \
    _UNROLL_RELAY_PUSHPULL(RELAY_ID); \
	_UNROLL_RELAY_SETLOW(RELAY_ID); \
} while (0)

#ifndef CONF_RELAYS
#error "You need to define number of relays via CONF_RELAYS" 
#endif

void init_relays(void)
{
    SET_PIN_FOR(RELAY1);

#if CONF_RELAYS >= 2
    SET_PIN_FOR(RELAY2);
#endif

#if CONF_RELAYS >= 3
    SET_PIN_FOR(RELAY3);
#endif

#if CONF_RELAYS >= 4
    SET_PIN_FOR(RELAY4);
#endif
}