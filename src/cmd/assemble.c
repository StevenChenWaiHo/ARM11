#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *input = argv[1];

  FILE *in = fopen(input, "r");
  if (!in) {
    fprintf(stderr, "Failed to open %s\n", input);
    return EXIT_FAILURE;
  }
  fseek(in, 0, SEEK_END);
  long len = ftell(in);
  fseek(in, 0, SEEK_SET);
  char *buff = malloc(len + 1);
  fread(buff, 1, len, in);
  buff[len] = '\0';
  printf("%s\n", buff);

  Lexer l = lexer_new(buff, input);
  Token t;
  do {
    t = lexer_next(&l);
    // fprintf(stderr, "%s:%ld:%ld: %s %s\n", input, t.line + 1, t.column,
    //         token_kind_name(t.kind), /*(int)t.len,*/ t.source);
    printf("%s:%ld:%ld %20s `%.*s`\n", input, t.line + 1, t.column,
           token_kind_name(t.kind), (int)t.len, t.source);
  } while (t.kind != TOKEN_EOF);

  return EXIT_SUCCESS;
}
