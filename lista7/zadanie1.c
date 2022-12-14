// Michał Bednarz 324757
// Lista 7 zadanie 1
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "i2c.h"


#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

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

#define i2cCheck(code, msg) \
  if ((TWSR & 0xf8) != (code)) { \
    printf(msg " failed, status: %.2x\r\n", TWSR & 0xf8); \
    i2cReset(); \
    continue; \
}


const uint8_t eeprom_addr = 0xa0;


int main()
{
  uart_init();
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;

  i2cInit();

  uint16_t addr = 0;
  uint8_t  data = 0;
  char   cmd[20]={};

  while(1) {
    scanf("%s",cmd);
    printf("%s ",cmd);

    if (!strncmp(cmd, "write", 5)) {

      scanf("%"SCNx16,&addr);
      printf("%.3"PRIx16" ", addr);
      scanf("%"SCNx8, &data);
      printf("%"PRIx8"\r\n", data);

      i2cStart();
      i2cCheck(0x08, "I2C write start")
      i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
      i2cCheck(0x18, "I2C EEPROM write request")
      i2cSend(addr & 0xff);
      i2cCheck(0x28, "I2C EEPROM set address")
      i2cSend(data);
      i2cCheck(0x28, "I2C EEPROM data transmitted")
      i2cStop();
      i2cCheck(0xf8, "I2C stop")
      
    } else if (!strncmp(cmd,"read", 4)) {

      scanf("%"SCNx16, &addr);
      printf("%.03x\r\n",addr);

      i2cStart();
      i2cCheck(0x08, "I2C read start")
      i2cSend(eeprom_addr | ((addr & 0x100) >> 7));
      i2cCheck(0x18, "I2C EEPROM write request")
      i2cSend(addr & 0xff);
      i2cCheck(0x28, "I2C EEPROM set address")
      i2cStart();
      i2cCheck(0x10, "I2C second start")
      i2cSend(eeprom_addr | 0x1 | ((addr & 0x100) >> 7));
      i2cCheck(0x40, "I2C EEPROM read request")
      data = i2cReadNoAck();
      i2cCheck(0x58, "I2C EEPROM read")
      i2cStop();
      i2cCheck(0xf8, "I2C stop")

      printf("Value at memory address %.3"PRIx16" = %"PRIx8"\r\n", addr, data);

    } else {
        printf("\r\n");
    }
  }
}
