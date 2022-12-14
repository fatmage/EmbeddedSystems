#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define LED_PORT PORTD
#define LED_DDR DDRD


int main() {

    int i = 0;
    LED_DDR |= 0b11111111;
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

    LED_PORT = 0b00000001;

    while (1) {

        for (int i = 0; i < 7; i++) {
            LED_PORT <<= 1;
            _delay_ms(100);
        }
        for (int i = 0; i < 7; i++ ) {
            LED_PORT >>= 1;
            _delay_ms(100);
        }

  }
}
