#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emu.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input>", argv[0]);
    return EXIT_FAILURE;
  }
  char *input = argv[1];

  FILE *in = fopen(input, "r");
  if (!in) {
    fprintf(stderr, "Failed to open %s\n", input);
    return EXIT_FAILURE;
  }
  fseek(in, 0, SEEK_END);
  long lenb = ftell(in);
  fseek(in, 0, SEEK_SET);
  // TODO: Handle errors

  uint32_t *mem = calloc(16384, 4);
  // TODO: Handle alloc failure

  fread(mem, 4, lenb / 4, in);
  // TODO: Handle Read error

  uint32_t regs[17] = {0};
  memset(regs, 0, sizeof(regs));

  CpuState cpu;
  cpu.mem = mem;
  memmove(cpu.regs, regs, sizeof(regs));

  emu(&cpu);
  print_state(&cpu);

  return EXIT_SUCCESS;
}
