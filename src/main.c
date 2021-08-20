#include "MS51_16K.h"

#include "UARThw/UARThw.h"

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
#define RELAY4 P01
#define RELAY3 P04
#define RELAY2 P03
#define RELAY1 P05

#define _UNROLL_RELAY(RELAY_ID) RELAY_ID ## _PUSHPULL_MODE
#define SET_PIN_FOR(RELAY_ID) do {  \
    _UNROLL_RELAY(RELAY_ID); \
} while (0)


static const uint8_t __xdata hello[] = "Alternative firmware for lc-tech board with Nuvoton.\n";

#ifdef DEBUG
void printbits(const uint8_t byte)
{
	unsigned char out[10];

	uint8_t mask = 1;
	uint8_t tmp = 0;

	int i = 0;
	for (; i < 8; ++i) {
		tmp = (byte >> i) & mask;
		out[7 - i] = tmp + '0';
	}

	out[8] = '\n';
    out[9] = '\0';
           const char *ptr = out;
           while(*ptr != '\0')
    {
            UART0_SendData(*ptr);
            while (UART0_GetFlag(UART0_TX_FLAG) == 0) {}
            UART0_ClearFlag(UART0_TX_FLAG);
            ++ptr;
   }
}
#endif

/* UART interrupt routine */
#define packet_size 4
static volatile uint8_t packet_content[packet_size];
static volatile uint8_t packet_count = 0;
static volatile BIT packet_received = 0;

void UART0_ISR(void) __interrupt(4)
{
    // received byte
    if (RI == 1)
	{
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
}

static uint8_t validate_packet()
{
    if (packet_content[0] != 0xA0u)
    {
        return 0;
    }
    const uint8_t checksum = packet_content[0] + packet_content[1] + packet_content[2];

    return (checksum == packet_content[3]);
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
		case 2:
			RELAY2 = state ? 1 : 0;
			break;
		case 3:
			RELAY3 = state ? 1 : 0;
			break;
		case 4:
			RELAY4 = state ? 1 : 0;
			break;
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
    /* CPU */
    set_CHPCON_IAPEN;
    init_cpu();

    /* Relays */
    SET_PIN_FOR(RELAY1);
    SET_PIN_FOR(RELAY2);
    SET_PIN_FOR(RELAY3);
    SET_PIN_FOR(RELAY4);

	//clr_P05;

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