#include "MS51_16K.h"

static void setup()
{
      ALL_GPIO_QUASI_MODE;
}

void main(void)
{
    setup();

    while(1)
    {
        P1 = 0xff;
        Timer0_Delay(16000000,200,1000);;
        P1=0x00;
        Timer0_Delay(16000000,200,1000);;
    }
}