/******************************************************************************
 * Header file inclusions.
 ******************************************************************************/

#include "FreeRTOS.h"
#include "task.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include <stdint.h>

/******************************************************************************
 * Private macro definitions.
 ******************************************************************************/

#define mainLED_TASK_PRIORITY    1
#define mainLEDBAR_TASK_PRIORITY 1

#define BAR_DDR DDRD
#define BAR_PORT PORTD
#define LED PB0
#define LED_DDR DDRB
#define LED_PORT PORTB
#define BUTTON PC0
#define BUTTON_DDR DDRC
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC

#define BIT(i)   (tab[(i)/8] & ((i)-((i)%8)))
#define SET(byte,bit,val) (((byte) & ~(1 << (bit))) | ((val) << (bit)))


/******************************************************************************
 * Private function prototypes.
 ******************************************************************************/

static void vBlinkLed(void* pvParameters);

static void vLedBar(void* pvParameters);

volatile uint8_t tab[128] = {};
volatile uint16_t curr = 0;
volatile uint16_t next = 1000; 

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

    BAR_DDR |= 0b11111111;
    BAR_PORT = 0b00000001;
    LED_DDR |= 0b00000001;
    LED_PORT &= 0b00000000;
    BUTTON_DDR &= 0b11111110;
    BUTTON_PORT |= 0b00000001;
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);


    // Create task.
    xTaskHandle blink_handle;
    xTaskHandle ledbar_handle;

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
         vLedBar,
         "ledbar",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLEDBAR_TASK_PRIORITY,
         &ledbar_handle
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

    for ( ;; )
    {
        if(BIT(curr)) {
            LED_PORT |=_BV(LED);
        } else 
            LED_PORT &= ~_BV(LED);

        tab[next/8] = SET(tab[next/8],next%8,bit_is_clear(BUTTON_PIN, BUTTON)); 
        next = (next < 1023) ? next+1 : 0;
        curr = (curr < 1023) ? curr+1 : 0;

        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    
}


/**************************************************************************//**
 * \fn static void vSerial(void* pvParameters)
 *
 * \brief
 *
 * \param[in]   pvParameters
 ******************************************************************************/
static void vLedBar(void* pvParameters)
{
    while (1) {

        for (int i = 0; i < 7; i++) {
            BAR_PORT <<= 1;
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 7; i++ ) {
            BAR_PORT >>= 1;
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

    }
}
