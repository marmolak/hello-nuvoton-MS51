#include <uart.h>
__bit PRINTFG = 0, uart0_receive_flag = 0, uart1_receive_flag;
volatile unsigned char uart0_receive_data, uart1_receive_data;

/*
Why push of SFRS?

To accommodate more than 128 SFR in the 0x80 to 0xFF address space, SFR paging has been
implemented. By default, all SFR accesses target SFR Page 0. During device initialization, some SFR
located on SFR Page 1/2 may need to be accessed. The register SFRS is used to switch SFR
addressing page.
Note: In particular, since the interrupt is unpredictable, it is recommended to add following action in
each interrupt subroutine. Push SFRS in the stack at the beginning of the interrupt vector program and
pop SFRS when exiting the interrupt.
*/
#define PUSH_SFRS __asm__("push 0x91")
#define POP_SFRS __asm__("pop 0x91")

void Serial_ISR(void) __interrupt (4)
{
    PUSH_SFRS;
  
    if (RI)
    {
        uart0_receive_flag = 1;
        uart0_receive_data = SBUF;
        // Clear RI (Receive Interrupt).
        clr_SCON_RI;                                         
    }

    if (TI)
    {
        if (!PRINTFG)
        {
            TI = 0;
        }
    }

    POP_SFRS;
}	



void SerialPort1_ISR(void) __interrupt (15)
{
    PUSH_SFRS;
  
    if (RI_1)
    {
        uart1_receive_data = SBUF_1;
        uart1_receive_flag = 1;
        clr_SCON_1_RI_1;                             /* clear reception flag for next reception */
    }

    if (TI_1 == 1)
    {
        if (!PRINTFG)
        {
            clr_SCON_1_TI_1;                             /* if emission occur */
        }
    }

    POP_SFRS;
}



/*MS51 new version buadrate */
void UART_Open(unsigned long u32SysClock, unsigned char u8UARTPort,unsigned long u32Baudrate)
{
  switch(u8UARTPort)
  {
    case UART0_Timer1:
          SCON = 0x50;       //UART0 Mode1,REN=1,TI=1
          TMOD |= 0x20;      //Timer1 Mode1
          set_PCON_SMOD;          //UART0 Double Rate Enable
          set_CKCON_T1M;
          clr_T3CON_BRCK;          //Serial port 0 baud rate clock source = Timer1
          TH1 = 256 - (u32SysClock/16/u32Baudrate);
          set_TCON_TR1;
      break;
      
      case UART0_Timer3:
          SCON = 0x50;     //UART0 Mode1,REN=1,TI=1
          set_PCON_SMOD;        //UART0 Double Rate Enable
          T3CON &= 0xF8;   //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)
          set_T3CON_BRCK;        //UART0 baud rate clock source = Timer3
          RH3    = HIBYTE(65536 - (u32SysClock/16/u32Baudrate));  
          RL3    = LOBYTE(65536 - (u32SysClock/16/u32Baudrate));  
          set_T3CON_TR3;         //Trigger Timer3
      break;
      
      case UART1_Timer3:
          SCON_1 = 0x50;     //UART1 Mode1,REN_1=1,TI_1=1
          T3CON = 0x80;     //T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1), UART1 in MODE 1
          RH3    = HIBYTE(65536 - (u32SysClock/16/u32Baudrate));  
          RL3    = LOBYTE(65536 - (u32SysClock/16/u32Baudrate));     
          set_T3CON_TR3;             //Trigger Timer3                                                             
      break; 
  }
}

#if 0 
unsigned char Receive_Data(unsigned char UARTPort)
{
    UINT8 c;
    switch (UARTPort)
    {
      case UART0:
        while (!RI);
        c = SBUF;
        RI = 0;
      break;
      case UART1:
        while (!RI_1);
        c = SBUF_1;
        RI_1 = 0;
      break;
    }
    return (c);
}
#endif

void UART_Send_Data(UINT8 UARTPort, UINT8 c)
{
    switch (UARTPort)
    {
      case UART0:
        SBUF = c;
      break;
      case UART1:
        SBUF_1 = c;
      break;
    }
}

