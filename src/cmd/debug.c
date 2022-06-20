#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "dbg.h"
#include "dis.h"

// TODO: share with assemble in asm.c if possible, move asm helpers if not
// returns an array with instr_no as index, corresponding line_no as element
static int *assemble_debug(char *src, char *filename, FILE *out,
                           int *total_line_no, int *total_instr_no) {
  Assembler a;
  a.lexer = lexer_new(src, filename);
  a.out = out;
  a.n_instrs = asm_pass1(&a);
  a.consts = NULL;
  a.n_consts = 0;

#ifdef AEMU_TRACE
  Token t;
  do {
    t = asm_advance(&a);
    int tkind = t.kind;
    printf("%ld:%ld %20s `%.*s`\n", t.line + 1, t.column,
           token_kind_name(tkind), (int)t.source.len, t.source.ptr);
  } while (t.kind != TOKEN_EOF);

  printf("\n{\n");
  sym_tab_foreach(&a.symtab, st_dbg);
  printf("}\n");
#endif

  int *instr_to_line_no =
      (int *)malloc(a.n_instrs * sizeof(int)); // create the mapping array

  if (!instr_to_line_no) {
    printf("The instruction_no to line_no array cannot be created");
    return 0;
  }

  asm_reset(&a);

  Instr instr_no = 0;
  Instr line_no = 0;

  for (;;) {
    Token t = asm_advance(&a);
    switch (t.kind) {
    case TOKEN_IDENT:
      asm_instr(&a, &t, instr_no);
      instr_to_line_no[instr_no] = line_no;
      instr_no++;
      line_no++;
      break;
    case TOKEN_LABEL:
      asm_expect(&a, TOKEN_NEWLINE);
      line_no++;
      break;
    case TOKEN_EOF:
      goto done;
    case TOKEN_NEWLINE:
      line_no++;
      break;
    default:
      asm_err(&a, &t, "Expected identifier or label, but got `%.*s` (%s)",
              (int)t.source.len, t.source.ptr, token_kind_name(t.kind));
    }
  }
done:
  assert(instr_no == a.n_instrs);
  for (size_t i = 0; i < a.n_consts; i++)
    asm_write_word(&a, a.consts[i]);

  asm_free(a);

  *total_instr_no = instr_no;
  *total_line_no = line_no;

  return instr_to_line_no;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input>", argv[0]);
    return EXIT_FAILURE;
  }
  char *input = argv[1];

  int innamelen = strlen(input) - 2;
  char in_wo_ext[innamelen + 1];
  strncpy(in_wo_ext, input, innamelen);
  in_wo_ext[innamelen] = '\0';

  // removed support for binary

  // TODO: share with main in assemble.c
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
  int total_line_no, total_instr_no;
  int *instr_to_line_no =
      assemble_debug(buff, input, out, &total_line_no, &total_instr_no);
  fclose(out);
  fclose(in);
  free(buff);

  input = in_wo_ext;

  in = fopen(input, "r");
  if (!in) {
    fprintf(stderr, "Failed to open %s\n", input);
    return EXIT_FAILURE;
  }
  fseek(in, 0, SEEK_END);
  lenb = ftell(in);
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

  dbg(&cpu, total_instr_no, instr_to_line_no);

  fclose(in);
  free(instr_to_line_no);
  free(mem);

  return EXIT_SUCCESS;
}
