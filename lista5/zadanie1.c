// Michał Bednarz 324757
// Lista 5, zadanie 1


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define LED PD7
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON PB5
#define BUTTON_PIN PINB
#define BUTTON_PORT PORTB


void timer_init() {
  TCCR0B |= _BV(CS01) | _BV(CS00);
  TIMSK0 |= _BV(OCIE0A);
  OCR0A = 156;
  TCCR0A |= _BV(WGM02);
}

volatile uint8_t tab[128] = {};
uint16_t curr = 0;
uint16_t next = 1000; 


#define BIT(i)   (tab[(i)/8] & ((i)-((i)%8)))
#define SET(byte,bit,val) (((byte) & ~(1 << (bit))) | ((val) << (bit)))


ISR (TIMER0_COMPA_vect)
{
  if (BIT(curr)) {
    LED_PORT |=_BV(LED);
  } else {
    LED_PORT &= ~_BV(LED);
  }

  tab[next/8] = SET(tab[next/8],next%8,bit_is_clear(BUTTON_PIN, BUTTON)); 
  next = (next < 1023) ? next+1 : 0;
  curr = (curr < 1023) ? curr+1 : 0;
}

int main(){
  BUTTON_PORT |= _BV(BUTTON);
  LED_DDR |= _BV(LED);
  
  set_sleep_mode(SLEEP_MODE_IDLE);
  sei();
  timer_init();  //konfigurowanie portow

  while(1) {
    sleep_mode();
  }
}
