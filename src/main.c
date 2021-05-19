#include "MS51_16K.h"

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