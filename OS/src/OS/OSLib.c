//Memory.c

#include "OS/OSLib.h"

void OS_memcpy(void* destination, const void* source, unsigned int num)
{
  unsigned int i;

  //transfer byte by byte
  char* src = (char *)source;
  char* dest = (char *)destination;

  for(i = 0; i < num; i++)
    {
      dest[i] = src[i];

    }

}