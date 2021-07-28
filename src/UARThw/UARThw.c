#include "UARThw/UARThw.h"

void UART0_Init(void)
{
	/* baud rate Timer 3 */
	T3CON |= (1 << 5);
	/* timer 3 */
	/* Pre-Scalar */
	T3CON &= ~0x07;
	T3CON |= 0x00;
	/* Interrupt */
	EIE1 &= ~(1 << 1);
	RH3 = 0xff;
	RL3 = 0xf7;
	T3CON |= (1 << 3);
	PCON |= (1 << 7);
	/* GPIO */
	P06 = 1;
	P0M1 &= ~(1 << 6);
	P0M2 |= (1 << 6);
	P07 = 1;
	P0M1 &= ~(1 << 7);
	P0M2 &= ~(1 << 7);
	/* uart 0 */
	PCON &= ~(1 << 6);
	SM0 = 0;
	SM1 = 1;
	/* enable */
	RI = 0;
	REN = 1;
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