#include <byteswap.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cond.h"
#include "dbg.h"
#ifdef AEMU_TRACE
#include "dis.h"
#endif

typedef bool (*CpuCondFn)(CpuState *);

static CpuCondFn condfns[] = {
    [COND_EQ] = emu_cond_eq, [COND_NE] = emu_cond_ne, [COND_GE] = emu_cond_ge,
    [COND_LT] = emu_cond_lt, [COND_GT] = emu_cond_gt, [COND_LE] = emu_cond_le,
    [COND_AL] = emu_cond_al,
};

void dbg(CpuState *cpu) {
  int *breakpoint = calloc(BREAKPOINT_NUMBER, sizeof(int));
  int bpt_ptr = 0;
  bool is_run;

  // TODO : implement word commands such as 'break' or 'step'

  for (;;) {
    printf("> ");
    char input[20];
    fgets(input, 20, stdin);
    if (input[0] == 'b' && input[1] == ' ') { // command break
      if (bpt_ptr >= BREAKPOINT_NUMBER) {
        printf("Amount of breakpoints possible is limited to %d",
               BREAKPOINT_NUMBER);
        continue;
      }
      int line_no = atoi(input + 2);
      if (line_no <= 0 || line_no >= MEMORY_SIZE / 4 ||
          !cpu->mem[line_no - 1]) {
        printf("Such line not found.\n");
        continue;
      }
      breakpoint[bpt_ptr] = line_no;
      printf("Breakpoint %d set at line %d.\n", bpt_ptr + 1, line_no);
      bpt_ptr++;
    }
    if (input[0] == 'd' && input[1] == ' ') { // command delete
      int bpt_no = atoi(input + 2) - 1;
      if (bpt_no < 0 || bpt_no >= BREAKPOINT_NUMBER) {
        printf("Such breakpoint not found.\n");
        continue;
      }
      int line_no = breakpoint[bpt_no];
      if (line_no == 0) {
        printf("Such breakpoint not found.\n");
        continue;
      }
      breakpoint[bpt_no] = 0;
      printf("Breakpoint %d removed from line %d.\n", bpt_no + 1, line_no);
    }
    if (input[0] == 'r') { // command run / continue
      is_run = true;
      if (sequence(cpu, breakpoint, bpt_ptr, false)) { // terminates
        break;
      }
    }
    if (input[0] == 'q') { // command quit
      break;
    }
    if (input[0] == 's') { // command step
      if (!is_run) {
        printf("No program is running.\n");
        continue;
      }
      if (sequence(cpu, breakpoint, bpt_ptr, true)) {
        break;
      }
    }
  }
  free(breakpoint);
}

bool sequence(CpuState *cpu, int *breakpoint, int bpt_ptr, bool step) {
  uint32_t *imem = cpu->mem;
  for (;;) {
    int curr = cpu->regs[REG_PC] >> 2;
    for (int i = 0; i < bpt_ptr; i++) {
      if (breakpoint[i] - 1 == curr && !step) {
        printf("Breakpoint %d at line %d\n", i + 1, curr + 1);
        print_state(cpu);
        return false;
      }
    }
    Instr i = imem[curr];
#ifdef AEMU_TRACE
    dis(cpu->regs[REG_PC] / 4, i);
    printf("CSPR: %08x\n\n", cpu->regs[REG_CPSR]);
#endif
    if (!i) {
      printf("Terminating...\n");
      print_state(cpu);
      return true;
    } // HLT special case

    Instr condno = cond_mask(i);
    CpuCondFn cond = condfns[condno];
    if (!cond(cpu)) {
      cpu->regs[REG_PC] += 4;
      continue;
    }

    Instr type = type_mask(i);
    Instr type_mul = type_mul_mask(i);
    Instr type_mul2 = type_mul2_mask(i);
    switch (type) {
    case 0: // Data processing or multiply
      if (type_mul == 0 && type_mul2 == 9)
        emu_mul(cpu, i);
      else
        emu_dp(cpu, i);
      cpu->regs[REG_PC] += 4;
      break;
    case 1: // Single data transfer
      emu_sdt(cpu, i);
      cpu->regs[REG_PC] += 4;
      break;
    case 2: // Branch
      emu_br(cpu, i);
      break;
    default:
      fprintf(stderr, "Unknown type %x\n", type);
      exit(EXIT_FAILURE);
    }
    if (step) {
      printf("Line %d:\n", curr + 1);
      print_state(cpu);
      return false;
    }
  }
}
