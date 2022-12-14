// Zadanie 1, lista 4
// Michał Bednarz 324757

#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

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

void timer1_init()
{
  // ustaw tryb licznika
  // WGM1  = 0000 -- normal
  // CS1   = 001  -- prescaler 1
  TCCR1B = _BV(CS10);
}
FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // zainicjalizuj licznik
  timer1_init();

  volatile int8_t  a8, b8,  r8;
  volatile int16_t a16, b16, r16;
  volatile int32_t a32, b32, r32;
  volatile int64_t a64, b64, r64;
  volatile float   af, bf, rf;
  
  uint16_t biast = TCNT1;
  uint16_t bias = TCNT1 - biast;

  while(1) {

    printf("uint8_t\r\n");
    volatile uint16_t add = TCNT1;
    r8 = a8+b8;
    add = TCNT1 - add;
    volatile uint16_t mul = TCNT1;
    r8 = a8*b8;
    mul = TCNT1 - mul;
    volatile uint16_t div = TCNT1;
    r8 = a8/b8;
    div = TCNT1 - div;
    printf("Dodawanie = %"PRIu16", mnozenie = %"PRIu16", dzielenie = %"PRIu16"\r\n", add-bias, mul-bias, div-bias);


    printf("uint16_t\r\n");
    add = TCNT1;
    r16 = a16+b16;
    add = TCNT1 - add;
    mul = TCNT1;
    r16 = a16*b16;
    mul = TCNT1 - mul;
    div = TCNT1;
    r16 = a16/b16;
    div = TCNT1 - div;
    printf("Dodawanie = %"PRIu16", mnozenie = %"PRIu16", dzielenie = %"PRIu16"\r\n", add-bias, mul-bias, div-bias);


    printf("uint32_t\r\n");
    add = TCNT1;
    r32 = a32+b32;
    add = TCNT1 - add;
    mul = TCNT1;
    r32 = a32*b32;
    mul = TCNT1 - mul;
    div = TCNT1;
    r32 = a32/b32;
    div = TCNT1 - div;
    printf("Dodawanie = %"PRIu16", mnozenie = %"PRIu16", dzielenie = %"PRIu16"\r\n", add-bias, mul-bias, div-bias);

    printf("uint64_t\r\n");
    add = TCNT1;
    r64 = a64+b64;
    add = TCNT1 - add;
    mul = TCNT1;
    r64 = a64*b64;
    mul = TCNT1 - mul;
    div = TCNT1;
    r64 = a64/b64;
    div = TCNT1 - div;
    printf("Dodawanie = %"PRIu16", mnozenie = %"PRIu16", dzielenie = %"PRIu16"\r\n", add-bias, mul-bias, div-bias);

    printf("float\r\n");
    add = TCNT1;
    rf = af+bf;
    add = TCNT1 - add;
    mul = TCNT1;
    rf = af*bf;
    mul = TCNT1 - mul;
    div = TCNT1;
    rf = af/bf;
    div = TCNT1 - div;
    printf("Dodawanie = %"PRIu16", mnozenie = %"PRIu16", dzielenie = %"PRIu16"\r\n", add-bias, mul-bias, div-bias);

    getchar();
    printf("\033[2J\033[H");
  }
}

