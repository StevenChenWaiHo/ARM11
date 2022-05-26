#include <byteswap.h> // TODO: Gnu extension
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t Instr;

#include "mask.inc"

void decode_dp(Instr i, char *cond) {
  Instr opcodeno = dp_opcode_mask(i);
  char *opcode;
  switch (opcodeno) {
  case 0:
    opcode = "and";
    break;
  case 1:
    opcode = "eor";
    break;
  case 2:
    opcode = "sub";
    break;
  case 3:
    opcode = "rsb";
    break;
  case 4:
    opcode = "add";
    break;
  case 8:
    opcode = "tst";
    break;
  case 9:
    opcode = "teq";
    break;
  case 10:
    opcode = "cmp";
    break;
  case 12:
    opcode = "orr";
    break;
  case 13:
    opcode = "mov";
    break;
  default:
    printf("Unknown opcode %d\n", opcodeno);
    exit(-1);
  }

  Instr imm = dp_i_mask(i);
  Instr rn = dp_rn_mask(i);
  Instr rd = dp_rd_mask(i);
  Instr operand2 = dp_operand2_mask(i);

  printf("%s%s r%d, r%d, #%d\n", opcode, cond, rn, rd, operand2);
}

// TODO: Seperate files
void decode(int offset, Instr i) {
  printf("%03d: %08x\n", offset, i);
  Instr condno = cond_mask(i);
  char *cond;
  switch (condno) {
  case 0:
    cond = "eq";
    break;
  case 1:
    cond = "ne";
    break;
  case 10:
    cond = "ge";
    break;
  case 11:
    cond = "lt";
    break;
  case 12:
    cond = "gt";
    break;
  case 13:
    cond = "le";
    break;
  case 14:
    cond = "";
    break;
  default:
    fprintf(stderr, "Unknown cond %x\n", condno);
    exit(EXIT_FAILURE);
  }
  // printf(" cond: %s\n", cond);
  Instr type = type_mask(i);
  printf(" type: %d\n", type);
  switch (type) {
  case 0: // Data processing or multiply
          // TODO: Is this data processing or multiply?
    decode_dp(i, cond);
    break;
  case 1: // Single data transfer
  case 2: // Branch
    printf("TODO\n");
    exit(-1);
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input> <output>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char *input = argv[1];
  char *output = argv[2];

  FILE *in = fopen(input, "r");
  fseek(in, 0, SEEK_END);
  long len = ftell(in) / sizeof(Instr);
  fseek(in, 0, SEEK_SET);
  // TODO: Handle errors

  Instr *code = malloc(len);
  // TODO: Handle alloc failure

  fread(code, sizeof(Instr), len, in);
  // TODO: Handle error, TOCTOU

  for (int i = 0; i < len; i++) {
    printf("%d: %08x\n", i, code[i]);
  }

  for (int i = 0; i < len; i++) {
    decode(i, code[i]);
  }
}
