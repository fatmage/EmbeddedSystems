#include <avr/io.h>
#include <util/delay.h>

#define LED 7
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON 5
#define BUTTON_PIN PINB
#define BUTTON_PORT PORTB



#define DEBOUNCE_TIME 1 /* miliseconds */

uint8_t debounce(void) {
  if (bit_is_clear(BUTTON_PIN, BUTTON)) { /* button is pressed now */
    _delay_ms(DEBOUNCE_TIME);
    if (bit_is_clear(BUTTON_PIN, BUTTON)) { /* still pressed */
      return 1;
    }
  }
  return 0;
}

int main(void) {

  uint8_t current_index = 0;
  uint8_t next_index = 1;
  int32_t buffer[256] = {};
  buffer[0] = 1000;

  BUTTON_PORT |= (1 << BUTTON);
  LED_DDR |= (1 << LED);
  LED_DDR |= (1 << 5); 


  while (!debounce()) {
  }
  


  while (1) {

    while (debounce()) {
      buffer[next_index]    += DEBOUNCE_TIME;
      buffer[current_index] -= DEBOUNCE_TIME;
      if (buffer[current_index] <= 0) {
        LED_PORT  ^= (1 << 5);
        LED_PORT ^= (1 << LED); // flip the bit
        current_index++;
      }
    }
    
    next_index++;

    while (!debounce()) {
      buffer[next_index]    += DEBOUNCE_TIME;
      buffer[current_index] -= DEBOUNCE_TIME;
      if (buffer[current_index] <= 0) {
        LED_PORT ^= (1 << 5);
        LED_PORT ^= (1 << LED); // flip the bit
        current_index++;
      }
    }

    next_index++;
  }
  

  return 0;
}