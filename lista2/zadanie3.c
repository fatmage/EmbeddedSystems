#include <avr/io.h>
#include <util/delay.h>


#define LED_DDR DDRD
#define LED_PORT PORTD

#define BTN_RESET PB0
#define BTN_NEXT  PB1
#define BTN_PREV  PB2
#define BTN_PIN PINC
#define BTN_PORT PORTC


#define DEBOUNCE_TIME 20 /* miliseconds */

uint8_t debounce(uint8_t button) {
  if (bit_is_clear(BTN_PIN, button)) { /* button is pressed now */
    _delay_ms(DEBOUNCE_TIME);
    if (bit_is_clear(BTN_PIN, button)) { /* still pressed */
      return 1;
    }
  }

  return 0;
}

int main(void) {

  BTN_PORT |= _BV(BTN_RESET) | _BV(BTN_PREV) | _BV(BTN_NEXT);
  LED_DDR |= 0b11111111;

  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

  uint8_t cnt = 0, rst_p = 0, nxt_p = 0, prv_p = 0;

  while (1) {

    if (debounce(BTN_RESET)) {
        if (rst_p == 0) {
            LED_PORT = 0b00000000;
            cnt = 0;
            rst_p = 1;
        }
    } else {
        rst_p = 0;
    }
    if (debounce(BTN_NEXT)) {
        if (nxt_p == 0) {
            LED_PORT = ((++cnt) ^ (cnt >> 1));
            nxt_p = 1;
        }
    } else {
        nxt_p = 0;
    }
    if (debounce(BTN_PREV)) {
        // prev gray code
        if (prv_p == 0) {
            LED_PORT = ((--cnt) ^ (cnt >> 1));
            prv_p = 1;
        }
    } else {
        prv_p = 0;
    }
  }
  

  return 0;
}