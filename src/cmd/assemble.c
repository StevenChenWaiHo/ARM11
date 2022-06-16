#include <stdio.h>
#include <stdlib.h>

#include "asm.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *inname = argv[1];
  char *outname = argv[2];

  FILE *in = fopen(inname, "r");
  if (!in) {
    fprintf(stderr, "Failed to open %s\n", inname);
    return EXIT_FAILURE;
  }
  fseek(in, 0, SEEK_END);
  long len = ftell(in);
  fseek(in, 0, SEEK_SET);
  char *buff = malloc(len + 1);
  fread(buff, 1, len, in);
  buff[len] = '\0';

  FILE *out = fopen(outname, "w");
  if (!out) {
    printf("Failed to create %s\n", outname);
    return EXIT_FAILURE;
  }

  assemble(buff, inname, out);
  fclose(out);

  // printf("%s\n", buff);
  // Lexer l = lexer_new(buff, input);
  // Token t;
  // do {
  //   t = lexer_next(&l);
  //   printf("%s:%ld:%ld %20s `%.*s`\n", input, t.line + 1, t.column,
  //          token_kind_name(t.kind), (int)t.len, t.source);
  // } while (t.kind != TOKEN_EOF);

  return EXIT_SUCCESS;
}
