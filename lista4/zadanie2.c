// Zadanie 2, lista 4
// Michał Bednarz 324757

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <inttypes.h>


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // włącz odbiornik i nadajnik
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // ustaw format 8n1
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
  // czekaj aż transmiter gotowy
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
  // czekaj aż znak dostępny
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

// inicjalizacja ADC
void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
}

FILE uart_file;

void timer1_init(){
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 001  -- prescaler 1
  // ICR1  = 1023
  ICR1 = 1023;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}

static const uint16_t tab[] PROGMEM = {0, 1, 1, 2, 3, 5, 7, 11, 15, 22, 31, 45, 63, 90, 127, 181, 255, 362, 511, 724, 1023};

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  timer1_init();
  OCR1A = 0;

  while(1) {
    ADCSRA |= _BV(ADSC);
    while (!(ADCSRA & _BV(ADIF)));
    ADCSRA |= _BV(ADIF);
    uint16_t v = ADC;

    //v *= 3;
    //v /= 2;
    printf("Opór = %"PRIu16", wartość = %"PRIu16"\n\r", ADC,pgm_read_word(&tab[v/50 > 20 ? 0 : 20 - v/50]));
    OCR1A = pgm_read_word(&tab[v/50 > 20 ? 0 : 20 - v/50]);

  }
}
