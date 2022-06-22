#include <byteswap.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"
#include "dis.h"

void dbg(CpuState *cpu, int total_instr_no, int *instr_to_line_no) {
  int *breakpoint = calloc(BREAKPOINT_NUMBER, sizeof(int));
  int bpt_ptr = 0;
  bool is_run;
  bool found_line_no;

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
      found_line_no = false;
      for (int i = 0; i < total_instr_no && !found_line_no; i++) {
        if (instr_to_line_no[i] == line_no - 1) {
          breakpoint[bpt_ptr] = i + 1; // breakpoint contains instrs
          found_line_no = true;
        }
      }
      if (!found_line_no) {
        printf("Such line not found.\n");
        continue;
      }
      printf("Breakpoint %d set at line %d.\n", bpt_ptr + 1, breakpoint[bpt_ptr]);
      bpt_ptr++;
    }
    if (input[0] == 'd' && input[1] == ' ') { // command delete
      int bpt_no = atoi(input + 2) - 1;       // TODO: change to start from 1
      if (bpt_no < 0 || bpt_no >= BREAKPOINT_NUMBER) {
        printf("Such breakpoint not found.\n");
        continue;
      }
      int line_no = breakpoint[bpt_no];
      if (line_no <= 0) {
        printf("Such breakpoint not found.\n");
        continue;
      }
      breakpoint[bpt_no] = 0;
      printf("Breakpoint %d removed from line %d.\n", bpt_no + 1, line_no);
    }
    if (input[0] == 'r') { // command run / continue
      is_run = true;
      if (terminate(cpu, breakpoint, bpt_ptr, false)) { // terminates
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
      if (terminate(cpu, breakpoint, bpt_ptr, true)) {
        break;
      }
    }
  }
  free(breakpoint);
}

// TODO: decide if prev instr, next instr or both be shown
bool terminate(CpuState *cpu, int *breakpoint, int bpt_ptr, bool step) {
  uint32_t *imem = cpu->mem;
  for (;;) {
    int curr = cpu->regs[REG_PC] >> 2;
    for (int i = 0; i < bpt_ptr; i++) {
      if (breakpoint[i] - 1 == curr && !step) {
        printf("Breakpoint %d at Line %d\n", i + 1, curr + 1);
        printf("Next Instruction at Line %d: \n", curr + 1);
        dis(stdout, curr, cpu->mem[curr]);
        printf("\n");
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
    entry(cpu, i);
    if (step) {
      printf("Previous Instruction at Line %d: \n", curr + 1);
      dis(stdout, curr, cpu->mem[curr]);
      curr = cpu->regs[REG_PC] >> 2;
      if (cpu->mem[curr]) {
        printf("Next Instruction at Line %d: \n", curr + 1);
        dis(stdout, curr, cpu->mem[curr]);
      } else {
        printf("No Next Instruction\n");
      }
      printf("\n");
      print_state(cpu);
      return false;
    }
  }
}
