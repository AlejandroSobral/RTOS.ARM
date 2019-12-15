#include "GPS.h"

#define UART1_PORT		(2)
#define UART1_TXD_PIN	(0)
#define UART1_RXD_PIN	(1)
#define UART1_BAUDRATE	(9600)



void UartMonitor (void);
void UartMonitor_Init (void);
int getdigit(int num, int n);
