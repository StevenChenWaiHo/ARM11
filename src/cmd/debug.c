#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "dbg.h"

int main(int argc, char **argv) {
  // assemble here -- not implemented

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input>", argv[0]);
    return EXIT_FAILURE;
  }
  char *input = argv[1];

  int innamelen = strlen(input) - 2;
  char in_wo_ext[innamelen + 1];
  strncpy(in_wo_ext, input, innamelen);
  in_wo_ext[innamelen] = '\0';

  char in_ext[2];
  strcpy(in_ext, input + innamelen);
  int is_s = strncmp(in_ext, ".s", 2);
  
  if (is_s == 0) {
    FILE *in = fopen(input, "r");
    if (!in) {
      fprintf(stderr, "Failed to open %s\n", input);
      return EXIT_FAILURE;
    }
    fseek(in, 0, SEEK_END);
    long lenb = ftell(in);
    fseek(in, 0, SEEK_SET);
    char *buff = malloc(lenb + 1);
    fread(buff, 1, lenb, in);
    buff[lenb] = '\0';
    
    FILE *out = fopen(in_wo_ext, "w");
    if (!out) {
      printf("Failed to create %s\n", in_wo_ext);
      return EXIT_FAILURE;
    }
    assemble(buff, input, out);
    fclose(out);
    fclose(in);
    free(buff);
    input = in_wo_ext;
  }

  FILE *in = fopen(input, "r");
  if (!in) {
    fprintf(stderr, "Failed to open %s\n", input);
    return EXIT_FAILURE;
  }
  fseek(in, 0, SEEK_END);
  long lenb = ftell(in);
  fseek(in, 0, SEEK_SET);
  // TODO: Handle errors

  uint32_t *mem = calloc(MEMORY_SIZE / 4, 4);
  // TODO: Handle alloc failure

  fread(mem, 4, lenb / 4, in);
  // TODO: Handle Read error

  uint32_t regs[17] = {0};
  memset(regs, 0, sizeof(regs));

  CpuState cpu;
  cpu.mem = mem;
  memmove(cpu.regs, regs, sizeof(regs));

  print_state(&cpu);

  dbg(&cpu);

  free(mem);

  return EXIT_SUCCESS;
}