#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include "i2c.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>

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

  // zainicjalizuj I2C
  i2cInit();

  uint16_t addr = 0, length = 0;
  uint8_t data = 0;

  char line[31] = {};
  char *command = NULL;
  char *arg1 = NULL;
  char *arg2 = NULL;
  char 

  while(1) {
    printf("zaczynamy\r\n");
    scanf("%30[0-9a-zA-Z ]", line);
    getc(stdin);

    command = strtok(line, " ");
    arg1 = strtok(NULL, " ");
    arg2 = strtok(NULL, " ");

    if (!strncmp(command, "write", 5)) {
        if (arg1 == NULL && arg2 == NULL) {
            while(true) {

            }

        } else if (arg1 != NULL && arg2 != NULL) {
            sscanf(arg1, "%"SCNx16,&addr);
            sscanf(arg2, "%"SCNx8, &data);

            printf("Write %"PRIx8" at address %.3"PRIx16"\r\n", data, addr);
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
        }
      
    } else if (!strncmp(command,"read", 4)) {

        if (arg2 != NULL) {
            sscanf(arg1, "%"SCNx16,&addr);
            sscanf(arg2, "%"SCNx16, &length);
            // tu bedize readowanie lengthu
            printf(":%.2"PRIx16"%.4"PRIx16"00",length,addr);
            




        } else if (arg1 != NULL) {
            sscanf(arg1, "%"SCNx16, &addr);

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
        }

    } else {
        printf("\r\n");
    }

    memset(line,0,31);
  }
}
