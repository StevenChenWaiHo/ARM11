#ifndef AEMU_STR_H
#define AEMU_STR_H

// Not null terminated
typedef struct {
  char *ptr;
  size_t len;
} Str;

#endif