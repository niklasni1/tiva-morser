#include <stdint.h>

#ifndef __MORSE_H
#define __MORSE_H

enum STATE {
  SPACING,
  TICKING,
  ENDING
};

struct morser {
  int8_t tick_count;
  enum STATE state;
  uint8_t char_pos;
  uint8_t char_length;
  uint8_t string_pos;
  char *string;
}; 

void init_morser(struct morser *m, char *str);

enum OUTPUT {
  LOW,
  HIGH,
  END
};

enum OUTPUT tick(struct morser *m); 

#endif
