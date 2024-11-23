#ifndef UART_H
#define UART_H

void UART_init(void);
void UART_Printf(const char *format, ...);

void UART_LoopbackTest(void);

#endif