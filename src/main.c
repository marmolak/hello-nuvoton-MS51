#include "MS51_16K.h"

#include "HW/UART/UART.h"
#include "HW/Relays/relays.h"
#include "Packet/packet.h"


static const uint8_t __xdata hello[] = "Alternative firmware for lc-tech board with Nuvoton.\n";

void UART0_ISR(void) __interrupt(4)
{
    // received byte
	handle_packet_isr();
}

static void set_relay_state_by_x(void)
{
	packet_received = 0;
    if (!validate_packet()) 
    {
		EA = 1;
        return;
    }

	const uint8_t relay = packet_content[1];
	const uint8_t state = packet_content[2];
	EA = 1;
	switch(relay)
    {
		case 1:
			RELAY1 = state ? 1 : 0;
			break;
		#ifdef CONF_RELAY >= 2
		case 2:
			RELAY2 = state ? 1 : 0;
			break;
		#endif
		#ifdef CONF_RELAY >= 3
		case 3:
			RELAY3 = state ? 1 : 0;
			break;
		#endif
		#ifdef CONF_RELAY >= 4
		case 4:
			RELAY4 = state ? 1 : 0;
			break;
		#endif
    }
}

static void init_cpu(void)
{
    // set cpu to 24Mhz mode
    unsigned char hircmap0,hircmap1;
    IAPAH = 0x00;
    IAPAL = 0x38;
    IAPCN = READ_UID;
    set_IAPTRG_IAPGO;
    hircmap0 = IAPFD;
    IAPAL = 0x39;
    set_IAPTRG_IAPGO;
    hircmap1 = IAPFD;

    TA=0XAA;
    TA=0X55;
    RCTRIM0 = hircmap0;
    TA=0XAA;
    TA=0X55;
    RCTRIM1 = hircmap1;
}

static void setup(void)
{
    // CPU
    set_CHPCON_IAPEN;
    init_cpu();

    // Relays
	init_relays();

	// UART
    UART0_Init();
    UART0_EnableInterrupt();

    // enable interrupts globally
    EA = 1;
}

static void loop(void)
{
    if (packet_received == 1)
    {
        set_relay_state_by_x();
    }
}

void main(void)
{
    setup();

    const char *ptr = hello;
    while (*ptr != '\0')
    {
        UART0_SendData(*ptr);
        while (UART0_GetFlag(UART0_TX_FLAG) == 0) {}
        UART0_ClearFlag(UART0_TX_FLAG);
        ++ptr;
    }

    for (;;)
    {
        loop();
    }
}