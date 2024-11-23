#include "uart.h"

void handle_error(const char *error_message)
{
    // Print the error message over UART
    UART_Printf("Error: %s\n", error_message);

    // Could operate an LED to alert for an error aswell
}

void Error_Handler(void)
{
    while (1)
    {
        // Stay here for debugging
    }
}