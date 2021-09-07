#include "Packet/packet.h"
#include "HW/UART/UART.h"
#include "MS51_16K.h"
#include "config.h"

volatile uint8_t packet_content[packet_size];
volatile uint8_t packet_count = 0;
volatile BIT packet_received = 0;

inline void handle_packet_isr(void)
{
    if (RI != 1) {
		return;
	}

	const uint8_t tmp = UART0_GetData();
	if (packet_count == 0 && tmp != 0xA0u)
	{
		RI = 0;
		return;
	}

	packet_content[packet_count++] = tmp;
	if (packet_count == packet_size)
	{
		EA = 0;
		packet_count = 0;
		packet_received = 1;
		RI = 0;
		// postpone parsing to loop
		return;
	}

	RI = 0;
}


uint8_t validate_packet(void)
{
    if (packet_content[0] != 0xA0u)
    {
        return 0;
    }

	if (packet_content[1] > CONF_RELAYS)
	{
		return 0;
	}

    const uint8_t checksum = packet_content[0] + packet_content[1] + packet_content[2];

    return (checksum == packet_content[3]);
}

