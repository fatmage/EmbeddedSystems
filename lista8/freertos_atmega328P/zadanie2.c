/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>


#include <stdio.h>
#include "uart.h"
#include "queue.h"

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY    1
#define mainSERIAL_TASK_PRIORITY 1

#define LED PB0
#define LED_DDR DDRB
#define LED_PORT PORTB

/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static void vBlinkLed(void* pvParameters);

static void vRead(void* pvParameters);

QueueHandle_t kolejka;

/******************************************************************************
 * Public function definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \fn int main(void)
 *
 * \brief Main function.
 *
 * \return
 ******************************************************************************/
int main(void)
{
    LED_DDR |= _BV(PB0);
    LED_PORT &= 0b00000000;
    // Create task.
    xTaskHandle read_handle;
    xTaskHandle blink_handle;

    kolejka = xQueueCreate(100, sizeof(unsigned int));
    xQueueReset(kolejka);
    if (kolejka == NULL)
        return 0;

    xTaskCreate
        (
         vBlinkLed,
         "blink",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_TASK_PRIORITY,
         &blink_handle
        );

    xTaskCreate
        (
         vRead,
         "read",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainSERIAL_TASK_PRIORITY,
         &read_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}

/**************************************************************************//**
 * \fn static vApplicationIdleHook(void)
 *
 * \brief
 ******************************************************************************/
void vApplicationIdleHook(void)
{

}

/******************************************************************************
 * Private function definitions.
 ******************************************************************************/

/**************************************************************************//**
 * \fn static void vBlinkLed(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vBlinkLed(void* pvParameters)
{
    while(1) {
        unsigned int duration = 0;
        if (xQueueReceive(kolejka, &duration, 0) == pdTRUE) {
            LED_PORT |= _BV(LED);
            vTaskDelay(duration / portTICK_PERIOD_MS);
            LED_PORT &= 0;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }


}


/**************************************************************************//**
 * \fn static void vSerial(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vRead(void* pvParameters)
{

    uart_init();
    stdin = stdout = stderr = &uart_file;

    unsigned int duration;

    for ( ;; )
    {
        printf("Podaj czas w ms: ");
        scanf("%u", &duration);
        printf("%u\r\n", duration);
        //printf("wpisales %u\r\n", duration);
        while (xQueueSend(kolejka, &duration, 0) == pdFALSE)
            ;
    }
}
