#ifndef PACKET_H
#define PACKET_H

#include "MS51_16K.h"

/* public */

/* UART interrupt routine */
#define packet_size 4

extern volatile uint8_t packet_content[packet_size];
extern volatile uint8_t packet_count;
extern volatile BIT packet_received;

uint8_t validate_packet(void);
inline void handle_packet_isr(void);

#endif