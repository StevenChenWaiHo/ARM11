#include <assert.h>
#include <byteswap.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"
#include "dis.h"
#include "linenoise.h"

static bool parse_reg_name(char *reg_name, Reg *reg) { // adapted from asm.c
  for (size_t i = 0; i < strlen(reg_name); i++) {      // ignores case
    reg_name[i] = tolower(reg_name[i]);
  }
  if (strncmp(reg_name, "r0\0", 3) == 0) {
    *reg = REG_0;
    return true;
  } else if (strncmp(reg_name, "r1\0", 3) == 0) {
    *reg = REG_1;
    return true;
  } else if (strncmp(reg_name, "r2\0", 3) == 0) {
    *reg = REG_2;
    return true;
  } else if (strncmp(reg_name, "r3\0", 3) == 0) {
    *reg = REG_3;
    return true;
  } else if (strncmp(reg_name, "r4\0", 3) == 0) {
    *reg = REG_4;
    return true;
  } else if (strncmp(reg_name, "r5\0", 3) == 0) {
    *reg = REG_5;
    return true;
  } else if (strncmp(reg_name, "r6\0", 3) == 0) {
    *reg = REG_6;
    return true;
  } else if (strncmp(reg_name, "r7\0", 3) == 0) {
    *reg = REG_7;
    return true;
  } else if (strncmp(reg_name, "r8\0", 3) == 0) {
    *reg = REG_8;
    return true;
  } else if (strncmp(reg_name, "r9\0", 3) == 0) {
    *reg = REG_9;
    return true;
  } else if (strncmp(reg_name, "r10\0", 4) == 0) {
    *reg = REG_10;
    return true;
  } else if (strncmp(reg_name, "r11\0", 4) == 0) {
    *reg = REG_11;
    return true;
  } else if (strncmp(reg_name, "r12\0", 4) == 0) {
    *reg = REG_12;
    return true;
  } else if (strncmp(reg_name, "pc\0", 3) == 0) {
    *reg = REG_PC;
    return true;
  } else if (strncmp(reg_name, "cpsr\0", 5) == 0) {
    *reg = REG_CPSR;
    return true;
  } else
    return false;
}

static void print_line(CpuState *cpu, int curr, bool next) {
  char seq[9];
  next ? strcpy(seq, "Next") : strcpy(seq, "Previous");
  printf("%s Instruction at Line %d: \n", seq, curr + 1);
  dis(stdout, curr, cpu->mem[curr]);
}

// TODO: decide if prev instr, next instr or both be shown
static bool run(CpuState *cpu, int *breakpoint, int bpt_ptr, bool step) {
  uint32_t *imem = cpu->mem;
  for (;;) {
    int curr = cpu->regs[REG_PC] >> 2;
    for (int i = 0; i < bpt_ptr; i++) {
      if (breakpoint[i] - 1 == curr && !step) {
        printf("Breakpoint %d at Line %d\n", i + 1, curr + 1);
        print_line(cpu, curr, true);
        // print_state(cpu);
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
      // print_state(cpu);
      return true;
    } // HLT special case

    Instr condno = cond_mask(i);
    CpuCondFn cond = condfns[condno];
    if (!cond(cpu)) {
      cpu->regs[REG_PC] += 4;
      continue;
    }
    select_func(cpu, i);
    if (step) {
      print_line(cpu, curr, false);
      curr = cpu->regs[REG_PC] >> 2;
      if (cpu->mem[curr]) {
        print_line(cpu, curr, true);
      } else {
        printf("No Next Instruction\n");
      }
      // print_state(cpu);
      return false;
    }
  }
}

static CpuState *cpu_reset(uint32_t *mem) {
  CpuState *cpu = malloc(sizeof(CpuState));
  cpu->mem = mem;
  uint32_t regs[17] = {0};
  memset(regs, 0, sizeof(regs));
  memmove(cpu->regs, regs, sizeof(regs));
  return cpu;
}

void dbg(uint32_t *mem, int total_instr_no, int *instr_to_line_no) {
  int *breakpoint = calloc(BREAKPOINT_NUMBER, sizeof(int));
  int bpt_ptr = 0;
  bool is_run = false;
  bool found_line_no;
  CpuState *cpu = cpu_reset(mem);

  for (;;) {
    fflush(stdout); // Keep things moving, for DBG tests

    char *input = linenoise("> ");
    if (!input) {
      free(cpu);
      goto breek;
    }
    linenoiseHistoryAdd(input);
    // defer linenoiseFree(line);  // TODO

    if (input[0] == 'b' && input[1] == ' ') { // command break
      if (bpt_ptr >= BREAKPOINT_NUMBER) {
        printf("Amount of breakpoints possible is limited to %d",
               BREAKPOINT_NUMBER);
        goto cintinue;
      }
      int line_no = atoi(input + 2); // input: start from 1
      found_line_no = false;
      int instr_no;
      for (int i = 0; i < total_instr_no && !found_line_no; i++) {
        if (instr_to_line_no[i] == line_no - 1) {
          instr_no = i + 1;
          breakpoint[bpt_ptr] =
              instr_to_line_no[i] + 1; // breakpoint contains instrs
          found_line_no = true;
        }
      }
      if (!found_line_no) {
        printf("Such line not found.\n");
        goto cintinue;
      }
      printf("Breakpoint %d set at line %d (instruction %d).\n", bpt_ptr + 1,
             breakpoint[bpt_ptr], instr_no);
      bpt_ptr++;
    }
    if (input[0] == 'd' && input[1] == ' ') { // command delete
      int bpt_no = atoi(input + 2) - 1;
      if (bpt_no < 0 || bpt_no >= BREAKPOINT_NUMBER) {
        printf("Such breakpoint not found.\n");
        goto cintinue;
      }
      int line_no = breakpoint[bpt_no];
      if (line_no <= 0) {
        printf("Such breakpoint not found.\n");
        goto cintinue;
      }
      breakpoint[bpt_no] = 0;
      printf("Breakpoint %d removed from line %d.\n", bpt_no + 1, line_no);
    }
    if (input[0] == 'r' && input[1] == '\0') { // command run / continue
      is_run = true;
      if (run(cpu, breakpoint, bpt_ptr, false)) { // terminates
        is_run = false;
        free(cpu);
        cpu = cpu_reset(mem);
        goto cintinue;
      }
    }
    if (input[0] == 'q' && input[1] == '\0') { // command quit
      free(cpu);
      goto breek;
    }
    if (input[0] == 's' && input[1] == '\0') { // command step
      if (!is_run) {
        printf("No program is running.\n");
        goto cintinue;
      }
      if (run(cpu, breakpoint, bpt_ptr, true)) {
        is_run = false;
        free(cpu);
        cpu = cpu_reset(mem);
        goto cintinue;
      }
    }
    if (input[0] == 'p' && input[1] == ' ') { // command print register
      char reg_name[5];
      strncpy(reg_name, input + 2, 5);
      Reg reg;
      bool valid = parse_reg_name(reg_name, &reg);
      if (valid) {
        if (reg == REG_PC) {
          printf("%d\n", cpu->regs[reg] + 8);
        } else {
          printf("%d\n", cpu->regs[reg]);
        }
      } else {
        printf("Register input invalid.\n");
      }
    }
    if (input[0] == 'l' && input[1] == ' ' && input[2] == 'n' &&
        input[3] == '\0') { // command print next line
      if (!is_run) {
        printf("No program is running.\n");
        goto cintinue;
      }
      int curr = cpu->regs[REG_PC] >> 2;
      print_line(cpu, curr, true);
    }
    // Awful, deplorable hack do make up for not having continue
  cintinue:
    linenoiseFree(input);
    continue;
  breek:
    linenoiseFree(input);
    break;
  }
  fflush(stdout);
  free(breakpoint);
}
