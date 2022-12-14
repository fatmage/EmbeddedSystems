#include <util/delay.h>
#include <avr/io.h>

#define LED_DDR DDRD
#define LED_PORT PORTD

int main()
{
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

  const char nums[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x02, 0xF8, 0x80, 0x10 };
  LED_DDR = 0b11111111;

  while(1) {
    for (int i = 0; i < 10; i++) {
        LED_PORT = nums[i];
        _delay_ms(1000);
    }
  }
}
