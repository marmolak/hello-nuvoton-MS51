#ifndef RELAYS_H
#define RELAYS_H

/**
 *  Stock protocol
 *  Open relay  1:A0 01 01 A2
 *  Close relay 1:A0 01 00 A1
 *  Open relay  2:A0 02 01 A3
 *  Close relay 2:A0 02 00 A2
 *  Open relay  3:A0 03 01 A4
 *  Close relay 3:A0 03 00 A3
 *  Open relay  4:A0 04 01 A5
 *  Close relay 4:A0 04 00 A4
 */
#define RELAY1 P00
//#define RELAY1 P12 // - LED - debug purposes

#define RELAY2 P03
#define RELAY3 P04
#define RELAY4 P01

void init_relays(void);

#endif