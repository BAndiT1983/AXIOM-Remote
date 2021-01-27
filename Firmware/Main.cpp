// Configuration
#include "Configuration/PIC32Config.h"

#include <xc.h>
#include <sys/attribs.h>

// Periphery
#include "Periphery/UART/UART.h"
#include "Periphery/USB/USBCDCDevice.h"

int main()
{
    __builtin_disable_interrupts();

    PRISS = 0x76543210;

    UART uart(115200);
    uart.SendText("Test");

    USBCDCDevice cdcDevice;
    cdcDevice.Initialize();

    __builtin_enable_interrupts();

    while (1)
    {
        cdcDevice.Process();
    }

    return 0;
}
