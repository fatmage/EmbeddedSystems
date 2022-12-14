#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB


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

void dot() {
        LED_PORT |= _BV(LED);
        _delay_ms(333);
        LED_PORT &= ~_BV(LED);
        _delay_ms(333);
}

void line() {
        LED_PORT |= _BV(LED);
        _delay_ms(1000);
        LED_PORT &= ~_BV(LED);
        _delay_ms(333);
}

FILE uart_file;

int main()
{
LED_DDR |= _BV(LED);
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  printf("Hello world!\r\n");
  while(1) {
    char a[100] = {};
    scanf("%s", a);

    for (int i = 0; i < strlen(a); i++) {
        printf("%c", a[i]);
        switch (a[i]) {
            case 'C':            // kreska kropka kreska kropka
            case 'c':
            line();
            dot();
            line();
            dot();
            break;
            case 'B':            // kreska kropka kropka kropka
            case 'b':
            line();
            dot();
            dot();
            dot();
            break;
            case 'L':            // kropka kreska kropka kropka
            case 'l':
            dot();
            line();
            dot();
            dot();
            break;
            case 'V':            // kropka kropka kropka kreska
            case 'v':
            dot();
            dot();
            dot();
            line();
            break;

            case 'P':            // kropka kreska kreska kropka
            case 'p':
            dot();
            case 'G':            // kreska kreska kropka
            case 'g':
            line();
            line();
            dot();
            break;

            case 'Q':            // kreska kreska kropka kreska
            case 'q':
            line();
            case 'K':            // kreska kropka kreska
            case 'k':
            line();
            dot();
            line();
            break;

            case 'Z':            // kreska kreska kropka kropka
            case 'z':
            line();
            case 'D':            // kreska kropka kropka
            case 'd':
            line();
            dot();
            dot();
            break;

            case 'Y':            // kreska kropka kreska kreska
            case 'y':
            line();
            case 'W':            // kropka kreska kreska
            case 'w':
            dot();
            line();
            line();
            break;

            case 'X':            // kreska kropka kropka kreska
            case 'x':
            line();
            case 'U':            // kropka kropka kreska
            case 'u':
            dot();
            case 'A':            // kropka kreska
            case 'a':
            dot();
            line();
            break;

            case 'F':            // kropka kropka kreska kropka
            case 'f':
            dot();
            case 'R':            // kropka kreska kropka
            case 'r':
            dot();
            case 'N':            // kreska kropka
            case 'n':
            line();
            dot();
            break;

            case 'H':            // kropka kropka kropka kropka
            case 'h':
            dot();
            case 'S':            // kropka kropka kropka
            case 's':
            dot();
            case 'I':            // kropka kropka
            case 'i':
            dot();
            case 'E':            // kropka
            case 'e':
            dot();
            break;

            case 'J':            // kropka kreska kreska kreska
            case 'j':
            dot(); 
            case 'O':            // kreska kreska kreska
            case 'o':
            line();
            case 'M':            // kreska kreska
            case 'm':
            line();
            case 'T':            // kreska
            case 't':
            line();
            break;

            default:             // wait

            _delay_ms(2333);
        };
        _delay_ms(666);

    }
    printf("\r\n");
    


  }
}

