#include "MS51_16K.h"

/*
Why here? According to sdcc docs:
    If you have multiple source files in your project, interrupt service routines can be present in any of them, but a
    prototype of the isr MUST be present or included in the file that contains the function main.

    And uart.h contains isr routines.
*/
#include "uart.h"

static void setup()
{
    ALL_GPIO_PUSHPULL_MODE;
}

static void loop(void)
{
    P1 = 0xff;
    Timer0_Delay(16000000,200,1000);
    P1=0x00;
    Timer0_Delay(16000000,200,1000);
}

void main(void)
{
    setup();

    for (;;)
    {
        loop();
    }
}