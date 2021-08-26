/* UARThw.h */
#ifndef UART_H
#define UART_H

#include "MS51_16K.h"

#define UART0_RX_FLAG (1 << 0)
#define UART0_TX_FLAG (1 << 1)

void UART0_Init(void);
void UART0_SendData(uint8_t u8Data);
uint8_t UART0_GetData(void);
void UART0_EnableInterrupt(void);
void UART0_DisableInterrupt(void);
void UART0_ClearFlag(uint8_t u8Flag);
uint8_t UART0_GetFlag(uint8_t u8Flag);

#endif /* UART_H */