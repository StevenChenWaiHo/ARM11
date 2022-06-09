#ifndef AEMU_COND_H
#define AEMU_COND_H

typedef enum {
  COND_EQ,
  COND_NE,
  COND_GE = 10,
  COND_LT,
  COND_GT,
  COND_LE,
  COND_AL
} Cond;

#endif
