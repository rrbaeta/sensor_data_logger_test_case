#include "stm32g0xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdlib.h>
#include <time.h>

#include "clock.h"
#include "error.h"
#include "uart.h"
#include "circular_buffer.h"
#include "sensor.h"

#define TIME_BETWEEN_READS 1000
#define TIME_BETWEEN_RETRIEVALS 5000

// variables
CircularBuffer tempBuffer = {0};
SemaphoreHandle_t bufferMutex;

// function prototypes
void log_temperature_task(void *pvParameters);
void data_retrieve_task(void *pvParameters);

int main(void)
{
    // Hardware initialization
    HAL_Init();
    SystemClock_Config();
    UART_init();
    bufferMutex = xSemaphoreCreateMutex();

    // Create tasks
    xTaskCreate(log_temperature_task, "TempTask", 128, NULL, 1, NULL);
    xTaskCreate(data_retrieve_task, "DataTask", 4 * 128, NULL, 1, NULL);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();

    while (1)
    {
        // Should never reach here
    }

    return 0;
}

void log_temperature_task(void *pvParameters)
{
    while (1)
    {
        // Log every 1 second
        vTaskDelay(pdMS_TO_TICKS(TIME_BETWEEN_READS)); // also free processing space

        int temperature = read_temperature();

        // Lock the buffer
        xSemaphoreTake(bufferMutex, portMAX_DELAY);
        buffer_add(&tempBuffer, temperature);

        if (tempBuffer.head >= BUFFER_SIZE) // should never happen
        {
            handle_error("Buffer overflow");
        }

        xSemaphoreGive(bufferMutex);
    }
}

void data_retrieve_task(void *pvParameters)
{
    while (1)
    {
        // Retrieve every 5 seconds
        vTaskDelay(pdMS_TO_TICKS(TIME_BETWEEN_RETRIEVALS)); // also free processing space

        xSemaphoreTake(bufferMutex, portMAX_DELAY);

        if (tempBuffer.count == 0)
        {
            handle_error("Attempted to retrieve from an empty buffer");
        }
        else
        {
            // UART_Printf("Retrieving logged temperatures:\n");
            for (int i = 0; i < tempBuffer.count; i++)
            {
                uint8_t index = (tempBuffer.tail + i) % BUFFER_SIZE;
                UART_Printf("Temperature[%d]: %i°C\n", i, tempBuffer.buffer[index]);
            }
        }

        xSemaphoreGive(bufferMutex);
    }
}