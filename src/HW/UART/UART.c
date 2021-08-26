#include "HW/UART/UART.h"

void UART0_Init(void)
{
    P06_QUASI_MODE;    //Setting UART pin as Quasi mode for transmit
	P07_QUASI_MODE;    //Setting UART pin as Quasi mode for transmit  
	SCON = 0x50;            /*UART0 Mode1,REN=1,TI=1*/
	set_PCON_SMOD;          /*UART0 Double Rate Enable*/
 	T3CON &= 0xF8;           /*T3PS2=0,T3PS1=0,T3PS0=0(Prescale=1)*/
 	set_T3CON_BRCK;          /*UART0 baud rate clock source = Timer3*/
	RH3    = 0xFF;   /* HIBYTE(65536 - 13)*/
	RL3    = 0xF3;   /* LOBYTE(65536 - 13); */
	set_T3CON_TR3;          /*Trigger Timer3*/
	ES=1;
}

void UART0_SendData(uint8_t u8Data)
{
	SBUF = u8Data;
}

uint8_t UART0_GetData(void)
{
	return SBUF;
}

void UART0_EnableInterrupt(void)
{
	ES = 1;
}

void UART0_DisableInterrupt(void)
{
	ES = 0;
}

void UART0_ClearFlag(uint8_t u8Flag)
{
	SCON &= ~(u8Flag);
}

uint8_t UART0_GetFlag(uint8_t u8Flag)
{
	if (SCON & (u8Flag)) {
		return 1;
	} else {
		return 0;
	}
}