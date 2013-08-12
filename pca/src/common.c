#include "common.h"

unsigned int common_memory_left () {
  extern int __heap_start, *__brkval; 
  unsigned int v; 
  return (unsigned int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
