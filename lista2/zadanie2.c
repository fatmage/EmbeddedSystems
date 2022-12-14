#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define WORD_LED  7
#define LETTER_LED 6
#define LINE_LED   5
#define DOT_LED    4
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BUTTON 5
#define BUTTON_PIN PINB
#define BUTTON_PORT PORTB

#define DELAY_TIME 1
#define DOT_TIME    500
#define LINE_TIME   1500
#define LETTER_TIME 1500
#define WORD_TIME   3500

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
  // ustaw baudrate
  UBRR0 = UBRR_VALUE;
  // wyczyść rejestr UCSR0A
  UCSR0A = 0;
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

int main(void) {
  BUTTON_PORT |= (1 << BUTTON);
  LED_DDR |= _BV(DOT_LED);
  LED_DDR |= _BV(LETTER_LED);
  LED_DDR |= _BV(LINE_LED);
  LED_DDR |= _BV(WORD_LED);
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  uint16_t time_cnt = 0;
  char morse[33] = {' ',' ','e','t','i','a','n','m','s','u','r','w','d','k','g','o',
                    'h','v','f',' ','l',' ','p','j','b','x','c','y','z','q',' ',' ', ' '} ;
  uint8_t index = 1;


  printf("WITAM\r\n");

  while(!bit_is_clear(BUTTON_PIN, BUTTON))
    ;

  while(1) {

    while(bit_is_clear(BUTTON_PIN, BUTTON)) {
        _delay_ms(DELAY_TIME);
        time_cnt += DELAY_TIME;

        if (time_cnt > DOT_TIME) // zapal niebieskie
            LED_PORT |= _BV(DOT_LED);
 
        if (time_cnt > LINE_TIME) // zapal czerwone
            LED_PORT |= _BV(LINE_LED);
    }

    if (time_cnt > DOT_TIME)
        index <<= 1;
    
    if (time_cnt > LINE_TIME)
        index += 1;

    // dodaj odpowiedni znaczek
    // zgas lampki
    LED_PORT = 0;
    time_cnt = 0;
    uint8_t printed = 0;

    while(!bit_is_clear(BUTTON_PIN, BUTTON)) {
        _delay_ms(DELAY_TIME);
        time_cnt += DELAY_TIME;

        if (time_cnt > LETTER_TIME) {
            // zapal zielone
            LED_PORT |= _BV(LETTER_LED);
            if (!printed) {
                printf("%c", morse[index]);
                index = 1;
                printed = 1;
            }
        }
        if (time_cnt > WORD_TIME)
          LED_PORT |= _BV(WORD_LED);
    }
    if (time_cnt > WORD_TIME)
        printf(" ");

    LED_PORT = 0;
    time_cnt = 0;

  }

  return 0;
}