#ifndef UART_H
#define UART_H

int uart_open(char *port);
void uart_close();
int uart_tx(int len,unsigned char *data);
int uart_rx(int len,unsigned char *data,int timeout_ms);
#endif // UART_H
