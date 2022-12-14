#include <avr/io.h>
#include <util/delay.h>

#define LED PD7
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON PB5
#define BUTTON_PIN PINB
#define BUTTON_PORT PORTB


#define DELAY_TIME 1 /* miliseconds */


int main(void) {

  uint8_t current_index = 0;
  uint8_t next_index = 1;
  uint16_t buffer[256] = {};
  buffer[0] = 1000;

  BUTTON_PORT |= (1 << BUTTON);
  LED_DDR |= (1 << LED);

  while (!bit_is_clear(BUTTON_PIN, BUTTON)) {
  }
  
  while (1) {

    while (bit_is_clear(BUTTON_PIN, BUTTON)) {
      _delay_ms(DELAY_TIME);
      buffer[next_index]    += DELAY_TIME;
      buffer[current_index] -= DELAY_TIME;

      if (!buffer[current_index]) {
        LED_PORT ^= (1 << LED); // flip the bit
        current_index++;
      }
    }
    
    next_index++;

    while (!bit_is_clear(BUTTON_PIN, BUTTON)) {
      _delay_ms(DELAY_TIME);
      buffer[next_index]    += DELAY_TIME;
      buffer[current_index] -= DELAY_TIME;

      if (!buffer[current_index]) {
        LED_PORT ^= (1 << LED); // flip the bit
        current_index++;
      }
    }

    next_index++;
  }
  

  return 0;
}