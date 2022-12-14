// Michał Bednarz 324757
// Lista 5, zadanie 2

#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <util/delay.h>

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

FILE uart_file;

// inicjalizacja ADC
void adc_init(){
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  ADCSRA |= _BV(ADATE) | _BV(ADIE); //Auto Trigger
  ADCSRB |= _BV(ADTS1); //INT0
}

void interrupt_init() {
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01) | _BV(ISC00);
  sei();
}

volatile uint32_t resistance = 0;
volatile uint16_t v = 0;

ISR(INT0_vect){
}

ISR(ADC_vect){
  v = ADC;
  float result = (33000.0/(v+1))*1022;
  resistance = (uint32_t)result;
}

int main(){
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj ADC
  adc_init();
  // maska przerwan
  interrupt_init();

  while(1) {
    _delay_ms(1000);
    cli();
    printf("ADC: %"PRIu16", opór %"PRIu32"\r\n", v,  resistance);
    sei();
  }
}

