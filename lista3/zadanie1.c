#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define N_PS 0
#define N_B0  31
#define N_C1  33
#define N_CS1 35
#define N_D1  37
#define N_DS1 39
#define N_E1  41
#define N_F1  44
#define N_FS1 46
#define N_G1  49
#define N_GS1 52
#define N_A1  55
#define N_AS1 58
#define N_B1  62
#define N_C2  65
#define N_CS2 69
#define N_D2  73
#define N_DS2 78
#define N_E2  82
#define N_F2  87
#define N_FS2 93
#define N_G2  98
#define N_GS2 104
#define N_A2  110
#define N_AS2 117
#define N_B2  123
#define N_C3  131
#define N_CS3 139
#define N_D3  147
#define N_DS3 156
#define N_E3  165
#define N_F3  175
#define N_FS3 185
#define N_G3  196
#define N_GS3 208
#define N_A3  220
#define N_AS3 233
#define N_B3  247
#define N_C4  262
#define N_CS4 277
#define N_D4  294
#define N_DS4 311
#define N_E4  330
#define N_F4  349
#define N_FS4 370
#define N_G4  392
#define N_GS4 415
#define N_A4  440
#define N_AS4 466
#define N_B4  494
#define N_C5  523
#define N_CS5 554
#define N_D5  587
#define N_DS5 622
#define N_E5  659
#define N_F5  698
#define N_FS5 740
#define N_G5  784
#define N_GS5 831
#define N_A5  880
#define N_AS5 932
#define N_B5  988
#define N_C6  1047
#define N_CS6 1109
#define N_D6  1175
#define N_DS6 1245
#define N_E6  1319
#define N_F6  1397
#define N_FS6 1480
#define N_G6  1568
#define N_GS6 1661
#define N_A6  1760
#define N_AS6 1865
#define N_B6  1976
#define N_C7  2093
#define N_CS7 2217
#define N_D7  2349
#define N_DS7 2489
#define N_E7  2637
#define N_F7  2794
#define N_FS7 2960
#define N_G7  3136
#define N_GS7 3322
#define N_A7  3520
#define N_AS7 3729
#define N_B7  3951
#define N_C8  4186
#define N_CS8 4435
#define N_D8  4699
#define N_DS8 4978

#define BUZZ PB5
#define BUZZ_DDR DDRB
#define BUZZ_PORT PORTB

#define SONG_SIZE 65

#define PACK(note, time) ((((note) & 0xFFFE) << 3) | (((time)-1) & 0xF))
#define UNPACK_NOTE(packed) (((packed) >> 4) << 1)
#define UNPACK_TIME(packed) (((packed) & 0xF) + 1)

static const uint16_t utwor[] PROGMEM = {

  PACK(N_E5,4), PACK(N_E5,4), PACK(N_F5,4), PACK(N_G5,4), PACK(N_G5,4), PACK(N_F5,4), PACK(N_E5,4), PACK(N_D5,4),
  PACK(N_C5,4), PACK(N_C5,4), PACK(N_D5,4), PACK(N_E5,4), PACK(N_E5,4), PACK(N_PS,8), PACK(N_D5,8), PACK(N_D5,2),
  PACK(N_E5,4), PACK(N_E5,4), PACK(N_F5,4), PACK(N_G5,4), PACK(N_G5,4), PACK(N_F5,4), PACK(N_E5,4), PACK(N_D5,4),
  PACK(N_C5,4), PACK(N_C5,4), PACK(N_D5,4), PACK(N_E5,4), PACK(N_D5,4), PACK(N_PS,8), PACK(N_C5,8), PACK(N_C5,2),

  PACK(N_D5,4), PACK(N_D5,4), PACK(N_E5,4), PACK(N_C5,4), PACK(N_D5,4), PACK(N_E5,8), PACK(N_F5,8), PACK(N_E5,4), PACK(N_C5,4),
  PACK(N_D5,4), PACK(N_E5,8), PACK(N_F5,8), PACK(N_E5,4), PACK(N_D5,4), PACK(N_C5,4), PACK(N_D5,4), PACK(N_G5,2),

  PACK(N_E5,3), PACK(N_E5,8), PACK(N_F5,4), PACK(N_G5,4), PACK(N_G5,4), PACK(N_F5,4), PACK(N_E5,4), PACK(N_D5,4),
  PACK(N_C5,4), PACK(N_C5,4), PACK(N_D5,4), PACK(N_E5,4), PACK(N_D5,4), PACK(N_PS,8), PACK(N_C5,8), PACK(N_C5,2)
 
};


int main(){

  BUZZ_DDR |= _BV(BUZZ); 

  uint8_t tempo = 50;


  while(1) {

    for (int i = 0; i < SONG_SIZE; i++) {  

      uint16_t packed = pgm_read_word(&utwor[i]);

		  uint32_t note = UNPACK_NOTE(packed);
      uint32_t time = UNPACK_TIME(packed);


      uint32_t step = (note > 0) ? 1000000/note : 1;

      if (note == N_PS) {

        uint32_t j = 60*(1000000/tempo)/time;

        while(j--)
          _delay_us(1);

      } else {

        for (uint32_t k = 0; k < (60*(1000000/tempo))/(time*step); k++) {
          BUZZ_PORT |= _BV(BUZZ);

          uint32_t j = step/2;
          while (j--)
            _delay_us(1); 

          BUZZ_PORT &= ~_BV(BUZZ);

          j = step/2;
          while (j--)
            _delay_us(1); 
        }
      }
    }          
  }
}

