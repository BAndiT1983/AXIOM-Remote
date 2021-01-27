#ifndef UART_H
#define UART_H

#include <stdint.h>

// NOTE: Using UART2 only at the moment
// NOTE: Removed destructor, as it's not required
class UART
{
    void Initialize(uint32_t baudRate);

  public:
    explicit UART(uint32_t baudRate);

    void SendText(const char* message) const;
};

#endif // UART_H
