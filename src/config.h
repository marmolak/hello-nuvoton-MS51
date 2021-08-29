#ifndef CONFIG_H
#define CONFIG_H

// Number of relays from 1 to 4
#define CONF_RELAYS 1


// Compile time checks
#ifndef CONF_RELAYS
#error "You need to define number of relays via CONF_RELAYS" 
#endif

#if CONF_RELAYS < 1 || CONF_RELAYS > 4
#error "Too much or too less relays defined."
#endif

#endif