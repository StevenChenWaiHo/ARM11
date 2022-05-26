#include <byteswap.h> // TODO: Gnu extension
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "dis.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *input = argv[1];
  char *output = argv[2];

  FILE *in = fopen(input, "r");
  if (!in) {
    fprintf(stderr, "Failed to open %s\n", input);
    return EXIT_FAILURE;
  }
  fseek(in, 0, SEEK_END);
  long lenb = ftell(in);
  fseek(in, 0, SEEK_SET);
  // TODO: Handle errors

  Instr *code = malloc(lenb);
  // TODO: Handle alloc failure

  long len = lenb / sizeof(Instr);

  fread(code, sizeof(Instr), len, in);
  // TODO: Handle error, TOCTOU


  for (int i = 0; i < len; i++) {
    printf("%d: %08x\n", i, code[i]);
  }

  for (int i = 0; i < len; i++) {
    dis(i, code[i]);
  }

  free(code);
}
