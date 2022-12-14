// Michał Bednarz, 324757
// Lista 6 zadanie 2
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define BUFFER_SIZE 100

uint8_t transmit_buffer[BUFFER_SIZE];
uint8_t receive_buffer[BUFFER_SIZE];
volatile int8_t tr_start = 0, tr_end = 1, rc_start = 0, rc_end = 1;


#define BUFFER_EMPTY(begin, end) ((((begin) + 1) == BUFFER_SIZE) && ((end) == 0) || (((begin) + 1) == (end)))
#define BUFFER_FULL(begin, end) (((begin) == (end)) || (((begin) == 0) && ((end) == BUFFER_SIZE)))

FILE uart_file;

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

// transmission

ISR(USART_UDRE_vect) {
  if (BUFFER_EMPTY(tr_start, tr_end))
    return;
  
  tr_start++;
  if (tr_start == BUFFER_SIZE) 
    tr_start = 0;
    

  UDR0 = transmit_buffer[tr_start];
}

int uart_transmit(char data, FILE *stream) {
  while (BUFFER_FULL(tr_start, tr_end))
    ;

  transmit_buffer[tr_end] = data;
  tr_end++;
  if (tr_end == BUFFER_SIZE) 
    tr_end = 0;

  if (!BUFFER_FULL(tr_start, tr_end))
    UCSR0B |= _BV(UDRIE0);

  return 0;
}


// receive
ISR(USART_RX_vect) {
  if (BUFFER_FULL(rc_start, rc_end))
    return;
  receive_buffer[rc_end] = UDR0;

  rc_end++;
  if (rc_end == BUFFER_SIZE) 
    rc_end = 0;
}

int uart_receive(FILE *stream) {
  while (BUFFER_EMPTY(rc_start, rc_end))
    ;

  rc_start++;
  if (rc_start == BUFFER_SIZE) 
    rc_start = 0;

  return receive_buffer[rc_start];
}


int main() {
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  sei();

  UCSR0B |= _BV(RXCIE0);

  while(1) {
    // transmitting
    printf("test test test test test test test test 123123 testujemy 12beb1ibib21b1ibe1i adawdwqibdia2292919___,da,awd\r\n");
    printf("What a fool you are, I'm a god! How can you kill a god? What a grand and intoxicating innocence! How could you be so naive? There is no escape, no recall or intervention can work in this place! Come! Lay down your weapons! It is not too late for my mercy...\r\n");
    printf("Why walk when you can ride?\r\n");

    // receiving
    char inputtest[100];
    uint16_t num;

    scanf("%s %"SCNu16, &inputtest, &num);
    printf("%s %"PRIu16"\r\n", inputtest, num);
  }


}

