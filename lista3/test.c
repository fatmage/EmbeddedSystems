#include <avr/io.h>
#include <util/delay.h>

int main()
{
    DDRC = 0xff;           // Configure PORTC as output

    while(1)
    {
        PORTC = 0xff;        // Turn ON the Buzzer conneted to PORTC
        _delay_ms(1);      // Wait for some time
        PORTC = 0x00;        // Turn OFF the Buzzer connected to PORTC
        _delay_ms(1);      // Wait for some time
    }
    return 0;
}