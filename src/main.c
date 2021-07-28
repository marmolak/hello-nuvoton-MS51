#include "MS51_16K.h"

#include "UARThw/UARThw.h"

static const uint8_t bufa[] = "hello"; 
static const uint8_t bufa_size = sizeof(bufa) / sizeof(bufa[0]);
static volatile uint8_t bufa_count = 0;
static volatile uint8_t bufa_match = 0;

// Message on output
static const char *const hello = "Hello world!\n";

void UART0_ISR(void) __interrupt(4)
{
    // received byte
    if (RI == 1)
    {
        const uint8_t tmp = UART0_GetData();
        if (bufa[bufa_count] != tmp)
        {
            RI = 0;
            bufa_count = 0;
            return;
        }

        ++bufa_count;
        if (bufa_count == (bufa_size - 1))
        {
            bufa_count = 0;
            bufa_match = 1;
            RI = 0;
            EA = 0;
            return;
        }

        RI = 0;
    }

}

static void setup(void)
{
    UART0_Init();
    UART0_EnableInterrupt();

    // enable interrupts globally
    EA = 1;
}


static void loop(void)
{
   const char *ptr = hello;

   if (bufa_match != 1)
   {
       return;
   }

   // ok... match found
   UART0_SendData(0x14);
   while (UART0_GetFlag(UART0_TX_FLAG) == 0) {}
   UART0_ClearFlag(UART0_TX_FLAG);

   while (*ptr != '\0')
   {
        UART0_SendData(*ptr);
        while (UART0_GetFlag(UART0_TX_FLAG) == 0) {}
        UART0_ClearFlag(UART0_TX_FLAG);
        ++ptr;   
    }
    bufa_match = 0;
    EA = 1;
}

void main(void)
{
    setup();

    for (;;)
    {
        loop();
    }
}